//! \file gicoreview.cpp
//! \brief 实现内核视图类 GiCoreView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#include "gicoreview.h"
#include "giview.h"
#include <gicanvas.h>
#include <testcanvas.h>

static float _lastx = 0;
static float _lasty = 0;

GiCoreView::GiCoreView()
{
}

GiCoreView::~GiCoreView()
{
}

void GiCoreView::draw(GiCanvas& canvas)
{
    TestCanvas::test(canvas, 0x08, 1000, true);
}

void GiCoreView::dyndraw(GiCanvas& canvas)
{
    static float phase = 0;
    phase += 1;
    canvas.setPen(0, 0, 1, phase);
    canvas.setBrush(0x10005500, 0);
    for (int i = 0; i < 300; i++) {
        canvas.drawEllipse(_lastx - 50, _lasty - 50, 100, 100, true, true);
    }
}

void GiCoreView::onTouch(GiView& view, float x, float y)
{
    _lastx = x;
    _lasty = y;
    view.redraw();
}
