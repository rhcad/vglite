//! \file gicoreview.cpp
//! \brief 实现内核视图类 GiCoreView
// Copyright (c) 2012-2013, https://github.com/rhcad/touchvg

#include "gicoreview.h"
#include "GcShapeDoc.h"
#include "GcMagnifierView.h"
#include "mgcmd.h"
#include <RandomShape.h>
#include <mgjsonstorage.h>
#include <tradecmd.h>
#include <mgshapetype.h>

#define CALL_VIEW(func) if (curview) curview->func
#define CALL_VIEW2(func, v) curview ? curview->func : v
MgCmdManager* mgCreateCmdManager();

class GiCoreViewImpl : public MgView
{
public:
    GcShapeDoc*     _doc;
    MgCmdManager*   _cmds;
    GcBaseView*     curview;
    long            refcount;
    MgMotion        motion;
    std::vector<int>    newids;
    int             gestureHandler;
    MgJsonStorage   defaultStorage;
    long            regenPending;
    long            appendPending;
    long            redrawPending;
    
public:
    GiCoreViewImpl() : curview(NULL), refcount(1), gestureHandler(0)
        , regenPending(-1), appendPending(-1), redrawPending(-1) {
        motion.view = this;
        motion.gestureType = 0;
        motion.gestureState = kMgGesturePossible;
        _doc = new GcShapeDoc();
        _cmds = mgCreateCmdManager();
    }
    
    ~GiCoreViewImpl() {
        _cmds->release();
        delete _doc;
    }
    
    MgCmdManager* cmds() { return _cmds; }
    GcShapeDoc* document() { return _doc; }
    MgShapeDoc* doc() { return _doc->doc(); }
    MgShapes* shapes() { return doc()->getCurrentShapes(); }
    GiContext* context() { return doc()->context(); }
    GiTransform* xform() { return CALL_VIEW2(xform(), NULL); }
    GiGraphics* graph() { return CALL_VIEW2(graph(), NULL); }

    bool shapeWillAdded(MgShape*) { return true; }
    bool shapeWillDeleted(MgShape*) { return true; }
    bool shapeCanRotated(MgShape*) { return true; }
    bool shapeCanTransform(MgShape*) { return true; }
    bool shapeCanUnlock(MgShape*) { return true; }
    bool shapeCanUngroup(MgShape*) { return true; }
    void shapeMoved(MgShape*, int) {}

    void commandChanged() {
        CALL_VIEW(deviceView()->commandChanged());
    }
    void selectionChanged() {
        CALL_VIEW(deviceView()->selectionChanged());
    }

    bool removeShape(MgShape* shape) {
        showContextActions(0, NULL, Box2d::kIdentity(), NULL);
        return (shape && shape->getParent()
            && shape->getParent()->removeShape(shape->getID()));
    }

    bool useFinger() {
        return CALL_VIEW2(deviceView()->useFinger(), true);
    }
    
    bool isContextActionsVisible() {
        return CALL_VIEW2(deviceView()->isContextActionsVisible(), false);
    }

    bool showContextActions(int /*selState*/, const int* actions, 
                            const Box2d& selbox, const MgShape*)
    {
        int n = 0;
        for (; actions && actions[n] > 0; n++) {}

        if (n > 0 && motion.pressDrag && isContextActionsVisible()) {
            return false;
        }
        mgvector<int> arr(actions, n);
        return CALL_VIEW2(deviceView()->showContextActions(arr, selbox.xmin, 
            selbox.ymin, selbox.width(), selbox.height()), false);
    }
    
    void shapeAdded(MgShape* sp) {
        if (newids.empty()) {
            newids.push_back(sp->getID());      // 记下新图形的ID
            regenAppend();                      // 通知视图获取快照并增量重绘
        }
        else if (newids.back() != 0) {    // 已经regenAppend，但视图还未重绘
            newids.insert(newids.begin(), sp->getID()); // 记下更多的ID
        }
        else {                                  // 已经regenAppend并增量重绘
            regenAll();
        }
    }
    
    void redraw() {
        if (redrawPending >= 0) {
            redrawPending++;
        }
        else {
            CALL_VIEW(deviceView()->redraw());  // 将调用dynDraw
        }
    }
    
    void regenAll() {
        newids.clear();
        if (regenPending >= 0) {
            regenPending++;
        }
        else {  // 将调用drawAll
            for (int i = 0; i < _doc->getViewCount(); i++) {
                _doc->getView(i)->deviceView()->regenAll();
            }
        }
    }
    
