//! \file PaintView.java
//! \brief Android绘图视图类
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

package touchvg.view;

import touchvg.jni.GiCoreView;
import touchvg.jni.GiView;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
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
            	if (mDynDrawView != null && event.getEventTime() > mDynDrawView.getEndPaintTime()) {
            		mCoreView.onTouch(mViewAdapter, event.getX(), event.getY());
            	}
                return true;
            }
        });
    }
    
    public View getDynDrawView() {
    	if (mDynDrawView == null) {
    		mDynDrawView = new DynDrawView(getContext());
    	}
    	return mDynDrawView;
    }
    
    public long getDrawnTime() {
    	return mDrawnTime;
    }

    @Override
    protected void onDraw(Canvas canvas) {
    	long ms = System.currentTimeMillis();
    	if (mCanvasAdapter.beginPaint(canvas)) {
    		canvas.drawColor(Color.TRANSPARENT);
			mCoreView.draw(mCanvasAdapter);
			mCanvasAdapter.endPaint();
		}
        mDrawnTime = System.currentTimeMillis() - ms;
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
        mDynDrawView = null;
        super.onDetachedFromWindow();
    }
    
    private class DynDrawView extends View {
        private CanvasAdapter mCanvasAdapter;

        public DynDrawView(Context context) {
            super(context);
            mCanvasAdapter = new CanvasAdapter(this);
        }
        
        public long getEndPaintTime() {
        	return mEndPaintTime;
        }

        @Override
        protected void onDraw(Canvas canvas) {
        	long ms = System.currentTimeMillis();
            if (mCanvasAdapter.beginPaint(canvas)) {
            	canvas.drawColor(Color.TRANSPARENT);
            	mCoreView.dyndraw(mCanvasAdapter);
                mCanvasAdapter.endPaint();
            }
            mDrawnTime = System.currentTimeMillis() - ms;
            mEndPaintTime = android.os.SystemClock.uptimeMillis();
        }

        @Override
        protected void onDetachedFromWindow() {
            if (mCanvasAdapter != null) {
                mCanvasAdapter.delete();
                mCanvasAdapter = null;
            }
            super.onDetachedFromWindow();
        }
    }
    
    private class ViewAdapter extends GiView {
    	@Override
    	public void regen() {
    		GraphView.this.invalidate();
    		if (mDynDrawView != null) {
    			mDynDrawView.invalidate();
    		}
    	}
    	
    	@Override
    	public void redraw() {
    		if (mDynDrawView != null) {
    			mDynDrawView.invalidate();
    		}
    	}
    }
}
