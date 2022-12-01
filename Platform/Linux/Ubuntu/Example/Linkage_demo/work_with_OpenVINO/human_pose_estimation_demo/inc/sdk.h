
#ifndef  __PC_SDK_H__
#define  __PC_SDK_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//defien video frame type  for camera_video_out
#define   YUV420p  2
#define   RAW14   11

#define   H26X     22
#define   JPEG     23
#define   METEDATA 24
#define   MONO     25
#define   DEPTH    26

#define OUTPUT_INDEX_SIZE   64 

enum usb_error {
	/** Success (no error) */
	USB_SUCCESS = 0,

	/** Input/output error */
	USB_ERROR_IO = -1,

	/** Invalid parameter */
	USB_ERROR_INVALID_PARAM = -2,

	/** Access denied (insufficient permissions) */
	USB_ERROR_ACCESS = -3,

	/** No such device (it may have been disconnected) */
	USB_ERROR_NO_DEVICE = -4,

	/** Entity not found */
	USB_ERROR_NOT_FOUND = -5,

	/** Resource busy */
	USB_ERROR_BUSY = -6,

	/** Operation timed out */
	USB_ERROR_TIMEOUT = -7,

	/** Overflow */
	USB_ERROR_OVERFLOW = -8,

	/** Pipe error */
	USB_ERROR_PIPE = -9,

	/** System call interrupted (perhaps due to signal) */
	USB_ERROR_INTERRUPTED = -10,

	/** Insufficient memory */
	USB_ERROR_NO_MEM = -11,

	/** Operation not supported or unimplemented on this platform */
	USB_ERROR_NOT_SUPPORTED = -12,

	/* NB: Remember to update LIBUSB_ERROR_COUNT below as well as the
	   message strings in strerror.c when adding new error codes here. */

	/** Other error */
	USB_ERROR_OTHER = -99,
};

typedef enum 
{
    IMG_FORMAT_GRAY = 0,
    IMG_FORMAT_I420,        //YUV420 planar
    IMG_FORMAT_I422,        //YUV422 planar
    IMG_FORMAT_I444,        //YUV444 planar
    IMG_FORMAT_YUV444,      //YUV444 packed
    IMG_FORMAT_RGB = 5,     //RGB packed
    IMG_FORMAT_RGB_PLANAR,  //RGB planar
    IMG_FORMAT_BGR,         //BGR packed
    IMG_FORMAT_BGR_PLANAR   //BGR planar
}IMAGE_FORMAT;

typedef enum
{
    ENCODE_H264_MODE,
    ENCODE_H265_MODE,
}encodeMode;

/* mode process setting */
typedef struct{
	int imageWidth;           
	int imageHeight;        
	int startX;              
	int startY;
	int endX;                
	int endY;
	int inputDimWidth;        
	int inputDimHeight;       
	IMAGE_FORMAT inputFormat; /* input image mode£¬only RGB/RGB_PLANAR/BGR/BGR_PLANAR */
	float meanValue[3];       /* inputFormat RGB:
	                          ¡¡   R = (R-meanValue[0])/stdValue
   ¡¡                              G = (G-meanValue[0])/stdValue
          ¡¡                       B = (B-meanValue[0])/stdValue */
	float stdValue;
	int   isOutputYUV;
	int   isOutputH26X;
	int   isOutputJPEG;
 	encodeMode mode;            /* H264/H265 */
} CameraInfo;

#define  MAX_LABEL_SIZE         (10000)         /* max object labels */
#define  MAX_EXTINPUT_SIZE      (64*1024)      /* max second input length*/
#define  MAX_OBJ                (100)            /* max second objects */
/* first mode process setting */
typedef struct{
	int imageWidth;           
	int imageHeight;        
	int startX;              
	int startY;
	int endX;                
	int endY;
	int inputDimWidth;        
	int inputDimHeight;       
	IMAGE_FORMAT inputFormat; /* input image mode£¬only RGB/RGB_PLANAR/BGR/BGR_PLANAR */
	float meanValue[3];       /* inputFormat RGB:
	                          ¡¡   R = (R-meanValue[0])/stdValue
   ¡¡                              G = (G-meanValue[0])/stdValue
          ¡¡                       B = (B-meanValue[0])/stdValue */
	float stdValue;
	int   isOutputYUV;
	int   isOutputH26X;
	int   isOutputJPEG;
 	encodeMode mode;            /* H264/H265 */
 	
  char extInputs[MAX_EXTINPUT_SIZE];       /* second model input   */
  int   modelCascade;                                  /* linked next model */
  int   inferenceACC;                                    /* Accelerating  inference 0:close 1:open */
} Network1Par;

/* second model param */
typedef struct{
    int startXAdj;                                   
    int startYAdj;
    int endXAdj;                                    
    int endYAdj;
    char  labelMask[MAX_LABEL_SIZE];          /* mask label, bit equal to 1  will be useful */
    float minConf;                                 /* conf value from first model */

    int inputDimWidth;        
    int inputDimHeight;       
    IMAGE_FORMAT inputFormat;   /* input image mode£¬only RGB/RGB_PLANAR/BGR/BGR_PLANAR */
    float meanValue[3];          /* inputFormat RGB:
                              ¡¡   R = (R-meanValue[0])/stdValue
   ¡¡                              G = (G-meanValue[0])/stdValue
          ¡¡                       B = (B-meanValue[0])/stdValue */
    float stdValue;
    char extInputs[MAX_EXTINPUT_SIZE];          /* second model input  */
    int   modelCascade;                 /*linked next model  for third model in future*/
} Network2Par;

typedef struct
{
     int      type;
     unsigned int  seqNo;
     unsigned int  size;
     unsigned int  temps[8];
     unsigned int  res[5];
}frameSpecOut;


int GetYuvData(char *pbuf, int slen);
int GetMetaData(char *pbuf, int slen);
int GetH26xData(char *pbuf, int slen);
int GetJpegData(char *buf, int slen);
void SetMeanValue(CameraInfo* cam_info,float val1,float val2,float val3);

#ifdef __cplusplus
extern "C" {
#endif
typedef void (*vscRecvCb)(void* param,void *data, int len);

void get_sdk_version( char* version);

//load firmware and vsc init
int load_fw(const char* bootExe, const char* firmware );

int load_fwExt(const char* bootExe, const char* firmware,int pid);

//sdk ini for single net model and one input
int sdk_init(vscRecvCb cb,void* param, const char *blob_path, CameraInfo * cam, int cam_Len);

//support two network model or two inputs for single model
int sdk_net2_init(vscRecvCb cb,void* param, const char *blob_path, Network1Par *par, int par_Len, const char *blob2_path, Network2Par *par2, int par2_Len);

#ifdef DEPTH_3D 
int sdk_init_ext( vscRecvCb cb,void* param, char *blob_path,char *blob1_path, CameraInfo * cam, int cam_Len);

int read_ir_data(char* pbuf,int * size,int blocked);

int read_depth_data(char* pbuf,int * size,int blocked);
#endif

void sdk_uninit(void);

//add by duke 2020.2
//return 20:usb2.0  11:usb1.1   30:usb3.0
int get_usb_version(void);

int get_err_no();

//blocked:1 read until a packet received 0:if no packet ,return at once
int read_yuv_data(char* pbuf,int * size,int blocked);

int read_26x_data(char* pbuf,int * size,int blocked);

int read_jpg_data(char* pbuf,int * size,int blocked);

int read_meta_data(char* pbuf,int * size,int blocked);

//download image data to device for ai analysize
int down_image_data(const char *data, int len);
//add end 

int get_meta_size();

#ifdef __cplusplus
}
#endif

#endif