    void regenAppend() {
        if (appendPending >= 0) {
            appendPending++;
        }
        else {  // 将调用drawAppend
            for (int i = 0; i < _doc->getViewCount(); i++) {
                _doc->getView(i)->deviceView()->regenAppend();
            }
        }
    }
    
    void setView(GcBaseView* view) {
        if (curview != view) {
            curview = view;
        }
    }

    static void onShapesLocked(MgShapeDoc* doc, void* obj, bool locked) {
        GiCoreViewImpl* p = (GiCoreViewImpl*)obj;
        if (!locked && p->curview && p->doc() == doc) {
            p->curview->deviceView()->contentChanged();
        }
    }
    
    void checkDrawAppendEnded();
    bool drawCommand(GcBaseView* view, const MgMotion& motion, GiGraphics& gs);
    bool gestureToCommand(const MgMotion& motion);
};

static int _dpi = 96;

class DrawLocker
{
    GiCoreViewImpl* _impl;
public:
    DrawLocker(GiCoreViewImpl* impl) : _impl(impl) {
		if (_impl->regenPending >= 0
			|| _impl->appendPending >= 0
			|| _impl->redrawPending >= 0)
		{
			_impl = NULL;
		}
		else {
			_impl->regenPending = 0;
			_impl->appendPending = 0;
			_impl->redrawPending = 0;
		}
    }

    ~DrawLocker() {
		if (!_impl) {
			return;
		}
        long regenPending = _impl->regenPending;
        long appendPending = _impl->appendPending;
        long redrawPending = _impl->redrawPending;

        _impl->regenPending = -1;
        _impl->appendPending = -1;
        _impl->redrawPending = -1;

        if (regenPending > 0)
            _impl->regenAll();
        else if (appendPending > 0)
            _impl->regenAppend();
        else if (redrawPending > 0)
            _impl->redraw();
    }
};

GcBaseView::GcBaseView(MgView* mgview, GiView *view)
    : _mgview(mgview), _view(view), _gs(&_xf)
{
    mgview->document()->addView(this);
}

MgShapeDoc* GcBaseView::doc()
{
    return cmdView()->document()->doc();
}

MgShapes* GcBaseView::shapes()
{
    return doc()->getCurrentShapes();
}

GiCoreView::GiCoreView(GiCoreView* mainView)
{
    if (mainView) {
        impl = mainView->impl;
        impl->refcount++;
    }
    else {
        impl = new GiCoreViewImpl;
    }
}

GiCoreView::~GiCoreView()
{
    if (--impl->refcount == 0) {
        delete impl;
    }
}

MgView* GiCoreView::viewAdapter()
{
    return impl;
}

void GiCoreView::createView(GiView* view, int type)
{
    if (view && !impl->_doc->findView(view)) {
        impl->curview = new GcGraphView(impl, view);
        if (type == 0) {
            setCommand(view, "splines");
        }
    }
}

void GiCoreView::createMagnifierView(GiView* newview, GiView* mainView)
{
    GcGraphView* refview = dynamic_cast<GcGraphView *>(impl->_doc->findView(mainView));
    
    if (refview && newview && !impl->_doc->findView(newview)) {
        new GcMagnifierView(impl, newview, refview);
    }
}

void GiCoreView::destoryView(GiView* view)
{
    GcBaseView* aview = this ? impl->_doc->findView(view) : NULL;

    if (aview) {
        impl->_doc->removeView(aview);

        if (impl->curview == aview) {
            impl->curview = impl->_doc->firstView();
        }
        delete aview;
    }
}

int GiCoreView::setBkColor(GiView* view, int argb)
{
    GcBaseView* aview = impl->_doc->findView(view);
    return aview ? aview->graph()->setBkColor(GiColor(argb)).getARGB() : 0;
}

void GiCoreView::setScreenDpi(int dpi)
{
    if (_dpi != dpi && dpi > 0) {
        _dpi = dpi;
    }
}

void GiCoreViewImpl::checkDrawAppendEnded()
{
    int n = 0;
    
    for (size_t i = 0; i < newids.size(); i++) {
        if (newids[i] == 0) {
            n++;
        }
    }
    if (n >= _doc->getViewCount()) {
        newids.clear();
    }
}

