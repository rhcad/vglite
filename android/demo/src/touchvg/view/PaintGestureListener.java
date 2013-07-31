//! \file PaintGestureListener.java
//! \brief Android绘图手势识别类
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

package touchvg.view;

import java.util.ArrayList;
import touchvg.jni.GiCoreView;
import touchvg.jni.GiGestureState;
import touchvg.jni.GiGestureType;
import touchvg.jni.GiView;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.MotionEvent;
import android.view.View;

//! Android绘图手势识别类
/*! \ingroup GROUP_ANDROID
*/
public class PaintGestureListener extends SimpleOnGestureListener {
	private static final int STOPPED = 0;
    private static final int READY_MOVE = 1;
    private static final int MOVING = 2;
    private static final int ENDMOVE = 3;
    private GiCoreView mCoreView;                   // 内核视图分发器
    private GiView mAdapter;                        // 视图回调适配器
    private long mEndPaintTime = 0;                 // 重绘结束时刻，用于忽略多余触摸移动消息
    private int mMoving = STOPPED;                  // 移动状态
    private int mFingerCount;                       // 上一次的触摸点数
    private ArrayList<Float> mPoints = new ArrayList<Float>();    // 待分发的移动轨迹

    public PaintGestureListener(GiCoreView coreView, GiView adapter) {
        mCoreView = coreView;
        mAdapter = adapter;
    }

    public void release() {
        mCoreView = null;
        mAdapter = null;
    }

    public void setGestureEnable(boolean enable) {
        if (!enable) {
        	if (mMoving == MOVING) {
        		mMoving = STOPPED;
        		onMove(GiGestureState.kGiGestureCancel, mFingerCount, 0, 0, 0, 0);
            }
            mCoreView.setCommand(mAdapter, null);
        }
    }

    public void setEndPaintTime(long endTime) {
        mEndPaintTime = endTime;
    }
    
    private boolean onMove(GiGestureState state, int fingerCount, 
    		float x1, float y1, float x2, float y2) {
    	if (fingerCount > 1) {
            return mCoreView.twoFingersMove(mAdapter, state, x1, y1, x2, y2);
        } else {
        	return mCoreView.onGesture(mAdapter, GiGestureType.kGiGesturePan, state, x1, y1);
        }
    }
    
    @Override
    public boolean onDown(MotionEvent e) {
    	mMoving = STOPPED;
    	mPoints.clear();
    	if (e.getPointerCount() == 1) {
    		mPoints.add(Float.valueOf(e.getX(0)));
    		mPoints.add(Float.valueOf(e.getY(0)));
    	}
        return true;
    }
    
    @Override
    public boolean onScroll(MotionEvent downEv, MotionEvent e, float dx, float dy) {
    	if (mMoving == STOPPED) {
    		mMoving = READY_MOVE;
    	}
        return mMoving == READY_MOVE || mMoving == MOVING;
    }
    
    private boolean applyPendingPoints() {
    	boolean ret = onMove(GiGestureState.kGiGestureBegan, mFingerCount, 
				mPoints.get(0).floatValue(), 
				mPoints.get(1).floatValue(), 0, 0);
		for (int i = 2; i < mPoints.size() && ret; i += 2) {
			onMove(GiGestureState.kGiGestureMoved, mFingerCount, 
					mPoints.get(0).floatValue(), 
					mPoints.get(1).floatValue(), 0, 0);
		}
		return ret;
    }

    public boolean onTouch(View v, MotionEvent e) {
        final int action = e.getAction();
        final float x1 = e.getPointerCount() > 0 ? e.getX(0) : 0;
        final float y1 = e.getPointerCount() > 0 ? e.getY(0) : 0;
        final float x2 = e.getPointerCount() > 1 ? e.getX(1) : x1;
        final float y2 = e.getPointerCount() > 1 ? e.getY(1) : y1;

        // 按下后不允许父视图拦截触摸事件，松开后允许
        if (action == MotionEvent.ACTION_UP
                || action == MotionEvent.ACTION_CANCEL) {
            v.getParent().requestDisallowInterceptTouchEvent(false);
        }
        else {
            v.getParent().requestDisallowInterceptTouchEvent(true);
        }

        switch (action) {
        case MotionEvent.ACTION_MOVE:
        	if (mMoving == READY_MOVE) {
        		mFingerCount = e.getPointerCount();
        		
        		if (mFingerCount == 1 && !mPoints.isEmpty()) {
        			mMoving = applyPendingPoints() ? MOVING : ENDMOVE;
        		}
        		else {
        			mMoving = onMove(GiGestureState.kGiGestureBegan, mFingerCount, x1, y1, x2, y2) ? MOVING : ENDMOVE;
        		}
        		mPoints.clear();
        	}
        	else if (mMoving == STOPPED && e.getPointerCount() == 1 && !mPoints.isEmpty()) {
            	mPoints.add(Float.valueOf(x1));
            	mPoints.add(Float.valueOf(y1));
        	}
        	if (mMoving != MOVING || e.getEventTime() < mEndPaintTime) {
                break;
            }

            if ((e.getPointerCount() > 1) != (mFingerCount > 1)) {
            	onMove(GiGestureState.kGiGestureEnded, mFingerCount, x1, y1, x2, y2);
                mFingerCount = e.getPointerCount();
                onMove(GiGestureState.kGiGestureBegan, mFingerCount, x1, y1, x2, y2);
            } else {
            	onMove(GiGestureState.kGiGestureMoved, mFingerCount, x1, y1, x2, y2);
            }
            break;

        case MotionEvent.ACTION_UP:
        case MotionEvent.ACTION_CANCEL:
        	if (action == MotionEvent.ACTION_UP && !mPoints.isEmpty()) {
        		if (applyPendingPoints()) {
        			mMoving = MOVING;
        		}
        	}
            if (mMoving == MOVING) {
            	onMove(action == MotionEvent.ACTION_UP ? GiGestureState.kGiGestureEnded
            			: GiGestureState.kGiGestureCancel, mFingerCount, x1, y1, x2, y2);
            }
            mMoving = STOPPED;
            mFingerCount = 0;
            mPoints.clear();
            break;
        }

        return false;	// to call GestureDetector.onTouchEvent
    }
    
    @Override
    public void onLongPress(MotionEvent e) {
    	if (mCoreView.onGesture(mAdapter, GiGestureType.kGiGesturePress,
                GiGestureState.kGiGestureBegan, e.getX(), e.getY())) {
    		mPoints.clear();
    	}
    	else if (mMoving == STOPPED) {
    		mMoving = READY_MOVE;
    	}
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent e) {
        if (mCoreView.onGesture(mAdapter, GiGestureType.kGiGestureTap,
                GiGestureState.kGiGestureBegan, e.getX(), e.getY())) {
        	mPoints.clear();
        	return true;
        }
        return false;
    }
    
    @Override
    public boolean onDoubleTap(MotionEvent e) {
        if (mCoreView.onGesture(mAdapter, GiGestureType.kGiGestureDblTap,
                GiGestureState.kGiGestureBegan, e.getX(), e.getY())) {
        	mPoints.clear();
        	return true;
        }
        return false;
    }
}
