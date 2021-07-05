#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>
#include <semaphore.h>
#include <fstream>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui_c.h>

#include "sdk.h"
#include "Fp16Convert.h"

//#include<vector>
using namespace std;
#include <sys/time.h>
using namespace cv;

typedef struct{
int id;
char type[100];
int  clolors;
}t_cls_type;

static t_cls_type cls_type[]=
{
		{1,	    "person", 0x0000ff},
		{17,	"cat",    0xffff00},
		{18,	"dog",    0x00ffff},
		{44,	"bottle", 0xff00ff},
		{47,	"cup",    0x800000},
		{48,	"fork",   0x00ff00},
		{49,	"knife",  0x00ff00},
		{50,	"spoon",  0x00ff00},
		{51,	"bowl",   0x00ff00},
		{52,	"banana", 0xffff00},
		{53,	"apple" , 0xff0000},
		{55,	"orange", 0x00ff00},
		{62,	"chair",  0xffff00},
		{64,	"plant", 0xffff00},
		{67,	"table", 0xffff00},
		{77,	"cell phone",0xff0000},
		{78,	"microwave", 0x808000},
		{80,	"toaster", 0x808000},
		{84,	"book", 0x800080},
		{85,	"clock", 0x800080},
		{86,	"vase", 0x800080},
		{87,	"scissors", 0x800080},
		{90,	"toothbrush", 0x000080}
};

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

void  cls_show_img_func(void *data, int w, int h, float scale, char *name, int nn_fov_show, Network1Par *nnparm, Network2Par *nnParm2, char *nnret,float min_score)
{
	cv::Mat yuvImg;
	yuvImg.create(h * 3 / 2, w, CV_8UC1);
	cv::Mat outgoing_img;
	int i,dis_w,dis_h,oft_x, oft_y;
	float x0, x1, y0, y1, score;

	/* YUV420P-->RGB */
	yuvImg.data = (unsigned char*)data;
	cv::cvtColor(yuvImg, outgoing_img, CV_YUV2BGR_I420);

    int outputSize[16];
    int firstModelOutputSize;
    int secondModelOutputSize;

    /* get model output size */
    memcpy(outputSize, nnret, sizeof(outputSize));
    firstModelOutputSize        = outputSize[0];
    secondModelOutputSize   = outputSize[1];

    // get the memdata offset address of the first-level model
    uint16_t* detMetadata          = (uint16_t*)((char*)nnret +sizeof(outputSize)) ;
    // get the memdata offset address of the second-level model
    char * firstOutput  = (char*)nnret +sizeof(outputSize) + firstModelOutputSize;

    uint16_t* cls_ret = (uint16_t*)detMetadata;

	oft_x = nnparm->startX;
	oft_y = nnparm->startY;
	dis_w = nnparm->endX - nnparm->startX;
	dis_h = nnparm->endY - nnparm->startY;

	for (i = 0; i < 100; i++)
	{
		int image_id = (int)(f16Tof32(cls_ret[i*7+0]));
		if (image_id < 0) {
			break;
		}

		x0 = f16Tof32(cls_ret[i*7+3]);
		y0 = f16Tof32(cls_ret[i*7+4]);
		x1 = f16Tof32(cls_ret[i*7+5]);
		y1 = f16Tof32(cls_ret[i*7+6]);

		score =(float)f16Tof32(cls_ret[i*7+2]);
		/* Invalid data, no frame */
		if( (coordinate_is_valid(x0, y0, x1, y1) ==0 )|| (score < min_score))
		{
			continue;
		}

		/* Determine whether to identify the object */
		int id =-1;
		int cls_id = (int)(f16Tof32(cls_ret[i*7+1]));
		for (int index = 0; index < sizeof(cls_type) / sizeof(cls_type[0]);index++)
		{
			if (cls_type[index].id == cls_id)
			{
				id = index;
				break;
			}
		}
		/* Object is not effectively recognized */
		if (id == -1)
		{
			continue;
		}

		/* Draw the recognized object frame */
		cv::Rect box;
		box.x = x0 * dis_w + oft_x;
		box.y = y0 * dis_h + oft_y;
		box.width  = (x1 - x0) * dis_w;
		box.height = (y1 - y0) * dis_h;
		cv::rectangle(outgoing_img, box, cv::Scalar(cls_type[id].clolors&0xff,(cls_type[id].clolors&0xff00)>>8,(cls_type[id].clolors&0xff0000)>>16), 2, 8, 0);

		cv::Point origin;
		origin.x = box.x ;
		origin.y = box.y + 32;
		char   result[128];
		memset(result, 0, sizeof(result));
		sprintf(result, "%s:%d%%",cls_type[id].type, (int)(100*score));
		cv::putText(outgoing_img, result, origin, cv::FONT_HERSHEY_COMPLEX, 1,  cv::Scalar(cls_type[id].clolors&0xff,(cls_type[id].clolors&0xff00)>>8,(cls_type[id].clolors&0xff0000)>>16), 1, 8, 0);
	}

	/* Algorithm effective area */
	if(nn_fov_show)
	{
		cv::Rect box_nn;
		box_nn.x = oft_x;
		box_nn.y = oft_y;
		box_nn.width  = dis_w;
		box_nn.height = dis_h;
		cv::rectangle(outgoing_img, box_nn,  cv::Scalar(0,0,0xff), 2, 8, 0);

		cv::Point origin_nn;
		origin_nn.x =oft_x;
		origin_nn.y =oft_y +32;
		cv::putText(outgoing_img, "NCS2 FOV", origin_nn, cv::FONT_HERSHEY_COMPLEX, 1,  cv::Scalar(0,0,0xff), 2, 8, 0);
	}

	Mat showImage;
	/* Zoom display */
	resize(outgoing_img,showImage,Size(outgoing_img.cols*scale,outgoing_img.rows*scale),0,0,INTER_LINEAR);
	cv::imshow(name, showImage);

	cv::waitKey(1);
}
