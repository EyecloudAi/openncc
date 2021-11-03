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


String nametable[100]={"person","bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat", "traffic light",
                     "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow",
                     "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
                     "skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard",
                     "tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
                     "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "couch",
                     "potted plant", "bed", "dining table", "toilet", "tv", "laptop", "mouse", "remote", "keyboard", "cell phone",
                     "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors", "teddy bear",
                     "hair drier", "toothbrush"};

float nms=0.5f;
 float min_score = 0.1;
void  yolov4_show_img_func(void *data, int w, int h, float scale, char *name, int nn_fov_show, Network1Par *nnParm1, char *nnret)
{
 
    vector<Rect> origin_rect;
    vector<float> origin_rect_cof;
    vector<String> origin_rect_name;

    int classnum=80;
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
    disW = (nnParm1->endX - nnParm1->startX);
    disH = (nnParm1->endY - nnParm1->startY);
    float DisW=(float)disW/(float)nnParm1->imageWidth;
    float DisH=(float)disH/(float)nnParm1->imageHeight;
    float cellW=(float)outgoingImage.cols/416;
    float cellH=(float)outgoingImage.rows/416;




    float anchors[18]={10, 14, 23, 27, 37, 58, 81, 82, 135, 169, 344, 319};



    for (int i = 0; i < 13; i++)
        for (int j = 0; j < 13; j++)
            for(int k=0;k<3;k++)
            {
                float x, y, w, h, conf;


                conf=(float)f16Tof32(detMetadata[k*169*(classnum+5)+4*13*13+i*13+j]);

                if( (conf < min_score))
                {
                    continue;
                }

                x = f16Tof32(detMetadata[k*169*(classnum+5)+0*13*13+i*13+j]);
                y = f16Tof32(detMetadata[k*169*(classnum+5)+1*13*13+i*13+j]);
                w = f16Tof32(detMetadata[k*169*(classnum+5)+2*13*13+i*13+j]);
                h = f16Tof32(detMetadata[k*169*(classnum+5)+3*13*13+i*13+j]);


                float classconf[80];
                int classid=-1;
                double maxclassP=0;
                for(int m=0;m<classnum;m++)
                {
                    classconf[m]=f16Tof32(detMetadata[k*169*(classnum+5)+(5+m)*13*13+i*13+j]);

                    if(classconf[m] > maxclassP){
                        maxclassP = classconf[m];
                        classid=m;
                    }
                }
                if( (maxclassP*conf < min_score))
                {
                    continue;
                }


                x = (x+j)*(416/13);
                y = (y+i)*(416/13);
                int idx=1;

                w=std::exp(w)* anchors[idx * 6 + 2 * k];
                h=std::exp(h)* anchors[idx * 6 + 2 * k+1];


                cv::Rect box;
                box.x = (x-w/2)*cellW;
                box.y =	(y-h/2)*cellH ;
                box.width  = w*cellW;
                box.height = h*cellH;

                origin_rect_name.push_back(nametable[classid]);
                origin_rect.push_back(box);
                origin_rect_cof.push_back(maxclassP*conf);


            }



    for (int i = 0; i < 26; i++)
        for (int j = 0; j < 26; j++)
            for(int k=0;k<3;k++)
            {
                double x, y, w, h, conf;


                conf=(double)f16Tof32(detMetadata[43104+k*676*(classnum+5)+4*26*26+i*26+j]);


                int classid=-1;
                if( (conf < min_score))
                {
                    continue;
                }

                x = f16Tof32(detMetadata[4064+k*676*(classnum+5)+0*26*26+i*26+j]);
                y = f16Tof32(detMetadata[4064+k*676*(classnum+5)+1*26*26+i*26+j]);
                w = f16Tof32(detMetadata[4064+k*676*(classnum+5)+2*26*26+i*26+j]);
                h = f16Tof32(detMetadata[4064+k*676*(classnum+5)+3*26*26+i*26+j]);

                double classconf[80];
                double maxclassP=0;
                for(int m=0;m<classnum;m++)
                {
                    classconf[m]=f16Tof32(detMetadata[4064+k*676*(classnum+5)+(5+m)*26*26+i*26+j]);

                    if(classconf[m] > maxclassP){
                        maxclassP = classconf[m];
                        classid=m;
                    }
                }

                if( (maxclassP*conf < min_score))
                {
                    continue;
                }

                x = (x+ j)*(416/26);
                y = (y + i)*(416/26);
                int idx=0;

                w=std::exp(w)* anchors[idx * 6 + 2 * k];
                h=std::exp(h)* anchors[idx * 6 + 2 * k+1];


                cv::Rect box;
                box.x = (x-w/2)*cellW;
                box.y =	(y-h/2)*cellH ;
                box.width  = w*cellW;
                box.height = h*cellH;
           
                origin_rect_name.push_back(nametable[classid]);
                origin_rect.push_back(box);
                origin_rect_cof.push_back(maxclassP*conf);


            }

    vector<int> final_id;

        constexpr char WIN_NAME[] = "nms";
          if (1) {
              cv::namedWindow(WIN_NAME);
              int initValue = static_cast<int>(nms * 100);
               int initValue2 = static_cast<int>(min_score * 100);
              cv::createTrackbar("nms", WIN_NAME, &initValue, 100,
                  [](int position, void* nmsPtr){*static_cast<float*>(nmsPtr) = position * 0.01f;},
                  &nms);
                  cv::createTrackbar("min_score", WIN_NAME, &initValue2, 100,
                  [](int position, void* min_scorePtr){*static_cast<float*>(min_scorePtr) = position * 0.01f;},
                  &min_score);
              }
    
    dnn::NMSBoxes(origin_rect,origin_rect_cof,min_score,nms,final_id);
    for(int i=0;i<final_id.size();++i)
    {
        Rect resize_rect= origin_rect[final_id[i]];
        float objconf=origin_rect_cof[final_id[i]];
        String name=origin_rect_name[final_id[i]];

        /* 算法有效区域 */
       if(nn_fov_show)
        {
            resize_rect.x = oftX+resize_rect.x*DisW;
            resize_rect.y = oftY+ resize_rect.y *DisH;
            resize_rect.width  *= DisW;
            resize_rect.height *= DisH;
        }


        cv::rectangle(outgoingImage, resize_rect, cv::Scalar(255, 128, 128), 2, 8, 0);
        cv::Point attribute_box1;
        attribute_box1.x =resize_rect.x;
        attribute_box1.y =resize_rect.y+20;

        const char* a = name.c_str();
        cv::putText(outgoingImage,a, attribute_box1, cv::FONT_HERSHEY_COMPLEX, 0.8,  cv::Scalar(0,0,0xff), 1, 8, 0);
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
    if(nn_fov_show)
    {
        cv::Rect boxNN;
        boxNN.x = oftX;
        boxNN.y = oftY;
        boxNN.width  = disW;
        boxNN.height = disH;
        cv::rectangle(outgoingImage, boxNN, cv::Scalar(0,0,0xff), 2, 8, 0);
    }


    Mat showImage;
    /* 缩放显示 */
    resize(outgoingImage,showImage,Size(outgoingImage.cols*scale,outgoingImage.rows*scale),0,0,INTER_LINEAR);
    cv::imshow(name, showImage);
    cv::waitKey(1);
}
