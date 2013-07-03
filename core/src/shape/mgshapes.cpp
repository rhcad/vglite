// mgshapes.cpp: 实现图形列表类 MgShapes
// Copyright (c) 2004-2012, Zhang Yungui
// License: LGPL, https://github.com/rhcad/touchvg

#include "mgshapes.h"
#include "mgshape.h"
#include "mgstorage.h"
#include "gigraph.h"
#include <list>

MgShape* mgCreateShape(int type);

struct MgShapes::I
{
    typedef std::list<MgShape*> Container;
    typedef Container::const_iterator citerator;
    typedef Container::iterator iterator;
    
    Container   shapes;
    MgObject*   owner;
    int         index;
    
    MgShape* findShape(int sid) const;
    int getNewID(int sid);
    bool hasFillColor(const MgShape* shape) const;
};

MgShapes* MgShapes::create(MgObject* owner, int index)
{
    return new MgShapes(owner, owner ? index : -1);
}

MgShapes::MgShapes(MgObject* owner, int index)
{
    im = new I();
    im->owner = owner;
    im->index = index;
}

MgShapes::~MgShapes()
{
    clear();
    delete im;
}

void MgShapes::release()
{
    delete this;
}

MgObject* MgShapes::clone() const
{
    MgShapes *p = new MgShapes(im->owner, im->index);
    p->copy(*this);
    return p;
}

void MgShapes::copy(const MgObject&)
{
}

void MgShapes::copyShapes(const MgShapes* src)
{
    clear();
    void* it;
    for (MgShape* sp = src->getFirstShape(it); sp;
         sp = src->getNextShape(it)) {
        addShape(*sp);
    }
    freeIterator(it);
}

bool MgShapes::equals(const MgObject& src) const
{
    bool ret = false;
    
    if (src.isKindOf(Type())) {
        const MgShapes& _src = (const MgShapes&)src;
        ret = (im->shapes == _src.im->shapes);
    }
    
    return ret;
}

void MgShapes::clear()
{
    for (I::iterator it = im->shapes.begin(); it != im->shapes.end(); ++it) {
        (*it)->release();
    }
    im->shapes.clear();
}

MgObject* MgShapes::getOwner() const
{
    return this ? im->owner : NULL;
}

MgShape* MgShapes::addShape(const MgShape& src)
{
    MgShape* p = (MgShape*)src.clone();
    if (p) {
        p->setParent(this, im->getNewID(src.getID()));
        im->shapes.push_back(p);
    }
    return p;
}

MgShape* MgShapes::addShapeByType(int type)
{
    MgShape* p = mgCreateShape(type);
    if (p) {
        p->setParent(this, im->getNewID(0));
        im->shapes.push_back(p);
    }
    return p;
}

MgShape* MgShapes::removeShape(int sid, bool skipLockedShape)
{
    for (I::iterator it = im->shapes.begin(); it != im->shapes.end(); ++it) {
        MgShape* shape = *it;
        if (shape->getID() == sid) {
            if (skipLockedShape && shape->shapec()->getFlag(kMgShapeLocked)) {
                return NULL;
            }
            im->shapes.erase(it);
            return shape;
        }
    }
    return NULL;
}

MgShape* MgShapes::moveTo(int sid, MgShapes* dest)
{
    MgShape* shape = NULL;
    
    if (dest && dest != this) {
        shape = removeShape(sid, false);
        
        if (shape && dest->isKindOf(Type())) {
            MgShapes* d = (MgShapes*)dest;
            
            shape->setParent(d, d->im->getNewID(shape->getID()));
            d->im->shapes.push_back(shape);
        }
        else if (shape) {
            MgShape* newsp = addShape(*shape);
            shape->release();
            shape = newsp;
        }
    }
    
    return shape;
}

void MgShapes::moveAllShapesTo(MgShapes* dest)
{
    while (!im->shapes.empty()) {
        moveTo(im->shapes.back()->getID(), dest);
    }
}

bool MgShapes::bringToFront(int sid)
{
    for (I::iterator it = im->shapes.begin(); it != im->shapes.end(); ++it) {
        MgShape* shape = *it;
        if (shape->getID() == sid) {
            im->shapes.erase(it);
            im->shapes.push_back(shape);
            return true;
        }
    }
    return false;
}

int MgShapes::getShapeCount() const
{
    return this ? (int)im->shapes.size() : 0;
}

void MgShapes::freeIterator(void*& it)
{
    delete (I::citerator*)it;
    it = NULL;
}

MgShape* MgShapes::getFirstShape(void*& it) const
{
    if (!this || im->shapes.empty()) {
        it = NULL;
        return NULL;
    }
    it = (void*)(new I::citerator(im->shapes.begin()));
    return im->shapes.empty() ? NULL : im->shapes.front();
}

MgShape* MgShapes::getNextShape(void*& it) const
{
    I::citerator* pit = (I::citerator*)it;
    if (pit && *pit != im->shapes.end()) {
        ++(*pit);
        if (*pit != im->shapes.end())
            return *(*pit);
    }
    return NULL;
}

MgShape* MgShapes::getHeadShape() const
{
    return (!this || im->shapes.empty()) ? NULL : im->shapes.front();
}

MgShape* MgShapes::getLastShape() const
{
    return (!this || im->shapes.empty()) ? NULL : im->shapes.back();
}

MgShape* MgShapes::findShape(int sid) const
{
    return im->findShape(sid);
}

