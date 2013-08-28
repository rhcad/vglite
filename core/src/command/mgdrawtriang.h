//! \file mgdrawtriang.h
//! \brief 定义三角形绘图命令
// License: LGPL, https://github.com/rhcad/touchvg

#ifndef __GEOMETRY_MGCOMMAND_DRAW_TRIANGLE_H_
#define __GEOMETRY_MGCOMMAND_DRAW_TRIANGLE_H_

#include "mgcmddraw.h"

//! 三角形绘图命令
/*! \ingroup CORE_COMMAND
*/
class MgCmdDrawTriangle : public MgCommandDraw
{
protected:
    MgCmdDrawTriangle();
    virtual ~MgCmdDrawTriangle();
    
public:
    static const char* Name() { return "triangle"; }
    static MgCommand* Create() { return new MgCmdDrawTriangle; }

private:
    virtual const char* getName() const { return Name(); }
    virtual void release() { delete this; }

    virtual bool initialize(const MgMotion* sender);
    virtual bool draw(const MgMotion* sender, GiGraphics* gs);
    virtual bool touchBegan(const MgMotion* sender);
    virtual bool touchMoved(const MgMotion* sender);
    virtual bool touchEnded(const MgMotion* sender);
};

//! 平行四边形绘图命令类
/*! \ingroup CORE_COMMAND
    \see MgParallelogram
*/
class MgCmdParallelogram : public MgCommandDraw
{
protected:
    MgCmdParallelogram() {}
public:
    static const char* Name() { return "parallelogram"; }
    static MgCommand* Create() { return new MgCmdParallelogram; }
private:
    virtual const char* getName() const { return Name(); }
    virtual void release() { delete this; }

    virtual bool initialize(const MgMotion* sender);
    virtual bool touchBegan(const MgMotion* sender) { return _touchBeganStep(sender); }
    virtual bool touchMoved(const MgMotion* sender) { return _touchMovedStep(sender); }
    virtual bool touchEnded(const MgMotion* sender) { return _touchEndedStep(sender); }
};

#endif // __GEOMETRY_MGCOMMAND_DRAW_TRIANGLE_H_
