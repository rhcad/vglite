//! \file IosGraphView.mm
//! \brief 实现iOS绘图视图类 IosGraphView
// Copyright (c) 2012-2013, https://github.com/rhcad/touchvg

#import "IosGraphViewImpl.h"
#import <QuartzCore/CALayer.h>
#include <math.h>

#pragma mark - IosViewAdapter implementation

IosViewAdapter::IosViewAdapter(UIView *mainView, GiCoreView *coreView)
: _view(mainView), _dynview(nil), _tmpshot(nil), _drawCount(0) {
    _coreView = new GiCoreView(coreView);
}

IosViewAdapter::~IosViewAdapter() {
    _coreView->destoryView(this);
    delete _coreView;
    [_tmpshot release];
}

UIImage * IosViewAdapter::snapshot(bool autoDraw) {
    if (!autoDraw) {
        _drawCount = 1;
    }
    long oldCount = _drawCount;
    UIImage *image = nil;
    
    UIGraphicsBeginImageContextWithOptions(_view.bounds.size, _view.opaque, 0);
    [_view.layer renderInContext:UIGraphicsGetCurrentContext()];
    
    if (autoDraw || oldCount == _drawCount) {   // 不允许renderInContext触发drawRect时返回nil
        image = UIGraphicsGetImageFromCurrentImageContext();
    }
    UIGraphicsEndImageContext();
    if (!autoDraw) {
        _drawCount = 0;
    }
    
    return image;
}

bool IosViewAdapter::drawAppend(GiCanvas* canvas) {
    if (_drawCount > 0) {   // 还在regenAppend调用中
        _drawCount++;       // 让snapshot函数返回nil
        return true;        // 不需要绘图，反正regenAppend调用snapshot将得到nil
    }
    if (_tmpshot) {
        [_tmpshot drawAtPoint:CGPointZero];         // 先绘制原来的内容
        [_tmpshot release];
        _tmpshot = nil;
        return _coreView->drawAppend(this, canvas); // 然后绘制增量图形
    }
    return false;
}

void IosViewAdapter::clearCachedData() {
    if (_tmpshot) {
        [_tmpshot release];
        _tmpshot = nil;
    }
    _coreView->clearCachedData();
}

void IosViewAdapter::regenAll() {
    [_view setNeedsDisplay];
    [_dynview setNeedsDisplay];
}

void IosViewAdapter::regenAppend() {
    [_tmpshot release];
    _tmpshot = nil;                 // renderInContext可能会调用drawRect
    _tmpshot = snapshot(false);     // 获取现有绘图快照
    [_tmpshot retain];
    
    [_view setNeedsDisplay];
    [_dynview setNeedsDisplay];
}

void IosViewAdapter::redraw() {
    if (!_dynview && _view && _view.superview) {    // 自动创建动态图形视图
        _dynview = [[IosTempView alloc]initView:_view.frame :this];
        _dynview.autoresizingMask = _view.autoresizingMask;
        [_view.superview addSubview:_dynview];
        [_dynview release];
    }
    [_dynview setNeedsDisplay];
}

bool IosViewAdapter::dispatchGesture(GiGestureType gestureType, GiGestureState gestureState, CGPoint pt) {
    return _coreView->onGesture(this, gestureType, gestureState, pt.x, pt.y);
}

bool IosViewAdapter::dispatchPan(GiGestureState gestureState, CGPoint pt, bool switchGesture) {
    return _coreView->onGesture(this, kGiGesturePan, gestureState, pt.x, pt.y, switchGesture);
}

bool IosViewAdapter::twoFingersMove(UIGestureRecognizer *sender, int state, bool switchGesture) {
    CGPoint pt1, pt2;
    
    if ([sender numberOfTouches] == 2) {
        pt1 = [sender locationOfTouch:0 inView:sender.view];
        pt2 = [sender locationOfTouch:1 inView:sender.view];
    }
    else {
        pt1 = [sender locationInView:sender.view];
        pt2 = pt1;
    }
    
    state = state < 0 ? sender.state : state;
    return _coreView->twoFingersMove(this, (GiGestureState)state, 
                                     pt1.x, pt1.y, pt2.x, pt2.y, switchGesture);
}

bool IosViewAdapter::isContextActionsVisible() {
    return false;
}

bool IosViewAdapter::showContextActions(const mgvector<int>& actions,
                                        float x, float y, float w, float h) {
    return false;
}

void IosViewAdapter::commandChanged() {
}

void IosViewAdapter::selectionChanged() {
}

#pragma mark - IosTempView implementation

@implementation IosTempView

- (id)initView:(CGRect)frame :(IosViewAdapter *)adapter {
    self = [super initWithFrame:frame];
    if (self) {
        _adapter = adapter;
        self.opaque = NO;                           // 透明背景
        self.userInteractionEnabled = NO;           // 禁止交互，避免影响主视图显示
    }
    return self;
}

