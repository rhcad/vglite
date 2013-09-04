#!/bin/sh
# Type 'sh swig.sh' to make src/jni/*.java and libtouchvg.so for Android.
#

cd ../demo/jni

mkdir ../src/touchvg/jni
rm -rf ../src/touchvg/jni/*.*

swig -c++ -java -package touchvg.jni \
    -outdir ../src/touchvg/jni \
    -o touchvg_java_wrap.cpp \
    -I../../../core/include/callback \
    -I../../../core/include/test \
    -I../../../core/include/view \
    -I../../../core/include/shape \
    -I../../../core/include \
    ../../../core/src/view/touchvg.swig

python replacejstr.py # fix code lines generated by swig
ndk-build

cd ../../mk
if [ -f ../output/android.jar ]; then
sh toapp.sh
fi