/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.11
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package touchvg.jni;

public class Ints {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected Ints(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(Ints obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        touchvgJNI.delete_Ints(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public Ints(int n) {
    this(touchvgJNI.new_Ints(n), true);
  }

  public int count() {
    return touchvgJNI.Ints_count(swigCPtr, this);
  }

  public int get(int index) {
    return touchvgJNI.Ints_get(swigCPtr, this, index);
  }

  public void set(int index, int value) {
    touchvgJNI.Ints_set__SWIG_0(swigCPtr, this, index, value);
  }

  public void set(int index, int v1, int v2) {
    touchvgJNI.Ints_set__SWIG_1(swigCPtr, this, index, v1, v2);
  }

}
