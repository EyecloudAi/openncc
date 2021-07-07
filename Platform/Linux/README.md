[TOC]

# Install ffmpeg and opencv

```shell
sudo apt-get update
sudo apt-get install libgl1-mesa-dev -y
sudo apt-get install unixodbc -y 
sudo apt-get install libpq-dev -y 
sudo apt-get install ffmpeg -y
sudo apt-get install libusb-dev -y
sudo apt-get install libusb-1.0.0-dev -y
```

opencv version:3.4.0 on ubuntu

download url:https://github.com/opencv/opencv/archive/refs/tags/3.4.0.tar.gz

refer to:https://blog.csdn.net/guduruyu/article/details/72965535

```shell
# 1.
sudo apt-get install build-essential
sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev
# 2.install cmake
sudo apt-get install cmake
# 3.install pkg-config
sudo apt-get install pkg-config
# 4.download opencv 3.4.0 https://github.com/opencv/opencv/archive/refs/tags/3.4.0.tar.gz
# 5.unzip 3.4.0.tar.gz
tar zxvf opencv-3.4.0.tar.gz
# 6.enter opencv-3.4.0 and mkdir build
cd opencv-3.4.0
mkdir build
# 7.cmake
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local .. 
```

Note: If you encounter the problem in the figure below when you cmake, Please download it from the link below and put it to `opencv-3.4.0/3rdparty/ippicv/downloads/linux-808b791a6eac9ed78d32a7666804320e/`

![img](./Image/Center.png)

download url:

```
链接:https://pan.baidu.com/s/16Sy0IZAq3gI4NPIefBVxLw
提取码:nfvo
```

```shell
# 8.make
make
sudo make install
# 9.enter /etc/ld.so.conf.d/
cd /etc/ld.so.conf.d/
# 10.add /usr/local/lib to opencv.conf
sudo gedit opencv.conf
```

![image-20210706172805362](./Image/opencvconf.png)

```shell
# 11.save and exit
sudo ldconfig
# 12.add 
# PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig 
# export PKG_CONFIG_PATH 
# to /etc/bash.bashrc
sudo gedit /etc/bash.bashrc
```

![image-20210706173115393](./Image/addbashrc.png)

```shell
# 13.save and exit
# use pkg-config opencv --modversion to find opencv version
pkg-config opencv --modversion
```

![image-20210706173303118](./Image/findopencvversion.png)

# Run Example

```shell
# current directory:openncc/Platform/Linux/
cd Example/How_to
```

```
└── How_to
	├── Capture_video
	├── How_to_use_sdk
	├── Load_a_model
	├── Multiple_models
	├── Python_demo
```

## 1.Capture_video

```shell
# current directory:openncc/Platform/Linux/Example/How_to/Capture_video
./copy.sh
./compile.sh
sudo ./run.sh
```

## 2.How_to_use_sdk

```shell
# current directory:openncc/Platform/Linux/Example/How_to/How_to_use_sdk
./copy.sh
./compile.sh
sudo ./run.sh
```

## 3.Load_a_model

```shell
# current directory:openncc/Platform/Linux/Example/How_to/Load_a_model
./copy.sh
./compile.sh
sudo ./run.sh
```

## 4.Multiple_models

```shell
# current directory:openncc/Platform/Linux/Example/How_to/Multiple_models
./copy.sh
./compile.sh
sudo ./run.sh
```

# Install QT

OS:Ubuntu16.04

**Note:If you install QT yourself, you'd better uninstall it , Then follow our steps to install** 

download url:https://download.qt.io/official_releases/qt/5.12/5.12.9/

file name:`qt-opensource-linux-x64-5.12.9.run`

**step 1**:enter Download and `chmod +x`

![image-20210701145141262](./Image/installQTstep1.jpg)

```shell
chmod +x qt-opensource-linux-x64-5.12.9.run
```

**step 2**:run qt install

```shell
sudo ./qt-opensource-linux-x64-5.12.9.run
```

![img](./Image/Setup1.jpg) 

