// ViewFactory.mm
// Copyright (c) 2012-2013, https://github.com/rhcad/touchvg

#import "GiGraphView1.h"
#import "LargeView1.h"
#import "GiViewHelper.h"
#include "gicoreview.h"

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

static bool fireGesture(GiGraphView *v, int type, int state, float x, float y)
{
    return [v coreView]->onGesture([v viewAdapter], (GiGestureType)type, (GiGestureState)state, x, y);
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
            GiGraphView2 *v2 = [[GiGraphView2 alloc]initWithFrame:wrapview.bounds];
            GiViewHelper *hlp = [GiViewHelper instance:v2];
            v = v2;
            
            if (type & 32) {
                [hlp addShapesForTest];
            }
            type = type & ~32;
            
            if (type == 1) {
                hlp.command = @"splines";
            }
            else if (type == 2) {
                hlp.command = @"select";
            }
            else if (type == 3) {
                [hlp loadFromFile:[GiGraphView2 lastFileName]];
                hlp.command = @"select";
            }
            else if (type == 4) {
                hlp.command = @"splines";
                fireGesture(v2, 1, 0, 786, 434);
                fireGesture(v2, 1, 1, 786, 434);
                fireGesture(v2, 1, 2, 828, 444);
                fireGesture(v2, 1, 2, 828, 444);
                fireGesture(v2, 1, 3, 828, 444);
                fireGesture(v2, 1, 0, 819, 408);
                fireGesture(v2, 1, 1, 819, 408);
                fireGesture(v2, 1, 3, 806, 444);
                hlp.command = @"select";
                [hlp zoomToExtent];
            }
            else if (type == 5) {
                hlp.command = @"line";
            }
            else if (type == 6) {
                hlp.command = @"lines";
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
        UIView *v = [GiGraphView createMagnifierView:magframe refView:nil parentView:wrapview];
        v.backgroundColor = [UIColor greenColor];
    }
}

static void gatherTestView(NSMutableArray *arr, NSUInteger index, CGRect frame)
{
    NSUInteger i = 0;
    
    addGraphView(arr, i, index, @"GiGraphView1", frame, 0);
    addLargeView1(arr, i, index, @"GiGraphView1 in large view", frame, 0);
    addGraphView(arr, i, index, @"GiGraphView splines", frame, 1);
    addGraphView(arr, i, index, @"GiGraphView draw", frame, 1|32);
    addGraphView(arr, i, index, @"GiGraphView line", frame, 5);
    addGraphView(arr, i, index, @"GiGraphView lines", frame, 6);
    addGraphView(arr, i, index, @"GiGraphView select randShapes", frame, 2|32);
    addGraphView(arr, i, index, @"GiGraphView select loadShapes", frame, 3);
    addGraphView(arr, i, index, @"GiGraphView fireGesture", frame, 4);
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
