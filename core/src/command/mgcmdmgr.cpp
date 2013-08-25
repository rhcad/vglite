// mgcmdmgr.cpp: 实现命令管理器类
// Copyright (c) 2004-2012, Zhang Yungui
// License: LGPL, https://github.com/rhcad/touchvg

#include "mgcmdmgr.h"
#include "mgcmdselect.h"
#include <mggrid.h>
#include "tradecmd.h"

MgCommand* mgCreateCoreCommand(const char* name);
float mgDisplayMmToModel(float mm, GiGraphics* gs);
float mgDisplayMmToModel(float mm, const MgMotion* sender);
void registerTransformCmd();

typedef std::map<std::string, MgCommand* (*)()> Factories;
static Factories    _factories;

MgCmdManager* mgCreateCmdManager()
{
    return new MgCmdManagerImpl();
}

void mgRegisterCommand(const char* name, MgCommand* (*factory)())
{
    if (!factory) {
        _factories.erase(name);
    }
    else {
        _factories[name] = factory;
    }
}

MgCmdManagerImpl::MgCmdManagerImpl()
{
    registerTransformCmd();
}

MgCmdManagerImpl::~MgCmdManagerImpl()
{
    unloadCommands();
}

void MgCmdManagerImpl::unloadCommands()
{
    for (CMDS::iterator it = _cmds.begin(); it != _cmds.end(); ++it)
        it->second->release();
    _cmds.clear();
    _cmdname = "";
    TradeCmd::onUnloadCmds();
}

const char* MgCmdManagerImpl::getCommandName()
{
    MgCommand* cmd = getCommand();
    return cmd ? cmd->getName() : "";
}

MgCommand* MgCmdManagerImpl::getCommand()
{
    CMDS::iterator it = _cmds.find(_cmdname);
    return (it != _cmds.end()) ? it->second : NULL;
}

MgCommand* MgCmdManagerImpl::findCommand(const char* name)
{
    CMDS::iterator it = _cmds.find(name);
    
    if (it == _cmds.end() && *name)
    {
        MgCommand* cmd = NULL;
        Factories::iterator itf = _factories.find(name);
        
        if (itf != _factories.end() && itf->second) {
            cmd = (itf->second)();
        }
        if (!cmd) {
            cmd = mgCreateCoreCommand(name);
        }
        if (cmd) {
            _cmds[name] = cmd;
            it = _cmds.find(name);
        }
    }
    
    return it != _cmds.end() ? it->second : NULL;
}

bool MgCmdManagerImpl::setCommand(const MgMotion* sender, const char* name)
{
    if (strcmp(name, "@draw") == 0) {
        name = _drawcmd.empty() ? "splines" : _drawcmd.c_str();
    }
    MgCommand* cmd = findCommand(name);
    
    if (strcmp(name, "erase") == 0) {
        MgSelection *sel = getSelection(sender->view);
        if (sel && sel->deleteSelection(sender))
            return false;
        
        if (!cmd && sel) {      // no erase command
            cmd = findCommand(_cmdname.c_str());
            if (cmd && cmd->isDrawingCommand()) {
                MgCommand* selcmd = findCommand(MgCmdSelect::Name());
                selcmd->initialize(sender);
                bool ret = sel->deleteSelection(sender);
                
                int actions[] = { 0 };
                sender->view->showContextActions(0, actions, Box2d(), NULL);
                
                return ret;
            }
        }
    }
    
    cancel(sender);
    
    bool ret = false;
    std::string oldname(_cmdname);
    
    if (cmd) {
        _cmdname = cmd->getName();
        
        ret = cmd->initialize(sender);
        if (!ret) {
            _cmdname = oldname;
        }
        else if (cmd->isDrawingCommand()) {
            _drawcmd = _cmdname;
        }
    }
    else {
        if (strcmp(name, "erasewnd") == 0) {
            eraseWnd(sender);
        }
        else {
            _cmdname = "select";
            cmd = findCommand(_cmdname.c_str());
            cmd->initialize(sender);
        }
    }
    
    if (MgBaseShape::minTol().equalPoint() < 1e-5) {
        MgBaseShape::minTol().setEqualPoint(sender->view->xform()->displayToModel(1.f, true));
    }
    if (oldname != _cmdname) {
        sender->view->commandChanged();
    }
    
    return ret;
}

bool MgCmdManagerImpl::cancel(const MgMotion* sender)
{
    clearSnap();
    
    int actions[] = { 0 };
    sender->view->showContextActions(0, actions, Box2d(), NULL);

    CMDS::iterator it = _cmds.find(_cmdname);
    if (it != _cmds.end()) {
        return it->second->cancel(sender);
    }
    return false;
}

