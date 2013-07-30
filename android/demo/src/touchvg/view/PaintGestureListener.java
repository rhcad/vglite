package touchvg.view;

import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.GestureDetector.SimpleOnGestureListener;
import touchvg.jni.GiCoreView;
import touchvg.jni.GiGestureState;
import touchvg.jni.GiGestureType;
import touchvg.jni.GiView;

public class PaintGestureListener extends SimpleOnGestureListener {
	private final String TAG = "Gesture";
    private GiCoreView mCoreView;                   // 内核视图分发器
    private GiView mAdapter;                        // 视图回调适配器
    private long mEndPaintTime = 0;                 // 重绘结束时刻，用于忽略多余触摸移动消息
    private static final int STOPPED = 0;
    private static final int READY_MOVE = 1;
    private static final int MOVING = 2;
    private int mMoving = STOPPED;
    private int mFingerCount;

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
                mGestureState = GiGestureState.kGiGestureCancel;
                mCoreView.onGesture(mAdapter, mGestureType, GiGestureState.kGiGestureCancel, 0, 0);
            }
        	mMoving = STOPPED;
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
        	return mCoreView.onGesture(mAdapter, type, state, x1, y1);
        }
    }

    public boolean onTouch(View v, MotionEvent e) {
        final int action = e.getAction();
        final float x1 = e.getPointerCount() > 0 ? e.getX(0) : 0;
        final float y1 = e.getPointerCount() > 0 ? e.getY(0) : 0;
        final float x2 = e.getPointerCount() > 1 ? e.getX(1) : x1;
        final float y2 = e.getPointerCount() > 1 ? e.getY(1) : y1;
        Log.d(TAG, "onTouch action=" + actionName(e) + " x=" + x1 + " y=" + y1);

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
        		mMoving = MOVING;
        		mFingerCount = e.getPointerCount();
        		onMove(GiGestureState.kGiGestureBegan, mFingerCount, x1, y1, x2, y2);
        	}
        	if (mMoving != MOVING || e.getEventTime() < mEndPaintTime) {
                break;
            }

            if (e.getPointerCount() != mFingerCount) {
            	onMove(GiGestureState.kGiGestureEnded, mFingerCount, x1, y1, x2, y2);
                mFingerCount = e.getPointerCount();
                onMove(GiGestureState.kGiGestureBegan, mFingerCount, x1, y1, x2, y2);
            } else {
            	onMove(GiGestureState.kGiGestureBegan, mFingerCount, x1, y1, x2, y2);
            }
            break;

        case MotionEvent.ACTION_UP:
            if (mMoving == MOVING) {
            	onMove(GiGestureState.kGiGestureEnded, mFingerCount, x1, y1, x2, y2);
            }
            mMoving = STOPPED;
            mFingerCount = 0;
            break;
        }

        return false;
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent e) {
    	Log.d(TAG, "onSingleTapConfirmed action=" + actionName(e));
        mCoreView.onGesture(mAdapter, GiGestureType.kGiGestureTap,
                GiGestureState.kGiGestureBegan, e.getX(), e.getY());
        return true;
    }
    
    @Override
    public boolean onDoubleTap(MotionEvent e) {
    	Log.d(TAG, "onDoubleTap action=" + actionName(e));
        mCoreView.onGesture(mAdapter, GiGestureType.kGiGestureDblTap,
                GiGestureState.kGiGestureBegan, e.getX(), e.getY());
        return true;
    }

    @Override
    public void onLongPress(MotionEvent e) {
    	Log.d(TAG, "onLongPress action=" + actionName(e));
    	mCoreView.onGesture(mAdapter, GiGestureType.kGiGesturePress,
                GiGestureState.kGiGestureBegan, e.getX(), e.getY());
    }

    @Override
    public boolean onDown(MotionEvent e) {
    	Log.d(TAG, "onDown action=" + actionName(e));
    	mMoving = STOPPED;
        return true;
    }

    @Override
    public boolean onScroll(MotionEvent downEv, MotionEvent ev, float dx, float dy) {
    	Log.d(TAG, "onScroll action=" + actionName(ev) + " dx=" + dx + " dy=" + dy);
    	if (mMoving == STOPPED) {
    		mMoving = READY_MOVE;
    	}
        return true;
    }
    
    private String actionName(MotionEvent e) {
    	switch (e.getAction()) {
    	case MotionEvent.ACTION_CANCEL:
    		return "CANCEL";
    	case MotionEvent.ACTION_DOWN:
    		return "DOWN";
    	case MotionEvent.ACTION_UP:
    		return "UP";
    	case MotionEvent.ACTION_MOVE:
    		return "MOVE";
    	case MotionEvent.ACTION_OUTSIDE:
    		return "OUTSIDE";
    	}
    	return "" + e.getAction();
    }
}
