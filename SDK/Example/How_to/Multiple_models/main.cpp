
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#include "pthread.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
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


////////////////////////////////////////////////////////////////////////

#define     TEST_FACE_DETCTION_ADAS_0001                                0
#define     TEST_FACE_DETCTION_RETAIL_0004                              1
#define     TEST_LICENSE_PLATE_RECOGNITION_BARRIER_0001      2
#define     TEST_PERSON_DETECTION_RETAIL_0002                        3
#define     TEST_OBJ_CLS                                                                4

#define    CUR_TEST_CASE     TEST_LICENSE_PLATE_RECOGNITION_BARRIER_0001

////////////////////////////////////////////////////////////////////////

void  cls_show_img_func(void *data, int w, int h, float scale, char *name, int nn_fov_show, Network1Par *nnparm,Network2Par *nnParm2, char *nnret,float conf);
void  obj_show_img_func(void *data, int w, int h, float scale, char *name, int nn_fov_show, Network1Par *nnparm,Network2Par *nnParm2, char *nnret,float conf);
void  vehicle_license_plate_detection_barrier(void *data, int w, int h, float scale, char *name, int nn_fov_show, Network1Par *nnParm1,Network2Par *nnParm2, char *nnret, float conf);
typedef void  (*analyzeMetedata)(void *data, int w, int h, float scale, char *name, int nn_fov_show, Network1Par *nnparm, Network2Par *nnParm2, char *nnret,float conf);

extern "C" void os_sleep(int ms);

/* Default parameters of the first level model */
static Network1Par cnn1PrmSet =
{
    imageWidth:-1, imageHeight:-1,                     /* Dynamic acquisition */
    startX:-1, startY:-1,                                          /* Set according to the acquired sensor resolution */
    endX:-1,endY: -1,                                            /*  Set according to the acquired sensor resolution */
    inputDimWidth:-1, inputDimHeight:-1,            /* Set according to the acquired model parameters */
    inputFormat:IMG_FORMAT_BGR_PLANAR,      /*  The default input is BRG  */
    meanValue:{0,0,0},
    stdValue:1,
    isOutputYUV:1,                                               /*Turn on YUV420 output function*/
    isOutputH26X:1,                                             /*Turn on H26X encoding function*/
    isOutputJPEG:1,                                              /*Turn on the MJPEG encoding function*/
    mode:ENCODE_H264_MODE,                          /* Use H264 encoding format */
    extInputs:{0},                                                  /* Model multi-input, the second input parameter */
    modelCascade:0 ,                                           /* The next level model is not cascaded by default */
    inferenceACC:0,
};

/* Default parameters of the second level model */
static Network2Par cnn2PrmSet =
{
    startXAdj:0,
    startYAdj:0,
    endXAdj:0,
    endYAdj:0,
    labelMask:{0},                                                 /* The label mask to be processed, will be processed if the corresponding position is 1 */
    minConf: 0.99,                                                /* The detection target's confidence level is greater than this value before processing */
    inputDimWidth:-1, inputDimHeight:-1,            /* Set according to the acquired model parameters */
    inputFormat:IMG_FORMAT_BGR_PLANAR,      /*  The default input is BRG */
    meanValue:{0,0,0},
    stdValue:1,
    extInputs:{0},                                                  /* Model multi-input, the second input parameter */
    modelCascade:0                                             /* The next level model is not cascaded by default */
};

