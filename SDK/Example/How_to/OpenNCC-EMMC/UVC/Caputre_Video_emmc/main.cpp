
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
void  opencv_show_img_func(void *data, int w, int h, float scale, char *name, int nn_fov_show, Network1Par *nnParm1, Network2Par *nnParm2, char *nnret,float minConf);
typedef void  (*analyzeMetedata)(void *data, int w, int h, float scale, char *name, int nn_fov_show, Network1Par *nnparm, Network2Par *nnParm2, char *nnret,float conf);

///////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    analyzeMetedata fun;
    
	//uart initialization
    uart_init();

	//Specifying the display function
    fun = opencv_show_img_func;

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

		//map buffer
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
    	/* show as 720P */
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
       fun((void*)framebuf[buf.index].start , 1920, 1080,  scale, "demo_video", 0, NULL,NULL, 0, 0);

       //Do something，process frame data
   		if (ioctl(fd, VIDIOC_QBUF, &buf) == -1)
   		{
   			perror("VIDIOC_QBUF failed!\n");
   			continue;
   		}
    }

    return 0;
}
