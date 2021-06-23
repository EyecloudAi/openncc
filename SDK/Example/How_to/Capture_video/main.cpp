
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

#ifndef _WINDOWS
#include <sys/time.h>
#else
#include <windows.h>	
extern "C" int gettimeofday(struct timeval *tp, void *tzp);
#pragma warning(disable:4996)
#endif

extern "C" void os_sleep(int ms);
void opencv_show_img_func(void *data, int w, int h, float scale, char *name);

void *yuvThread(void *arg)
{
    SensorModesConfig cameraCfg;
    memcpy(&cameraCfg, arg, sizeof(cameraCfg));

    //Turn on YUVdata output switch
    camera_video_out(YUV420p,VIDEO_OUT_CONTINUOUS);

    char *recv_data = (char*)malloc(sizeof(frameSpecOut) + cameraCfg.camWidth*cameraCfg.camHeight*3/2);
    if (recv_data < 0)
           return 0;

    while (1)
    {
        char src[64];
        frameSpecOut  hdr;
        float scale = 960 * 1.0 / cameraCfg.camWidth;
        int size = sizeof(frameSpecOut) + cameraCfg.camWidth*cameraCfg.camHeight*3/2;
        char *yuv420p;
        if (read_yuv_data(recv_data, &size, 1) < 0)
        {
            os_sleep(1);
            break;
        }
        memcpy(&hdr, recv_data, sizeof(frameSpecOut));
//        printf("hdr: type %d, seqNo %d, size %d\n", hdr.type, hdr.seqNo,hdr.size);

        //get YUV data
        yuv420p = (char*)recv_data + sizeof(frameSpecOut);

        sprintf(src, "demo_yuv420_video_%dx%d@%dfps(scale:%d%%)", cameraCfg.camWidth,
                cameraCfg.camHeight, cameraCfg.camFps, (int) (100 * scale));
        opencv_show_img_func(yuv420p , cameraCfg.camWidth, cameraCfg.camHeight, scale, src);

        //exit test 
        if(hdr.seqNo >= 800)
        {
            free(recv_data);
            //turn off yuv420p data output
            camera_video_out(YUV420p,VIDEO_OUT_DISABLE );
            break;
        }
    }

    printf("yuv420p test exit\n");

    return 0;
}

void *jpegThread(void *arg)
{
    SensorModesConfig cameraCfg;
    FILE *fp;
    int max_recv_size, size;
    char *recv_data;
    frameSpecOut hdr;
    char src[64];

    memcpy(&cameraCfg, arg, sizeof(cameraCfg));
    max_recv_size = sizeof(frameSpecOut) + cameraCfg.camWidth * cameraCfg.camHeight * 3 / 2;

    recv_data = (char*) malloc(max_recv_size);
    if (recv_data < 0)
        return 0;

    os_sleep(3000);
    ///////////////////////////////////////////////////////////////
    //Output single frame，read jpeg data
    camera_video_out(JPEG,VIDEO_OUT_SINGLE);
    size = max_recv_size;
    if (read_jpg_data(recv_data, &size, 1) < 0)
    {
        os_sleep(1);
        return 0;
    }
    memcpy(&hdr, recv_data, sizeof(frameSpecOut));
    printf("hdr: type %d, seqNo %d, size %d\n", hdr.type, hdr.seqNo, hdr.size);
    sprintf(src, "test_%d.jpg",hdr.seqNo);
    fp = fopen(src, "w");
    size = fwrite((char*)recv_data + sizeof(frameSpecOut), 1, size, fp);
    fclose(fp);
    printf("save jpg file %d\n", size);
    ///////////////////////////////////////////////////////////////

    //Output continuous frames
    camera_video_out(JPEG,VIDEO_OUT_CONTINUOUS);
    while(1)
    {
        char *jpeg;

        //Block reading jpeg data
        size = max_recv_size;
        if (read_jpg_data(recv_data, &size, 1) < 0)
        {
            os_sleep(1);
            break;
        }
        memcpy(&hdr, recv_data, sizeof(frameSpecOut));
//        printf("hdr: type %d, seqNo %d, size %d\n", hdr.type, hdr.seqNo,hdr.size);

        //gert jpeg data
        jpeg = (char*)recv_data + sizeof(frameSpecOut);

        //exit test
        if(hdr.seqNo >= 700)
        {
            free(recv_data);

            //turn off jpeg data output
            camera_video_out(JPEG,VIDEO_OUT_DISABLE);
            break;
        }
    }

    printf("jpeg test exit\n");

    return 0;
}

