#!/bin/sh
# Type 'sh ndk.sh' to make libtouchvg.so for Android.
#
if [ -f android/demo/jni/touchvg_java_wrap.cpp ]; then
    cd android/demo/jni
    ndk-build
    cd ../../..
else
    cd android/mk
    sh swig.sh
    cd ../..
fi
if [ -f android/output/android.jar ]; then
    cd android/mk
    sh toapp.sh
fi