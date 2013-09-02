#!/bin/sh
# Type 'sh build.sh' to make libtouchvg.so and touchvg.jar for Android applications.
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