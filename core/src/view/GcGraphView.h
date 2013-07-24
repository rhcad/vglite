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
    GcShapeView(MgView* mgview, GiView *view);
    virtual ~GcShapeView();
    
    virtual int drawAll(GiGraphics& gs);
    virtual int drawAppend(const int* newids, GiGraphics& gs);
    virtual void dynDraw(const MgMotion& motion, GiGraphics& gs);
    virtual void onSize(int dpi, int w, int h);
    virtual bool onGesture(const MgMotion& motion);
    virtual bool twoFingersMove(const MgMotion& motion);

private:
    Point2d     _lastCenter;
    float       _lastScale;
};

//! 主绘图视图类
/*! \ingroup GROUP_VIEW
 */
class GcGraphView : public GcShapeView
{
public:
    GcGraphView(MgView* mgview, GiView *view);
    virtual ~GcGraphView();
};

#endif // VGLITE_CORE_MAINVIEW_H
