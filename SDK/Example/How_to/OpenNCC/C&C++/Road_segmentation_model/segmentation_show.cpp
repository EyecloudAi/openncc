#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>
#include <semaphore.h>
#include <fstream>
#include <vector>
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
#include <bits/stdc++.h>
//#include<vector>
using namespace std;
#include <sys/time.h>
using namespace cv;

/////////////////////////////////////////////////////////////////////////////////
/**
 * @class Color
 * @brief A Color class stores channels of a given color
 */
class Color {
private:
    unsigned char _r;
    unsigned char _g;
    unsigned char _b;

public:
    /**
     * A default constructor.
     * @param r - value for red channel
     * @param g - value for green channel
     * @param b - value for blue channel
     */
    Color(unsigned char r,
          unsigned char g,
          unsigned char b) : _r(r), _g(g), _b(b) {}

    inline unsigned char red() const {
        return _r;
    }

    inline unsigned char blue() const {
        return _b;
    }

    inline unsigned char green() const {
        return _g;
    }
};

// Known colors for training classes from the Cityscapes dataset
static  const Color CITYSCAPES_COLORS[] = {
    { 0, 0,  0 },
    { 255, 255,  255 },
    { 70,  70,  70 },
    { 156, 102, 102 },
    { 153, 153, 190 },
    { 153, 153, 153 },
    { 30,  170, 250 },
    { 0,   220, 220 },
    { 35,  142, 107 },
    { 152, 251, 152 },
    { 180, 130, 70 },
    { 60,  20,  220 },
    { 0,   0,   255 },
    { 142, 0,   0 },
    { 70,  0,   0 },
    { 100, 60,  0 },
    { 90,  0,   0 },
    { 230, 0,   0 },
    { 32,  11,  119 },
    { 0,   74,  111 },
    { 81,  0,   81 }
};
template <typename T, std::size_t N>
constexpr std::size_t arraySize(const T (&)[N]) noexcept {
    return N;
}

/////////////////////////////////////////////////////////////////////////////////
float blending = 0.3f;
int num=0;

float getDist_P2L(Point2f pointP, Point2f pointA, Point2f pointB)
{
    //求直线方程
    int A = 0, B = 0, C = 0;
    A = pointA.y - pointB.y;
    B = pointB.x - pointA.x;
    C = pointA.x*pointB.y - pointA.y*pointB.x;
    //代入点到直线距离公式
    float distance = 0;
    distance = ((float)abs(A*pointP.x + B*pointP.y + C)) / ((float)sqrtf(A*A + B*B));
    return distance;
}


