//! \file mgview.h
//! \brief 定义视图回调接口 MgView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#ifndef VGLITE_CORE_COMMAND_VIEW_H
#define VGLITE_CORE_COMMAND_VIEW_H

#include "gigraph.h"
#include "mgshapedoc.h"

struct MgCmdManager;
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
    virtual MgCmdManager* cmds() = 0;       //!< 返回命令管理器对象
    virtual GiTransform* xform() = 0;           //!< 得到坐标系对象
    virtual GiGraphics* graph() = 0;            //!< 得到图形显示对象
    virtual MgShapeDoc* doc() = 0;              //!< 得到图形文档
    virtual MgShapes* shapes() = 0;             //!< 得到图形列表
    virtual GiContext* context() = 0;           //!< 得到当前绘图属性
    
    virtual void regenAll() = 0;                //!< 标记视图待重新构建显示
    virtual void regenAppend() = 0;             //!< 标记视图待追加显示新图形
    virtual void redraw() = 0;                  //!< 标记视图待更新显示
    
    virtual bool useFinger() = 0;               //!< 使用手指或鼠标交互
    virtual void selChanged() = 0;              //!< 选择集改变的通知
    
    virtual bool shapeWillAdded(MgShape* shape) = 0;    //!< 通知将添加图形
    virtual void shapeAdded(MgShape* shape) = 0;        //!< 通知已添加图形，由视图重新构建显示
    virtual bool shapeWillDeleted(MgShape* shape) = 0;  //!< 通知将删除图形
    virtual bool removeShape(MgShape* shape) = 0;       //!< 删除图形
    virtual bool shapeCanRotated(MgShape* shape) = 0;   //!< 通知是否能旋转图形
    virtual bool shapeCanTransform(MgShape* shape) = 0; //!< 通知是否能对图形变形
    virtual void shapeMoved(MgShape* shape, int segment) = 0;   //!< 通知图形已拖动
    virtual void commandChanged() = 0;                  //!< 命令改变
    
    virtual bool isContextActionsVisible() = 0;         //!< 返回上下文菜单是否已显示
#ifndef SWIG
    virtual bool showContextActions(int selState, const int* actions,
            const Box2d& selbox, const MgShape* shape) = 0; //!< 显示上下文菜单
#endif
};

//! 触摸动作参数
/*! \ingroup GROUP_COMMAND
 */
class MgMotion
{
public:
    MgView*         view;
    int             gestureType;
    MgGestureState  gestureState;
    bool            pressDrag;
    bool            switchGesture;
    Point2d         startPt;
    Point2d         startPtM;
    Point2d         lastPt;
    Point2d         lastPtM;
    Point2d         point;
    Point2d         pointM;
    Point2d         startPt2;
    Point2d         startPt2M;
    Point2d         point2;
    Point2d         point2M;
    
    MgMotion() : view(NULL), gestureType(0), gestureState(kMgGesturePossible)
        , pressDrag(false), switchGesture(false) {}
    
    bool dragging() const {                             //!< 是否正按下拖动
        return gestureState >= kMgGestureBegan && gestureState <= kMgGestureMoved;
    }
    
    MgCmdManager* cmds() const { return view->cmds(); }   //!< 返回命令管理器对象
    Point2d startCenterM() const { return (startPtM + startPt2M) / 2; }
    Point2d centerM() const { return (pointM + point2M) / 2; }
    float startDistanceM() const { return startPtM.distanceTo(startPt2M); }
    float distanceM() const { return pointM.distanceTo(point2M); }
};

#endif // VGLITE_CORE_COMMAND_VIEW_H
