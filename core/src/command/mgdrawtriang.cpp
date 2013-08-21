// mgdrawtriang.cpp: 实现三角形绘图命令
// Author: pengjun, 2012.6.4
// License: LGPL, https://github.com/rhcad/touchvg

#include "mgdrawtriang.h"
#include <mgshapet.h>
#include <mgbasicsp.h>
#include <mgbase.h>
#include <stdio.h>
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
    float distmin = mgDisplayMmToModel(2.f, sender);
    
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

// MgCmdArc3P
//

bool MgCmdArc3P::initialize(const MgMotion* sender)
{
    return _initialize(MgShapeT<MgArc>::create, sender);
}

bool MgCmdArc3P::draw(const MgMotion* sender, GiGraphics* gs)
{
    if (getStep() > 0) {
        for (int i = 0; i <= m_step && i < 3; i++) {
            gs->drawHandle(_points[i], 1);
        }
        if (m_step >= 1) {
            GiContext ctx(0, GiColor(0, 126, 0, 64), kGiLineDashDot);
            gs->drawLine(&ctx, _points[0], _points[1]);
            MgArc* arc = (MgArc*)dynshape()->shape();
            gs->drawEllipse(&ctx, arc->getCenter(), arc->getRadius());
        }
    }
    return _draw(sender, gs);
}

void MgCmdArc3P::setStepPoint(int step, const Point2d& pt)
{
    MgArc* arc = (MgArc*)dynshape()->shape();

    if (step == 0) {
        _points[0] = pt;
    }
    else if (step == 1) {
        _points[1] = pt;
        _points[2] = pt;
        arc->setStartMidEnd(_points[0], pt, pt);
    }
    else if (step == 2) {
        _points[2] = pt;
        arc->setStartMidEnd(_points[0], _points[1], pt);
    }
}

void MgCmdArcCSE::setStepPoint(int step, const Point2d& pt)
{
    MgArc* arc = (MgArc*)dynshape()->shape();

    if (step == 0) {
        _points[0] = pt;    // 记下圆心
    }
    else if (step == 1) {
        _points[1] = pt;    // 记下起点
        _points[2] = pt;    // 起点与终点重合
        arc->setCenterStartEnd(_points[0], pt, pt); // 初始转角为0
    }
    else if (step == 2) {
        arc->setCenterStartEnd(_points[0], _points[1], pt);
        _points[2] = pt;    // 记下终点
    }
}

void MgCmdArcTan::setStepPoint(int step, const Point2d& pt)
{
    MgArc* arc = (MgArc*)dynshape()->shape();

    if (step == 0) {
        _points[0] = pt;
    }
    else if (step == 1) {
        _points[1] = pt;
        _points[2] = pt;
        arc->setTanStartEnd(_points[1] - _points[0], pt, pt);
    }
    else if (step == 2) {
        arc->setTanStartEnd(_points[1] - _points[0], _points[1], pt);
        _points[2] = pt;
    }
}
