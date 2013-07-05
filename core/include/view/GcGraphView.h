//! \file GcGraphView.h
//! \brief 定义主绘图视图类 GcGraphView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#ifndef VGLITE_CORE_MAINVIEW_H
#define VGLITE_CORE_MAINVIEW_H

#include "GcBaseView.h"
#include "mgshapedoc.h"

//! 内核绘图视图类
/*! \ingroup GROUP_VIEW
 */
class GcShapeView : public GcBaseView
{
public:
    GcShapeView(GiView *view);
    virtual ~GcShapeView();
    
    virtual void drawAll(GiCanvas* canvas);
    virtual void drawAppend(GiCanvas* canvas);
    virtual void dynDraw(GiCanvas* canvas);
    virtual void onSize(int dpi, int w, int h);
    virtual bool onGesture(GiGestureType gestureType,
                           GiGestureState gestureState, float x, float y);
    virtual bool twoFingersMove(GiGestureState gestureState,
                                float x1, float y1, float x2, float y2);
};

//! 主绘图视图类
/*! \ingroup GROUP_VIEW
 */
class GcGraphView : public GcShapeView
{
public:
    GcGraphView(GiView *view);
    virtual ~GcGraphView();
};

#endif // VGLITE_CORE_MAINVIEW_H
