//! \file GiViewHelper.h
//! \brief 定义iOS绘图视图辅助类 GiViewHelper
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#import <UIKit/UIKit.h>

@class GiGraphView;

//! iOS绘图视图辅助类
/*! \ingroup GROUP_IOS
 */
@interface GiViewHelper

+ (UIView *)createGraphView:(CGRect)frame;
+ (UIView *)createMagnifierView:(CGRect)frame refView:(UIView *)refView;

+ (void)setView:(UIView *)view;

+ (NSString *)command;
+ (BOOL)setCommand:(NSString *)name;

+ (UIImage *)snapshot;                      //!< 得到静态图形的快照，自动释放
+ (BOOL)savePng:(NSString *)filename;       //!< 保存静态图形的快照到PNG文件

+ (void)addShapesForTest;                   //!< 添加测试图形
+ (void)fireGesture:(int)type state:(int)state x:(float)x y:(float)y;
+ (void)zoomToExtent;                       //!< 放缩显示全部内容

+ (NSString *)content;                      //!< 保存图形到JSON内容
+ (BOOL)setContent:(NSString *)content;     //!< 从JSON内容中加载图形
+ (BOOL)loadFromFile:(NSString *)vgfile;    //!< 从JSON文件中加载图形
+ (BOOL)saveToFile:(NSString *)vgfile;      //!< 保存图形到JSON文件

@end