int GiCoreView::drawAll(GiView* view, GiCanvas* canvas)
{
    GcBaseView* aview = impl->_doc->findView(view);
    GiGraphics* gs = aview->graph();
    int n = 0;

    if (aview && gs->beginPaint(canvas)) {
        n = aview->drawAll(*gs);
        gs->endPaint();
    }
    if (!impl->newids.empty()) {
        impl->newids.push_back(0);
    }
    impl->checkDrawAppendEnded();
    
    return n;
}

bool GiCoreView::drawAppend(GiView* view, GiCanvas* canvas)
{
    GcBaseView* aview = impl->_doc->findView(view);
    GiGraphics* gs = aview->graph();
    int n = 0;

    if (aview && !impl->newids.empty()
        && gs->beginPaint(canvas)) {
        n = aview->drawAppend(&impl->newids.front(), *gs);
        gs->endPaint();
    }
    if (!impl->newids.empty()) {
        impl->newids.push_back(0);
    }
    impl->checkDrawAppendEnded();

    return n > 0;
}

void GiCoreView::dynDraw(GiView* view, GiCanvas* canvas)
{
    GcBaseView* aview = impl->_doc->findView(view);
    GiGraphics* gs = aview->graph();

    if (aview && gs->beginPaint(canvas)) {
        impl->drawCommand(aview, impl->motion, *gs);
        aview->dynDraw(impl->motion, *gs);
        gs->endPaint();
    }
}

void GiCoreView::onSize(GiView* view, int w, int h)
{
    GcBaseView* aview = impl->_doc->findView(view);
    if (aview) {
        aview->onSize(_dpi, w, h);
    }
}

bool GiCoreView::onGesture(GiView* view, GiGestureType type,
                           GiGestureState state, float x, float y, bool switchGesture)
{
    DrawLocker locker(impl);
    GcBaseView* aview = impl->_doc->findView(view);
    bool ret = false;

    if (aview) {
        impl->setView(aview);
        impl->motion.gestureType = type;
        impl->motion.gestureState = (MgGestureState)state;
        impl->motion.pressDrag = (type == kGiGesturePress && state < kGiGestureEnded);
        impl->motion.switchGesture = switchGesture;
        impl->motion.point.set(x, y);
        impl->motion.pointM = impl->motion.point * aview->xform()->displayToModel();
        impl->motion.point2 = impl->motion.point;
        impl->motion.point2M = impl->motion.pointM;

        if (state <= kGiGestureBegan) {
            impl->motion.startPt = impl->motion.point;
            impl->motion.startPtM = impl->motion.pointM;
            impl->motion.lastPt = impl->motion.point;
            impl->motion.lastPtM = impl->motion.pointM;
            impl->motion.startPt2 = impl->motion.point;
            impl->motion.startPt2M = impl->motion.pointM;
            impl->gestureHandler = (impl->gestureToCommand(impl->motion) ? 1
                                    : (aview->onGesture(impl->motion) ? 2 : 0));
        }
        else if (impl->gestureHandler == 1) {
            impl->gestureToCommand(impl->motion);
        }
        else if (impl->gestureHandler == 2) {
            aview->onGesture(impl->motion);
        }
        ret = (impl->gestureHandler > 0);

        impl->motion.lastPt = impl->motion.point;
        impl->motion.lastPtM = impl->motion.pointM;
    }
    
    return ret;
}

bool GiCoreView::twoFingersMove(GiView* view, GiGestureState state,
                                float x1, float y1, float x2, float y2, bool switchGesture)
{
    DrawLocker locker(impl);
    GcBaseView* aview = impl->_doc->findView(view);
    bool ret = false;

    if (aview) {
        impl->setView(aview);
        impl->motion.gestureType = kGiTwoFingersMove;
        impl->motion.gestureState = (MgGestureState)state;
        impl->motion.pressDrag = false;
        impl->motion.switchGesture = switchGesture;
        impl->motion.point.set(x1, y1);
        impl->motion.pointM = impl->motion.point * aview->xform()->displayToModel();
        impl->motion.point2.set(x2, y2);
        impl->motion.point2M = impl->motion.point2 * aview->xform()->displayToModel();

        if (state <= kGiGestureBegan) {
            impl->motion.startPt = impl->motion.point;
            impl->motion.startPtM = impl->motion.pointM;
            impl->motion.lastPt = impl->motion.point;
            impl->motion.lastPtM = impl->motion.pointM;
            impl->motion.startPt2 = impl->motion.point2;
            impl->motion.startPt2M = impl->motion.point2M;
            impl->gestureHandler = (impl->gestureToCommand(impl->motion) ? 1
                                    : (aview->twoFingersMove(impl->motion) ? 2 : 0));
        }
        else if (impl->gestureHandler == 1) {
            impl->gestureToCommand(impl->motion);
        }
        else if (impl->gestureHandler == 2) {
            aview->twoFingersMove(impl->motion);
        }
        ret = (impl->gestureHandler > 0);

        impl->motion.lastPt = impl->motion.point;
        impl->motion.lastPtM = impl->motion.pointM;
    }
    
    return ret;
}

