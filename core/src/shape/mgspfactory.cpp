// mgspfactory.cpp: 实现 registerCoreCreators
// Copyright (c) 2004-2012, Zhang Yungui
// License: LGPL, https://github.com/rhcad/touchvg

#include "mgshapedoc.h"
#include "mgshapet.h"
#include "mgcomposite.h"
#include "mggrid.h"
#include <map>

static std::map<int, MgShape* (*)()>   s_shapeCreators;

template <class T>
static inline void addCreator() {
    s_shapeCreators[MgShapeT<T>::Type() & 0xFFFF] = MgShapeT<T>::create;
}

static void registerCoreCreators()
{
    addCreator<MgGroup>();
    addCreator<MgLine>();
    addCreator<MgRect>();
    addCreator<MgEllipse>();
    addCreator<MgRoundRect>();
    addCreator<MgDiamond>();
    addCreator<MgParallelogram>();
    addCreator<MgLines>();
    addCreator<MgSplines>();
    addCreator<MgGrid>();
    addCreator<MgImageShape>();
    addCreator<MgArc>();
    addCreator<MgGrid>();
}

void MgShape::registerCreator(int type, MgShape* (*factory)())
{
    if (s_shapeCreators.empty()) {
        registerCoreCreators();
    }
    type = type & 0xFFFF;
    if (type > 20) {
        if (factory) {
            s_shapeCreators[type] = factory;
        }
        else {
            s_shapeCreators.erase(type);
        }
    }
}

MgShape* MgShape::createShape(int type)
{
    if (s_shapeCreators.empty())
        registerCoreCreators();
    
    std::map<int, MgShape* (*)()>::const_iterator it = s_shapeCreators.find(type & 0xFFFF);
    return (it != s_shapeCreators.end()) ? (it->second)() : NULL;
}
