//! \file GcMainView.h
//! \brief 定义主绘图视图类 GcMainView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#ifndef VGLITE_CORE_MAINVIEW_H
#define VGLITE_CORE_MAINVIEW_H

#include "gicoreview.h"

//! 主绘图视图类
class GcMainView : public GiCoreView
{
public:
    GcMainView();
    virtual ~GcMainView();
    
    static GcMainView* lastView;
    
    virtual void drawAll(GiCanvas& canvas);
    virtual bool drawAppend(GiCanvas& canvas);
    virtual void dynDraw(GiCanvas& canvas);
    virtual void onSize(GiView& view, int w, int h);
    virtual bool onGesture(GiView& view, GiGestureType gestureType,
                           GiGestureState gestureState, float x, float y);
    virtual bool twoFingersMove(GiView& view, GiGestureState gestureState,
                                float x1, float y1, float x2, float y2);
};

#endif // VGLITE_CORE_MAINVIEW_H
