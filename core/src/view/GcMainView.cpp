//! \file GcMainView.cpp
//! \brief 实现主绘图视图类 GcMainView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#include "GcMainView.h"

GcMainView* GcMainView::lastView = (GcMainView*)0;

GcMainView::GcMainView()
{
    lastView = this;
}

GcMainView::~GcMainView()
{
    if (lastView == this) {
        lastView = (GcMainView*)0;
    }
}

void GcMainView::drawAll(GiCanvas& canvas)
{
}

bool GcMainView::drawAppend(GiCanvas& canvas)
{
    return true;
}

void GcMainView::dynDraw(GiCanvas& canvas)
{
}

void GcMainView::onSize(GiView& view, int w, int h)
{
}

bool GcMainView::onGesture(GiView& view, GiGestureType gestureType,
                           GiGestureState gestureState, float x, float y)
{
    return true;
}

bool GcMainView::twoFingersMove(GiView& view, GiGestureState gestureState,
                                float x1, float y1, float x2, float y2)
{
    return true;
}
