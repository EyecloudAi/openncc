#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
//#include <pthread.h>
#include <time.h>
#include <stdint.h>
//#include <semaphore.h>
#include <fstream>
//#include<sys/types.h>
//#include<sys/stat.h>
//#include<fcntl.h>
#include "sdk.h"

#ifndef _WINDOWS
#include <unistd.h>
#else
#pragma warning(disable:4996)
#include <windows.h>	
#endif

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui_c.h>

#include "sdk.h"
#include "Fp16Convert.h"

//#include<vector>
using namespace std;
//#include <sys/time.h>
using namespace cv;

static float min_fd_score = 0.9;

static int coordinate_is_valid(float x1, float y1, float x2, float y2)
{
    if((x1<0) || (x1>1))
        return 0;
    if((y1<0) || (y1>1))
        return 0;
    if((x2<0) || (x2>1))
        return 0;
    if((y2<0) || (y2>1))
        return 0;
    if((x1>=x2) || (y1>=y2))
        return 0;

    return 1;
}

bool bsave = false;
extern bool g_run;
void *ControlThread(void* arg)
{
	printf("enter control thread!\n");
	int c;
	//while ((c = waitKey(0)) != 'q')
	while ((c = getchar()) != 'q')
	{
		switch (c)
		{
		case ('s'):
			bsave = true;
			break;
		default:
			break;
		}
	}
	g_run = false;
	return NULL;
}

void  fd_show_img_func(void *data, int w, int h, float scale, char *name, int nn_fov_show, CameraInfo *nnparm, char *nnret)
{
	cv::Mat yuvImg;
	yuvImg.create(h * 3/2, w, CV_8UC1);
	cv::Mat yuvImg_resized;
	cv::Mat outgoing_img;
	int i,dis_w,dis_h,oft_x, oft_y;
	float x0, x1, y0, y1, score;
	uint16_t* cls_ret = (uint16_t*)nnret;
  
	/* YUV420P-->RGB */
	yuvImg.data = (unsigned char*)data;
	cv::cvtColor(yuvImg, outgoing_img, CV_YUV2BGR_I420);

	/* 获取算法的fov */
	oft_x = nnparm->startX;
	oft_y = nnparm->startY;
	dis_w = nnparm->endX - nnparm->startX;
	dis_h = nnparm->endY - nnparm->startY;
	
//image id+label+confidence+x+y+w+h
	//printf("yuv image %d X %d \n",yuvImg.cols,yuvImg.rows);
	
	for (i = 0; i < 100; i++)
	{
		int image_id = (int)(f16Tof32(cls_ret[i*7+0])); 
		if (image_id < 0) {
//			printf("image_id %d, %d\n",image_id, i);
			break;
		}
		int label_id = (int)(f16Tof32(cls_ret[i*7+1])); 
    score =(float)f16Tof32(cls_ret[i*7+2]);
		x0 = f16Tof32(cls_ret[i*7+3]);
		y0 = f16Tof32(cls_ret[i*7+4]);
		x1 = f16Tof32(cls_ret[i*7+5]);
		y1 = f16Tof32(cls_ret[i*7+6]);
	
	//	printf("score:%.2f<->min:%.2f  rec:(%.3f,%.3f)<->(%.3f,%.3f) \n" ,score,min_fd_score,x0,y0,x1,y1);
		/* 不显示无效数据或者概率太低的框 */
		if( (coordinate_is_valid(x0, y0, x1, y1) ==0 )|| (score < min_fd_score))
		{
			continue;
		}

		/* 画识别的人脸框 */
		cv::Rect box;
		box.x = x0 * dis_w + oft_x;
		box.y = y0 * dis_h + oft_y;
		box.width  = (x1 - x0) * dis_w;
		box.height = (y1 - y0) * dis_h;
		cv::rectangle(outgoing_img, box, cv::Scalar(255, 128, 128), 2, 8, 0);

		cv::Point origin;
		origin.x = box.x ;
		origin.y = box.y + 32;
		char   result[128];
		memset(result, 0, sizeof(result));
		sprintf(result, "score:%d%%", (int)(100*score));
		cv::putText(outgoing_img, result, origin, cv::FONT_HERSHEY_COMPLEX, 1,  cv::Scalar(255, 255, 128), 1, 8, 0);
			
		if (100*score>90)
   	 printf("*******label id %d ********\n",label_id);			
			
	}

	/* 算法有效区域 */
	if(nn_fov_show)
	{
		cv::Rect box_nn;
		box_nn.x = oft_x;
		box_nn.y = oft_y;
		box_nn.width  = dis_w;
		box_nn.height = dis_h;
		cv::rectangle(outgoing_img, box_nn, cv::Scalar(0,0,0xff), 2, 8, 0);
		cv::Point origin_nn;
		origin_nn.x =oft_x;
		origin_nn.y = oft_y + 32;
		cv::putText(outgoing_img, "NCS2 FOV", origin_nn, cv::FONT_HERSHEY_COMPLEX, 1,  cv::Scalar(0,0,0xff), 2, 8, 0);
	}

	Mat showImage;
	/* 缩放显示 */
	resize(outgoing_img,showImage,Size(outgoing_img.cols*scale,outgoing_img.rows*scale),0,0,INTER_LINEAR);
	cv::imshow(name, showImage);
	cv::waitKey(1);
}

