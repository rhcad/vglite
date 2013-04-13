// ViewFactory.mm
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#import "GiGraphView.h"
#import "GraphView1.h"
#import "LargeView1.h"
#import "BasicAnimationView.h"

static UIViewController *_tmpController = nil;

static void addView(NSMutableArray *arr, NSString* title, UIView* view)
{
    if (arr) {
        [arr addObject:title];
    }
    else if (view) {
        _tmpController = [[UIViewController alloc] init];
        _tmpController.title = title;
        _tmpController.view = view;
        [view release];
    }
}

static void addView1(NSMutableArray *arr, NSUInteger &i, NSUInteger index, 
                     NSString* title, int tests, CGRect frame)
{
    GraphView1 *view = nil;
    
    if (!arr && index == i++) {
        view = [[GraphView1 alloc]initWithFrame:frame withTests:tests];
    }
    addView(arr, title, view);
}

static void addView2(NSMutableArray *arr, NSUInteger &i, NSUInteger index, 
                     NSString* title, int tests, CGRect frame)
{
    GraphView2 *view = nil;
    
    if (!arr && index == i++) {
        view = [[GraphView2 alloc]initWithFrame:frame withTests:tests];
    }
    addView(arr, title, view);
}

static void addLargeView1(NSMutableArray *arr, NSUInteger &i, NSUInteger index, 
                         NSString* title, int tests, CGRect frame)
{
    LargeView1 *view = nil;
    
    if (!arr && index == i++) {
        view = [[LargeView1 alloc]initWithFrame:frame withTests:tests];
    }
    addView(arr, title, view);
}

static void addGraphView(NSMutableArray *arr, NSUInteger &i, NSUInteger index,
                         NSString* title, CGRect frame)
{
    UIView *wrapview = nil;
    
    if (!arr && index == i++) {
        wrapview = [[UIView alloc]initWithFrame:frame];
        wrapview.opaque = NO;
    }
    addView(arr, title, wrapview);
    if (wrapview) {
        GiGraphView *v = [[GiGraphView alloc]initWithFrame:wrapview.bounds];
        [wrapview addSubview:v];
        [v release];
    }
}

static void gatherTestView(NSMutableArray *arr, NSUInteger index, CGRect frame)
{
    NSUInteger i = 0;
    
    addGraphView(arr, i, index, @"GiGraphView", frame);
    
    addView1(arr, i, index, @"testRect", 0x01, frame);
    addView1(arr, i, index, @"testLine", 0x02, frame);
    addView1(arr, i, index, @"testTextAt", 0x04, frame);
    addView1(arr, i, index, @"testEllipse", 0x08, frame);
    addView1(arr, i, index, @"testQuadBezier", 0x10, frame);
    addView1(arr, i, index, @"testCubicBezier", 0x20, frame);
    addView1(arr, i, index, @"testPolygon", 0x40, frame);
    addView1(arr, i, index, @"testClearRect", 0x80|0x40|0x02, frame);
    addView1(arr, i, index, @"testClipPath", 0x100, frame);
    addView1(arr, i, index, @"testHandle", 0x200, frame);
    addView1(arr, i, index, @"testDynamicCurves", 0x400, frame);
    addView1(arr, i, index, @"testBeziers with dynview", 0x20|0x10000, frame);
    
    addView2(arr, i, index, @"testDynCurves with thread", 0x400|0x40000, frame);
    addView2(arr, i, index, @"testCubicBezier with thread", 0x20|0x40000, frame);
    addView2(arr, i, index, @"testEllipse with thread", 0x08|0x40000, frame);
    
    addLargeView1(arr, i, index, @"testTextAt in large view", 0x04|0x20000, frame);
    addLargeView1(arr, i, index, @"testCubicBezier in large view", 0x20|0x20000, frame);
    addLargeView1(arr, i, index, @"testHandle in large view", 0x200|0x20000, frame);
    addLargeView1(arr, i, index, @"testDynCurves in large view", 0x400|0x20000, frame);
    
    BasicAnimationView *baview = nil;
    if (!arr && index == i++) {
        baview = [[BasicAnimationView alloc]initWithFrame:frame];
    }
    addView(arr, @"CABasicAnimation", baview);
}

void getTestViewTitles(NSMutableArray *arr)
{
    gatherTestView(arr, 0, CGRectNull);
}

UIViewController *createTestView(NSUInteger index, CGRect frame)
{
    _tmpController = nil;
    gatherTestView(nil, index, frame);
    return _tmpController;
}
