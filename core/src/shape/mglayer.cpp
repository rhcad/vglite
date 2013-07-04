//! \file mglayer.cpp
//! \brief 实现图形文档 MgLayer
// Copyright (c) 2004-2012, Zhang Yungui
// License: LGPL, https://github.com/rhcad/touchvg

#include "mglayer.h"
#include "mgshapedoc.h"
#include <mgstorage.h>

MgLayer::MgLayer(MgShapeDoc* doc, int index) : MgShapes(doc, index)
{
}

MgLayer::~MgLayer()
{
}

MgLayer* MgLayer::create(MgShapeDoc* doc, int index)
{
    return new MgLayer(doc, index);
}

void MgLayer::release()
{
    delete this;
}

MgShapeDoc* MgLayer::doc() const
{
    return (MgShapeDoc*)getOwner();
}

MgObject* MgLayer::clone() const
{
    MgObject* p = new MgLayer(doc(), -1);
    p->copy(*this);
    return p;
}

void MgLayer::copy(const MgObject& src)
{
    MgShapes::copy(src);
    if (src.isKindOf(Type())) {
        //const MgLayer& layer = (const MgLayer&)src;
    }
}

bool MgLayer::equals(const MgObject& src) const
{
    if (src.isKindOf(Type()) && MgShapes::equals(src)) {
        //const MgLayer& layer = (const MgLayer&)src;
        return true;
    }
    return false;
}

bool MgLayer::saveExtra(MgStorage* s) const
{
    return !!s;
}

bool MgLayer::loadExtra(MgStorage* s)
{
    return !!s;
}
