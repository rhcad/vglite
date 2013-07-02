//! \file mgview.h
//! \brief 定义视图回调接口 MgView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#ifndef VGLITE_CORE_COMMAND_VIEW_H
#define VGLITE_CORE_COMMAND_VIEW_H

class GiView;
class GcShapeDoc;
class MgCmdManager;

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
    
    //! 返回当前回调视图对象
    virtual GiView* currentView() = 0;
    
    //! 返回当前手势状态
    virtual MgGestureState gestureState() = 0;
    
    //! 返回图形文档对象
    virtual GcShapeDoc* doc() = 0;
    
    //! 返回命令管理器对象
    virtual MgCmdManager* cmds() = 0;
    
    //! 标记视图待重新构建显示
    virtual void regenAll() = 0;
    
    //! 标记视图待追加显示新图形
    virtual void regenAppend() = 0;
    
    //! 标记视图待更新显示
    virtual void redraw() = 0;
};

#endif // VGLITE_CORE_COMMAND_VIEW_H
