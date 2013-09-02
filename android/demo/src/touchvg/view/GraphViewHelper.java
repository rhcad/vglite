//! \file GraphViewHelper.java
//! \brief Android绘图视图辅助类
// Copyright (c) 2012-2013, https://github.com/rhcad/touchvg

package touchvg.view;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import android.content.Context;
import android.graphics.Bitmap;

//! Android绘图视图辅助类
/*! \ingroup GROUP_ANDROID
 */
public class GraphViewHelper {
    private GraphView mView;
    
    //! 默认构造函数，还未设置视图(setView)
    public GraphViewHelper() {
    }
    
    //! 指定视图的构造函数
    public GraphViewHelper(GraphView view) {
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
    
    //! 设置当前图形视图，供下列函数使用
    public void setView(GraphView view) {
        mView = view;
    }
    
    //! 得到当前命令名称
    public String command() {
        return mView.coreView().command();
    }
    
    //! 启动指定名称的命令
    public boolean setCommand(String name) {
        return mView.coreView().setCommand(mView.viewAdapter(), name);
    }
    
    //! 添加测试图形
    public int addShapesForTest() {
        return mView.coreView().addShapesForTest();
    }
    
    //! 放缩显示全部内容
    public boolean zoomToExtent() {
        return mView.coreView().zoomToExtent();
    }
    
    //! 得到静态图形的快照
    public Bitmap snapshot() {
        return mView.snapshot();
    }
    
    //! 得到静态图形的快照PNG字节流
    public byte[] getPngBytes() {
        synchronized(mView.snapshot()) {
            final ByteArrayOutputStream os = new ByteArrayOutputStream();
            mView.snapshot().compress(Bitmap.CompressFormat.PNG, 100, os);
            return os.toByteArray();
        }
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
}
