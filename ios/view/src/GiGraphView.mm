//! \file GiGraphView.mm
//! \brief 实现iOS绘图视图类 GiGraphView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#import "GiGraphView.h"
#import <QuartzCore/CALayer.h>
#include "GiQuartzCanvas.h"
#include "giview.h"
#include "gicoreview.h"
#include <vector>

#pragma mark - DynDrawView declaration

//! 动态图形的绘图视图类
@interface DynDrawView : UIView {
    GiViewAdapter   *_adapter;
}

- (id)initView:(CGRect)frame :(GiViewAdapter *)adapter;

@end

#pragma mark - GiViewAdapter declaration

//! 绘图视图适配器
class GiViewAdapter : public GiView
{
private:
    UIView      *_view;
    UIView      *_dynview;
    GiCoreView  *_coreView;
    UIImage     *_tmpshot;
    
public:
    GiViewAdapter(UIView *mainView, GiCoreView *coreView)
    : _view(mainView), _dynview(nil), _tmpshot(nil) {
        _coreView = new GiCoreView(coreView);
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
            _dynview = [[DynDrawView alloc]initView:_view.frame :this];
            _dynview.autoresizingMask = _view.autoresizingMask;
            [_view.superview addSubview:_dynview];
            [_dynview release];
        }
        [_dynview setNeedsDisplay];
    }
    
    bool dispatchGesture(GiGestureType gestureType, GiGestureState gestureState, CGPoint pt) {
        return _coreView->onGesture(this, gestureType, gestureState, pt.x, pt.y);
    }
    
    bool twoFingersMove(UIGestureRecognizer *sender, int state = -1) {
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
                                         pt1.x, pt1.y, pt2.x, pt2.y);
    }
};

#pragma mark - DynDrawView implementation

@implementation DynDrawView

- (id)initView:(CGRect)frame :(GiViewAdapter *)adapter
{
    self = [super initWithFrame:frame];
    if (self) {
        _adapter = adapter;
        self.opaque = NO;                           // 透明背景
        self.userInteractionEnabled = NO;           // 禁止交互，避免影响主视图显示
    }
    return self;
}

- (void)drawRect:(CGRect)rect
{
    GiQuartzCanvas canvas;
    
    if (canvas.beginPaint(UIGraphicsGetCurrentContext())) {
        _adapter->coreView()->dynDraw(_adapter, &canvas);
        canvas.endPaint();
    }
}

@end

#pragma mark - GiBaseView implementation

@interface GiBaseView()<UIGestureRecognizerDelegate> {
    NSTimeInterval          _timeBegan;     //!< 开始触摸时的时刻
    std::vector<CGPoint>    _points;        //!< 手势生效前的轨迹
    CGPoint                 _tapPoint;      //!< 点击位置
    int                     _tapCount;      //!< 点击次数
    int                     _touchCount;    //!< 触点个数
    BOOL                    _moved;         //!< 是否已移动触点
}

- (void)setupGestureRecognizers;
- (BOOL)panHandler:(UIPanGestureRecognizer *)sender;
- (BOOL)tapHandler:(UITapGestureRecognizer *)sender;
- (BOOL)twoTapsHandler:(UITapGestureRecognizer *)sender;
- (BOOL)pressHandler:(UILongPressGestureRecognizer *)sender;
- (BOOL)pinchHandler:(UIPinchGestureRecognizer *)sender;
- (BOOL)rotationHandler:(UIRotationGestureRecognizer *)sender;
- (BOOL)twoFingersPanHandler:(UIPanGestureRecognizer *)sender;
- (BOOL)delayTap;

@end

@implementation GiBaseView

@synthesize panRecognizer, tapRecognizer, twoTapsRecognizer, pressRecognizer;
@synthesize pinchRecognizer, rotationRecognizer, twoFingersPanRecognizer;