int MgCmdManagerImpl::getSelection(MgView* view, int count, MgShape** shapes, bool forChange)
{
    if (_cmdname == MgCmdSelect::Name() && view) {
        MgCmdSelect* sel = (MgCmdSelect*)getCommand();
        return sel ? sel->getSelection(view, count, shapes, forChange) : 0;
    }
    return 0;
}

bool MgCmdManagerImpl::dynamicChangeEnded(MgView* view, bool apply)
{
    bool changed = false;
    if (_cmdname == MgCmdSelect::Name() && view) {
        MgCmdSelect* sel = (MgCmdSelect*)getCommand();
        changed = sel && sel->dynamicChangeEnded(view, apply);
    }
    return changed;
}

MgSelection* MgCmdManagerImpl::getSelection(MgView*)
{
    return (MgCmdSelect*)findCommand(MgCmdSelect::Name());
}

MgActionDispatcher* MgCmdManagerImpl::getActionDispatcher()
{
    return this;
}

void MgCmdManagerImpl::doContextAction(const MgMotion* sender, int action)
{
    doAction(sender, action);
}

MgSnap* MgCmdManagerImpl::getSnap()
{
    return this;
}

class SnapItem {
public:
    Point2d pt;             // 捕捉到的坐标
    Point2d base;           // 参考线基准点、原始点
    float   dist;           // 捕捉距离
    int     type;           // 特征点类型
    int     shapeid;        // 捕捉到的图形
    int     handleIndex;    // 捕捉到图形上的控制点序号
    int     handleIndexSrc; // 待确定位置的源图形上的控制点序号，与handleIndex点匹配
    
    SnapItem() {}
    SnapItem(const Point2d& _pt, const Point2d& _base, float _dist, int _type = 0,
        int _shapeid = 0, int _handleIndex = -1, int _handleIndexSrc = -1)
        : pt(_pt), base(_base), dist(_dist), type(_type), shapeid(_shapeid)
        , handleIndex(_handleIndex), handleIndexSrc(_handleIndexSrc) {}
};

static int snapHV(const Point2d& basePt, Point2d& newPt, SnapItem arr[3])
{
    int ret = 0;
    float diff;
    
    diff = arr[1].dist - fabsf(newPt.x - basePt.x);
    if (diff > _MGZERO || (diff > - _MGZERO
                           && fabsf(newPt.y - basePt.y) < fabsf(newPt.y - arr[1].base.y))) {
        arr[1].dist = fabsf(newPt.x - basePt.x);
        arr[1].base = basePt;
        newPt.x = basePt.x;
        arr[1].pt = newPt;
        arr[1].type = kMgSnapSameX;
        ret |= 1;
    }
    diff = arr[2].dist - fabsf(newPt.y - basePt.y);
    if (diff > _MGZERO || (diff > - _MGZERO
                           && fabsf(newPt.x - basePt.x) < fabsf(newPt.x - arr[2].base.x))) {
        arr[2].dist = fabsf(newPt.y - basePt.y);
        arr[2].base = basePt;
        newPt.y = basePt.y;
        arr[2].pt = newPt;
        arr[2].type = kMgSnapSameY;
        ret |= 2;
    }
    
    return ret;
}

static bool skipShape(const int* ignoreids, const MgShape* sp)
{
    bool skip = false;
    for (int t = 0; ignoreids[t] != 0 && !skip; t++) {
        skip = (ignoreids[t] == sp->getID());           // 跳过当前图形
    }
    return skip;
}

