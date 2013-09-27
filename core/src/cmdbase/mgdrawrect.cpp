// mgdrawrect.cpp: 实现矩形绘图命令类
// Copyright (c) 2004-2013, Zhang Yungui
// License: LGPL, https://github.com/rhcad/touchvg

#include "mgdrawrect.h"
#include <mgshapet.h>
#include <mgbasicsp.h>

MgCmdDrawRect::MgCmdDrawRect()
{
}

MgCmdDrawRect::~MgCmdDrawRect()
{
}

bool MgCmdDrawRect::initialize(const MgMotion* sender)
{
    return _initialize(MgShapeT<MgRect>::create, sender);
}

bool MgCmdDrawRect::undo(const MgMotion* sender)
{
    return MgCommandDraw::_undo(sender);
}

bool MgCmdDrawRect::touchBegan(const MgMotion* sender)
{
    m_step = 1;
    m_startPt = snapPoint(sender, true);
    ((MgBaseRect*)dynshape()->shape())->setRect2P(m_startPt, m_startPt);
    dynshape()->shape()->update();

    return _touchBegan(sender);
}

bool MgCmdDrawRect::touchMoved(const MgMotion* sender)
{
    Point2d pt1(m_startPt);
    Point2d pt2(snapPoint(sender));
    MgBaseRect* shape = (MgBaseRect*)dynshape()->shape();
    
    shape->setRect2P(pt1, pt2);
    dynshape()->shape()->update();

    return _touchMoved(sender);
}

bool MgCmdDrawRect::touchEnded(const MgMotion* sender)
{
    Point2d pt1(m_startPt);
    Point2d pt2(snapPoint(sender));
    MgBaseRect* shape = (MgBaseRect*)dynshape()->shape();
    
    shape->setRect2P(pt1, pt2);
    dynshape()->shape()->update();

    float minDist = sender->displayMmToModel(5.f);

    if (shape->getWidth() > minDist && shape->getHeight() > minDist) {
        addRectShape(sender);
    }

    return _touchEnded(sender);
}

void MgCmdDrawRect::addRectShape(const MgMotion* sender)
{
    _addshape(sender);
    _delayClear();
}