- (void)drawRect:(CGRect)rect {
    IosCanvasAdapter canvas;
    
    if (canvas.beginPaint(UIGraphicsGetCurrentContext())) {
        _adapter->coreView()->dynDraw(_adapter, &canvas);
        canvas.endPaint();
    }
}

@end

#pragma mark - IosGraphView implementation

static IosGraphView* _activeGraphView = nil;
GiColor CGColorToGiColor(CGColorRef color);

@implementation IosGraphView

@synthesize panRecognizer = _panRecognizer;
@synthesize tapRecognizer = _tapRecognizer;
@synthesize twoTapsRecognizer = _twoTapsRecognizer;
@synthesize pressRecognizer = _pressRecognizer;
@synthesize pinchRecognizer = _pinchRecognizer;
@synthesize rotationRecognizer = _rotationRecognizer;
@synthesize gestureEnabled;

- (void)dealloc {
    if (_activeGraphView == self)
        _activeGraphView = nil;
    delete _adapter;
    [super dealloc];
}

- (void)initView:(GiView*)mainView :(GiCoreView*)coreView {
    self.opaque = NO;                               // 透明背景
    self.multipleTouchEnabled = YES;                // 检测多个触点
    
    GiCoreView::setScreenDpi(IosCanvasAdapter::getScreenDpi());
    [self setupGestureRecognizers];
    
    if (mainView && coreView) {
        _adapter = new IosViewAdapter(self, coreView);
        coreView->createMagnifierView(_adapter, mainView);
    }
    else {
        _adapter = new IosViewAdapter(self, NULL);
        _adapter->coreView()->createView(_adapter);
    }
}

+ (IosGraphView *)createGraphView:(CGRect)frame :(UIView *)parentView {
    IosGraphView *v = [[IosGraphView alloc]initWithFrame:frame];
    
    v.autoresizingMask = 0xFF;                      // 自动适应大小
    _activeGraphView = v;                           // 设置为当前绘图视图
    
    [v initView:NULL :NULL];
    [parentView addSubview:v];
    [v release];
    
    return v;
}

+ (IosGraphView *)createMagnifierView:(CGRect)frame
                              refView:(IosGraphView *)refView
                           parentView:(UIView *)parentView
{
    refView = refView ? refView : [IosGraphView activeView];
    if (!refView)
        return nil;
    
    IosGraphView *v = [[IosGraphView alloc]initWithFrame:frame];
    
    [v initView:[refView viewAdapter] :[refView coreView]];
    [parentView addSubview:v];
    [v release];
    
    return v;
}

+ (IosGraphView *)activeView {
    return _activeGraphView;
}

- (GiView *)viewAdapter {
    return _adapter;
}

- (GiCoreView *)coreView {
    return _adapter->coreView();
}

- (UIImage *)snapshot {
    return _adapter->snapshot(true);
}

- (BOOL)savePng:(NSString *)filename {
    BOOL ret = NO;
    NSData* imageData = UIImagePNGRepresentation([self snapshot]);
    
    if (imageData) {
        ret = [imageData writeToFile:filename atomically:NO];
    }
    
    return ret;
}

- (void)setBackgroundColor:(UIColor *)color {
    [super setBackgroundColor:color];
    
    if (!color && self.superview) {
        color = self.superview.backgroundColor;
        if (!color && self.superview.superview) {
            color = self.superview.superview.backgroundColor;
        }
    }
    if (color) {
        [self coreView]->setBkColor(_adapter, CGColorToGiColor(color.CGColor).getARGB());
    }
}

- (void)drawRect:(CGRect)rect {
    GiCoreView *coreView = _adapter->coreView();
    IosCanvasAdapter canvas;
    
    coreView->onSize(_adapter, self.bounds.size.width, self.bounds.size.height);
    
    if (canvas.beginPaint(UIGraphicsGetCurrentContext())) {
        if (!_adapter->drawAppend(&canvas)) {
            coreView->drawAll(_adapter, &canvas);
        }
        canvas.endPaint();
    }
}

- (void)clearCachedData {
    _adapter->clearCachedData();
}

- (BOOL)gestureEnabled {
    return self.userInteractionEnabled;
}

- (void)setGestureEnabled:(BOOL)enabled {
    UIGestureRecognizer *recognizers[] = {
        _pinchRecognizer, _rotationRecognizer, _panRecognizer, 
        _tapRecognizer, _twoTapsRecognizer, _pressRecognizer, nil
    };
    for (int i = 0; recognizers[i]; i++) {
        recognizers[i].enabled = enabled;
    }
    self.userInteractionEnabled = enabled;
}

- (void)activiteView {
    if (_activeGraphView != self) {
        _activeGraphView = self;
    }
}

@end
