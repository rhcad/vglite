// GraphView1.mm
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#import "GraphView1.h"
#import "CalloutView.h"
#import <QuartzCore/CALayer.h>  // renderInContext
#include "GiQuartzCanvas.h"
#include <testcanvas.h>
#include <mach/mach_time.h>

static int machToMs(uint64_t start)
{
    uint64_t elapsedTime = mach_absolute_time() - start;
    static double ticksToNanoseconds = -1.0;
    
    if (ticksToNanoseconds < 0) {
        mach_timebase_info_data_t timebase;
        mach_timebase_info(&timebase);
        ticksToNanoseconds = (double)timebase.numer / timebase.denom * 1e-6;
    }
    
    return (int)(elapsedTime * ticksToNanoseconds);
}

//! 绘制动态图形的视图类
@interface DynGraphView1 : UIView
@end

@implementation DynGraphView1

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
    GraphView1 *parent = (GraphView1 *)self.superview;
    CGContextRef context = UIGraphicsGetCurrentContext();
    GiQuartzCanvas canvas;
    
    if (canvas.beginPaint(context)) {
        [parent dynDraw:&canvas];
        canvas.endPaint();
    }
}

@end

@implementation GraphView1

- (void)dealloc
{
    if (_dynview && _dynview != self) {
        [_dynview release];
        _dynview = nil;
    }
    delete _canvas;
    [super dealloc];
}

- (id)initWithFrame:(CGRect)frame withTests:(int)t
{
    self = [super initWithFrame:frame];
    if (self) {
        self.contentMode = UIViewContentModeRedraw; // 不缓存图像，每次重画
        self.opaque = NO;                           // 透明背景
        
        _tests = t;
        _canvas = new GiQuartzCanvas();
        _dynview = self;
        if (_tests & 0x10000) {                     // 使用内嵌视图来绘制动态图形
            _dynview = [[DynGraphView1 alloc]initWithFrame:self.bounds];
            [self addSubview:_dynview];             // 添加到本静态图形视图中
        }
        
        _lineWidth = 1;
        _lineStyle = 0;
        _useFill = false;
        _antiAlias = true;
        _flatness = 3;
    }
    return self;
}

- (void)dynDraw:(GiQuartzCanvas*)canvas
{
    static float phase = 0;
    phase += 1;
    canvas->setPen(0, 0, 1, phase);
    canvas->setBrush(0x88005500, 0);
    canvas->drawEllipse(_lastpt.x - 50, _lastpt.y - 50, 100, 100, true, true);
}

- (void)draw:(GiQuartzCanvas*)canvas
{
    CGContextRef c = canvas->context();
    
    CGContextSetFlatness(c, _flatness);
    CGContextSetAllowsAntialiasing(c, _antiAlias);
    canvas->setPen(0, _lineWidth, _lineStyle, 0);
    canvas->setBrush(_useFill ? 0x4400ff00 : 0, 0);
    
    if (_tests & 0x20000) {     // in scroll view
        TestCanvas::test(*canvas, _tests, 200);
    }
    else {
        TestCanvas::test(*canvas, _tests, 100);
    }
}

