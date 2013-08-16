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
    private DynDrawView mDynDrawView;       // 动态绘图视图
    private CanvasAdapter mCanvasAdapter;   // 画布适配器
    private ViewAdapter mViewAdapter;       // 视图回调适配器
    private GiCoreView mCoreView;           // 内核视图分发器
    private Bitmap mCachedBitmap;           // 用于增量绘图的快照
    private GestureDetector mDetector;      // 手势识别器
    private PaintGestureListener mGestureListener;
    private boolean mGestureEnable = true;  // 是否允许交互
    private boolean mRegenning = false;     // 是否正在regenAll
    private static GraphView mActiveView;   // 当前激活视图

    //! 普通绘图视图的构造函数
    public GraphView(Context context) {
        super(context);
        createAdapter(context);
        mCoreView = new GiCoreView(null);
        mCoreView.createView(mViewAdapter);
        mActiveView = this;
        initView(context);
    }

    //! 放大镜视图的构造函数
    public GraphView(Context context, GraphView mainView) {
        super(context);
        createAdapter(context);
        mainView = mainView != null ? mainView : mActiveView;
        mCoreView = new GiCoreView(mainView.coreView());
        mCoreView.createMagnifierView(mViewAdapter, mainView.viewAdapter());
        initView(context);
    }
    
    private void createAdapter(Context context) {
        mCanvasAdapter = new CanvasAdapter(this);
        mViewAdapter = new ViewAdapter();
    }

    private void initView(Context context) {
        mGestureListener = new PaintGestureListener(mCoreView, mViewAdapter);
        mDetector = new GestureDetector(context, mGestureListener);

        final DisplayMetrics dm = context.getApplicationContext().getResources().getDisplayMetrics();
        GiCoreView.setScreenDpi(dm.densityDpi);
        setLayerType(View.LAYER_TYPE_SOFTWARE, null);
        
        this.setOnTouchListener(new OnTouchListener() {
            public boolean onTouch(View v, MotionEvent event) {
                mActiveView = GraphView.this;
                return mGestureEnable && (mGestureListener.onTouch(v, event)
                        || mDetector.onTouchEvent(event));
            }
        });
    }
    
    //! 返回当前激活视图
    public static GraphView activeView() {
        return mActiveView;
    }
    
    //! 创建动态图形视图
    public View createDynDrawView(Context context) {
        if (mDynDrawView == null) {
            mDynDrawView = new DynDrawView(context, viewAdapter(), coreView());
        }
        return mDynDrawView;
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
        if (mCachedBitmap != null) {
        	drawShapes(canvas, true);
        } else {
        	mViewAdapter.regenAll();
        }
    }
    
    private void drawShapes(Canvas canvas, boolean dyndraw) {
        if (mCachedBitmap != null && mCanvasAdapter.beginPaint(canvas)) {
            if (!mRegenning || !dyndraw) {
                if (mCachedBitmap != null && dyndraw) {
                    canvas.drawBitmap(mCachedBitmap, 0, 0, null);
                } else {
                    if (this.getBackground() != null) {
                        this.getBackground().draw(canvas);
                    } else {
                        canvas.drawColor(Color.WHITE);
                    }
                    mCoreView.drawAll(mViewAdapter, mCanvasAdapter);
                }
            }
            if (dyndraw) {
                mCoreView.dynDraw(mViewAdapter, mCanvasAdapter);
            }
            mCanvasAdapter.endPaint();
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
        if (mGestureListener != null) {
            mGestureListener.release();
            mGestureListener = null;
        }
        if (mCachedBitmap != null) {
            mCachedBitmap.recycle();
            mCachedBitmap = null;
        }
        mDetector = null;
        mDynDrawView = null;
        
        super.onDetachedFromWindow();
    }

    //! 视图回调适配器
    private class ViewAdapter extends GiView {
        @Override
        public void regenAll() {
            if (getWidth() < 1 || getHeight() < 1 || mRegenning) {
                return;
            }
            mRegenning = true;
            
            if (mCachedBitmap != null && (mCachedBitmap.getWidth() != getWidth()
                    || mCachedBitmap.getHeight() != getHeight())) {
                mCachedBitmap.recycle();
                mCachedBitmap = null;
            }
            try {
                if (mCachedBitmap == null) {
                    mCachedBitmap = Bitmap.createBitmap(getWidth(), getHeight(), Bitmap.Config.ARGB_8888);
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
            
            final Thread drawThread = new Thread(new Runnable() {
                public void run() {
                    drawShapes(new Canvas(mCachedBitmap), false);
                    mRegenning = false;
                    postInvalidate();
                    if (mDynDrawView != null) {
                        mDynDrawView.doDraw();
                    }
                }
            });
            drawThread.start();
        }

        @Override
        public void regenAppend() {
            if (mCachedBitmap != null && !mRegenning) {
                final Canvas canvas = new Canvas(mCachedBitmap);
                if (mCanvasAdapter.beginPaint(canvas)) {
                    mCoreView.drawAppend(mViewAdapter, mCanvasAdapter);
                    mCanvasAdapter.endPaint();
                }
                invalidate();
                if (mDynDrawView != null) {
                    mDynDrawView.doDraw();
                }
            } else {
                regenAll();
            }
        }

        @Override
        public void redraw() {
            if (mCachedBitmap == null) {
            	regenAll();
            }
            if (mDynDrawView != null) {
                mDynDrawView.doDraw();
            }
            else {
                invalidate();
            }
        }
    }
}
