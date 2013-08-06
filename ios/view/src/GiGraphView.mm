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
    UIView      *_view;         //!< 静态图形视图, GiGraphView
    UIView      *_dynview;      //!< 动态图形视图, DynDrawView
    GiCoreView  *_coreView;     //!< 内核视图分发器
    UIImage     *_tmpshot;      //!< 用于增量绘图的临时快照
    long        _drawCount;     //!< 用于增量绘图的计数
    
public:
    
    GiViewAdapter(UIView *mainView, GiCoreView *coreView)
    : _view(mainView), _dynview(nil), _tmpshot(nil), _drawCount(0) {
        _coreView = new GiCoreView(coreView);
    }
    
    virtual ~GiViewAdapter() {
        _coreView->destoryView(this);
        delete _coreView;
        [_tmpshot release];
    }
    
    GiCoreView *coreView() {
        return _coreView;
    }
    
    UIImage *snapshot(bool autoDraw) {
        long oldCount = _drawCount;
        UIImage *image = nil;
        
        UIGraphicsBeginImageContextWithOptions(_view.bounds.size, _view.opaque, 0);
        [_view.layer renderInContext:UIGraphicsGetCurrentContext()];
        
        if (autoDraw || oldCount == _drawCount) {   // 不允许renderInContext触发drawRect时返回nil
            image = UIGraphicsGetImageFromCurrentImageContext();
        }
        UIGraphicsEndImageContext();
        
        return image;
    }
    
    bool drawAppend(GiQuartzCanvas* canvas) {
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
    
    void clearCachedData() {
        if (_tmpshot) {
            [_tmpshot release];
            _tmpshot = nil;
        }
        _coreView->clearCachedData();
    }
    
    virtual void regenAll() {
        [_view setNeedsDisplay];
        [_dynview setNeedsDisplay];
    }
    
    virtual void regenAppend() {
        _drawCount = 1;
        [_tmpshot release];
        _tmpshot = nil;                 // renderInContext可能会调用drawRect
        _tmpshot = snapshot(false);     // 获取现有绘图快照
        [_tmpshot retain];
        
        _drawCount = 0;
        [_view setNeedsDisplay];
        [_dynview setNeedsDisplay];
    }
    
    virtual void redraw() {
        if (!_dynview && _view && _view.superview) {    // 自动创建动态图形视图
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

@interface GiGraphView()<UIGestureRecognizerDelegate> {
    std::vector<CGPoint>    _points;        //!< 手势生效前的轨迹
    CGPoint                 _startPt;       //!< 开始位置
    CGPoint                 _lastPt;        //!< 上次位置
    CGPoint                 _tapPoint;      //!< 点击位置
    int                     _tapCount;      //!< 点击次数
    int                     _touchCount;    //!< 触点个数
    BOOL                    _moved;         //!< 是否已移动触点
    UIGestureRecognizerState    _state;     //!< 当前手势状态
}

- (void)setupGestureRecognizers;
- (BOOL)panHandler:(UIPanGestureRecognizer *)sender;
- (BOOL)tapHandler:(UITapGestureRecognizer *)sender;
- (BOOL)twoTapsHandler:(UITapGestureRecognizer *)sender;
- (BOOL)pressHandler:(UILongPressGestureRecognizer *)sender;
- (BOOL)pinchHandler:(UIPinchGestureRecognizer *)sender;
- (BOOL)rotationHandler:(UIRotationGestureRecognizer *)sender;
- (void)delayTap;
- (void)dispatchTapPending;
- (BOOL)gestureNotRecognized;

@end

static GiGraphView* _activeGraphView = nil;

@implementation GiGraphView

@synthesize panRecognizer, tapRecognizer, twoTapsRecognizer;
@synthesize pressRecognizer, pinchRecognizer, rotationRecognizer;

- (void)dealloc
{
    if (_activeGraphView == self)
        _activeGraphView = nil;
    delete _adapter;
    [super dealloc];
}

- (void)initView:(BOOL)mainView
{
    self.opaque = NO;                               // 透明背景
    self.multipleTouchEnabled = YES;                // 检测多个触点
    
    GiCoreView::setScreenDpi(GiQuartzCanvas::getScreenDpi());
    [self setupGestureRecognizers];
    
    if (mainView) {
        self.autoresizingMask = 0xFF;               // 自动适应大小
        _adapter = new GiViewAdapter(self, NULL);   // 将创建文档对象
        [self coreView]->createView(_adapter);
        _activeGraphView = self;
    }
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    [self initView:YES];
    return self;
}

- (id)initWithFrame:(CGRect)frame refView:(GiGraphView *)refView
{
    self = [super initWithFrame:frame];
    [self initView:!refView];
    if (self && refView) {
        _adapter = new GiViewAdapter(self, [refView coreView]);     // 将引用文档对象
        [self coreView]->createMagnifierView(_adapter, [refView viewAdapter]);
    }
    return self;
}

+ (GiGraphView *)activeView
{
    return _activeGraphView;
}

- (GiView *)viewAdapter
{
    return _adapter;
}

- (GiCoreView *)coreView
{
    return _adapter->coreView();
}

- (UIImage *)snapshot
{
    return _adapter->snapshot(true);
}

- (BOOL)savePng:(NSString *)filename
{
    BOOL ret = NO;
    NSData* imageData = UIImagePNGRepresentation([self snapshot]);
    
    if (imageData) {
        ret = [imageData writeToFile:filename atomically:NO];
    }
    
    return ret;
}

- (void)drawRect:(CGRect)rect
{
    GiCoreView *coreView = _adapter->coreView();
    GiQuartzCanvas canvas;
    
    coreView->onSize(_adapter, self.bounds.size.width, self.bounds.size.height);
    
    if (canvas.beginPaint(UIGraphicsGetCurrentContext())) {
        if (!_adapter->drawAppend(&canvas)) {
            coreView->drawAll(_adapter, &canvas);
        }
        canvas.endPaint();
    }
}

- (void)clearCachedData
{
    _adapter->clearCachedData();
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
    pressRecognizer.minimumPressDuration = 1;
    
    for (i--; i >= 0; i--) {
        recognizers[i].delegate = self;
        [self addGestureRecognizer:recognizers[i]];
    }
}

- (void)setGestureEnable:(BOOL)enabled
{
    UIGestureRecognizer *recognizers[] = {
        pinchRecognizer, rotationRecognizer, panRecognizer, 
        tapRecognizer, twoTapsRecognizer, pressRecognizer, nil
    };
    for (int i = 0; recognizers[i]; i++) {
        recognizers[i].enabled = enabled;
    }
    self.userInteractionEnabled = enabled;
}

// 手势即将开始，在 touchesBegan 后发生，即将调用本类的相应手势响应函数
- (BOOL)gestureRecognizerShouldBegin:(UIGestureRecognizer *)recognizer
{
    BOOL allow = YES;
    
    // 将状态为 UIGestureRecognizerStatePossible 的手势传递到内核，看是否允许此手势
    //
    if (recognizer == pinchRecognizer) {
        allow = [self pinchHandler:(UIPinchGestureRecognizer *)recognizer];
    }
    else if (recognizer == rotationRecognizer) {
        allow = [self rotationHandler:(UIRotationGestureRecognizer *)recognizer];
    }
    else if (recognizer == panRecognizer) {
        allow = [self panHandler:(UIPanGestureRecognizer *)recognizer];
    }
    else if (recognizer == tapRecognizer) {
        allow = [self tapHandler:(UITapGestureRecognizer *)recognizer];
    }
    else if (recognizer == twoTapsRecognizer) {
        allow = [self twoTapsHandler:(UITapGestureRecognizer *)recognizer];
    }
    else if (recognizer == pressRecognizer) {
        allow = [self pressHandler:(UILongPressGestureRecognizer *)recognizer];
    }
    
    return allow;
}

// 某个手指接触到屏幕，先于 gestureRecognizerShouldBegin 发生
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint pt = [touch locationInView:touch.view];
    
    if ([self gestureNotRecognized]) {
        _activeGraphView = self;                            // 设置为当前激活的视图
        _touchCount += [touches count];                     // 累计触点
        _moved = NO;
        _points.clear();                                    // 清除触摸轨迹
        
        if (_touchCount == 1) {                             // 是第一个触点
            _startPt = pt;                                  // 记下起始点
            _points.push_back(pt);
        }
    }
    
    [super touchesBegan:touches withEvent:event];
}

// 手势生效前会触发本事件
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint pt = [touch locationInView:touch.view];
    
    if ([self gestureNotRecognized] && !_points.empty()) {
        _points.push_back(pt);                          // 记录轨迹
    }
    
    [super touchesMoved:touches withEvent:event];
}

// 手势没有生效、手指松开时会触发本事件
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint pt = [touch locationInView:touch.view];
    
    if ([self gestureNotRecognized]) {
        _touchCount -= [touches count];
        
        if (!_points.empty()) {                             // 手势未生效，模拟分发手势
            [self dispatchTapPending];                      // 分发挂起的单击手势
            
            if ([touches count] != 1) {
                pt = _points.back();
            }
            if (_points.size() > 1 || !CGPointEqualToPoint(_startPt, pt)) {
                if (_adapter->dispatchGesture(kGiGesturePan, kGiGestureBegan, _startPt)) {
                    for (size_t i = 1; i < _points.size(); i++) {
                        _adapter->dispatchGesture(kGiGesturePan, kGiGestureMoved, _points[i]);
                    }
                    _adapter->dispatchGesture(kGiGesturePan, kGiGestureEnded, pt);
                }
            }
            else {
                _adapter->dispatchGesture(kGiGestureTap, kGiGestureEnded, pt);
            }
            _points.clear();
            _moved = NO;
        }
    }
    
    [super touchesEnded:touches withEvent:event];
}

