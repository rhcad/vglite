//! \file GiGraphView.h
//! \brief 定义iOS绘图视图类 GiGraphView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#import <UIKit/UIKit.h>

class GiQuartzCanvas;

//! iOS绘图视图类
/*! \ingroup GROUP_IOS
 */
@interface GiGraphView : UIView {
    GiQuartzCanvas  *_canvas;
    UIView          *_dynview;
}

- (BOOL)savePng:(NSString *)filename;

@end
