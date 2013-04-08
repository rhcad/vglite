//! \file PaintView.java
//! \brief Android绘图视图类
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

package touchvg.view;

import touchvg.jni.GiCoreView;
import touchvg.jni.GiView;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.graphics.PorterDuff.Mode;
import android.os.SystemClock;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

//! Android绘图视图类
/*! \ingroup GROUP_ANDROID
 */
public class GraphSurfaceView extends SurfaceView {
    private CanvasAdapter mCanvasAdapter;
    private ViewAdapter mViewAdapter;
    private GiCoreView mCoreView;
    private DynDrawView mDynDrawView;
    private long mDrawnTime;
    private long mEndPaintTime;

    public GraphSurfaceView(Context context) {
        super(context);
        mCanvasAdapter = new CanvasAdapter(this);
        mViewAdapter = new ViewAdapter();
        mCoreView = new GiCoreView();
        
        getHolder().addCallback(new SurfaceCallback());
        setZOrderOnTop(true);
        getHolder().setFormat(PixelFormat.TRANSPARENT);
        
        this.setOnTouchListener(new OnTouchListener() {
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_UP) {
                    mViewAdapter.regenAfterAddShape();
                }
                else if ((mDynDrawView != null && !mDynDrawView.isDrawing()
                        && event.getEventTime() > mDynDrawView.getEndPaintTime())
                        || (mDynDrawView == null && !isDrawing()
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
    
    public boolean isDrawing() {
        return mCanvasAdapter != null && mCanvasAdapter.isDrawing();
    }

    private void drawShapes(Canvas canvas, Bitmap bitmap) {
        if (mCanvasAdapter.beginPaint(canvas)) {
        	if (bitmap != null) {
        		canvas.drawBitmap(bitmap, getMatrix(), null);
        		mCoreView.drawNewShape(mCanvasAdapter);
        	}
        	else {
        		if (getBackground() == null) {
        			canvas.drawColor(Color.TRANSPARENT, Mode.CLEAR);
        		}
        		mCoreView.drawAll(mCanvasAdapter);
    		}
            if (mDynDrawView == null) {
                mCoreView.dynDraw(mCanvasAdapter);
            }
            mCanvasAdapter.endPaint();
        }
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
    
    private void doDraw(boolean afterAddShape) {
    	new Thread(new DrawThread(afterAddShape)).start();
    }
    
    private class SurfaceCallback implements SurfaceHolder.Callback {
        public void surfaceCreated(SurfaceHolder holder) {
            doDraw(false);
        }

        public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        }

        public void surfaceDestroyed(SurfaceHolder holder) {
        }
    }
    
    private class DrawThread implements Runnable {
    	private boolean afterAddShape;
    	
    	DrawThread(boolean afterAddShape) {
    		this.afterAddShape = afterAddShape;
    	}
    	
        public void run() {
            long ms = SystemClock.currentThreadTimeMillis();
            Canvas canvas = null;
            Bitmap bitmap = null;
            
            try {
            	if (afterAddShape) {
            		bitmap = Bitmap.createBitmap(getWidth(), getHeight(), Bitmap.Config.ARGB_8888);
            		canvas = new Canvas(bitmap);
            		drawShapes(canvas, null);
            	}
                canvas = getHolder().lockCanvas();
                if (canvas != null) {
                	drawShapes(canvas, bitmap);
                }
            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                if (canvas != null) {
                    getHolder().unlockCanvasAndPost(canvas);
                }
                if (bitmap != null) {
                	bitmap.recycle();
                	bitmap = null;
                }
            }
            mDrawnTime = SystemClock.currentThreadTimeMillis() - ms;
            mEndPaintTime = android.os.SystemClock.uptimeMillis();
        }
    }
    
    private class ViewAdapter extends GiView {
        @Override
        public void regenAll() {
            doDraw(false);
            if (mDynDrawView != null) {
                mDynDrawView.doDraw();
            }
        }
        
        @Override
        public void regenAfterAddShape() {
        	doDraw(true);
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
                doDraw(false);
            }
        }
    }
}
