#!/bin/bash

echo "copy openncc.py"
if [ ! -f "openncc.py" ];then
	cp ../../../Source/Library/python3/openncc.py .
else
	echo "openncc.py is exist"
fi

echo "copy _openncc.so"
if [ ! -f "_openncc.so" ];then
	cp ../../../Source/Library/python3/_openncc.so .
else
	echo "_openncc.so is exist"
fi

echo "copy moviUsbBoot...."
if [ ! -f "moviUsbBoot" ];then
	cp ../../../Source/Firmware/moviUsbBoot .	
else
	echo "moviUsbBoot is exist"
fi

echo "copy blob ......"
if [ ! -d "blob/" ];then
	cp -r ../../../Source/Model/ ./blob
else
	echo "blob/ is exist"
fi

echo "copy fw ...... "
if [ ! -d "fw/" ];then
	cp -r ../../../Source/Firmware/fw/ .
else
	echo "bin/fw/ is exist"
	echo "copy flicRefApp.mvcmd"
	if [ ! -f "fw/flicRefApp.mvcmd" ];then
		cp ../../../Source/Firmware/fw/flicRefApp.mvcmd fw/
	else
		echo "flicRefApp.mvcmd is exist"
	fi
	echo "copy usb-ma2x8x.mvcmd"
	if [ ! -f "fw/usb-ma2x8x.mvcmd" ];then
		cp ../../../Source/Firmware/fw/usb-ma2x8x.mvcmd fw/
	else
		echo "usb-ma2x8x.mvcmd is exist"
	fi
fi
