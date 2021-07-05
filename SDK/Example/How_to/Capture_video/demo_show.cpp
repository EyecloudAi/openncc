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

using namespace std;
#include <sys/time.h>
using namespace cv;


void  opencv_show_img_func(void *data, int w, int h, float scale, char *name)
{
    cv::Mat yuvImg;
    yuvImg.create(h * 3 / 2, w, CV_8UC1);
    cv::Mat yuvImg_resized;
    cv::Mat outgoing_img;

    /* YUV420P-->RGB */
    yuvImg.data = (unsigned char*)data;
    cv::cvtColor(yuvImg, outgoing_img, CV_YUV2BGR_I420);

    Mat showImage;
    /* Zoom display */
    resize(outgoing_img,showImage,Size(outgoing_img.cols*scale,outgoing_img.rows*scale),0,0,INTER_LINEAR);
    cv::imshow(name, showImage);
    cv::waitKey(1);
}
