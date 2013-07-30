package touchvg.view;

import android.view.MotionEvent;
import android.view.View;
import android.view.GestureDetector.SimpleOnGestureListener;
import touchvg.jni.GiCoreView;
import touchvg.jni.GiGestureState;
import touchvg.jni.GiGestureType;
import touchvg.jni.GiView;

public class PaintGestureListener extends SimpleOnGestureListener {
    private GiCoreView mCoreView;                   // 内核视图分发器
    private GiView mAdapter;                        // 视图回调适配器
    private long mEndPaintTime = 0;                 // 重绘结束时刻，用于忽略多余触摸移动消息
    private GiGestureState mGestureState = GiGestureState.kGiGesturePossible;
    private GiGestureType mGestureType = GiGestureType.kGiGestureUnknown;
    private boolean mIsMoving = false;
    private int mFingerCount = 0;
    private float mLastX;
    private float mLastY;

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
        	if (mGestureState == GiGestureState.kGiGestureMoved) {
                mGestureState = GiGestureState.kGiGestureCancel;
                mCoreView.onGesture(mAdapter, mGestureType, mGestureState, 0, 0);
            }
            mCoreView.setCommand(mAdapter, null);
        }
    }

    public void setEndPaintTime(long endTime) {
        mEndPaintTime = endTime;
    }

    public boolean onTouch(View v, MotionEvent event) {
        final int action = event.getAction() & MotionEvent.ACTION_MASK;
        final float x1 = event.getPointerCount() > 0 ? event.getX(0) : 0;
        final float y1 = event.getPointerCount() > 0 ? event.getY(0) : 0;
        final float x2 = event.getPointerCount() > 1 ? event.getX(1) : x1;
        final float y2 = event.getPointerCount() > 1 ? event.getY(1) : y1;

        // 按下后不允许父视图拦截触摸事件，松开后允许
        if (action == MotionEvent.ACTION_UP
                || action == MotionEvent.ACTION_CANCEL) {
            v.getParent().requestDisallowInterceptTouchEvent(false);
        }
        else {
            v.getParent().requestDisallowInterceptTouchEvent(true);
        }

        switch (action) {
        case MotionEvent.ACTION_DOWN:
            mFingerCount = event.getPointerCount();
            break;  // call onGesture in onTouch() after later.

        case MotionEvent.ACTION_MOVE:
            if ((mGestureState == GiGestureState.kGiGestureBegan)
                    && !mIsMoving
                    && Math.hypot(mLastX - x1, mLastY - y1) > 10) {
                mIsMoving = true;
            }
            if (!mIsMoving || event.getEventTime() < mEndPaintTime) {
                break;
            }

            if (event.getPointerCount() == 2 && mFingerCount == 1) {
                mGestureState = GiGestureState.kGiGestureEnded;
                mCoreView.twoFingersMove(mAdapter, mGestureState, x1, y1, x2, y2);
                mGestureType = GiGestureType.kGiGestureUnknown;
                mFingerCount = event.getPointerCount();
                mGestureState = GiGestureState.kGiGestureBegan;
            }
            if (mGestureState == GiGestureState.kGiGestureBegan) {
                mGestureType = mFingerCount > 1 ? GiGestureType.kGiTwoFingersMove
                        : GiGestureType.kGiGesturePan;
                if (mFingerCount > 1) {
                    mCoreView.twoFingersMove(mAdapter, mGestureState, x1, y1, x2, y2);
                } else {
                    mCoreView.onGesture(mAdapter, mGestureType, mGestureState, x1, y1);
                }
            }
            mGestureState = GiGestureState.kGiGestureMoved;
            mIsMoving = true;
            if (event.getPointerCount() >= mFingerCount) {
                if (mFingerCount > 1) {
                    mCoreView.twoFingersMove(mAdapter, mGestureState, x1, y1, x2, y2);
                } else {
                    mCoreView.onGesture(mAdapter, mGestureType, mGestureState, x1, y1);
                }
            }
            break;

        case MotionEvent.ACTION_UP:
            if (mIsMoving) {
                mIsMoving = false;
                mGestureState = GiGestureState.kGiGestureEnded;
                if (mFingerCount > 1) {
                    mCoreView.twoFingersMove(mAdapter, mGestureState, x1, y1, x2, y2);
                } else {
                    mCoreView.onGesture(mAdapter, mGestureType, mGestureState, x1, y1);
                }
                mGestureType = GiGestureType.kGiGestureUnknown;
                mFingerCount = 0;
            }
            break;
        }

        mLastX = x1;
        mLastY = y1;

        return false;
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent e) {
        mCoreView.onGesture(mAdapter, GiGestureType.kGiGestureTap,
                GiGestureState.kGiGestureBegan, e.getX(), e.getY());
        return true;
    }

    @Override
    public void onLongPress(MotionEvent e) {
        if (mGestureState == GiGestureState.kGiGestureBegan) {
            mCoreView.onGesture(mAdapter, GiGestureType.kGiGesturePress,
                    GiGestureState.kGiGestureBegan, e.getX(), e.getY());
        }
    }

    @Override
    public boolean onDown(MotionEvent e) {
        mGestureState = GiGestureState.kGiGestureBegan;
        return true;
    }

    @Override
    public boolean onDoubleTap(MotionEvent e) {
        mCoreView.onGesture(mAdapter, GiGestureType.kGiGestureDblTap,
                GiGestureState.kGiGestureBegan, e.getX(), e.getY());
        return true;
    }

    @Override
    public boolean onScroll(MotionEvent downEv, MotionEvent ev, float dx, float dy) {
        if (ev.getAction() == MotionEvent.ACTION_CANCEL) {
            mGestureState = GiGestureState.kGiGestureCancel;
            mCoreView.onGesture(mAdapter, mGestureType, mGestureState, 0, 0);
        }
        return true;
    }
}
