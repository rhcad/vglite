// BasicAnimationView.m
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#import "BasicAnimationView.h"
#import <QuartzCore/QuartzCore.h>

@implementation BasicAnimationView

- (void)dealloc
{
    [shapeLayer release];
    [super dealloc];
}

- (id)initWithFrame:(CGRect)frame withTests:(int)t
{
    self = [super initWithFrame:frame];
    if (self) {
    }
    return self;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesBegan:touches withEvent:event];
    
    UITouch *touch = [touches anyObject];
    CGPoint pt = [touch locationInView:touch.view];
    
    float radius = 200.f;
    float newRadius = 400.f;
    
    if (!shapeLayer) {
        shapeLayer = [[CAShapeLayer alloc]init];
        
        shapeLayer.fillColor = nil;	
        shapeLayer.strokeColor = [UIColor blackColor].CGColor;	
        shapeLayer.lineWidth = 7;	
        shapeLayer.bounds = CGRectMake(pt.x - 2 * radius, pt.y - 2 * radius, 
                                       2 * radius, 2 * radius);	
        shapeLayer.path = [UIBezierPath bezierPathWithRect:shapeLayer.bounds].CGPath;
        
        [self.layer addSublayer:shapeLayer];
    }
    else {
        CGRect newBounds = CGRectMake(pt.x - 2 * newRadius, pt.y - 2 * newRadius, 
                                      2 * newRadius, 2 * newRadius);
        UIBezierPath *newPath;
        CGColorRef fillColor = nil;
        
        if ((++step) % 3 == 1) {
            newPath = [UIBezierPath bezierPathWithOvalInRect:newBounds];
            fillColor = [UIColor clearColor].CGColor;
        }
        else if (step % 3 == 2) {
            newBounds = CGRectMake(pt.x - 2 * radius, pt.y - 2 * radius, 
                                   2 * radius, 2 * radius);
            newPath = [UIBezierPath bezierPathWithOvalInRect:newBounds];
            fillColor = [UIColor blueColor].CGColor;
        }
        else {
            newPath = [UIBezierPath bezierPathWithRect:newBounds];
            fillColor = [UIColor greenColor].CGColor;
        }
        
        CABasicAnimation* pathAnim = [CABasicAnimation animationWithKeyPath: @"path"];	
        pathAnim.toValue = (id)newPath.CGPath;
        
        CABasicAnimation* boundsAnim = [CABasicAnimation animationWithKeyPath: @"bounds"];	
        boundsAnim.toValue = [NSValue valueWithCGRect:newBounds];
        
        CABasicAnimation* fillAnim = [CABasicAnimation animationWithKeyPath: @"fillColor"];	
        fillAnim.toValue = (id)fillColor;
        
        CAAnimationGroup *anims = [CAAnimationGroup animation];	
        anims.animations = [NSArray arrayWithObjects:pathAnim, boundsAnim, fillAnim, nil];	
        anims.removedOnCompletion = NO;	
        anims.duration = 1.0f;
        anims.fillMode  = kCAFillModeForwards;
        
        [shapeLayer addAnimation:anims forKey:nil];
    }
}

@end