- (void)dealloc
{
    delete _adapter;
    [super dealloc];
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.opaque = NO;                           // 透明背景
        self.multipleTouchEnabled = YES;            // 检测多个触点
        
        GiCoreView::setScreenDpi(GiQuartzCanvas::getScreenDpi());
        [self setupGestureRecognizers];
    }
    return self;
}

- (GiViewAdapter *)viewAdapter
{
    return _adapter;
}

- (GiCoreView *)coreView
{
    return _adapter->coreView();
}

- (UIImage *)snapshot
{
    return _adapter->snapshot();
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

- (void)drawRect:(CGRect)rect
{
    GiCoreView *coreView = _adapter->coreView();
    CGContextRef context = UIGraphicsGetCurrentContext();
    GiQuartzCanvas canvas;
    
    coreView->onSize(_adapter, self.bounds.size.width, self.bounds.size.height);
    
    if (canvas.beginPaint(context)) {
        if (!_adapter->drawAppend(&canvas)) {
            coreView->drawAll(_adapter, &canvas);
        }
        canvas.endPaint();
    }
}

#pragma mark - GiBaseView gesture recognization

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
    panRecognizer.maximumNumberOfTouches = 2;                       // 允许单指拖动变为双指拖动
    
    recognizers[i++] = tapRecognizer =
    [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapHandler:)];
    [tapRecognizer requireGestureRecognizerToFail:panRecognizer];   // 不是拖动才算点击
    
    recognizers[i++] = twoTapsRecognizer =
    [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(twoTapsHandler:)];
    twoTapsRecognizer.numberOfTapsRequired = 2;
    
    recognizers[i++] = pressRecognizer =
    [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(pressHandler:)];
    pressRecognizer.minimumPressDuration = 0.8;                     // 默认0.8秒
    
    for (i--; i >= 0; i--) {
        recognizers[i].delegate = self;
        [self addGestureRecognizer:recognizers[i]];
    }
}

// 手势即将开始，在 touchesBegan 后发生，即将调用本类的相应手势响应函数
- (BOOL)gestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer
{
    BOOL allow = YES;
    NSTimeInterval seconds = [[NSProcessInfo processInfo]systemUptime] - _timeBegan;
    
    // 从按下到开始移动经过的秒数超过阀值，就向当前命令触发长按动作，当前命令响应长按操作时手势取消
    if (gestureRecognizer != pressRecognizer && seconds > 0.8) {
        if ([self pressHandler:(UILongPressGestureRecognizer *)gestureRecognizer]) {
            return NO;
        }
    }
    
    // 将状态为 UIGestureRecognizerStatePossible 的手势传递到内核，看是否允许此手势
    //
    if (gestureRecognizer == pinchRecognizer) {
        allow = [self pinchHandler:(UIPinchGestureRecognizer *)gestureRecognizer];
    }
    else if (gestureRecognizer == rotationRecognizer) {
        allow = [self rotationHandler:(UIRotationGestureRecognizer *)gestureRecognizer];
    }
    else if (gestureRecognizer == twoFingersPanRecognizer) {
        allow = [self twoFingersPanHandler:(UIPanGestureRecognizer *)gestureRecognizer];
    }
    else if (gestureRecognizer == panRecognizer) {
        allow = ([gestureRecognizer numberOfTouches] == 1    // 只在移动过程中识别第二个触点
                 && [self panHandler:(UIPanGestureRecognizer *)gestureRecognizer]);
    }
    else if (gestureRecognizer == tapRecognizer) {
        allow = [self tapHandler:(UITapGestureRecognizer *)gestureRecognizer];
    }
    else if (gestureRecognizer == twoTapsRecognizer) {
        allow = [self twoTapsHandler:(UITapGestureRecognizer *)gestureRecognizer];
    }
    else if (gestureRecognizer == pressRecognizer) {
        allow = [self pressHandler:(UILongPressGestureRecognizer *)gestureRecognizer];
    }
    
    return allow;
}

