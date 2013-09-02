#!/bin/sh
# Type 'sh toapp.sh' to make touchvg.jar for Android.
# Need to copy $ANDROID_SDK_HOME/platforms/android-?/android.jar to ../output/ .
#
if [ -f ../output/android.jar ]; then
    python utf8togbk.py # the javac application don't support UTF8 files.
    cd ../demo/src/touchvg/jni; javac *.java;
    
    cd ../..; jar -cf vgjni.jar touchvg/jni/*.class;
    cd touchvg/view; javac -cp ../../vgjni.jar:../../../../output/android.jar *.java;
    
    cd ../..; jar -cfv touchvg.jar touchvg/jni/*.class touchvg/view/*.class;
    cd ..; mv -v src/touchvg.jar ../output;
    rm -rf src/touchvg/jni/*.class;
    rm -rf src/touchvg/view/*.class;
    rm -rf src/*.jar;
    cp -v libs/armeabi/libtouchvg.so ../output/armeabi
    cd ../mk

    if [ -n "$TOUCHVG_ANDROID_APP" ]; then
    	cp -v ../output/touchvg.jar $TOUCHVG_ANDROID_APP/libs;
    	cp -v ../output/armeabi/libtouchvg.so $TOUCHVG_ANDROID_APP/libs/armeabi;
    fi
    python restore_utf8.py
fi