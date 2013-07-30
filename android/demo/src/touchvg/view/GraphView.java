//! \file GraphView.java
//! \brief Android绘图视图类
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

package touchvg.view;

import touchvg.jni.GiCoreView;
import touchvg.jni.GiView;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.util.DisplayMetrics;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;

//! Android绘图视图类
/*! \ingroup GROUP_ANDROID
 */
public class GraphView extends View {
    private CanvasAdapter mCanvasAdapter;   // 画布适配器
    private ViewAdapter mViewAdapter;       // 视图回调适配器
    private GiCoreView mCoreView;           // 内核视图分发器
    private Bitmap mCachedBitmap;           // 用于增量绘图的快照
    private GestureDetector mDetector;      // 手势识别器
    private PaintGestureListener mGestureListener;
    
    //! 普通绘图视图的构造函数
    public GraphView(Context context) {
        super(context);
        initView(context);
        mCoreView = new GiCoreView(null);
        mCoreView.createView(mViewAdapter);
    }
    
    //! 放大镜视图的构造函数
    public GraphView(Context context, GraphView mainView) {
        super(context);
        initView(context);
        mCoreView = new GiCoreView(mainView.getCoreView());
        mCoreView.createMagnifierView(mViewAdapter, mainView.getViewAdapter());
    }
    
    private void initView(Context context) {
        mCanvasAdapter = new CanvasAdapter(this);
        mViewAdapter = new ViewAdapter();
        mGestureListener = new PaintGestureListener(mCoreView, mViewAdapter);
        mDetector = new GestureDetector(context, mGestureListener);
        setDisallowInterceptTouchEvent(true);
        
        final DisplayMetrics dm = context.getApplicationContext().getResources().getDisplayMetrics();
        GiCoreView.setScreenDpi(dm.densityDpi);
    }
    
    //! 返回内核视图分发器对象
    public GiCoreView getCoreView() {
        return mCoreView;
    }
    
    //! 返回视图回调适配器对象
    public GiView getViewAdapter() {
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
    
    public void setGestureEnable(boolean enable) {
    	mGestureListener.setGestureEnable(enable);
    }
    
    public void setDisallowInterceptTouchEvent(boolean enable) {
    	mGestureListener.setGestureEnable(enable);
    	if (!enable) {
            this.setOnTouchListener(null);
        } else {
        	this.setOnTouchListener(new OnTouchListener() {
                public boolean onTouch(View v, MotionEvent event) {
                    return mGestureListener.onTouch(v, event) || mDetector.onTouchEvent(event);
                }
            });
        }
    }
    
    @Override
    protected void onDraw(Canvas canvas) {
        autoBuildCache();
        mCoreView.onSize(mViewAdapter, getWidth(), getHeight());
        
        if (mCanvasAdapter.beginPaint(canvas)) {
            if (mCachedBitmap != null) {
                canvas.drawBitmap(mCachedBitmap, 0, 0, null);
            } else {
                mCoreView.drawAll(mViewAdapter, mCanvasAdapter);
            }
            mCoreView.dynDraw(mViewAdapter, mCanvasAdapter);
            mCanvasAdapter.endPaint();
        }
        mGestureListener.setEndPaintTime(android.os.SystemClock.uptimeMillis());
    }
    
    private void autoBuildCache() {
        if (mCachedBitmap != null && (mCachedBitmap.getWidth() != getWidth()
                                     || mCachedBitmap.getHeight() != getHeight())) {
            mCachedBitmap.recycle();			// 视图宽高改变后就销毁快照
            mCachedBitmap = null;
        }
        if (mCachedBitmap == null) {
            mCachedBitmap = Bitmap.createBitmap(getWidth(), getHeight(), Bitmap.Config.ARGB_8888);
            final Canvas canvas = new Canvas(mCachedBitmap);	// 重建快照
            
            if (mCanvasAdapter.beginPaint(canvas)) {
                canvas.drawColor(0);							// 透明背景
                mCoreView.drawAll(mViewAdapter, mCanvasAdapter);
                mCanvasAdapter.endPaint();
            } else {
                mCachedBitmap.recycle();
                mCachedBitmap = null;
            }
        }
    }
    
    @Override
    protected void onDetachedFromWindow() {
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
        if (mCachedBitmap != null) {
            mCachedBitmap.recycle();
            mCachedBitmap = null;
        }
        if (mGestureListener != null) {
        	mGestureListener.release();
        	mGestureListener = null;
        }
        mDetector = null;
        super.onDetachedFromWindow();
    }
    
    //! 视图回调适配器
    private class ViewAdapter extends GiView {
        @Override
        public void regenAll() {
            if (mCachedBitmap != null) {
                mCachedBitmap.recycle();
                mCachedBitmap = null;
            }
            invalidate();
        }
        
        @Override
        public void regenAppend() {
            if (mCachedBitmap != null) {
                final Canvas canvas = new Canvas(mCachedBitmap);
                if (mCanvasAdapter.beginPaint(canvas)) {
                    mCoreView.drawAppend(mViewAdapter, mCanvasAdapter);
                    mCanvasAdapter.endPaint();
                }
            }
            invalidate();
        }
        
        @Override
        public void redraw() {
            invalidate();
        }
    }
}
