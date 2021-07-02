#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pthread.h"
#include "sdk.h"
#include "cameraCtrl.h"

#ifndef _WINDOWS
#include <sys/time.h>
#else
#include <windows.h>	
extern "C" int gettimeofday(struct timeval *tp, void *tzp);
#pragma warning(disable:4996)
#endif

extern "C"  void os_sleep(int ms);

/* 1. 物体分类demo程序 */
//#define  DEMO_CLS   1
/* 2. 人脸检测demo程序 */
#define  DEMO_FACE_DETECTION  1

#define  OPENCV_SHOW_SCALE    (0.5)    /* 显示缩放系数 */

static char metadata[1024*1024];
static char yuv420p[sizeof(frameSpecOut)+1920*1080*2];
static volatile int show_update = 0;


///////////////////////////////////////////////////////////////////////////////////
/*
 参羿face-detection-retail-0004-fp16.xml文件
 input
 <dim>1</dim>
 <dim>3</dim>
 <dim>300</dim>
 <dim>300</dim>
 optput_fp16
 <dim>1</dim>
 <dim>1</dim>
 <dim>100</dim>
 <dim>7</dim>
 */

extern void  fd_show_img_func(void *data, int w, int h, float scale, char *name, int nn_fov_show, CameraInfo *nnparm, char *nnret);
///////////////////////////////////////////////////////////////////////////////////
static int fd = 0;
bool g_run = true;
//static char * myfifo = "/tmp/myfifo";
/* ffplay /tmp/myfifo  -framerate 30 */

static char cali_file[] = "./blob/calibration.bin";
#if 0   //read data by call back
static CameraInfo fd_nn_par =
{
	-1,  //imageWidth
	-1,  //imageHeight                  
    -1,  //startX
	-1,  //startY                 
	-1,   //endX
	-1,   //endY                  
	300,  //inputDimWidth
	300,   //inputDimHeight                   /* <dim>300</dim>  <dim>300</dim> */
	IMG_FORMAT_BGR_PLANAR,      //IMAGE_FORMAT   
	0,                    //meanValue
	0,
	0 ,
	1,                         //stdValue
    1,                           /*打开YUV420输出功能*/
    1,                           /*打开H26X编码功能*/
    1,                           /*打开MJPEG编码功能*/
	ENCODE_H264_MODE,            /* 使用H264编码格式 */
};

static void vscRead(void* param,void* data, int len)
{
    static  struct timeval tv1, tv2;
    static  int res_cnt = 0;

    /* 获取视频流数据头 */
    frameSpecOut *out = (frameSpecOut *)data;
    if(JPEG == out->type)
        printf("Meta:type:%d,size:%d,seqNo:%d\n", out->type, out->size, out->seqNo);
		return;
    switch (out->type)
    {
        case( YUV420p) :
        {
            /* 获取视频流数拿YUV420P */
            char *yuv_data = (char *) data + sizeof(frameSpecOut);
            #if (DEMO_CLS!=0 || DEMO_FACE_DETECTION!=0)
            memcpy(yuv420p, yuv_data, out->size<sizeof(yuv420p)?out->size:sizeof(yuv420p));
            show_update = 1;
            #endif
        }
            break;

        case (H26X ):
        {
            /* 获取H65视频流数懿
            char *h265_data = (char *) data + sizeof(frameSpecOut);
            if(fd ==0)
            {
                if (access(myfifo, F_OK) == -1)
                {
                    int res = mkfifo(myfifo, 0777);
                    if (res != 0)
                    {
                        fprintf(stderr, "Could not create fifo %s\n", myfifo);
                        exit(EXIT_FAILURE);
                    }
                }
                fd = open(myfifo, O_RDWR);
            }
            if(fd)
                write(fd, h265_data, out->size);
				*/
        }
            break;

        case (METEDATA) :
        {
//            printf("seqNo:%d, CNN runtime=%dms\n",out->seqNo, out->res[0]);
            /* 获取NCS计算结果, 数据格式是fp16 */
            char *nn_ret = (char *) data + sizeof(frameSpecOut);
            /* 算法性能统计 */
            if (res_cnt == 0)
            {
                gettimeofday(&tv1, NULL);
            }
            res_cnt++;
            if (res_cnt == 100)
            {
                gettimeofday(&tv2, NULL);
                long int d = tv2.tv_sec * 1000000 + tv2.tv_usec - tv1.tv_sec * 1000000 - tv1.tv_usec;
                printf("NCS2_FPS %.3f param:%s\n", (float) (100 * 1000000) / d,(char*)param);
                res_cnt = 0;
            }

        #if (DEMO_CLS!=0 || DEMO_FACE_DETECTION!=0)
            memcpy(metadata, nn_ret, out->size<sizeof(metadata)?out->size:sizeof(metadata));
        #endif
        }
        break;
        case (JPEG ):
        {
            /* 获取JPEG视频流数*/
            char *jpeg_data = (char *) data + sizeof(frameSpecOut);

//          if(out->seqNo %300 ==0)
//          {
//              char src[64];
//              FILE *fp;
//              sprintf(src, "seq_%d.jpeg", out->seqNo);
//              if((fp=fopen(src,"wb")) != NULL)
//              {
//                  fwrite(jpeg_data, out->size, 1, fp);
//                  fclose(fp);
//              }
//          }
        }
        break;

        default:
            break;
    }
}

