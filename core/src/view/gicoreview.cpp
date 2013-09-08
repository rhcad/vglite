//! \file gicoreview.cpp
//! \brief 实现内核视图类 GiCoreView
// Copyright (c) 2012-2013, https://github.com/rhcad/touchvg

#include "gicoreview.h"
#include "giview.h"
#include "gicanvas.h"
#include "testcanvas.h"
#include <stdio.h>
#include <vector>

#include "GcShapeDoc.h"
#include "GcMagnifierView.h"
#include "mgcmd.h"
#include <RandomShape.h>
#include <mglog.h>
#include <mgjsonstorage.h>

//! 测试视图
class GcDummyView : public GcBaseView
{
public:
    GcDummyView(MgView* mgview, GiView *view) : GcBaseView(mgview, view) {}
    virtual ~GcDummyView() {}
    
    virtual int drawAll(GiGraphics& gs);
    virtual int drawAppend(const int* newids, GiGraphics& gs);
    virtual void dynDraw(const MgMotion& motion, GiGraphics& gs);
    virtual void onSize(int dpi, int w, int h);
    virtual bool onGesture(const MgMotion& motion);
    virtual bool twoFingersMove(const MgMotion& motion);

private:
    void drawPoints(GiCanvas* canvas);
    
    std::vector<float>   _pts;
};

// GiCoreView
//

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
    
public:
    GiCoreViewImpl() : curview(NULL), refcount(1), gestureHandler(0) {
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

    bool useFinger() { return CALL_VIEW2(deviceView()->useFinger(), true); }
    void selChanged() {}
    bool shapeWillAdded(MgShape*) { return true; }
    bool shapeWillDeleted(MgShape*) { return true; }
    bool removeShape(MgShape*) { return true; }
    bool shapeCanRotated(MgShape*) { return true; }
    bool shapeCanTransform(MgShape*) { return true; }
    void shapeMoved(MgShape*, int) {}
    void commandChanged() {}
    
    bool isContextActionsVisible() { return false; }
    bool showContextActions(int, const int*,
                            const Box2d&, const MgShape*) { return false; }
    
    void shapeAdded(MgShape* sp) {
        if (sp && newids.empty()) {
            newids.push_back(sp->getID());      // 记下新图形的ID
            regenAppend();                      // 通知视图获取快照并增量重绘
        }
        else if (sp && newids.back() != 0) {    // 已经regenAppend，但视图还未重绘
            newids.insert(newids.begin(), sp->getID()); // 记下更多的ID
        }
        else {                                  // 已经regenAppend并增量重绘
            regenAll();
        }
    }
    
    void redraw() {
        CALL_VIEW(deviceView()->redraw());                  // 将调用dynDraw
    }
    
    void regenAll() {
        newids.clear();
        for (int i = 0; i < _doc->getViewCount(); i++) {
            _doc->getView(i)->deviceView()->regenAll();     // 将调用drawAll
        }
    }
    
    void regenAppend() {
        for (int i = 0; i < _doc->getViewCount(); i++) {
            _doc->getView(i)->deviceView()->regenAppend();  // 将调用drawAppend
        }
    }
    
    void setView(GcBaseView* view) {
        if (curview != view) {
            curview = view;
        }
    }
    
    void checkDrawAppendEnded();
    bool drawCommand(GcBaseView* view, const MgMotion& motion, GiGraphics& gs);
    bool gestureToCommand(const MgMotion& motion);
};

static int _dpi = 96;

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

void GiCoreView::createView(GiView* view, int type)
{
    if (view && !impl->_doc->findView(view)) {
        if (type == 1) {
            impl->curview = new GcGraphView(impl, view);
        }
        else {
            new GcDummyView(impl, view);
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

const char* GiCoreView::command() const
{
    return impl->_cmds->getCommandName();
}

bool GiCoreView::setCommand(GiView* view, const char* name)
{
    GcBaseView* aview = impl->_doc->findView(view);
    bool ret = false;
    
    if (aview) {
        impl->setView(aview);
        ret = impl->_cmds->setCommand(&impl->motion, name);
    }
    
    return ret;
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

static bool loadShapes(GiCoreViewImpl* impl, MgStorage* s)
{
    bool ret = true;

    if (s) {
        MgShapesLock locker(MgShapesLock::Load, impl->doc());
        ret = impl->doc()->load(s);
    }
    else {
        MgShapesLock locker(MgShapesLock::Remove, impl->doc());
        impl->doc()->clear();
    }
    impl->regenAll();

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

bool GiCoreViewImpl::drawCommand(GcBaseView* view, const MgMotion& motion, GiGraphics& gs)
{
    if (view == curview) {
        MgDynShapeLock locker(false);
        MgCommand* cmd = _cmds->getCommand();
        return cmd && cmd->draw(&motion, &gs);
    }
    return false;
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

// GcDummyView
//

int GcDummyView::drawAll(GiGraphics& gs)
{
    int n = TestCanvas::randInt(900, 1000);
    TestCanvas::test(gs.getCanvas(), 0x08, n, true);
    return n;
}

void GcDummyView::drawPoints(GiCanvas* canvas)
{
    if (!_pts.empty()) {
        canvas->drawEllipse(_pts[_pts.size() - 2] - 50.f,
                           _pts.back() - 50.f, 100, 100, true, true);
        canvas->beginPath();
        canvas->moveTo(_pts[0], _pts[1]);
        for (size_t i = 2; i + 1 < _pts.size(); i += 2) {
            canvas->lineTo(_pts[i], _pts[i+1]);
        }
        canvas->drawPath(true, false);
    }
}

int GcDummyView::drawAppend(const int*, GiGraphics& gs)
{
    GiCanvas* canvas = gs.getCanvas();
    canvas->setPen(TestCanvas::randInt(20, 0xFF) << 24 | TestCanvas::randInt(0, 0xFFFFFF),
                  TestCanvas::randFloat(1, 10), -1, 0);
    canvas->setBrush(TestCanvas::randInt(10, 0xFF) << 24 | TestCanvas::randInt(0, 0xFFFFFF), 0);
    drawPoints(canvas);
    _pts.clear();
    return 1;
}

void GcDummyView::dynDraw(const MgMotion& motion, GiGraphics& gs)
{
    if (motion.gestureState == kMgGestureBegan
        || motion.gestureState == kMgGestureMoved) {
        static float phase = 0;
        GiCanvas* canvas = gs.getCanvas();

        phase -= 1;
        canvas->setPen(0, 0, 1, phase);
        canvas->setBrush(0x80005500, 0);
        drawPoints(canvas);

        float x = _pts[_pts.size() - 2];
        float y = _pts.back();
        char text[40] = "";
#if defined(_MSC_VER) && _MSC_VER >= 1400 // VC8
        sprintf_s(text, sizeof(text), "%.1f, %.1f", x, y);
#else
        sprintf(text, "%.1f, %.1f", x, y);
#endif
        canvas->drawTextAt(text, x < 80 ? 80 : x, y < 80 ? 80 : y - 70, 20, 1);
    }
}

void GcDummyView::onSize(int, int, int)
{
}

bool GcDummyView::onGesture(const MgMotion& motion)
{
    if (motion.gestureState == kMgGestureBegan) {
        _pts.clear();
    }
    _pts.push_back(motion.point.x);
    _pts.push_back(motion.point.y);
    
    if (motion.gestureType == kGiGesturePan && motion.gestureState == kMgGestureEnded) {
        deviceView()->regenAppend();
    }
    else {
        deviceView()->redraw();
    }
    return true;
}

bool GcDummyView::twoFingersMove(const MgMotion&)
{
    return true;
}
