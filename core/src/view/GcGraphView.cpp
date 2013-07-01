//! \file GcGraphView.cpp
//! \brief 实现主绘图视图类 GcGraphView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#include "GcGraphView.h"

GcShapeView::GcShapeView(GiView *view) : GcBaseView(view)
{
}

GcShapeView::~GcShapeView()
{
}

void GcShapeView::drawAll(GiCanvas* canvas)
{
}

bool GcShapeView::drawAppend(GiCanvas* canvas)
{
    return true;
}

void GcShapeView::dynDraw(GiCanvas* canvas)
{
}

void GcShapeView::onSize(int w, int h)
{
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

GcGraphView* GcGraphView::lastView = (GcGraphView*)0;

GcGraphView::GcGraphView(GiView *view) : GcShapeView(view)
{
    lastView = this;
}

GcGraphView::~GcGraphView()
{
    if (lastView == this) {
        lastView = (GcGraphView*)0;
    }
}
