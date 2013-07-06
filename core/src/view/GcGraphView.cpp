//! \file GcGraphView.cpp
//! \brief 实现主绘图视图类 GcGraphView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#include "GcGraphView.h"
#include <RandomShape.h>

GcShapeView::GcShapeView(GcShapeDoc* doc, GiView *view) : GcBaseView(doc, view)
{
    RandomParam(1000).addShapes(shapes());
}

GcShapeView::~GcShapeView()
{
}

void GcShapeView::drawAll(GiCanvas* canvas)
{
    if (graph()->beginPaint(canvas)) {
        doc()->draw(*graph());
        graph()->endPaint();
    }
}

void GcShapeView::drawAppend(GiCanvas* canvas)
{
}

void GcShapeView::dynDraw(GiCanvas* canvas)
{
}

void GcShapeView::onSize(int dpi, int w, int h)
{
    xform()->setResolution((float)dpi);
    xform()->setWndSize(w, h);
}

bool GcShapeView::onGesture(GiGestureType gestureType,
                            GiGestureState gestureState, float x, float y)
{
    return true;
}

bool GcShapeView::twoFingersMove(GiGestureState gestureState,
                                 float x1, float y1, float x2, float y2)
{
    return true;
}

// GcGraphView
//

GcGraphView::GcGraphView(GcShapeDoc* doc, GiView *view) : GcShapeView(doc, view)
{
}

GcGraphView::~GcGraphView()
{
}