* Next

![img](./Image/Setup2.jpg) 

* Next

![img](./Image/Setup3.jpg) 

* Next

![img](./Image/Setup4.jpg) 

* Next

![img](./Image/Setup5.jpg) 

* Next

![img](./Image/Setup6.jpg) 

* Next

![img](./Image/Setup7.jpg) 

* Next

![img](./Image/Setup8.jpg) 

* Install

![img](./Image/Setup9.jpg) 

* Finish

# Run View

**step 1**:open qtcreator

```shell
# enter your qt install directory
cd /opt/Qt5.12.9/Tools/QtCreator/bin/
sudo ./qtcreator
```

![image-20210701145723395](./Image/RunViewstep1.jpg)

**step 2**:open project

`ctrl+O` or `

![image-20210701145812096](./Image/RunViewstep2.jpg)

`OpenNCC.pro` directory:`openncc/Platform/Linux/Viewer/QT_Package/OpenNCC`

![image-20210701145921106](./Image/RunViewstep2_1.jpg)

`Open`

**step 3**:build

![image-20210701153705572](./Image/RunViewstep3.jpg)

**step 4**:Run

![image-20210701153820161](./Image/RunViewstep4.jpg)

![image-20210701150443715](./Image/RunViewstep4_1.jpg)

copy `Configuration` to `build-OpenNCC-Desktop_Qt_5_12_9_GCC_64bit-Release`

![image-20210701150552363](./Image/RunViewstep4_2.jpg)

```shell
cd Configuration
sudo chmod +x moviUsbBoot
```

![image-20210701150749597](./Image/RunViewstep4_3.jpg)

**step 5**:Run again

![image-20210701150629618](./Image/RunViewstep5.jpg)

* Get device info

![image-20210701150817746](./Image/RunViewstep5_1.jpg)

* Select `Stream Resolution`

![image-20210701154142480](./Image/RunViewstep5_2.jpg)

* You can choose the model or not 

![image-20210701134129554](./Image/RunViewstep5_3.jpg)

* You can modify `Model Score` or not

![image-20210701134229926](./Image/RunViewstep5_4.jpg)

* You can modify `Display Scaler` or not

![image-20210701134253454](./Image/RunViewstep5_5.jpg)

* choose `Exposure control` Auto or Manual

![image-20210701134421596](./Image/RunViewstep5_6.jpg)

* Start running model

![image-20210701151042079](./Image/RunViewstep5_7.jpg)

# Pack QT program

download linuxdeployqt from https://github.com/probonopd/linuxdeployqt/releases

![image-20210707113905727](./Image/linuxdeployqt.png)

**step 1**:copy linuxdeployqt-7-x86_64.AppImage to /usr/local/bin/

```shell
sudo cp linuxdeployqt-7-x86_64.AppImage /usr/local/bin/
```

**step 2**:copy `OpenNCC` to other folder.

```shell
cd /home/beam/gitlab/openncc/Platform/Linux/Viewer/QT_Package/build-OpenNCC-Desktop_Qt_5_12_9_GCC_64bit-Release/
mkdir ~/OpenNCC_Linux
cp OpenNCC ~/OpenNCC_Linux/
```

**step 3**:copy `Configuration` to `OpenNCC_Linux`

```shell
cp -r Configuration/ ~/OpenNCC_Linux/
```

**step 4**:modify /usr/local/bin/linuxdeployqt-7-x86_64.AppImage to /usr/local/bin/linuxdeployqt

```shell
sudo mv /usr/local/bin/linuxdeployqt-7-x86_64.AppImage /usr/local/bin/linuxdeployqt 
sudo chmod +x /usr/local/bin/linuxdeployqt
```

**step 5**:pack qt program

```shell
cd ~/OpenNCC_Linux/
linuxdeployqt OpenNCC -appimage
```

![image-20210701152037265](./Image/Packstep5.jpg)

**step 6**:Run AppRun

```shell
sudo ./AppRun
```

![image-20210701152128308](./Image/Packstep6.jpg)
