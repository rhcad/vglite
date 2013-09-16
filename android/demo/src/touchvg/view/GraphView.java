//! \file GraphView.java
//! \brief Android绘图视图类
// Copyright (c) 2012-2013, https://github.com/rhcad/touchvg

package touchvg.view;

import java.util.ArrayList;

import touchvg.jni.GiCoreView;
import touchvg.jni.GiView;
import touchvg.jni.Ints;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.util.DisplayMetrics;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;

//! Android绘图视图类
/*! \ingroup GROUP_ANDROID
 */
public class GraphView extends View {
    private static GraphView mActiveView;   // 当前激活视图
    private CanvasAdapter mCanvasAdapter;   // 画布适配器
    private CanvasAdapter mCanvasRegen;     // 画布适配器
    private ViewAdapter mViewAdapter;       // 视图回调适配器
    private GiCoreView mCoreView;           // 内核视图分发器
    private GestureDetector mGestureDetector;      // 手势识别器
    private GestureListener mGestureListener; // 手势识别实现
    private boolean mGestureEnable = true;  // 是否允许交互
    private boolean mRegenning = false;     // 是否正在regenAll
    private Bitmap mCachedBitmap;           // 缓存快照
    private int mBkColor = Color.TRANSPARENT;

    //! 普通绘图视图的构造函数
    public GraphView(Context context) {
        super(context);
        createAdapter(context);
        mCoreView = new GiCoreView(null);
        mCoreView.createView(mViewAdapter);
        initView(context);
    }

    //! 放大镜视图的构造函数
    public GraphView(Context context, GraphView mainView) {
        super(context);
        createAdapter(context);
        mCoreView = new GiCoreView(mainView.coreView());
        mCoreView.createMagnifierView(mViewAdapter, mainView.viewAdapter());
        initView(context);
    }

    private void createAdapter(Context context) {
        mCanvasAdapter = new CanvasAdapter(this);
        mCanvasRegen = new CanvasAdapter(this);
        mViewAdapter = new ViewAdapter();
    }

    private void initView(Context context) {
        mGestureListener = new GestureListener(mCoreView, mViewAdapter);
        mGestureDetector = new GestureDetector(context, mGestureListener);

        final DisplayMetrics dm = context.getApplicationContext().getResources().getDisplayMetrics();
        GiCoreView.setScreenDpi(dm.densityDpi);         // 应用API
        setLayerType(View.LAYER_TYPE_SOFTWARE, null);   // 避免路径太大不能渲染

        this.setOnTouchListener(new OnTouchListener() {
            public boolean onTouch(View v, MotionEvent event) {
                mActiveView = GraphView.this;
                return mGestureEnable && (mGestureListener.onTouch(v, event)
                        || mGestureDetector.onTouchEvent(event));
            }
        });
    }

    //! 返回当前激活视图
    public static GraphView activeView() {
        return mActiveView;
    }

    //! 返回内核视图分发器对象
    public GiCoreView coreView() {
        return mCoreView;
    }

    //! 返回视图回调适配器对象
    public GiView viewAdapter() {
        return mViewAdapter;
    }

    //! 释放临时缓存
    public void clearCachedData() {
        mCoreView.clearCachedData();
        if (mCachedBitmap != null) {
            mCachedBitmap.recycle();
            mCachedBitmap = null;
        }
    }

    //! 设置背景色
    public void setBackgroundColor(int color) {
        mBkColor = color;
        mCoreView.setBkColor(mViewAdapter, color);
        regen(false);
    }

    //! 设置是否允许触摸交互
    public void setGestureEnable(boolean enabled) {
        mGestureEnable = enabled;
        mGestureListener.setGestureEnable(enabled);
    }

    //! 得到静态图形的快照
    public Bitmap snapshot() {
        if (mCachedBitmap == null) {
            mCachedBitmap = Bitmap.createBitmap(getWidth(), getHeight(), Bitmap.Config.ARGB_8888);
            synchronized(mCachedBitmap) {
                mCoreView.onSize(mViewAdapter, getWidth(), getHeight());
                mCachedBitmap.eraseColor(mBkColor);
                drawShapes(new Canvas(mCachedBitmap), mCanvasAdapter, false);
            }
        }
        return mCachedBitmap;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        mCoreView.onSize(mViewAdapter, getWidth(), getHeight());
        if (mCachedBitmap != null) {
            drawShapes(canvas, mCanvasAdapter, true);
        }
        else if (!regen(false)) {
            canvas.drawColor(mBkColor);
            drawShapes(canvas, mCanvasAdapter, true);
        }
    }

    private void drawShapes(Canvas canvas, CanvasAdapter adapter, boolean dyndraw) {
        if (adapter.beginPaint(canvas)) {
            if (mCachedBitmap == null || !dyndraw) {
                if (this.getBackground() != null) {
                    this.getBackground().draw(canvas);
                }
                mCoreView.drawAll(mViewAdapter, adapter);
            }
            else if (mCachedBitmap != null) {
                synchronized(mCachedBitmap) {
                    canvas.drawBitmap(mCachedBitmap, 0, 0, null);
                }
            }

            if (dyndraw) {
                mCoreView.dynDraw(mViewAdapter, adapter);
            }
            adapter.endPaint();
        }
    }

