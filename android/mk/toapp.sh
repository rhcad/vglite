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

    if [ -n "$VGLITE_ANDROID_APP" ]; then
    	cp -v ../output/touchvg.jar $VGLITE_ANDROID_APP/libs;
    	cp -v ../output/armeabi/libtouchvg.so $VGLITE_ANDROID_APP/libs/armeabi;
    	else
    	echo "you have not the environment variable VGLITE_ANDROID_APP."
    fi
    python restore_utf8.py
else
    echo "Warning: Need to copy android.jar from ANDROID_SDK_HOME/platforms to android/output/ ."
    if [ -n "$VGLITE_ANDROID_APP" ]; then
    	cp -v ../output/armeabi/libtouchvg.so $VGLITE_ANDROID_APP/libs/armeabi
    fi
fi