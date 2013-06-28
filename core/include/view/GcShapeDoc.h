//! \file GcShapeDoc.h
//! \brief 定义图形文档类 GcShapeDoc
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#ifndef VGLITE_CORE_SHAPEDOC_H
#define VGLITE_CORE_SHAPEDOC_H

#include <vector>

class GcBaseView;
class GiView;

//! 图形文档类
class GcShapeDoc
{
public:
    GcShapeDoc();
    virtual ~GcShapeDoc();
    
    void addView(GcBaseView* view);
    void removeView(GcBaseView* view);
    GcBaseView* findView(GiView* view);
    
private:
    std::vector<GcBaseView*>    _views;
};

#endif // VGLITE_CORE_SHAPEDOC_H