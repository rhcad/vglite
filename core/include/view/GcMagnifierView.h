//! \file GcMagnifierView.h
//! \brief 定义放大镜视图类 GcMagnifierView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#ifndef VGLITE_CORE_MAGNIFIERVIEW_H
#define VGLITE_CORE_MAGNIFIERVIEW_H

#include "GcGraphView.h"

//! 放大镜视图类
/*! \ingroup GROUP_VIEW
 */
class GcMagnifierView : public GcShapeView
{
public:
    GcMagnifierView(GcShapeDoc* doc, GiView *view, GcGraphView* mainView);
    virtual ~GcMagnifierView();
    
private:
    GcGraphView*     _mainView;
};

#endif // VGLITE_CORE_MAGNIFIERVIEW_H
