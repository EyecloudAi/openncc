
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#include "pthread.h"
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "sdk.h"
#include "cameraCtrl.h"
#include "usb_uart.h"
#include <linux/videodev2.h>
#ifndef _WINDOWS
#include <sys/time.h>
#else
#include <windows.h>	
extern "C" int gettimeofday(struct timeval *tp, void *tzp);
#pragma warning(disable:4996)
#endif


typedef struct __video_buffer
{
    void *start;
    size_t length;
} video_buf_t;
////////////////////////////////////////////////////////////////////////
void  obj_show_img_func(void *data, int w, int h, float scale, char *name, int nn_fov_show, Network1Par *nnparm,Network2Par *nnParm2, char *nnret,float conf);

typedef void  (*analyzeMetedata)(void *data, int w, int h, float scale, char *name, int nn_fov_show, Network1Par *nnparm, Network2Par *nnParm2, char *nnret,float conf);
///////////////////////////////////////////////////////////////////////////////////

/* 第一级模型默认参数 */
static Network1Par cnn1PrmSet =
{
    imageWidth:-1, imageHeight:-1,                     /* dynamic get */
    startX:-1, startY:-1,                              /* set according to resolution */
    endX:-1,endY: -1,                                  /* set according to resolution */
    inputDimWidth:-1, inputDimHeight:-1,               /* model input parameter */
    inputFormat:IMG_FORMAT_BGR_PLANAR,                 /* format of image */
    meanValue:{0,0,0},
    stdValue:1,
    isOutputYUV:1,                                     /* YUV420 enabled*/
    isOutputH26X:1,                                    /* H26X enabled */
    isOutputJPEG:1,                                    /* MJPEG enabled */
    mode:ENCODE_H265_MODE,                             /* select H264 mode*/
    extInputs:{0},                                     /* additional parameter inputs */
    modelCascade:0 ,                                   /* cascade model enabled */
    inferenceACC:0,
};


char metadata[10*1024*1024]={0};
///////////////////////////////////////////////////////////////////////////////////
float  nccFps = 0;
double nccFpsSum=0;
unsigned int  frameCnt = 0;
float  sensorFps = 30;

static void uartEp1Read(unsigned char * data, int len)
{
    static  struct timeval tv1, tv2;
    static  int res_cnt = 0;

    /* 获取视频流数据头 */
    frameSpecOut *out = (frameSpecOut *)data;

    /* 异常数据，丢弃 */
    if(out->size>len)
    	return;

    printf("uartEp1Read seqNo: %d\n" ,out->seqNo );

    if(JPEG == out->type && out->seqNo%120==0)
        printf("Meta:type:%d,size:%d,seqNo:%d,T CSS=%d MSS=%d UPA0=%d UPA1=%d\n", out->type, out->size, out->seqNo,
        		 out->res[1], out->res[2], out->res[3], out->res[4]);

    switch (out->type)
    {
        case (METEDATA) :
        {
        	frameSpecOut hdr;
        	memcpy(&hdr, data, sizeof(hdr));
        	nccFpsSum +=1000.0/hdr.res[0];
        	frameCnt++;
        	nccFps = nccFpsSum/frameCnt;
            /* 获取NCS计算结果, 数据格式是fp16 */
            char *nn_ret = (char *) data + sizeof(frameSpecOut);
            memcpy(metadata, nn_ret, out->size<sizeof(metadata)?out->size:sizeof(metadata));
        }
        break;

        default:
            break;
    }
}
///////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    float conf=0.3;

    analyzeMetedata fun;
    
    uart_init();
    registerUartCb(uartEp1Read, 1);

    cnn1PrmSet.imageWidth  = 1920;
    cnn1PrmSet.imageHeight = 1080;

    cnn1PrmSet.startX      = 0;
    cnn1PrmSet.startY      = 0;
    cnn1PrmSet.endX        = 1920;
    cnn1PrmSet.endY        = 1080;
    cnn1PrmSet.inputDimWidth = 300;
    cnn1PrmSet.inputDimHeight = 300;

    fun = obj_show_img_func;
    conf = 0.7;

    usleep(10);

    int fd;
    fd = open(argv[1],O_RDWR);

    //Query camera video format，VIDIOC_ENUM_FMT
    printf("-------------\n");
    struct v4l2_fmtdesc fmtdesc;

    fmtdesc.index = 0;
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    printf("Supprotformat:\n");

    while(ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc)!=-1)
    {
        printf("\t%d.%c%c%c%c\t%s\n",fmtdesc.index+1,fmtdesc.pixelformat & 0xFF,\
        		(fmtdesc.pixelformat >> 8) & 0xFF,(fmtdesc.pixelformat >> 16) & 0xFF, (fmtdesc.pixelformat >> 24) & 0xFF,fmtdesc.description);
        fmtdesc.index++;
    }
    //Set the buffer management mode
    struct v4l2_requestbuffers reqbuf;
    reqbuf.count = 4;
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuf.memory = V4L2_MEMORY_MMAP;

    if(ioctl(fd, VIDIOC_REQBUFS, &reqbuf) == -1)
	{
	    perror("VIDIOC_REQBUFS failed!\n");
	    return false;
	}

    //Map buffer to user program
    video_buf_t *framebuf;
    framebuf = (video_buf_t*)calloc(reqbuf.count,sizeof(video_buf_t));
    struct v4l2_buffer buf;

    for(int i=0;i<reqbuf.count;i++)
    {
        buf.index = i;
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		if (ioctl(fd, VIDIOC_QUERYBUF, &buf) == -1)
		{
			perror("VIDIOC_QUERYBUF failed!\n");
			return false;
		}

		//mmap buffer
		framebuf[i].length = buf.length;
		framebuf[i].start = mmap(NULL, buf.length,
								 PROT_READ | PROT_WRITE,
								 MAP_SHARED, fd, buf.m.offset);
		if (framebuf[i].start == MAP_FAILED)
		{
			perror("mmap failed!\n");
			return false;
		}
		//buffer queue
		if (ioctl(fd, VIDIOC_QBUF, &buf) == -1)
		{
			perror("VIDIOC_QBUF failed!\n");
			return false;
		}
    }

    //Start capturing images
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd, VIDIOC_STREAMON, &type) == -1)
    {
        perror("VIDIOC_STREAMON failed!\n");
        return false;
    }

    while(1)
    {
    	/* 720P显示 */
    	float scale = 1.0*1280/1920;

    	//Get and process data
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        if (ioctl(fd, VIDIOC_DQBUF, &buf) == -1)
        {
            perror("VIDIOC_DQBUF failed!\n");
            usleep(10000);
            continue;
        }

        /* Get frame number */
        unsigned int *pSeqNo = (unsigned int *)framebuf[buf.index].start;
        printf("UVC SeqNo %d\n", *pSeqNo);

        /* analysis metadata */
       fun((void*)framebuf[buf.index].start , 1920, 1080,  scale, "demo_video", 0, &cnn1PrmSet,NULL, metadata, conf);

       //Do something，process frame data
   		if (ioctl(fd, VIDIOC_QBUF, &buf) == -1)
   		{
   			perror("VIDIOC_QBUF failed!\n");
   			continue;
   		}
    }

    return 0;
}
