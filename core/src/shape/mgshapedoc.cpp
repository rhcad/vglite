//! \file mgshapedoc.cpp
//! \brief 实现图形文档 MgShapeDoc
// Copyright (c) 2004-2012, Zhang Yungui
// License: LGPL, https://github.com/rhcad/touchvg

#include <mgshapedoc.h>
#include <mgstorage.h>
#include <gidef.h>

MgShapeDoc::MgShapeDoc() : _viewScale(0), _changeCount(0)
{
    for (int i = 0; i < kMaxLayers; i++)
        _layers[i] = NULL;
    _layers[0] = mgCreateShapes(this, 0);
    _shapes = _layers[0];
}

MgShapeDoc::~MgShapeDoc()
{
    for (int i = 0; i < kMaxLayers; i++) {
        if (_layers[i]) {
            _layers[i]->release();
            _layers[i] = NULL;
        }
    }
}

MgShapeDoc* MgShapeDoc::create()
{
    return new MgShapeDoc();
}

void MgShapeDoc::release()
{
    delete this;
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
        _xf = doc._xf;
        _rectW = doc._rectW;
        _viewScale = doc._viewScale;
    }
}

bool MgShapeDoc::equals(const MgObject& src) const
{
    if (src.isKindOf(Type())) {
        const MgShapeDoc& doc = (const MgShapeDoc&)src;

        if (_xf != doc._xf)
            return false;

        for (int i = 0; i < kMaxLayers; i++) {
            if (!_layers[i] != !doc._layers[i])
                return false;
            if (_layers[i] && !_layers[i]->equals(*(doc._layers[i]))) {
                return false;
            }
        }

        return true;
    }

    return false;
}

void MgShapeDoc::setPageRectW(const Box2d& rectW, float viewScale)
{
    _rectW = rectW;
    _viewScale = viewScale;
}

void MgShapeDoc::clear()
{
    for (int i = 0; i < kMaxLayers; i++) {
        if (_layers[i]) {
            _layers[i]->clear();
        }
    }
    _shapes = _layers[0];
}

Box2d MgShapeDoc::getExtent() const
{
    Box2d rect;

    for (int i = 0; i < kMaxLayers; i++) {
        if (_layers[i]) {
            rect.unionWith(_layers[i]->getExtent());
        }
    }

    return rect;
}

int MgShapeDoc::getShapeCount() const
{
    int n = 0;

    for (int i = 0; i < kMaxLayers; i++) {
        if (_layers[i]) {
            n += _layers[i]->getShapeCount();
        }
    }

    return n;
}

MgShapes* MgShapeDoc::getCurrentShapes() const
{
    return _shapes;
}

bool MgShapeDoc::setCurrentShapes(MgShapes* shapes)
{
    _shapes = shapes ? shapes : _layers[0];
    return true;
}

bool MgShapeDoc::switchLayer(int index)
{
    bool ret = false;

    if (index >= 0 && index < kMaxLayers) {
        if (!_layers[index]) {
            _layers[index] = mgCreateShapes(this, index);
        }
        _shapes = _layers[index];
        ret = true;
    }

    return ret;
}

int MgShapeDoc::draw(GiGraphics& gs) const
{
    int n = 0;

    for (int i = 0; i < kMaxLayers; i++) {
        if (_layers[i]) {
            n += _layers[i]->draw(gs);
        }
    }

    return n;
}

void MgShapeDoc::afterChanged()
{
    giInterlockedIncrement(&_changeCount);
}

bool MgShapeDoc::save(MgStorage* s, int startIndex) const
{
    bool ret = true;
    Box2d rect;

    if (!s || !s->writeNode("shapedoc", -1, false))
        return false;

    s->writeFloatArray("transform", &_xf.m11, 6);
    s->writeFloatArray("zoomExtent", &_rectW.xmin, 4);
    s->writeFloat("viewScale", _viewScale);
    rect = getExtent();
    s->writeFloatArray("extent", &rect.xmin, 4);
    s->writeUInt32("count", 1);

    for (int i = 0; i < kMaxLayers; i++) {
        if (_layers[i] && (i == 0 || _layers[i]->getShapeCount() > 0)) {
            ret = _layers[i]->save(s, startIndex) || ret;
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

    if (!s || !s->readNode("shapedoc", -1, false))
        return s->setError("No root node.");

    s->readFloatArray("transform", &_xf.m11, 6);
    s->readFloatArray("zoomExtent", &_rectW.xmin, 4);
    _viewScale = s->readFloat("viewScale", _viewScale);
    s->readFloatArray("extent", &rect.xmin, 4);
    s->readUInt32("count", 0);

    for (int i = 0; i < kMaxLayers; i++) {
        if (!_layers[i]) {
            _layers[i] = mgCreateShapes(this, i);

            if (_layers[i]->load(s, addOnly)) {
                ret = true;
            }
            else if (i > 0) {
                _layers[i]->release();
                _layers[i] = NULL;
            }
        }
        else {
            ret = _layers[i]->load(s, addOnly) || ret;
        }
        addOnly = false;
    }

    s->readNode("shapedoc", -1, true);

    return ret;
}
