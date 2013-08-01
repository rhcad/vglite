// ViewFactory.mm
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#import "GiGraphView1.h"
#import "LargeView1.h"
#import "GiViewHelper.h"

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

static void addLargeView1(NSMutableArray *arr, NSUInteger &i, NSUInteger index, 
                         NSString* title, CGRect frame, int type)
{
    LargeView1 *view = nil;
    
    if (!arr && index == i++) {
        view = [[LargeView1 alloc]initWithFrame:frame withType:type];
    }
    addView(arr, title, view);
}

static UIView* addGraphView(NSMutableArray *arr, NSUInteger &i, NSUInteger index,
                            NSString* title, CGRect frame, int type)
{
    UIView *v, *wrapview = nil;
    
    if (!arr && index == i++) {
        wrapview = [[UIView alloc]initWithFrame:frame];
        wrapview.opaque = NO;
    }
    addView(arr, title, wrapview);
    if (wrapview) {
        if (type == 0) {
            v = [[GiGraphView1 alloc]initWithFrame:wrapview.bounds];
        }
        else {
            v = [[GiGraphView2 alloc]initWithFrame:wrapview.bounds];
            [GiViewHelper setView:v];
            
            if (type == 1) {
                [GiViewHelper setCommand:@"splines"];
            }
            else if (type == 2) {
                [GiViewHelper addShapesForTest];
                [GiViewHelper setCommand:@"select"];
            }
            else if (type == 3) {
                [GiViewHelper setCommand:@"splines"];
                [GiViewHelper fireGesture:1 state:0 x:786 y:434];
                [GiViewHelper fireGesture:1 state:1 x:786 y:434];
                [GiViewHelper fireGesture:1 state:2 x:828 y:444];
                [GiViewHelper fireGesture:1 state:2 x:828 y:444];
                [GiViewHelper fireGesture:1 state:3 x:828 y:444];
                [GiViewHelper fireGesture:1 state:0 x:819 y:408];
                [GiViewHelper fireGesture:1 state:1 x:819 y:408];
                [GiViewHelper fireGesture:1 state:3 x:806 y:444];
                [GiViewHelper setCommand:@"select"];
                [GiViewHelper zoomToExtent];
            }
        }
        [wrapview addSubview:v];
        [v release];
    }
    
    return wrapview;
}

static void testMagnifierView(NSMutableArray *arr, NSUInteger &i, NSUInteger index,
                              NSString* title, CGRect frame, int type)
{
    UIView *wrapview = addGraphView(arr, i, index, title, frame, type);
    
    if (wrapview) {
        CGRect magframe = CGRectMake(10, 10, 200, 200);
        UIView *v = [GiViewHelper createMagnifierView:magframe refView:nil];
        [wrapview addSubview:v];
        [v release];
        v.backgroundColor = [UIColor greenColor];
    }
}

static void gatherTestView(NSMutableArray *arr, NSUInteger index, CGRect frame)
{
    NSUInteger i = 0;
    
    addGraphView(arr, i, index, @"GiGraphView1", frame, 0);
    addLargeView1(arr, i, index, @"GiGraphView1 in large view", frame, 0);
    addGraphView(arr, i, index, @"GiGraphView draw", frame, 1);
    addGraphView(arr, i, index, @"GiGraphView select", frame, 2);
    addGraphView(arr, i, index, @"GiGraphView fireGesture", frame, 3);
    addLargeView1(arr, i, index, @"GiGraphView in large view", frame, 1);
    testMagnifierView(arr, i, index, @"MagnifierView", frame, 1);
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
