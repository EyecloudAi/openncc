#!/bin/bash

echo -n "Please make sure you have backed up fw or bin(y or n)"
read Key
if [ $Key = "y" ];then
	rm -rf fw/ bin/ moviUsbBoot openncc.py _openncc.so
elif [ $Key = "n" ];then
	echo "n"
else
	echo "other"
fi

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
	cp ../../../Source/Library/C\&C++/moviUsbBoot .
	chmod +x moviUsbBoot	
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
	echo "copy OpenNcc.mvcmd"
	if [ ! -f "fw/OpenNcc.mvcmd" ];then
		cp ../../../Source/Firmware/fw/OpenNcc.mvcmd fw/
	else
		echo "OpenNcc.mvcmd is exist"
	fi
fi
