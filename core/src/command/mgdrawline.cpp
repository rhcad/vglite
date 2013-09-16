// mgdrawlines.cpp: 实现直线段绘图命令
// Author: pengjun, 2012.6.4
// License: LGPL, https://github.com/rhcad/touchvg

#include "mgdrawline.h"
#include <mgshapet.h>
#include <mgbasicsp.h>

MgCmdDrawLine::MgCmdDrawLine()
{
}

MgCmdDrawLine::~MgCmdDrawLine()
{
}

bool MgCmdDrawLine::initialize(const MgMotion* sender)
{
    return _initialize(MgShapeT<MgLine>::create, sender);
}

bool MgCmdDrawLine::undo(const MgMotion* sender)
{
    return MgCommandDraw::_undo(sender);
}

bool MgCmdDrawLine::touchBegan(const MgMotion* sender)
{
    m_step = 1;

    Point2d pnt(snapPoint(sender, true));
    dynshape()->shape()->setPoint(0, pnt);
    dynshape()->shape()->setPoint(1, pnt);
    dynshape()->shape()->update();

    return _touchBegan(sender);
}

bool MgCmdDrawLine::touchMoved(const MgMotion* sender)
{
    dynshape()->shape()->setPoint(1, snapPoint(sender));
    dynshape()->shape()->update();

    return _touchMoved(sender);
}

bool MgCmdDrawLine::touchEnded(const MgMotion* sender)
{
    dynshape()->shape()->setPoint(1, snapPoint(sender));
    dynshape()->shape()->update();

    if ( ((MgLine*)dynshape()->shape())->length() > displayMmToModel(2, sender)) {
        _addshape(sender);
    }
    _delayClear();

    return _touchEnded(sender);
}

bool MgCmdDrawFixedLine::initialize(const MgMotion* sender)
{
    bool ret = _initialize(MgShapeT<MgLine>::create, sender);
    if (ret) {
        dynshape()->shape()->setFlag(kMgFixedLength, true);
    }
    return ret;
}
