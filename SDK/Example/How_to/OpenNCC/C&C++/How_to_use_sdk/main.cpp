#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mutex>
#include "pthread.h"
#include "sdk.h"
#include "cameraCtrl.h"
#include "Fp16Convert.h"

#ifndef _WINDOWS
#include <sys/time.h>
#else
#include <windows.h>	
extern "C" int gettimeofday(struct timeval *tp, void *tzp);
#pragma warning(disable:4996)
#endif

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui_c.h>
using namespace std;
using namespace cv;

#define  DEMO_FACE_DETECTION  1

extern "C"  void os_sleep(int ms);
static char metadata[1024*1024];
static char yuv420p[sizeof(frameSpecOut)+3840*2160*2];
static volatile int show_update = 0;

extern void  obj_show_img_func(void *data, int w, int h, float scale, char *name, int nn_fov_show, CameraInfo *nnparm, char *nnret,float min_score);
extern void  fd_show_img_func(void *data, int w, int h, float scale, char *name, int nn_fov_show, CameraInfo *nnparm, char *nnret);
extern void  vehicle_license_plate_detection_barrier(void *data, int w, int h, float scale, char *name, int nn_fov_show, Network1Par *nnParm1,Network2Par *nnParm2, char *nnret, float min_score);
///////////////////////////////////////////////////////////////////////////////////

bool g_run = true;
static CameraInfo cam_info =
{
	-1,  //imageWidth
	-1,  //imageHeight                  
    -1,  //startX
	-1,  //startY                 
	-1,  //endX
	-1,  //endY
	0,   //inputDimWidth, if o,auto get from xml
	0,   //inputDimHeight,if o,auto get from xml
	IMG_FORMAT_BGR_PLANAR,      //IMAGE_FORMAT   
	0,                          //meanValue
	0,
	0 ,
	1,                          //stdValue
    1,                          /*Turn on YUV420 output function*/
    0,                          /*Turn on h26x coding function*/
    0,                          /*Turn on MJPEG encoding*/
	ENCODE_H264_MODE,           /* Use H264 encoding format */
};





