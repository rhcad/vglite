/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.11
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package touchvg.jni;

public class TestCanvas {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected TestCanvas(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(TestCanvas obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        touchvgJNI.delete_TestCanvas(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public static void initRand() {
    touchvgJNI.TestCanvas_initRand();
  }

  public static int randInt(int minv, int maxv) {
    return touchvgJNI.TestCanvas_randInt(minv, maxv);
  }

  public static float randFloat(float minv, float maxv) {
    return touchvgJNI.TestCanvas_randFloat(minv, maxv);
  }

  public static void test(GiCanvas canvas, int bits, int n, boolean randStyle) {
    touchvgJNI.TestCanvas_test__SWIG_0(GiCanvas.getCPtr(canvas), canvas, bits, n, randStyle);
  }

  public static void test(GiCanvas canvas, int bits, int n) {
    touchvgJNI.TestCanvas_test__SWIG_1(GiCanvas.getCPtr(canvas), canvas, bits, n);
  }

  public static void test(GiCanvas canvas, int bits) {
    touchvgJNI.TestCanvas_test__SWIG_2(GiCanvas.getCPtr(canvas), canvas, bits);
  }

  public static void testRect(GiCanvas canvas, int n) {
    touchvgJNI.TestCanvas_testRect(GiCanvas.getCPtr(canvas), canvas, n);
  }

  public static void testLine(GiCanvas canvas, int n) {
    touchvgJNI.TestCanvas_testLine(GiCanvas.getCPtr(canvas), canvas, n);
  }

  public static void testEllipse(GiCanvas canvas, int n) {
    touchvgJNI.TestCanvas_testEllipse(GiCanvas.getCPtr(canvas), canvas, n);
  }

  public static void testQuadBezier(GiCanvas canvas, int n) {
    touchvgJNI.TestCanvas_testQuadBezier(GiCanvas.getCPtr(canvas), canvas, n);
  }

  public static void testCubicBezier(GiCanvas canvas, int n) {
    touchvgJNI.TestCanvas_testCubicBezier(GiCanvas.getCPtr(canvas), canvas, n);
  }

  public static void testPolygon(GiCanvas canvas, int n) {
    touchvgJNI.TestCanvas_testPolygon(GiCanvas.getCPtr(canvas), canvas, n);
  }

  public static void testClipPath(GiCanvas canvas, int n) {
    touchvgJNI.TestCanvas_testClipPath(GiCanvas.getCPtr(canvas), canvas, n);
  }

  public static void testHandle(GiCanvas canvas, int n) {
    touchvgJNI.TestCanvas_testHandle(GiCanvas.getCPtr(canvas), canvas, n);
  }

  public static void testDynCurves(GiCanvas canvas) {
    touchvgJNI.TestCanvas_testDynCurves(GiCanvas.getCPtr(canvas), canvas);
  }

  public static void testTextAt(GiCanvas canvas, int n) {
    touchvgJNI.TestCanvas_testTextAt(GiCanvas.getCPtr(canvas), canvas, n);
  }

  public TestCanvas() {
    this(touchvgJNI.new_TestCanvas(), true);
  }

}
