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
    GiCoreView();
    ~GiCoreView();
    
    //! 显示所有图形
    void drawAll(GiCanvas& canvas);

    //! 显示新图形，在 GiView.regenAppend() 后调用
    bool drawAppend(GiCanvas& canvas);
    
    //! 显示动态图形
    void dynDraw(GiCanvas& canvas);
    
    //! 传递单指触摸手势消息
    bool onGesture(GiView& view, GiGestureType gestureType,
            GiGestureState gestureState, float x, float y);

    //! 传递双指移动手势(可放缩旋转)
    bool twoFingersMove(GiView& view, GiGestureState gestureState,
            float x1, float y1, float x2, float y2);

private:
    GiGestureState  _gestureState;
};

#endif // VGLITE_CORE_VIEWDISPATCHER_H
