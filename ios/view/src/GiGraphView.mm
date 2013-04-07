//! \file GiGraphView.mm
//! \brief 实现iOS绘图视图类 GiGraphView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#import "GiGraphView.h"
#include "GiQuartzCanvas.h"

//! 动态图形的绘图视图类
@interface DynGiGraphView : UIView
@end

@implementation DynGiGraphView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.contentMode = UIViewContentModeRedraw; // 不缓存图像，每次重画
        self.opaque = NO;                           // 透明背景
    }
    return self;
}

- (void)drawRect:(CGRect)rect
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    GiQuartzCanvas canvas;
    
    if (canvas.beginPaint(context)) {
        canvas.endPaint();
    }
}

@end

@implementation GiGraphView

- (void)dealloc
{
    if (_dynview && _dynview != self) {
        [_dynview release];
        _dynview = nil;
    }
    delete _canvas;
    [super dealloc];
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.contentMode = UIViewContentModeRedraw; // 不缓存图像，每次重画
        self.opaque = NO;                           // 透明背景
        _canvas = new GiQuartzCanvas();
        _dynview = self;
    }
    return self;
}

- (void)didMoveToSuperview
{
    [super didMoveToSuperview];
    if (_dynview == self) {
        _dynview = [[DynGiGraphView alloc]initWithFrame:self.bounds];
        [self addSubview:_dynview];
    }
}

- (void)drawRect:(CGRect)rect
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    if (_canvas->beginPaint(context)) {         // 开始在画布上绘制
        _canvas->endPaint();                    // 结束绘制
    }
}

- (BOOL)savePng:(NSString *)filename
{
    UIGraphicsBeginImageContext(self.bounds.size);
    
    CGContextRef ctx = UIGraphicsGetCurrentContext();
    CGContextSetTextMatrix(ctx, CGAffineTransformMake(1, 0, 0, -1, 0, self.bounds.size.height));
    
    BOOL ret = NO;
    GiQuartzCanvas canvas;
    
    if (canvas.beginPaint(ctx)) {
        canvas.endPaint();
    }
    
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    NSData* imageData = UIImagePNGRepresentation(image);
    if (imageData) {
        ret = [imageData writeToFile:filename atomically:NO];                 
    }
    
    return ret;
}

@end
