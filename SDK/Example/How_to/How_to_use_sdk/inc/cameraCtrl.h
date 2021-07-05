
#ifndef _CAMERA_CONTROL_H
#define _CAMERA_CONTROL_H

#include <stdint.h>

#define MAX_MODE_SIZE   5

typedef enum {
    CAMERA_CONTROL__AE_AUTO__FLASH_MODE__OFF,
    CAMERA_CONTROL__AE_AUTO__FLASH_MODE__AUTO,
}camera_ctrl_ae_mode;

typedef enum {
    CAMERA_CONTROL__AWB_MODE__OFF, //0
    CAMERA_CONTROL__AWB_MODE__AUTO, //1
}camera_ctrl_awb_mode;

typedef enum {
    CAMERA_CONTROL__AF_MODE_OFF, //0
    CAMERA_CONTROL__AF_MODE_AUTO, //1

}camera_ctrl_af_mode;

typedef enum
{
    VIDEO_OUT_DISABLE,       /* disable video out */
    VIDEO_OUT_SINGLE,        /* output one frame */
    VIDEO_OUT_CONTINUOUS,    /* output continues frames */
}camera_ctrl_video_out;

typedef enum
{
    PWM_OUT_DISABLE,        /* close output */
    PWM_OUT_ENABLE,         /* enable output */
}camera_ctrl_pwm_out;

typedef struct
{
  char     moduleName[16];
  int      camWidth;   // width
  int      camHeight;  // heigth
  int      camFps;     // fps
  int      AFmode;     // support AF or not 
  int      maxEXP;     // max export time£¬unit us
  int      minGain;    // min gain 
  int      maxGain;    // max gain
} SensorModesConfig; // config structure

typedef struct  {
    uint8_t  key_version;
    uint8_t  lock[8];
    uint8_t  hw_config;
    uint8_t  accelerate_num;
    uint8_t  hw_version;
    uint8_t  hw_version_reserve;
    uint8_t  batch_num[3];
    uint8_t  reserve[32];
}Encrypt_t;

typedef struct
{
  int              num;
  SensorModesConfig mode[MAX_MODE_SIZE];
} SensorModesList; // config structure

//caete this class for easy used by python 2020.3.26
class  CameraSensor  
{
	public :
		CameraSensor();
		~CameraSensor();
		int GetFirstSensor(SensorModesConfig* sensor);
		int GetNextSensor(SensorModesConfig* sensor);
	private:
		SensorModesList list;
		int nreaded;	
};

#ifdef __cplusplus
extern "C" {
#endif
//get camera propety
int camera_control_get_features(SensorModesList* list);

//get cam sensor info returne array of sensors
int camera_control_get_sensorinfo(SensorModesConfig info[],int num);

//set af mode
int camera_control_af_mode(camera_ctrl_af_mode af_mode);

//set lens pos
int camera_control_lens_move(uint32_t lens_position);//1-100

//triger af
int camera_control_focus_trigger(void);

//set camera expose mode
int camera_control_ae_mode(camera_ctrl_ae_mode flash_mode);

//set expose value unit :us, <= 1/fps ¡£
int camera_control_ae_set_exp( uint32_t exp_compensation);

//set gai value
int camera_control_ae_set_gain( uint32_t iso_val);

//set WB
int camera_control_awb_mode(camera_ctrl_awb_mode awb_mode);

int camera_video_out(int video_type,camera_ctrl_video_out mode);

//select camera sernsor
int camera_select_sensor(int sensorid);

#ifdef DEPTH_3D 
int camera_control_pwm_control(camera_ctrl_pwm_out mode);
int camera_control_pwm_set(int value);
int camera_control_alg_bf(char* buf, int len);
int camera_control_alg_cal(char* buf, int len);
int camera_control_get_temp(int* temp);
#endif

int  device_control_get_fw_version(char* fw,int len);
int  device_control_get_device_id(uint8_t* id,int size);

//return string id for export python
void  camera_get_ncc_id(char* cpuid);
void  camera_get_fw_version(char* fwversion);

int  device_control_get_device_info(Encrypt_t* info);
int  device_control_set_device_info(Encrypt_t* info);

int  camera_control_set_bps(int value);

#ifdef __cplusplus
}
#endif
#endif /* _CAMERA_CONTROL_H */

