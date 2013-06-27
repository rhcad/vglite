//! \file gicoreview.h
//! \brief 定义内核视图类 GiCoreView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#ifndef VGLITE_CORE_VIEWDISPATCHER_H
#define VGLITE_CORE_VIEWDISPATCHER_H

class GiView;
class GiCanvas;

typedef enum {                  //!< 手势类型
    kGiGestureUnknown,          //!< 未知的手势
    kGiGesturePan,              //!< 单指滑动
    kGiGestureTap,              //!< 单指单击
    kGiGestureDblTap,           //!< 单指双击
    kGiGesturePress,            //!< 单指长按
    kGiTwoFingersMove,          //!< 双指移动(可放缩旋转)
} GiGestureType;

typedef enum {                  //!< 手势状态
    kGiGesturePossible,         //!< 待检查手势有效性
    kGiGestureBegan,            //!< 开始
    kGiGestureMoved,            //!< 改变
    kGiGestureEnded,            //!< 结束
    kGiGestureCancel,           //!< 取消
} GiGestureState;

//! 内核视图类
/*! 本类拥有图形对象，负责显示和手势动作的分发。
    \ingroup GROUP_VIEW
 */
class GiCoreView
{
public:
    //! 创建内核视图
    static GiCoreView* createView(int type);
    
    //! 设置屏幕的点密度
    static void setScreenDpi(int dpi);
    
    //! 析构函数
    virtual ~GiCoreView() {}
    
    //! 显示所有图形
    virtual void drawAll(GiCanvas& canvas) = 0;

    //! 显示新图形，在 GiView.regenAppend() 后调用
    virtual bool drawAppend(GiCanvas& canvas) = 0;
    
    //! 显示动态图形
    virtual void dynDraw(GiCanvas& canvas) = 0;

    //! 设置视图的宽高
    virtual void onSize(GiView& view, int w, int h) = 0;
    
    //! 传递单指触摸手势消息
    virtual bool onGesture(GiView& view, GiGestureType gestureType,
            GiGestureState gestureState, float x, float y) = 0;

    //! 传递双指移动手势(可放缩旋转)
    virtual bool twoFingersMove(GiView& view, GiGestureState gestureState,
            float x1, float y1, float x2, float y2) = 0;
};

#endif // VGLITE_CORE_VIEWDISPATCHER_H
