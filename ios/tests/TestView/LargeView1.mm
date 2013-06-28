// LargeView1.mm
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#import "LargeView1.h"
#import "GiGraphView1.h"

@implementation LargeView1

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        float w = (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) ? 1024 : 2048;
        
        UIView *view = [[UIView alloc]initWithFrame:CGRectMake(0, 0, w, w)];
        [self addSubview:view];
        [view release];
        
        _subview = [[GiGraphView1 alloc]initWithFrame:view.bounds];
        [view addSubview:_subview];
        [_subview release];
        
        self.delegate = self;
        self.contentSize = _subview.frame.size;
        self.minimumZoomScale = 0.25f;
        self.maximumZoomScale = 5.f;
    }
    
    return self;
}

- (UIView *)viewForZoomingInScrollView:(UIScrollView *)scrollView
{
    return _subview.superview;
}

- (void)save
{
    [_subview save];
}

- (void)edit
{
    [_subview edit];
}

@end
