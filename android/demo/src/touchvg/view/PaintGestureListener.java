package touchvg.view;

import android.view.MotionEvent;
import android.view.View;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.View.OnTouchListener;
import touchvg.jni.GiCoreView;
import touchvg.jni.GiGestureState;
import touchvg.jni.GiGestureType;
import touchvg.jni.GiView;

public class PaintGestureListener extends SimpleOnGestureListener {
	private GiCoreView mCoreView;                   // 内核视图分发器
	private GiView mAdapter;                        // 视图回调适配器
	private long mEndPaintTime = 0;                 // 重绘结束时刻，用于忽略多余触摸移动消息
	private boolean mGestureEnable = true;
    private GiGestureState mGestureState = GiGestureState.kGiGesturePossible;
    private GiGestureType mGestureType = GiGestureType.kGiGestureUnknown;
    private int mFingerCount;
    private float mX = 1;
    private float mY = 1;
    private float mX1 = 1;
    private float mY1 = 1;
    private boolean mMoving = false;
    
    public PaintGestureListener(GiCoreView coreView, GiView adapter) {
    	mCoreView = coreView;
    	mAdapter = adapter;
    }
    
    public void release() {
    	mCoreView = null;
    	mAdapter = null;
    }
    
    public void setGestureEnable(boolean enable) {
        mGestureEnable = enable;
        if (!enable && mGestureState == GiGestureState.kGiGestureMoved) {
            mGestureState = GiGestureState.kGiGestureCancel;
            mCoreView.onGesture(mAdapter, mGestureType, mGestureState, 0, 0);
        }
        if (!enable) {
        	mCoreView.setCommand(mAdapter, null);
        }
    }
    
    public void setEndPaintTime(long endTime) {
    	mEndPaintTime = endTime;
    }
    
    public boolean onTouch(View v, MotionEvent event) {
    	if (!mGestureEnable) {
            return true;
        }
    	
    	if (event.getAction() == MotionEvent.ACTION_UP || event.getAction() == MotionEvent.ACTION_CANCEL) {
    		v.getParent().requestDisallowInterceptTouchEvent(false);	// 松开后允许父视图拦截触摸事件
    	}
    	else {
    		v.getParent().requestDisallowInterceptTouchEvent(true);		// 按下后不允许父视图拦截触摸事件
    	}
    	
    	switch (event.getAction()) {
        case MotionEvent.ACTION_DOWN:
            mFingerCount = event.getPointerCount();
            mX = event.getX(0);
            mY = event.getY(0);
            if (mFingerCount > 1) {
                mX1 = event.getX(1);
                mY1 = event.getY(1);
            } // call mCoreView.onGesture in onTouch() after later.
            break;
        case MotionEvent.ACTION_MOVE:
            if ((mGestureState == GiGestureState.kGiGestureBegan)
                    && !mMoving
                    && Math.hypot(mX - event.getX(0),
                            mY - event.getY(0)) > 10) {
                mMoving = true;
            }
            if (!mMoving) {
                break;
            }

            if (event.getPointerCount() == 2 && mFingerCount == 1) {
                mGestureState = GiGestureState.kGiGestureEnded;
                mCoreView.twoFingersMove(mAdapter, mGestureState, mX, mY, mX1, mY1);
                mGestureType = GiGestureType.kGiGestureUnknown;
                mFingerCount = event.getPointerCount();
                mX = event.getX(0);
                mY = event.getY(0);
                mX1 = event.getX(1);
                mY1 = event.getY(1);
                mGestureState = GiGestureState.kGiGestureBegan;

            }
            if (mGestureState == GiGestureState.kGiGestureBegan) {
                mGestureType = mFingerCount > 1 ? GiGestureType.kZoomRotatePan
                        : GiGestureType.kSinglePan;
                mCoreView.onGesture(mAdapter, mGestureType, mGestureState, mFingerCount,
                        mX, mY, mX1, mY1);
            }
            mGestureState = GiGestureState.kGiGestureMoved;
            mMoving = true;
            mX = event.getX(0);
            mY = event.getY(0);
            if (event.getPointerCount() > 1) {
                mX1 = event.getX(1);
                mY1 = event.getY(1);
            }
            if (event.getPointerCount() >= mFingerCount) {
                mCoreView.onGesture(mAdapter, mGestureType, mGestureState, mFingerCount,
                        mX, mY, mX1, mY1);
            }
            break;

        case MotionEvent.ACTION_UP:
            if (mMoving) {
                mMoving = false;
                mGestureState = GiGestureState.kGiGestureEnded;
                mCoreView.onGesture(mAdapter, mGestureType, mGestureState, mFingerCount,
                        mX, mY, mX1, mY1);
                mGestureType = GiGestureType.kGiGestureUnknown;
                mFingerCount = 0;
            }
            break;
        }
    	
        return false;
    }
    
    @Override
    public boolean onSingleTapConfirmed(MotionEvent e) {
        mCoreView.onGesture(mAdapter, GiGestureType.kSingleTap,
                GiGestureState.kGiGestureBegan, 1, e.getX(), e.getY(), 0, 0);
        return true;
    }

    @Override
    public void onLongPress(MotionEvent e) {
        if (mGestureState == GiGestureState.kGiGestureBegan) {
            mCoreView.onGesture(mAdapter, GiGestureType.kLongPress,
                    GiGestureState.kGiGestureBegan, 1, e.getX(), e.getY(), 0,
                    0);
        }
    }

    @Override
    public boolean onDown(MotionEvent e) {
        mGestureState = GiGestureState.kGiGestureBegan;
        return true;
    }

    @Override
    public boolean onDoubleTap(MotionEvent e) {
        mCoreView.onGesture(mAdapter, GiGestureType.kDoubleTap,
                GiGestureState.kGiGestureBegan, 1, e.getX(), e.getY(), 0, 0);
        return true;
    }

    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float dx,
            float dy) {
        if (e2.getAction() == MotionEvent.ACTION_CANCEL) {
            mGestureState = GiGestureState.kGiGestureCancel;
            mCoreView.onGesture(mAdapter, mGestureType, mGestureState, 0, 0, 0, 0, 0);
        }
        return true;
    }
}
