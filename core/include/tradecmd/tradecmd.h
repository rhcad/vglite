//! \file tradecmd.h
//! \brief 定义行业应用功能类 TradeCmd
// Copyright (c) 2012-2013, https://github.com/rhcad/touchvg

#ifndef __TRADECMD_H_
#define __TRADECMD_H_

#include "mgcmddraw.h"

//! 行业应用功能类
/*! \ingroup CORE_COMMAND
 */
struct TradeCmd {
    static void onUnloadCmds();
    static bool selectActionHided(const MgMotion* sender);
    static void addShapeActions(int* actions, int &n, const MgShape* shape);
    static bool doAction(const MgMotion* sender, int action);
    static bool doEndAction(const MgMotion* sender, int action);
    static void drawInTriangle(MgCommandDraw* cmd, GiGraphics* gs);
    static void onLineAdded(const MgMotion* sender, MgShape* shape);
    static void onInitSelCmd(const MgMotion* sender);
    static void drawInSelect(const MgShape* shape, int handleIndex,
                             const MgMotion* sender, GiGraphics* gs, bool forceDraw = false);
    static void onSelectTouchEnded(const MgMotion* sender, int shapeid,
                                   int handleIndex, int snapid, int snapHandle,
                                   int count, const int* ids);
};

#endif // __TRADECMD_H_
