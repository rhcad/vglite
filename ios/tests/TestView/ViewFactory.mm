// ViewFactory.mm
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#import "GiGraphView1.h"
#import "GiGraphView.h"
#import "LargeView1.h"

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
                         NSString* title, CGRect frame)
{
    LargeView1 *view = nil;
    
    if (!arr && index == i++) {
        view = [[LargeView1 alloc]initWithFrame:frame];
    }
    addView(arr, title, view);
}

static void addGraphView(NSMutableArray *arr, NSUInteger &i, NSUInteger index,
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
            v = [[GiGraphView alloc]initWithFrame:wrapview.bounds];
        }
        [wrapview addSubview:v];
        [v release];
    }
}

static void gatherTestView(NSMutableArray *arr, NSUInteger index, CGRect frame)
{
    NSUInteger i = 0;
    
    addGraphView(arr, i, index, @"GiGraphView1", frame, 0);
    addLargeView1(arr, i, index, @"GiGraphView1 in large view", frame);
    addGraphView(arr, i, index, @"GiGraphView", frame, 1);
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
