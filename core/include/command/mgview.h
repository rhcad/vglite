//! \file mgview.h
//! \brief 定义视图回调接口 MgView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#ifndef VGLITE_CORE_COMMAND_VIEW_H
#define VGLITE_CORE_COMMAND_VIEW_H

#include "gigraph.h"
#include "mgshapedoc.h"

class MgCmdManager;
class GcShapeDoc;

typedef enum {                  //!< 手势状态
    kMgGesturePossible,         //!< 待检查手势有效性
    kMgGestureBegan,            //!< 开始
    kMgGestureMoved,            //!< 改变
    kMgGestureEnded,            //!< 结束
    kMgGestureCancel,           //!< 取消
} MgGestureState;

//! 视图回调接口
/*! \ingroup GROUP_COMMAND
 */
class MgView
{
public:
    virtual ~MgView() {}
    
    virtual GcShapeDoc* document() = 0;
    virtual MgCmdManager* cmds() = 0;           //!< 返回命令管理器对象
    virtual GiTransform* xform() = 0;           //!< 得到坐标系对象
    virtual GiGraphics* graph() = 0;            //!< 得到图形显示对象
    virtual MgShapeDoc* doc() = 0;              //!< 得到图形文档
    virtual MgShapes* shapes() = 0;             //!< 得到图形列表
    virtual GiContext* context() = 0;           //!< 得到当前绘图属性
    
    virtual void regenAll() = 0;                //!< 标记视图待重新构建显示
    virtual void regenAppend() = 0;             //!< 标记视图待追加显示新图形
    virtual void redraw() = 0;                  //!< 标记视图待更新显示
    
    virtual bool useFinger() = 0;               //!< 使用手指或鼠标交互
    virtual MgGestureState gestureState() = 0;  //!< 返回当前手势状态
};

#endif // VGLITE_CORE_COMMAND_VIEW_H
