//! \file GcShapeDoc.cpp
//! \brief 实现图形文档类 GcShapeDoc
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#include "GcShapeDoc.h"
#include "GcBaseView.h"

GcShapeDoc::GcShapeDoc()
{
}

GcShapeDoc::~GcShapeDoc()
{
}

void GcShapeDoc::addView(GcBaseView* view)
{
    if (view && view->deviceView() && !view->doc()
        && !findView(view->deviceView())) {
        _views.push_back(view);
        view->setDoc(this);
    }
}

void GcShapeDoc::removeView(GcBaseView* view)
{
    std::vector<GcBaseView*>::iterator it;
    
    for (it = _views.begin(); it != _views.end(); ++it) {
        if (*it == view) {
            view->setDoc(NULL);
            _views.erase(it);
            break;
        }
    }
}

GcBaseView* GcShapeDoc::findView(GiView* view)
{
    std::vector<GcBaseView*>::iterator it;
    
    for (it = _views.begin(); it != _views.end(); ++it) {
        if ((*it)->deviceView() == view) {
            return *it;
        }
    }
    return NULL;
}
