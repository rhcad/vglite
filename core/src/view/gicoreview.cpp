//! \file gicoreview.cpp
//! \brief 实现内核视图类 GiCoreView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#include "gicoreview.h"
#include "giview.h"
#include "gicanvas.h"
#include "testcanvas.h"
#include <stdio.h>
#include <vector>

#include "GcShapeDoc.h"
#include "GcGraphView.h"
#include "GcMagnifierView.h"
#include "mgcmd.h"
#include <RandomShape.h>

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
    
public:
    GiCoreViewImpl() : curview(NULL), refcount(1) {
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
        if (sp && (newids.empty() || newids.back() != 0)) {
            newids.push_back(sp->getID());
            regenAppend();
        }
        else {
            regenAll();
        }
    }
    
    void redraw() {
        CALL_VIEW(deviceView()->redraw());
    }
    
    void regenAll() {
        newids.clear();
        for (int i = 0; i < _doc->getViewCount(); i++) {
            _doc->getView(i)->deviceView()->regenAll();
        }
    }
    
    void regenAppend() {
        for (int i = 0; i < _doc->getViewCount(); i++) {
            _doc->getView(i)->deviceView()->regenAppend();
        }
    }
    
    void setView(GcBaseView* view) {
        if (curview != view) {
            curview = view;
        }
    }
    
    bool dynDraw(const MgMotion& motion, GiGraphics& gs);
    bool onGesture(const MgMotion& motion);
};

static int _dpi = 1;

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
    GcBaseView* aview = impl->_doc->findView(view);

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
    if (_dpi != dpi) {
        _dpi = dpi;
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
    
    return n;
}

bool GiCoreView::drawAppend(GiView* view, GiCanvas* canvas)
{
    GcBaseView* aview = impl->_doc->findView(view);
    GiGraphics* gs = aview->graph();
    int n = 0;

    if (aview && !impl->newids.empty()
        && gs->beginPaint(canvas)) {
        impl->newids.push_back(0);
        n = aview->drawAppend(&impl->newids.front(), *gs);
        gs->endPaint();
    }

    return n > 0;
}

void GiCoreView::dynDraw(GiView* view, GiCanvas* canvas)
{
    GcBaseView* aview = impl->_doc->findView(view);
    GiGraphics* gs = aview->graph();

    if (aview && gs->beginPaint(canvas)) {
        impl->dynDraw(impl->motion, *gs);
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

bool GiCoreView::onGesture(GiView* view, GiGestureType gestureType,
                           GiGestureState gestureState, float x, float y)
{
    GcBaseView* aview = impl->_doc->findView(view);
    bool ret = false;

    if (aview) {
        impl->setView(aview);
        impl->motion.gestureType = gestureType;
        impl->motion.gestureState = (MgGestureState)gestureState;
        impl->motion.point.set(x, y);
        impl->motion.pointM = impl->motion.point * aview->xform()->displayToModel();
        impl->motion.point2 = impl->motion.point;
        impl->motion.point2M = impl->motion.pointM;

        if (gestureState <= kGiGestureBegan) {
            impl->motion.startPt = impl->motion.point;
            impl->motion.startPtM = impl->motion.pointM;
            impl->motion.lastPt = impl->motion.point;
            impl->motion.lastPtM = impl->motion.pointM;
            impl->motion.startPt2 = impl->motion.point;
            impl->motion.startPt2M = impl->motion.pointM;
        }

        ret = (impl->onGesture(impl->motion)
            || aview->onGesture(impl->motion));

        impl->motion.lastPt = impl->motion.point;
        impl->motion.lastPtM = impl->motion.pointM;
    }
    
    return ret;
}

bool GiCoreView::twoFingersMove(GiView* view, GiGestureState gestureState,
                                float x1, float y1, float x2, float y2)
{
    GcBaseView* aview = impl->_doc->findView(view);
    bool ret = false;

    if (aview) {
        impl->setView(aview);
        impl->motion.gestureType = kGiTwoFingersMove;
        impl->motion.gestureState = (MgGestureState)gestureState;
        impl->motion.point.set(x1, y1);
        impl->motion.pointM = impl->motion.point * aview->xform()->displayToModel();
        impl->motion.point2.set(x2, y2);
        impl->motion.point2M = impl->motion.point2 * aview->xform()->displayToModel();

        if (gestureState <= kGiGestureBegan) {
            impl->motion.startPt = impl->motion.point;
            impl->motion.startPtM = impl->motion.pointM;
            impl->motion.lastPt = impl->motion.point;
            impl->motion.lastPtM = impl->motion.pointM;
            impl->motion.startPt2 = impl->motion.point2;
            impl->motion.startPt2M = impl->motion.point2M;
        }

        ret = (impl->onGesture(impl->motion)
            || aview->twoFingersMove(impl->motion));

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

void GiCoreView::addShapesForTest()
{
    RandomParam(200).addShapes(impl->shapes());
    impl->regenAll();
}

bool GiCoreView::loadShapes(MgStorage* s)
{
    return impl->doc()->load(s);
}

bool GiCoreView::saveShapes(MgStorage* s)
{
    return impl->doc()->save(s);
}

void GiCoreView::zoomToExtent()
{
    Box2d rect(impl->doc()->getExtent() * impl->xform()->modelToWorld());
    impl->xform()->zoomTo(rect);
    impl->regenAll();
}

bool GiCoreViewImpl::dynDraw(const MgMotion& motion, GiGraphics& gs)
{
    return _cmds->draw(&motion, &gs);
}

bool GiCoreViewImpl::onGesture(const MgMotion& motion)
{
    bool forTouch = (motion.gestureState >= kMgGestureBegan
                     && motion.gestureState <= kMgGestureEnded);
    MgCommand* cmd = _cmds->getCommand(motion.view, forTouch);

    if (motion.gestureState == kMgGestureCancel) {
        return cmd && cmd->cancel(&motion);
    }
    if (!forTouch || !cmd) {    // kMgGesturePossible
        return !!cmd;
    }

    switch (motion.gestureType)
    {
    case kGiGesturePan:
        switch (motion.gestureState)
        {
        case kMgGestureBegan:
            return cmd->touchBegan(&motion);
        case kMgGestureMoved:
            return cmd->touchMoved(&motion);
        case kMgGestureEnded:
        default:
            return cmd->touchEnded(&motion);
        }
    case kGiGestureTap:
        return cmd->click(&motion);
    case kGiGestureDblTap:
        return cmd->doubleClick(&motion);
    case kGiGesturePress:
        return cmd->longPress(&motion);
    case kGiTwoFingersMove:
        return cmd->twoFingersMove(&motion);
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
        for (size_t i = 0; i + 1 < _pts.size(); i += 2) {
            if (i == 0) {
                canvas->moveTo(_pts[i], _pts[i+1]);
            }
            else {
                canvas->lineTo(_pts[i], _pts[i+1]);
            }
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
