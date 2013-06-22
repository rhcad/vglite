//! \file gicoreview.cpp
//! \brief 实现内核视图类 GiCoreView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#include "gicoreview.h"
#include "giview.h"
#include "gicanvas.h"
#include "testcanvas.h"
#include <stdio.h>
#include <vector>

static std::vector<float>   _pts;

GiCoreView::GiCoreView() : _gestureState(kGiGestureCancel)
{
}

GiCoreView::~GiCoreView()
{
}

void GiCoreView::drawAll(GiCanvas& canvas)
{
    int n = TestCanvas::randInt(900, 1000);
    TestCanvas::test(canvas, 0x08, n, true);
}

static void drawPoints(GiCanvas& canvas)
{
    if (!_pts.empty()) {
        canvas.drawEllipse(_pts[_pts.size() - 2] - 50,
                           _pts.back() - 50, 100, 100, true, true);
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

bool GiCoreView::drawAppend(GiCanvas& canvas)
{
    canvas.setPen(TestCanvas::randInt(20, 0xFF) << 24 | TestCanvas::randInt(0, 0xFFFFFF),
                  TestCanvas::randFloat(1, 10), -1, 0);
    canvas.setBrush(TestCanvas::randInt(10, 0xFF) << 24 | TestCanvas::randInt(0, 0xFFFFFF), 0);
    drawPoints(canvas);
    _pts.clear();
    return true;
}

void GiCoreView::dynDraw(GiCanvas& canvas)
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
		sprintf(text, "%.1f, %.1f", x, y);
		canvas.drawTextAt(text, x < 80 ? 80 : x, y < 80 ? 80 : y - 70, 20, 1);
	}
}

void GiCoreView::setScreenDpi(int dpi)
{
    if (dpi) {}
}

void GiCoreView::onSize(GiView&, int w, int h)
{
    if (w && h) {}
}

bool GiCoreView::onGesture(GiView& view, GiGestureType gestureType,
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

bool GiCoreView::twoFingersMove(GiView& view,
        GiGestureState gestureState, float x1, float y1, float x2, float y2)
{
	_gestureState = gestureState;
    if (gestureState == kGiGestureMoved) {
        x2 = x1; y2 = y1;
        x2 = y2; y2 = x2;
        view.redraw();
    }
    return true;
}