- (void)drawRect:(CGRect)rect
{
    uint64_t start = mach_absolute_time();
    CGContextRef context = UIGraphicsGetCurrentContext();

    if (_canvas->beginPaint(context)) {         // 开始在画布上绘制
        [self draw:_canvas];                    // 绘制测试图形
        if (_dynview == self)
            [self dynDraw:_canvas];             // 绘制动态图形
        _canvas->endPaint();                    // 结束绘制
    }
    
    int drawTime = machToMs(start);
    bool inthread = ((_tests & 0x40000) != 0);
    
    [self showDrawnTime:drawTime logging:!inthread];
    if (_drawTimes > 0) {
        _drawTimes++;
    }
    if (inthread && _testOrder >= 0 && (_tests & 0x400) == 0) {
        if (_testOrder == 0) {
            NSLog(@"\torder\tantiAlias\tfill\tstyle\tflatness\twidth\ttime(ms)");
        }
        else {
            NSLog(@"\t%d\t%d\t%d\t%d\t%.1f\t%.0f\t%d", 
                  _testOrder, _antiAlias, _useFill, _lineStyle, _flatness, _lineWidth, drawTime);
        }
        
        ++_testOrder;
        int i = 0;
        
        for (int j = 0; j < 4; j++) {
            _antiAlias = (j % 4 < 2);
            _useFill = (j % 2 == 0);
            for (_lineStyle = 0; _lineStyle <= 1; _lineStyle++) {
                for (_flatness = 1.f; _flatness <= 5.f; _flatness += 1.f) {
                    for (_lineWidth = 1; _lineWidth <= 50; _lineWidth += 10) {
                        if (++i == _testOrder)
                            return;
                    }
                }
            }
        }
        _testOrder = -1;
    }
}

- (void)showDrawnTime:(int)ms logging:(BOOL)log
{
    UIViewController *detailc = (UIViewController *)self.superview.nextResponder;
    if (![detailc respondsToSelector:@selector(saveDetailPage:)]) {
        detailc = (UIViewController *)self.superview.superview.nextResponder;
    }
    NSString *title = detailc.title;
    NSRange range = [title rangeOfString:@" ms - "];
    if (range.length > 0) {
        title = [title substringFromIndex:range.location + 6];
    }
    if (_testOrder > 0) {
        detailc.title = [[NSString stringWithFormat:@"%d:%d ms - ", _testOrder, ms]
                         stringByAppendingString:title];
    }
    else if (_drawTimes > 0) {
        detailc.title = [[NSString stringWithFormat:@"%d:%d ms - ", _drawTimes, ms]
                         stringByAppendingString:title];
    }
    else {
        detailc.title = [[NSString stringWithFormat:@"%d ms - ", ms]
                         stringByAppendingString:title];
    }
    if (log) {
        NSLog(@"drawRect: %@", detailc.title);
    }
}

- (UIImage *)snapshot
{
    uint64_t start = mach_absolute_time();
    CGSize size = self.bounds.size;
    
    if (UIGraphicsBeginImageContextWithOptions) {
        UIGraphicsBeginImageContextWithOptions(size, self.opaque, 0);
    } else {
        UIGraphicsBeginImageContext(size);
    }
    
    int drawTime = machToMs(start);
    NSLog(@"UIGraphicsBeginImageContext: %d ms", drawTime);
    
#if 1   // 不加速
    CGContextRef ctx = UIGraphicsGetCurrentContext();
    GiQuartzCanvas canvas;
    
    if (canvas.beginPaint(ctx)) {
        [self draw:&canvas];
        canvas.endPaint();
    }
    
    int drawTime2 = machToMs(start);
    NSLog(@"draw: %d ms", drawTime2 - drawTime);
    
#else   // 由于内部使用了Cache，渲染快好几倍
    [self.layer renderInContext:UIGraphicsGetCurrentContext()];
#endif
    
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return image;
}

