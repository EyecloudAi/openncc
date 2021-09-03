#!/bin/bash



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

	cp -r ../../SDK/Source/Library/Linux/ ./Source/Library

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

	if [ ! -f "Source/Firmware/OpenNcc.mvcmd" ];then

		cp ../../SDK/Source/Firmware/OpenNcc.mvcmd Source/Firmware/

	else

		echo "OpenNcc.mvcmd is exist,don't need copy"

	fi

fi



echo "copy Example ..... "

if [ ! -d "Example/" ];then

	cp -r ../../SDK/Example/ ./

else

	echo "Example is exist"

fi

#rm -rf Example/How_to/Python_demo

echo "copy Viewer ...... "

if [ ! -d "Viewer/" ];then

	cp -r ../../SDK/Viewer/ ./

	chmod +x Viewer/OpenNcc_Linux/OpenNCC

	rm -rf Viewer/OpenNcc_Linux/AppRun

	ln Viewer/OpenNcc_Linux/OpenNCC Viewer/OpenNcc_Linux/AppRun

	chmod +x Viewer/OpenNcc_Linux/Configuration/moviUsbBoot

	rm -rf Viewer/OpenNcc_Windows

	rm -rf Viewer/OpenNCC_Raspberry

else

	echo "Viewer is exist"

fi



echo "copy Linux lib to Viewer"

cp Source/Library/C\&C++/libOpenNCC.a Viewer/QT_Package/OpenNCC/ncc_sdk/libOpenNCC.a
chmod +x Example/How_to/Capture_video/copy.sh
chmod +x Example/How_to/Capture_video/compile.sh
chmod +x Example/How_to/Capture_video/run.sh

chmod +x Example/How_to/How_to_use_sdk/copy.sh
chmod +x Example/How_to/How_to_use_sdk/compile.sh
chmod +x Example/How_to/How_to_use_sdk/run.sh

chmod +x Example/How_to/Load_a_model/copy.sh
chmod +x Example/How_to/Load_a_model/compile.sh
chmod +x Example/How_to/Load_a_model/run.sh

chmod +x Example/How_to/Multiple_models/copy.sh
chmod +x Example/How_to/Multiple_models/compile.sh
chmod +x Example/How_to/Multiple_models/run.sh


