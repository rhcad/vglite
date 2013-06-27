//! \file GiMagnifierView.h
//! \brief 定义放大镜视图类 GiMagnifierView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#ifndef VGLITE_CORE_MAGNIFIERVIEW_H
#define VGLITE_CORE_MAGNIFIERVIEW_H

#include "gicoreview.h"

class GiMainView;

//! 放大镜视图类
class GiMagnifierView : public GiCoreView
{
public:
    GiMagnifierView(GiMainView* mainView);
    virtual ~GiMagnifierView();
    
    virtual void drawAll(GiCanvas& canvas);
    virtual bool drawAppend(GiCanvas& canvas);
    virtual void dynDraw(GiCanvas& canvas);
    virtual void onSize(GiView& view, int w, int h);
    virtual bool onGesture(GiView& view, GiGestureType gestureType,
                           GiGestureState gestureState, float x, float y);
    virtual bool twoFingersMove(GiView& view, GiGestureState gestureState,
                                float x1, float y1, float x2, float y2);
    
private:
    GiMainView*     _mainView;
};

#endif // VGLITE_CORE_MAGNIFIERVIEW_H
