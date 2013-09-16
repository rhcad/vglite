//! \file tradecmd.h
//! \brief 定义行业应用功能类 TradeCmd
// Copyright (c) 2012-2013, https://github.com/rhcad/touchvg

#ifndef __TOUCHVG_TRADECMD_H_
#define __TOUCHVG_TRADECMD_H_

class MgShape;
class MgMotion;
class GiGraphics;
class MgCommandDraw;

//! 行业应用功能类
/*! \ingroup CORE_COMMAND
 */
struct TradeCmd {
    //! 图形文档内容加载后的通知
    static void onDocLoaded(const MgMotion* sender);

    //! 进入选择命令时的通知
    static void onEnterSelectCommand(const MgMotion* sender);

    //! 视图销毁前、所有命令卸载后的通知
    static void onUnloadCommands();

    //! 选择命令中的上下文操作是否隐藏的回调通知
    static bool selectActionsNeedHided(const MgMotion* sender);

    //! 对选中的图形增加上下文操作的回调通知
    static void addShapeActions(int* actions, int &n, const MgShape* shape);

    //! 执行非内置上下文操作的通知
    static bool doAction(const MgMotion* sender, int action);

    //! 在非绘图的命令中执行额外的上下文操作的通知
    static bool doEndAction(const MgMotion* sender, int action);

    //! 在绘图命令中创建了图形的通知
    static void onShapeAdded(const MgMotion* sender, MgShape* shape);

    //! 在绘图命令中显示额外内容的通知
    static void drawInShapeCommand(const char* name, 
        MgCommandDraw* cmd, GiGraphics* gs);

    //! 在选择命令中显示额外内容的通知
    static void drawInSelectCommand(const MgShape* shape, int handleIndex,
        const MgMotion* sender, GiGraphics* gs);

    //! 在选择命令中拖放图形后的通知
    static void onSelectTouchEnded(const MgMotion* sender, int shapeid,
        int handleIndex, int snapid, int snapHandle,
        int count, const int* ids);
};

#endif // __TOUCHVG_TRADECMD_H_