const char* GiCoreView::getCommand() const
{
    return impl->_cmds->getCommandName();
}

bool GiCoreView::setCommand(GiView* view, const char* name)
{
    DrawLocker locker(impl);
    GcBaseView* aview = impl->_doc->findView(view);
    bool ret = false;
    
    if (aview) {
        impl->setView(aview);
        ret = impl->_cmds->setCommand(&impl->motion, name);
    }
    
    return ret;
}

bool GiCoreView::doContextAction(int action)
{
    DrawLocker locker(impl);
    return impl->_cmds->doContextAction(&impl->motion, action);
}

void GiCoreView::clearCachedData()
{
    impl->doc()->clearCachedData();
}

int GiCoreView::addShapesForTest()
{
    int n = RandomParam(1000).addShapes(impl->shapes());
    impl->regenAll();
    return n;
}

int GiCoreView::getShapeCount()
{
    MgShapesLock locker(MgShapesLock::ReadOnly, impl->doc());
    return impl->doc()->getShapeCount();
}

int GiCoreView::getSelectedShapeCount()
{
	return impl->cmds()->getSelection(impl, 0, NULL);
}

int GiCoreView::getSelectedShapeType()
{
	int n = impl->cmds()->getSelection(impl, 0, NULL);
	int type = 0;
	std::vector<MgShape*> arr(n, (MgShape*)0);

	if (n > 0) {
		n = impl->cmds()->getSelection(impl, n, (MgShape**)&arr.front());
		for (int i = 0; i < n; i++) {
			if (type == 0) {
				type = arr[i]->shapec()->getType();
			}
			else if (type != arr[i]->shapec()->getType()) {
				type = kMgShapeMultiType;
				break;
			}
		}
	}

	return type;
}

static bool loadShapes(GiCoreViewImpl* impl, MgStorage* s)
{
    bool ret = true;

	impl->_cmds->setCommand(&impl->motion, impl->_cmds->getCommandName());

    if (s) {
        MgShapesLock locker(MgShapesLock::Load, impl->doc());
        ret = impl->doc()->load(s);
    }
    else {
        MgShapesLock locker(MgShapesLock::Remove, impl->doc());
        impl->doc()->clear();
    }
    impl->regenAll();
	TradeCmd::onDocLoaded(&impl->motion);

    return ret;
}

static bool saveShapes(GiCoreViewImpl* impl, MgStorage* s)
{
    MgShapesLock locker(MgShapesLock::ReadOnly, impl->doc());
    return s && impl->doc()->save(s);
}

const char* GiCoreView::getContent()
{
    const char* content = "";
    if (saveShapes(impl, impl->defaultStorage.storageForWrite())) {
        content = impl->defaultStorage.stringify();
    }
    return content; // has't free defaultStorage's string buffer
}

void GiCoreView::freeContent()
{
    impl->defaultStorage.clear();
}

bool GiCoreView::setContent(const char* content)
{
	DrawLocker locker(impl);
    bool ret = loadShapes(impl, impl->defaultStorage.storageForRead(content));
    impl->defaultStorage.clear();
    return ret;
}

bool GiCoreView::loadFromFile(const char* vgfile)
{
#if defined(_MSC_VER) && _MSC_VER >= 1400 // VC8
    FILE *fp = NULL;
    fopen_s(&fp, vgfile, "rt");
#else
    FILE *fp = fopen(vgfile, "rt");
#endif
	DrawLocker locker(impl);
    MgJsonStorage s;
    bool ret = loadShapes(impl, s.storageForRead(fp));
    
    if (fp) {
        fclose(fp);
    }

    return ret;
}