int main(void)
{
	int ret;
	 memset(metadata, 0, sizeof(metadata));

    //1. 加载固件
    load_fw("./fw/flicRefApp.mvcmd");//AUTO_LOAD_FW
		
		printf("usb sersion:%d \n",get_usb_version());
		
   /* 2. 获取camera参数 */
    SensorModesConfig cameraCfg;
    camera_control_get_features(&cameraCfg);
    printf("\n\ncamera: %s, %dX%d@%dfps, AFmode:%d, maxEXP:%dus,gain[%d, %d]\n\n", cameraCfg.moduleName, cameraCfg.camWidth, cameraCfg.camHeight, cameraCfg.camFps,
            cameraCfg.AFmode, cameraCfg.maxEXP, cameraCfg.minGain, cameraCfg.maxGain);

    // 3. 算法有效区域初始冿  fd_nn_par.imageWidth  = cameraCfg.camWidth;
    fd_nn_par.imageWidth  = cameraCfg.camWidth;
    fd_nn_par.imageHeight = cameraCfg.camHeight;
    fd_nn_par.startX      = 0;
    fd_nn_par.startY      = 0;
    fd_nn_par.endX        = cameraCfg.camWidth;
    fd_nn_par.endY        = cameraCfg.camHeight;

	// 4. sdk初始冿 
    //int ret = sdk_init(vscRead, (void*)"12345",cali_file, NULL, NULL);
    ret = sdk_init(vscRead, (void*)"12345", (char*) "./blob/face-detection-retail-0004-fp16.blob", (void*)&fd_nn_par, sizeof(fd_nn_par));
		printf("xlink_init %d\n", ret);

    //5. 输出配置
 //    camera_yuv420_out(YUV420_OUT_CONTINUOUS);
 //   camera_h26x_out(H26X_OUT_ENABLE);
    camera_mjpeg_out(MJPEG_OUT_CONTINUOUS);
	/***************************demo test start********************************/
	while(1)
	{
		char src[64];
		float scale = OPENCV_SHOW_SCALE; /* 显示缩放 */
		if(show_update ==0)
		{
			os_sleep(1);
			continue;
		}

		sprintf(src, "fd_demo_video_%dx%d@%dfps(scale:%d%%)", cameraCfg.camWidth, cameraCfg.camHeight, cameraCfg.camFps,(int)(100*scale));
		fd_show_img_func(yuv420p , cameraCfg.camWidth, cameraCfg.camHeight, scale,src, true, &fd_nn_par, metadata);
		show_update = 0;
	}
	/***************************test end********************************/
}
#else
extern  void *ControlThread(void* arg);
int main(void)
{
	int ret;
	memset(metadata, 0, sizeof(metadata));
	CameraInfo fd_nn_par =
	{
		-1,  //imageWidth
		-1,  //imageHeight                  
	    -1,  //startX
		-1,  //startY                 
		-1,   //endX
		-1,   //endY                  
		300,  //inputDimWidth
		300,   //inputDimHeight                   /* <dim>300</dim>  <dim>300</dim> */
		IMG_FORMAT_BGR_PLANAR,      //IMAGE_FORMAT   
		0,                    //meanValue
		0,
		0 ,
		1,                         //stdValue
	    1,                           /*打开YUV420输出功能*/
	    1,                           /*打开H26X编码功能*/
	    1,                           /*打开MJPEG编码功能*/
		ENCODE_H264_MODE,            /* 使用H264编码格式 */
	};

  //1. 加载固件
  // ret= load_fw(AUTO_LOAD_FW);//AUTO_LOAD_FW
   ret=load_fw("./fw/flicRefApp.mvcmd");
   if (ret<0)
   {
   	printf("init device error! return \n");
		return -1;
   }
  printf("usb sersion:%d \n",get_usb_version());
	
 /* 2. 获取camera参数 */
  SensorModesConfig cameraCfg;
  camera_control_get_features(&cameraCfg);
  printf("\n\ncamera: %s, %dX%d@%dfps, AFmode:%d, maxEXP:%dus,gain[%d, %d]\n\n", cameraCfg.moduleName, cameraCfg.camWidth, cameraCfg.camHeight, cameraCfg.camFps,
          cameraCfg.AFmode, cameraCfg.maxEXP, cameraCfg.minGain, cameraCfg.maxGain);

  // 3. 算法有效区域初始冿  fd_nn_par.imageWidth  = cameraCfg.camWidth;
  fd_nn_par.imageWidth  = cameraCfg.camWidth;
  fd_nn_par.imageHeight = cameraCfg.camHeight;
  fd_nn_par.startX      = 0;
  fd_nn_par.startY      = 0;
  fd_nn_par.endX        = cameraCfg.camWidth;
  fd_nn_par.endY        = cameraCfg.camHeight;

// 4. sdk初始冿 
 // ret = sdk_init(NULL, NULL,cali_file, NULL, NULL);
  ret = sdk_init(NULL, NULL, (char*) "./blob/face-detection-retail-0004-fp16.blob", (void*)&fd_nn_par, sizeof(fd_nn_par));
//  cnn.blob_path=(char*)"./blob/face-detection-retail-0004-fp16.blob";
 // cnn.ir_xml = "./blob/face-detection-retail-0004-fp16.xml";
 // cnn.ir_bin = "./blob/face-detection-retail-0004-fp16.bin";
	
  printf("xlink_init %d\n", ret);
	if (ret<0) return -1;
  //5. 输出配置
  camera_yuv420_out(YUV420_OUT_CONTINUOUS);
//  camera_h26x_out(H26X_OUT_ENABLE);
 // camera_mjpeg_out(MJPEG_OUT_CONTINUOUS);
	pthread_t ctrlThd;
	pthread_create(&ctrlThd, NULL,ControlThread,NULL);
  FILE* file=NULL;
	while(g_run)
	{
		char src[64];
		float scale = OPENCV_SHOW_SCALE; /* 显示缩放 */
		int size= sizeof(yuv420p);
		if (read_yuv_data(yuv420p,&size,1)<0)
		//if (read_jpg_data(yuv420p,size,1)<0)
		{	
			os_sleep(1);
			break;
		}
		#if 0
		file=fopen("test.jpg","w");
	  size=fwrite(yuv420p+sizeof(frameSpecOut) ,1,size,file);	
	  fclose(file);
	  printf("save jpg file %d\n",size);
	  #endif
		//size = sizeof(metadata);
		//read_meta_data(metadata,&size,0);	
		//os_sleep(200);
		sprintf(src, "fd_demo_video_%dx%d@%dfps(scale:%d%%)", cameraCfg.camWidth, cameraCfg.camHeight, cameraCfg.camFps,(int)(100*scale));
		fd_show_img_func(yuv420p+sizeof(frameSpecOut) , cameraCfg.camWidth, cameraCfg.camHeight, scale,src, false, &fd_nn_par, metadata+sizeof(frameSpecOut));
	}    		 
	printf("exit test main....\n");
	sdk_uninit();
	return 0;
}
#endif
