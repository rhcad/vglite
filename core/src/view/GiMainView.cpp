//! \file GiMainView.cpp
//! \brief 实现主绘图视图类 GiMainView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#include "GiMainView.h"

GiMainView* GiMainView::lastView = (GiMainView*)0;

GiMainView::GiMainView()
{
    lastView = this;
}

GiMainView::~GiMainView()
{
    if (lastView == this) {
        lastView = (GiMainView*)0;
    }
}

void GiMainView::drawAll(GiCanvas& canvas)
{
}

bool GiMainView::drawAppend(GiCanvas& canvas)
{
    return true;
}

void GiMainView::dynDraw(GiCanvas& canvas)
{
}

void GiMainView::onSize(GiView& view, int w, int h)
{
}

bool GiMainView::onGesture(GiView& view, GiGestureType gestureType,
                           GiGestureState gestureState, float x, float y)
{
    return true;
}

bool GiMainView::twoFingersMove(GiView& view, GiGestureState gestureState,
                                float x1, float y1, float x2, float y2)
{
    return true;
}