///////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    int ret;
    float conf=0.5;
    CameraInfo cam_info;
    memset(&cam_info, 0, sizeof(cam_info));

    //1.load firmware
    printf("=============================\n");
    printf("1.load firmware\n");
    printf("=============================\n");
    ret = load_fw("./moviUsbBoot","./fw/flicRefApp.mvcmd");
    if (ret < 0)
    {
        printf("lowd firmware error! return \n");
        return -1;
    }
    printf("usb sersion:%d \n", get_usb_version());

    //2. Get the current camera mode support list
    printf("=============================\n");
    printf("2. Get the current camera mode support list\n");
    printf("=============================\n");
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

    //3. Select camera working mode
    printf("=============================\n");
    printf("3. Select camera working mode\n");
    printf("=============================\n");
    int sensorModeId = 0; //0:1080P, 1:4K
    camera_select_sensor(sensorModeId);
    memcpy(&cameraCfg, &list.mode[sensorModeId], sizeof(cameraCfg)); //select camera info
    cnn1PrmSet.imageWidth  = cameraCfg.camWidth;
    cnn1PrmSet.imageHeight = cameraCfg.camHeight;

    //4. Configure device resoures
    printf("=============================\n");
    printf("4. Configure device resoures\n");
    printf("=============================\n");
    //////////////////////////////////////Model parameter initialization start///////////////////////////////////////////////////
 #if (CUR_TEST_CASE == TEST_FACE_DETCTION_ADAS_0001)
     //////////single model demo/////////
     cnn1PrmSet.startX      = 0;
     cnn1PrmSet.startY      = 0;
     cnn1PrmSet.endX        = cameraCfg.camWidth;
     cnn1PrmSet.endY        = cameraCfg.camHeight;
     cnn1PrmSet.inputDimWidth  = 672;
     cnn1PrmSet.inputDimHeight = 384;
     /*URL:      https://docs.openvinotoolkit.org/2019_R1.1/_face_detection_adas_0001_description_face_detection_adas_0001.html
     */
     char *blob  = "./blob/2020.3/face-detection-adas-0001/face-detection-adas-0001.blob";
     char *blob2 = NULL;
     analyzeMetedata fun = obj_show_img_func;
     conf = 0.8;
 #elif  (CUR_TEST_CASE == TEST_FACE_DETCTION_RETAIL_0004)
     //////////single model demo/////////
      cnn1PrmSet.startX      = 0;
      cnn1PrmSet.startY      = 0;
      cnn1PrmSet.endX        = cameraCfg.camWidth;
      cnn1PrmSet.endY        = cameraCfg.camHeight;
      cnn1PrmSet.inputDimWidth  = 300;
      cnn1PrmSet.inputDimHeight = 300;

      char *blob  = "./blob/2020.3/face-detection-retail-0004/face-detection-retail-0004.blob";
      char *blob2 = NULL;
      analyzeMetedata fun = obj_show_img_func;
      conf = 0.8;

 #elif  (CUR_TEST_CASE == TEST_OBJ_CLS)
     //////////single model demo/////////
      cnn1PrmSet.startX      = 0;
      cnn1PrmSet.startY      = 0;
      cnn1PrmSet.endX        = cameraCfg.camWidth;
      cnn1PrmSet.endY        = cameraCfg.camHeight;
      cnn1PrmSet.inputDimWidth  = 300;
      cnn1PrmSet.inputDimHeight = 300;

      char *blob  = "./blob/2020.3/object_classification/object_classification.blob";
      char *blob2 = NULL;
      analyzeMetedata fun = cls_show_img_func;
      conf = 0.7;

 #elif  (CUR_TEST_CASE == TEST_LICENSE_PLATE_RECOGNITION_BARRIER_0001)
     ///////////////////
     // 4.1 Initialization of the first level model
     cnn1PrmSet.startX      = 0;
     cnn1PrmSet.startY      = 0;
     cnn1PrmSet.endX        = cameraCfg.camWidth;
     cnn1PrmSet.endY        = cameraCfg.camHeight;
     cnn1PrmSet.inputDimWidth  = 300;
     cnn1PrmSet.inputDimHeight = 300;
     /* Cascade next level model */
     cnn1PrmSet.modelCascade = 1;

     //4.2 Second-level model initialization
     if(cnn1PrmSet.modelCascade !=0)
     {
         /* Based on the detection results of the first level, appropriately fine-tune the detection coordinates of the first level to facilitate recognition */
         if(1)
         {
             /*
              *Fine-tune the starting point to the upper left（startXAdj，startYAdj ）
              *The bottom point is fine-tuned to the bottom right（endXAdj，endYAdj） */
             cnn2PrmSet.startXAdj  = -5;
             cnn2PrmSet.startYAdj  = -5;
             cnn2PrmSet.endXAdj   = 5;
             cnn2PrmSet.endYAdj   = 5;
         }

         cnn2PrmSet.minConf        = 0.7;  //Confidence level setting, the first level target detection is greater than this value before entering the second level model processing
         cnn2PrmSet.labelMask[2] = 1;     // License plate label id = 2，vehicle label id = 1, only handle id=2

         /*name1: "data" , shape: [1x3x24x94] - An input image in following format [1xCxHxW]. Expected color order is BGR.*/
         cnn2PrmSet.inputDimWidth  = 94;
         cnn2PrmSet.inputDimHeight = 24;
         /*
          *URL:  https://docs.openvinotoolkit.org/2019_R1.1/_license_plate_recognition_barrier_0001_description_license_plate_recognition_barrier_0001.html
          * name2: "seq_ind" , shape: [88,1] - An auxiliary blob that is needed for correct decoding. Set this to [0, 1, 1, ..., 1]. */
         //Multi-input initialization, converted to FP16
         uint16_t *p = (uint16_t *)cnn2PrmSet.extInputs;
         p[0] = (uint16_t)f32Tof16(0);
         for(int i=1;i<88;i++)
         {
             p[i] = (uint16_t)f32Tof16(1.0);
         }
     }

     const char *blob =   "./blob/2020.3/vehicle-license-plate-detection-barrier-0106/vehicle-license-plate-detection-barrier-0106.blob";
     const char *blob2 = "./blob/2020.3/license-plate-recognition-barrier-0001/license-plate-recognition-barrier-0001.blob";
     analyzeMetedata fun = vehicle_license_plate_detection_barrier;
     conf =  cnn2PrmSet.minConf;

 #elif (CUR_TEST_CASE == TEST_PERSON_DETECTION_RETAIL_0002)

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
      char *blob2 = NULL;
      analyzeMetedata fun = obj_show_img_func;
      conf = 0.4;
 #else
 //     "TEST_CASE definition error "
 #endif

     //////////////////////////////////////Model parameter initialization end///////////////////////////////////////////////////
     //5. sdk initialization
    printf("=============================\n");
    printf("5. sdk initialization\n");
    printf("=============================\n");
     ret = sdk_net2_init(0,0,
             blob,  &cnn1PrmSet, sizeof(cnn1PrmSet), 
             blob2,  &cnn2PrmSet, sizeof(cnn2PrmSet));

       

    //6. configure output
    camera_video_out(YUV420p,VIDEO_OUT_CONTINUOUS);
    //camera_yuv420_out(YUV420_OUT_CONTINUOUS);
