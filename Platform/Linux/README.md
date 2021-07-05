# Install QT

OS:Ubuntu16.04

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

# Install ffmpeg and opencv

```shell
sudo apt-get update
sudo apt-get install libgl1-mesa-dev -y
sudo apt-get install unixodbc -y 
sudo apt-get install libpq-dev -y 
sudo apt-get install libopencv-dev -y
sudo apt-get install ffmpeg -y
sudo apt-get install libusb-dev -y
sudo apt-get install libusb-1.0.0-dev -y
```

# Run Example

```shell
# current directory:/home/beam/gitlab/openncc/Platform/Linux/
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
# current directory:/home/beam/gitlab/openncc/Platform/Linux/Example/How_to/Capture_video
sudo ./copy.sh
sudo ./compile.sh
sudo ./run.sh
```

## 2.How_to_use_sdk

```shell
# current directory:/home/beam/gitlab/openncc/Platform/Linux/Example/How_to/How_to_use_sdk
sudo ./copy.sh
sudo ./compile.sh
sudo ./run.sh
```

## 3.Load_a_model

```shell
# current directory:/home/beam/gitlab/openncc/Platform/Linux/Example/How_to/Load_a_model
sudo ./copy.sh
sudo ./compile.sh
sudo ./run.sh
```

## 4.Multiple_models

```shell
# current directory:/home/beam/gitlab/openncc/Platform/Linux/Example/How_to/Multiple_models
sudo ./copy.sh
sudo ./compile.sh
sudo ./run.sh
```



# Run View

**step 1**:open qtcreator

```shell
cd /opt/Qt5.12.9/Tools/QtCreator/bin/
sudo ./qtcreator
```

![image-20210701145723395](./Image/RunViewstep1.jpg)

**step 2**:open project

`ctrl+O`

![image-20210701145812096](./Image/RunViewstep2.jpg)

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

![image-20210701150629618](/home/beam/gitlab/openncc/Platform/Linux/Image/RunViewstep5.jpg)

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

**step 1**:copy linuxdeployqt-linux(in QT_Package) to /usr/local/bin/

```shell
sudo cp linuxdeployqt-linux /usr/local/bin/
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

**step 4**:modify /usr/local/bin/linuxdeployqt-linux to /usr/local/bin/linuxdeployqt

```shell
sudo mv /usr/local/bin/linuxdeployqt-linux /usr/l
ocal/bin/linuxdeployqt 
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
