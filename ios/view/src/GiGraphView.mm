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
    GiViewAdapter   *_viewAdapter;
}

- (id)initWithFrame:(CGRect)frame :(GiViewAdapter *)viewAdapter;

@end

//! 绘图视图适配器
class GiViewAdapter : public GiView
{
private:
    UIView      *_view;
    UIView      *_dynview;
    GiCoreView  *_coreView;
    UIImage     *_tmpshot;
    
public:
    GiViewAdapter(UIView *mainView, int viewType, GiCoreView *coreView)
    : _view(mainView), _dynview(nil), _tmpshot(nil) {
        _coreView = new GiCoreView(coreView);
        _coreView->createView(this, viewType);
    }
    
    virtual ~GiViewAdapter() {
        _coreView->destoryView(this);
        [_tmpshot release];
    }
    
    GiCoreView *coreView() {
        return _coreView;
    }
    
    UIImage *snapshot() {
        UIGraphicsBeginImageContextWithOptions(_view.bounds.size, _view.opaque, 0);
        [_view.layer renderInContext:UIGraphicsGetCurrentContext()];
        UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
        UIGraphicsEndImageContext();
        return image;
    }
    
    bool drawAppend(GiQuartzCanvas* canvas) {
        if (_tmpshot) {
            [_tmpshot drawAtPoint:CGPointZero];
            [_tmpshot release];
            _tmpshot = nil;
            return _coreView->drawAppend(this, canvas);
        }
        return false;
    }
    
    virtual void regenAll() {
        [_view setNeedsDisplay];
        [_dynview setNeedsDisplay];
    }
    
    virtual void regenAppend() {
        [_tmpshot release];
        _tmpshot = nil;                 // renderInContext可能会调用drawRect
        _tmpshot = snapshot();
        [_tmpshot retain];
        
        [_view setNeedsDisplay];
        [_dynview setNeedsDisplay];
    }
    
    virtual void redraw() {
        if (!_dynview && _view) {       // 自动创建动态图形视图
            _dynview = [[DynDrawView alloc]initWithFrame:_view.frame :this];
            _dynview.autoresizingMask = _view.autoresizingMask;
            [_view.superview addSubview:_dynview];
            [_dynview release];
        }
        [_dynview setNeedsDisplay];
    }
};

@implementation DynDrawView

- (id)initWithFrame:(CGRect)frame :(GiViewAdapter *)viewAdapter
{
    self = [super initWithFrame:frame];
    if (self) {
        _viewAdapter = viewAdapter;
        self.opaque = NO;                           // 透明背景
        self.userInteractionEnabled = NO;           // 禁止交互，避免影响主视图显示
    }
    return self;
}

- (void)drawRect:(CGRect)rect
{
    GiQuartzCanvas canvas;
    
    if (canvas.beginPaint(UIGraphicsGetCurrentContext())) {
        _viewAdapter->coreView()->dynDraw(_viewAdapter, &canvas);
        canvas.endPaint();
    }
}

@end

@implementation GiBaseView

@synthesize panRecognizer, tapRecognizer, twoTapsRecognizer, pressRecognizer;
@synthesize pinchRecognizer, rotationRecognizer, twoFingersPanRecognizer;

- (void)dealloc
{
    delete _viewAdapter;
    [super dealloc];
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.opaque = NO;                           // 透明背景
        
        GiCoreView::setScreenDpi(GiQuartzCanvas::getScreenDpi());
        [self setupGestureRecognizers];
    }
    return self;
}

- (GiCoreView *)coreView
{
    return _viewAdapter->coreView();
}

