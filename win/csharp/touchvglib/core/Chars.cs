/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.11
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

namespace touchvg.core {

using System;
using System.Runtime.InteropServices;

public class Chars : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal Chars(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(Chars obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~Chars() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          touchvgcsPINVOKE.delete_Chars(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
    }
  }

  public Chars(int n) : this(touchvgcsPINVOKE.new_Chars(n), true) {
  }

  public int count() {
    int ret = touchvgcsPINVOKE.Chars_count(swigCPtr);
    return ret;
  }

  public char get(int index) {
    char ret = touchvgcsPINVOKE.Chars_get(swigCPtr, index);
    return ret;
  }

  public void set(int index, char value) {
    touchvgcsPINVOKE.Chars_set__SWIG_0(swigCPtr, index, value);
  }

  public void set(int index, char v1, char v2) {
    touchvgcsPINVOKE.Chars_set__SWIG_1(swigCPtr, index, v1, v2);
  }

}

}
