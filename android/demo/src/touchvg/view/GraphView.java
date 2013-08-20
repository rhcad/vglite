//! \file GraphView.java
//! \brief Android绘图视图类
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

package touchvg.view;

import touchvg.jni.GiCoreView;
import touchvg.jni.GiView;
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
    private CanvasAdapter mCanvasAdapter;   // 画布适配器
    private CanvasAdapter mCanvasRegen;     // 画布适配器
    private ViewAdapter mViewAdapter;       // 视图回调适配器
    private GiCoreView mCoreView;           // 内核视图分发器
    private GestureDetector mGestureDetector;      // 手势识别器
    private PaintGestureListener mGestureListener; // 手势识别实现
    private boolean mGestureEnable = true;  // 是否允许交互
    private boolean mRegenning = false;     // 是否正在regenAll
    private Bitmap mCachedBitmap;           // 缓存快照
    private static GraphView mActiveView;   // 当前激活视图

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
        mGestureListener = new PaintGestureListener(mCoreView, mViewAdapter);
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

    //! 设置是否允许触摸交互
    public void setGestureEnable(boolean enabled) {
        mGestureEnable = enabled;
        mGestureListener.setGestureEnable(enabled);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        mCoreView.onSize(mViewAdapter, getWidth(), getHeight());
        if (mCachedBitmap != null || !mViewAdapter.regenAll(canvas)) {
            drawShapes(canvas, mCanvasAdapter);
        }
    }
    
    private void drawShapes(Canvas canvas, CanvasAdapter adapter) {
        if (adapter.beginPaint(canvas)) {
            if (mCachedBitmap == null || adapter != mCanvasAdapter) {
                if (this.getBackground() != null) {
                    this.getBackground().draw(canvas);
                } else {
                    canvas.drawColor(Color.WHITE);
                }
                mCoreView.drawAll(mViewAdapter, adapter);
            }
            else if (mCachedBitmap != null) {
                synchronized(mCachedBitmap) {
                    canvas.drawBitmap(mCachedBitmap, 0, 0, null);
                }
            }
            
            if (adapter == mCanvasAdapter) {
                mCoreView.dynDraw(mViewAdapter, adapter);
            }
            adapter.endPaint();
        }
    }

    @Override
    protected void onDetachedFromWindow() {
        if (mActiveView == this) {
            mActiveView = null;
        }
        if (mViewAdapter != null) {
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

    //! 视图回调适配器
    private class ViewAdapter extends GiView {
        public boolean regenAll(Canvas canvas) {
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
                            drawShapes(new Canvas(mCachedBitmap), mCanvasRegen);
                        }
                        mRegenning = false;
                        postInvalidate();
                    }
                }).start();
            }
            else if (canvas == null) {
                invalidate();
            }
            
            return mCachedBitmap != null;
        }
        
        @Override
        public void regenAll() {
            if (mCachedBitmap != null && !mRegenning
                    && (mCachedBitmap.getWidth() != getWidth()
                    || mCachedBitmap.getHeight() != getHeight())) {
                mCachedBitmap.recycle();
                mCachedBitmap = null;
            }
            regenAll(null);
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
    }
}
