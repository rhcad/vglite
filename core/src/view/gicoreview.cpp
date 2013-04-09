//! \file gicoreview.cpp
//! \brief 实现内核视图类 GiCoreView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#include "gicoreview.h"
#include "giview.h"
#include <gicanvas.h>
#include <testcanvas.h>
#include <stdio.h>

static float _lastx = 0;
static float _lasty = 0;

GiCoreView::GiCoreView()
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

bool GiCoreView::drawNewShape(GiCanvas& canvas)
{
	canvas.setBrush(0x80005500, 0);
	canvas.drawEllipse(_lastx - 50, _lasty - 50, 100, 100, true, true);
	return true;
}

void GiCoreView::dynDraw(GiCanvas& canvas)
{
    static float phase = 0;
    phase += 1;
    canvas.setPen(0, 0, 1, phase);
    canvas.setBrush(0x80005500, 0);
    canvas.drawEllipse(_lastx - 50, _lasty - 50, 100, 100, true, true);

    char text[40] = "";
    sprintf(text, "%.1f, %.1f", _lastx, _lasty);
    canvas.drawTextAt(text, _lastx < 80 ? 80 : _lastx,
    		_lasty < 80 ? 80 : _lasty - 70, 20, 1);
}

void GiCoreView::onTouch(GiView& view, float x, float y)
{
    _lastx = x;
    _lasty = y;
    view.redraw();
}
