//! \file GiMagnifierView.cpp
//! \brief 实现放大镜视图类 GiMagnifierView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#include "GiMagnifierView.h"
#include "GiMainView.h"

GiMagnifierView::GiMagnifierView(GiMainView* mainView) : _mainView(mainView)
{
}

GiMagnifierView::~GiMagnifierView()
{
}

void GiMagnifierView::drawAll(GiCanvas& canvas)
{
}

bool GiMagnifierView::drawAppend(GiCanvas& canvas)
{
    return true;
}

void GiMagnifierView::dynDraw(GiCanvas& canvas)
{
}

void GiMagnifierView::onSize(GiView& view, int w, int h)
{
}

bool GiMagnifierView::onGesture(GiView& view, GiGestureType gestureType,
                           GiGestureState gestureState, float x, float y)
{
    return true;
}

bool GiMagnifierView::twoFingersMove(GiView& view, GiGestureState gestureState,
                                float x1, float y1, float x2, float y2)
{
    return true;
}
