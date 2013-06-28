//! \file GcGraphView.cpp
//! \brief 实现主绘图视图类 GcGraphView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#include "GcGraphView.h"

GcGraphView* GcGraphView::lastView = (GcGraphView*)0;

GcGraphView::GcGraphView(GiView *view) : GcBaseView(view)
{
    lastView = this;
}

GcGraphView::~GcGraphView()
{
    if (lastView == this) {
        lastView = (GcGraphView*)0;
    }
}

void GcGraphView::drawAll(GiCanvas* canvas)
{
}

bool GcGraphView::drawAppend(GiCanvas* canvas)
{
    return true;
}

void GcGraphView::dynDraw(GiCanvas* canvas)
{
}

void GcGraphView::onSize(int w, int h)
{
}

bool GcGraphView::onGesture(GiGestureType gestureType,
                            GiGestureState gestureState, float x, float y)
{
    return true;
}

bool GcGraphView::twoFingersMove(GiGestureState gestureState,
                                 float x1, float y1, float x2, float y2)
{
    return true;
}