/* H26x file display: ffplay x_x.h26x */
void *h26xThread(void *arg)
{
    SensorModesConfig cameraCfg;
    FILE *fp;
    int max_recv_size, size;
    char *recv_data;
    frameSpecOut hdr;
    char src[64];

    memcpy(&cameraCfg, arg, sizeof(cameraCfg));
    max_recv_size = sizeof(frameSpecOut) + cameraCfg.camWidth * cameraCfg.camHeight * 3 / 2;

    recv_data = (char*) malloc(max_recv_size);
    if (recv_data < 0)
        return 0;

    sprintf(src, "%d_%d.h26x", cameraCfg.camWidth , cameraCfg.camHeight);
    fp = fopen(src, "w");

    //turn on h26x data output
    camera_video_out(H26X,VIDEO_OUT_CONTINUOUS);

    while (1)
    {
        char *h26x;
        int  w_size;
        //block reading jpeg data
        size = max_recv_size;
        if (read_26x_data(recv_data, &size, 1) < 0)
        {
            os_sleep(1);
            break;
        }
        memcpy(&hdr, recv_data, sizeof(frameSpecOut));

        /* get H65 video stream data */
        char *h265_data = (char *) recv_data + sizeof(frameSpecOut);
        if(fp !=0)
        {
            fwrite((char*)h265_data, 1, hdr.size, fp);
        }

        //exit test
        if (hdr.seqNo >= 700)
        {
            fclose(fp);
            free(recv_data);
            //turn off h26x output
            camera_video_out(H26X,VIDEO_OUT_DISABLE);
            break;
        }
    }

    printf("h26x test exit\n");

    return 0;
}
///////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    int ret;
    CameraInfo cam_info;
    pthread_t threadid_yuv,threadid_h26x,threadid_jpeg;
    memset(&cam_info, 0, sizeof(cam_info));

    //1.load firmware
    ret = load_fw("./moviUsbBoot","./fw/flicRefApp.mvcmd");
    if (ret < 0)
    {
        printf("lowd firmware error! return \n");
        return -1;
    }
    printf("usb sersion:%d \n", get_usb_version());

    //2. Get the current camera mode support list
    SensorModesConfig cameraCfg;
    SensorModesList list;
    camera_control_get_features(&list);
    printf("list num:%d\n", list.num);
    for (int i = 0; i < list.num; i++)
    {
        SensorModesConfig features;
        memcpy(&features, &list.mode[i], sizeof(features));
        printf("[%d/%d]camera: %s, %dX%d@%dfps, AFmode:%d, maxEXP:%dus,gain[%d, %d]\n",
                i, list.num, features.moduleName, features.camWidth,
                features.camHeight, features.camFps, features.AFmode,
                features.maxEXP, features.minGain, features.maxGain);
    }

    //3. Select camera working mode
    int sensorModeId = 0; //0:1080P, 1:4K
    camera_select_sensor(sensorModeId);
    memcpy(&cameraCfg, &list.mode[sensorModeId], sizeof(cameraCfg)); //select camera info

    //4. Configure device resoures
    cam_info.imageWidth   = cameraCfg.camWidth;
    cam_info.imageHeight  = cameraCfg.camHeight;
    cam_info.isOutputYUV  = 1;
    cam_info.isOutputH26X = 1;
    cam_info.isOutputJPEG = 1;
    cam_info.mode         = ENCODE_H264_MODE;
    ret = sdk_init(NULL, NULL, (char*) NULL, &cam_info, sizeof(cam_info));

    printf("xlink_init %d\n", ret);
    if (ret < 0)
        return -1;

    //5. Create receiving thread
    int rv = pthread_create(&threadid_yuv, NULL, yuvThread, &cameraCfg);
    if (rv)
    {
        printf("Error - pthread_create() return code: %d\n", rv);
        return -1;
    }
    pthread_create(&threadid_h26x, NULL, h26xThread, &cameraCfg);
    if (rv)
    {
        printf("Error - pthread_create() return code: %d\n", rv);
        return -1;
    }
    pthread_create(&threadid_jpeg,NULL, jpegThread, &cameraCfg);
    if (rv)
    {
        printf("Error - pthread_create() return code: %d\n", rv);
        return -1;
    }

    pthread_join(threadid_yuv, NULL);
    pthread_join(threadid_h26x, NULL);
    pthread_join(threadid_jpeg, NULL);


    printf("exit test main....\n");
    sdk_uninit();
    return 0;
}
