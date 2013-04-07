// GraphView1.h
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#import <UIKit/UIKit.h>

@interface GraphView1 : UIView {
    CGPoint         _lastpt;
    int             _tests;
}

@property(nonatomic, readonly) UIPanGestureRecognizer *panGestureRecognizer;

- (id)initWithFrame:(CGRect)frame withTests:(int)t;

@end
