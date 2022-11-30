
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "cJSON.h"
#include <opencv2/opencv.hpp>
#ifndef _WINDOWS
#include <sys/time.h>
#include "str_mpl.h"
#else
#include <windows.h>
extern "C" int gettimeofday(struct timeval *tp, void *tzp);
#pragma warning(disable : 4996)
#endif

extern "C" void os_sleep(int ms);

typedef struct
{
    char oName[32];
    float oConf;
    int x1;
    int y1;
    int x2;
    int y2;
} yoloOBJ;

///////////////////////////////////////////////////////////////////////////////////

size_t get_file_size(const char *filepath)
{

    if (NULL == filepath)
        return 0;
    struct stat filestat;
    memset(&filestat, 0, sizeof(struct stat));
    /*获取文件信息*/
    if (0 == stat(filepath, &filestat))
        return filestat.st_size;
    else
        return 0;
}

int main(void)
{
    FILE *fp;

    char *filename = "./result.json";
    yoloOBJ Obj[3] = {{0}};

    /*get file size*/
    size_t size = get_file_size(filename);
    if (0 == size)
    {
        printf("get_file_size failed!!!\n");
    }

    /*malloc memory*/
    char *buf = (char *)malloc(size + 1);
    if (NULL == buf)
    {
        printf("malloc failed!!!\n");
    }
    memset(buf, 0, size + 1);

    fp = fopen(filename, "rb");

    fread(buf, 1, size, fp);
    fclose(fp);

    printf("read file %s complete, size=%d.\n", filename, size);

    cJSON *json, *arrayItem, *item, *object;
    int i = 0;

    json = cJSON_Parse(buf);

    if (!json)
    {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
    }

    else
    {
        arrayItem=cJSON_GetObjectItem(json,"YOLO");  // 获取json对象中的people节点
        int size;
        if(arrayItem!=NULL)
        {
            size=cJSON_GetArraySize(arrayItem);  //获取数组大小
        }

          for(i=0;i<size;i++)
            {
                printf("obj%d\n",i);
                object=cJSON_GetArrayItem(arrayItem,i);

                item=cJSON_GetObjectItem(object,"Class"); 
                 memcpy(Obj[i].oName,item->valuestring,strlen(item->valuestring));  
               printf("Obj[i].oName=%s\n",Obj[i].oName);


                item=cJSON_GetObjectItem(object,"Conf"); // 获取json对象中的firstName节点
                 Obj[i].oConf=item->valuedouble;
                 printf("Obj[i].oConf=%f\n",Obj[i].oConf);

                item=cJSON_GetObjectItem(object,"x1"); // 获取json对象中的firstName节点
                 Obj[i].x1=item->valueint;
                 printf("Obj[i].x1=%d\n",Obj[i].x1);
            }

        arrayItem = cJSON_GetObjectItem(json, "jpeg"); // 获取json对象中的people节点

        uint32_t inputLen = strlen(arrayItem->valuestring);

        char *dataBase64 = (char *)malloc(inputLen + 1);

        memcpy(dataBase64, arrayItem->valuestring, inputLen);

        uint32_t ouputLen = StrMpl_getBase64DecBufSize((uint8_t *)dataBase64, inputLen);

        uint8_t *jpegBuf = (uint8_t *)malloc(ouputLen + 1);
        int ret = StrMpl_decodeBase64((uint8_t *)dataBase64, inputLen, jpegBuf, &ouputLen);

        std::vector<unsigned char> buff_jpg;
        for (int i = 0; i < ouputLen; ++i)
        {
            buff_jpg.push_back(jpegBuf[i]);
        }

        cv::Mat img_decode;
        img_decode = cv::imdecode(buff_jpg, CV_LOAD_IMAGE_COLOR);

        cv::resize(img_decode, img_decode, cv::Size(800, 600));
        cv::imshow("img", img_decode);
        cv::waitKey(0);
    }

    free(buf);

    return 0;
}
