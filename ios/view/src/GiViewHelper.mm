//! \file GiViewHelper.mm
//! \brief 实现iOS绘图视图辅助类 GiViewHelper
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#import "GiViewHelper.h"
#import "GiGraphView.h"
#include "gicoreview.h"
#include "mgjsonstorage.h"

static GiGraphView  *_curview = nil;

@implementation GiViewHelper

+ (UIView *)createGraphView:(CGRect)frame {
    _curview = [[GiGraphView alloc]initWithFrame:frame];
    return _curview;
}

+ (UIView *)createMagnifierView:(CGRect)frame refView:(UIView *)refView {
    [GiViewHelper setView:refView];
    _curview = _curview ? [[GiGraphView alloc]initWithFrame:frame refView:_curview] : nil;
    return _curview;
}

+ (void)setView:(UIView *)view {
    _curview = [view isKindOfClass:[GiGraphView class]] ? (GiGraphView *)view : nil;
}

+ (NSString *)command {
    return _curview ? [NSString stringWithCString:[_curview coreView]->command()
                                      encoding:NSUTF8StringEncoding] : nil;
}

+ (BOOL)setCommand:(NSString *)name {
    return _curview && [_curview coreView]->setCommand([_curview viewAdapter], [name UTF8String]);
}

+ (UIImage *)snapshot {
    return [_curview snapshot];
}

+ (BOOL)savePng:(NSString *)filename {
    return _curview && [_curview savePng:filename];
}

+ (void)addShapesForTest {
    if (_curview) {
        [_curview coreView]->addShapesForTest();
    }
}

+ (void)fireGesture:(int)type state:(int)state x:(float)x y:(float)y {
    if (_curview) {
        [_curview coreView]->onGesture([_curview viewAdapter], (GiGestureType)type, 
                                    (GiGestureState)state, x, y);
    }
}

+ (void)zoomToExtent {
    if (_curview) {
        [_curview coreView]->zoomToExtent();
    }
}

+ (NSString *)content {
    MgJsonStorage s;
    const char* content = "";
    
    if (_curview && [_curview coreView]->saveShapes(s.storageForWrite())) {
        content = s.stringify(true);
    }
    
    return [NSString stringWithCString:content encoding:NSUTF8StringEncoding];
}

+ (BOOL)setContent:(NSString *)content {
    MgJsonStorage s;
    return _curview && [_curview coreView]->loadShapes(s.storageForRead([content UTF8String]));
}

+ (BOOL)loadFromFile:(NSString *)vgfile {
    NSString *content = [NSString stringWithContentsOfFile:vgfile
                                                  encoding:NSUTF8StringEncoding error:nil];
    return [GiViewHelper setContent:content];
}

+ (BOOL)saveToFile:(NSString *)vgfile {
    MgJsonStorage s;
    
    if (_curview && [_curview coreView]->saveShapes(s.storageForWrite())) {
        const char* content = s.stringify(true);
        NSData* data = [NSData dataWithBytesNoCopy:(void*)content length:strlen(content)];
        
        NSFileManager *fm = [NSFileManager defaultManager];
        [fm removeItemAtPath:vgfile error:nil];
        
        return data && [data writeToFile:vgfile atomically:NO];
    }
    
    return NO;
}

@end
