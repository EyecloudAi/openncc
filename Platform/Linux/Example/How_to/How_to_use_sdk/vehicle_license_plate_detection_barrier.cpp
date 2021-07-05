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
#if 0
Metadata Output Parsing:
|---------------|
| 16* int |                   ///////// outputSize Index
|---------------|
| outputSize[0]|          /////////////model_0 output, outputSize[0]
| |
|---------------|
| outputSize[1] |         /////////////model_1 output,outputSize[1]
| |
|---------------|
| outputSize[1] |
        .
        .
        .                         MAX_OBJ=100
        .
        .
| outputSize[1] |        /////////////model_1 output,outputSize[1]
| |
|---------------|
#endif
void  vehicle_license_plate_detection_barrier(void *data, int w, int h, float scale, char *name, int nn_fov_show, Network1Par *nnParm1,Network2Par *nnParm2, char *nnret, float min_score)
{
	cv::Mat yuvImg;
	yuvImg.create(h * 3 / 2, w, CV_8UC1);
	cv::Mat outgoingImage;
	int i,disW,disH,oftX, oftY;
	int outputSize[16];
    int firstModelOutputSize;
    int secondModelOutputSize;

	/* 获取模型输出size */
	memcpy(outputSize, nnret, sizeof(outputSize));
	firstModelOutputSize        = outputSize[0];
	secondModelOutputSize   = outputSize[1];
//	printf("oft %d %d\n",  outputSize[0],  outputSize[1]);

    // 获取一级模型memdata偏移地址
	uint16_t* detMetadata          = (uint16_t*)((char*)nnret +sizeof(outputSize)) ;
    // 获取二级模型memdata偏移地址
	char * firstOutput  = (char*)nnret +sizeof(outputSize) + firstModelOutputSize;

	/* YUV420P-->RGB */
	yuvImg.data = (unsigned char*)data;
	cv::cvtColor(yuvImg, outgoingImage, CV_YUV2BGR_I420);

	/* 获取算法的fov */
	oftX = nnParm1->startX;
	oftY = nnParm1->startY;
	disW = nnParm1->endX - nnParm1->startX;
	disH = nnParm1->endY - nnParm1->startY;

	for (i = 0; i < MAX_OBJ; i++)
	{
		int    image_id  = (int)    f16Tof32(detMetadata[i*7+0]);
		int    label        = (int)    f16Tof32(detMetadata[i*7+1]);
        float conf        =  (float)f16Tof32(detMetadata[i*7+2]);
        float x0, x1, y0, y1;

		if (image_id < 0) {
//			printf("image_id %d, %d\n",image_id, i);
			break;
		}

		x0 = f16Tof32(detMetadata[i*7+3]);
		y0 = f16Tof32(detMetadata[i*7+4]);
		x1 = f16Tof32(detMetadata[i*7+5]);
		y1 = f16Tof32(detMetadata[i*7+6]);

		/* 不处理无效数据，
		 * 低于执行度，
		 * 无效label的数据 */
        if(  (coordinate_is_valid(x0, y0, x1, y1) ==0 )\
            ||(conf<nnParm2->minConf)\
            ||(nnParm2->labelMask[label] == 0)
           )
		{
			continue;
		}

        /* 画识别的框 */
        cv::Rect box;
        box.x = x0 * disW + oftX;
        box.y = y0 * disH  + oftY ;
        box.width  = (x1 - x0) * disW;
        box.height = (y1 - y0) * disH;
        cv::rectangle(outgoingImage, box, cv::Scalar(255, 128, 128), 2, 8, 0);

//        printf("x1,y1,x2,y2:%d %d %d %d\n",box.x,box.y,box.width,box.height );

        int regRet[88];

        // 获取二级模型memdata
        uint16_t* regMetadata = (uint16_t*)(firstOutput + secondModelOutputSize * i);
        //            printf("\n\nret:");
        for(int j=0;j<sizeof(regRet)/sizeof(regRet[0]);j++)
        {
            regRet[j]= (int) f16Tof32(regMetadata[j]);
        //                printf("%d ",regRet[j] );
        }
        //            printf(" \n");

        char items[][20] = {
                       "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                       "<Anhui>", "<Beijing>", "<Chongqing>", "<Fujian>",
                       "<Gansu>", "<Guangdong>", "<Guangxi>", "<Guizhou>",
                       "<Hainan>", "<Hebei>", "<Heilongjiang>", "<Henan>",
                       "<HongKong>", "<Hubei>", "<Hunan>", "<InnerMongolia>",
                       "<Jiangsu>", "<Jiangxi>", "<Jilin>", "<Liaoning>",
                       "<Macau>", "<Ningxia>", "<Qinghai>", "<Shaanxi>",
                       "<Shandong>", "<Shanghai>", "<Shanxi>", "<Sichuan>",
                       "<Tianjin>", "<Tibet>", "<Xinjiang>", "<Yunnan>",
                       "<Zhejiang>", "<police>",
                       "A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
                       "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
                       "U", "V", "W", "X", "Y", "Z"
               };

        char result[256]={0};

        for(int j=0;j<sizeof(regRet)/sizeof(regRet[0]);j++)
        {
                if (regRet[j] == -1)
                    break;

                strcat(result, items[(int)regRet[j]]);
          }
        printf("str:%s\n",result);
        cv::Point origin;
        origin.x = box.x;
        origin.y = box.y-20 ;
        cv::putText(outgoingImage, result, origin, cv::FONT_HERSHEY_COMPLEX, 1.1,  cv::Scalar(0, 0, 255), 2, 8, 0);
	}

	/* 算法有效区域 */
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
		cv::putText(outgoingImage, "ALG FOV", originNN, cv::FONT_HERSHEY_COMPLEX, 1,  cv::Scalar(0,0,0xff), 2, 8, 0);
	}

	Mat showImage;
	/* 缩放显示 */
	resize(outgoingImage,showImage,Size(outgoingImage.cols*scale,outgoingImage.rows*scale),0,0,INTER_LINEAR);
	cv::imshow(name, showImage);
	cv::waitKey(1);
}
