//! \file mgaction.h
//! \brief 定义上下文动作接口 MgActionDispatcher
// Copyright (c) 2004-2012, Zhang Yungui
// License: LGPL, https://github.com/rhcad/touchvg

#ifndef __GEOMETRY_MGACTION_H_
#define __GEOMETRY_MGACTION_H_

//! 默认上下文动作
typedef enum {
    kMgActionInvalid,
    kMgActionSelAll,            //!< 全选
    kMgActionSelReset,          //!< 重选
    kMgActionDraw,              //!< 绘图
    kMgActionCancel,            //!< 取消
    kMgActionDelete,            //!< 删除
    kMgActionClone,             //!< 克隆
    kMgActionBreak,             //!< 剪开
    kMgActionCorner,            //!< 角标
    kMgActionFixedLength,       //!< 定长
    kMgActionFreeLength,        //!< 不定长
    kMgActionLocked,            //!< 锁定
    kMgActionUnlocked,          //!< 解锁
    kMgActionEditVertex,        //!< 编辑顶点
    kMgActionHideVertex,        //!< 隐藏顶点
    kMgActionClosed,            //!< 闭合
    kMgActionOpened,            //!< 不闭合
    kMgActionAddVertex,         //!< 加点
    kMgActionDelVertex,         //!< 删点
    kMgActionGroup,             //!< 成组
    kMgActionUngroup,           //!< 解组
    kMgActionOverturn,          //!< 翻转
    kMgAction3Views,            //!< 三视图
    kMgActionCustomized = 100   //!< 定制的起始值
} MgContextAction;

class MgMotion;
class MgShape;

//! 上下文动作分发接口
/*! \ingroup CORE_COMMAND
    \interface MgActionDispatcher
    \see MgCmdManager, MgContextAction
*/
struct MgActionDispatcher {
    virtual bool showInSelect(const MgMotion* sender, int selState, const MgShape* shape, const Box2d& selbox) = 0;
    virtual bool showInDrawing(const MgMotion* sender, const MgShape* shape) = 0;
    virtual bool doAction(const MgMotion* sender, int action) = 0;
};

#endif // __GEOMETRY_MGACTION_H_
