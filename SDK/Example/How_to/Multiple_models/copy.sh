#!/bin/bash


echo -n "Please make sure you have backed up lib or bin(y or n)"
read Key
if [ $Key = "y" ];then
	rm -rf lib/ bin/
elif [ $Key = "n" ];then
	echo "n"
else
	echo "other"
fi

echo "copy lib/libOpenNCC.a ...."
if [ ! -d "lib/" ];then
	mkdir lib/
	if [ ! -f "lib/libOpenNCC.a" ];then
		cp ../../../Source/Library/C\&C++/libOpenNCC.a lib/
	else
		echo "libOpenNCC.a is exist"
	fi
else
	echo "lib/ is exist"
	if [ ! -f "lib/libOpenNCC.a" ];then
		cp ../../../Source/Library/C\&C++/libOpenNCC.a lib/
	else
		echo "libOpenNCC.a is exist"
	fi
fi

echo "mkdir bin...."
if [ ! -d "bin/" ];then
	mkdir bin/
	echo "copy blob...."
	cp -r ../../../Source/Model/ bin/blob
	echo "copy moviUsbBoot...."
	cp ../../../Source/Firmware/moviUsbBoot bin/
	echo "copy fw....."
	cp -r ../../../Source/Firmware/fw/ bin/
else
	echo "bin/ is exist"
	echo "copy blob ......"
	if [ ! -d "bin/blob/" ];then
		cp -r ../../../Source/Model/ bin/blob
	else
		echo "bin/blob/ is exist"
	fi

	echo "copy moviUsbBoot...."
	if [ ! -f "bin/moviUsbBoot" ];then
		cp ../../../Source/Firmware/moviUsbBoot bin/	
	else
		echo "moviUsbBoot is exist"
	fi

	echo "copy fw ...... "
	if [ ! -d "bin/fw/" ];then
		cp -r ../../../Source/Firmware/fw/ bin/
	else
		echo "bin/fw/ is exist"
		echo "copy flicRefApp.mvcmd"
		if [ ! -f "bin/fw/flicRefApp.mvcmd" ];then
			cp ../../../Source/Firmware/fw/flicRefApp.mvcmd bin/fw/
		else
			echo "flicRefApp.mvcmd is exist"
		fi
		echo "copy usb-ma2x8x.mvcmd"
		if [ ! -f "bin/fw/usb-ma2x8x.mvcmd" ];then
			cp ../../../Source/Firmware/fw/usb-ma2x8x.mvcmd bin/fw/
		else
			echo "usb-ma2x8x.mvcmd is exist"
		fi
	fi
fi
