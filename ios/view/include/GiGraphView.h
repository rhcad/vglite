//! \file GiGraphView.h
//! \brief 定义iOS绘图视图类 GiGraphView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#import <UIKit/UIKit.h>

class GiViewAdapter;
class GiCoreView;

//! iOS绘图视图类
/*! \ingroup GROUP_IOS
 */
@interface GiGraphView : UIView {
    GiViewAdapter   *_viewAdapter;
}

- (GiCoreView *)coreView;
- (UIImage *)snapshot;
- (BOOL)savePng:(NSString *)filename;

@end