static bool snapHandle(const MgMotion* sender, const MgShape* shape, int ignoreHandle,
                       const MgShape* sp, SnapItem& arr0, Point2d* matchpt)
{
    int n = sp->shapec()->getHandleCount();
    bool curve = sp->shapec()->isKindOf(MgSplines::Type());
    bool dragHandle = (!shape || shape->getID() == 0 || sender->pointM
                       == shape->shapec()->getHandlePoint(ignoreHandle));
    bool handleFound = false;
    
    for (int i = 0; i < n; i++) {                    // 循环每一个控制点
        if (curve && ((i > 0 && i + 1 < n) || sp->shapec()->isClosed())) {
            continue;                                   // 对于开放曲线只捕捉端点
        }
        Point2d pnt(sp->shapec()->getHandlePoint(i));   // 已有图形的一个顶点
        int handleType = sp->shapec()->getHandleType(i);
        
        float dist = pnt.distanceTo(sender->pointM);    // 触点与顶点匹配
        if (arr0.dist > dist && handleType < kMgHandleOutside) {
            arr0.dist = dist;
            arr0.base = sender->pointM;
            arr0.pt = pnt;
            arr0.type = kMgSnapPoint + handleType - kMgHandleVertext;
            arr0.shapeid = sp->getID();
            arr0.handleIndex = i;
            arr0.handleIndexSrc = dragHandle ? ignoreHandle : -1;
            handleFound = true;
        }
        
        int d = matchpt ? shape->shapec()->getHandleCount() - 1 : -1;
        for (; d >= 0; d--) {                           // 整体移动图形，顶点匹配
            if (d == ignoreHandle || shape->shapec()->isHandleFixed(d))
                continue;
            Point2d ptd (shape->shapec()->getHandlePoint(d));   // 当前图形的顶点
            
            dist = pnt.distanceTo(ptd);                 // 当前图形与其他图形顶点匹配
            if (arr0.dist > dist - _MGZERO && handleType < kMgHandleOutside) {
                arr0.dist = dist;
                arr0.base = ptd;  // 新的移动起点为当前图形的一个顶点
                arr0.pt = pnt;    // 将从ptd移到其他图形顶点pnt
                arr0.type = kMgSnapPoint + handleType - kMgHandleVertext;
                arr0.shapeid = sp->getID();
                arr0.handleIndex = i;
                arr0.handleIndexSrc = d;
                handleFound = true;
                
                // 因为对当前图形先从startM移到pointM，然后再从pointM移到matchpt
                *matchpt = sender->pointM + (pnt - ptd); // 所以最后差量为(pnt-ptd)
            }
        }
    }
    
    return handleFound;
}

static void snapNear(const MgMotion* sender, const MgShape* shape, int ignoreHandle,
                     const MgShape* sp, SnapItem& arr0, Point2d* matchpt)
{
    Point2d nearpt, ptd;
    float dist;
    float minDist = arr0.dist;
    float tolNear = mgDisplayMmToModel(0.7f, sender);
    int d = matchpt ? shape->shapec()->getHandleCount() : 0;
    
    for (; d >= 0; d--) {
        if (d == 0) {
            ptd = sender->pointM;
        }
        else {
            if (d - 1 == ignoreHandle || shape->shapec()->isHandleFixed(d - 1))
                continue;
            ptd = shape->shapec()->getHandlePoint(d - 1);
        }
        dist = sp->shapec()->hitTest2(ptd, tolNear, nearpt);
        
        if (minDist > dist) {
            minDist = dist;
            arr0.base = ptd;  // 新的移动起点为当前图形的一个顶点
            arr0.pt = nearpt; // 将从ptd移到其他图形顶点pnt
            arr0.type = kMgSnapNearPt;
            arr0.shapeid = sp->getID();
            arr0.handleIndex = -1;
            arr0.handleIndexSrc = d - 1;
            if (d > 0) {    // 因为对当前图形先从startM移到pointM，然后再从pointM移到matchpt
                *matchpt = sender->pointM + (nearpt - ptd);
            }
        }
    }
    if (arr0.dist > minDist) {
        arr0.dist = minDist + mgDisplayMmToModel(4.f, sender);
    }
}

static void snapGrid(const MgMotion* sender, const MgShape* shape, int ignoreHandle,
                     const MgShape* sp, SnapItem arr[3], Point2d* matchpt)
{
    if (sp->shapec()->isKindOf(MgGrid::Type())) {
        Point2d newPt (sender->pointM);
        const MgGrid* grid = (const MgGrid*)(sp->shapec());
        
        int type = grid->snap(newPt, arr[1].dist, arr[2].dist);
        if (type & 1) {
            arr[1].base = newPt;
            arr[1].pt = newPt;
            arr[1].type = kMgSnapGridX;
        }
        if (type & 2) {
            arr[2].base = newPt;
            arr[2].pt = newPt;
            arr[2].type = kMgSnapGridY;
        }
        
        int d = matchpt ? shape->shapec()->getHandleCount() - 1 : -1;
        for (; d >= 0; d--) {
            if (d == ignoreHandle || shape->shapec()->isHandleFixed(d))
                continue;
            
            Point2d ptd (shape->shapec()->getHandlePoint(d));
            float distx = mgMin(arr[0].dist, arr[1].dist);
            float disty = mgMin(arr[0].dist, arr[2].dist);
            
            newPt = ptd;
            type = grid->snap(newPt, distx, disty);
            float dist = newPt.distanceTo(ptd);
            
            if ((type & 3) == 3 && arr[0].dist > dist - _MGZERO) {
                arr[0].dist = dist;
                arr[0].base = ptd;
                arr[0].pt = newPt;
                arr[0].type = kMgSnapGrid;
                arr[0].shapeid = sp->getID();
                arr[0].handleIndex = -1;
                arr[0].handleIndexSrc = d;
                
                // 因为对当前图形先从startM移到pointM，然后再从pointM移到matchpt
                *matchpt = sender->pointM + (newPt - ptd); // 所以最后差量为(pnt-ptd)
            }
        }
    }
}

