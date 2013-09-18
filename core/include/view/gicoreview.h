//! \file gicoreview.h
//! \brief 定义内核视图分发器类 GiCoreView
// Copyright (c) 2012-2013, https://github.com/rhcad/touchvg

#ifndef TOUCHVG_CORE_VIEWDISPATCHER_H
#define TOUCHVG_CORE_VIEWDISPATCHER_H

#include "gigesture.h"
#include "giview.h"
#include "gicontxt.h"

class GiCanvas;
class GiCoreViewImpl;
class MgView;

//! 内核视图分发器类
/*! 本对象拥有图形文档对象，负责显示和手势动作的分发。
    \ingroup CORE_VIEW
 */
class GiCoreView
{
public:
    //! 构造函数，传入NULL构造主视图，传入主视图构造辅助视图
    GiCoreView(GiCoreView* mainView = (GiCoreView*)0);
    
    //! 析构函数
    ~GiCoreView();
    
    //! 创建内核视图
    void createView(GiView* view, int type = 1);
    
    //! 创建放大镜视图
    void createMagnifierView(GiView* newview, GiView* mainView);
    
    //! 销毁内核视图
    void destoryView(GiView* view);
    
    //! 显示所有图形
    int drawAll(GiView* view, GiCanvas* canvas);

    //! 显示新图形，在 GiView.regenAppend() 后调用
    bool drawAppend(GiView* view, GiCanvas* canvas);
    
    //! 显示动态图形
    void dynDraw(GiView* view, GiCanvas* canvas);
    
    //! 设置背景颜色
    int setBkColor(GiView* view, int argb);

    //! 设置屏幕的点密度
    static void setScreenDpi(int dpi);
    
    //! 设置视图的宽高
    void onSize(GiView* view, int w, int h);
    
    //! 传递单指触摸手势消息
    bool onGesture(GiView* view, GiGestureType type,
            GiGestureState state, float x, float y, bool switchGesture = false);

    //! 传递双指移动手势(可放缩旋转)
    bool twoFingersMove(GiView* view, GiGestureState state,
            float x1, float y1, float x2, float y2, bool switchGesture = false);
    
    //! 返回当前命令名称
    const char* getCommand() const;
    
    //! 启动命令
    bool setCommand(GiView* view, const char* name);

    //! 执行上下文动作
    bool doContextAction(int action);
    
    //! 释放临时数据内存
    void clearCachedData();
    
    //! 添加测试图形
    int addShapesForTest();
    
    //! 返回图形总数
    int getShapeCount();

	//! 返回选中的图形个数
    int getSelectedShapeCount();

	//! 返回选中的图形的类型, MgShapeType
    int getSelectedShapeType();

    //! 从JSON文件中加载图形
    bool loadFromFile(const char* vgfile);
    
    //! 保存图形到JSON文件
    bool saveToFile(const char* vgfile, bool pretty = true);

    //! 得到图形的JSON内容，需要再调用 freeContent()
    const char* getContent();
    
    //! 释放 getContent() 产生的缓冲资源
    void freeContent();

    //! 从JSON内容中加载图形
    bool setContent(const char* content);
    
    //! 放缩显示全部内容到视图区域
    bool zoomToExtent();
    
    //! 放缩显示指定范围到视图区域
    bool zoomToModel(float x, float y, float w, float h);
    
    //! 计算画笔的像素宽度
    float calcPenWidth(float lineWidth);

    //! 返回当前绘图属性
    GiContext& getContext(bool forChange);

    //! 绘图属性改变后提交更新
    /** 在 getContext(true) 后调用本函数。
     * \param ctx 绘图属性
     * \param mask 需要应用哪些属性(GiContextBits)，-1表示全部属性，0则不修改，按位组合值见 GiContextBits
     * \param apply 0表示还要继续动态修改属性，1表示结束动态修改并提交，-1表示结束动态修改并放弃改动
     */
    void setContext(const GiContext& ctx, int mask, int apply);
    
    //! 绘图属性改变后提交更新
    void setContext(int mask);
    
    //! 设置线条属性是否正在动态修改. getContext(false)将重置为未动态修改.
    void setContextEditing(bool editing);
    
    //! 添加一个容纳图像的矩形图形
    /*!
        \param name 图像的标识名称，例如可用无路径的文件名
        \param width 图像矩形的宽度，单位为点
        \param height 图像矩形的高度，单位为点
        \return 是否添加成功
     */
    bool addImageShape(const char* name, float width, float height);

#ifndef SWIG
    MgView* viewAdapter();
#endif

private:
    GiCoreViewImpl* impl;
};

#endif // TOUCHVG_CORE_VIEWDISPATCHER_H
