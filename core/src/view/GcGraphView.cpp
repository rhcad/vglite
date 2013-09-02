//! \file GcGraphView.cpp
//! \brief 实现主绘图视图类 GcGraphView
// Copyright (c) 2012-2013, https://github.com/rhcad/touchvg

#include "GcGraphView.h"
#include "mglog.h"

GcShapeView::GcShapeView(MgView* mgview, GiView *view) : GcBaseView(mgview, view)
{
}

GcShapeView::~GcShapeView()
{
}

int GcShapeView::drawAll(GiGraphics& gs)
{
    MgShapesLock locker(MgShapesLock::ReadOnly, doc());
    return doc()->draw(gs);
}

int GcShapeView::drawAppend(const int* newids, GiGraphics& gs)
{
    MgShapesLock locker(MgShapesLock::ReadOnly, doc());
    int n = 0;
    for (; *newids; newids++) {
        MgShape* sp = shapes()->findShape(*newids);
        if (sp && sp->draw(0, gs)) {
            n++;
        }
    }
    return n;
}

void GcShapeView::dynDraw(const MgMotion&, GiGraphics&)
{
}

void GcShapeView::onSize(int dpi, int w, int h)
{
    xform()->setResolution((float)dpi);
    xform()->setWndSize(w, h);
}

bool GcShapeView::onGesture(const MgMotion& motion)
{
    LOGD("GcShapeView::onGesture %d, %d", motion.gestureType, motion.gestureState);
    if (motion.gestureType != kGiGesturePan){
        return false;
    }
    if (motion.gestureState == kMgGestureBegan) {
        _lastScale = xform()->getZoomValue(_lastCenter);
    }
    if (motion.gestureState == kMgGestureMoved) {
        Vector2d vec(motion.point - motion.startPt);
        xform()->zoom(_lastCenter, _lastScale);     // 先恢复
        xform()->zoomPan(vec.x, vec.y);             // 平移到当前点
        
        cmdView()->regenAll();
    }
    
    return true;
}

bool GcShapeView::twoFingersMove(const MgMotion& motion)
{
    LOGD("GcShapeView::twoFingersMove %d, %d", motion.gestureType, motion.gestureState);
    if (motion.gestureState == kMgGestureBegan) {
        _lastScale = xform()->getZoomValue(_lastCenter);
    }
    if (motion.gestureState == kMgGestureMoved
        && motion.startPt != motion.startPt2
        && motion.point != motion.point2) {         // 双指变单指则忽略移动
        Point2d at((motion.startPt + motion.startPt2) / 2);
        Point2d pt((motion.point + motion.point2) / 2);
        float d1 = motion.point.distanceTo(motion.point2);
        float d0 = motion.startPt.distanceTo(motion.startPt2);
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
