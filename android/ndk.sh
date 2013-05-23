#!/bin/sh
# Type 'sh ndk.sh' to make libtouchvg.so for Android.
#
if [ -f demo/jni/touchvg_java_wrap.cpp ]; then
    cd demo/jni
    ndk-build
    cd ../..
    if [ -f output/android.jar ]; then
        cd mk
        sh toapp.sh
    fi
else
    cd mk
    sh swig.sh
    cd ..
fi