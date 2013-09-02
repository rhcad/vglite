#!/bin/sh
# Type 'sh build.sh' to make libtouchvg.so and touchvg.jar for Android applications.
# Need to copy $ANDROID_SDK_HOME/platforms/android-?/android.jar to ./output/ .
#
if [ -f demo/jni/touchvg_java_wrap.cpp ]; then
    cd demo/jni
    ndk-build
    cd ../..
    if [ -f output/android.jar ]; then
        cd mk
        sh toapp.sh # Make touchvg.jar
    fi
else
    cd mk
    sh swig.sh
    cd ..
fi