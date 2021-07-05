#ifndef H264DECODEVIDEO_H
#define H264DECODEVIDEO_H

extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libswscale/swscale.h"
    #include <libavutil/imgutils.h>
    #include <libavutil/samplefmt.h>
    #include <libavutil/timestamp.h>
};


//初始化解码器，codec_id为选择的解码器
//codec_id : AV_CODEC_ID_H264 or AV_CODEC_ID_MJPEG for OpenNCC
int InitDecoder(enum AVCodecID codec_id);

//入参：framedata、framelen为H264数据源和数据长度
//出参：outputframe为解码后的yuyv数据存放区，width/height/pixfmt返回视频宽/高/解码后的数据格式，
int DecodeFrame(unsigned char * framedata , int framelen ,//input
                       unsigned char *outputframe , int *width , int *height , int *pixfmt);//output

//参数意义同上一接口，作用是将解码器中的缓存数据刷出
int DecodeFrameFlush(unsigned char *outputframe , int *width , int *height , int *pixfmt);//output

//清理解码器申请的资源
void UninitDecoder(void);

int NCCVideoCtrl(int video_type,int en,int h26x_type);

#endif // H264DECODEVIDEO_H
