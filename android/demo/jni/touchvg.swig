// Copyright (c) 2013, Zhang Yungui, https://github.com/rhcad/touchvg

#ifdef SWIGJAVA
%insert("runtime") %{
#ifdef __ANDROID__
#define SWIG_JAVA_NO_DETACH_CURRENT_THREAD
#define SWIG_JAVA_ATTACH_CURRENT_THREAD_AS_DAEMON
#include <android/log.h>
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,"vgjni",__VA_ARGS__)
#endif
%}
#endif

%module(directors="1") touchvg

%{
#include "gicanvas.h"
#include "giview.h"
#include "gicoreview.h"
#include "testcanvas.h"
#include "mgstorage.h"
%}

%feature("director") GiCanvas;
%include "gicanvas.h"

%feature("director") GiView;
%include "giview.h"

%include "gigesture.h"
%include "gicoreview.h"
%include "testcanvas.h"
%include "mgstorage.h"

#ifdef SWIGJAVA
%{
struct TmpJOBJ {
    JNIEnv* env;
    jobject* jo;
    template <class T>
    TmpJOBJ(JNIEnv* e, T* v) : env(e), jo((jobject*)v) { }
    ~TmpJOBJ() { if (*jo) { env->DeleteLocalRef(*jo); *jo = 0; } }
};
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* /*ajvm*/, void* /*reserved*/) {
    return JNI_VERSION_1_6;
}
%}
#endif