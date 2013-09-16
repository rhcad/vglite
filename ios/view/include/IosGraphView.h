//! \file IosGraphView.h
//! \brief 定义iOS绘图视图类 IosGraphView
// Copyright (c) 2012-2013, https://github.com/rhcad/touchvg

#import <UIKit/UIKit.h>

#ifdef __cplusplus
class GiCoreView;
class GiView;
#endif

//! iOS绘图视图类
/*! \ingroup GROUP_IOS
 */
@interface IosGraphView : UIView

@property(nonatomic, readonly) UIPanGestureRecognizer *panRecognizer;           //!< 拖动手势识别器
@property(nonatomic, readonly) UITapGestureRecognizer *tapRecognizer;           //!< 单指点击手势识别器
@property(nonatomic, readonly) UITapGestureRecognizer *twoTapsRecognizer;       //!< 单指双击手势识别器
@property(nonatomic, readonly) UILongPressGestureRecognizer *pressRecognizer;   //!< 单指长按手势识别器
@property(nonatomic, readonly) UIPinchGestureRecognizer *pinchRecognizer;       //!< 双指放缩手势识别器
@property(nonatomic, readonly) UIRotationGestureRecognizer *rotationRecognizer; //!< 双指旋转手势识别器
@property(nonatomic)           BOOL gestureEnabled;     //!< 是否允许触摸交互

//! 创建普通图形视图并添加到父视图，不需要额外释放
+ (IosGraphView *)createGraphView:(CGRect)frame :(UIView *)parentView;

//! 创建放大镜视图并添加到父视图，不需要额外释放
+ (IosGraphView *)createMagnifierView:(CGRect)frame
                              refView:(IosGraphView *)refView
                           parentView:(UIView *)parentView;

+ (IosGraphView *)activeView;                //!< 得到当前激活的绘图视图

#ifdef __cplusplus
- (GiView *)viewAdapter;                    //!< 得到视图适配器对象
- (GiCoreView *)coreView;                   //!< 得到跨平台内核视图
#endif

- (UIImage *)snapshot;                      //!< 得到静态图形的快照，自动释放
- (BOOL)savePng:(NSString *)filename;       //!< 保存静态图形的快照到PNG文件
- (void)clearCachedData;                    //!< 释放临时数据内存

@end