// 某个手指接触到屏幕，先于 gestureRecognizerShouldBegin 发生
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint pt = [touch locationInView:touch.view];
    
    _points.clear();
    if (_timeBegan < 0.1 && [touches count] == 1) {     // 是第一个触点
        _timeBegan = touch.timestamp;                   // 记下第一个触点的时刻
        _points.push_back(pt);                          // 记下起始点
        _moved = NO;
        if (_tapCount == 1) {
            [self delayTap];
        }
    }
    else if (_moved) {
        _moved = NO;
        _adapter->dispatchGesture(kGiGesturePan, kGiGestureCancel, pt);
    }
    
    if (touch.view == self) {
        [super touchesBegan:touches withEvent:event];
    }
}

// 手势生效前会触发本事件
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint pt = [touch locationInView:touch.view];
    
    if ([touches count] == 1 && !_points.empty()) {
        _points.push_back(pt);
        
        if (!_moved) {                                  // 分发拖动开始
            _moved = YES;
            _adapter->dispatchGesture(kGiGesturePan, kGiGestureBegan, _points.front());
        }
        _adapter->dispatchGesture(kGiGesturePan, kGiGestureMoved, pt);  // 分发拖动
    }
    
    if (touch.view == self) {
        [super touchesMoved:touches withEvent:event];
    }
}

// 手势没有生效、手指松开时会触发本事件
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint pt = [touch locationInView:touch.view];
    
    if (!_points.empty()) {         // 手势未生效，模拟分发手势
        if ([touches count] != 1) {
            pt = _points.back();
        }
        if (_moved) {
            _adapter->dispatchGesture(kGiGesturePan, kGiGestureEnded, pt);
        }
        else {
            _adapter->dispatchGesture(kGiGestureTap, kGiGestureEnded, pt);
        }
    }
    
    _timeBegan = 0;
    _points.clear();
    _moved = NO;
    
    if (touch.view == self) {
        [super touchesEnded:touches withEvent:event];
    }
}

// 手势生效时会触发本事件
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    
    if (!_points.empty() && _moved) {   // 手势未生效，取消已分发的模拟手势
        _adapter->dispatchGesture(kGiGesturePan, kGiGestureCancel, _points.back());
    }
    
    _timeBegan = 0;
    _points.clear();
    _moved = NO;
    
    if (!touch || touch.view == self) {
        [super touchesCancelled:touches withEvent:event];
    }
}

#pragma mark - GiBaseView gesture handlers

- (BOOL)gestureCheck:(UIGestureRecognizer*)sender
{
    return YES;
}

- (BOOL)gesturePost:(UIGestureRecognizer*)sender
{
    if (sender.state == UIGestureRecognizerStateBegan
        || sender.state >= UIGestureRecognizerStateEnded) {
        _timeBegan = 0;
        _points.clear();
        _moved = NO;
        _touchCount = 0;
    }
    
    return YES;
}

- (BOOL)panHandler:(UIPanGestureRecognizer *)sender
{
    if (![self gestureCheck:sender]) {
        return NO;
    }
    
    CGPoint pt = [sender locationInView:sender.view];
    
    // 检查手势的有效性
    if (sender.state == UIGestureRecognizerStatePossible) {
        return _adapter->dispatchGesture(kGiGesturePan, kGiGesturePossible, pt);
    }
    // 在 touchesMoved 中已经分发了开始状态，就转为移动状态分发
    if (sender.state == UIGestureRecognizerStateBegan && !_points.empty()) {
        _touchCount = 1;
        _tapPoint = pt;
        return _adapter->dispatchGesture(kGiGesturePan, kGiGestureMoved, pt);
    }
    // 切换单指与双指模式，就结束前一模式
    if (_touchCount != [sender numberOfTouches]
        && sender.state == UIGestureRecognizerStateChanged) {
        if (_moved && _touchCount > 0) {
            _moved = NO;
            if (_touchCount > 1) {          // 双指变为单指
                _adapter->twoFingersMove(sender, kGiGestureEnded);
            }
            else {                          // 单指变为双指
                _adapter->dispatchGesture(kGiGesturePan, kGiGestureEnded, _tapPoint);
            }
        }
        _touchCount = [sender numberOfTouches];
    }
    else {
        GiGestureState state = (GiGestureState)sender.state;
        
        if (!_moved && sender.state == UIGestureRecognizerStateChanged) {
            _moved = YES;
            state = kGiGestureBegan;
        }
        if (_touchCount > 1) {
            _adapter->twoFingersMove(sender, state);
        }
        else {
            _adapter->dispatchGesture(kGiGesturePan, state, pt);
        }
    }
    
    _tapPoint = pt;
    
    return [self gesturePost:sender];
}

