// mgellipse.cpp: 实现椭圆图形类 MgEllipse
// Copyright (c) 2004-2012, Zhang Yungui
// License: LGPL, https://github.com/rhcad/touchvg

#include "mgbasicsp.h"
#include <mgshape_.h>
#include <mgnear.h>
#include <mgcurv.h>
#include <mgbase.h>
#include <mgstorage.h>

MG_IMPLEMENT_CREATE(MgEllipse)

MgEllipse::MgEllipse()
{
}

MgEllipse::~MgEllipse()
{
}

float MgEllipse::getRadiusX() const
{
    return getWidth() / 2;
}

float MgEllipse::getRadiusY() const
{
    return getHeight() / 2;
}

void MgEllipse::setRadius(float rx, float ry)
{
    rx = fabsf(rx);
    ry = fabsf(ry);
    if (ry < _MGZERO)
        ry = rx;

    Box2d rect(getCenter(), rx * 2, ry * 2);
    setRectWithAngle(rect.leftTop(), rect.rightBottom(), getAngle(), getCenter());
}

void MgEllipse::_update()
{
    __super::_update();

    mgEllipseToBezier(_bzpts, getCenter(), getWidth() / 2, getHeight() / 2);

    Matrix2d mat(Matrix2d::rotation(getAngle(), getCenter()));
    for (int i = 0; i < 13; i++)
        _bzpts[i] *= mat;

    mgBeziersBox(_extent, 13, _bzpts, true);
}

int MgEllipse::_getHandleCount() const
{
    return getFlag(kMgSquare) ? 5 : 9;
}

Point2d MgEllipse::_getHandlePoint(int index) const
{
    int index2 = getFlag(kMgSquare) ? index + 4 : index;
    return (index < _getHandleCount() - 1
            ? MgBaseRect::_getHandlePoint(index2) : getCenter());
}

int MgEllipse::_getHandleType(int index) const
{
    int index2 = getFlag(kMgSquare) ? index + 4 : index;
    return (index < _getHandleCount() - 1
            ? MgBaseRect::_getHandleType(index2) : kMgHandleCenter);
}

bool MgEllipse::_setHandlePoint(int index, const Point2d& pt, float tol)
{
    int index2 = getFlag(kMgSquare) ? index + 4 : index;
    return (index < _getHandleCount() - 1
            ? MgBaseRect::_setHandlePoint(index2, pt, tol)
            : offset(pt - getCenter(), -1));
}

float MgEllipse::_hitTest(const Point2d& pt, float tol, 
                          Point2d& nearpt, int& segment) const
{
    float distMin = _FLT_MAX;
    const Box2d rect (pt, 2 * tol, 2 * tol);
    Point2d ptTemp;

    segment = -1;
    for (int i = 0; i < 4; i++)
    {
        if (rect.isIntersect(Box2d(4, _bzpts + 3 * i)))
        {
            mgNearestOnBezier(pt, _bzpts + 3 * i, ptTemp);
            float dist = pt.distanceTo(ptTemp);
            if (dist <= tol && dist < distMin)
            {
                distMin = dist;
                nearpt = ptTemp;
                segment = i;
            }
        }
    }

    return distMin;
}

bool MgEllipse::_hitTestBox(const Box2d& rect) const
{
    if (!getExtent().isIntersect(rect))
        return false;
    
    return mgBeziersIntersectBox(rect, 13, _bzpts, true);
}

bool MgEllipse::_draw(int mode, GiGraphics& gs, const GiContext& ctx, int segment) const
{
    bool ret = false;

    if (isOrtho())
    {
        ret = gs.drawEllipse(&ctx, Box2d(_points[0], _points[2]));
    }
    else
    {
        ret = gs.drawBeziers(&ctx, 13, _bzpts, true);
    }

    return __super::_draw(mode, gs, ctx, segment) || ret;
}

// MgArc
//

MG_IMPLEMENT_CREATE(MgArc)

MgArc::MgArc()
{
}

MgArc::~MgArc()
{
}

Point2d MgArc::getCenter() const
{
    return _points[0];
}

Point2d MgArc::getStartPoint() const
{
    return _points[1];
}

Point2d MgArc::getEndPoint() const
{
    return _points[2];
}

Point2d MgArc::getMidPoint() const
{
    return _points[3];
}

float MgArc::getRadius() const
{
    return getStartPoint().distanceTo(getCenter());
}

float MgArc::getStartAngle() const
{
   return (getStartPoint() - getCenter()).angle2();
}

float MgArc::getEndAngle() const
{
    return (getEndPoint() - getCenter()).angle2();
}

