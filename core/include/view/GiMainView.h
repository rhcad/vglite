//! \file GiMainView.h
//! \brief 定义主绘图视图类 GiMainView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#ifndef VGLITE_CORE_MAINVIEW_H
#define VGLITE_CORE_MAINVIEW_H

#include "gicoreview.h"

//! 主绘图视图类
class GiMainView : public GiCoreView
{
public:
    GiMainView();
    virtual ~GiMainView();
    
    static GiMainView* lastView;
    
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