- (UIImage *)snapshot
{
    return _viewAdapter->snapshot();
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

- (void)setupGestureRecognizers
{
    UIGestureRecognizer *recognizers[7];
    int i = 0;
    
    recognizers[i++] = pinchRecognizer =
    [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(pinchHandler:)];
    
    recognizers[i++] = rotationRecognizer =
    [[UIRotationGestureRecognizer alloc] initWithTarget:self action:@selector(rotationHandler:)];
    
    recognizers[i++] = twoFingersPanRecognizer =
    [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(twoFingersPanHandler:)];
    twoFingersPanRecognizer.maximumNumberOfTouches = 2;
    twoFingersPanRecognizer.minimumNumberOfTouches = 2;
    
    recognizers[i++] = panRecognizer =
    [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(panHandler:)];
    panRecognizer.maximumNumberOfTouches = 2;                       // 允许单指滑动变为双指滑动
    
    recognizers[i++] = tapRecognizer =
    [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapHandler:)];
    [tapRecognizer requireGestureRecognizerToFail:panRecognizer];   // 不是滑动才算点击
    
    recognizers[i++] = twoTapsRecognizer =
    [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(twoTapsHandler:)];
    twoTapsRecognizer.numberOfTapsRequired = 2;
    
    recognizers[i++] = pressRecognizer =
    [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(pressHandler:)];
    pressRecognizer.minimumPressDuration = 0.8;
    
    for (i--; i >= 0; i--) {
        recognizers[i].delegate = self;
        [self addGestureRecognizer:recognizers[i]];
    }
}

- (BOOL)panHandler:(UIPanGestureRecognizer *)sender
{
    CGPoint pt = [sender locationInView:sender.view];
    [self coreView]->onGesture(_viewAdapter, kGiGesturePan,
                               (GiGestureState)[sender state], pt.x, pt.y);
    return YES;
}

- (BOOL)tapHandler:(UITapGestureRecognizer *)sender
{
    return YES;
}

- (BOOL)twoTapsHandler:(UITapGestureRecognizer *)sender
{
    return YES;
}

- (BOOL)pressHandler:(UILongPressGestureRecognizer *)sender
{
    return YES;
}

- (BOOL)pinchHandler:(UIPinchGestureRecognizer *)sender
{
    return YES;
}

- (BOOL)rotationHandler:(UIRotationGestureRecognizer *)sender
{
    return YES;
}

- (BOOL)twoFingersPanHandler:(UIPanGestureRecognizer *)sender
{
    return YES;
}

@end

@implementation GiGraphView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.autoresizingMask = 0xFF;               // 自动适应大小
        _viewAdapter = new GiViewAdapter(self, 1, NULL);    // 将创建文档对象
    }
    return self;
}

- (void)drawRect:(CGRect)rect
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    GiQuartzCanvas canvas;
    
    [self coreView]->onSize(_viewAdapter, self.bounds.size.width, self.bounds.size.height);
    
    if (canvas.beginPaint(context)) {
        if (!_viewAdapter->drawAppend(&canvas)) {
            [self coreView]->drawAll(_viewAdapter, &canvas);
        }
        canvas.endPaint();
    }
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesMoved:touches withEvent:event];
    
    UITouch *touch = [touches anyObject];
    CGPoint pt = [touch locationInView:touch.view];
    
    [self coreView]->onGesture(_viewAdapter, kGiGesturePan,
                               kGiGestureBegan, pt.x, pt.y);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesMoved:touches withEvent:event];
    
    UITouch *touch = [touches anyObject];
    CGPoint pt = [touch locationInView:touch.view];
    
    [self coreView]->onGesture(_viewAdapter, kGiGesturePan,
                               kGiGestureMoved, pt.x, pt.y);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesMoved:touches withEvent:event];
    
    UITouch *touch = [touches anyObject];
    CGPoint pt = [touch locationInView:touch.view];
    
    [self coreView]->onGesture(_viewAdapter, kGiGesturePan,
                               kGiGestureEnded, pt.x, pt.y);
}

@end

@implementation GiMagnifierView

- (id)initWithFrame:(CGRect)frame { return nil; }

- (id)initWithFrame:(CGRect)frame :(GiGraphView *)mainView
{
    self = [super initWithFrame:frame];
    if (self) {
        _viewAdapter = new GiViewAdapter(self, 2, [mainView coreView]); // 将引用文档对象
    }
    return self;
}

- (void)drawRect:(CGRect)rect
{
    GiCoreView *coreView = _viewAdapter->coreView();
    CGContextRef context = UIGraphicsGetCurrentContext();
    GiQuartzCanvas canvas;
        
    coreView->onSize(_viewAdapter, self.bounds.size.width, self.bounds.size.height);
    
    if (canvas.beginPaint(context)) {
        if (!_viewAdapter->drawAppend(&canvas)) {
            coreView->drawAll(_viewAdapter, &canvas);
        }
        canvas.endPaint();
    }
}

@end
