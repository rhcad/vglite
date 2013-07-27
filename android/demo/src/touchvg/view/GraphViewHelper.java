//! \file GraphViewHelper.java
//! \brief Android绘图视图辅助类
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

package touchvg.view;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import org.apache.http.util.EncodingUtils;
import touchvg.jni.MgJsonStorage;
import android.content.Context;

//! Android绘图视图辅助类
/*! \ingroup GROUP_ANDROID
 */
public class GraphViewHelper {
    private GraphView mView;
    
    //! 默认构造函数，还未设置视图
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
    
    //! 创建放大镜视图，并记下此视图
    public GraphView createMagnifierView(Context context, GraphView mainView) {
        mView = new GraphView(context, mainView);
        return mView;
    }
    
    //! 设置当前图形视图，供下列函数使用
    public void setView(GraphView view) {
        mView = view;
    }
    
    //! 得到当前命令名称
    public String command() {
        return mView.getCoreView().command();
    }
    
    //! 启动指定名称的命令
    public boolean setCommand(String name) {
        return mView.getCoreView().setCommand(mView.getViewAdapter(), name);
    }
    
    //! 添加测试图形
    public void addShapesForTest() {
        mView.getCoreView().addShapesForTest();
    }
    
    //! 放缩显示全部内容
    public void zoomToExtent() {
        mView.getCoreView().zoomToExtent();
    }
    
    //! 返回图形总数
    public int getShapeCount() {
        return mView.getCoreView().getShapeCount();
    }
    
    //! 保存图形到JSON内容
    public String getContent() {
        final MgJsonStorage s = new MgJsonStorage();
        mView.getCoreView().saveShapes(s.storageForWrite());
        final String content = s.stringify(true);
        s.delete();
        return content;
    }
    
    //! 从JSON内容中加载图形
    public boolean setContent(String content) {
        final MgJsonStorage s = new MgJsonStorage();
        boolean ret = mView.getCoreView().loadShapes(s.storageForRead(content));
        s.delete();
        return ret;
    }
    
    //! 从JSON文件中加载图形
    public boolean loadFromFile(String vgfile) {
        final MgJsonStorage s = new MgJsonStorage();
        final String content = readFile(vgfile);
        boolean ret = mView.getCoreView().loadShapes(s.storageForRead(content));
        s.delete();
        return ret;
    }
    
    //! 保存图形到JSON文件
    public boolean saveToFile(String vgfile) {
        boolean ret;
        if (getShapeCount() == 0) {
            ret = new File(vgfile).delete();
        } else {
            ret = writeFile(vgfile, getContent());
        }
        return ret;
    }
    
    public static String readFile(String filename) {
        final File file = new File(filename);
        String content = null;

        if (!file.exists() || !file.isFile()) {
            return null;
        }
        try {
            final FileInputStream fin = new FileInputStream(file);
            int length = fin.available();
            if (length > 0) {
                final byte[] buffer = new byte[length];
                fin.read(buffer);
                content = EncodingUtils.getString(buffer, "UTF-8");
            }
            fin.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        
        return content;
    }
    
    public static boolean writeFile(String filename, String content) {
        int savelen = 0;

        if (content != null && content.length() > 0) {
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

            try {
                final FileOutputStream fout = new FileOutputStream(file);
                final byte[] bytes = content.getBytes();
                fout.write(bytes);
                fout.close();
                savelen = bytes.length;
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        return savelen > 0;
    }
}
