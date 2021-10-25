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
#include <opencv2/dnn.hpp>
#include "sdk.h"
#include "Fp16Convert.h"
#include <bits/stdc++.h>
//#include<vector>
using namespace std;
#include <sys/time.h>
using namespace cv;


String classname[100]={"person","bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat", "traffic light",
                     "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow",
                     "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
                     "skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard",
                     "tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
                     "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "couch",
                     "potted plant", "bed", "dining table", "toilet", "tv", "laptop", "mouse", "remote", "keyboard", "cell phone",
                     "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors", "teddy bear",
                     "hair drier", "toothbrush"};



double sigmoid(double x){
    return (1 / (1 + exp(-x)));
}
void  yolov5_show_img_func(void *data, int w, int h, float scale, char *name, int nn_fov_show, Network1Par *nnParm1, char *nnret,float minConf)
{

    vector<Rect> origin_rect;
    vector<float> origin_rect_cof;
    vector<String> origin_rect_name;

    cv::Mat yuvImg;
    yuvImg.create(h * 3 / 2, w, CV_8UC1);
    cv::Mat outgoingImage;
    int i,disW,oftX,disH, oftY;
    uint16_t* detMetadata = (uint16_t*)(nnret);

    /* YUV420P-->RGB */
    yuvImg.data = (unsigned char*)data;
    cv::cvtColor(yuvImg, outgoingImage, CV_YUV2BGR_I420);
    /* YUV420P-->RGB */
    yuvImg.data = (unsigned char*)data;
    cv::cvtColor(yuvImg, outgoingImage, CV_YUV2BGR_I420);

    /* 获取算法的fov */
    oftX = nnParm1->startX;
    oftY = nnParm1->startY;
    disW = nnParm1->endX - nnParm1->startX;
    disH = nnParm1->endY - nnParm1->startY;

    float cellW=(float)outgoingImage.cols/640;
    float cellH=(float)outgoingImage.rows/640;


    float anchors[18]={10.0, 13.0, 16.0, 30.0, 33.0, 23.0, 30.0, 61.0, 62.0, 45.0, 59.0, 119.0, 116.0, 90.0, 156.0,198.0,373.0, 326.0};


    for(int k=0;k<3;k++)
        for (int i = 0; i < 80; i++)
            for (int j = 0; j < 80; j++)

            {
                float x, y, w, h, conf;


                conf=(float)f16Tof32(detMetadata[k*544000+i*80*85+j*85+4]);

                conf = sigmoid(conf);

                if( (conf < minConf))
                {
                    continue;
                }

                x = f16Tof32(detMetadata[k*544000+i*80*85+j*85+0]);
                y = f16Tof32(detMetadata[k*544000+i*80*85+j*85+1]);
                w = f16Tof32(detMetadata[k*544000+i*80*85+j*85+2]);
                h = f16Tof32(detMetadata[k*544000+i*80*85+j*85+3]);


                float classconf[80];
                int classid=-1;
                double maxclassP=0;
                for(int m=0;m<80;m++)
                {
                    classconf[m]=f16Tof32(detMetadata[k*544000+i*80*85+j*85+5+m]);

                    classconf[m] = sigmoid(classconf[m]);

                    if(classconf[m] > maxclassP){
                        maxclassP = classconf[m];
                        classid=m;
                    }
                }
                if( (maxclassP*conf < minConf))
                {
                    continue;
                }


                x = (2*sigmoid(x) - 0.5 + j)*(640/80);
                y = (2*sigmoid(y) - 0.5 + i)*(640/80);
                int idx=0;

                w=pow(sigmoid(w)*2,2)* anchors[idx * 6 + 2 * k];
                h=pow(sigmoid(h)*2,2)* anchors[idx * 6 + 2 * k+1];


                cv::Rect box;
                box.x = (x-w/2)*cellW;
                box.y =	(y-h/2)*cellH ;
                box.width  = w*cellW;
                box.height = h*cellH;
                origin_rect.push_back(box);
                origin_rect_cof.push_back(maxclassP*conf);
               origin_rect_name.push_back(classname[classid]);
            }


    for(int k=0;k<3;k++)
        for (int i = 0; i < 40; i++)
            for (int j = 0; j < 40; j++)
            {
                double x, y, w, h, conf;


                conf=(double)f16Tof32(detMetadata[1632000+k*136000+i*40*85+j*85+4]);

                conf = sigmoid(conf);
                int classid=-1;
                if( (conf < minConf))
                {
                    continue;
                }

                x = f16Tof32(detMetadata[1632000+k*136000+i*40*85+j*85+0]);
                y = f16Tof32(detMetadata[1632000+k*136000+i*40*85+j*85+1]);
                w = f16Tof32(detMetadata[1632000+k*136000+i*40*85+j*85+2]);
                h = f16Tof32(detMetadata[1632000+k*136000+i*40*85+j*85+3]);

                double classconf[80];
                double maxclassP=0;
                for(int m=0;m<80;m++)
                {
                    classconf[m]=f16Tof32(detMetadata[1632000+k*136000+i*40*85+j*85+5+m]);
                     classconf[m] = sigmoid(classconf[m]);
                    if(classconf[m] > maxclassP){
                        maxclassP = classconf[m];
                        classid=m;
                    }
                }
                if( (maxclassP*conf < minConf))
                {
                    continue;
                }

                x = (2*sigmoid(x) - 0.5 + j)*(640/40);
                y = (2*sigmoid(y) - 0.5 + i)*(640/40);
                int idx=1;

                w=pow(sigmoid(w)*2,2)* anchors[idx * 6 + 2 * k];
                h=pow(sigmoid(h)*2,2)* anchors[idx * 6 + 2 * k+1];


                cv::Rect box;
                box.x = (x-w/2)*cellW;
                box.y =	(y-h/2)*cellH ;
                box.width  = w*cellW;
                box.height = h*cellH;
                origin_rect.push_back(box);
                origin_rect_cof.push_back(maxclassP*conf);
                origin_rect_name.push_back(classname[classid]);
            }


    for(int k=0;k<3;k++)
        for (int i = 0; i < 20; i++)
            for (int j = 0; j < 20; j++)

            {
                double x, y, w, h, conf;

                conf=(float)f16Tof32(detMetadata[2040000+k*34000+i*20*85+j*85+4]);

                conf = sigmoid(conf);

                if( (conf < minConf))
                {
                    continue;
                }

                x = f16Tof32(detMetadata[2040000+k*34000+i*20*85+j*85+0]);
                y = f16Tof32(detMetadata[2040000+k*34000+i*20*85+j*85+1]);
                w = f16Tof32(detMetadata[2040000+k*34000+i*20*85+j*85+2]);
                h = f16Tof32(detMetadata[2040000+k*34000+i*20*85+j*85+3]);

                double classconf[80];
                double maxclassP=0;
                int classid=-1;
                for(int m=0;m<80;m++)
                {
                    classconf[m]=f16Tof32(detMetadata[2040000+k*34000+i*20*85+j*85+5+m]);

                    classconf[m] = sigmoid(classconf[m]);

                    if(classconf[m] > maxclassP){
                        maxclassP = classconf[m];
                        classid=m;
                    }
                }
                if( (maxclassP*conf < minConf))
                {
                    continue;
                }


                x = (2*sigmoid(x) - 0.5 + j)*(640/20);
                y = (2*sigmoid(y) - 0.5 + i)*(640/20);
                int idx=2;

                w=pow(sigmoid(w)*2,2)* anchors[idx * 6 + 2 * k];
                h=pow(sigmoid(h)*2,2)* anchors[idx * 6 + 2 * k+1];


                cv::Rect box;
                box.x = (x-w/2)*cellW;
                box.y =	(y-h/2)*cellH ;
                box.width  = w*cellW;
                box.height = h*cellH;
                origin_rect.push_back(box);
                origin_rect_cof.push_back(maxclassP*conf);
                origin_rect_name.push_back(classname[classid]);
            }

    vector<int> final_id;
    dnn::NMSBoxes(origin_rect,origin_rect_cof,minConf,0.5,final_id);
    for(int i=0;i<final_id.size();++i)
    {
        Rect resize_rect= origin_rect[final_id[i]];
        float objconf=origin_rect_cof[final_id[i]];
        String name=origin_rect_name[final_id[i]];
        //qDebug()<<resize_rect.x<<i<<endl;
        cv::rectangle(outgoingImage, resize_rect, cv::Scalar(255, 128, 128), 2, 8, 0);
        cv::Point attribute_box1;
        attribute_box1.x =resize_rect.x;
        attribute_box1.y =resize_rect.y+20;
    
        const char* a = name.c_str();
        cv::putText(outgoingImage,a, attribute_box1, cv::FONT_HERSHEY_COMPLEX, 0.8,  cv::Scalar(0,0,0xff), 0.6, 8, 0);
        cv::Point attribute_box2;
        attribute_box2.x =resize_rect.x;
        attribute_box2.y =resize_rect.y+40;
        char b[20];
        sprintf(b, "%4.2f", objconf);
        cv::putText(outgoingImage,b , attribute_box2, cv::FONT_HERSHEY_COMPLEX, 0.8,  cv::Scalar(0,0,0xff), 0.6, 8, 0);
    }

    vector<int>().swap(final_id);
    vector<Rect>().swap(origin_rect);
    vector<float>().swap(origin_rect_cof);
    vector<String>().swap(origin_rect_name);
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
        cv::putText(outgoingImage, "NCS2 FOV", originNN, cv::FONT_HERSHEY_COMPLEX, 1,  cv::Scalar(0,0,0xff), 2, 8, 0);
    }


    Mat showImage;
    /* 缩放显示 */
    resize(outgoingImage,showImage,Size(outgoingImage.cols*scale,outgoingImage.rows*scale),0,0,INTER_LINEAR);
    cv::imshow(name, showImage);
    cv::waitKey(1);
}
