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
    touchvgJNI.GiCoreView_createView__SWIG_0(swigCPtr, this, GiView.getCPtr(view), view, type);
  }

  public void createView(GiView view) {
    touchvgJNI.GiCoreView_createView__SWIG_1(swigCPtr, this, GiView.getCPtr(view), view);
  }

  public void createMagnifierView(GiView newview, GiView mainView) {
    touchvgJNI.GiCoreView_createMagnifierView(swigCPtr, this, GiView.getCPtr(newview), newview, GiView.getCPtr(mainView), mainView);
  }

  public void destoryView(GiView view) {
    touchvgJNI.GiCoreView_destoryView(swigCPtr, this, GiView.getCPtr(view), view);
  }

  public int drawAll(GiView view, GiCanvas canvas) {
    return touchvgJNI.GiCoreView_drawAll(swigCPtr, this, GiView.getCPtr(view), view, GiCanvas.getCPtr(canvas), canvas);
  }

  public boolean drawAppend(GiView view, GiCanvas canvas) {
    return touchvgJNI.GiCoreView_drawAppend(swigCPtr, this, GiView.getCPtr(view), view, GiCanvas.getCPtr(canvas), canvas);
  }

  public void dynDraw(GiView view, GiCanvas canvas) {
    touchvgJNI.GiCoreView_dynDraw(swigCPtr, this, GiView.getCPtr(view), view, GiCanvas.getCPtr(canvas), canvas);
  }

  public int setBkColor(GiView view, int argb) {
    return touchvgJNI.GiCoreView_setBkColor(swigCPtr, this, GiView.getCPtr(view), view, argb);
  }

  public static void setScreenDpi(int dpi) {
    touchvgJNI.GiCoreView_setScreenDpi(dpi);
  }

  public void onSize(GiView view, int w, int h) {
    touchvgJNI.GiCoreView_onSize(swigCPtr, this, GiView.getCPtr(view), view, w, h);
  }

  public boolean onGesture(GiView view, GiGestureType gestureType, GiGestureState gestureState, float x, float y, boolean switchGesture) {
    return touchvgJNI.GiCoreView_onGesture__SWIG_0(swigCPtr, this, GiView.getCPtr(view), view, gestureType.swigValue(), gestureState.swigValue(), x, y, switchGesture);
  }

  public boolean onGesture(GiView view, GiGestureType gestureType, GiGestureState gestureState, float x, float y) {
    return touchvgJNI.GiCoreView_onGesture__SWIG_1(swigCPtr, this, GiView.getCPtr(view), view, gestureType.swigValue(), gestureState.swigValue(), x, y);
  }

  public boolean twoFingersMove(GiView view, GiGestureState gestureState, float x1, float y1, float x2, float y2, boolean switchGesture) {
    return touchvgJNI.GiCoreView_twoFingersMove__SWIG_0(swigCPtr, this, GiView.getCPtr(view), view, gestureState.swigValue(), x1, y1, x2, y2, switchGesture);
  }

  public boolean twoFingersMove(GiView view, GiGestureState gestureState, float x1, float y1, float x2, float y2) {
    return touchvgJNI.GiCoreView_twoFingersMove__SWIG_1(swigCPtr, this, GiView.getCPtr(view), view, gestureState.swigValue(), x1, y1, x2, y2);
  }

  public String command() {
    return touchvgJNI.GiCoreView_command(swigCPtr, this);
  }

  public boolean setCommand(GiView view, String name) {
    return touchvgJNI.GiCoreView_setCommand(swigCPtr, this, GiView.getCPtr(view), view, name);
  }

  public void clearCachedData() {
    touchvgJNI.GiCoreView_clearCachedData(swigCPtr, this);
  }

  public int addShapesForTest() {
    return touchvgJNI.GiCoreView_addShapesForTest(swigCPtr, this);
  }

  public int getShapeCount() {
    return touchvgJNI.GiCoreView_getShapeCount(swigCPtr, this);
  }

  public boolean loadFromFile(String vgfile) {
    return touchvgJNI.GiCoreView_loadFromFile(swigCPtr, this, vgfile);
  }

  public boolean saveToFile(String vgfile, boolean pretty) {
    return touchvgJNI.GiCoreView_saveToFile__SWIG_0(swigCPtr, this, vgfile, pretty);
  }

  public boolean saveToFile(String vgfile) {
    return touchvgJNI.GiCoreView_saveToFile__SWIG_1(swigCPtr, this, vgfile);
  }

  public String getContent() {
    return touchvgJNI.GiCoreView_getContent(swigCPtr, this);
  }

  public void freeContent() {
    touchvgJNI.GiCoreView_freeContent(swigCPtr, this);
  }

  public boolean setContent(String content) {
    return touchvgJNI.GiCoreView_setContent(swigCPtr, this, content);
  }

  public boolean loadShapes(MgStorage s) {
    return touchvgJNI.GiCoreView_loadShapes(swigCPtr, this, MgStorage.getCPtr(s), s);
  }

  public boolean saveShapes(MgStorage s) {
    return touchvgJNI.GiCoreView_saveShapes(swigCPtr, this, MgStorage.getCPtr(s), s);
  }

  public boolean zoomToExtent() {
    return touchvgJNI.GiCoreView_zoomToExtent(swigCPtr, this);
  }

}
