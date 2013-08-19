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
    NSString *content = [NSString stringWithContentsOfFile:vgfile
                                                  encoding:NSUTF8StringEncoding error:nil];
    return [GiViewHelper setContent:v :content];
}

+ (BOOL)saveToFile:(GiGraphView *)v :(NSString *)vgfile {
    MgJsonStorage s;
    
    if (v && [v coreView]->saveShapes(s.storageForWrite())) {
        const char* content = s.stringify(true);
        NSData* data = [NSData dataWithBytesNoCopy:(void*)content length:strlen(content)];
        
        NSFileManager *fm = [NSFileManager defaultManager];
        [fm removeItemAtPath:vgfile error:nil];
        
        return data && [data writeToFile:vgfile atomically:NO];
    }
    
    return NO;
}

@end
