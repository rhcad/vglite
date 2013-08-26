//! \file GiViewHelper.mm
//! \brief 实现iOS绘图视图辅助类 GiViewHelper
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#import "GiViewHelper.h"
#import "GiGraphView.h"
#include "gicoreview.h"
#include "mgjsonstorage.h"

@implementation GiViewHelper

+ (GiGraphView *)createGraphView:(CGRect)frame :(UIView *)parentView {
    GiGraphView *v = [[GiGraphView alloc]initWithFrame:frame];
    [parentView addSubview:v];
    [v release];
    return v;
}

+ (GiGraphView *)createMagnifierView:(CGRect)frame
                             refView:(GiGraphView *)refView
                          parentView:(UIView *)parentView
{
    refView = refView ? refView : [GiGraphView activeView];
    GiGraphView *v = [[GiGraphView alloc]initWithFrame:frame refView:refView];
    
    [parentView addSubview:v];
    [v release];
    return v;
}

+ (NSString *)command:(GiGraphView *)v {
    return v ? [NSString stringWithCString:[v coreView]->command()
                                      encoding:NSUTF8StringEncoding] : nil;
}

+ (BOOL)setCommand:(GiGraphView *)v :(NSString *)name {
    return v && [v coreView]->setCommand([v viewAdapter], [name UTF8String]);
}

+ (UIImage *)snapshot:(GiGraphView *)v {
    return [v snapshot];
}

+ (BOOL)savePng:(GiGraphView *)v :(NSString *)filename {
    return v && [v savePng:filename];
}

+ (int)addShapesForTest:(GiGraphView *)v {
    return v ? [v coreView]->addShapesForTest() : 0;
}

+ (BOOL)fireGesture:(GiGraphView *)v type:(int)type state:(int)state x:(float)x y:(float)y {
    return v && [v coreView]->onGesture([v viewAdapter], (GiGestureType)type, 
                                        (GiGestureState)state, x, y);
}

+ (BOOL)zoomToExtent:(GiGraphView *)v {
    return v && [v coreView]->zoomToExtent();
}

+ (int)getShapeCount:(GiGraphView *)v {
    return v ? [v coreView]->getShapeCount() : 0;
}

+ (NSString *)content:(GiGraphView *)v {
    MgJsonStorage s;
    const char* content = "";
    
    if (v && [v coreView]->saveShapes(s.storageForWrite())) {
        content = s.stringify(true);
    }
    
    return [NSString stringWithCString:content encoding:NSUTF8StringEncoding];
}

+ (BOOL)setContent:(GiGraphView *)v :(NSString *)content {
    MgJsonStorage s;
    return v && [v coreView]->loadShapes(s.storageForRead([content UTF8String]));
}

+ (BOOL)loadFromFile:(GiGraphView *)v :(NSString *)vgfile {
    return v && [v coreView]->loadFromFile([vgfile UTF8String]);
}

+ (BOOL)saveToFile:(GiGraphView *)v :(NSString *)vgfile {
    BOOL ret = NO;
    NSFileManager *fm = [NSFileManager defaultManager];
    
    if (v && vgfile) {
        if ([v coreView]->getShapeCount() > 0) {
            if (![fm fileExistsAtPath:vgfile]) {
                [fm createFileAtPath:vgfile contents:[NSData data] attributes:nil];
            }
            ret = [v coreView]->saveToFile([vgfile UTF8String]);
        } else {
            ret = [fm removeItemAtPath:vgfile error:nil];
        }
    }
    
    return ret;
}

@end