static void snapPoints(const MgMotion* sender, const MgShape* shape, int ignoreHandle,
                       const int* ignoreids, SnapItem arr[3], Point2d* matchpt)
{
    Box2d snapbox(sender->pointM, 2 * arr[0].dist, 0);      // 捕捉容差框
    GiTransform* xf = sender->view->xform();
    void* it = NULL;
    
    for (const MgShape* sp = sender->view->shapes()->getFirstShape(it);
         sp; sp = sender->view->shapes()->getNextShape(it)) {
        
        if (skipShape(ignoreids, sp)) {
            continue;
        }
        Box2d extent(sp->shapec()->getExtent());
        if (extent.width() < xf->displayToModel(2, true)
            && extent.height() < xf->displayToModel(2, true)) { // 图形太小就跳过
            continue;
        }
        if (extent.isIntersect(snapbox)) {                      // 不是整体拖动图形
            if (!snapHandle(sender, shape, ignoreHandle, sp, arr[0], matchpt)) {
                snapNear(sender, shape, ignoreHandle, sp, arr[0], matchpt);
            }
            snapGrid(sender, shape, ignoreHandle, sp, arr, matchpt);
        }
    }
    sender->view->shapes()->freeIterator(it);
}

// hotHandle: 绘新图时，起始步骤为-1，后续步骤>0；拖动一个或多个整体图形时为-1，拖动顶点时>=0
Point2d MgCmdManagerImpl::snapPoint(const MgMotion* sender, const MgShape* shape,
                                    int hotHandle, int ignoreHandle,
                                    const int* ignoreids)
{
    int ignoreids_tmp[2] = { shape ? shape->getID() : 0, 0 };
    if (!ignoreids) ignoreids = ignoreids_tmp;
    
    if (!shape || hotHandle >= shape->shapec()->getHandleCount()) {
        hotHandle = -1;         // 对hotHandle进行越界检查
    }
    _ptSnap = sender->pointM;   // 默认结果为当前触点位置
    
    SnapItem arr[3] = {         // 设置捕捉容差和捕捉初值
        SnapItem(_ptSnap, _ptSnap, mgDisplayMmToModel(3.f, sender)), // XY点捕捉
        SnapItem(_ptSnap, _ptSnap, mgDisplayMmToModel(2.f, sender)), // X分量捕捉，竖直线
        SnapItem(_ptSnap, _ptSnap, mgDisplayMmToModel(2.f, sender)), // Y分量捕捉，水平线
    };
    
    if (shape && shape->getID() == 0 && hotHandle > 0               // 绘图命令中的临时图形
        && !shape->shapec()->isKindOf(MgBaseRect::Type())) {        // 不是矩形或椭圆
        Point2d pt (sender->pointM);
        snapHV(shape->shapec()->getPoint(hotHandle - 1), pt, arr);  // 和上一个点对齐
    }
    
    Point2d pnt(-1e10f, -1e10f);                    // 当前图形的某一个顶点匹配到其他顶点pnt
    bool matchpt = (shape && shape->getID() != 0    // 拖动整个图形
                    && (hotHandle < 0 || (ignoreHandle >= 0 && ignoreHandle != hotHandle)));
    
    snapPoints(sender, shape, ignoreHandle, ignoreids,
               arr, matchpt ? &pnt : NULL);         // 在所有图形中捕捉
    checkResult(arr);
    
    return matchpt && pnt.x > -1e8f ? pnt : _ptSnap;    // 顶点匹配优先于用触点捕捉结果
}

void MgCmdManagerImpl::checkResult(SnapItem* arr)
{
    if (arr[0].type > 0) {                          // X和Y方向同时捕捉到一个点
        _ptSnap = arr[0].pt;                        // 结果点
        _snapBase[0] = arr[0].base;                 // 原始点
        _snapType[0] = arr[0].type;
        _snapShapeId = arr[0].shapeid;
        _snapHandle = arr[0].handleIndex;
        _snapHandleSrc = arr[0].handleIndexSrc;
    }
    else {
        _snapShapeId = 0;
        _snapHandle = -1;
        _snapHandleSrc = -1;
        
        _snapType[0] = arr[1].type;                 // 竖直方向捕捉到一个点
        if (arr[1].type > 0) {
            _ptSnap.x = arr[1].pt.x;
            _snapBase[0] = arr[1].base;
        }
        _snapType[1] = arr[2].type;                 // 水平方向捕捉到一个点
        if (arr[2].type > 0) {
            _ptSnap.y = arr[2].pt.y;
            _snapBase[1] = arr[2].base;
        }
    }
}

