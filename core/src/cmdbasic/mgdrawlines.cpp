// mgdrawlines.cpp: 实现折线绘图命令类 MgCmdDrawLines
// Copyright (c) 2004-2013, Zhang Yungui
// License: LGPL, https://github.com/rhcad/touchvg

#include "mgdrawlines.h"
#include <mgshapet.h>
#include <mgbasicsp.h>

MgCmdDrawLines::MgCmdDrawLines()
{
}

MgCmdDrawLines::~MgCmdDrawLines()
{
}

bool MgCmdDrawLines::initialize(const MgMotion* sender)
{
    return _initialize(MgShapeT<MgLines>::create, sender);
}

bool MgCmdDrawLines::undo(const MgMotion* sender)
{
    if (m_step > 2) {                   // 去掉倒数第二个点，倒数第一点是临时动态点
        ((MgBaseLines*)dynshape()->shape())->removePoint(m_step - 1);
        dynshape()->shape()->update();
    }
    return MgCommandDraw::_undo(sender);
}

static bool _lastClicked = false;

bool MgCmdDrawLines::touchBegan(const MgMotion* sender)
{
    if (0 == m_step) {
        m_step = 1;
        Point2d pnt(snapPoint(sender, true));
        ((MgBaseLines*)dynshape()->shape())->resize(2);
        dynshape()->shape()->setPoint(0, pnt);
        dynshape()->shape()->setPoint(1, pnt);
    }
    else {
        dynshape()->shape()->setPoint(m_step, snapPoint(sender));
    }
    dynshape()->shape()->update();
	_lastClicked = true;
    
    return _touchBegan(sender);
}

bool MgCmdDrawLines::touchMoved(const MgMotion* sender)
{
    Point2d pnt(snapPoint(sender));
    dynshape()->shape()->setPoint(m_step, pnt);
    
    if (needCheckClosed()) {
        float distmin = sender->displayMmToModel(2.f, sender);
        bool closed = m_step > 2 && pnt.distanceTo(dynshape()->shape()->getPoint(0)) < distmin;
        ((MgBaseLines*)dynshape()->shape())->setClosed(closed);
    }
    
    dynshape()->shape()->update();
	_lastClicked = false;
    
    return _touchMoved(sender);
}

bool MgCmdDrawLines::touchEnded(const MgMotion* sender)
{
    Point2d pnt(snapPoint(sender));
    float distmin = sender->displayMmToModel(2.f, sender);
    bool closed = false;
    
    dynshape()->shape()->setPoint(m_step, pnt);
    
    if (needCheckClosed()) {
        closed = m_step > 2 && pnt.distanceTo(dynshape()->shape()->getPoint(0)) < distmin;
        ((MgBaseLines*)dynshape()->shape())->setClosed(closed);
    }
    
    dynshape()->shape()->update();
    
    if (pnt.distanceTo(dynshape()->shape()->getPoint(m_step - 1)) > distmin) {
        if (closed || needEnded()) {
            if (closed) {
                ((MgBaseLines*)dynshape()->shape())->removePoint(m_step);
            }
            _addshape(sender);
            _delayClear();
            m_step = 0;
			_lastClicked = false;
        }
        else if (++m_step >= dynshape()->shape()->getPointCount()) {
            ((MgBaseLines*)dynshape()->shape())->addPoint(pnt);
        }
    }
    
    return _touchEnded(sender);
}

bool MgCmdDrawLines::doubleClick(const MgMotion* sender)
{
    MgBaseLines* lines = (MgBaseLines*)dynshape()->shape();
	Point2d pnt(_lastClicked ? lines->endPoint() : sender->pointM);
    
    if (m_step > (lines->isClosed() ? 2 : 1)) {
		while (lines->getPointCount() > (lines->isClosed() ? 2 : 1)
               && sender->displayMmToModel(5.f, sender) > pnt.distanceTo(lines->endPoint())) {
            lines->removePoint(lines->getPointCount() - 1);
        }
        _addshape(sender);
        _delayClear();
        m_step = 0;
    }
    return true;
}

bool MgCmdDrawLines::cancel(const MgMotion* sender)
{
    MgBaseLines* lines = (MgBaseLines*)dynshape()->shape();
    
    if (m_step > (lines->isClosed() ? 2 : 1)) {
        lines->removePoint(m_step--);
        _addshape(sender);
        _delayClear();
        m_step = 0;
        return true;
    }
    
    return MgCommandDraw::cancel(sender);
}
