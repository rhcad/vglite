#!/bin/sh
# Type 'sh build.sh' to make TouchVG's library and bundle.

rm -fdr build
mkdir build
mkdir build/include

iphoneos61=`xcodebuild -showsdks | grep -i iphoneos6.1`
iphoneos51=`xcodebuild -showsdks | grep -i iphoneos5.1`
iphoneos43=`xcodebuild -showsdks | grep -i iphoneos4.3`

if [ -n "$iphoneos61" ]; then
    xcodebuild -project lib/TouchVG.xcodeproj -sdk iphoneos6.1 -configuration Release
else
if [ -n "$iphoneos51" ]; then
    xcodebuild -project lib/TouchVG.xcodeproj -sdk iphoneos5.1 -configuration Release
else
if [ -n "$iphoneos43" ]; then
    xcodebuild -project lib/TouchVG.xcodeproj -sdk iphoneos4.3 -configuration Release
fi
fi
fi

cp -R lib/build/Release-universal/libTouchVG.a build
cp -R lib/build/Release-universal/TouchVG.bundle build
cp -R lib/build/Release-universal/usr/local/include/*.h build/include