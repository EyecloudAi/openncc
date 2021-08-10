# Background

In order to understand how to load the model, you have this demo.Demo has been tested on raspberry pi and Ubuntu(usb 3.0).

# Install

This demo uses libusb,ffmpeg and opencv.Go check them out if you don't have them locally installed.

```shell
$ sudo apt-get install libusb-dev  libusb-1.0-0-dev
$ sudo apt-get install ffmpeg
$ sudo apt-get install libopencv-dev
```

Note:if you want to copy the command to the command line,remember to remove `$`.

# Usage

1. Run the copy script to copy the required files and libraries 

```shell
$ ./copy.sh
```

2.  Run the compile script to compile 

in linux(ubuntu16.04) or raspberry pi

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



# Demo introduce 

1. USB load firmware

   ```c++
   load_fw();
   ```

2. Get the current camera mode support list，Including resolution and frame rate

   ```c++
   camera_control_get_features();
   ```

3. Select camera working mode

   ```c++
   camera_select_sensor();
   ```

4. Configure device resoures

   ```c++
   cam_info.imageWidth   = cameraCfg.camWidth;　
   cam_info.imageHeight  = cameraCfg.camHeight;　　
   cam_info.isOutputYUV  = 1;　　　　　　　　　　//Initialize YUV output
   cam_info.isOutputH26X = 1;　　　　　　　　　　//Initialize H26X output
   cam_info.isOutputJPEG = 1;          //Initialize MJPEG output
   cam_info.mode         = ENCODE_H264_MODE;    //Select H26X encoding mode, 264/265
   ```

5. NCC configure related parameters

   5.1 Configure the effective area of the algorithm（left vertex and right bottom）

   ```c++
   cam_info.startX      = 0;
   cam_info.startY      = 0;
   cam_info.endX        = cameraCfg.camWidth;
   cam_info.endY        = cameraCfg.camHeight;
   ```

   5.2 Image preprocessing parameter initialization

   ```c++
   cam_info.inputDimWidth  = xxx;
   cam_info.inputDimHeight = xxx;
   cam_info.inputFormat    = IMG_FORMAT_BGR_PLANAR;
   cam_info.meanValue[0]   = 0;
   cam_info.meanValue[1]   = 0;
   cam_info.meanValue[2]   = 0;
   cam_info.stdValue       = 1;
   ```

   5.3 Blob file save path of the algorithm 

   ```shell
   "./blob/FD.blob"
   ```

6. Image display and metedata analysis test

   ```
   obj_show_img_func
   ```

   

## Maintainers

beam.
