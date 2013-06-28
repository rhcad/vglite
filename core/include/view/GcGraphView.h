//! \file GcGraphView.h
//! \brief 定义主绘图视图类 GcGraphView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#ifndef VGLITE_CORE_MAINVIEW_H
#define VGLITE_CORE_MAINVIEW_H

#include "GcBaseView.h"

//! 主绘图视图类
class GcGraphView : public GcBaseView
{
public:
    GcGraphView(GiView *view);
    virtual ~GcGraphView();
    
    static GcGraphView* lastView;
    
    virtual void drawAll(GiCanvas* canvas);
    virtual bool drawAppend(GiCanvas* canvas);
    virtual void dynDraw(GiCanvas* canvas);
    virtual void onSize(int w, int h);
    virtual bool onGesture(GiGestureType gestureType,
                           GiGestureState gestureState, float x, float y);
    virtual bool twoFingersMove(GiGestureState gestureState,
                                float x1, float y1, float x2, float y2);
};

#endif // VGLITE_CORE_MAINVIEW_H