- (UIImage *)shotCG
{
    uint64_t start = mach_absolute_time();
    float scale = [UIScreen mainScreen].scale;
    CGSize size = self.bounds.size;
    
    size.width *= scale;
    size.height *= scale;
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef ctx = CGBitmapContextCreate(NULL, size.width, size.height, 8, size.width * 4,
                                             colorSpace, kCGImageAlphaPremultipliedLast);
    CGColorSpaceRelease(colorSpace);
    
    int drawTime = machToMs(start);
    NSLog(@"CGBitmapContextCreate: %d ms", drawTime);
    
    CGContextClearRect(ctx, CGRectMake(0, 0, size.width, size.height));
    
    int drawTime2 = machToMs(start);
    NSLog(@"CGContextClearRect: %d ms", drawTime2 - drawTime);
    
    // 坐标系改为Y朝下，原点在左上角，设置放大倍数，与view坐标系一致
    CGContextTranslateCTM(ctx, 0, size.height);
    CGContextScaleCTM(ctx, scale, - scale);
    
#if 1
    GiQuartzCanvas canvas;
    
    if (canvas.beginPaint(ctx)) {
        [self draw:&canvas];
        canvas.endPaint();
    }
#else   // 略慢约十微秒
    [self.layer drawInContext:ctx];
#endif
    
    int drawTime3 = machToMs(start);
    NSLog(@"draw: %d ms", drawTime3 - drawTime2);
    
#if 1
    CGImageRef cgimage = CGBitmapContextCreateImage(ctx);
    UIImage *image = [[[UIImage alloc]initWithCGImage:cgimage scale:scale
                                         orientation:UIImageOrientationUp] autorelease];
    CGImageRelease(cgimage);
    CGContextRelease(ctx);
#else   // 下面取图像会失败
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    CGContextRelease(ctx);
    
    int drawTime4 = machToMs(start);
    NSLog(@"GetImage: %d ms", drawTime4 - drawTime3);
#endif
    
    return image;
}

- (void)saveImage:(UIImage *)image start:(uint64_t)start
{
    int drawTime = machToMs(start);
    NSLog(@"saveAsPng: %d ms", drawTime);
    
    if (!image) {
        return;
    }
    
    NSString *path = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, 
                                                          NSUserDomainMask, YES) objectAtIndex:0];
    static int order = 0;
    NSString *fmt = image.scale > 1 ? @"%@/page%d@2x.png" : @"%@/page%d.png";
    NSString *filename = [NSString stringWithFormat:fmt, path, order++ % 10];
    
    NSData* imageData = UIImagePNGRepresentation(image);
    if (imageData) {
        [imageData writeToFile:filename atomically:NO];                 
    }
    
    int savedTime = machToMs(start) - drawTime;
    NSString *msg = [NSString stringWithFormat:@"%d ms, %d ms, %d x %d\n%@", 
                     drawTime, savedTime, 
                     (int)image.size.width, (int)image.size.height,
                     [filename substringFromIndex:[filename length] - 22]];
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Save" message:msg
                                                   delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
    [alert show];
    [alert release];
}

- (void)save
{
    uint64_t start = mach_absolute_time();
#if 0
    [self saveImage:[self snapshot] start:start];   // 生成图像最快
#else
    [self saveImage:[self shotCG] start:start];
#endif
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesBegan:touches withEvent:event];
    
    UITouch *touch = [touches anyObject];
    _lastpt = [touch locationInView:touch.view];
    [_dynview setNeedsDisplay];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesMoved:touches withEvent:event];
    
    UITouch *touch = [touches anyObject];
    _lastpt = [touch locationInView:touch.view];
    [_dynview setNeedsDisplay];
}

- (void)edit
{
    CGRect rect = CGRectMake(200, 200, 200, 200);
    CalloutView *callout = [[CalloutView alloc]initWithFrame:rect];
    
    [self addSubview:callout];
    [callout release];
}

@end

@implementation GraphView2

- (void)run
{
    while (![_thread isCancelled]) {
        [_dynview performSelectorOnMainThread:@selector(setNeedsDisplay)
                                   withObject:nil waitUntilDone:YES];
    }
}

- (void)removeFromSuperview
{
    if (_thread) {
        [_thread cancel];
        if (![_thread isFinished])
            [NSThread sleepForTimeInterval:0.1];
        [_thread release];
        _thread = nil;
    }
    [super removeFromSuperview];
}

- (void)didMoveToSuperview
{
    [super didMoveToSuperview];
    if (!_thread) {
        _thread = [[NSThread alloc] initWithTarget:self selector:@selector(run) object:nil];
        [_thread start];
        _drawTimes++;
    }
}

@end