- (BOOL)gestureNotRecognized
{
    return (_state < UIGestureRecognizerStateBegan
            || _state > UIGestureRecognizerStateEnded);
}

#pragma mark - GiBaseView gesture handlers

- (BOOL)gestureCheck:(UIGestureRecognizer*)sender
{
    _state = sender.state;
    if (sender != tapRecognizer && sender != twoTapsRecognizer) {
        [self dispatchTapPending];                  // 分发挂起的单击手势
    }
    return YES;
}

- (BOOL)gesturePost:(UIGestureRecognizer*)sender
{
    if (_moved && !_points.empty() && sender != panRecognizer) {
        if (sender != tapRecognizer) {
            _adapter->dispatchGesture(kGiGesturePan, kGiGestureCancel, _points.back());
            _moved = NO;
            _points.clear();
        }
    }
    if (sender.state == UIGestureRecognizerStateBegan) {
        _points.clear();
    }
    else if (sender.state >= UIGestureRecognizerStateEnded) {
        _touchCount = 0;
        _moved = NO;
        _points.clear();
    }
    
    return YES;
}

- (BOOL)panHandler:(UIPanGestureRecognizer *)sender
{
    if (![self gestureCheck:sender]) {
        return NO;
    }
    
    CGPoint pt = [sender locationInView:sender.view];
    const int touchCount = [sender numberOfTouches];
    BOOL ret = YES;
    
    // 检查手势的有效性
    if (sender.state == UIGestureRecognizerStatePossible) {
        _touchCount = touchCount;
        return (touchCount > 1 ? _adapter->twoFingersMove(sender)
                : _adapter->dispatchGesture(kGiGesturePan, kGiGesturePossible, _startPt));
    }
    // 切换单指与双指模式，就结束前一模式
    if (_touchCount != touchCount
        && sender.state == UIGestureRecognizerStateChanged) {
        if (_moved && _touchCount > 0) {
            _moved = NO;
            if (_touchCount > 1) {          // 双指变为单指
                _adapter->twoFingersMove(sender, kGiGestureEnded);
            }
            else {                          // 单指变为双指
                _adapter->dispatchGesture(kGiGesturePan, kGiGestureEnded, _lastPt);
            }
        }
        _touchCount = touchCount;
    }
    else {
        if (_points.size() > 1 || !CGPointEqualToPoint(_startPt, pt)) {
            if (_adapter->dispatchGesture(kGiGesturePan, kGiGestureBegan, _startPt)) {
                for (size_t i = 1; i < _points.size(); i++) {
                    _adapter->dispatchGesture(kGiGesturePan, kGiGestureMoved, _points[i]);
                }
                _adapter->dispatchGesture(kGiGesturePan, kGiGestureEnded, pt);
            }
        }
        
        GiGestureState state = (GiGestureState)sender.state;
        
        if (!_moved && sender.state <= UIGestureRecognizerStateChanged) {
            _moved = YES;
            state = kGiGestureBegan;
            pt = _startPt;
        }
        if (_touchCount > 1) {
            _adapter->twoFingersMove(sender, state);
        }
        else {
            _adapter->dispatchGesture(kGiGesturePan, state, pt);
        }
    }
    
    _lastPt = pt;
    
    return [self gesturePost:sender] && ret;
}

