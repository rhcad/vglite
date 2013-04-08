//! \file giview.h
//! \brief 定义设备相关的视图接口 GiView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#ifndef VGLITE_CORE_GIVIEW_H
#define VGLITE_CORE_GIVIEW_H

//! 设备相关的视图接口
/*! 在派生类中使用某种界面框架实现其视图函数
    \ingroup GROUP_VIEW
 */
class GiView
{
public:
    virtual ~GiView() {}

    //! 标记视图待重新构建显示
    virtual void regenAll() = 0;

    //! 标记视图待追加显示新图形
    virtual void regenAfterAddShape() = 0;

    //! 标记视图待更新显示
    virtual void redraw() = 0;
};

#endif // VGLITE_CORE_GIVIEW_H