/***test one AI mode*****/
extern void face_detect_handle(cv::Mat& img,CameraInfo *nnparm, char *nnret);
int main_1net(void)
{
	int ret;
	memset(metadata, 0, sizeof(metadata));

   ret=load_fw("./moviUsbBoot","./fw/OpenNcc.mvcmd");
//   ret=load_fw("./moviUsbBoot",NULL);
   if (ret<0)
   {
			printf("lowd firmware error! return \n");
			return -1;
   }
   
   char version[100];
   char cpuid[100];
   camera_get_ncc_id(cpuid);
   camera_get_fw_version(version);
   printf("Firmvare version: %s id:%s\n",version,cpuid);
   
   get_sdk_version(version);
   printf("sdk sersion:%d %s \n",get_usb_version(),version);
#if 1
 		SensorModesConfig mode[MAX_MODE_SIZE];
		int num=camera_control_get_sensorinfo(mode,MAX_MODE_SIZE);
	  for(int i=0;i<num;i++)
    {
        printf("[%d/%d]camera: %s, %dX%d@%dfps, AFmode:%d, maxEXP:%dus,gain[%d, %d]\n",i,num,
                mode[i].moduleName, mode[i].camWidth, mode[i].camHeight, mode[i].camFps,
                mode[i].AFmode, mode[i].maxEXP, mode[i].minGain, mode[i].maxGain);
    }	    

    int sensorModeId = 0; //1080P
   // int sensorModeId = 1; //4K
    camera_select_sensor(sensorModeId);
   // memcpy(&cameraCfg, &list.mode[sensorModeId], sizeof(cameraCfg));//select camera info

  cam_info.imageWidth  = mode[sensorModeId].camWidth;
  cam_info.imageHeight = mode[sensorModeId].camHeight;
  cam_info.startX      = 0;
  cam_info.startY      = 0;
  cam_info.endX        = mode[sensorModeId].camWidth;
  cam_info.endY        = mode[sensorModeId].camHeight;
#endif
   
  ret = sdk_init(NULL, NULL, "./blob/2020.3/face-detection-retail-0004/face-detection-retail-0004.blob", &cam_info, sizeof(cam_info));
	
  printf("xlink_init %d,mata size=%d\n", ret,get_meta_size());
	if (ret<0) return -1;

  camera_video_out(YUV420p,VIDEO_OUT_CONTINUOUS);
  
	while(g_run)
	{
		char src[64];
		float scale = 960 * 1.0 / mode[sensorModeId].camWidth; 
		int size= sizeof(yuv420p);
		//if (read_yuv_data(yuv420p,&size,1)<0)
		//if (read_jpg_data(yuv420p,&size,1)<0)
		//if ((GetYuvData(yuv420p, size)<0))
		ret=read_yuv_data(yuv420p,&size,1);
		if	(ret==USB_ERROR_NO_DEVICE)
		{
			g_run=false;
			//break;
			exit(-1);
		}
		
		//size = sizeof(metadata);
		ret=read_meta_data(metadata,&size,0);	
		//ret=GetMetaData(metadata,size);
	//	printf("GetMetaData size=%d \n",ret);

#if  1
		cv::Mat yuvImg;
		yuvImg.create(cam_info.imageHeight * 3/2, cam_info.imageWidth, CV_8UC1);	
		cv::Mat rgb_img;
	  
		/* YUV420P-->RGB */
		yuvImg.data = (unsigned char*)yuv420p+sizeof(frameSpecOut);
		cv::cvtColor(yuvImg, rgb_img, CV_YUV2BGR_I420);
		face_detect_handle(rgb_img,&cam_info, (char*)metadata+sizeof(frameSpecOut)+OUTPUT_INDEX_SIZE);
		
		Mat showImage;
		
		resize(rgb_img,showImage,Size(rgb_img.cols*scale,rgb_img.rows*scale),0,0,INTER_LINEAR);
		{
			cv::imshow("rgb", showImage);
			if (cv::waitKey(1)==27) break;
		}
#else
		fd_show_img_func(yuv420p+sizeof(frameSpecOut) , cam_info.imageWidth, cam_info.imageHeight, scale,src, true, &cam_info, (char*)metadata+sizeof(frameSpecOut)+OUTPUT_INDEX_SIZE);
#endif
	}    		 
	printf("exit test main....\n");
	sdk_uninit();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////
int main_2net(void)
{
	/* 1 input param*/
	Network1Par cnn1PrmSet =
	{
	    imageWidth:-1, imageHeight:-1,                     /* dynamic param */
	    startX:-1, startY:-1,                              /* auto get from sensor */
	    endX:-1,endY: -1,                                  /* auto get from sensor*/
	    inputDimWidth:-1, inputDimHeight:-1,               /* input1 param*/
	    inputFormat:IMG_FORMAT_BGR_PLANAR,                 /*  image format */
	    meanValue:{0,0,0},
	    stdValue:1,
	    isOutputYUV:1,                                    /*enable YUV420 output*/
	    isOutputH26X:1,                                   /*enable H26X output*/
	    isOutputJPEG:1,                                   /*enable MJPEG output*/
	    mode:ENCODE_H264_MODE,                            /* use H264 encode param */
	    extInputs:{0},                                    /* for 2 input param with 1 mode */
	    modelCascade:0 ,                                  /* cascade with next AI mode*/
	    inferenceACC:1,
	};
	
	/* 2 input param */
	Network2Par cnn2PrmSet =
	{
	    startXAdj:0,
	    startYAdj:0,
	    endXAdj:0,
	    endYAdj:0,
	    labelMask:{0},                                  
	    minConf: 0.99,                                  
	    inputDimWidth:-1, inputDimHeight:-1,           
	    inputFormat:IMG_FORMAT_BGR_PLANAR,      			
	    meanValue:{0,0,0},
	    stdValue:1,
	    extInputs:{0},                                                  
	    modelCascade:0                                            
	};	
    int ret;
    float conf=0.5;
    CameraInfo cam_info;
    memset(&cam_info, 0, sizeof(cam_info));

   printf(".........test run main....\n"); 
   char version[100];
   get_sdk_version(version);
   printf("sdk sersion:%d %s \n",get_usb_version(),version);
   
    ret = load_fw("./moviUsbBoot","./fw/OpenNcc.mvcmd");
    if (ret < 0)
    {
        printf("lowd firmware error! return \n");
        return -1;
    }
    printf("usb sersion:%d \n", get_usb_version());

    SensorModesConfig cameraCfg;
    SensorModesList list;
    camera_control_get_features(&list);
    printf("list num:%d\n", list.num);
    for (int i = 0; i < list.num; i++)
    {
        SensorModesConfig features;
        memcpy(&features, &list.mode[i], sizeof(features));
        printf("[%d/%d]camera: %s, %dX%d@%dfps, AFmode:%d, maxEXP:%dus,gain[%d, %d]\n",
                i, list.num, features.moduleName, features.camWidth,
                features.camHeight, features.camFps, features.AFmode,
                features.maxEXP, features.minGain, features.maxGain);
    }

    int sensorModeId = 0; //0:1080P, 1:4K
    camera_select_sensor(sensorModeId);
    memcpy(&cameraCfg, &list.mode[sensorModeId], sizeof(cameraCfg)); //select camera info
    cnn1PrmSet.imageWidth  = cameraCfg.camWidth;
    cnn1PrmSet.imageHeight = cameraCfg.camHeight;

    cnn1PrmSet.startX      = 0;
    cnn1PrmSet.startY      = 0;
    cnn1PrmSet.endX        = cameraCfg.camWidth;
    cnn1PrmSet.endY        = cameraCfg.camHeight;
    cnn1PrmSet.inputDimWidth  = 300;
    cnn1PrmSet.inputDimHeight = 300;

    if(cameraCfg.camWidth > 1920)
        cnn1PrmSet.inferenceACC = 0;

    cnn1PrmSet.modelCascade = 1;

     if(cnn1PrmSet.modelCascade !=0)
     {
         /* adjust first net pos for 2 input */
         if(1)
         {
             cnn2PrmSet.startXAdj  = -5;
             cnn2PrmSet.startYAdj  = -5;
             cnn2PrmSet.endXAdj   = 5;
             cnn2PrmSet.endYAdj   = 5;
         }

         cnn2PrmSet.minConf        = 0.7;  // conf for first inputs
         cnn2PrmSet.labelMask[2] = 1;      // car board label id = 2 car label id = 1, id=2

         /*name1: "data" , shape: [1x3x24x94] - An input image in following format [1xCxHxW]. Expected color order is BGR.*/
         cnn2PrmSet.inputDimWidth  = 94;
         cnn2PrmSet.inputDimHeight = 24;
         /*
          *URL:  https://docs.openvinotoolkit.org/2019_R1.1/_license_plate_recognition_barrier_0001_description_license_plate_recognition_barrier_0001.html
          * name2: "seq_ind" , shape: [88,1] - An auxiliary blob that is needed for correct decoding. Set this to [0, 1, 1, ..., 1]. */

         uint16_t *p = (uint16_t *)cnn2PrmSet.extInputs;
         p[0] = (uint16_t)f32Tof16(0);
         for(int i=1;i<88;i++)
         {
             p[i] = (uint16_t)f32Tof16(1.0);
         }
     }

     char *blob =  "./blob/2020.3/vehicle-license-plate-detection-barrier-0106/vehicle-license-plate-detection-barrier-0106.blob";
     char *blob2 = "./blob/2020.3/license-plate-recognition-barrier-0001/license-plate-recognition-barrier-0001.blob";

     conf =  cnn2PrmSet.minConf;

     ret = sdk_net2_init(0,0,\
             blob,  &cnn1PrmSet, sizeof(cnn1PrmSet), \
             blob2,  &cnn2PrmSet, sizeof(cnn2PrmSet));

    camera_video_out(YUV420p,VIDEO_OUT_CONTINUOUS);

    ///////////////////////////////////////////////////////////////////////////////////////////
    char *recvImageData     = (char*) malloc(sizeof(frameSpecOut) + cameraCfg.camWidth * cameraCfg.camHeight * 3 / 2);
    int  meteDataSize  = 5*1024*1024;
    char *recvMetaData = (char*) malloc(meteDataSize);
    if ((recvImageData == 0) || (recvMetaData ==0))
          return -1;
    while (1)
    {
        char src[64];
        frameSpecOut hdr;
        float scale = 960 * 1.0 / cameraCfg.camWidth;
        int maxReadSize;
        char *yuv420p, *metaData;

        maxReadSize  = sizeof(frameSpecOut)+ cameraCfg.camWidth * cameraCfg.camHeight * 3 / 2;
        if (read_yuv_data(recvImageData, &maxReadSize, 1) < 0)
        {
            break;
        }
       // printf("recv yuv len=%d ok!\n",maxReadSize);
        memcpy(&hdr, recvImageData, sizeof(frameSpecOut));

        maxReadSize = meteDataSize;
        if (read_meta_data(recvMetaData, &maxReadSize, 0) == 0)
        {
            memcpy(&hdr, recvMetaData, sizeof(frameSpecOut));
        }

        yuv420p  = (char*) recvImageData + sizeof(frameSpecOut);
        metaData = (char*) recvMetaData + sizeof(frameSpecOut);

        sprintf(src, "demo_video_%dx%d@%dfps(scale:%d%%)", cameraCfg.camWidth, cameraCfg.camHeight, cameraCfg.camFps,(int)(100*scale));
        vehicle_license_plate_detection_barrier(yuv420p , cameraCfg.camWidth, cameraCfg.camHeight, scale,src, 0, &cnn1PrmSet,&cnn2PrmSet, metaData, conf);
    }
    printf("exit test main....\n");
    sdk_uninit();
    return 0;
}

int main_2input(void)
{
    int ret;
    float conf=0.5;

		Network1Par cnn1PrmSet =
		{
		    imageWidth:-1, imageHeight:-1,                     
		    startX:-1, startY:-1,                                          
		    endX:-1,endY: -1,                                            
		    inputDimWidth:-1, inputDimHeight:-1,          
		    inputFormat:IMG_FORMAT_BGR_PLANAR,     
		    meanValue:{0,0,0},
		    stdValue:1,
		    isOutputYUV:1,                                              
		    isOutputH26X:1,                                          
		    isOutputJPEG:1,                                            
		    mode:ENCODE_H264_MODE,                          
		    extInputs:{0},                                                 
		    modelCascade:0 ,                                       
		    inferenceACC:0,
		};
    
   char version[100];
   get_sdk_version(version);
   printf("sdk sersion:%d %s \n",get_usb_version(),version);
   
    ret = load_fw("./moviUsbBoot","./fw/OpenNcc.mvcmd");
    if (ret < 0)
    {
        printf("lowd firmware error! return \n");
        return -1;
    }
    printf("usb sersion:%d \n", get_usb_version());

    SensorModesConfig cameraCfg;
    SensorModesList list;
    camera_control_get_features(&list);
    printf("list num:%d\n", list.num);
    for (int i = 0; i < list.num; i++)
    {
        SensorModesConfig features;
        memcpy(&features, &list.mode[i], sizeof(features));
        printf("[%d/%d]camera: %s, %dX%d@%dfps, AFmode:%d, maxEXP:%dus,gain[%d, %d]\n",
                i, list.num, features.moduleName, features.camWidth,
                features.camHeight, features.camFps, features.AFmode,
                features.maxEXP, features.minGain, features.maxGain);
    }

    int sensorModeId = 0; //0:1080P, 1:4K
    camera_select_sensor(sensorModeId);
    memcpy(&cameraCfg, &list.mode[sensorModeId], sizeof(cameraCfg)); //select camera info

     cnn1PrmSet.imageWidth  = cameraCfg.camWidth;
     cnn1PrmSet.imageHeight = cameraCfg.camHeight;

     cnn1PrmSet.startX      = 0;
     cnn1PrmSet.startY      = 0;
     cnn1PrmSet.endX        = cameraCfg.camWidth;
     cnn1PrmSet.endY        = cameraCfg.camHeight;
     cnn1PrmSet.inputDimWidth  = 992;
     cnn1PrmSet.inputDimHeight = 544;

      /* https://docs.openvinotoolkit.org/2019_R1.1/person-detection-retail-0002.html */
      /*input1 name: data , shape: [1x3x544x992] - An input image in following format [1xCxHxW]. The expected channel order is BGR.
        input2  name: im_info, shape: [1x6] - An image information [544, 992, 992/frame_width, 544/frame_height, 992/frame_width, 544/frame_height] */
      uint16_t *p = (uint16_t *)cnn1PrmSet.extInputs;
      int oft=0;
      p[oft++] = f32Tof16(554);
      p[oft++] = f32Tof16(992);
      p[oft++] = f32Tof16(1.0*992/(cnn1PrmSet.endX -cnn1PrmSet.startX ));
      p[oft++] = f32Tof16(1.0*544/(cnn1PrmSet.endY -cnn1PrmSet.startY ));
      p[oft++] = f32Tof16(1.0*992/(cnn1PrmSet.endX -cnn1PrmSet.startX ));
      p[oft++] = f32Tof16(1.0*544/(cnn1PrmSet.endY -cnn1PrmSet.startY ));

      char *blob  = "./blob/2020.3/person-detection-retail-0002/person-detection-retail-0002.blob";
    
      conf = 0.4;

     ret = sdk_net2_init(0,0,\
             blob,  &cnn1PrmSet, sizeof(cnn1PrmSet), \
             NULL,  NULL, 0);

    camera_video_out(YUV420p,VIDEO_OUT_CONTINUOUS);

    char *recvImageData     = (char*) malloc(sizeof(frameSpecOut) + cameraCfg.camWidth * cameraCfg.camHeight * 3 / 2);
    int  meteDataSize  = 5*1024*1024;
    char *recvMetaData = (char*) malloc(meteDataSize);
    if ((recvImageData == 0) || (recvMetaData ==0))
          return -1;
    while (1)
    {
        char src[64];
        frameSpecOut hdr;
        float scale = 960 * 1.0 / cameraCfg.camWidth;
        int maxReadSize;
        char *yuv420p, *metaData;

        maxReadSize  = sizeof(frameSpecOut)+ cameraCfg.camWidth * cameraCfg.camHeight * 3 / 2;
        if (read_yuv_data(recvImageData, &maxReadSize, 1) < 0)
        {
            break;
        }
        memcpy(&hdr, recvImageData, sizeof(frameSpecOut));

        maxReadSize = meteDataSize;
        if (read_meta_data(recvMetaData, &maxReadSize, false) == 0)
        {
            memcpy(&hdr, recvMetaData, sizeof(frameSpecOut));
        }

        yuv420p  = (char*) recvImageData + sizeof(frameSpecOut);
        metaData = (char*) recvMetaData + sizeof(frameSpecOut)+OUTPUT_INDEX_SIZE;

        sprintf(src, "demo_video_%dx%d@%dfps(scale:%d%%)", cameraCfg.camWidth, cameraCfg.camHeight, cameraCfg.camFps,(int)(100*scale));
      //  void  obj_show_img_func(void *data, int w, int h, float scale, char *name, int nn_fov_show, CameraInfo *nnparm, char *nnret,float min_score)
        obj_show_img_func(yuv420p , cameraCfg.camWidth, cameraCfg.camHeight, scale,src, 0, (CameraInfo*)&cnn1PrmSet, metaData, conf);
    }
    printf("exit test main....\n");
    sdk_uninit();
    return 0;
}
const char* test_example[]=
{
	"one ai net",
	"two ai net",
	"1net 2 input",
	"use call back"
};

int main(int argc ,char *argv[])
{
    int nselect =0;
	if (argc>1)
	  	nselect=atoi(argv[1]);
	printf("test example:%s \n",test_example[nselect]);  	
	if  	(nselect == 0)
	 	return main_1net();
	else  if  (nselect == 1)
	  return main_2net();
	else  if  (nselect == 2)
	  return main_2input();	  
	else  
		printf("no select example!\n");

	return 0;	  	  
}

