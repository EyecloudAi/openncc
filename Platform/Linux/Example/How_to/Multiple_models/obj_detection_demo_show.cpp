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

void  obj_show_img_func(void *data, int w, int h, float scale, char *name, int nn_fov_show, Network1Par *nnParm1, Network2Par *nnParm2, char *nnret,float minConf)
{
	cv::Mat yuvImg;
	yuvImg.create(h * 3 / 2, w, CV_8UC1);
	cv::Mat yuvImg_resized;
	cv::Mat outgoingImage;
	int i,disW,disH,oftX, oftY;

	int outputSize[16];
    int firstModelOutputSize;
    int secondModelOutputSize;

    /* Get model output size */
    memcpy(outputSize, nnret, sizeof(outputSize));
    firstModelOutputSize        = outputSize[0];
    secondModelOutputSize   = outputSize[1];

    // Get the offset address of the first-level model memdata 
    uint16_t* detMetadata          = (uint16_t*)((char*)nnret +sizeof(outputSize)) ;
    // Get the offset address of the secondary model memdata
    char * firstOutput  = (char*)nnret +sizeof(outputSize) + firstModelOutputSize;
	/* YUV420P-->RGB */
	yuvImg.data = (unsigned char*)data;
	cv::cvtColor(yuvImg, outgoingImage, CV_YUV2BGR_I420);

	/* get Algorithm  fov */
	oftX = nnParm1->startX;
	oftY = nnParm1->startY;
	disW = nnParm1->endX - nnParm1->startX;
	disH = nnParm1->endY - nnParm1->startY;

	for (i = 0; i < 200; i++)
	{
	    float x0, x1, y0, y1, conf;

		int image_id = (int)(f16Tof32(detMetadata[i*7+0]));
		int label        = (int)(f16Tof32(detMetadata[i*7+1]));
        conf              =(float)f16Tof32(detMetadata[i*7+2]);

		if (image_id < 0) {
//			printf("image_id %d, %d\n",image_id, i);
			break;
		}

		x0 = f16Tof32(detMetadata[i*7+3]);
		y0 = f16Tof32(detMetadata[i*7+4]);
		x1 = f16Tof32(detMetadata[i*7+5]);
		y1 = f16Tof32(detMetadata[i*7+6]);
		/* Do not display boxes with invalid data or too low probability */
        if( (coordinate_is_valid(x0, y0, x1, y1) ==0 )|| (conf < minConf))
		{
			continue;
		}

		/* Draw the recognized face frame */
		cv::Rect box;
		box.x = x0 * disW + oftX;
		box.y = y0 * disH + oftY;
		box.width  = (x1 - x0) * disW;
		box.height = (y1 - y0) * disH;
		cv::rectangle(outgoingImage, box, cv::Scalar(255, 128, 128), 2, 8, 0);

		cv::Point origin;
		origin.x = box.x ;
		origin.y = box.y + 32;
		char   result[128];
		memset(result, 0, sizeof(result));
		sprintf(result, "id:%d_score:%d%%", label, (int)(100*conf));
		cv::putText(outgoingImage, result, origin, cv::FONT_HERSHEY_COMPLEX, 1,  cv::Scalar(255, 255, 128), 1, 8, 0);
	}

	/* Algorithm effective area */
	if(nn_fov_show)
	{
		cv::Rect boxNN;
		boxNN.x = oftX;
		boxNN.y = oftY;
		boxNN.width  = disW;
		boxNN.height = disH;
		cv::rectangle(outgoingImage, boxNN, cv::Scalar(0,0,0xff), 2, 8, 0);
		cv::Point originNN;
		originNN.x =oftX;
		originNN.y = oftY + 32;
		cv::putText(outgoingImage, "NCS2 FOV", originNN, cv::FONT_HERSHEY_COMPLEX, 1,  cv::Scalar(0,0,0xff), 2, 8, 0);
	}

	Mat showImage;
	/* Zoom display */
	resize(outgoingImage,showImage,Size(outgoingImage.cols*scale,outgoingImage.rows*scale),0,0,INTER_LINEAR);
	cv::imshow(name, showImage);
	cv::waitKey(1);
}
