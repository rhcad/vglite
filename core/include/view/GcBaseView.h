//! \file GcBaseView.h
//! \brief 定义内核视图基类 GcBaseView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#ifndef VGLITE_CORE_BASEVIEW_H
#define VGLITE_CORE_BASEVIEW_H

#include "gigesture.h"
#include "gigraph.h"

class GiView;
class GiCanvas;
class GcShapeDoc;
class MgShapeDoc;
class MgShapes;

//! 内核视图基类
/*! \ingroup GROUP_VIEW
 */
class GcBaseView
{
public:
    GcBaseView(GcShapeDoc* doc, GiView *view);
    
    //! 析构函数
    virtual ~GcBaseView() {}
    
    //! 返回回调视图对象
    GiView* deviceView() { return _view; }
    
    MgShapeDoc* doc();
    MgShapes* shapes();
    GcShapeDoc* document() { return _doc; }
    
    //! 得到坐标系对象
    GiTransform* xform() { return &_xf; }
    
    //! 得到图形显示对象
    GiGraphics* graph() { return &_gs; }
    
    //! 显示所有图形
    virtual void drawAll(GiCanvas* canvas) = 0;

    //! 显示新图形，在 GiView.regenAppend() 后调用
    virtual void drawAppend(GiCanvas* canvas) = 0;
    
    //! 显示动态图形
    virtual void dynDraw(GiCanvas* canvas) = 0;

    //! 设置视图的宽高
    virtual void onSize(int dpi, int w, int h) = 0;
    
    //! 传递单指触摸手势消息
    virtual bool onGesture(GiGestureType gestureType, 
                           GiGestureState gestureState, float x, float y) = 0;

    //! 传递双指移动手势(可放缩旋转)
    virtual bool twoFingersMove(GiGestureState gestureState,
            float x1, float y1, float x2, float y2) = 0;
private:
    GiView*     _view;
    GcShapeDoc* _doc;
    GiTransform _xf;
    GiGraphics  _gs;
};

#endif // VGLITE_CORE_BASEVIEW_H
