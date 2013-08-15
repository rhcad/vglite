//! \file DynDrawView.java
//! \brief 基于SurfaceView的动态绘图视图类
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

package touchvg.view;

import touchvg.jni.GiCoreView;
import touchvg.jni.GiView;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.graphics.PorterDuff.Mode;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

//! 基于SurfaceView的动态绘图视图类
/*! \ingroup GROUP_ANDROID
 */
public class DynDrawView extends SurfaceView {
    private CanvasAdapter mCanvasAdapter;
    private GiView mViewAdapter;
    private GiCoreView mCoreView;

    private DynDrawView(Context context) {
        super(context);
    }
    
    public DynDrawView(Context context, GiView viewAdapter, GiCoreView coreView) {
        super(context);
        mViewAdapter = viewAdapter;
        mCoreView = coreView;
        mCanvasAdapter = new CanvasAdapter(this);
        
        getHolder().addCallback(new SurfaceCallback());
        setZOrderOnTop(true);
        getHolder().setFormat(PixelFormat.TRANSLUCENT);
    }
    
    public boolean isDrawing() {
        return mCanvasAdapter != null && mCanvasAdapter.isDrawing();
    }

    @Override
    protected void onDraw(Canvas canvas) {
        if (mCanvasAdapter.beginPaint(canvas)) {
            canvas.drawColor(Color.TRANSPARENT, Mode.CLEAR);
            mCoreView.dynDraw(mViewAdapter, mCanvasAdapter);
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
        mViewAdapter = null;
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
        }
    }
}
