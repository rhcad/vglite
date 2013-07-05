//! \file GcShapeDoc.h
//! \brief 定义图形文档类 GcShapeDoc
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#ifndef VGLITE_CORE_SHAPEDOC_H
#define VGLITE_CORE_SHAPEDOC_H

#include <vector>

class GcBaseView;
class GiView;
class MgShapeDoc;

//! 图形文档类
/*! \ingroup GROUP_VIEW
 */
class GcShapeDoc
{
public:
    GcShapeDoc();
    ~GcShapeDoc();
    
    void addView(GcBaseView* view);
    void removeView(GcBaseView* view);
    GcBaseView* findView(GiView* view) const;
    GcBaseView* getView(int index) const;
    int getViewCount() const;
    GcBaseView* firstView() const;
    
private:
    std::vector<GcBaseView*>    _views;
    MgShapeDoc*     _doc;
};

#endif // VGLITE_CORE_SHAPEDOC_H
