# Background

In order to let users use Python for development, we launched this demo 

# Install

## Pi or Arm64
This demo uses libusb,ffmpeg,python3-opencv and opencv.Go check them out if you don't have them locally installed.

```shell
$ sudo apt-get install libusb-dev  libusb-1.0-0-dev
$ sudo apt-get install ffmpeg
$ sudo apt-get install libopencv-dev
$ sudo apt-get install python3-opencv
```
## Linux
$ sudo apt-get install libusb-dev  libusb-1.0-0-dev
$ sudo apt-get install ffmpeg
then cat Python_linux.md

Note:if you want to copy the command to the command line,remember to remove `$`.

# Usage

1. Run the copy script to copy the required files and libraries 

```shell
$ ./copy.sh
```

2. Run demo

```shell
$ sudo ./run.sh
```

**<font color="red">Note:You may come across problems that need keys</font>**

```shell
Firmvare version:MV02.101.17 Device SN:xxxxxxxxxxxxx
07:17:13 : sdk/sdk.cpp(667) need check key file ./fw/eyecloud.key from firm:./fw/flicRefApp.mvcmd pos 4
07:17:13 : sdk/sdk.cpp(671) no find eyecloud key file and no support!
load firmware error! return
```

At this time, you need to send the SN number to `info@eyecloud.tech`, and we will generate a key file for you.

copy key file to `fw` like this.

```shell
# current directory:openncc/Platform/Raspberry/Example/How_to/Python_demo
cp ~/eyecloud.key ./fw/
```

 Now,you can run `Openncc`

```shell
# current directory:openncc/Platform/Raspberry/Example/How_to/Python_demo
sudo ./run.sh
```

## Maintainers

beam.
