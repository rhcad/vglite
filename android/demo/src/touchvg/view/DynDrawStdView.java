//! \file DynDrawStdView.java
//! \brief 基于View的动态绘图视图类
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

package touchvg.view;

import touchvg.jni.GiCoreView;
import android.content.Context;
import android.graphics.Canvas;
import android.os.SystemClock;
import android.view.View;

//! 基于View的动态绘图视图类
/*! \ingroup GROUP_ANDROID
 */
public class DynDrawStdView extends View implements DynDrawView {
    private CanvasAdapter mCanvasAdapter;
    private GiCoreView mCoreView;
    private long mDrawnTime;
    private long mEndPaintTime;

    public DynDrawStdView(Context context) {
        super(context);
        mCanvasAdapter = new CanvasAdapter(this);
    }
    
    public void setCoreView(GiCoreView coreView) {
        mCoreView = coreView;
    }
    
    public long getDrawnTime() {
        return mDrawnTime;
    }
    
    public long getEndPaintTime() {
        return mEndPaintTime;
    }
    
    public boolean isDrawing() {
        return mCanvasAdapter != null && mCanvasAdapter.isDrawing();
    }
    
    public void doDraw() {
        this.invalidate();
    }

    @Override
    protected void onDraw(Canvas canvas) {
        long ms = SystemClock.currentThreadTimeMillis();
        if (mCanvasAdapter.beginPaint(canvas)) {
            mCoreView.dynDraw(mCanvasAdapter);
            mCanvasAdapter.endPaint();
        }
        mDrawnTime = SystemClock.currentThreadTimeMillis() - ms;
        mEndPaintTime = android.os.SystemClock.uptimeMillis();
    }

    @Override
    protected void onDetachedFromWindow() {
        if (mCanvasAdapter != null) {
            mCanvasAdapter.delete();
            mCanvasAdapter = null;
        }
        mCoreView = null;
        super.onDetachedFromWindow();
    }
}
