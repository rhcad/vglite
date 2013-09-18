//! \file ViewHelper.java
//! \brief Android绘图视图辅助类
// Copyright (c) 2012-2013, https://github.com/rhcad/touchvg

package touchvg.view;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import touchvg.jni.Chars;
import touchvg.jni.DemoTrade;
import touchvg.jni.Floats;
import touchvg.jni.GiContextBits;
import android.content.Context;
import android.graphics.Bitmap;

//! Android绘图视图辅助类
/*! \ingroup GROUP_ANDROID
 */
public class ViewHelper {
    private GraphView mView;                //!< 要操作的视图
    
    //! 指定视图的构造函数
    public ViewHelper(GraphView view) {
        mView = view;
    }
    
    //! 创建普通绘图视图，并记下此视图
    public GraphView createGraphView(Context context) {
        mView = new GraphView(context);
        return mView;
    }
    
    /**
     * @brief 创建放大镜视图，并记下此视图
     * @param context 视图上下文对象
     * @param mainView 参照的主视图对象，如果为null则取本对象的视图或当前激活视图
     * @return 创建的新视图对象
     */
    public GraphView createMagnifierView(Context context, GraphView mainView) {
        mView = new GraphView(context, mainView != null ? mainView : mView);
        return mView;
    }
    
    //! 返回当前激活视图
    public static GraphView activeView() {
        return GraphView.activeView();
    }
    
    //! 得到当前命令名称
    public String getCommand() {
        return mView.coreView().getCommand();
    }
    
    //! 启动指定名称的命令
    public boolean setCommand(String name) {
        return mView.coreView().setCommand(mView.viewAdapter(), name);
    }
    
    //! 返回线宽，正数表示单位为0.01毫米，零表示1像素宽，负数表示单位为像素
    public int getLineWidth() {
        return Math.round(mView.coreView().getContext(false).getLineWidth());
    }
    
    //! 设置线宽，正数表示单位为0.01毫米，零表示1像素宽，负数表示单位为像素
    public void setLineWidth(int w) {
        mView.coreView().getContext(true).setLineWidth(w, true);
        mView.coreView().setContext(GiContextBits.kContextLineWidth.swigValue());
    }
    
    //! 返回像素单位的线宽，总是为正数
    public int getStrokeWidth() {
        float w = mView.coreView().getContext(false).getLineWidth();
        return Math.round(mView.coreView().calcPenWidth(w));
    }
    
    //! 设置像素单位的线宽，总是为正数
    public void setStrokeWidth(int w) {
        mView.coreView().getContext(true).setLineWidth(-Math.abs(w), true);
        mView.coreView().setContext(GiContextBits.kContextLineWidth.swigValue());
    }
    
    //! 返回线型, 0-5:实线,虚线,点线,点划线,双点划线,空线
    public int getLineStyle() {
        float w = mView.coreView().getContext(false).getLineStyle();
        return Math.round(mView.coreView().calcPenWidth(w));
    }
    
    //! 设置线型, 0-5:实线,虚线,点线,点划线,双点划线,空线
    public void setLineStyle(int style) {
        mView.coreView().getContext(true).setLineStyle(style);
        mView.coreView().setContext(GiContextBits.kContextLineStyle.swigValue());
    }
    
    //! 返回线条颜色，忽略透明度分量，0 表示不画线条
    public int getLineColor() {
        return mView.coreView().getContext(false).getLineColor().getARGB();
    }
    
    //! 设置线条颜色，忽略透明度分量，0 表示不画线条
    public void setLineColor(int argb) {
        mView.coreView().getContext(true).setLineARGB(argb);
        mView.coreView().setContext(argb == 0 ? GiContextBits.kContextLineARGB.swigValue()
                : GiContextBits.kContextLineRGB.swigValue());
    }
    
    //! 返回线条透明度, 0-255
    public int getLineAlpha() {
        return mView.coreView().getContext(false).getLineColor().getA();
    }
    
    //! 设置线条透明度, 0-255
    public void setLineAlpha(int alpha) {
        mView.coreView().getContext(true).setLineAlpha(alpha);
        mView.coreView().setContext(GiContextBits.kContextLineAlpha.swigValue());
    }

