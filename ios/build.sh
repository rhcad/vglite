#!/bin/sh
# Type 'sh build.sh' to make TouchVG's framework and bundle.

rm -fdr build
mkdir build
mkdir build/iphoneos
mkdir build/iphonesimulator

xcodebuild -project framework/TouchVG.xcodeproj -sdk iphoneos5.1 -configuration Release

cp -R framework/build/Release-iphoneos/TouchVG.framework build/iphoneos
cp -R framework/build/Release-iphoneos/TouchVG.bundle build/iphoneos
cp -R framework/build/Release-iphonesimulator/TouchVG.framework build/iphonesimulator
cp -R framework/build/Release-iphonesimulator/TouchVG.bundle build/iphonesimulator