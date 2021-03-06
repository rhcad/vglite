/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.11
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package touchvg.jni;

public class touchvgJNI {
  public final static native void delete_Ints(long jarg1);
  public final static native long new_Ints(int jarg1);
  public final static native int Ints_count(long jarg1, Ints jarg1_);
  public final static native int Ints_get(long jarg1, Ints jarg1_, int jarg2);
  public final static native void Ints_set__SWIG_0(long jarg1, Ints jarg1_, int jarg2, int jarg3);
  public final static native void Ints_set__SWIG_1(long jarg1, Ints jarg1_, int jarg2, int jarg3, int jarg4);
  public final static native void delete_Floats(long jarg1);
  public final static native long new_Floats(int jarg1);
  public final static native int Floats_count(long jarg1, Floats jarg1_);
  public final static native float Floats_get(long jarg1, Floats jarg1_, int jarg2);
  public final static native void Floats_set__SWIG_0(long jarg1, Floats jarg1_, int jarg2, float jarg3);
  public final static native void Floats_set__SWIG_1(long jarg1, Floats jarg1_, int jarg2, float jarg3, float jarg4);
  public final static native void delete_Chars(long jarg1);
  public final static native long new_Chars(int jarg1);
  public final static native int Chars_count(long jarg1, Chars jarg1_);
  public final static native char Chars_get(long jarg1, Chars jarg1_, int jarg2);
  public final static native void Chars_set__SWIG_0(long jarg1, Chars jarg1_, int jarg2, char jarg3);
  public final static native void Chars_set__SWIG_1(long jarg1, Chars jarg1_, int jarg2, char jarg3, char jarg4);
  public final static native void delete_GiCanvas(long jarg1);
  public final static native void GiCanvas_setPen(long jarg1, GiCanvas jarg1_, int jarg2, float jarg3, int jarg4, float jarg5);
  public final static native void GiCanvas_setBrush(long jarg1, GiCanvas jarg1_, int jarg2, int jarg3);
  public final static native void GiCanvas_clearRect(long jarg1, GiCanvas jarg1_, float jarg2, float jarg3, float jarg4, float jarg5);
  public final static native void GiCanvas_drawRect(long jarg1, GiCanvas jarg1_, float jarg2, float jarg3, float jarg4, float jarg5, boolean jarg6, boolean jarg7);
  public final static native void GiCanvas_drawLine(long jarg1, GiCanvas jarg1_, float jarg2, float jarg3, float jarg4, float jarg5);
  public final static native void GiCanvas_drawEllipse(long jarg1, GiCanvas jarg1_, float jarg2, float jarg3, float jarg4, float jarg5, boolean jarg6, boolean jarg7);
  public final static native void GiCanvas_beginPath(long jarg1, GiCanvas jarg1_);
  public final static native void GiCanvas_moveTo(long jarg1, GiCanvas jarg1_, float jarg2, float jarg3);
  public final static native void GiCanvas_lineTo(long jarg1, GiCanvas jarg1_, float jarg2, float jarg3);
  public final static native void GiCanvas_bezierTo(long jarg1, GiCanvas jarg1_, float jarg2, float jarg3, float jarg4, float jarg5, float jarg6, float jarg7);
  public final static native void GiCanvas_quadTo(long jarg1, GiCanvas jarg1_, float jarg2, float jarg3, float jarg4, float jarg5);
  public final static native void GiCanvas_closePath(long jarg1, GiCanvas jarg1_);
  public final static native void GiCanvas_drawPath(long jarg1, GiCanvas jarg1_, boolean jarg2, boolean jarg3);
  public final static native void GiCanvas_saveClip(long jarg1, GiCanvas jarg1_);
  public final static native void GiCanvas_restoreClip(long jarg1, GiCanvas jarg1_);
  public final static native boolean GiCanvas_clipRect(long jarg1, GiCanvas jarg1_, float jarg2, float jarg3, float jarg4, float jarg5);
  public final static native boolean GiCanvas_clipPath(long jarg1, GiCanvas jarg1_);
  public final static native void GiCanvas_drawHandle(long jarg1, GiCanvas jarg1_, float jarg2, float jarg3, int jarg4);
  public final static native void GiCanvas_drawBitmap(long jarg1, GiCanvas jarg1_, String jarg2, float jarg3, float jarg4, float jarg5, float jarg6, float jarg7);
  public final static native float GiCanvas_drawTextAt(long jarg1, GiCanvas jarg1_, String jarg2, float jarg3, float jarg4, float jarg5, int jarg6);
  public final static native long new_GiCanvas();
  public final static native void GiCanvas_director_connect(GiCanvas obj, long cptr, boolean mem_own, boolean weak_global);
  public final static native void GiCanvas_change_ownership(GiCanvas obj, long cptr, boolean take_or_release);
  public final static native void delete_GiView(long jarg1);
  public final static native void GiView_regenAll(long jarg1, GiView jarg1_);
  public final static native void GiView_regenAppend(long jarg1, GiView jarg1_);
  public final static native void GiView_redraw(long jarg1, GiView jarg1_);
  public final static native boolean GiView_useFinger(long jarg1, GiView jarg1_);
  public final static native boolean GiView_useFingerSwigExplicitGiView(long jarg1, GiView jarg1_);
  public final static native boolean GiView_isContextActionsVisible(long jarg1, GiView jarg1_);
  public final static native boolean GiView_isContextActionsVisibleSwigExplicitGiView(long jarg1, GiView jarg1_);
  public final static native boolean GiView_showContextActions(long jarg1, GiView jarg1_, long jarg2, Ints jarg2_, long jarg3, Floats jarg3_, float jarg4, float jarg5, float jarg6, float jarg7);
  public final static native boolean GiView_showContextActionsSwigExplicitGiView(long jarg1, GiView jarg1_, long jarg2, Ints jarg2_, long jarg3, Floats jarg3_, float jarg4, float jarg5, float jarg6, float jarg7);
  public final static native void GiView_commandChanged(long jarg1, GiView jarg1_);
  public final static native void GiView_commandChangedSwigExplicitGiView(long jarg1, GiView jarg1_);
  public final static native void GiView_selectionChanged(long jarg1, GiView jarg1_);
  public final static native void GiView_selectionChangedSwigExplicitGiView(long jarg1, GiView jarg1_);
  public final static native void GiView_contentChanged(long jarg1, GiView jarg1_);
  public final static native void GiView_contentChangedSwigExplicitGiView(long jarg1, GiView jarg1_);
  public final static native long new_GiView();
  public final static native void GiView_director_connect(GiView obj, long cptr, boolean mem_own, boolean weak_global);
  public final static native void GiView_change_ownership(GiView obj, long cptr, boolean take_or_release);
  public final static native void GiColor_r_set(long jarg1, GiColor jarg1_, short jarg2);
  public final static native short GiColor_r_get(long jarg1, GiColor jarg1_);
  public final static native void GiColor_g_set(long jarg1, GiColor jarg1_, short jarg2);
  public final static native short GiColor_g_get(long jarg1, GiColor jarg1_);
  public final static native void GiColor_b_set(long jarg1, GiColor jarg1_, short jarg2);
  public final static native short GiColor_b_get(long jarg1, GiColor jarg1_);
  public final static native void GiColor_a_set(long jarg1, GiColor jarg1_, short jarg2);
  public final static native short GiColor_a_get(long jarg1, GiColor jarg1_);
  public final static native long new_GiColor__SWIG_0();
  public final static native long new_GiColor__SWIG_1(int jarg1, int jarg2, int jarg3, int jarg4);
  public final static native long new_GiColor__SWIG_2(int jarg1, int jarg2, int jarg3);
  public final static native long new_GiColor__SWIG_3(long jarg1, GiColor jarg1_);
  public final static native long new_GiColor__SWIG_4(int jarg1, boolean jarg2);
  public final static native long new_GiColor__SWIG_5(int jarg1);
  public final static native long GiColor_White();
  public final static native long GiColor_Black();
  public final static native long GiColor_Invalid();
  public final static native int GiColor_getARGB(long jarg1, GiColor jarg1_);
  public final static native void GiColor_setARGB(long jarg1, GiColor jarg1_, int jarg2);
  public final static native void GiColor_set__SWIG_0(long jarg1, GiColor jarg1_, int jarg2, int jarg3, int jarg4);
  public final static native void GiColor_set__SWIG_1(long jarg1, GiColor jarg1_, int jarg2, int jarg3, int jarg4, int jarg5);
  public final static native boolean GiColor_isInvalid(long jarg1, GiColor jarg1_);
  public final static native boolean GiColor_equals(long jarg1, GiColor jarg1_, long jarg2, GiColor jarg2_);
  public final static native void delete_GiColor(long jarg1);
  public final static native int kGiLineSolid_get();
  public final static native int kContextCopyNone_get();
  public final static native int kContextLineRGB_get();
  public final static native int kContextLineAlpha_get();
  public final static native int kContextLineARGB_get();
  public final static native int kContextLineWidth_get();
  public final static native int kContextLineStyle_get();
  public final static native int kContextFillRGB_get();
  public final static native int kContextFillAlpha_get();
  public final static native int kContextFillARGB_get();
  public final static native int kContextCopyAll_get();
  public final static native long new_GiContext__SWIG_0();
  public final static native long new_GiContext__SWIG_1(float jarg1, long jarg2, GiColor jarg2_, int jarg3, long jarg4, GiColor jarg4_, boolean jarg5);
  public final static native long new_GiContext__SWIG_2(float jarg1, long jarg2, GiColor jarg2_, int jarg3, long jarg4, GiColor jarg4_);
  public final static native long new_GiContext__SWIG_3(float jarg1, long jarg2, GiColor jarg2_, int jarg3);
  public final static native long new_GiContext__SWIG_4(float jarg1, long jarg2, GiColor jarg2_);
  public final static native long new_GiContext__SWIG_5(float jarg1);
  public final static native long new_GiContext__SWIG_6(long jarg1, GiContext jarg1_);
  public final static native long GiContext_copy__SWIG_0(long jarg1, GiContext jarg1_, long jarg2, GiContext jarg2_, int jarg3);
  public final static native long GiContext_copy__SWIG_1(long jarg1, GiContext jarg1_, long jarg2, GiContext jarg2_);
  public final static native boolean GiContext_equals(long jarg1, GiContext jarg1_, long jarg2, GiContext jarg2_);
  public final static native int GiContext_getLineStyle(long jarg1, GiContext jarg1_);
  public final static native void GiContext_setLineStyle(long jarg1, GiContext jarg1_, int jarg2);
  public final static native float GiContext_getLineWidth(long jarg1, GiContext jarg1_);
  public final static native boolean GiContext_isAutoScale(long jarg1, GiContext jarg1_);
  public final static native void GiContext_setLineWidth(long jarg1, GiContext jarg1_, float jarg2, boolean jarg3);
  public final static native boolean GiContext_isNullLine(long jarg1, GiContext jarg1_);
  public final static native void GiContext_setNullLine(long jarg1, GiContext jarg1_);
  public final static native long GiContext_getLineColor(long jarg1, GiContext jarg1_);
  public final static native void GiContext_setLineColor__SWIG_0(long jarg1, GiContext jarg1_, long jarg2, GiColor jarg2_);
  public final static native void GiContext_setLineColor__SWIG_1(long jarg1, GiContext jarg1_, int jarg2, int jarg3, int jarg4);
  public final static native void GiContext_setLineColor__SWIG_2(long jarg1, GiContext jarg1_, int jarg2, int jarg3, int jarg4, int jarg5);
  public final static native int GiContext_getLineARGB(long jarg1, GiContext jarg1_);
  public final static native void GiContext_setLineARGB(long jarg1, GiContext jarg1_, int jarg2);
  public final static native int GiContext_getLineAlpha(long jarg1, GiContext jarg1_);
  public final static native void GiContext_setLineAlpha(long jarg1, GiContext jarg1_, int jarg2);
  public final static native boolean GiContext_hasFillColor(long jarg1, GiContext jarg1_);
  public final static native void GiContext_setNoFillColor(long jarg1, GiContext jarg1_);
  public final static native long GiContext_getFillColor(long jarg1, GiContext jarg1_);
  public final static native void GiContext_setFillColor__SWIG_0(long jarg1, GiContext jarg1_, long jarg2, GiColor jarg2_);
  public final static native void GiContext_setFillColor__SWIG_1(long jarg1, GiContext jarg1_, int jarg2, int jarg3, int jarg4);
  public final static native void GiContext_setFillColor__SWIG_2(long jarg1, GiContext jarg1_, int jarg2, int jarg3, int jarg4, int jarg5);
  public final static native int GiContext_getFillARGB(long jarg1, GiContext jarg1_);
  public final static native void GiContext_setFillARGB(long jarg1, GiContext jarg1_, int jarg2);
  public final static native int GiContext_getFillAlpha(long jarg1, GiContext jarg1_);
  public final static native void GiContext_setFillAlpha(long jarg1, GiContext jarg1_, int jarg2);
  public final static native boolean GiContext_isAutoFillColor(long jarg1, GiContext jarg1_);
  public final static native void GiContext_setAutoFillColor(long jarg1, GiContext jarg1_, boolean jarg2);
  public final static native int GiContext_getType(long jarg1, GiContext jarg1_);
  public final static native void delete_GiContext(long jarg1);
  public final static native long new_GiCoreView__SWIG_0(long jarg1, GiCoreView jarg1_);
  public final static native long new_GiCoreView__SWIG_1();
  public final static native void delete_GiCoreView(long jarg1);
  public final static native void GiCoreView_createView__SWIG_0(long jarg1, GiCoreView jarg1_, long jarg2, GiView jarg2_, int jarg3);
  public final static native void GiCoreView_createView__SWIG_1(long jarg1, GiCoreView jarg1_, long jarg2, GiView jarg2_);
  public final static native void GiCoreView_createMagnifierView(long jarg1, GiCoreView jarg1_, long jarg2, GiView jarg2_, long jarg3, GiView jarg3_);
  public final static native void GiCoreView_destoryView(long jarg1, GiCoreView jarg1_, long jarg2, GiView jarg2_);
  public final static native int GiCoreView_drawAll(long jarg1, GiCoreView jarg1_, long jarg2, GiView jarg2_, long jarg3, GiCanvas jarg3_);
  public final static native boolean GiCoreView_drawAppend(long jarg1, GiCoreView jarg1_, long jarg2, GiView jarg2_, long jarg3, GiCanvas jarg3_);
  public final static native void GiCoreView_dynDraw(long jarg1, GiCoreView jarg1_, long jarg2, GiView jarg2_, long jarg3, GiCanvas jarg3_);
  public final static native int GiCoreView_setBkColor(long jarg1, GiCoreView jarg1_, long jarg2, GiView jarg2_, int jarg3);
  public final static native void GiCoreView_setScreenDpi(int jarg1);
  public final static native void GiCoreView_onSize(long jarg1, GiCoreView jarg1_, long jarg2, GiView jarg2_, int jarg3, int jarg4);
  public final static native boolean GiCoreView_onGesture__SWIG_0(long jarg1, GiCoreView jarg1_, long jarg2, GiView jarg2_, int jarg3, int jarg4, float jarg5, float jarg6, boolean jarg7);
  public final static native boolean GiCoreView_onGesture__SWIG_1(long jarg1, GiCoreView jarg1_, long jarg2, GiView jarg2_, int jarg3, int jarg4, float jarg5, float jarg6);
  public final static native boolean GiCoreView_twoFingersMove__SWIG_0(long jarg1, GiCoreView jarg1_, long jarg2, GiView jarg2_, int jarg3, float jarg4, float jarg5, float jarg6, float jarg7, boolean jarg8);
  public final static native boolean GiCoreView_twoFingersMove__SWIG_1(long jarg1, GiCoreView jarg1_, long jarg2, GiView jarg2_, int jarg3, float jarg4, float jarg5, float jarg6, float jarg7);
  public final static native boolean GiCoreView_isPressDragging(long jarg1, GiCoreView jarg1_);
  public final static native int GiCoreView_getGestureType(long jarg1, GiCoreView jarg1_);
  public final static native int GiCoreView_getGestureState(long jarg1, GiCoreView jarg1_);
  public final static native String GiCoreView_getCommand(long jarg1, GiCoreView jarg1_);
  public final static native boolean GiCoreView_setCommand(long jarg1, GiCoreView jarg1_, long jarg2, GiView jarg2_, String jarg3);
  public final static native boolean GiCoreView_doContextAction(long jarg1, GiCoreView jarg1_, int jarg2);
  public final static native void GiCoreView_clearCachedData(long jarg1, GiCoreView jarg1_);
  public final static native int GiCoreView_addShapesForTest(long jarg1, GiCoreView jarg1_);
  public final static native int GiCoreView_getShapeCount(long jarg1, GiCoreView jarg1_);
  public final static native int GiCoreView_getChangeCount(long jarg1, GiCoreView jarg1_);
  public final static native int GiCoreView_getSelectedShapeCount(long jarg1, GiCoreView jarg1_);
  public final static native int GiCoreView_getSelectedShapeType(long jarg1, GiCoreView jarg1_);
  public final static native void GiCoreView_clear(long jarg1, GiCoreView jarg1_);
  public final static native boolean GiCoreView_loadFromFile(long jarg1, GiCoreView jarg1_, String jarg2);
  public final static native boolean GiCoreView_saveToFile__SWIG_0(long jarg1, GiCoreView jarg1_, String jarg2, boolean jarg3);
  public final static native boolean GiCoreView_saveToFile__SWIG_1(long jarg1, GiCoreView jarg1_, String jarg2);
  public final static native String GiCoreView_getContent(long jarg1, GiCoreView jarg1_);
  public final static native void GiCoreView_freeContent(long jarg1, GiCoreView jarg1_);
  public final static native boolean GiCoreView_setContent(long jarg1, GiCoreView jarg1_, String jarg2);
  public final static native boolean GiCoreView_zoomToExtent(long jarg1, GiCoreView jarg1_);
  public final static native boolean GiCoreView_zoomToModel(long jarg1, GiCoreView jarg1_, float jarg2, float jarg3, float jarg4, float jarg5);
  public final static native float GiCoreView_calcPenWidth(long jarg1, GiCoreView jarg1_, float jarg2);
  public final static native long GiCoreView_getContext(long jarg1, GiCoreView jarg1_, boolean jarg2);
  public final static native void GiCoreView_setContext__SWIG_0(long jarg1, GiCoreView jarg1_, long jarg2, GiContext jarg2_, int jarg3, int jarg4);
  public final static native void GiCoreView_setContext__SWIG_1(long jarg1, GiCoreView jarg1_, int jarg2);
  public final static native void GiCoreView_setContextEditing(long jarg1, GiCoreView jarg1_, boolean jarg2);
  public final static native boolean GiCoreView_addImageShape(long jarg1, GiCoreView jarg1_, String jarg2, float jarg3, float jarg4);
  public final static native boolean GiCoreView_getBoundingBox(long jarg1, GiCoreView jarg1_, long jarg2, Floats jarg2_);
  public final static native void TestCanvas_initRand();
  public final static native int TestCanvas_randInt(int jarg1, int jarg2);
  public final static native float TestCanvas_randFloat(float jarg1, float jarg2);
  public final static native void TestCanvas_test__SWIG_0(long jarg1, GiCanvas jarg1_, int jarg2, int jarg3, boolean jarg4);
  public final static native void TestCanvas_test__SWIG_1(long jarg1, GiCanvas jarg1_, int jarg2, int jarg3);
  public final static native void TestCanvas_test__SWIG_2(long jarg1, GiCanvas jarg1_, int jarg2);
  public final static native void TestCanvas_testRect(long jarg1, GiCanvas jarg1_, int jarg2);
  public final static native void TestCanvas_testLine(long jarg1, GiCanvas jarg1_, int jarg2);
  public final static native void TestCanvas_testEllipse(long jarg1, GiCanvas jarg1_, int jarg2);
  public final static native void TestCanvas_testQuadBezier(long jarg1, GiCanvas jarg1_, int jarg2);
  public final static native void TestCanvas_testCubicBezier(long jarg1, GiCanvas jarg1_, int jarg2);
  public final static native void TestCanvas_testPolygon(long jarg1, GiCanvas jarg1_, int jarg2);
  public final static native void TestCanvas_testClipPath(long jarg1, GiCanvas jarg1_, int jarg2);
  public final static native void TestCanvas_testHandle(long jarg1, GiCanvas jarg1_, int jarg2);
  public final static native void TestCanvas_testDynCurves(long jarg1, GiCanvas jarg1_);
  public final static native void TestCanvas_testTextAt(long jarg1, GiCanvas jarg1_, int jarg2);
  public final static native long new_TestCanvas();
  public final static native void delete_TestCanvas(long jarg1);
  public final static native void EduCmds_registerCmds(long jarg1, GiCoreView jarg1_);
  public final static native int EduCmds_getDimensions(long jarg1, GiCoreView jarg1_, long jarg2, Floats jarg2_, long jarg3, Chars jarg3_);
  public final static native long new_EduCmds();
  public final static native void delete_EduCmds(long jarg1);

