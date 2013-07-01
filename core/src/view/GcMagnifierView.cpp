//! \file GcMagnifierView.cpp
//! \brief 实现放大镜视图类 GcMagnifierView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#include "GcMagnifierView.h"
#include "GcGraphView.h"

GcMagnifierView::GcMagnifierView(GiView *view, GcGraphView* mainView)
    : GcShapeView(view), _mainView(mainView)
{
}

GcMagnifierView::~GcMagnifierView()
{
}