MgShape* MgShapes::findShapeByTag(int tag) const
{
    if (!this || 0 == tag)
        return NULL;
    for (I::citerator it = im->shapes.begin(); it != im->shapes.end(); ++it) {
        if ((*it)->getTag() == tag)
            return *it;
    }
    return NULL;
}

MgShape* MgShapes::findShapeByType(int type) const
{
    if (!this || 0 == type)
        return NULL;
    for (I::citerator it = im->shapes.begin(); it != im->shapes.end(); ++it) {
        if ((*it)->shapec()->getType() == type)
            return *it;
    }
    return NULL;
}

Box2d MgShapes::getExtent() const
{
    Box2d extent;
    for (I::citerator it = im->shapes.begin(); it != im->shapes.end(); ++it) {
        extent.unionWith((*it)->shapec()->getExtent());
    }
    
    return extent;
}

MgShape* MgShapes::hitTest(const Box2d& limits, Point2d& nearpt, 
                           int* segment, Filter filter) const
{
    MgShape* retshape = NULL;
    float distMin = _FLT_MAX;
    
    for (I::citerator it = im->shapes.begin(); it != im->shapes.end(); ++it) {
        const MgBaseShape* shape = (*it)->shapec();
        Box2d extent(shape->getExtent());
        
        if (!shape->getFlag(kMgShapeLocked)
            && extent.isIntersect(limits)
            && (!filter || filter(*it))) {
            Point2d tmpNear;
            int    tmpSegment;
            float  tol = (!im->hasFillColor(*it) ? limits.width() / 2
                          : mgMax(extent.width(), extent.height()));
            float  dist = shape->hitTest(limits.center(), tol, tmpNear, tmpSegment);
            
            if (distMin > dist - _MGZERO) {     // 让末尾图形优先选中
                distMin = dist;
                if (segment) {
                    *segment = tmpSegment;
                }
                nearpt = tmpNear;
                retshape = *it;
            }
        }
    }
    if (retshape && distMin > limits.width() && !im->hasFillColor(retshape)) {
        retshape = NULL;
    }
    
    return retshape;
}

int MgShapes::draw(GiGraphics& gs, const GiContext *ctx) const
{
    return dyndraw(0, gs, ctx, -1);
}

int MgShapes::dyndraw(int mode, GiGraphics& gs, const GiContext *ctx, int segment) const
{
    Box2d clip(gs.getClipModel());
    int count = 0;
    
    for (I::citerator it = im->shapes.begin(); it != im->shapes.end(); ++it) {
        if ((*it)->shapec()->getExtent().isIntersect(clip)) {
            if ((*it)->draw(mode, gs, ctx, segment))
                count++;
        }
    }
    
    return count;
}

bool MgShapes::save(MgStorage* s, int startIndex) const
{
    bool ret = false;
    Box2d rect;
    int index = 0;
    
    if (s && s->writeNode("shapes", im->index, false)) {
        ret = true;
        rect = getExtent();
        s->writeFloatArray("extent", &rect.xmin, 4);
        
        s->writeUInt32("count", im->shapes.size() - (int)startIndex);
        for (I::citerator it = im->shapes.begin();
             ret && it != im->shapes.end(); ++it, ++index)
        {
            if (index < startIndex)
                continue;
            ret = s->writeNode("shape", index - startIndex, false);
            if (ret) {
                s->writeUInt32("type", (*it)->getType() & 0xFFFF);
                s->writeUInt32("id", (*it)->getID());
                
                rect = (*it)->shapec()->getExtent();
                s->writeFloatArray("extent", &rect.xmin, 4);
                
                ret = (*it)->save(s);
                s->writeNode("shape", index - startIndex, true);
            }
        }
        s->writeNode("shapes", im->index, true);
    }
    
    return ret;
}

bool MgShapes::load(MgStorage* s, bool addOnly)
{
    bool ret = false;
    Box2d rect;
    int index = 0;
    
    if (s && s->readNode("shapes", im->index, false)) {
        ret = true;
        s->readFloatArray("extent", &rect.xmin, 4);
        s->readUInt32("count", 0);
        
        if (!addOnly)
            clear();
        
        while (ret && s->readNode("shape", index, false)) {
            int type = s->readUInt32("type", 0);
            int id = s->readUInt32("id", 0);
            MgShape* shape = mgCreateShape(type);
            
            s->readFloatArray("extent", &rect.xmin, 4);
            if (shape) {
                shape->setParent(this, id);
                ret = shape->load(s);
                if (ret) {
                    im->shapes.push_back(shape);
                }
                else {
                    shape->release();
                }
            }
            s->readNode("shape", index++, true);
        }
        s->readNode("shapes", im->index, true);
    }
    else if (im->index == 0) {
        s->setError("No shapes node.");
    }
    
    return ret;
}

MgShape* MgShapes::I::findShape(int sid) const
{
    if (!this || 0 == sid)
        return NULL;
    for (citerator it = shapes.begin(); it != shapes.end(); ++it) {
        if ((*it)->getID() == sid)
            return *it;
    }
    return NULL;
}

int MgShapes::I::getNewID(int sid)
{
    if (0 == sid || findShape(sid)) {
        sid = 1;
        if (!shapes.empty())
            sid = shapes.back()->getID() + 1;
        while (findShape(sid))
            sid++;
    }
    return sid;
}

bool MgShapes::I::hasFillColor(const MgShape* shape) const
{
    return shape->contextc()->hasFillColor() && shape->shapec()->isClosed();
}
