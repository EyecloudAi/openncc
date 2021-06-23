
Test environment(pass) ：ubuntu16.04, usb3.0.

Software dependencies and installation：
1. sudo apt-get install libusb-dev  libusb-1.0-0-dev 
2. sudo apt-get install ffmpeg
3. sudo apt-get install libopencv-dev
////////////////////////////////////////////////////////////////////////

Flow and description of the test procedure : 
The test initialization creates 3 threads to process YUV/H26X and mjpeg respectively. Yuv uses opencv to display directly, and a Mjpeg picture saved locally.At the same time a h26x picture saved locally and can be played by ffplay . 
Supports both 1080p and 4K modes, please refer to "Selecting Camera Working Mode".

1. USB load firmware
load_fw();

2. Get the current camera mode support list，Including resolution and frame rate
camera_control_get_features();
　
3. Select camera working mode
camera_select_sensor();
         
4. Configure device resoures
cam_info.imageWidth   = cameraCfg.camWidth;　
cam_info.imageHeight  = cameraCfg.camHeight;　　
cam_info.isOutputYUV  = 1;　　　　　　　　　　//Initialize YUV output
cam_info.isOutputH26X = 1;　　　　　　　　　　//Initialize H26X output
cam_info.isOutputJPEG = 1;          //Initialize MJPEG output
cam_info.mode         = ENCODE_H264_MODE;    //Select H26X encoding mode, 264/265

5. Create receiving thread
5.1 Turn on YUVdata output switch	(isOutputYUV must be set to 1)
    camera_yuv420_out(YUV420_OUT_SINGLE);　　　//Output single frame
    camera_yuv420_out(YUV420_OUT_CONTINUOUS);　//Output continuous frames 
    camera_yuv420_out(YUV420_OUT_DISABLE);　   //turn off output

5.2  Turn on MJPEGdata output switch(isOutputJPEG must be set to 1)
   camera_mjpeg_out(MJPEG_OUT_SINGLE); 　　　　//Output single frame
   camera_mjpeg_out(MJPEG_OUT_CONTINUOUS); 　　//Output continuous frames
   camera_mjpeg_out(MJPEG_OUT_DISABLE); 　　   //turn off output

5.3  Turn on H26x output switch(isOutputH26X must be set to 1)
   camera_h26x_out(H26X_OUT_ENABLE); 　　　　   //Turn on output
   camera_h26x_out(H26X_OUT_DISABLE); 　　　　　//turn off output　

////////////////////////////////////////////////////////////////////////

6. Test run
6.1 Compile: make clean;make -j
6.2 Run: Enter the upper director:Samples/bin，command:sudo ./Capturevideo

mjepg:    demo automatically saves MJPEG pictures
h26x Preview: ffplay x_x.h26x