float MgArc::getSweepAngle() const
{
    const float midAngle = (getMidPoint() - getCenter()).angle2();
    const float startAngle = getStartAngle();
    const float endAngle = getEndAngle();

    if (mgEquals(midAngle, startAngle) && mgEquals(startAngle, endAngle)) {
        return endAngle - startAngle;
    }

    float startAngle2 = startAngle;
    float midAngle2 = midAngle;
    float endAngle2 = endAngle;

    // 先尝试看是否为逆时针方向：endAngle2 > midAngle2 > startAngle2 >= 0
    if (startAngle2 < 0)
        startAngle2 += _M_2PI;
    while (midAngle2 < startAngle2)
        midAngle2 += _M_2PI;
    while (endAngle2 < midAngle2)
        endAngle2 += _M_2PI;

    if (fabsf(startAngle2 + endAngle2 - 2 * midAngle2) < _M_PI_6
        && endAngle2 - startAngle2 < _M_2PI) {
        return endAngle2 - startAngle2;
    }

    // 再尝试看是否为顺时针方向：endAngle2 < midAngle2 < startAngle2 <= 0
    startAngle2 = startAngle;
    midAngle2 = midAngle;
    endAngle2 = endAngle;
    if (startAngle2 > 0)
        startAngle2 -= _M_2PI;
    while (midAngle2 > startAngle2)
        midAngle2 -= _M_2PI;
    while (endAngle2 > midAngle2)
        endAngle2 -= _M_2PI;

    if (fabsf(startAngle2 + endAngle2 - 2 * midAngle2) < _M_PI_6) {
        if (endAngle2 - startAngle2 > -_M_2PI)
            return endAngle2 - startAngle2;
        return mgToRange(endAngle2 - startAngle2, -_M_2PI, 0);
    }

    return endAngle - startAngle;   // error
}

Vector2d MgArc::getStartTangent() const
{
    Vector2d rec(getStartPoint() - getCenter());
    return getSweepAngle() > 0 ? rec.perpVector() : -rec.perpVector();
}

Vector2d MgArc::getEndTangent() const
{
    Vector2d rec(getEndPoint() - getCenter());
    return getSweepAngle() > 0 ? rec.perpVector() : -rec.perpVector();
}

bool MgArc::setCenterRadius(const Point2d& center, float radius, float startAngle, float sweepAngle)
{
    _points[0] = center;
    _points[1] = center.polarPoint(startAngle, radius);
    _points[2] = center.polarPoint(startAngle + sweepAngle, radius);
    _points[3] = center.polarPoint(startAngle + sweepAngle / 2, radius);
    _update();
    return true;
}

bool MgArc::setStartMidEnd(const Point2d& start, const Point2d& point, const Point2d& end)
{
    Point2d center;
    float radius, startAngle, sweepAngle = 0;

    return mgArc3P(start, point, end, center, radius, &startAngle, &sweepAngle)
        && setCenterRadius(center, radius, startAngle, sweepAngle);
}

bool MgArc::setCenterStartEnd(const Point2d& center, const Point2d& start, const Point2d& end)
{
    float startAngle = (start - center).angle2();
    float endAngle = (end - center).angle2();
    float sweepAngle = mgToRange(endAngle - startAngle, -_M_2PI, _M_2PI);

    if (!mgIsZero(sweepAngle)) {
        float lastSweepAngle = getSweepAngle();
        if (fabsf( fabsf(sweepAngle) - fabsf(lastSweepAngle) ) > _M_PI_6) {
            sweepAngle = sweepAngle + (sweepAngle > 0 ?  -_M_2PI :  _M_2PI);
        }
    }

    return setCenterRadius(center, start.distanceTo(center), startAngle, sweepAngle);
}

bool MgArc::setTanStartEnd(const Vector2d& startTan, const Point2d& start, const Point2d& end)
{
    Point2d center;
    float radius, startAngle, sweepAngle = 0;

    return mgArcTan(start, end, startTan, center, radius, &startAngle, &sweepAngle)
        && setCenterRadius(center, radius, startAngle, sweepAngle);
}

bool MgArc::_draw(int mode, GiGraphics& gs, const GiContext& ctx, int segment) const
{
    bool ret = gs.drawArc(&ctx, getCenter(), getRadius(), 0, getStartAngle(), getSweepAngle());
    if (mode > 0) {
        GiContext ctxln(0, GiColor(0, 126, 0, 64), kGiLineDashDot);
        gs.drawLine(&ctxln, getCenter(), getStartPoint());
        gs.drawLine(&ctxln, getCenter(), getEndPoint());
        gs.drawLine(&ctxln, getStartPoint(), getStartPoint() + getStartTangent());
        gs.drawLine(&ctxln, getEndPoint(), getEndPoint() + getEndTangent());
    }
    return __super::_draw(mode, gs, ctx, segment) || ret;
}

void MgArc::_update()
{
    Point2d points[16];
    int n = mgAngleArcToBezier(points, getCenter(), getRadius(), 0, getStartAngle(), getSweepAngle());

    mgBeziersBox(_extent, n, points);
}

bool MgArc::_reverse()
{
    mgSwap(_points[1], _points[2]);
    return true;
}

int MgArc::_getPointCount() const
{
    return 4;
}

