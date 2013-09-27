//! \file mgdrawlines.h
//! \brief 定义折线绘图命令类 MgCmdDrawLines
// Copyright (c) 2004-2013, Zhang Yungui
// License: LGPL, https://github.com/rhcad/touchvg

#ifndef TOUCHVG_CMD_DRAW_LINES_H_
#define TOUCHVG_CMD_DRAW_LINES_H_

#include "mgcmddraw.h"

//! 折线绘图命令类
/*! \ingroup CORE_COMMAND
    \see MgLines
*/
class MgCmdDrawLines : public MgCommandDraw
{
protected:
    MgCmdDrawLines();
    virtual ~MgCmdDrawLines();
    
public:
    static const char* Name() { return "lines"; }
    static MgCommand* Create() { return new MgCmdDrawLines; }
    
private:
    virtual bool needCheckClosed() { return true; }
    virtual bool needEnded() { return false; }
    
    virtual const char* getName() const { return Name(); }
    virtual void release() { delete this; }
    
    virtual bool initialize(const MgMotion* sender);
    virtual bool undo(const MgMotion* sender);
    virtual bool draw(const MgMotion* sender, GiGraphics* gs);
    virtual bool touchBegan(const MgMotion* sender);
    virtual bool touchMoved(const MgMotion* sender);
    virtual bool touchEnded(const MgMotion* sender);
    virtual bool click(const MgMotion* sender);
    virtual bool doubleClick(const MgMotion* sender);
    virtual bool cancel(const MgMotion* sender);

private:
    bool checkClosed(const MgMotion* sender, const Point2d& pnt);
    bool canAddPoint(const MgMotion* sender, const Point2d& pnt);

private:
    int     m_index;
};

#endif // TOUCHVG_CMD_DRAW_LINES_H_
