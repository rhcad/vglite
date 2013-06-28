//! \file GcMagnifierView.cpp
//! \brief 实现放大镜视图类 GcMagnifierView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#include "GcMagnifierView.h"
#include "GcMainView.h"

GcMagnifierView::GcMagnifierView(GiView *view, GcMainView* mainView)
    : GcBaseView(view), _mainView(mainView)
{
}

GcMagnifierView::~GcMagnifierView()
{
}

void GcMagnifierView::drawAll(GiCanvas* canvas)
{
}

bool GcMagnifierView::drawAppend(GiCanvas* canvas)
{
    return true;
}

void GcMagnifierView::dynDraw(GiCanvas* canvas)
{
}

void GcMagnifierView::onSize(int w, int h)
{
}

bool GcMagnifierView::onGesture(GiGestureType gestureType,
                                GiGestureState gestureState, float x, float y)
{
    return true;
}

bool GcMagnifierView::twoFingersMove(GiGestureState gestureState,
                                     float x1, float y1, float x2, float y2)
{
    return true;
}
