// LargeView1.mm
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#import "LargeView1.h"
#import "GiGraphView1.h"
#import "GiGraphView.h"

@implementation LargeView1

- (id)initWithFrame:(CGRect)frame withType:(int)type
{
    self = [super initWithFrame:frame];
    if (self) {
        float w = (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) ? 1024 : 2048;
        
        UIView *view = [[UIView alloc]initWithFrame:CGRectMake(0, 0, w, w)];
        [self addSubview:view];
        [view release];
        
        UIView *subview;
        
        if (type == 0) {
            _subview1 = [[GiGraphView1 alloc]initWithFrame:view.bounds];
            subview = _subview1;
        }
        else {
            _subview2 = [[GiGraphView alloc]initWithFrame:view.bounds];
            subview = _subview2;
#ifdef __IPHONE_5_0
            if (_subview2.panGestureRecognizer) {
                [self.panGestureRecognizer requireGestureRecognizerToFail:_subview2.panGestureRecognizer];
            }
#endif
        }
        [view addSubview:subview];
        [subview release];
        
        self.delegate = self;
        self.contentSize = subview.frame.size;
        self.minimumZoomScale = 0.25f;
        self.maximumZoomScale = 5.f;
    }
    
    return self;
}

- (UIView *)viewForZoomingInScrollView:(UIScrollView *)scrollView
{
    return _subview1 ? _subview1.superview : _subview2.superview;
}

- (void)save
{
    [_subview1 save];
}

- (void)edit
{
    [_subview1 edit];
}

@end
