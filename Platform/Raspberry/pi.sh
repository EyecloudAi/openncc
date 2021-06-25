#!/bin/bash

project_path=$(cd `dirname $0`;pwd)
project_name="${project_path##*/}"
#echo $project_path
echo $project_name

echo "mkdir Source"
if [ ! -d "Source/" ];then
	mkdir Source
else
	echo "Source is exist"
fi

echo "copy Library ...."
if [ ! -d "Source/Library/" ];then
	cp -r ../../SDK/Source/Library/Raspberry/ ./Source/Library
else
	echo "Library is exist"
fi

echo "copy Model ..... "
if [ ! -d "Source/Model/" ];then
	cp -r ../../SDK/Source/Model/ ./Source
else
	echo "Model is exist"
fi

echo "copy Firmware ..... "
if [ ! -d "Source/Firmware/" ];then
	cp -r ../../SDK/Source/Firmware/ ./Source
else
	echo "Firmware is exist"
	if [ ! -f "Source/Firmware/flicRefAPP.mvcmd" ];then
		cp ../../SDK/Source/Firmware/flicRefApp.mvcmd Source/Firmware/
	else
		echo "flicRefAPP.mvcmd is exist,don't need copy"
	fi
	if [ ! -f "Source/Firmware/usb-ma2x8x.mvcmd" ];then
		cp ../../SDK/Source/Firmware/usb-ma2x8x.mvcmd Source/Firmware/
	else
		echo "usb-ma2x8x.mvcmd is exist,don't need copy"
	fi
fi

echo "copy Example ..... "
if [ ! -d "Example/" ];then
	cp -r ../../SDK/Example/ ./
else
	echo "Example is exist"
fi

echo "copy Viewer ...... "
if [ ! -d "Viewer/" ];then
	cp -r ../../SDK/Viewer/ ./
else
	echo "Viewer is exist"
fi

echo "copy Pi lib to Viewer"
cp Source/Library/C\&C++/libOpenNCC.a Viewer/QT_example/OpenNCC/ncc_sdk/libOpenNCC_rasp.a