- (BOOL)tapHandler:(UITapGestureRecognizer *)sender
{
    CGPoint pt = [sender locationInView:sender.view];
    
    if (![self gestureCheck:sender]) {
        return NO;
    }
    if (sender.state == UIGestureRecognizerStatePossible) {
        return (_tapCount == 1 ||
                _adapter->dispatchGesture(kGiGestureTap, kGiGesturePossible, _startPt));
    }
    if (_tapCount == 1) {
        _adapter->dispatchGesture(kGiGestureTap, kGiGestureEnded, _tapPoint);
        _adapter->dispatchGesture(kGiGestureTap, kGiGesturePossible, pt);
    }
    
    _tapCount = 1;
    _tapPoint = pt;
    
    [self performSelector:@selector(delayTap) withObject:nil afterDelay:0.5];
    
    return [self gesturePost:sender];
}

- (void)delayTap
{
    if (_tapCount == 1) {
        _adapter->dispatchGesture(kGiGestureTap, kGiGestureEnded, _tapPoint);
        _tapCount = 0;
        if (_moved && !_points.empty()) {
            _adapter->dispatchGesture(kGiGesturePan, kGiGestureEnded, _points.back());
            _moved = NO;
            _points.clear();
        }
    }
}

- (void)dispatchTapPending
{
    if (_tapCount == 1) {                           // 单击还未分发
        _tapCount = 0;
        _adapter->dispatchGesture(kGiGestureTap, kGiGestureEnded, _tapPoint);
        [NSObject cancelPreviousPerformRequestsWithTarget:self selector:@selector(delayTap) object:nil];
    }
}

- (BOOL)twoTapsHandler:(UITapGestureRecognizer *)sender
{
    if (![self gestureCheck:sender]) {
        return NO;
    }
    
    if (sender.state == UIGestureRecognizerStatePossible) {
        return _adapter->dispatchGesture(kGiGestureDblTap, kGiGesturePossible, _startPt);
    }
    
    _tapCount = 0;
    
    return ([self gesturePost:sender] 
            && _adapter->dispatchGesture(kGiGestureDblTap, kGiGestureEnded, 
                                         [sender locationInView:sender.view]));
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

@end