- (BOOL)tapHandler:(UITapGestureRecognizer *)sender
{
    if (![self gestureCheck:sender] || _tapCount != 0) {
        return NO;
    }
    
    CGPoint pt = [sender locationInView:sender.view];
    
    if (sender.state == UIGestureRecognizerStatePossible) {
        return _adapter->dispatchGesture(kGiGestureTap, kGiGesturePossible, pt);
    }
    
    _tapCount = 1;
    _tapPoint = pt;
    [self performSelector:@selector(delayTap) withObject:sender afterDelay:0.5];
    
    return [self gesturePost:sender];
}

- (BOOL)delayTap
{
    BOOL ret = (_tapCount == 1);
    _tapCount = 0;
    return ret && _adapter->dispatchGesture(kGiGestureTap, kGiGestureEnded, _tapPoint);
}

- (BOOL)twoTapsHandler:(UITapGestureRecognizer *)sender
{
    if (![self gestureCheck:sender]) {
        return NO;
    }
    
    _tapCount = 0;
    _tapPoint = [sender locationInView:sender.view];
    
    if (sender.state == UIGestureRecognizerStatePossible) {
        return _adapter->dispatchGesture(kGiGestureDblTap, kGiGesturePossible, _tapPoint);
    }
    
    return ([self gesturePost:sender] 
            && _adapter->dispatchGesture(kGiGestureDblTap, kGiGestureEnded, _tapPoint));
}

- (BOOL)pressHandler:(UILongPressGestureRecognizer *)sender
{
    return ([self gestureCheck:sender]
            && [self gesturePost:sender]
            && _adapter->dispatchGesture(kGiGesturePress, (GiGestureState)sender.state, 
                                         [sender locationInView:sender.view]));
}

- (BOOL)pinchHandler:(UIPinchGestureRecognizer *)sender
{
    return ([self gestureCheck:sender]
            && [self gesturePost:sender]
            && _adapter->twoFingersMove(sender));
}

- (BOOL)rotationHandler:(UIRotationGestureRecognizer *)sender
{
    return ([self gestureCheck:sender]
            && [self gesturePost:sender]
            && _adapter->twoFingersMove(sender));
}

- (BOOL)twoFingersPanHandler:(UIPanGestureRecognizer *)sender
{
    return ([self gestureCheck:sender]
            && [self gesturePost:sender]
            && _adapter->twoFingersMove(sender));
}

@end

#pragma mark - GiGraphView implementation

@implementation GiGraphView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.autoresizingMask = 0xFF;                   // 自动适应大小
        _adapter = new GiViewAdapter(self, NULL);       // 将创建文档对象
        [self coreView]->createView(_adapter, 1);
    }
    return self;
}

@end

#pragma mark - GiMagnifierView implementation

@implementation GiMagnifierView

- (id)initWithFrame:(CGRect)frame { return nil; }

- (id)initWithFrame:(CGRect)frame refView:(GiGraphView *)refView
{
    self = [super initWithFrame:frame];
    if (self) {
        _adapter = new GiViewAdapter(self, [refView coreView]);     // 将引用文档对象
        [self coreView]->createMagnifierView(_adapter, [refView viewAdapter]);
    }
    return self;
}

@end
