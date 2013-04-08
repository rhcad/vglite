//! \file gicoreview.h
//! \brief 定义内核视图类 GiCoreView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#ifndef VGLITE_CORE_VIEWDISPATCHER_H
#define VGLITE_CORE_VIEWDISPATCHER_H

class GiView;
class GiCanvas;

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

    //! 显示新图形，在 GiView.regenAfterAddShape() 后调用
    void drawNewShape(GiCanvas& canvas);
    
    //! 显示动态图形
    void dynDraw(GiCanvas& canvas);
    
    void onTouch(GiView& view, float x, float y);
};

#endif // VGLITE_CORE_VIEWDISPATCHER_H
