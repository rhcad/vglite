//! \file giview.h
//! \brief 定义设备相关的视图接口 GiView
// Copyright (c) 2012-2013, https://github.com/rhcad/touchvg

#ifndef TOUCHVG_CORE_GIVIEW_H
#define TOUCHVG_CORE_GIVIEW_H

//! 设备相关的视图回调接口
/*! 在派生类中使用某种界面框架实现其视图回调函数
    \ingroup CORE_VIEW
 */
class GiView
{
public:
    virtual ~GiView() {}

    //! 标记视图待重新构建显示
    virtual void regenAll() = 0;

    //! 标记视图待追加显示新图形
    virtual void regenAppend() = 0;

    //! 标记视图待更新显示
    virtual void redraw() = 0;

    //! 使用手指(true)或鼠标(false)交互
    virtual bool useFinger() { return true; }
};

#endif // TOUCHVG_CORE_GIVIEW_H
