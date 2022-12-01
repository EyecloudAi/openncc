/**
 * @file   cameraCtrl.h
 * @author Zed
 * @date   2021.10.18
 * @modify 2021.11.3
 */

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
  int      camHeight;  // height
  int      camFps;     // fps
  int      AFmode;     // support AF or not 
  int      maxEXP;     // max exposure time unit us
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
	int    bitrate;
	int    maxBitrate;
	int    keyframeFreq;
	int    numBFrame;
} EncodePara_t;


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

/**
 * @brief Get camera supported resolution list
 * @return
 *      @retval 0  success
 */
int camera_control_get_features(SensorModesList* list);

/**
 * @brief Get camera supported resolution list
 * @return
 *      @retval 0 success
 */
int camera_control_get_sensorinfo(SensorModesConfig info[],int num);

/**
 * @brief Set auto focus mode
 * @return
 *      @retval 0 success
 */
int camera_control_af_mode(camera_ctrl_af_mode af_mode);

/**
 * @brief Set lens position
 * @param lens_position[1-100]
 * @return
 *      @retval 0 success
 */
int camera_control_lens_move(uint32_t lens_position);

/**
 * @brief Trigger auto focus
 * @return
 *      @retval 0 success
 */
int camera_control_focus_trigger(void);

/**
 * @brief Set exposure mode
 * @return
 *      @retval 0 success
 */
int camera_control_ae_mode(camera_ctrl_ae_mode flash_mode);

/**
 * @brief Set exposure time
 * @param exp_compensation [unit: us, Max value: 1/fps*1000*1000]
 * @return
 *      @retval 0 success
 */
int camera_control_ae_set_exp( uint32_t exp_compensation);

/**
 * @brief Set gain
 * @param iso_val [Max value: get from SensorModesConfig]
 * @return
 *      @retval 0 success
 */
int camera_control_ae_set_gain( uint32_t iso_val);

/**
 * @brief Set white balance mode
 * @return
 *      @retval 0 success
 */
int camera_control_awb_mode(camera_ctrl_awb_mode awb_mode);

/**
 * @brief Set H.264 encoding rate
 * @return
 *      @retval 0 success
 */
int  camera_control_set_bps(int value);

/**
 * @brief Set video output type
 * @return
 *      @retval 0 success
 */
int camera_video_out(int video_type,camera_ctrl_video_out mode);

/**
 * @brief Select sensor and resolution
 * @param sensorid [Range from 0 to SensorModesConfig.num]
 * @return
 *      @retval 0 success
 */
int camera_select_sensor(int sensorid);

/**
 * @brief Get firmware version
 * @return
 *      @retval 0 success
 */
int device_control_get_fw_version(char* fw,int len);

/**
 * @brief Get device serial number
 * @return
 *      @retval 0 success
 */
int  device_control_get_device_id(uint8_t* id,int size);

/**
 * @brief String id for python
 */
void  camera_get_ncc_id(char* cpuid);

/**
 * @brief Get firmware version for python
 */
void  camera_get_fw_version(char* fwversion);

/**
 * @brief Get hardware information
 * @return
 *      @retval 0 success
 */
int  device_control_get_device_info(Encrypt_t* info);

/**
 * @brief set H26X information add 2021.11.3
 * @return
 *      @retval 0 success
 */
int device_ctrl_set_bps(EncodePara_t* info);

/**
 * @brief update emmc information add 2021.11.3
 * @return
 *      @retval 0 success
 */
int  emmc_control_update_app();

#ifdef __cplusplus
}
#endif
#endif /* _CAMERA_CONTROL_H */

