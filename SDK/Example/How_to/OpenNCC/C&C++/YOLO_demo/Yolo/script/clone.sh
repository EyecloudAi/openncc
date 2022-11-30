#!/bin/bash

SDK_BASE_PATH=../../../../..

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
		cp $SDK_BASE_PATH/Source/Library/C\&C++/libOpenNCC.a lib/
	else
		echo "libOpenNCC.a is exist"
	fi
else
	echo "lib/ is exist"
	if [ ! -f "lib/libOpenNCC.a" ];then
		cp $SDK_BASE_PATH/Source/Library/C\&C++/libOpenNCC.a lib/
	else
		echo "libOpenNCC.a is exist"
	fi
fi

echo "mkdir bin...."
if [ ! -d "bin/" ];then
	mkdir bin/
	echo "copy blob...."
	cp -r $SDK_BASE_PATH/Source/Model/ bin/blob
	echo "copy moviUsbBoot...."
	cp $SDK_BASE_PATH/Source/Library/C\&C++/moviUsbBoot bin/
	chmod +x bin/moviUsbBoot
	echo "copy fw....."
	cp -r $SDK_BASE_PATH/Source/Firmware/fw/ bin/
else
	echo "bin/ is exist"
	echo "copy blob ......"
	if [ ! -d "bin/blob/" ];then
		cp -r $SDK_BASE_PATH/Source/Model/ bin/blob
	else
		echo "bin/blob/ is exist"
	fi

	echo "copy moviUsbBoot...."
	if [ ! -f "bin/moviUsbBoot" ];then
		cp $SDK_BASE_PATH/Source/Library/C\&C++/moviUsbBoot bin/
		chmod +x bin/moviUsbBoot	
	else
		echo "moviUsbBoot is exist"
	fi

	echo "copy fw ...... "
	if [ ! -d "bin/fw/" ];then
		cp -r $SDK_BASE_PATH/Source/Firmware/fw/ bin/
	else
		echo "bin/fw/ is exist"
		echo "copy OpenNcc.mvcmd"
		if [ ! -f "bin/fw/OpenNcc.mvcmd" ];then
			cp $SDK_BASE_PATH/Source/Firmware/fw/OpenNcc.mvcmd bin/fw/
		else
			echo "OpenNcc.mvcmd is exist"
		fi
	fi
fi
