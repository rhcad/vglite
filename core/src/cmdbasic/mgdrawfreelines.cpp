// mgdrawfreelines.cpp: 实现自由折线绘图命令类 MgCmdDrawFreeLines
// Copyright (c) 2004-2013, Zhang Yungui
// License: LGPL, https://github.com/rhcad/touchvg

#include "mgdrawfreelines.h"
#include <mgshapet.h>
#include <mgbasicsp.h>

MgCmdDrawFreeLines::MgCmdDrawFreeLines()
{
}

MgCmdDrawFreeLines::~MgCmdDrawFreeLines()
{
}

bool MgCmdDrawFreeLines::initialize(const MgMotion* sender)
{
    return _initialize(MgShapeT<MgLines>::create, sender);
}

bool MgCmdDrawFreeLines::undo(const MgMotion* sender)
{
    if (m_step > 2) {                   // 去掉倒数第二个点，倒数第一点是临时动态点
        ((MgBaseLines*)dynshape()->shape())->removePoint(m_step - 1);
        dynshape()->shape()->update();
    }
    return MgCommandDraw::_undo(sender);
}

bool MgCmdDrawFreeLines::draw(const MgMotion* sender, GiGraphics* gs)
{
    return MgCommandDraw::draw(sender, gs);
}

bool MgCmdDrawFreeLines::touchBegan(const MgMotion* sender)
{
    ((MgBaseLines*)dynshape()->shape())->resize(2);
    m_step = 1;
    dynshape()->shape()->setPoint(0, sender->startPtM);
    dynshape()->shape()->setPoint(1, sender->pointM);
    dynshape()->shape()->update();

    return _touchBegan(sender);
}

bool MgCmdDrawFreeLines::touchMoved(const MgMotion* sender)
{
    MgBaseLines* lines = (MgBaseLines*)dynshape()->shape();
    
    float closelen  = sender->displayMmToModel(5.f);
    float closedist = sender->pointM.distanceTo(dynshape()->shape()->getPoint(0));
    bool  closed    = (m_step > 2 && closedist < closelen
        && dynshape()->shape()->getExtent().width() > closedist * 1.5f
        && dynshape()->shape()->getExtent().height() > closedist * 1.5f);
    
    if (m_step > 2 && dynshape()->shape()->isClosed() != closed) {
        lines->setClosed(closed);
        if (closed)
            lines->removePoint(m_step);
        else
            lines->addPoint(sender->pointM);
    }
    if (!closed) {
        dynshape()->shape()->setPoint(m_step, sender->pointM);
        if (m_step > 0 && canAddPoint(sender, false)) {
            m_step++;
            if (m_step >= dynshape()->shape()->getPointCount()) {
                ((MgBaseLines*)dynshape()->shape())->addPoint(sender->pointM);
            }
        }
    }
    dynshape()->shape()->update();

    return _touchMoved(sender);
}

bool MgCmdDrawFreeLines::touchEnded(const MgMotion* sender)
{
    MgBaseLines* lines = (MgBaseLines*)dynshape()->shape();
    
    float closelen  = sender->displayMmToModel(5.f);
    float closedist = sender->pointM.distanceTo(dynshape()->shape()->getPoint(0));
    bool  closed    = (m_step > 2 && closedist < closelen
        && dynshape()->shape()->getExtent().width() > closedist * 1.5f
        && dynshape()->shape()->getExtent().height() > closedist * 1.5f);
    
    if (m_step > 2 && dynshape()->shape()->isClosed() != closed) {
        lines->setClosed(closed);
        if (closed)
            lines->removePoint(m_step);
        else
            lines->addPoint(sender->pointM);
    }
    if (!closed) {
        dynshape()->shape()->setPoint(m_step, sender->pointM);
        if (m_step > 0 && !canAddPoint(sender, true))
            lines->removePoint(m_step);
    }
    dynshape()->shape()->update();
    
    if (m_step > 1) {
        _addshape(sender);
    }
    else {
        click(sender);  // add a point
    }
    _delayClear();

    return _touchEnded(sender);
}

bool MgCmdDrawFreeLines::canAddPoint(const MgMotion* /*sender*/, bool /*ended*/)
{
    /*float minDist = sender->displayMmToModel(3.f);
    Point2d endPt  = dynshape()->shape()->getPoint(m_step - 1);
    float distToEnd = endPt.distanceTo(sender->pointM);
    float turnAngle = 90;
    
    if (m_step > 1)
    {
        Point2d lastPt = dynshape()->shape()->getPoint(m_step - 2);
        turnAngle = (endPt - lastPt).angleTo(sender->pointM - endPt);
        turnAngle = mgRad2Deg(fabsf(turnAngle));
    }
    
    if (distToEnd < minDist * (ended ? 0.25 : 1))
        return false;
    if (!ended && sin(turnAngle) * distToEnd < 5)
        return false;*/
    
    return true;
}
