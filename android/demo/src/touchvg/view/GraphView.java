//! \file PaintView.java
//! \brief Android绘图视图类
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

package touchvg.view;

import touchvg.jni.GiCoreView;
import touchvg.jni.GiView;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
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
                if (event.getAction() == MotionEvent.ACTION_UP) {
                    mViewAdapter.regen();
                }
                else if ((mDynDrawView != null
                        && event.getEventTime() > mDynDrawView.getEndPaintTime())
                        || (mDynDrawView == null
                        && event.getEventTime() > mEndPaintTime)) {
                    mCoreView.onTouch(mViewAdapter, event.getX(), event.getY());
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
            canvas.drawColor(Color.TRANSPARENT);
            mCoreView.draw(mCanvasAdapter);
            if (mDynDrawView == null) {
                mCoreView.dyndraw(mCanvasAdapter);
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
        public void regen() {
            GraphView.this.invalidate();
            if (mDynDrawView != null) {
                mDynDrawView.doDraw();
            }
        }
        
        @Override
        public void redraw() {
            if (mDynDrawView != null) {
                mDynDrawView.doDraw();
            }
            else {
                GraphView.this.invalidate();
            }
        }
    }
}
