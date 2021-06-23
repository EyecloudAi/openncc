
Test environment(pass)：ubuntu16.04, usb3.0.

Software dependencies and installation
1. sudo apt-get install libusb-dev  libusb-1.0-0-dev 
2. sudo apt-get install ffmpeg
3. sudo apt-get install libopencv-dev

////////////////////////////////////////////////////////////////////////

Flow and description of the test procedure :
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

5. NCC configure related parameters
 5.1 Configure the effective area of the algorithm（left vertex and right bottom）
    cam_info.startX      = 0;
    cam_info.startY      = 0;
    cam_info.endX        = cameraCfg.camWidth;
    cam_info.endY        = cameraCfg.camHeight;
 5.2 Image preprocessing parameter initialization
    cam_info.inputDimWidth  = xxx;
    cam_info.inputDimHeight = xxx;
    cam_info.inputFormat    = IMG_FORMAT_BGR_PLANAR;
    cam_info.meanValue[0]   = 0;
    cam_info.meanValue[1]   = 0;
    cam_info.meanValue[2]   = 0;
    cam_info.stdValue       = 1;
 5.3 Blob file save path of the algorithm 
   "./blob/FD.blob"

6. Image display and metedata analysis test
obj_show_img_func
////////////////////////////////////////////////////////////////////////
7. Test run
7.1 Compile: make clean;make -j
7.2 Run: Enter the upper director:Samples/bin，command　sudo ./Openncc





