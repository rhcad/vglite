//! \file mggrid.h
//! \brief 定义网格图形类
// Copyright (c) 2004-2012, Zhang Yungui
// License: LGPL, https://github.com/rhcad/touchvg

#ifndef __GEOMETRY_GRIDSHAPE_H_
#define __GEOMETRY_GRIDSHAPE_H_

#include "mgbasicsp.h"

//! 网格图形类
/*! \ingroup CORE_SHAPE
*/
class MgGrid : public MgBaseRect
{
    MG_INHERIT_CREATE(MgGrid, MgBaseRect, 20)
public:
    virtual int snap(Point2d& pnt, float& distx, float& disty) const;
    bool valid(float tol) const;

protected:
    virtual void setFlag(MgShapeBit bit, bool on);
    void _copy(const MgGrid& src);
    bool _equals(const MgGrid& src) const;
    void _clear();
    bool _save(MgStorage* s) const;
    bool _load(MgStorage* s);
    int _getHandleCount() const;
    Point2d _getHandlePoint(int index) const;
    bool _setHandlePoint(int index, const Point2d& pt, float tol);
    bool isFloatingCommand() { return true; }

protected:
    Vector2d    m_cell;
};

#endif // __GEOMETRY_GRIDSHAPE_H_
