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

void  obj_show_img_func(void *data, int w, int h, float scale, char *name, int nn_fov_show, CameraInfo *nnparm, char *nnret,float min_score)
{
    cv::Mat yuvImg;
    yuvImg.create(h * 3 / 2, w, CV_8UC1);
    cv::Mat yuvImg_resized;
    cv::Mat outgoing_img;
    int i,dis_w,dis_h,oft_x, oft_y;
    float x0, x1, y0, y1, score;
    uint16_t* cls_ret = (uint16_t*)nnret;

    /* YUV420P-->RGB */
    yuvImg.data = (unsigned char*)data;
    cv::cvtColor(yuvImg, outgoing_img, CV_YUV2BGR_I420);

    /* get algorithm fov */
    oft_x = nnparm->startX;
    oft_y = nnparm->startY;
    dis_w = nnparm->endX - nnparm->startX;
    dis_h = nnparm->endY - nnparm->startY;

    for (i = 0; i < 200; i++)
    {
        int image_id = (int)(f16Tof32(cls_ret[i*7+0]));
        int label    = (int)(f16Tof32(cls_ret[i*7+1]));
        score =(float)f16Tof32(cls_ret[i*7+2]);

        if (image_id < 0) {
            break;
        }

        x0 = f16Tof32(cls_ret[i*7+3]);
        y0 = f16Tof32(cls_ret[i*7+4]);
        x1 = f16Tof32(cls_ret[i*7+5]);
        y1 = f16Tof32(cls_ret[i*7+6]);

        /* Not display boxes with invalid data or too low probability */
        if( (coordinate_is_valid(x0, y0, x1, y1) ==0 )|| (score < min_score))
        {
            continue;
        }

        /* Draw recognition frame */
        cv::Rect box;
        box.x = x0 * dis_w + oft_x;
        box.y = y0 * dis_h + oft_y;
        box.width  = (x1 - x0) * dis_w;
        box.height = (y1 - y0) * dis_h;
        cv::rectangle(outgoing_img, box, cv::Scalar(255, 128, 128), 2*w/1920, 8, 0);

        cv::Point origin;
        origin.x = box.x ;
        origin.y = box.y + 32;
        char   result[128];
        memset(result, 0, sizeof(result));
        sprintf(result, "id:%d_score:%d%%", label, (int)(100*score));
        cv::putText(outgoing_img, result, origin, cv::FONT_HERSHEY_COMPLEX, 1,  cv::Scalar(255, 255, 128), 2*w/1920, 8, 0);
    }

    /* Display algorithm effective area */
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
        cv::putText(outgoing_img, "NCC FOV", origin_nn, cv::FONT_HERSHEY_COMPLEX, 1,  cv::Scalar(0,0,0xff), 2, 8, 0);
    }

    Mat showImage;
    /* Zoom display */
    resize(outgoing_img,showImage,Size(outgoing_img.cols*scale,outgoing_img.rows*scale),0,0,INTER_LINEAR);
    cv::imshow(name, showImage);
    cv::waitKey(1);
}