//    camera_h26x_out(H26X_OUT_ENABLE);
 //  camera_mjpeg_out(MJPEG_OUT_CONTINUOUS);

    //////////////////////////////////////////read test/////////////////////////////////////////////////
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

        //Block reading yuv420 data
        maxReadSize  = sizeof(frameSpecOut)+ cameraCfg.camWidth * cameraCfg.camHeight * 3 / 2;
        if (read_yuv_data(recvImageData, &maxReadSize, true) < 0)
        {
            os_sleep(1);
            break;
        }
        memcpy(&hdr, recvImageData, sizeof(frameSpecOut));

        //Non-blocking read metedata data
        maxReadSize = meteDataSize;
        if (read_meta_data(recvMetaData, &maxReadSize, false) == 0)
        {
            memcpy(&hdr, recvMetaData, sizeof(frameSpecOut));
        }

        //get YUV data and metedata data
        yuv420p  = (char*) recvImageData + sizeof(frameSpecOut);
        metaData = (char*) recvMetaData + sizeof(frameSpecOut);

        //display test
        sprintf(src, "demo_video_%dx%d@%dfps(scale:%d%%)", cameraCfg.camWidth, cameraCfg.camHeight, cameraCfg.camFps,(int)(100*scale));
         fun(yuv420p , cameraCfg.camWidth, cameraCfg.camHeight, scale,src, 1, &cnn1PrmSet,&cnn2PrmSet, metaData, conf);
    }
    printf("exit test main....\n");
    sdk_uninit();
    return 0;
}
