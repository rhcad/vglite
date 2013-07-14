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
#include "mgcmdmgr.h"

//! 测试视图
class GcDummyView : public GcBaseView
{
public:
    GcDummyView(MgView* mgview, GiView *view) : GcBaseView(mgview, view) {}
    virtual ~GcDummyView() {}
    
    virtual void drawAll(GiGraphics& gs);
    virtual void drawAppend(GiGraphics& gs);
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

class GiCoreViewImpl : public MgView
{
public:
    GcShapeDoc*     _doc;
    MgCmdManager*   _cmds;
    GcBaseView*     curview;
    long            refcount;
    MgMotion        motion;
    
    GiCoreViewImpl() : curview(NULL), refcount(1) {
        motion.view = this;
        motion.gestureType = 0;
        motion.gestureState = kMgGesturePossible;
        _doc = new GcShapeDoc();
        _cmds = new MgCmdManager();
    }
    
    ~GiCoreViewImpl() {
        delete _doc;
        delete _cmds;
    }
    
    MgCmdManager* cmds() { return _cmds; }
    GcShapeDoc* document() { return _doc; }
    MgShapeDoc* doc() { return _doc->doc(); }
    MgShapes* shapes() { return doc()->getCurrentShapes(); }
    GiContext* context() { return doc()->context(); }
    
    GcBaseView* currentView() {
        return curview ? curview : _doc->firstView();
    }

    bool useFinger() {
        GcBaseView* view = currentView();
        return view ? view->deviceView()->useFinger() : true;
    }
    
    GiTransform* xform() {
        GcBaseView* view = currentView();
        return view ? view->xform() : NULL;
    }
    
    GiGraphics* graph() {
        GcBaseView* view = currentView();
        return view ? view->graph() : NULL;
    }
    
    void regenAll() {
        for (int i = 0; i < _doc->getViewCount(); i++) {
            _doc->getView(i)->deviceView()->regenAll();
        }
    }
    
    void regenAppend() {
        for (int i = 0; i < _doc->getViewCount(); i++) {
            _doc->getView(i)->deviceView()->regenAppend();
        }
    }
    
    void redraw() {
        GcBaseView* view = currentView();
        if (view) {
            view->deviceView()->redraw();
        }
    }
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
        if (impl->curview == aview) {
            impl->curview = NULL;
        }
        impl->_doc->removeView(aview);
        delete aview;
    }
}

void GiCoreView::setScreenDpi(int dpi)
{
    if (_dpi != dpi) {
        _dpi = dpi;
    }
}

void GiCoreView::drawAll(GiView* view, GiCanvas* canvas)
{
    GcBaseView* aview = impl->_doc->findView(view);

    if (aview && aview->graph()->beginPaint(canvas)) {
        aview->drawAll(*aview->graph());
        aview->graph()->endPaint();
    }
}

bool GiCoreView::drawAppend(GiView* view, GiCanvas* canvas)
{
    GcBaseView* aview = impl->_doc->findView(view);

    if (aview && aview->graph()->beginPaint(canvas)) {
        aview->drawAppend(*aview->graph());
        aview->graph()->endPaint();
        return true;
    }

    return false;
}

void GiCoreView::dynDraw(GiView* view, GiCanvas* canvas)
{
    GcBaseView* aview = impl->_doc->findView(view);

    if (aview && aview->graph()->beginPaint(canvas)) {
        aview->dynDraw(impl->motion, *aview->graph());
        aview->graph()->endPaint();
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
        impl->curview = aview;
        impl->motion.gestureType = gestureType;
        impl->motion.gestureState = (MgGestureState)gestureState;
        impl->motion.point.set(x, y);
        impl->motion.pointM = impl->motion.point * aview->xform()->displayToModel();
        impl->motion.point2 = impl->motion.point;
        impl->motion.point2M = impl->motion.pointM;

        if (gestureState <= kGiGestureBegan) {
            impl->motion.firstPt = impl->motion.point;
            impl->motion.firstPtM = impl->motion.pointM;
            impl->motion.lastPt = impl->motion.point;
            impl->motion.lastPtM = impl->motion.pointM;
            impl->motion.firstPt2 = impl->motion.point;
            impl->motion.firstPt2M = impl->motion.pointM;
        }

        ret = (impl->_cmds->onGesture(impl->motion)
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
        impl->curview = aview;
        impl->motion.gestureType = kGiTwoFingersMove;
        impl->motion.gestureState = (MgGestureState)gestureState;
        impl->motion.point.set(x1, y1);
        impl->motion.pointM = impl->motion.point * aview->xform()->displayToModel();
        impl->motion.point2.set(x2, y2);
        impl->motion.point2M = impl->motion.point2 * aview->xform()->displayToModel();

        if (gestureState <= kGiGestureBegan) {
            impl->motion.firstPt = impl->motion.point;
            impl->motion.firstPtM = impl->motion.pointM;
            impl->motion.lastPt = impl->motion.point;
            impl->motion.lastPtM = impl->motion.pointM;
            impl->motion.firstPt2 = impl->motion.point2;
            impl->motion.firstPt2M = impl->motion.point2M;
        }

        ret = (impl->_cmds->twoFingersMove(impl->motion)
            || aview->twoFingersMove(impl->motion));

        impl->motion.lastPt = impl->motion.point;
        impl->motion.lastPtM = impl->motion.pointM;
    }
    
    return ret;
}

void GiCoreView::clearCachedData()
{
    impl->_doc->doc()->clearCachedData();
}

// GcDummyView
//

void GcDummyView::drawAll(GiGraphics& gs)
{
    int n = TestCanvas::randInt(900, 1000);
    TestCanvas::test(gs.getCanvas(), 0x08, n, true);
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

void GcDummyView::drawAppend(GiGraphics& gs)
{
    GiCanvas* canvas = gs.getCanvas();
    canvas->setPen(TestCanvas::randInt(20, 0xFF) << 24 | TestCanvas::randInt(0, 0xFFFFFF),
                  TestCanvas::randFloat(1, 10), -1, 0);
    canvas->setBrush(TestCanvas::randInt(10, 0xFF) << 24 | TestCanvas::randInt(0, 0xFFFFFF), 0);
    drawPoints(canvas);
    _pts.clear();
}

void GcDummyView::dynDraw(const MgMotion& motion, GiGraphics& gs)
{
	if (motion.gestureState == kGiGestureBegan
        || motion.gestureState == kGiGestureMoved) {
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
    if (motion.gestureState == kGiGestureBegan) {
        _pts.clear();
    }
    _pts.push_back(motion.point.x);
    _pts.push_back(motion.point.y);
    
    if (motion.gestureType == kGiGesturePan && motion.gestureState == kGiGestureEnded) {
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
