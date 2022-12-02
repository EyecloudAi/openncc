
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#include "pthread.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "sdk.h"
#include "cameraCtrl.h"
#include "cJSON.h"
#ifndef _WINDOWS
#include <sys/time.h>
#else
#include <windows.h>	
extern "C" int gettimeofday(struct timeval *tp, void *tzp);
#pragma warning(disable:4996)
#endif

extern "C" void os_sleep(int ms);
void  yolov4_show_img_func(void *data, int w, int h, float scale, char *name, int nn_fov_show, Network1Par *nnparm, char *nnret);

///////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    cJSON *ROOT;
    int ret;
    Network1Par cam_info;
    memset(&cam_info, 0, sizeof(cam_info));
    cam_info =
    {
        -1,  //imageWidth
        -1,  //imageHeight
        -1,  //startX
        -1,  //startY
        -1,   //endX
        -1,   //endY
        0,  //inputDimWidth
        0,   //inputDimHeight                   /* <dim>300</dim>  <dim>300</dim> */
        IMG_FORMAT_BGR_PLANAR,      //IMAGE_FORMAT
        0,                          //meanValue
        0,
        0 ,
        1,                           //stdValue
        1,                           /*Turn on YUV420 output function*/
        1,                           /*Turn on h26x coding function*/
        1,                           /*Turn on MJPEG encoding*/
        ENCODE_H264_MODE,            /* Use H264 encoding format */
        NULL,
        0,
        0
    };
    //1.Load firmware
    printf("==============================\n");
    printf("1.Load firmware\n");
    printf("==============================\n");
    ret = load_fw("./moviUsbBoot","./fw/OpenNcc.mvcmd");
    if (ret < 0)
    {
        printf("lowd firmware error! return \n");
        return -1;
    }
    printf("usb sersion:%d \n", get_usb_version());

    //2. Get the current camera mode support list
    printf("==============================\n");
    printf("2. Get the current camera mode support list\n");
    printf("==============================\n");
    SensorModesConfig cameraCfg;
    SensorModesList list;
    camera_control_get_features(&list);
   
    //3. Select camera working mode
    printf("==============================\n");
    printf("3. Select camera working mode\n");
    printf("==============================\n");
    int sensorModeId = 0; //0:1080P, 1:4K
    camera_select_sensor(sensorModeId);
    memcpy(&cameraCfg, &list.mode[sensorModeId], sizeof(cameraCfg)); //select camera info

    //4. Configure device resoures
    printf("==============================\n");
    printf("4. Configure device resoures\n");
    printf("==============================\n");
    cam_info.imageWidth   = cameraCfg.camWidth;
    cam_info.imageHeight  = cameraCfg.camHeight;
    cam_info.isOutputYUV  = 1;
    cam_info.isOutputJPEG = 1;
    cam_info.isOutputH26X = 1;
   cam_info.mode         = ENCODE_H264_MODE;

    //5. NCC configure related parameters
    printf("==============================\n");
    printf("5. NCC configure related parameters\n");
    printf("==============================\n");
   //5.1 Configure the effective area of the algorithm
    cam_info.startX      = 0;
    cam_info.startY      = 0;
    cam_info.endX        = cameraCfg.camWidth;
    cam_info.endY        = cameraCfg.camHeight;
    //5.2 Image preprocessing parameter initialization
    cam_info.inputDimWidth  = 416;
    cam_info.inputDimHeight = 416;
    cam_info.inputFormat    = IMG_FORMAT_BGR_PLANAR;
    cam_info.meanValue[0]   = 0;
    cam_info.meanValue[1]   = 0;
    cam_info.meanValue[2]   = 0;
    cam_info.stdValue       = 1;

     cam_info.inferenceACC=0;
    // 5.2 Blob file save path of the algorithm
    const char *blob = "./blob/2020.3/yolo/yolov4-tiny-fp16.blob";

    //6. sdk initialization
    printf("==============================\n");
    printf("6. sdk initialization\n");
    printf("==============================\n");
    ret = sdk_init_ex(NULL,NULL, (char*)blob, &cam_info, sizeof(cam_info));
    printf("sdk_init %d\n", ret);
    if (ret < 0)
        return -1;

    //7. Configure output
    printf("==============================\n");
    printf("7. Configure output\n");
    printf("==============================\n");
    camera_video_out(YUV420p,VIDEO_OUT_CONTINUOUS);


    char *recv_data     = (char*) malloc(sizeof(frameSpecOut) + cameraCfg.camWidth * cameraCfg.camHeight * 3 / 2);
    char *recv_metedata = (char*) malloc(1024*1024*10);
    if ((recv_data == 0) || (recv_metedata ==0))
          return -1;

    while (1)
    {
        char src[64];
        frameSpecOut hdr;
        float scale = 960 * 1.0 / cameraCfg.camWidth;
        int max_read_size;
       
        char *yuv420p, *memedata;

        //Block reading yuv420 data
        max_read_size  = sizeof(frameSpecOut)+ cameraCfg.camWidth * cameraCfg.camHeight * 3 / 2;
   
        if (read_yuv_data(recv_data, &max_read_size, true) < 0)
        {
            os_sleep(1);
            break;
        }
         //printf("read_yuv_data\n");
        memcpy(&hdr, recv_data, sizeof(frameSpecOut));
        //printf("yuvhdr :type %2d,seqNo:%-6d,size %d\n", hdr.type, hdr.seqNo,hdr.size);

        //Non-blocking read metedata data
        max_read_size = 1024*1024*10;
        //memset(recv_metedata, 0, max_read_size);
        if (read_meta_data(recv_metedata, &max_read_size, false) == 0)
        {
            memcpy(&hdr, recv_metedata, sizeof(frameSpecOut));
           // printf("metehdr:type:%2d,seqNo:%-6d,size %d, NCC_T:(%dMS)\n", hdr.type, hdr.seqNo,hdr.size, hdr.res[0]);
        }

        //Get YUV data and metedata data
        yuv420p  = (char*) recv_data + sizeof(frameSpecOut);
        memedata = (char*) recv_metedata + sizeof(frameSpecOut)+16*sizeof(int);

        //Test display
       // sprintf(src, "FD_video_%dx%d@%dfps(scale:%d%%)", cameraCfg.camWidth,
        //        cameraCfg.camHeight, cameraCfg.camFps, (int) (100 * scale));
        yolov4_show_img_func(yuv420p, cameraCfg.camWidth, cameraCfg.camHeight,scale, src, 1, &cam_info,memedata );
    }
    printf("exit test main....\n");
    sdk_uninit();
    return 0;
}