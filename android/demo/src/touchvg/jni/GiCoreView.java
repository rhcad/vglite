/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.10
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package touchvg.jni;

public class GiCoreView {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected GiCoreView(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(GiCoreView obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        touchvgJNI.delete_GiCoreView(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public GiCoreView(GiCoreView mainView) {
    this(touchvgJNI.new_GiCoreView(GiCoreView.getCPtr(mainView), mainView), true);
  }

  public void createView(GiView view, int type) {
    touchvgJNI.GiCoreView_createView(swigCPtr, this, GiView.getCPtr(view), view, type);
  }

  public void destoryView(GiView view) {
    touchvgJNI.GiCoreView_destoryView(swigCPtr, this, GiView.getCPtr(view), view);
  }

  public void drawAll(GiView view, GiCanvas canvas) {
    touchvgJNI.GiCoreView_drawAll(swigCPtr, this, GiView.getCPtr(view), view, GiCanvas.getCPtr(canvas), canvas);
  }

  public boolean drawAppend(GiView view, GiCanvas canvas) {
    return touchvgJNI.GiCoreView_drawAppend(swigCPtr, this, GiView.getCPtr(view), view, GiCanvas.getCPtr(canvas), canvas);
  }

  public void dynDraw(GiView view, GiCanvas canvas) {
    touchvgJNI.GiCoreView_dynDraw(swigCPtr, this, GiView.getCPtr(view), view, GiCanvas.getCPtr(canvas), canvas);
  }

  public static void setScreenDpi(int dpi) {
    touchvgJNI.GiCoreView_setScreenDpi(dpi);
  }

  public void onSize(GiView view, int w, int h) {
    touchvgJNI.GiCoreView_onSize(swigCPtr, this, GiView.getCPtr(view), view, w, h);
  }

  public boolean onGesture(GiView view, GiGestureType gestureType, GiGestureState gestureState, float x, float y) {
    return touchvgJNI.GiCoreView_onGesture(swigCPtr, this, GiView.getCPtr(view), view, gestureType.swigValue(), gestureState.swigValue(), x, y);
  }

  public boolean twoFingersMove(GiView view, GiGestureState gestureState, float x1, float y1, float x2, float y2) {
    return touchvgJNI.GiCoreView_twoFingersMove(swigCPtr, this, GiView.getCPtr(view), view, gestureState.swigValue(), x1, y1, x2, y2);
  }

}