int MgCmdManagerImpl::getSnappedType()
{
    if (_snapType[0] >= kMgSnapPoint)
        return _snapType[0];
    return (_snapType[0] == kMgSnapGridX && _snapType[1] == kMgSnapGridY) ? kMgSnapPoint : 0;
}

int MgCmdManagerImpl::getSnappedPoint(Point2d& fromPt, Point2d& toPt)
{
    fromPt = _snapBase[0];
    toPt = _ptSnap;
    return getSnappedType();
}

bool MgCmdManagerImpl::getSnappedHandle(int& shapeid, int& handleIndex, int& handleIndexSrc)
{
    shapeid = _snapShapeId;
    handleIndex = _snapHandle;
    handleIndexSrc = _snapHandleSrc;
    return shapeid != 0;
}

void MgCmdManagerImpl::clearSnap()
{
    _snapType[0] = 0;
    _snapType[1] = 0;
}

bool MgCmdManagerImpl::drawSnap(const MgMotion* sender, GiGraphics* gs)
{
    bool ret = false;
    
    if (sender->dragging() || !sender->view->useFinger()) {
        if (_snapType[0] >= kMgSnapPoint) {
            bool isnear = (_snapType[0] >= kMgSnapNearPt);
            GiContext ctx(-2, GiColor(0, 255, 0, 200), kGiLineDash, GiColor(0, 255, 0, 64));
            ret = gs->drawEllipse(&ctx, _ptSnap, mgDisplayMmToModel(isnear ? 3.f : 6.f, gs));
        }
        else {
            GiContext ctx(0, GiColor(0, 255, 0, 200), kGiLineDash, GiColor(0, 255, 0, 64));
            GiContext ctxcross(-2, GiColor(0, 255, 0, 200));
            
            if (_snapType[0] > 0) {
                if (_snapBase[0] == _ptSnap) {
                    if (_snapType[0] == kMgSnapGridX) {
                        Vector2d vec(0, mgDisplayMmToModel(15.f, gs));
                        ret = gs->drawLine(&ctxcross, _ptSnap - vec, _ptSnap + vec);
                        gs->drawEllipse(&ctx, _snapBase[0], mgDisplayMmToModel(4.f, gs));
                    }
                }
                else {  // kMgSnapSameX
                    ret = gs->drawLine(&ctx, _snapBase[0], _ptSnap);
                    gs->drawEllipse(&ctx, _snapBase[0], mgDisplayMmToModel(2.5f, gs));
                }
            }
            if (_snapType[1] > 0) {
                if (_snapBase[1] == _ptSnap) {
                    if (_snapType[1] == kMgSnapGridY) {
                        Vector2d vec(mgDisplayMmToModel(15.f, gs), 0);
                        ret = gs->drawLine(&ctxcross, _ptSnap - vec, _ptSnap + vec);
                        gs->drawEllipse(&ctx, _snapBase[1], mgDisplayMmToModel(4.f, gs));
                    }
                }
                else {  // kMgSnapSameY
                    ret = gs->drawLine(&ctx, _snapBase[1], _ptSnap);
                    gs->drawEllipse(&ctx, _snapBase[1], mgDisplayMmToModel(2.5f, gs));
                }
            }
        }
    }
    
    return ret;
}

void MgCmdManagerImpl::eraseWnd(const MgMotion* sender)
{
    Box2d snap(sender->view->xform()->getWndRectW() * sender->view->xform()->worldToModel());
    std::vector<int> delIds;
    void *it = NULL;
    MgShapes* s = sender->view->shapes();
    
    for (MgShape* shape = s->getFirstShape(it); shape; shape = s->getNextShape(it)) {
        if (shape->shape()->hitTestBox(snap)) {
            delIds.push_back(shape->getID());
        }
    }
    s->freeIterator(it);
    
    if (!delIds.empty()
        && sender->view->shapeWillDeleted(s->findShape(delIds.front()))) {
        MgShapesLock locker(MgShapesLock::Remove, sender->view->doc());
        
        for (std::vector<int>::iterator i = delIds.begin(); i != delIds.end(); ++i) {
            MgShape* shape = s->findShape(*i);
            if (shape && sender->view->removeShape(shape)) {
                shape->release();
            }
        }
        sender->view->regenAll();
    }
}
