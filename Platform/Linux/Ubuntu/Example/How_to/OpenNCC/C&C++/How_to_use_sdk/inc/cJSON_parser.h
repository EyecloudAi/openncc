#ifndef CJSON_TOOLS_H_
#define CJSON_TOOLS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../inc/native_vpu_api.h"
#include "cJSON.h"
#include "sdk.h"
#define FILTER_NUM (4)
#define MAX_EXTINPUT_SIZE  (64*1024)

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct{
    int dim;
    int tensor[4];
    METATYPE type;
}TensorSpec_t;

typedef struct{
    int enabled;
    char linkIn[32];
    char linkOut[32];
    TensorSpec_t input;
    TensorSpec_t output;
}GenFilterPara_t;

typedef struct{
    GenFilterPara_t  GenPara;
    IMAGE_FORMAT inFormat;
    IMAGE_FORMAT outFormat;
    int ROI[4];   //[start_X start_Y end_X end_Y]
    float mean_value[3];
    float std_value;
}NccVideoConvert_t;

typedef struct{
    GenFilterPara_t  GenPara;
    char  extInputs1[MAX_EXTINPUT_SIZE];  /* 模型多输入  */
    char  extInputs2[MAX_EXTINPUT_SIZE];  /* 模型多输入  */
    char  extInputs3[MAX_EXTINPUT_SIZE];  /* 模型多输入  */
}NccInference_t;

typedef struct{
    GenFilterPara_t  GenPara;
}NccTensorConvert_t;

typedef struct{
    GenFilterPara_t  GenPara;
    int conf;
}TensorDecode_t;
typedef struct
{
    NccVideoConvert_t  NccVideoConvert;
    NccInference_t     NccInference;
    NccTensorConvert_t NccTensorConvert;
    TensorDecode_t     TensorDecode;
}JsonPara_t,*pJsonPara;

int json_parse_obj_lv1(pJsonPara config, const char *JsonString);

#ifdef __cplusplus
}
#endif
#endif /* CJSON_TOOLS_H_ */
