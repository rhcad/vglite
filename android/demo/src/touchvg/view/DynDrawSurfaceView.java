//! \file DynDrawSurfaceView.java
//! \brief Android绘图视图类
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

package touchvg.view;

import touchvg.jni.GiCoreView;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.graphics.PorterDuff.Mode;
import android.os.SystemClock;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

//! Android绘图视图类
/*! \ingroup GROUP_ANDROID
 */
public class DynDrawSurfaceView extends SurfaceView {
    private CanvasAdapter mCanvasAdapter;
    private GiCoreView mCoreView;
    private long mDrawnTime;
    private long mEndPaintTime;

    public DynDrawSurfaceView(Context context) {
        super(context);
        mCanvasAdapter = new CanvasAdapter(this);
        
        getHolder().addCallback(new SurfaceCallback());
        setZOrderOnTop(true);
        getHolder().setFormat(PixelFormat.TRANSLUCENT);
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

    @Override
    protected void onDraw(Canvas canvas) {
        if (mCanvasAdapter.beginPaint(canvas)) {
        	canvas.drawColor(Color.TRANSPARENT, Mode.CLEAR);
        	mCoreView.dyndraw(mCanvasAdapter);
            mCanvasAdapter.endPaint();
        }
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
    
    public void doDraw() {
    	if (!mCanvasAdapter.isDrawing()) {
            new Thread(new DrawThread()).start();
        }
    }
    
    private class SurfaceCallback implements SurfaceHolder.Callback {
        public void surfaceCreated(SurfaceHolder holder) {
        	doDraw();
        }

        public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
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
                    draw(canvas);
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
}