    //! 返回填充颜色，忽略透明度分量，0 表示不填充
    public int getFillColor() {
        return mView.coreView().getContext(false).getFillColor().getARGB();
    }
    
    //! 设置填充颜色，忽略透明度分量，0 表示不填充
    public void setFillColor(int argb) {
        mView.coreView().getContext(true).setFillARGB(argb);
        mView.coreView().setContext(argb == 0 ? GiContextBits.kContextFillARGB.swigValue()
                : GiContextBits.kContextFillRGB.swigValue());
    }
    
    //! 返回填充透明度, 0-255
    public int getFillAlpha() {
        return mView.coreView().getContext(false).getFillColor().getA();
    }
    
    //! 设置填充透明度, 0-255
    public void setFillAlpha(int alpha) {
        mView.coreView().getContext(true).setFillAlpha(alpha);
        mView.coreView().setContext(GiContextBits.kContextFillAlpha.swigValue());
    }
    
    //! 绘图属性是否正在动态修改. 拖动时先设为true，然后改变绘图属性，完成后设为false.
    public void setContextEditing(boolean editing) {
        mView.coreView().setContextEditing(editing);
    }
    
    //! 添加测试图形
    public int addShapesForTest() {
        return mView.coreView().addShapesForTest();
    }
    
    //! 放缩显示全部内容
    public boolean zoomToExtent() {
        return mView.coreView().zoomToExtent();
    }
    
    //! 放缩显示指定范围到视图区域
    public boolean zoomToModel(float x, float y, float w, float h) {
        return mView.coreView().zoomToModel(x, y, w, h);
    }
    
    //! 得到静态图形的快照
    public Bitmap snapshot() {
        return mView.snapshot();
    }
    
    //! 保存静态图形的快照到PNG文件
    public boolean savePng(String filename) {
        boolean ret = false;
        synchronized(mView.snapshot()) {
            try {
                final FileOutputStream os = new FileOutputStream(filename);
                ret = mView.snapshot().compress(Bitmap.CompressFormat.PNG, 100, os);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return ret;
    }
    
    //! 返回图形总数
    public int getShapeCount() {
        return mView.coreView().getShapeCount();
    }
    
    //! 返回选中的图形个数
    public int getSelectedCount() {
        return mView.coreView().getSelectedShapeCount();
    }
    
    //! 返回选中的图形的类型, MgShapeType
    public int getSelectedType() {
        return mView.coreView().getSelectedShapeType();
    }
    
    //! 得到图形的JSON内容
    public String getContent() {
        final String str = mView.coreView().getContent();
        mView.coreView().freeContent();
        return str;
    }
    
    //! 从JSON内容中加载图形
    public boolean setContent(String content) {
        return mView.coreView().setContent(content);
    }
    
    //! 从JSON文件中加载图形
    public boolean loadFromFile(String vgfile) {
        return mView.coreView().loadFromFile(vgfile);
    }
    
    //! 保存图形到JSON文件
    public boolean saveToFile(String vgfile) {
        boolean ret;
        if (getShapeCount() == 0) {
            ret = new File(vgfile).delete();
        } else {
            ret = createFile(vgfile)
                    && mView.coreView().saveToFile(vgfile);
        }
        return ret;
    }
    
    //! 创建指定的文件和上一级文件夹
    public static boolean createFile(String filename) {
        final File file = new File(filename);
        final File pf = file.getParentFile();
        if (!pf.exists()) {
            pf.mkdirs();
        }
        if (!file.exists()) {
            try {
                file.createNewFile();
            } catch (IOException e) {
                e.printStackTrace();
                return false;
            }
        }
        return true;
    }
    
    //! 得到当前图形的各种度量尺寸
    public String getDimensions(float[] vars) {
        Floats v = new Floats(vars.length);
        Chars types = new Chars(vars.length);
        int n = DemoTrade.getDimensions(mView.coreView(), v, types);
        
        if (n > 0) {
            StringBuffer buf = new StringBuffer(n);
            for (int i = 0; i < n; i++) {
                buf.setCharAt(i, types.get(i));
                vars[i] = v.get(i);
            }
            return buf.toString();
        }
        return "";
    }
}