void face_detect_handle(cv::Mat& img,CameraInfo *nnparm, char *nnret)
{
	int i,dis_w,dis_h,oft_x, oft_y;
	float x0, x1, y0, y1, score;
	uint16_t* cls_ret = (uint16_t*)nnret;
  

	/* 获取算法的fov */
	oft_x = nnparm->startX;
	oft_y = nnparm->startY;
	dis_w = nnparm->endX - nnparm->startX;
	dis_h = nnparm->endY - nnparm->startY;
	
//image id+label+confidence+x+y+w+h
	//printf("yuv image %d X %d \n",yuvImg.cols,yuvImg.rows);
	
	for (i = 0; i < 100; i++)
	{
		int image_id = (int)(f16Tof32(cls_ret[i*7+0])); 
		if (image_id < 0) {
//			printf("image_id %d, %d\n",image_id, i);
			break;
		}
  //  printf("face id %d \n",image_id);
    score =(float)f16Tof32(cls_ret[i*7+2]);
		x0 = f16Tof32(cls_ret[i*7+3]);
		y0 = f16Tof32(cls_ret[i*7+4]);
		x1 = f16Tof32(cls_ret[i*7+5]);
		y1 = f16Tof32(cls_ret[i*7+6]);
	
	//	printf("score:%.2f<->min:%.2f  rec:(%.3f,%.3f)<->(%.3f,%.3f) \n" ,score,min_fd_score,x0,y0,x1,y1);
		/* 不显示无效数据或者概率太低的框 */
		if( (coordinate_is_valid(x0, y0, x1, y1) ==0 )|| (score < min_fd_score))
		{
			continue;
		}

		/* 画识别的人脸框 */
		cv::Rect box;
		box.x = x0 * dis_w + oft_x;
		box.y = y0 * dis_h + oft_y;
		box.width  = (x1 - x0) * dis_w;
		box.height = (y1 - y0) * dis_h;
		cv::rectangle(img, box, cv::Scalar(255, 128, 128), 2, 8, 0);
		
		cv::Point origin;
		origin.x = box.x ;
		origin.y = box.y + 32;
		char   result[128];
		memset(result, 0, sizeof(result));
		sprintf(result, "score:%d%%", (int)(100*score));
		cv::putText(img, result, origin, cv::FONT_HERSHEY_COMPLEX, 1,  cv::Scalar(255, 255, 128), 1, 8, 0);
	}  	
	
	/* 算法有效区域 */
		if(1)
		{
			cv::Rect box_nn;
			box_nn.x = oft_x;
			box_nn.y = oft_y;
			box_nn.width  = dis_w;
			box_nn.height = dis_h;
			cv::rectangle(img, box_nn, cv::Scalar(0,0,0xff), 2, 8, 0);
			cv::Point origin_nn;
			origin_nn.x =oft_x;
			origin_nn.y = oft_y + 32;
			cv::putText(img, "NCS2 FOV", origin_nn, cv::FONT_HERSHEY_COMPLEX, 1,  cv::Scalar(0,0,0xff), 2, 8, 0);
		}
}	
