#!/bin/bash

SDK_BASE_PATH=../../..

project_path=$(cd `dirname $0`;pwd)

project_name="${project_path##*/}"

#echo $project_path

echo $project_name



echo -n "Please make sure you have backed up what you need?(y or n)"

read Key

if [ $Key = "y" ];then

	rm -rf Source/ Example/ Viewer/

elif [ $Key = "n" ];then

	echo "n"

else

	echo "other"

fi

echo "mkdir Source"

if [ ! -d "Source/" ];then

	mkdir Source

else

	echo "Source is exist"

fi



echo "copy Library ...."

if [ ! -d "Source/Library/" ];then

	cp -r $SDK_BASE_PATH/SDK/Source/Library/Linux/NVIDIA/ ./Source/Library

else

	echo "Library is exist"

fi



echo "copy Model ..... "

if [ ! -d "Source/Model/" ];then

	cp -r $SDK_BASE_PATH/SDK/Source/Model/ ./Source

else

	echo "Model is exist"

fi



echo "copy Firmware ..... "

if [ ! -d "Source/Firmware/" ];then

	cp -r $SDK_BASE_PATH/SDK/Source/Firmware/ ./Source

else

	echo "Firmware is exist"

	if [ ! -f "Source/Firmware/OpenNcc.mvcmd" ];then

		cp $SDK_BASE_PATH/SDK/Source/Firmware/OpenNcc.mvcmd Source/Firmware/

	else

		echo "OpenNcc.mvcmd is exist,don't need copy"

	fi

fi



echo "copy Example ..... "

if [ ! -d "Example/" ];then

	cp -r $SDK_BASE_PATH/SDK/Example/ ./

else

	echo "Example is exist"

fi

#rm -rf Example/How_to/Python_demo

#echo "copy Viewer ...... "

#if [ ! -d "Viewer/" ];then

#	cp -r ../../SDK/Viewer/ ./

#	chmod +x Viewer/OpenNcc_Linux/OpenNCC

#	rm -rf Viewer/OpenNcc_Linux/AppRun

#	ln Viewer/OpenNcc_Linux/OpenNCC Viewer/OpenNcc_Linux/AppRun

#	chmod +x Viewer/OpenNcc_Linux/Configuration/moviUsbBoot

#	rm -rf Viewer/OpenNcc_Windows

#	rm -rf Viewer/OpenNCC_Raspberry

#else

#	echo "Viewer is exist"

#fi



#echo "copy Linux lib to Viewer"

#cp Source/Library/C\&C++/libOpenNCC.a Viewer/QT_Package/OpenNCC/ncc_sdk/libOpenNCC.a
chmod +x Example/How_to/OpenNCC/C\&C++/Capture_video/script/clone.sh
chmod +x Example/How_to/OpenNCC/C\&C++/Capture_video/script/run.sh

chmod +x Example/How_to/OpenNCC/C\&C++/How_to_use_sdk/script/clone.sh
chmod +x Example/How_to/OpenNCC/C\&C++/How_to_use_sdk/script/run.sh

chmod +x Example/How_to/OpenNCC/C\&C++/Load_a_model/script/clone.sh
chmod +x Example/How_to/OpenNCC/C\&C++/Load_a_model/script/run.sh

chmod +x Example/How_to/OpenNCC/C\&C++/Multiple_models/script/clone.sh
chmod +x Example/How_to/OpenNCC/C\&C++/Multiple_models/script/run.sh

