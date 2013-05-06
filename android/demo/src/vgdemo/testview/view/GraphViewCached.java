//! \file GraphViewCached.java
//! \brief Android绘图视图类，使用缓存位图
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

package vgdemo.testview.view;

import touchvg.jni.GiCoreView;
import touchvg.jni.GiGestureState;
import touchvg.jni.GiGestureType;
import touchvg.jni.GiView;
import touchvg.view.CanvasAdapter;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.os.SystemClock;
import android.util.DisplayMetrics;
import android.view.MotionEvent;
import android.view.View;

//! Android绘图视图类，使用缓存位图
/*! \ingroup GROUP_ANDROID
 */
public class GraphViewCached extends View {
    private CanvasAdapter mCanvasAdapter;
    private ViewAdapter mViewAdapter;
    private GiCoreView mCoreView;
    private Bitmap mCacheBitmap;
    private long mDrawnTime;
    private long mEndPaintTime;

    public GraphViewCached(Context context) {
        super(context);
        mCanvasAdapter = new CanvasAdapter(this);
        mViewAdapter = new ViewAdapter();
        mCoreView = new GiCoreView();
        
        DisplayMetrics dm = context.getApplicationContext().getResources().getDisplayMetrics();
        GiCoreView.setScreenDpi(dm.densityDpi);
        
        this.setOnTouchListener(new OnTouchListener() {
            public boolean onTouch(View v, MotionEvent event) {
            	if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    mCoreView.onGesture(mViewAdapter, GiGestureType.kGiGesturePan, 
                            GiGestureState.kGiGestureBegan, event.getX(), event.getY());
                }
            	else if (event.getAction() == MotionEvent.ACTION_UP) {
                    mCoreView.onGesture(mViewAdapter, GiGestureType.kGiGesturePan, 
                            GiGestureState.kGiGestureEnded, event.getX(), event.getY());
                }
                else if (event.getEventTime() > mEndPaintTime) {
                    mCoreView.onGesture(mViewAdapter, GiGestureType.kGiGesturePan, 
                            GiGestureState.kGiGestureMoved, event.getX(), event.getY());
                }
                return true;
            }
        });
    }
    
    public GiCoreView getCoreView() {
        return mCoreView;
    }
    
    public long getDrawnTime() {
        return mDrawnTime;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        long ms = SystemClock.currentThreadTimeMillis();
        
        autoBuildCache();
        mCoreView.onSize(mViewAdapter, this.getWidth(), this.getHeight());
        
        if (mCanvasAdapter.beginPaint(canvas)) {
        	if (mCacheBitmap != null) {
        		canvas.drawBitmap(mCacheBitmap, 0, 0, null);
        	}
        	else {
        		mCoreView.drawAll(mCanvasAdapter);
        	}
            mCoreView.dynDraw(mCanvasAdapter);
            mCanvasAdapter.endPaint();
        }
        mDrawnTime = SystemClock.currentThreadTimeMillis() - ms;
        mEndPaintTime = android.os.SystemClock.uptimeMillis();
    }
    
    private void autoBuildCache() {
    	if (mCacheBitmap != null && (mCacheBitmap.getWidth() != getWidth()
    			|| mCacheBitmap.getHeight() != getHeight())) {
    		mCacheBitmap.recycle();
            mCacheBitmap = null;
    	}
    	if (mCacheBitmap == null) {
    		mCacheBitmap = Bitmap.createBitmap(getWidth(), getHeight(), Bitmap.Config.ARGB_8888);
    		Canvas canvas = new Canvas(mCacheBitmap);
    		
    		if (mCanvasAdapter.beginPaint(canvas)) {
    			canvas.drawColor(0);
    			mCoreView.drawAll(mCanvasAdapter);
    			mCanvasAdapter.endPaint();
    		}
    		else {
    			mCacheBitmap.recycle();
                mCacheBitmap = null;
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
        if (mCacheBitmap != null) {
        	mCacheBitmap.recycle();
        	mCacheBitmap = null;
        }
        super.onDetachedFromWindow();
    }
    
    private class ViewAdapter extends GiView {
        @Override
        public void regenAll() {
        	if (mCacheBitmap != null) {
            	mCacheBitmap.recycle();
            	mCacheBitmap = null;
            }
            invalidate();
        }
        
        @Override
        public void regenAppend() {
        	if (mCacheBitmap != null) {
        		Canvas canvas = new Canvas(mCacheBitmap);
        		if (mCanvasAdapter.beginPaint(canvas)) {
        			mCoreView.drawAppend(mCanvasAdapter);
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