Point2d MgArc::_getPoint(int index) const
{
    return _points[index % 4];
}

void MgArc::_setPoint(int index, const Point2d& pt)
{
    _points[index % 4] = pt;
}

void MgArc::_copy(const MgArc& src)
{
    for (int i = 0; i < _getPointCount(); i++)
        _points[i] = src._points[i];
    __super::_copy(src);
}

bool MgArc::_equals(const MgArc& src) const
{
    for (int i = 0; i < _getPointCount(); i++) {
        if (_points[i] != src._points[i])
            return false;
    }
    return __super::_equals(src);
}

void MgArc::_transform(const Matrix2d& mat)
{
    for (int i = 0; i < _getPointCount(); i++)
        _points[i] *= mat;
    __super::_transform(mat);
}

void MgArc::_clear()
{
    for (int i = 0; i < _getPointCount(); i++)
        _points[i] = Point2d();
    __super::_clear();
}

float MgArc::_hitTest(const Point2d& pt, float tol, 
                      Point2d& nearpt, int&) const
{
    Point2d points[16];
    int n = mgAngleArcToBezier(points, getCenter(), getRadius(), 0, getStartAngle(), getSweepAngle());

    float distMin = _FLT_MAX;
    Point2d ptTemp;

    for (int i = 0; i + 3 < n; i += 3) {
        mgNearestOnBezier(pt, points + i, ptTemp);
        float dist = pt.distanceTo(ptTemp);
        if (dist <= tol && dist < distMin) {
            distMin = dist;
            nearpt = ptTemp;
        }
    }
    
    return distMin;
}

bool MgArc::_hitTestBox(const Box2d& rect) const
{
    if (!getExtent().isIntersect(rect))
        return false;

    Point2d points[16];
    int n = mgAngleArcToBezier(points, getCenter(), getRadius(), 0, getStartAngle(), getSweepAngle());
    
    return mgBeziersIntersectBox(rect, n, points);
}

bool MgArc::_save(MgStorage* s) const
{
    bool ret = __super::_save(s);
    s->writeFloatArray("points", &(_points[0].x), 8);
    return ret;
}

bool MgArc::_load(MgStorage* s)
{
    return __super::_load(s) && s->readFloatArray("points", &(_points[0].x), 8) == 8;
}

int MgArc::_getHandleCount() const
{
    return 8;
}

int MgArc::_getHandleType(int index) const
{
    if (index == 3) {
        return kMgHandleMidPoint;
    }
    if (index >= 4 && index <= 7) {
        return kMgHandleOutside;
    }
    if (index == 0) {
        return kMgHandleCenter;
    }
    return __super::_getHandleType(index);
}

Point2d MgArc::_getHandlePoint(int index) const
{
    if (index == 1) {
        return getStartPoint();
    }
    if (index == 2) {
        return getEndPoint();
    }
    if (index == 3) {
        return getMidPoint();
    }
    if (index == 4) {
        return (getStartPoint() * 2 + getCenter()) / 3;
    }
    if (index == 5) {
        return (getEndPoint() * 2 + getCenter()) / 3;
    }
    if (index == 6) {
        return getStartPoint() + getStartTangent();
    }
    if (index == 7) {
        return getEndPoint() + getEndTangent();
    }
    return getCenter();
}

bool MgArc::_setHandlePoint(int index, const Point2d& pt, float)
{
    if (index == 1) {
        return setStartMidEnd(pt, getMidPoint(), getEndPoint());
    }
    if (index == 2) {
        return setStartMidEnd(getStartPoint(), getMidPoint(), pt);
    }
    if (index == 3) {
        return setStartMidEnd(getStartPoint(), pt, getEndPoint());
    }
    if (index == 4) {
        float a = (pt - getCenter()).angle2();
        return setCenterRadius(getCenter(), getRadius(), a, getEndAngle() - a);
    }
    if (index == 5) {
        float a = (pt - getCenter()).angle2();
        return setCenterRadius(getCenter(), getRadius(), getStartAngle(), a - getStartAngle());
    }
    if (index == 6) {
        return setTanStartEnd(pt - getStartPoint(), getStartPoint(), getEndPoint());
    }
    if (index == 7) {
        return (setTanStartEnd(getEndPoint() - pt, getEndPoint(), getStartPoint())
            && _reverse());
    }
    return setCenterStartEnd(pt, getStartPoint(), getEndPoint());
}

int MgArc::_getDimensions(const Matrix2d& m2w, float* vars, char* types, int count) const
{
    int ret = 0;
    
    if (count > ret) {
        types[ret] = 'r';
        vars[ret++] = fabsf(getRadius() * m2w.m11);
    }
    if (count > ret) {
        types[ret] = 'a';
        vars[ret++] = mgRad2Deg(mgTo0_2PI(getSweepAngle() * (m2w.m22 < 0 ? -1.f : 1.f)));
    }
    
    return ret;
}
