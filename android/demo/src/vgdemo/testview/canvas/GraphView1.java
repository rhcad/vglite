﻿//! \file GraphView1.java
//! \brief 基于普通View类的绘图测试视图类
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

package vgdemo.testview.canvas;

import touchvg.jni.TestCanvas;
import touchvg.view.CanvasAdapter;
import vgdemo.app.R;
import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.os.SystemClock;
import android.view.MotionEvent;
import android.view.View;

//! 基于普通View类的绘图测试视图类，在onDraw()中使用Canvas绘图
public class GraphView1 extends View {
    protected CanvasAdapter mCanvas;
    protected int mCreateFlags;
    private long mDrawnTime;
    private float lastX = 50;
    private float lastY = 50;
    private float mPhase = 0;

    public GraphView1(Context context) {
        super(context);
        mCanvas = new CanvasAdapter(this);
        mCreateFlags = ((Activity) context).getIntent().getExtras().getInt("flags");
        initCanvas();

        this.setOnTouchListener(new OnTouchListener() {
            public boolean onTouch(View v, MotionEvent event) {
                showTime(mDrawnTime);
                lastX = event.getX(0);
                lastY = event.getY(0);
                v.invalidate();
                return true;
            }
        });
    }
    
    private Bitmap getHandleBitmap(int id) {
        return BitmapFactory.decodeResource(getResources(), id);
    }

    private void initCanvas() {
        if (mCanvas.getHandleBitmap(0) == null) {
            mCanvas.setHandleBitmaps(new Bitmap[] { 
                    getHandleBitmap(R.drawable.vgdot1), 
                    getHandleBitmap(R.drawable.vgdot2), 
                    getHandleBitmap(R.drawable.vgdot1), 
                    getHandleBitmap(R.drawable.vgdot1), 
                    getHandleBitmap(R.drawable.ic_launcher) });
        }
    }

    @Override
    public void setBackgroundColor(int color) {
        mCanvas.setBackgroundColor(color); // 视图仍然是透明色
    }

    protected void showTime(long ms) {
        Activity activity = (Activity) this.getContext();
        String title = activity.getTitle().toString();
        int pos = title.indexOf(' ');
        if (pos >= 0) {
            title = title.substring(0, pos);
        }
        activity.setTitle(title + " - " + Long.toString(ms) + " ms");
    }

    @Override
    protected void onDraw(Canvas canvas) {
        long ms = SystemClock.currentThreadTimeMillis();
        if (mCanvas.beginPaint(canvas)) {
            canvas.drawColor(Color.TRANSPARENT);
            if ((mCreateFlags & 0xF0000) != 0) {  // in scroll view
                TestCanvas.test(mCanvas, mCreateFlags, 400);
            }
            else {
                TestCanvas.test(mCanvas, mCreateFlags);
            }
            dynDraw(mCanvas);
            mCanvas.endPaint();
        }
        mDrawnTime = SystemClock.currentThreadTimeMillis() - ms;
        showTime(mDrawnTime);
    }

    private void dynDraw(CanvasAdapter canvas) {
        mPhase += 1;
        canvas.setPen(0, 0, 1, mPhase);
        canvas.setBrush(0x22005500, 0);
        mCanvas.drawEllipse(lastX - 50, lastY - 50, 100, 100, true, true);
    }

    @Override
    protected void onDetachedFromWindow() {
        if (mCanvas != null) {
            mCanvas.delete();
            mCanvas = null;
        }
        super.onDetachedFromWindow();
    }
}
