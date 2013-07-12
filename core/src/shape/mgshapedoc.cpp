//! \file mgshapedoc.cpp
//! \brief 实现图形文档 MgShapeDoc
// Copyright (c) 2004-2012, Zhang Yungui
// License: LGPL, https://github.com/rhcad/touchvg

#include "mgshapedoc.h"
#include <mgstorage.h>
#include <gidef.h>
#include "mglayer.h"

static const int kMaxLayers = 10;

struct MgShapeDoc::Impl {
    MgLayer*    layers[kMaxLayers];
    MgShapes*   curShapes;
    GiContext   context;
    Matrix2d    xf;
    Box2d       rectW;
    float       viewScale;
    long        changeCount;
    MgLockRW    lock;
    volatile long   refcount;
};

MgShapeDoc::MgShapeDoc()
{
    im = new Impl();
    for (int i = 0; i < kMaxLayers; i++) {
        im->layers[i] = NULL;
    }
    im->layers[0] = MgLayer::create(this, 0);
    im->curShapes = im->layers[0];
    im->viewScale = 0;
    im->changeCount = 0;
    im->refcount = 1;
}

MgShapeDoc::~MgShapeDoc()
{
    for (int i = 0; i < kMaxLayers; i++) {
        if (im->layers[i]) {
            im->layers[i]->release();
            im->layers[i] = NULL;
        }
    }
    delete im;
}

MgShapeDoc* MgShapeDoc::create()
{
    return new MgShapeDoc();
}

void MgShapeDoc::addRef()
{
    giInterlockedIncrement(&im->refcount);
}

void MgShapeDoc::release()
{
    if (giInterlockedDecrement(&im->refcount)) {
        delete this;
    }
}

MgObject* MgShapeDoc::clone() const
{
    MgObject* p = new MgShapeDoc();
    p->copy(*this);
    return p;
}

void MgShapeDoc::copy(const MgObject& src)
{
    if (src.isKindOf(Type())) {
        const MgShapeDoc& doc = (const MgShapeDoc&)src;
        im->xf = doc.im->xf;
        im->rectW = doc.im->rectW;
        im->viewScale = doc.im->viewScale;
    }
}

bool MgShapeDoc::equals(const MgObject& src) const
{
    if (src.isKindOf(Type())) {
        const MgShapeDoc& doc = (const MgShapeDoc&)src;

        if (im->xf != doc.im->xf)
            return false;

        for (int i = 0; i < kMaxLayers; i++) {
            if (!im->layers[i] != !doc.im->layers[i])
                return false;
            if (im->layers[i] && !im->layers[i]->equals(*(doc.im->layers[i]))) {
                return false;
            }
        }

        return true;
    }

    return false;
}

GiContext* MgShapeDoc::context() { return &im->context; }
Matrix2d& MgShapeDoc::modelTransform() { return im->xf; }
Box2d MgShapeDoc::getPageRectW() const { return im->rectW; }
float MgShapeDoc::getViewScale() const { return im->viewScale; }
int MgShapeDoc::getChangeCount() const { return im->changeCount; }
MgLockRW* MgShapeDoc::getLockData() { return &im->lock; }

void MgShapeDoc::setPageRectW(const Box2d& rectW, float viewScale)
{
    im->rectW = rectW;
    im->viewScale = viewScale;
}

void MgShapeDoc::clear()
{
    for (int i = 0; i < kMaxLayers; i++) {
        if (im->layers[i]) {
            im->layers[i]->clear();
        }
    }
    im->curShapes = im->layers[0];
}

void MgShapeDoc::clearCacheData()
{
    for (int i = 0; i < kMaxLayers; i++) {
        if (im->layers[i]) {
            im->layers[i]->clearCacheData();
        }
    }
}

Box2d MgShapeDoc::getExtent() const
{
    Box2d rect;

    for (int i = 0; i < kMaxLayers; i++) {
        if (im->layers[i]) {
            rect.unionWith(im->layers[i]->getExtent());
        }
    }

    return rect;
}

int MgShapeDoc::getShapeCount() const
{
    int n = 0;

    for (int i = 0; i < kMaxLayers; i++) {
        if (im->layers[i]) {
            n += im->layers[i]->getShapeCount();
        }
    }

    return n;
}

MgShapes* MgShapeDoc::getCurrentShapes() const
{
    return im->curShapes;
}

bool MgShapeDoc::setCurrentShapes(MgShapes* shapes)
{
    im->curShapes = shapes ? shapes : im->layers[0];
    return true;
}

bool MgShapeDoc::switchLayer(int index)
{
    bool ret = false;

    if (index >= 0 && index < kMaxLayers) {
        if (!im->layers[index]) {
            im->layers[index] = MgLayer::create(this, index);
        }
        im->curShapes = im->layers[index];
        ret = true;
    }

    return ret;
}

int MgShapeDoc::draw(GiGraphics& gs) const
{
    int n = 0;

    for (int i = 0; i < kMaxLayers; i++) {
        if (im->layers[i]) {
            n += im->layers[i]->draw(gs);
        }
    }

    return n;
}

void MgShapeDoc::afterChanged()
{
    giInterlockedIncrement(&im->changeCount);
}

bool MgShapeDoc::save(MgStorage* s, int startIndex) const
{
    bool ret = true;
    Box2d rect;

    if (!s || !s->writeNode("shapedoc", -1, false))
        return false;

    s->writeFloatArray("transform", &im->xf.m11, 6);
    s->writeFloatArray("zoomExtent", &im->rectW.xmin, 4);
    s->writeFloat("viewScale", im->viewScale);
    rect = getExtent();
    s->writeFloatArray("extent", &rect.xmin, 4);
    s->writeUInt32("count", 1);

    for (int i = 0; i < kMaxLayers; i++) {
        if (im->layers[i] && (i == 0 || im->layers[i]->getShapeCount() > 0)) {
            ret = im->layers[i]->save(s, startIndex) || ret;
            startIndex = -1;
        }
    }

    s->writeNode("shapedoc", -1, true);

    return ret;
}

bool MgShapeDoc::load(MgStorage* s, bool addOnly)
{
    bool ret = false;
    Box2d rect;

    if (!s || !s->readNode("shapedoc", -1, false)) {
        return s && s->setError("No root node.");
    }

    s->readFloatArray("transform", &im->xf.m11, 6);
    s->readFloatArray("zoomExtent", &im->rectW.xmin, 4);
    im->viewScale = s->readFloat("viewScale", im->viewScale);
    s->readFloatArray("extent", &rect.xmin, 4);
    s->readUInt32("count", 0);

    for (int i = 0; i < kMaxLayers; i++) {
        if (!im->layers[i]) {
            im->layers[i] = MgLayer::create(this, i);

            if (im->layers[i]->load(s, addOnly)) {
                ret = true;
            }
            else if (i > 0) {
                im->layers[i]->release();
                im->layers[i] = NULL;
            }
        }
        else {
            ret = im->layers[i]->load(s, addOnly) || ret;
        }
        addOnly = false;
    }

    s->readNode("shapedoc", -1, true);

    return ret;
}
