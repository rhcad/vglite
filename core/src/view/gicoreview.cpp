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
    GcDummyView(GiView *view) : GcBaseView(view), _gestureState(kGiGestureCancel) {}
    virtual ~GcDummyView() {}
    
    virtual void drawAll(GiCanvas* canvas);
    virtual void drawAppend(GiCanvas* canvas);
    virtual void dynDraw(GiCanvas* canvas);
    virtual void onSize(int w, int h);
    virtual bool onGesture(GiGestureType gestureType,
                           GiGestureState gestureState, float x, float y);
    virtual bool twoFingersMove(GiGestureState gestureState,
                                float x1, float y1, float x2, float y2);
private:
    void drawPoints(GiCanvas* canvas);
    
    std::vector<float>   _pts;
    GiGestureState  _gestureState;
};

// GiCoreView
//

class GiCoreViewImpl : public MgView
{
public:
    GcShapeDoc*     _doc;
    MgCmdManager*   _cmds;
    long            refcount;
    GiView*         curview;
    int             state;
    
    GiCoreViewImpl() : refcount(1), curview((GiView*)0), state(0) {
        _doc = new GcShapeDoc();
        _cmds = new MgCmdManager();
    }
    
    ~GiCoreViewImpl() {
        delete _doc;
        delete _cmds;
    }
    
    GcShapeDoc* doc() { return _doc; }
    MgCmdManager* cmds() { return _cmds; }
    MgGestureState gestureState() { return (MgGestureState)state; }
    
    GiView* currentView() {
        return curview ? curview : _doc->firstView();
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
        GiView* view = currentView();
        if (view) {
            view->redraw();
        }
    }
};

static int _dpi = 1;

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
    GcBaseView* aview;
    
    if (view && !impl->_doc->findView(view)) {
        if (type == 1) {
            aview = new GcGraphView(view);
            impl->curview = view;
        }
        else {
            aview = new GcDummyView(view);
        }
        
        impl->_doc->addView(aview);
    }
}

void GiCoreView::createMagnifierView(GiView* newview, GiView* mainView)
{
    GcGraphView* refview = dynamic_cast<GcGraphView *>(impl->_doc->findView(mainView));
    
    if (refview && newview && !impl->_doc->findView(newview)) {
        GcBaseView* aview = new GcMagnifierView(newview, refview);
        impl->_doc->addView(aview);
    }
}

void GiCoreView::destoryView(GiView* view)
{
    GcBaseView* aview = impl->_doc->findView(view);
    if (aview) {
        if (impl->curview == view) {
            impl->curview = (GiView *)0;
        }
        impl->_doc->removeView(aview);
        delete aview;
    }
}

void GiCoreView::setScreenDpi(int dpi)
{
    _dpi = dpi;
}

void GiCoreView::drawAll(GiView* view, GiCanvas* canvas)
{
    GcBaseView* aview = impl->_doc->findView(view);
    if (aview) {
        aview->drawAll(canvas);
    }
}

bool GiCoreView::drawAppend(GiView* view, GiCanvas* canvas)
{
    GcBaseView* aview = impl->_doc->findView(view);
    if (aview) {
        aview->drawAppend(canvas);
        return true;
    }
    return false;
}

void GiCoreView::dynDraw(GiView* view, GiCanvas* canvas)
{
    GcBaseView* aview = impl->_doc->findView(view);
    if (aview) {
        aview->dynDraw(canvas);
    }
}

void GiCoreView::onSize(GiView* view, int w, int h)
{
    GcBaseView* aview = impl->_doc->findView(view);
    if (aview) {
        aview->onSize(w, h);
    }
}

bool GiCoreView::onGesture(GiView* view, GiGestureType gestureType,
                           GiGestureState gestureState, float x, float y)
{
    GcBaseView* aview = impl->_doc->findView(view);
    
    impl->curview = view;
    impl->state = gestureState;
    
    return aview && aview->onGesture(gestureType, gestureState, x, y);
}

bool GiCoreView::twoFingersMove(GiView* view, GiGestureState gestureState,
                                float x1, float y1, float x2, float y2)
{
    GcBaseView* aview = impl->_doc->findView(view);
    
    impl->curview = view;
    impl->state = gestureState;
    
    return aview && aview->twoFingersMove(gestureState, x1, y1, x2, y2);
}

// GcDummyView
//

void GcDummyView::drawAll(GiCanvas* canvas)
{
    int n = TestCanvas::randInt(900, 1000);
    TestCanvas::test(canvas, 0x08, n, true);
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

void GcDummyView::drawAppend(GiCanvas* canvas)
{
    canvas->setPen(TestCanvas::randInt(20, 0xFF) << 24 | TestCanvas::randInt(0, 0xFFFFFF),
                  TestCanvas::randFloat(1, 10), -1, 0);
    canvas->setBrush(TestCanvas::randInt(10, 0xFF) << 24 | TestCanvas::randInt(0, 0xFFFFFF), 0);
    drawPoints(canvas);
    _pts.clear();
}

void GcDummyView::dynDraw(GiCanvas* canvas)
{
	if (_gestureState == kGiGestureBegan || _gestureState == kGiGestureMoved) {
		static float phase = 0;
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

void GcDummyView::onSize(int w, int h)
{
    if (w && h) {}
}

bool GcDummyView::onGesture(GiGestureType gestureType,
                            GiGestureState gestureState, float x, float y)
{
	_gestureState = gestureState;
    if (gestureState == kGiGestureBegan) {
        _pts.clear();
    }
    _pts.push_back(x);
    _pts.push_back(y);
    
    if (gestureType == kGiGesturePan && gestureState == kGiGestureEnded) {
        deviceView()->regenAppend();
    }
    else {
        deviceView()->redraw();
    }
    return true;
}

bool GcDummyView::twoFingersMove(GiGestureState gestureState,
                                 float x1, float y1, float x2, float y2)
{
	_gestureState = gestureState;
    if (gestureState == kGiGestureMoved) {
        x2 = x1; y2 = y1; if (x2 > y2) x2 = x1; // avoid not used
        deviceView()->redraw();
    }
    return true;
}
