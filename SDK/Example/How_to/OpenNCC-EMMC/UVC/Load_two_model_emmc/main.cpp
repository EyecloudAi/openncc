
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
#include "Fp16Convert.h"
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
void  vehicle_license_plate_detection_barrier(void *data, int w, int h, float scale, char *name, int nn_fov_show, Network1Par *nnParm1,Network2Par *nnParm2, char *nnret, float conf);
typedef void  (*analyzeMetedata)(void *data, int w, int h, float scale, char *name, int nn_fov_show, Network1Par *nnparm, Network2Par *nnParm2, char *nnret,float conf);
///////////////////////////////////////////////////////////////////////////////////

/* 第一级模型默认参数 */
static Network1Par cnn1PrmSet =
{
    imageWidth:-1, imageHeight:-1,                     /* 动态获取 */
    startX:-1, startY:-1,                              /* 根据获取的sensor分辨率设置 */
    endX:-1,endY: -1,                                  /*  根据获取的sensor分辨率设置 */
    inputDimWidth:-1, inputDimHeight:-1,               /* 根据获取的模型参数设置 */
    inputFormat:IMG_FORMAT_BGR_PLANAR,                 /*  默认为BRG输入 */
    meanValue:{0,0,0},
    stdValue:1,
    isOutputYUV:1,                                     /*打开YUV420输出功能*/
    isOutputH26X:1,                                    /*打开H26X编码功能*/
    isOutputJPEG:1,                                    /*打开MJPEG编码功能*/
    mode:ENCODE_H265_MODE,                             /* 使用H264编码格式 */
    extInputs:{0},                                     /* model多输入，第二个输入参数 */
    modelCascade:0 ,                                   /* 默认不级联下一级模型 */
    inferenceACC:0,
};

/* 第二级模型默认参数 */
static Network2Par cnn2PrmSet =
{
    startXAdj:0,
    startYAdj:0,
    endXAdj:0,
    endYAdj:0,
    labelMask:{0},                                       /* 需要处理的label掩码，如果对应位置至1才会处理 */
    minConf: 0.99,                                       /* 检测目标的置信度大于该值才会处理 */
    inputDimWidth:-1, inputDimHeight:-1,                 /* 根据获取的模型参数设置 */
    inputFormat:IMG_FORMAT_BGR_PLANAR,                   /*  默认为BRG输入 */
    meanValue:{0,0,0},
    stdValue:1,
    extInputs:{0},                                       /* model多输入，第二个输入参数 */
    modelCascade:0                                       /* 默认不级联下一级模型 */
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
        case (JPEG ):
        {
            /* 获取JPEG视频流数据 */
            char *jpeg_data = (char *) data + sizeof(frameSpecOut);
            FILE *fp;
			char src[64];
			sprintf(src, "%d.jpeg", out->seqNo);
            /* 获取H65视频流数据 */

//			if(out->seqNo %120 ==0)
//			{
//				fp = fopen(src, "w+");
//				if (fp != 0)
//				{
//					int size = fwrite((char*) jpeg_data, 1, out->size, fp);
//					fclose(fp);
//				}
//			}
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

    //////////2级模型demo/////////
    // 3. 1 第一级模型初始化初始化
    cnn1PrmSet.imageWidth  = 1920;
    cnn1PrmSet.imageHeight = 1080;

    cnn1PrmSet.startX      = 0;
    cnn1PrmSet.startY      = 0;
    cnn1PrmSet.endX        = 1920;
    cnn1PrmSet.endY        = 1080;
    cnn1PrmSet.inputDimWidth  = 300;
    cnn1PrmSet.inputDimHeight = 300;
    /* 级联下一级模型 */
    cnn1PrmSet.modelCascade = 1;

//3.2 第二级模型初始化
    if(cnn1PrmSet.modelCascade !=0)
    {

#if 1
    /* 适当微调第一级输出的检测范围
     *起点向左上方微调（startXAdj，startYAdj ）
     *底点向右下方微调（endXAdj，endYAdj） */
        cnn2PrmSet.startXAdj  = -5;
        cnn2PrmSet.startYAdj  = -5;
        cnn2PrmSet.endXAdj   = 5;
        cnn2PrmSet.endYAdj   = 5;
#endif
        cnn2PrmSet.minConf       = 0.7;  // 置信度设置，第一级目标检测大于这个值，才进入第二级模型处理
        cnn2PrmSet.labelMask[2] = 1;    // 车牌检测 label id = 2

        /*name1: "data" , shape: [1x3x24x94] - An input image in following format [1xCxHxW]. Expected color order is BGR.*/
        cnn2PrmSet.inputDimWidth  = 94;
        cnn2PrmSet.inputDimHeight = 24;
        /*
         *URL:  https://docs.openvinotoolkit.org/2019_R1.1/_license_plate_recognition_barrier_0001_description_license_plate_recognition_barrier_0001.html
         * name2: "seq_ind" , shape: [88,1] - An auxiliary blob that is needed for correct decoding. Set this to [0, 1, 1, ..., 1]. */
        //多输入初始化，转换成FP16
        uint16_t *p = (uint16_t *)cnn2PrmSet.extInputs;
        p[0] = (uint16_t)f32Tof16(0);
        for(int i=1;i<88;i++)
        {
        p[i] = (uint16_t)f32Tof16(1.0);
        }

        
    }
    fun = vehicle_license_plate_detection_barrier;
    conf =  cnn2PrmSet.minConf;
    
    
    usleep(10);

    int fd;

    fd = open(argv[1],O_RDWR);

    //查询摄像头视频格式，VIDIOC_ENUM_FMT
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
    //设置缓冲区管理方式
    struct v4l2_requestbuffers reqbuf;
    reqbuf.count = 4;
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuf.memory = V4L2_MEMORY_MMAP;

    if(ioctl(fd, VIDIOC_REQBUFS, &reqbuf) == -1)
	{
	    perror("VIDIOC_REQBUFS failed!\n");
	    return false;
	}

    //将申请到的缓冲区映射到用户程序中，以便在用户程序访问，方便处理
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

    //开始采集图像
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

    	//获取和处理数据
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        if (ioctl(fd, VIDIOC_DQBUF, &buf) == -1)
        {
            perror("VIDIOC_DQBUF failed!\n");
            usleep(10000);
            continue;
        }

        /* 获取帧号 */
        unsigned int *pSeqNo = (unsigned int *)framebuf[buf.index].start;
        printf("UVC SeqNo %d\n", *pSeqNo);

        /* metadata解析和显示 */
       fun((void*)framebuf[buf.index].start , 1920, 1080,  scale, "demo_video", 0, &cnn1PrmSet,&cnn2PrmSet, metadata, conf);

       //Do something，process frame data
   		if (ioctl(fd, VIDIOC_QBUF, &buf) == -1)
   		{
   			perror("VIDIOC_QBUF failed!\n");
   			continue;
   		}
    }

    return 0;
}