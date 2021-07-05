/*
Jon Zhao 2020.4.6
*/
#include "decodevideo.h"
#include "ncc_sdk/cameraCtrl.h"
#include "ncc_sdk/sdk.h"

static AVCodec *codec = NULL;
static AVCodecContext *codec_ctx= NULL;
static AVFrame *frame = NULL;
static AVPacket avpkt;
static bool codecInited = false;


int InitDecoder(enum AVCodecID codec_id)
{
    if(codecInited != false)
    {
        printf("decoder inited fail\n");
        return -1;
    }
    avcodec_register_all();
    codec = avcodec_find_decoder(codec_id);
    if(!codec)
    {
        printf("avcodec_find_decoder fail\n");
        return -1;
    }
    codec_ctx = avcodec_alloc_context3(codec);
    if(!codec_ctx)
    {
        printf("avcodec_alloc_context3 fail\n");
        return -1;
    }/*
    else
    {
        codec_ctx->time_base.num = 1;
        codec_ctx->frame_number = 1; //每包一个视频帧
        codec_ctx->codec_type = AVMEDIA_TYPE_VIDEO;
        codec_ctx->bit_rate = 0;
        codec_ctx->time_base.den = den;//帧率
        codec_ctx->width = width;//视频宽
        codec_ctx->height = height;//视频高
    }*/
    codec_ctx->thread_count = 8;
    if(avcodec_open2(codec_ctx, codec, NULL) < 0)
    {
        printf("avcodec_open2 fail\n");
        return -1;
    }
    frame = av_frame_alloc();
    if(!frame)
    {
        printf("av_frame_alloc fail\n");
        return -1;
    }
    av_init_packet(&avpkt);
    codecInited = true;
    return 0;
}


static int decode_frame(AVCodecContext *avctx,AVFrame *frame,//input
                        AVPacket *pkt, int last,//input
                        unsigned char *outputframe)//output
{
    int len = 0;
    int got_frame = 0;
    if(!codecInited)
    {
        printf("%s decoder uninted\n",__func__);
        return -1;
    }
    len = avcodec_decode_video2(avctx, frame, &got_frame, pkt);
    if(len < 0)
    {
        printf("Error while decoding frames\n");
        return len;
    }
    if(got_frame)
    {
        switch(avctx->pix_fmt)
        {
        case AV_PIX_FMT_YUV422P:
        {
            int index = 0;
            int y_i  = 0 , u_i = 0 , v_i = 0;
            for(index = 0 ; index < frame->width*frame->height*2 ;)
            {
                outputframe[index++] = frame->data[0][y_i++];
                outputframe[index++] = frame->data[1][u_i++];
                outputframe[index++] = frame->data[0][y_i++];
                outputframe[index++] = frame->data[2][v_i++];
            }
            printf("decode_frame the format is AV_PIX_FMT_YUV422P\n");
        }
            break;
        case AV_PIX_FMT_YUVJ420P:
        case AV_PIX_FMT_YUV420P:
        {
            unsigned char* cpyptr = NULL;
            //                copy Y
            cpyptr = outputframe;
            memcpy(cpyptr,frame->data[0],frame->width*frame->height);

            //copy U
            cpyptr = cpyptr + frame->width*frame->height;
            memcpy(cpyptr,frame->data[1],frame->width*frame->height/4);

            //copy V
            cpyptr = cpyptr + frame->width*frame->height/4;
            memcpy(cpyptr,frame->data[2],frame->width*frame->height/4);
            //                printf("decode_frame the format is AV_PIX_FMT_YUV420P\n");
        }
            break;
        default:
            break;
        }
    }
    if(pkt->data)
    {
        pkt->size -= len;
        pkt->data += len;
    }
    return 0;
}


int DecodeFrame(unsigned char * framedata , int framelen ,//input
                unsigned char *outputframe , int *width , int *height , int *pixfmt)//output
{
    avpkt.size = framelen;
    avpkt.data = framedata;
    while(avpkt.size > 0)
    {
        if(decode_frame(codec_ctx, frame, &avpkt, 0, outputframe) < 0)
        {
            printf("%s decode fail\n",__func__);
            return -1;
        }
    }
    *width = codec_ctx->width;
    *height = codec_ctx->height;
    *pixfmt = codec_ctx->pix_fmt;
    return 0;
}



int DecodeFrameFlush(unsigned char *outputframe , int *width , int *height , int *pixfmt)//output
{
    if(!codecInited)
    {
        printf("%s decoder uninted\n",__func__);
        return -1;
    }
    avpkt.data = NULL;
    avpkt.size = 0;
    if(decode_frame(codec_ctx, frame, &avpkt, 1, outputframe) < 0)
        return -1;
    *width = codec_ctx->width;
    *height = codec_ctx->height;
    *pixfmt = codec_ctx->pix_fmt;
    return 0;
}

void UninitDecoder(void)
{
    if(codecInited)
    {
        avcodec_close(codec_ctx);
        av_free(codec_ctx);
        av_frame_free(&frame);
        codecInited = false;
    }
}

int NCCVideoCtrl(int video_type,int mode,int h26x_type)
{
    switch(video_type)
    {
    case YUV420p:
        camera_video_out(video_type,(camera_ctrl_video_out)mode);
        break;
    case H26X:
    {
        if((camera_ctrl_video_out)mode == VIDEO_OUT_CONTINUOUS)
        {
            if(h26x_type==1)
                InitDecoder(AV_CODEC_ID_H264);
            else if(h26x_type==2)
                InitDecoder(AV_CODEC_ID_H265);
        }
        else
            UninitDecoder();
        camera_video_out(video_type,(camera_ctrl_video_out)mode);
    }
        break;


    case JPEG:
        if((camera_ctrl_video_out)mode == VIDEO_OUT_CONTINUOUS)
        {
            InitDecoder(AV_CODEC_ID_MJPEG);
        }
        else
            UninitDecoder();
        camera_video_out(video_type,(camera_ctrl_video_out)mode);
        break;
    default:
        return -1;
        break;
    }
    return 0;
}
