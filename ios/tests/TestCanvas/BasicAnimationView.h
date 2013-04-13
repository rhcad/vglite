// BasicAnimationView.h
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#import <UIKit/UIKit.h>

@class CAShapeLayer;

@interface BasicAnimationView : UIView {
    CAShapeLayer    *shapeLayer;
    int             step;
}

@end
