#!/bin/sh
# Type 'sh build.sh' to make TouchVG's framework and bundle.

rm -fdr build
mkdir build
mkdir build/iphoneos
mkdir build/iphonesimulator

iphoneos61=`xcodebuild -showsdks | grep -i iphoneos6.1`
iphoneos51=`xcodebuild -showsdks | grep -i iphoneos5.1`
iphoneos43=`xcodebuild -showsdks | grep -i iphoneos4.3`

if [ -n "$iphoneos61" ]; then
    xcodebuild -project framework/TouchVG.xcodeproj -sdk iphoneos6.1 -configuration Release
else
if [ -n "$iphoneos51" ]; then
    xcodebuild -project framework/TouchVG.xcodeproj -sdk iphoneos5.1 -configuration Release
else
if [ -n "$iphoneos43" ]; then
    xcodebuild -project framework/TouchVG.xcodeproj -sdk iphoneos4.3 -configuration Release
fi
fi
fi

cp -R framework/build/Release-iphoneos/TouchVG.framework build/iphoneos
cp -R framework/build/Release-iphoneos/TouchVG.bundle build/iphoneos
cp -R framework/build/Release-iphonesimulator/TouchVG.framework build/iphonesimulator
cp -R framework/build/Release-iphonesimulator/TouchVG.bundle build/iphonesimulator