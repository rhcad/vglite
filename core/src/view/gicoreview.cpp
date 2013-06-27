//! \file gicoreview.cpp
//! \brief 实现内核视图类 GiCoreView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#include "gicoreview.h"
#include "giview.h"
#include "gicanvas.h"
#include "testcanvas.h"
#include <stdio.h>
#include <vector>
#include "GcMainView.h"
#include "GcMagnifierView.h"

static int _dpi = 1;

//! 测试视图
class GcDummyView : public GiCoreView
{
public:
    GcDummyView() : _gestureState(kGiGestureCancel) {}
    virtual ~GcDummyView() {}
    
    virtual void drawAll(GiCanvas& canvas);
    virtual bool drawAppend(GiCanvas& canvas);
    virtual void dynDraw(GiCanvas& canvas);
    virtual void onSize(GiView& view, int w, int h);
    virtual bool onGesture(GiView& view, GiGestureType gestureType,
                           GiGestureState gestureState, float x, float y);
    virtual bool twoFingersMove(GiView& view, GiGestureState gestureState,
                                float x1, float y1, float x2, float y2);
private:
    GiGestureState  _gestureState;
};

// GiCoreView
//

GiCoreView* GiCoreView::createView(int type)
{
    if (type == 1) {
        return new GcMainView();
    }
    else if (type == 2 && GcMainView::lastView) {
        return new GcMagnifierView(GcMainView::lastView);
    }
    return new GcDummyView();
}

void GiCoreView::setScreenDpi(int dpi)
{
    _dpi = dpi;
}

// GcDummyView
//

void GcDummyView::drawAll(GiCanvas& canvas)
{
    int n = TestCanvas::randInt(900, 1000);
    TestCanvas::test(canvas, 0x08, n, true);
}

static std::vector<float>   _pts;

static void drawPoints(GiCanvas& canvas)
{
    if (!_pts.empty()) {
        canvas.drawEllipse(_pts[_pts.size() - 2] - 50.f,
                           _pts.back() - 50.f, 100, 100, true, true);
        canvas.beginPath();
        for (size_t i = 0; i + 1 < _pts.size(); i += 2) {
            if (i == 0) {
                canvas.moveTo(_pts[i], _pts[i+1]);
            }
            else {
                canvas.lineTo(_pts[i], _pts[i+1]);
            }
        }
        canvas.drawPath(true, false);
    }
}

bool GcDummyView::drawAppend(GiCanvas& canvas)
{
    canvas.setPen(TestCanvas::randInt(20, 0xFF) << 24 | TestCanvas::randInt(0, 0xFFFFFF),
                  TestCanvas::randFloat(1, 10), -1, 0);
    canvas.setBrush(TestCanvas::randInt(10, 0xFF) << 24 | TestCanvas::randInt(0, 0xFFFFFF), 0);
    drawPoints(canvas);
    _pts.clear();
    return true;
}

void GcDummyView::dynDraw(GiCanvas& canvas)
{
	if (_gestureState == kGiGestureBegan || _gestureState == kGiGestureMoved) {
		static float phase = 0;
		phase -= 1;
		canvas.setPen(0, 0, 1, phase);
		canvas.setBrush(0x80005500, 0);
		drawPoints(canvas);

        float x = _pts[_pts.size() - 2];
        float y = _pts.back();
		char text[40] = "";
#if defined(_MSC_VER) && _MSC_VER >= 1400 // VC8
		sprintf_s(text, sizeof(text), "%.1f, %.1f", x, y);
#else
        sprintf(text, "%.1f, %.1f", x, y);
#endif
		canvas.drawTextAt(text, x < 80 ? 80 : x, y < 80 ? 80 : y - 70, 20, 1);
	}
}

void GcDummyView::onSize(GiView&, int w, int h)
{
    if (w && h) {}
}

bool GcDummyView::onGesture(GiView& view, GiGestureType gestureType,
        GiGestureState gestureState, float x, float y)
{
	_gestureState = gestureState;
    if (gestureState == kGiGestureBegan) {
        _pts.clear();
    }
    _pts.push_back(x);
    _pts.push_back(y);
    
    if (gestureType == kGiGesturePan && gestureState == kGiGestureEnded) {
        view.regenAppend();
    }
    else {
        view.redraw();
    }
    return true;
}

bool GcDummyView::twoFingersMove(GiView& view,
        GiGestureState gestureState, float x1, float y1, float x2, float y2)
{
	_gestureState = gestureState;
    if (gestureState == kGiGestureMoved) {
        x2 = x1; y2 = y1; if (x2 > y2) x2 = x1; // avoid not used
        view.redraw();
    }
    return true;
}
