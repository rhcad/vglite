//! \file GcGraphView.cpp
//! \brief 实现主绘图视图类 GcGraphView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#include "GcGraphView.h"
#include <RandomShape.h>

GcShapeView::GcShapeView(MgView* mgview, GiView *view) : GcBaseView(mgview, view)
{
    RandomParam(200).addShapes(mgview->shapes());
}

GcShapeView::~GcShapeView()
{
}

void GcShapeView::drawAll(GiCanvas* canvas)
{
    if (graph()->beginPaint(canvas)) {
        cmdView()->doc()->draw(*graph());
        graph()->endPaint();
    }
}

void GcShapeView::drawAppend(GiCanvas* canvas)
{
}

void GcShapeView::dynDraw(GiCanvas* canvas)
{
}

void GcShapeView::onSize(int dpi, int w, int h)
{
    xform()->setResolution((float)dpi);
    xform()->setWndSize(w, h);
}

bool GcShapeView::onGesture(GiGestureType gestureType,
                            GiGestureState gestureState, float x, float y)
{
    if (gestureType != kGiGesturePan)
        return false;
    
    if (gestureState == kGiGesturePossible) {
        return true;
    }
    if (gestureState == kGiGestureBegan) {
        _lastScale = xform()->getZoomValue(_lastCenter);
        _startPt[0].set(x, y);
    }
    if (gestureState == kGiGestureMoved) {
        xform()->zoom(_lastCenter, _lastScale);     // 先恢复
        xform()->zoomPan(x - _startPt[0].x, y - _startPt[0].y); // 平移到当前点
        
        cmdView()->regenAll();
    }
    
    return true;
}

bool GcShapeView::twoFingersMove(GiGestureState gestureState,
                                 float x1, float y1, float x2, float y2)
{
    if (gestureState == kGiGesturePossible) {
        return true;
    }
    if (gestureState == kGiGestureBegan) {
        _lastScale = xform()->getZoomValue(_lastCenter);
        _startPt[0].set(x1, y1);
        _startPt[1].set(x2, y2);
    }
    if (gestureState == kGiGestureMoved && _startPt[0] != _startPt[1]) {
        Point2d at((_startPt[0] + _startPt[1]) / 2);
        Point2d pt((x1 + x2) / 2, (y1 + y2) / 2);
        float d1 = Point2d(x1, y1).distanceTo(Point2d(x2, y2));
        float d0 = _startPt[0].distanceTo(_startPt[1]);
        float scale = d1 / d0;
        
        xform()->zoom(_lastCenter, _lastScale);     // 先恢复
        xform()->zoomByFactor(scale - 1, &at);      // 以起始点为中心放缩显示
        xform()->zoomPan(pt.x - at.x, pt.y - at.y); // 平移到当前点
        
        cmdView()->regenAll();
    }
    
    return true;
}

// GcGraphView
//

GcGraphView::GcGraphView(MgView* mgview, GiView *view) : GcShapeView(mgview, view)
{
}

GcGraphView::~GcGraphView()
{
}
