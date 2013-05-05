//! \file GraphView.java
//! \brief Android绘图视图类
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

package vgdemo.testview.view;

import touchvg.jni.GiCoreView;
import touchvg.jni.GiView;
import touchvg.jni.GiGestureType;
import touchvg.jni.GiGestureState;
import touchvg.view.CanvasAdapter;
import android.content.Context;
import android.graphics.Canvas;
import android.os.SystemClock;
import android.view.MotionEvent;
import android.view.View;

//! Android绘图视图类
/*! \ingroup GROUP_ANDROID
 */
public class GraphView extends View {
    private CanvasAdapter mCanvasAdapter;
    private ViewAdapter mViewAdapter;
    private GiCoreView mCoreView;
    private DynDrawView mDynDrawView;
    private long mDrawnTime;
    private long mEndPaintTime;

    public GraphView(Context context) {
        super(context);
        mCanvasAdapter = new CanvasAdapter(this);
        mViewAdapter = new ViewAdapter();
        mCoreView = new GiCoreView();
        
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
                else if ((mDynDrawView != null
                        && event.getEventTime() > mDynDrawView.getEndPaintTime())
                        || (mDynDrawView == null
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
    
    public long getDrawnTime() {
        return mDrawnTime;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        long ms = SystemClock.currentThreadTimeMillis();
        if (mCanvasAdapter.beginPaint(canvas)) {
            mCoreView.drawAll(mCanvasAdapter);
            if (mDynDrawView == null) {
                mCoreView.dynDraw(mCanvasAdapter);
            }
            mCanvasAdapter.endPaint();
        }
        mDrawnTime = SystemClock.currentThreadTimeMillis() - ms;
        mEndPaintTime = android.os.SystemClock.uptimeMillis();
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
    
    private class ViewAdapter extends GiView {
        @Override
        public void regenAll() {
            invalidate();
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
                invalidate();
            }
        }
    }
}
