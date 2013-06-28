//! \file GcMagnifierView.h
//! \brief 定义放大镜视图类 GcMagnifierView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#ifndef VGLITE_CORE_MAGNIFIERVIEW_H
#define VGLITE_CORE_MAGNIFIERVIEW_H

#include "GcBaseView.h"

class GcGraphView;

//! 放大镜视图类
class GcMagnifierView : public GcBaseView
{
public:
    GcMagnifierView(GiView *view, GcGraphView* mainView);
    virtual ~GcMagnifierView();
    
    virtual void drawAll(GiCanvas* canvas);
    virtual bool drawAppend(GiCanvas* canvas);
    virtual void dynDraw(GiCanvas* canvas);
    virtual void onSize(int w, int h);
    virtual bool onGesture(GiGestureType gestureType,
                           GiGestureState gestureState, float x, float y);
    virtual bool twoFingersMove(GiGestureState gestureState,
                                float x1, float y1, float x2, float y2);
    
private:
    GcGraphView*     _mainView;
};

#endif // VGLITE_CORE_MAGNIFIERVIEW_H