    private boolean regen(boolean fromRegenAll) {
        if (getWidth() < 1 || getHeight() < 1 || mRegenning) {
            return true;
        }
        try {
            if (mCachedBitmap == null) {
                mCachedBitmap = Bitmap.createBitmap(getWidth(),
                        getHeight(), Bitmap.Config.ARGB_8888);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        if (mCachedBitmap != null) {
            mRegenning = true;
            new Thread(new Runnable() {
                public void run() {
                    synchronized(mCachedBitmap) {
                        mCachedBitmap.eraseColor(mBkColor);
                        drawShapes(new Canvas(mCachedBitmap), mCanvasRegen, false);
                    }
                    mRegenning = false;
                    postInvalidate();
                }
            }).start();
        }
        else if (fromRegenAll) {
            invalidate();
        }

        return mCachedBitmap != null;
    }

    @Override
    protected void onDetachedFromWindow() {
        if (mActiveView == this) {
            mActiveView = null;
        }
        if (mViewAdapter != null) {
            mCoreView.destoryView(mViewAdapter);
            mViewAdapter.delete();
            mViewAdapter = null;
        }
        if (mCoreView != null) {
            mCoreView.delete();
            mCoreView = null;
        }
        if (mCanvasAdapter != null) {
            mCanvasAdapter.delete();
            mCanvasAdapter = null;
        }
        if (mCanvasRegen != null) {
            mCanvasRegen.delete();
            mCanvasRegen = null;
        }
        if (mGestureListener != null) {
            mGestureListener.release();
            mGestureListener = null;
        }
        if (mCachedBitmap != null) {
            mCachedBitmap.recycle();
            mCachedBitmap = null;
        }
        mGestureDetector = null;

        super.onDetachedFromWindow();
    }
    
    //! 当前命令改变的通知
    public interface CommandChangedListener {
        void onCommandChanged(GraphView view);
    }
    
    //! 图形选择集改变的通知
    public interface SelectionChangedListener {
        void onSelectionChanged(GraphView view);
    }
    
    //! 图形数据改变的通知
    public interface ContentChangedListener {
        void onContentChanged(GraphView view);
    }
    
    private ArrayList<CommandChangedListener> commandChangedListeners;
    private ArrayList<SelectionChangedListener> selectionChangedListeners;
    private ArrayList<ContentChangedListener> contentChangedListeners;
    
    //! 添加当前命令改变的观察者
    public void setOnCommandChangedListener(CommandChangedListener listener) {
        if (this.commandChangedListeners == null)
            this.commandChangedListeners = new ArrayList<CommandChangedListener>();
        this.commandChangedListeners.add(listener);
    }
    
    //! 添加图形选择集改变的观察者
    public void setOnSelectionChangedListener(SelectionChangedListener listener) {
        if (this.selectionChangedListeners == null)
            this.selectionChangedListeners = new ArrayList<SelectionChangedListener>();
        this.selectionChangedListeners.add(listener);
    }
    
    //! 添加图形数据改变的观察者
    public void setOnContentChangedListener(ContentChangedListener listener) {
        if (this.contentChangedListeners == null)
            this.contentChangedListeners = new ArrayList<ContentChangedListener>();
        this.contentChangedListeners.add(listener);
    }

    //! 视图回调适配器
    /*! \ingroup GROUP_ANDROID
     */
    private class ViewAdapter extends GiView {
        @Override
        public void regenAll() {
            if (mCachedBitmap != null &&
                    (mCachedBitmap.getWidth() != getWidth()
                    || mCachedBitmap.getHeight() != getHeight())) {
                mCachedBitmap.recycle();
                mCachedBitmap = null;
            }
            regen(true);
        }

        @Override
        public void regenAppend() {
            if (mCachedBitmap != null && !mRegenning) {
                synchronized(mCachedBitmap) {
                    if (mCanvasAdapter.beginPaint(new Canvas(mCachedBitmap))) {
                        mCoreView.drawAppend(mViewAdapter, mCanvasAdapter);
                        mCanvasAdapter.endPaint();
                    }
                }
            }
            invalidate();
        }

        @Override
        public void redraw() {
            if (!mRegenning) {
                invalidate();
            }
        }
        
        @Override
        public boolean isContextActionsVisible() {
            return false;
        }
        
        @Override
        public boolean showContextActions(Ints actions, float x, float y, float w, float h) {
            return false;
        }
        
        @Override
        public void commandChanged() {
            if (commandChangedListeners != null) {
                for (CommandChangedListener listener : commandChangedListeners) {
                    listener.onCommandChanged(GraphView.this);
                }
            }
        }
        
        @Override
        public void selectionChanged() {
            if (selectionChangedListeners != null) {
                for (SelectionChangedListener listener : selectionChangedListeners) {
                    listener.onSelectionChanged(GraphView.this);
                }
            }
        }
        
        @Override
        public void contentChanged() {
            if (contentChangedListeners != null) {
                for (ContentChangedListener listener : contentChangedListeners) {
                    listener.onContentChanged(GraphView.this);
                }
            }
        }
    }
}
