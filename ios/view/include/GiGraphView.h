//! \file GiGraphView.h
//! \brief 定义iOS绘图视图类 GiGraphView
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#import <UIKit/UIKit.h>

class GiViewAdapter;
class GiCoreView;

//! iOS绘图视图基类
/*! \ingroup GROUP_IOS
 */
@interface GiBaseView : UIView<UIGestureRecognizerDelegate> {
    GiViewAdapter   *_viewAdapter;
}

@property(nonatomic, readonly) UIPanGestureRecognizer *panRecognizer;           //!< 拖动
@property(nonatomic, readonly) UITapGestureRecognizer *tapRecognizer;           //!< 点击
@property(nonatomic, readonly) UITapGestureRecognizer *twoTapsRecognizer;       //!< 双击
@property(nonatomic, readonly) UILongPressGestureRecognizer *pressRecognizer;   //!< 长按

@property(nonatomic, readonly) UIPinchGestureRecognizer *pinchRecognizer;       //!< 放缩
@property(nonatomic, readonly) UIRotationGestureRecognizer *rotationRecognizer; //!< 旋转
@property(nonatomic, readonly) UIPanGestureRecognizer *twoFingersPanRecognizer; //!< 双指拖动

- (GiCoreView *)coreView;               //!< 得到跨平台内核视图
- (UIImage *)snapshot;                  //!< 得到静态图形的快照，自动释放
- (BOOL)savePng:(NSString *)filename;   //!< 保存静态图形的快照到PNG文件

@end

//! iOS绘图视图类
/*! \ingroup GROUP_IOS
 */
@interface GiGraphView : GiBaseView

@end

//! iOS放大镜视图类
/*! \ingroup GROUP_IOS
    参照最近创建的 GiGraphView 进行同步显示，必须先创建 GiGraphView 视图。
 */
@interface GiMagnifierView : GiBaseView

- (id)initWithFrame:(CGRect)frame :(GiGraphView *)mainView;

@end