  public static void SwigDirector_GiCanvas_setPen(GiCanvas self, int argb, float width, int style, float phase) {
    self.setPen(argb, width, style, phase);
  }
  public static void SwigDirector_GiCanvas_setBrush(GiCanvas self, int argb, int style) {
    self.setBrush(argb, style);
  }
  public static void SwigDirector_GiCanvas_clearRect(GiCanvas self, float x, float y, float w, float h) {
    self.clearRect(x, y, w, h);
  }
  public static void SwigDirector_GiCanvas_drawRect(GiCanvas self, float x, float y, float w, float h, boolean stroke, boolean fill) {
    self.drawRect(x, y, w, h, stroke, fill);
  }
  public static void SwigDirector_GiCanvas_drawLine(GiCanvas self, float x1, float y1, float x2, float y2) {
    self.drawLine(x1, y1, x2, y2);
  }
  public static void SwigDirector_GiCanvas_drawEllipse(GiCanvas self, float x, float y, float w, float h, boolean stroke, boolean fill) {
    self.drawEllipse(x, y, w, h, stroke, fill);
  }
  public static void SwigDirector_GiCanvas_beginPath(GiCanvas self) {
    self.beginPath();
  }
  public static void SwigDirector_GiCanvas_moveTo(GiCanvas self, float x, float y) {
    self.moveTo(x, y);
  }
  public static void SwigDirector_GiCanvas_lineTo(GiCanvas self, float x, float y) {
    self.lineTo(x, y);
  }
  public static void SwigDirector_GiCanvas_bezierTo(GiCanvas self, float c1x, float c1y, float c2x, float c2y, float x, float y) {
    self.bezierTo(c1x, c1y, c2x, c2y, x, y);
  }
  public static void SwigDirector_GiCanvas_quadTo(GiCanvas self, float cpx, float cpy, float x, float y) {
    self.quadTo(cpx, cpy, x, y);
  }
  public static void SwigDirector_GiCanvas_closePath(GiCanvas self) {
    self.closePath();
  }
  public static void SwigDirector_GiCanvas_drawPath(GiCanvas self, boolean stroke, boolean fill) {
    self.drawPath(stroke, fill);
  }
  public static void SwigDirector_GiCanvas_saveClip(GiCanvas self) {
    self.saveClip();
  }
  public static void SwigDirector_GiCanvas_restoreClip(GiCanvas self) {
    self.restoreClip();
  }
  public static boolean SwigDirector_GiCanvas_clipRect(GiCanvas self, float x, float y, float w, float h) {
    return self.clipRect(x, y, w, h);
  }
  public static boolean SwigDirector_GiCanvas_clipPath(GiCanvas self) {
    return self.clipPath();
  }
  public static void SwigDirector_GiCanvas_drawHandle(GiCanvas self, float x, float y, int type) {
    self.drawHandle(x, y, type);
  }
  public static void SwigDirector_GiCanvas_drawBitmap(GiCanvas self, String name, float xc, float yc, float w, float h, float angle) {
    self.drawBitmap(name, xc, yc, w, h, angle);
  }
  public static float SwigDirector_GiCanvas_drawTextAt(GiCanvas self, String text, float x, float y, float h, int align) {
    return self.drawTextAt(text, x, y, h, align);
  }
  public static void SwigDirector_GiView_regenAll(GiView self) {
    self.regenAll();
  }
  public static void SwigDirector_GiView_regenAppend(GiView self) {
    self.regenAppend();
  }
  public static void SwigDirector_GiView_redraw(GiView self) {
    self.redraw();
  }
  public static boolean SwigDirector_GiView_useFinger(GiView self) {
    return self.useFinger();
  }
  public static boolean SwigDirector_GiView_isContextActionsVisible(GiView self) {
    return self.isContextActionsVisible();
  }
  public static boolean SwigDirector_GiView_showContextActions(GiView self, long actions, long buttonXY, float x, float y, float w, float h) {
    return self.showContextActions(new Ints(actions, false), new Floats(buttonXY, false), x, y, w, h);
  }
  public static void SwigDirector_GiView_commandChanged(GiView self) {
    self.commandChanged();
  }
  public static void SwigDirector_GiView_selectionChanged(GiView self) {
    self.selectionChanged();
  }
  public static void SwigDirector_GiView_contentChanged(GiView self) {
    self.contentChanged();
  }

  private final static native void swig_module_init();
  static {
    swig_module_init();
  }
}
