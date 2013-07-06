// LargeView1.h
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#import <UIKit/UIKit.h>

@class GiGraphView1;
@class GiGraphView;

//! 测试内嵌一个绘图视图的长幅滚动视图类
@interface LargeView1 : UIScrollView<UIScrollViewDelegate> {
    GiGraphView1    *_subview1;
    GiGraphView     *_subview2;
}

- (id)initWithFrame:(CGRect)frame withType:(int)type;
- (void)save;
- (void)edit;

@end