bool GiCoreView::saveToFile(const char* vgfile, bool pretty)
{
#if defined(_MSC_VER) && _MSC_VER >= 1400 // VC8
    FILE *fp = NULL;
    fopen_s(&fp, vgfile, "wt");
#else
    FILE *fp = fopen(vgfile, "wt");
#endif
    MgJsonStorage s;
    bool ret = (fp != NULL
        && saveShapes(impl, s.storageForWrite())
        && s.save(fp, pretty));

    if (fp) {
        fclose(fp);
    }

    return ret;
}

bool GiCoreView::zoomToExtent()
{
    Box2d rect(impl->doc()->getExtent() * impl->xform()->modelToWorld());
    bool ret = impl->xform()->zoomTo(rect);
    if (ret) {
        impl->regenAll();
    }
    return ret;
}

bool GiCoreView::zoomToModel(float x, float y, float w, float h)
{
    Box2d rect(Box2d(x, y, x + w, y + h) * impl->xform()->modelToWorld());
    bool ret = impl->xform()->zoomTo(rect);
    if (ret) {
        impl->regenAll();
    }
    return ret;
}

float GiCoreView::calcPenWidth(float lineWidth)
{
    return impl->graph()->calcPenWidth(lineWidth, false);
}

static GiContext _tmpContext;
static bool _contextEditing = false;

GiContext& GiCoreView::getContext(bool forChange)
{
    if (!forChange) {
        _contextEditing = false;
    }
    
    MgShape* shape = NULL;
    impl->_cmds->getSelection(impl, 1, &shape, forChange);
    _tmpContext = shape ? *shape->context() : *impl->context();
    
    return _tmpContext;
}

void GiCoreView::setContext(int mask)
{
    setContext(_tmpContext, mask, _contextEditing ? 0 : 1);
}

void GiCoreView::setContextEditing(bool editing)
{
    if (_contextEditing != editing) {
        _contextEditing = editing;
        if (!editing) {
            DrawLocker locker(impl);
            impl->_cmds->dynamicChangeEnded(impl, true);
        }
    }
}

void GiCoreView::setContext(const GiContext& ctx, int mask, int apply)
{
    DrawLocker locker(impl);

    if (mask != 0) {
        int n = impl->_cmds->getSelection(impl, 0, NULL, true);
        std::vector<MgShape*> shapes(n, (MgShape*)0);

        if (n > 0 && impl->_cmds->getSelection(impl, n, 
            (MgShape**)&shapes.front(), true) > 0)
        {
            for (int i = 0; i < n; i++) {
                if (shapes[i]) {
                    shapes[i]->context()->copy(ctx, mask);
                }
            }
            impl->redraw();
        }
        else {
            impl->context()->copy(ctx, mask);
        }
    }

    if (apply != 0) {
        impl->_cmds->dynamicChangeEnded(impl, apply > 0);
    }
}

bool GiCoreViewImpl::drawCommand(GcBaseView* view, const MgMotion& motion, GiGraphics& gs)
{
    bool ret = false;

    if (view == curview) {
        MgDynShapeLock locker(false);
        MgCommand* cmd = _cmds->getCommand();

        ret = cmd && cmd->draw(&motion, &gs);
        if (ret && cmd->isDrawingCommand()) {
            TradeCmd::drawInShapeCommand(_cmds->getCommandName(), 
                (MgCommandDraw *)cmd, &gs);
        }
    }

    return ret;
}

bool GiCoreViewImpl::gestureToCommand(const MgMotion& motion)
{
    MgDynShapeLock locker(true);
    MgCommand* cmd = _cmds->getCommand();

    if (motion.gestureState == kMgGestureCancel || !cmd) {
        return cmd && cmd->cancel(&motion);
    }
    if (motion.gestureState == kMgGesturePossible
        && motion.gestureType != kGiTwoFingersMove) {
        return true;
    }

    switch (motion.gestureType) {
        case kGiTwoFingersMove:
            return cmd->twoFingersMove(&motion);
        case kGiGesturePan:
            switch (motion.gestureState) {
                case kMgGestureBegan:
                    return cmd->touchBegan(&motion);
                case kMgGestureMoved:
                    return cmd->touchMoved(&motion);
                case kMgGestureEnded:
                default:
                    return cmd->touchEnded(&motion);
            }
            break;
        case kGiGestureTap:
            return cmd->click(&motion);
        case kGiGestureDblTap:
            return cmd->doubleClick(&motion);
        case kGiGesturePress:
            return cmd->longPress(&motion);
    }

    return false;
}
