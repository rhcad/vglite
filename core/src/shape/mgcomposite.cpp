// mgcomposite.cpp: 实现复合图形基类 MgComposite
// Copyright (c) 2004-2013, Zhang Yungui
// License: LGPL, https://github.com/rhcad/touchvg

#include <mgcomposite.h>
#include <mgshapes.h>

MgComposite::MgComposite()
{
    _shapes = MgShapes::create(this);
}

MgComposite::~MgComposite()
{ 
    _shapes->release();
}

bool MgComposite::_isKindOf(int type) const
{
    return type == Type() || MgBaseShape::_isKindOf(type);
}

int MgComposite::_getPointCount() const
{
    MgShape* sp = _shapes->getHeadShape();
    return sp ? sp->shapec()->getPointCount() : 0;
}

Point2d MgComposite::_getPoint(int index) const
{
    MgShape* sp = _shapes->getHeadShape();
    return sp ? sp->shapec()->getPoint(index) : Point2d();
}

void MgComposite::_setPoint(int, const Point2d&)
{
}

int MgComposite::_getHandleCount() const
{
    MgShape* sp = _shapes->getHeadShape();
    return sp ? sp->shapec()->getHandleCount() : 0;
}

int MgComposite::_getHandleType(int index) const
{
    MgShape* sp = _shapes->getHeadShape();
    return sp ? sp->shapec()->getHandleType(index) : kMgHandleOutside;
}

Point2d MgComposite::_getHandlePoint(int index) const
{
    MgShape* sp = _shapes->getHeadShape();
    return sp ? sp->shapec()->getHandlePoint(index) : Point2d();
}

bool MgComposite::_setHandlePoint(int index, const Point2d& pt, float)
{
    return _offset(pt - _getHandlePoint(index), -1);
}

int MgComposite::getShapeCount() const
{
    return _shapes->getShapeCount();
}

void MgComposite::_clearCachedData()
{
    __super::_clearCachedData();
    _shapes->clearCachedData();
}

void MgComposite::_update()
{
    void* it;
    _extent.empty();

    for (MgShape* sp = _shapes->getFirstShape(it); sp;
        sp = _shapes->getNextShape(it)) {
        sp->shape()->update();
        _extent.unionWith(sp->shapec()->getExtent());
    }
    _shapes->freeIterator(it);
}

void MgComposite::updateExtent()
{
    void* it;
    _extent.empty();

    for (MgShape* sp = _shapes->getFirstShape(it); sp;
        sp = _shapes->getNextShape(it)) {
        _extent.unionWith(sp->shapec()->getExtent());
    }
    _shapes->freeIterator(it);
}

void MgComposite::_transform(const Matrix2d& mat)
{
    void* it;
    for (MgShape* sp = _shapes->getFirstShape(it); sp;
        sp = _shapes->getNextShape(it)) {
        sp->shape()->transform(mat);
    }
    _shapes->freeIterator(it);
}

void MgComposite::_clear()
{
    _shapes->clear();
    MgBaseShape::_clear();
}

void MgComposite::_copy(const MgComposite& src)
{
    _shapes->copyShapes(src._shapes);
    __super::_copy(src);
}

bool MgComposite::_equals(const MgComposite& src) const
{
    return _shapes->equals(*(src._shapes)) && __super::_equals(src);
}

float MgComposite::_hitTest(const Point2d& pt, float tol, 
                            Point2d& nearpt, int& segment) const
{
    void* it;
    float mindist = 1e10f;
    Point2d tmpNear;
    int tmpseg;
    Box2d limits(pt, 2 * tol, 0);

    segment = 0;
    for (MgShape* sp = _shapes->getFirstShape(it); sp;
        sp = _shapes->getNextShape(it))
    {
        if (limits.isIntersect(sp->shapec()->getExtent())) {
            float dist = sp->shapec()->hitTest(pt, tol, tmpNear, tmpseg);

            if (mindist > dist - _MGZERO) {
                mindist = dist;
                nearpt = tmpNear;
                segment = sp->getID();
            }
        }
    }
    _shapes->freeIterator(it);

    return mindist;
}

bool MgComposite::_offset(const Vector2d& vec, int)
{
    void* it;
    int n = 0;

    for (MgShape* sp = _shapes->getFirstShape(it); sp;
        sp = _shapes->getNextShape(it)) {
        n += sp->shape()->offset(vec, -1) ? 1 : 0;
    }
    _shapes->freeIterator(it);

    return n > 0;
}

bool MgComposite::_draw(int mode, GiGraphics& gs, const GiContext& ctx, int) const
{
    void* it;
    int n = 0;

    for (MgShape* sp = _shapes->getFirstShape(it); sp;
        sp = _shapes->getNextShape(it)) {
        n += sp->draw(mode, gs, ctx.isNullLine() ? NULL : &ctx, -1) ? 1 : 0;
    }
    _shapes->freeIterator(it);

    return n > 0;
}

#include <mgshape_.h>

MG_IMPLEMENT_CREATE(MgGroup)

MgGroup::MgGroup()
{
}

MgGroup::~MgGroup()
{
}

bool MgGroup::_offset(const Vector2d& vec, int segment)
{
    MgShape* sp = _shapes->findShape(segment);

    if (sp && canOffsetShapeAlone(sp)) {
        return sp->shape()->offset(vec, -1);
    }

    return __super::_offset(vec, segment);
}

bool MgGroup::_draw(int mode, GiGraphics& gs, const GiContext& ctx, int segment) const
{
    MgShape* sp = _shapes->findShape(segment);
    if (sp) {
        return sp->draw(mode, gs, ctx.isNullLine() ? NULL : &ctx);
    }
    return __super::_draw(mode, gs, ctx, segment);
}

bool MgGroup::_save(MgStorage* s) const
{
    return __super::_save(s) && _shapes->save(s);
}

bool MgGroup::_load(MgStorage* s)
{
    return __super::_load(s) && _shapes->load(s);
}

bool MgGroup::addShape(MgShape* shape)
{
    MgShape* ret = NULL;

    if (shape && shape->getParent()) {
        ret = shape->getParent()->moveTo(shape->getID(), _shapes);
    }
    else if (shape) {
        ret = _shapes->addShape(*shape);
    }

    return !!ret;
}
