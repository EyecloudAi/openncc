# Background

It is a demo on how to used the sdk in linux os(ubunto) or raspberry pi.

# Install

This demo uses libusb,ffmpeg and opencv.Go check them out if you don't have them locally installed.

```shell
$ sudo apt-get install libusb-dev  libusb-1.0-0-dev
$ sudo apt-get install ffmpeg
$ sudo apt-get install libopencv-dev
```

Note:

**if you want to copy the command to the command line,remember to remove `$`.**

**we used opecv4.4 which are from openvino2020.3 version**
**if you used opencv3.x,just edit the makefile to select it**
**OPENCV = `pkg-config opencv --cflags --libs`**
**remove -I/usr/local/include/opencv4**

# Usage

1. Run the copy script to copy the required files and libraries 

```shell
$ ./copy.sh
```

2.  Run the compile script to compile 

```shell
$ ./compile.sh
```

in arm64(nvidia)

Because opencv installed is 4.1.1,So you need to change the content in your Makefile.

Change 
```shell
OPENCV = `pkg-config opencv --cflags --libs`
```
to 
```shell
OPENCV = `pkg-config opencv4 --cflags --libs`
```

add `$(OPENCV)` after `COMMFLAG = -DUSE_WD`
like `COMMFLAG = -DUSE_WD $(OPENCV)`

then

```shell
$ ./compile.sh
```


3. Run demo

```shell
$ sudo ./run.sh
#"#0 support one ai net example [default] face blob"
#"#1 two ai net vehicle_license_plate_detection_barrier"
#"#2 one ai net and 2 input"
#"#3 used call back to get data from ncc device"

```

Note: If you are a raspberry pi system, you may encounter the following problems 

**<font color="red">Note:You may come across problems that need keys</font>**

```shell
Firmvare version:MV02.101.17 Device SN:xxxxxxxxxxxxx
07:17:13 : sdk/sdk.cpp(667) need check key file ./fw/eyecloud.key from firm:./fw/flicRefApp.mvcmd pos 4
07:17:13 : sdk/sdk.cpp(671) no find eyecloud key file and no support!
load firmware error! return
```

At this time, you need to send the SN number to `info@eyecloud.tech`, and we will generate a key file for you.

copy key file to `bin/fw` like this.

```shell
cp ~/eyecloud.key ./fw/
```

 Now,you can run 

```shell
sudo ./run.sh
```



## Maintainers

beam.