void  segmentation_func(void *data, int w, int h, float scale, char *name, int nn_fov_show, CameraInfo *nnParm1, char *nnret,float minConf)
  {
	cv::Mat yuvImg;
	yuvImg.create(h * 3 / 2, w, CV_8UC1);
	cv::Mat yuvImg_resized;
	cv::Mat outgoingImage;
	int i,disW,disH,oftX, oftY;

	int outputSize[16];
    int firstModelOutputSize;
    int secondModelOutputSize;

    /* 获取模型输出size */
    memcpy(outputSize, nnret, sizeof(outputSize));
    firstModelOutputSize        = outputSize[0];
    secondModelOutputSize   = outputSize[1];

    // 获取一级模型memdata偏移地址
    uint16_t* detMetadata          = (uint16_t*)(nnret) ;

	/* YUV420P-->RGB */
	yuvImg.data = (unsigned char*)data;
	cv::cvtColor(yuvImg, outgoingImage, CV_YUV2BGR_I420);
    



    int outWidth  = nnParm1->inputDimWidth;
    int outHeight = nnParm1->inputDimHeight;
    int outChannels = 4;

    cv::resize(outgoingImage, outgoingImage, Size(outWidth,outHeight));
    std::mt19937 rng;
    std::uniform_int_distribution<int> distr(0, 255);
    cv::Mat   resImg, maskImg(outHeight, outWidth,  CV_8UC3);
    std::vector<cv::Vec3b> colors(arraySize(CITYSCAPES_COLORS));
    for (std::size_t i = 0; i < colors.size(); ++i)
        colors[i] = {CITYSCAPES_COLORS[i].blue(), CITYSCAPES_COLORS[i].green(), CITYSCAPES_COLORS[i].red()};

     float  predictions[896*512*4];
    for(int i=0;i<896*512*4;i++)
        predictions[i]=f16Tof32(detMetadata[i]);


	for (int rowId = 0; rowId < outHeight; ++rowId) {
		for (int colId = 0; colId < outWidth; ++colId) {
			std::size_t classId = 0;
			if (outChannels < 2) {  // assume the output is already ArgMax'ed
				classId = static_cast<std::size_t>(predictions[rowId * outWidth + colId]);
			} else {
				float maxProb = -1.0f;
                for (int chId = 0; chId < outChannels; ++chId) {
					float prob = predictions[chId * outHeight * outWidth + rowId * outWidth + colId];               
					if (prob > maxProb) {
						classId = chId;
						maxProb = prob;
					}
				}
			}
			while (classId >= colors.size()) {
				cv::Vec3b color(distr(rng), distr(rng), distr(rng));
				colors.push_back(color);
			}
			maskImg.at<cv::Vec3b>(rowId, colId) = colors[classId];
		}
	}




    Mat maskImgG;
	cv::cvtColor(maskImg, maskImgG, CV_BGR2GRAY);
	
	
	vector<std::vector<cv::Point>> contours;
  	vector<cv::Vec4i> hierarchy;
	cv::findContours(maskImgG, contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	vector<RotatedRect> box(contours.size());
Point2f rect[4];

float maxarea=0;
float angle=0;
float width=0;
float height=0;
Point2f center;
 for(int i=0; i<contours.size(); i++)
   {
	
	 box[i] = cv::minAreaRect(Mat(contours[i]));
	float area=box[i].size.width*box[i].size.height;
	
	if(area>maxarea)
	{
		maxarea=area;
		angle=box[i].angle;
		center=box[i].center;
		width=box[i].size.width;
		height=box[i].size.height;
		box[i].points(rect); 
	}    
}
for(int i=0;i<4;i++)
    for(int j=i;j<3;j++)
        {
            if(rect[i].y<rect[j+1].y)
                {
                    float tempx=0;
                      float tempy=0;
                    tempy=rect[i].y;
                    rect[i].y=rect[j+1].y;
                    rect[j+1].y=tempy;
                      tempx=rect[i].x;
                    rect[i].x=rect[j+1].x;
                    rect[j+1].x=tempx;
                }
        }


Mat dst,dst1,dst2;
dst=Mat::zeros(maskImg.size(),CV_32FC1);

cornerHarris(maskImgG,dst,2,3,0.04,BORDER_DEFAULT);
normalize(dst,dst1,0,255,CV_MINMAX);

convertScaleAbs(dst1,dst2);

vector<cv::Point> top;
for(int i=0;i<dst2.rows;i++)
{
    for(int j=0;j<dst2.cols;j++)
    {
        
        if(dst2.at<uchar>(i,j)>100)
        {
            float dist=getDist_P2L(Point(j,i),rect[2],rect[3]);
    
            if(dist<5)
                    top.push_back(Point(j,i));
                    //circle(image1,Point(j,i),2,Scalar(0,0,255),2);
        }
    }
}

if(!top.empty())
{
float topx=0;
float topy=0;
 for(int i=0; i<top.size(); i++)
   {
       topx+=top[i].x;
        topy+=top[i].y;
   }
Point2f toppoint;
Point2f bottompoint;
toppoint.x=topx/top.size();
toppoint.y=topy/top.size();
circle(maskImg,toppoint,2,Scalar(0,0,255),10);
top.clear();
 //  line(maskImg, rect[2], rect[3], Scalar(255, 255, 255), 2, 8);  //绘制最小外接矩形每条边
bottompoint.x=(rect[0].x+rect[1].x) /2;     
bottompoint.y=(rect[0].y+rect[1].y) /2;     
line(maskImg, toppoint, bottompoint, Scalar(255, 0, 0), 2, 5);

  }
        constexpr char WIN_NAME[] = "segmentation";
          if (1) {
              cv::namedWindow(WIN_NAME);
              int initValue = static_cast<int>(blending * 100);
              cv::createTrackbar("blending", WIN_NAME, &initValue, 100,
                  [](int position, void* blendingPtr){*static_cast<float*>(blendingPtr) = position * 0.01f;},
                  &blending);
              }
		
    
outgoingImage = outgoingImage * blending + maskImg * (1 - blending);

//	Mat showImage;
	/* 缩放显示 */
	cv::imshow(name, outgoingImage);
   
	cv::waitKey(1);
}
