//! \file GraphSfView.java
//! \brief Android绘图视图类
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

package vgdemo.testview.view;

import touchvg.jni.GiCoreView;
import touchvg.jni.GiGestureState;
import touchvg.jni.GiGestureType;
import touchvg.jni.GiView;
import touchvg.view.CanvasAdapter;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.graphics.PorterDuff.Mode;
import android.os.SystemClock;
import android.util.DisplayMetrics;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

//! Android绘图视图类
/*! \ingroup GROUP_ANDROID
 */
public class GraphSfView extends SurfaceView {
    private CanvasAdapter mCanvasAdapter;
    private ViewAdapter mViewAdapter;
    private GiCoreView mCoreView;
    private DynDrawView mDynDrawView;
    private long mDrawnTime;
    private long mEndPaintTime;
    private int mBkColor = Color.WHITE;

    public GraphSfView(Context context) {
        super(context);
        mCanvasAdapter = new CanvasAdapter(this);
        mViewAdapter = new ViewAdapter();
        mCoreView = new GiCoreView();
        
        DisplayMetrics dm = context.getApplicationContext().getResources().getDisplayMetrics();
        GiCoreView.setScreenDpi(dm.densityDpi);
        
        getHolder().addCallback(new SurfaceCallback());
        
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
                else if ((mDynDrawView != null && !mDynDrawView.isDrawing()
                        && event.getEventTime() > mDynDrawView.getEndPaintTime())
                        || (mDynDrawView == null && !isDrawing()
                        && event.getEventTime() > mEndPaintTime)) {
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
    
    public void setDynDrawView(DynDrawView view) {
        mDynDrawView = view;
        if (mDynDrawView != null) {
            mDynDrawView.setCoreView(mCoreView);
        }
    }
    
    public void setBackgroundColor(int color) {
        mBkColor = color;
        if (mBkColor == Color.TRANSPARENT) {
            setZOrderOnTop(true);
            getHolder().setFormat(PixelFormat.TRANSPARENT);
        }
    }
    
    public long getDrawnTime() {
        return mDrawnTime;
    }
    
    public boolean isDrawing() {
        return mCanvasAdapter != null && mCanvasAdapter.isDrawing();
    }
    
    public void drawShapes(Canvas canvas) {
        if (mCanvasAdapter.beginPaint(canvas)) {
            if (getBackground() == null) {
                if (mBkColor == Color.TRANSPARENT)
                    canvas.drawColor(mBkColor, Mode.CLEAR);
                else
                    canvas.drawColor(mBkColor);
            }
            else {
                getBackground().draw(canvas);
            }
            mCoreView.drawAll(mCanvasAdapter);
            if (mDynDrawView == null) {
                mCoreView.dynDraw(mCanvasAdapter);
            }
            mCanvasAdapter.endPaint();
        }
    }

    @Override
    protected void onDetachedFromWindow() {
        if (mDynDrawView != null) {
            mDynDrawView.setCoreView(null);
            mDynDrawView = null;
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
        super.onDetachedFromWindow();
    }
    
    private void doDraw() {
        new Thread(new DrawThread()).start();
    }
    
    private class SurfaceCallback implements SurfaceHolder.Callback {
        public void surfaceCreated(SurfaceHolder holder) {
            doDraw();
        }

        public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        	mCoreView.onSize(mViewAdapter, width, height);
        }

        public void surfaceDestroyed(SurfaceHolder holder) {
        }
    }
    
    private class DrawThread implements Runnable {
        
        public void run() {
            long ms = SystemClock.currentThreadTimeMillis();
            Canvas canvas = null;
            
            try {
                canvas = getHolder().lockCanvas();
                if (canvas != null) {
                    drawShapes(canvas);
                }
            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                if (canvas != null) {
                    getHolder().unlockCanvasAndPost(canvas);
                }
            }
            mDrawnTime = SystemClock.currentThreadTimeMillis() - ms;
            mEndPaintTime = android.os.SystemClock.uptimeMillis();
        }
    }
    
    private class ViewAdapter extends GiView {
        @Override
        public void regenAll() {
            doDraw();
            if (mDynDrawView != null) {
                mDynDrawView.doDraw();
            }
        }
        
        @Override
        public void regenAppend() {
            regenAll();
        }
        
        @Override
        public void redraw() {
            if (mDynDrawView != null) {
                mDynDrawView.doDraw();
            }
            else {
                doDraw();
            }
        }
    }
}
