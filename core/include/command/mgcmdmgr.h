//! \file mgcmdmgr.h
//! \brief 定义命令管理器类 MgCmdManager
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#ifndef VGLITE_CORE_COMMAND_MANAGER_H
#define VGLITE_CORE_COMMAND_MANAGER_H

#include "mgview.h"

//! 命令管理器类
/*! \ingroup GROUP_COMMAND
 */
class MgCmdManager
{
public:
    MgCmdManager();
    ~MgCmdManager();

    //! 显示动态图形
    void dynDraw(const MgMotion& motion, GiGraphics& gs);
    
    //! 传递单指触摸手势消息
    bool onGesture(const MgMotion& motion);

    //! 传递双指移动手势(可放缩旋转)
    bool twoFingersMove(const MgMotion& motion);
};

#endif // VGLITE_CORE_COMMAND_MANAGER_H
