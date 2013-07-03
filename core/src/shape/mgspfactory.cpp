// mgspfactory.cpp: 实现 registerCoreCreators
// Copyright (c) 2004-2012, Zhang Yungui
// License: LGPL, https://github.com/rhcad/touchvg

#include "mgshapedoc.h"
#include "mgshapet.h"
#include "mgcomposite.h"
#include "mggrid.h"
#include <map>

// mgRegisterShapeCreator, mgCreateShape
//

static std::map<int, MgShape* (*)()>   s_shapeCreators;

static void registerCoreCreators()
{
    s_shapeCreators[MgShapeT<MgGroup>::Type() & 0xFFFF] = MgShapeT<MgGroup>::create;
    s_shapeCreators[MgShapeT<MgLine>::Type() & 0xFFFF] = MgShapeT<MgLine>::create;
    s_shapeCreators[MgShapeT<MgRect>::Type() & 0xFFFF] = MgShapeT<MgRect>::create;
    s_shapeCreators[MgShapeT<MgEllipse>::Type() & 0xFFFF] = MgShapeT<MgEllipse>::create;
    s_shapeCreators[MgShapeT<MgRoundRect>::Type() & 0xFFFF] = MgShapeT<MgRoundRect>::create;
    s_shapeCreators[MgShapeT<MgDiamond>::Type() & 0xFFFF] = MgShapeT<MgDiamond>::create;
    s_shapeCreators[MgShapeT<MgParallelogram>::Type() & 0xFFFF] = MgShapeT<MgParallelogram>::create;
    s_shapeCreators[MgShapeT<MgLines>::Type() & 0xFFFF] = MgShapeT<MgLines>::create;
    s_shapeCreators[MgShapeT<MgSplines>::Type() & 0xFFFF] = MgShapeT<MgSplines>::create;
    s_shapeCreators[MgShapeT<MgGrid>::Type() & 0xFFFF] = MgShapeT<MgGrid>::create;
    s_shapeCreators[MgShapeT<MgImageShape>::Type() & 0xFFFF] = MgShapeT<MgImageShape>::create;
    s_shapeCreators[MgShapeT<MgArc>::Type() & 0xFFFF] = MgShapeT<MgArc>::create;
    s_shapeCreators[MgShapeT<MgGrid>::Type() & 0xFFFF] = MgShapeT<MgGrid>::create;
}

void mgRegisterShapeCreator(int type, MgShape* (*factory)())
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

MgShape* mgCreateShape(int type)
{
    if (s_shapeCreators.empty())
        registerCoreCreators();
    
    std::map<int, MgShape* (*)()>::const_iterator it = s_shapeCreators.find(type & 0xFFFF);
    return (it != s_shapeCreators.end()) ? (it->second)() : NULL;
}
