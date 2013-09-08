// mgdrawtriang.cpp: 实现三角形绘图命令
// License: LGPL, https://github.com/rhcad/touchvg

#include "mgdrawtriang.h"
#include <mgshapet.h>
#include <mgbasicsp.h>
#include <mgbase.h>
#include "tradecmd.h"

MgCmdDrawTriangle::MgCmdDrawTriangle()
{
}

MgCmdDrawTriangle::~MgCmdDrawTriangle()
{
}

bool MgCmdDrawTriangle::initialize(const MgMotion* sender)
{
    return _initialize(MgShapeT<MgLines>::create, sender);
}

bool MgCmdDrawTriangle::draw(const MgMotion* sender, GiGraphics* gs)
{
    TradeCmd::drawInTriangle(this, gs);
    return MgCommandDraw::draw(sender, gs);
}

bool MgCmdDrawTriangle::touchBegan(const MgMotion* sender)
{
    MgBaseLines* lines = (MgBaseLines*)dynshape()->shape();
    
    if (0 == m_step) {
        m_step = 1;
        lines->setClosed(true);
        lines->resize(3);
        Point2d pnt(snapPoint(sender, true));
        for (int i = 0; i < 3; i++) {
            dynshape()->shape()->setPoint(i, pnt);
        }
    }
    else {
        dynshape()->shape()->setPoint(m_step, snapPoint(sender));
    }
    
    dynshape()->shape()->update();

    return _touchBegan(sender);
}

bool MgCmdDrawTriangle::touchMoved(const MgMotion* sender)
{
    dynshape()->shape()->setPoint(m_step, snapPoint(sender));
    dynshape()->shape()->update();

    return _touchMoved(sender);
}

bool MgCmdDrawTriangle::touchEnded(const MgMotion* sender)
{
    Point2d pnt(snapPoint(sender));
    float distmin = displayMmToModel(2.f, sender);
    
    dynshape()->shape()->setPoint(m_step, pnt);
    dynshape()->shape()->update();
    
    if (pnt.distanceTo(dynshape()->shape()->getPoint(m_step - 1)) > distmin) {
        m_step++;
        if (3 == m_step) {
            _addshape(sender);
            _delayClear();
            m_step = 0;
        }
    }

    return _touchEnded(sender);
}

// MgCmdParallelogram
//

bool MgCmdParallelogram::initialize(const MgMotion* sender)
{
    return _initialize(MgShapeT<MgParallelogram>::create, sender);
}
