//! \file GiViewHelper.h
//! \brief 定义iOS绘图视图辅助类 GiViewHelper
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#import <UIKit/UIKit.h>

@class GiGraphView;

//! iOS绘图视图辅助类
/*! \ingroup GROUP_IOS
 */
@interface GiViewHelper : NSObject

+ (GiGraphView *)createGraphView:(CGRect)frame :(UIView *)parentView;   //!< 创建普通图形视图
+ (GiGraphView *)createMagnifierView:(CGRect)frame
                             refView:(GiGraphView *)refView
                          parentView:(UIView *)parentView;  //!< 创建放大镜视图

+ (NSString *)command:(GiGraphView *)v;                     //!< 得到当前命令名称
+ (BOOL)setCommand:(GiGraphView *)v :(NSString *)name;      //!< 启动指定名称的命令

+ (UIImage *)snapshot:(GiGraphView *)v;                     //!< 得到静态图形的快照，自动释放
+ (BOOL)savePng:(GiGraphView *)v :(NSString *)filename;     //!< 保存静态图形的快照到PNG文件

+ (int)addShapesForTest:(GiGraphView *)v;                   //!< 添加测试图形
+ (BOOL)fireGesture:(GiGraphView *)v type:(int)type state:(int)state x:(float)x y:(float)y;
+ (BOOL)zoomToExtent:(GiGraphView *)v;                      //!< 放缩显示全部内容

+ (int)getShapeCount:(GiGraphView *)v;                      //!< 返回图形总数
+ (NSString *)content:(GiGraphView *)v;                     //!< 保存图形到JSON内容
+ (BOOL)setContent:(GiGraphView *)v :(NSString *)content;   //!< 从JSON内容中加载图形
+ (BOOL)loadFromFile:(GiGraphView *)v :(NSString *)vgfile;  //!< 从JSON文件中加载图形
+ (BOOL)saveToFile:(GiGraphView *)v :(NSString *)vgfile;    //!< 保存图形到JSON文件

@end
