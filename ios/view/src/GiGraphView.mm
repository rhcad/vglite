//! \file GiGraphView.mm
//! \brief 实现iOS绘图视图类 GiGraphView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#import "GiGraphView.h"
#import <QuartzCore/CALayer.h>
#include "GiQuartzCanvas.h"
#include "giview.h"
#include "gicoreview.h"

//! 动态图形的绘图视图类
@interface DynDrawView : UIView {
    GiGraphView  *_mainView;
}
@end

@implementation DynDrawView

- (id)initWithFrame:(CGRect)frame :(GiGraphView *)mainView
{
    self = [super initWithFrame:frame];
    if (self) {
        _mainView = mainView;
        self.contentMode = UIViewContentModeRedraw; // 每次重画
        self.opaque = NO;                           // 透明背景
    }
    return self;
}

- (void)drawRect:(CGRect)rect
{
    GiQuartzCanvas canvas;
    
    if (canvas.beginPaint(UIGraphicsGetCurrentContext())) {
        [_mainView coreView]->dynDraw(canvas);
        canvas.endPaint();
    }
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [_mainView touchesBegan:touches withEvent:event];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    [_mainView touchesMoved:touches withEvent:event];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    [_mainView touchesEnded:touches withEvent:event];
}

@end

//! 绘图视图适配器
class GiViewAdapter : public GiView
{
    GiGraphView *view;
public:
    UIView      *dynview;
    GiCoreView  *coreView;
    UIImage     *tmpshot;
    
    GiViewAdapter(GiGraphView *mainView)
        : view(mainView), dynview(nil), tmpshot(nil) {
        coreView = new GiCoreView();
    }
    
    ~GiViewAdapter() {
        delete coreView;
        [tmpshot release];
    }
    
    virtual void regenAll() {
        [view setNeedsDisplay];
    }
    
    virtual void regenAfterAddShape() {
        [tmpshot release];
        tmpshot = [view snapshot];
        [tmpshot retain];
        [view setNeedsDisplay];
    }
    
    virtual void redraw() {
        if (!dynview && view) {
            dynview = [[DynDrawView alloc]initWithFrame:view.frame :view];
            [view.superview addSubview:dynview];
            [dynview release];
        }
        [dynview setNeedsDisplay];
    }
};

@implementation GiGraphView

- (void)dealloc
{
    delete _viewAdapter;
    [super dealloc];
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.contentMode = UIViewContentModeRedraw; // 每次重画
        self.opaque = NO;                           // 透明背景
        _viewAdapter = new GiViewAdapter(self);
    }
    return self;
}

- (void)drawRect:(CGRect)rect
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    GiQuartzCanvas canvas;
    
    if (canvas.beginPaint(context)) {
        if (_viewAdapter->tmpshot) {
            [_viewAdapter->tmpshot drawAtPoint:CGPointZero];
            [_viewAdapter->tmpshot release];
            _viewAdapter->tmpshot = nil;
            
            [self coreView]->drawNewShape(canvas);
        }
        else {
            [self coreView]->drawAll(canvas);
        }
        canvas.endPaint();
    }
}

- (GiCoreView *)coreView
{
    return _viewAdapter->coreView;
}

- (UIImage *)snapshot
{
    CGSize size = self.bounds.size;
    
    if (UIGraphicsBeginImageContextWithOptions) {
        UIGraphicsBeginImageContextWithOptions(size, self.opaque, [UIScreen mainScreen].scale);
    } else {
        UIGraphicsBeginImageContext(size);
    }
    
    [self.layer renderInContext:UIGraphicsGetCurrentContext()];
    
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return image;
}

- (BOOL)savePng:(NSString *)filename
{
    BOOL ret = NO;
    UIImage *image = [self snapshot];
    NSData* imageData = UIImagePNGRepresentation(image);
    
    if (imageData) {
        ret = [imageData writeToFile:filename atomically:NO];                 
    }
    
    return ret;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesMoved:touches withEvent:event];
    
    UITouch *touch = [touches anyObject];
    CGPoint pt = [touch locationInView:touch.view];
    
    [self coreView]->onGesture(*_viewAdapter, kGiGesturePan,
                               kGiGestureBegan, pt.x, pt.y);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesMoved:touches withEvent:event];
    
    UITouch *touch = [touches anyObject];
    CGPoint pt = [touch locationInView:touch.view];
    
    [self coreView]->onGesture(*_viewAdapter, kGiGesturePan,
                               kGiGestureMoved, pt.x, pt.y);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesMoved:touches withEvent:event];
    
    UITouch *touch = [touches anyObject];
    CGPoint pt = [touch locationInView:touch.view];
    
    [self coreView]->onGesture(*_viewAdapter, kGiGesturePan,
                               kGiGestureEnded, pt.x, pt.y);
}

@end
