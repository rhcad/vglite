//! \file mgview.h
//! \brief 定义视图回调接口 MgView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#ifndef VGLITE_CORE_COMMAND_VIEW_H
#define VGLITE_CORE_COMMAND_VIEW_H

//! 视图回调接口
/*! \ingroup GROUP_COMMAND
 */
class MgView
{
public:
    virtual ~MgView() {}
    
    //! 标记视图待重新构建显示
    virtual void regenAll() = 0;
    
    //! 标记视图待追加显示新图形
    virtual void regenAppend() = 0;
    
    //! 标记视图待更新显示
    virtual void redraw() = 0;
};

#endif // VGLITE_CORE_COMMAND_VIEW_H
