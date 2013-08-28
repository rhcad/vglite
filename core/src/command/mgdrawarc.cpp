// mgdrawarc.cpp: 实现圆弧绘图命令
// License: LGPL, https://github.com/rhcad/touchvg

#include "mgdrawarc.h"
#include <mgshapet.h>
#include <mgbasicsp.h>

// MgCmdArc3P
//

bool MgCmdArc3P::initialize(const MgMotion* sender)
{
    return _initialize(MgShapeT<MgArc>::create, sender);
}

bool MgCmdArc3P::draw(const MgMotion* sender, GiGraphics* gs)
{
    if (m_step > 0 && m_step < 3) {
        gs->drawHandle(_points[m_step], 1);
    }
    if (m_step > 0) {
        GiContext ctx(0, GiColor(0, 126, 0, 64), kGiLineDashDot);
        gs->drawLine(&ctx, _points[0], _points[1]);
        MgArc* arc = (MgArc*)dynshape()->shape();
        gs->drawEllipse(&ctx, arc->getCenter(), arc->getRadius());
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
        arc->offset(pt - arc->getCenter(), -1);
    }
    else if (step == 1) {
        _points[1] = pt;    // 记下起点
        _points[2] = pt;    // 起点与终点重合
        arc->setCenterStartEnd(_points[0], pt); // 初始转角为0
    }
    else if (step == 2) {
        arc->setCenterStartEnd(_points[0], _points[1], pt);
        _points[2] = arc->getEndPoint();    // 记下终点
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
