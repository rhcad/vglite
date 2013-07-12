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
    GiViewAdapter   *_adapter;
}

@property(nonatomic, readonly) UIPanGestureRecognizer *panRecognizer;           //!< 拖动
@property(nonatomic, readonly) UITapGestureRecognizer *tapRecognizer;           //!< 点击
@property(nonatomic, readonly) UITapGestureRecognizer *twoTapsRecognizer;       //!< 双击
@property(nonatomic, readonly) UILongPressGestureRecognizer *pressRecognizer;   //!< 长按
@property(nonatomic, readonly) UIPinchGestureRecognizer *pinchRecognizer;       //!< 放缩
@property(nonatomic, readonly) UIRotationGestureRecognizer *rotationRecognizer; //!< 旋转

//! 初始化放大镜视图. refView为nil或调用 initWithFrame:(CGRect)frame 函数则创建普通绘图视图
- (id)initWithFrame:(CGRect)frame refView:(GiGraphView *)refView;

- (GiViewAdapter *)viewAdapter;         //!< 得到视图适配器对象
- (GiCoreView *)coreView;               //!< 得到跨平台内核视图

- (UIImage *)snapshot;                  //!< 得到静态图形的快照，自动释放
- (BOOL)savePng:(NSString *)filename;   //!< 保存静态图形的快照到PNG文件

- (void)clearCachedData;                 //!< 释放临时数据内存

@end
