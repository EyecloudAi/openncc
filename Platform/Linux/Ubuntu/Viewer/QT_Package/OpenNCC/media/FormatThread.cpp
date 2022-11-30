#include "FormatThread.h"
#include <QDebug>
#include <QFileInfo>
#include <QImage>
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libavutil/opt.h"
#include "libavutil/mathematics.h"
#include "libavutil/samplefmt.h"
#include "libavutil/pixfmt.h"
}
// =============================== video start =============================
FormatThread::FormatThread()
{
    pFormatCtx = NULL;
    videoStreamIdx = -1;
    fileLen=0;

    av_register_all();
}

FormatThread::~FormatThread()
{
    qDebug()<<"Format thread exit!";

    if(NULL!=pFormatCtx)
    {
        avformat_close_input(&pFormatCtx);
        free(pFormatCtx);
    }
}

int FormatThread::openUrl(QString url)
{
    int nRet=0;
    inFileName = url;
    pFormatCtx = avformat_alloc_context();
    //input
    nRet = avformat_open_input(&pFormatCtx, url.toStdString().c_str(), NULL,NULL);
    if (nRet < 0)
    {
        //qDebug()<<nRet;
        qDebug()<<"Can not open:"<<url;
        return -1;
    }

    if (avformat_find_stream_info(pFormatCtx,NULL) < 0)
    {
        qDebug()<<"Unable to get stream info!";
        return -2;
    }

    int i;
    videoStreamIdx = -1;
    for (i = 0; i < pFormatCtx->nb_streams; i++)
    {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStreamIdx = i;
            break;
        }
    }
    if (videoStreamIdx == -1)
    {
       qDebug()<<"Unable to find video stream!";
       return -3;
    }

    QString strValue;

    strValue=QString::number(pFormatCtx->streams[videoStreamIdx]->codec->width) + "*" +
            QString::number(pFormatCtx->streams[videoStreamIdx]->codec->height);
    //emit sig(0,strValue,0); // 发送分辨率

    strValue=QString::number(pFormatCtx->streams[videoStreamIdx]->avg_frame_rate.num);
    //emit sig(1,strValue,0); // 发送帧率

    QFileInfo fileInfo;
    fileInfo.setFile(url);
    if(!fileInfo.isFile()) return -4;
    fileLen=fileInfo.size()/1024;
    strValue=QString::number(fileLen)+" KB";
    //emit sig(2,strValue,0);         // 文件大小
}

void FormatThread::setOutFileName(QString fileName)
{
    outFileName=fileName;
}

 void  FormatThread::run()
 {
    if(NULL==pFormatCtx) return;
    qDebug()<<"Thread start.";

    AVFormatContext *o_fmt_ctx;
    AVStream *o_video_stream;
    AVStream *i_video_stream;
    int startFlag=false;
    int frameCount=0;
    uint32_t sizeCount=0;

    i_video_stream=pFormatCtx->streams[videoStreamIdx];
    //初始化输出视频码流的AVFormatContext
    avformat_alloc_output_context2(&o_fmt_ctx, NULL, NULL,outFileName.toStdString().c_str());

    //创建输出码流的AVStream
    o_video_stream = avformat_new_stream(o_fmt_ctx, NULL);

    //拷贝输入视频码流的AVCodecContext的数值到输出视频的AVCodecContext
    avcodec_copy_context(o_video_stream->codec, i_video_stream->codec);
    o_video_stream->codec->codec_tag = 0;
    o_video_stream->codec->flags = i_video_stream->codec->flags;
    //o_video_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
    o_video_stream->codec->time_base.num = i_video_stream->avg_frame_rate.den;
    o_video_stream->codec->time_base.den = i_video_stream->avg_frame_rate.num;

    //打开输出文件
    avio_open(&o_fmt_ctx->pb, outFileName.toStdString().c_str(), AVIO_FLAG_WRITE);

    //写文件头
    avformat_write_header(o_fmt_ctx, NULL);

     while (av_read_frame(pFormatCtx, &packet) >= 0)
     {
         sizeCount+=packet.size/1024;
         emit sig(3,NULL,100*sizeCount/fileLen);

         if(packet.stream_index==videoStreamIdx)
         {
             if(!startFlag)
             {
                 if(packet.flags)
                 {
                     startFlag=true;
                 }
                 else continue;
             }

             frameCount++;
             //qDebug()<<"frame:"<<frameCount;

            if(packet.pts==AV_NOPTS_VALUE)
            {
              //Write PTS
              AVRational time_base1=i_video_stream->time_base;
              //Duration between 2 frames (us)
              int64_t calc_duration=(double)AV_TIME_BASE/av_q2d(i_video_stream->r_frame_rate);
              //Parameters
              packet.pts=(double)(frameCount*calc_duration)/(double)(av_q2d(time_base1)*AV_TIME_BASE);
              packet.dts=packet.pts;
              packet.duration=(double)calc_duration/(double)(av_q2d(time_base1)*AV_TIME_BASE);
            }

            //Convert PTS/DTS
            packet.pts = av_rescale_q_rnd(packet.pts, i_video_stream->time_base, o_video_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
            packet.dts = av_rescale_q_rnd(packet.dts, i_video_stream->time_base, o_video_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
            packet.duration = av_rescale_q(packet.duration, i_video_stream->time_base, o_video_stream->time_base);

            packet.pos = -1;
            packet.flags |= AV_PKT_FLAG_KEY;
            packet.stream_index = 0;

            //将AVPacket写入文件
            av_interleaved_write_frame(o_fmt_ctx, &packet);
        }
        av_free_packet(&packet);
     }

    //写文件尾
    av_write_trailer(o_fmt_ctx);

    avcodec_close(o_fmt_ctx->streams[0]->codec);
    av_freep(&o_fmt_ctx->streams[0]->codec);
    av_freep(&o_fmt_ctx->streams[0]);
    avio_close(o_fmt_ctx->pb);
    av_free(o_fmt_ctx);

    avformat_close_input(&pFormatCtx);
    free(pFormatCtx);
    pFormatCtx=NULL;

    //emit sig(3,NULL,100);

    QFile::remove(inFileName);
    qDebug()<<"Finish!";
 }
// =============================== video stop =============================


// =============================== img start =============================
ImgThread::ImgThread()
{
    pFormatCtx = NULL;
    pCodecCtx=NULL;
    pFrame=NULL;
    pSwsCtx=NULL;
    pCodec=NULL;

    videoStreamIdx = -1;
    fileLen=0;
    imgFormat=0;
    intervalSec=1;
    frameRate=25;
    width=0;
    height=0;

    av_register_all();
}

ImgThread::~ImgThread()
{
    qDebug()<<"Format thread exit!";

    if(NULL!=pFormatCtx)
    {
        avformat_close_input(&pFormatCtx);
        avformat_free_context(pFormatCtx);
        av_frame_free(&pFrame);
    }
}

int ImgThread::openUrl(QString url)
{
    int nRet=0;

    pFrame=av_frame_alloc();
    pFormatCtx = avformat_alloc_context();
    nRet = avformat_open_input(&pFormatCtx, url.toStdString().c_str(), NULL,NULL);
    if (nRet < 0)
    {
        qDebug()<<"Can not open:"<<url;
        return -1;
    }

    if (avformat_find_stream_info(pFormatCtx,NULL) < 0)
    {
        qDebug()<<"Unable to get stream info!";
        return -2;
    }

    int i;
    videoStreamIdx = -1;
    for (i = 0; i < pFormatCtx->nb_streams; i++)
    {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStreamIdx = i;
            break;
        }
    }
    if (videoStreamIdx == -1)
    {
       qDebug()<<"Unable to find video stream!";
       return -3;
    }

   pCodecCtx = pFormatCtx->streams[videoStreamIdx]->codec;

    QString strValue;

    width=pCodecCtx->width;
    height=pCodecCtx->height;
    strValue=QString::number(width) + "*" + QString::number(height);
    emit sig(0,strValue,0); // 发送分辨率

    frameRate=pFormatCtx->streams[videoStreamIdx]->avg_frame_rate.num;
    strValue=QString::number(frameRate);
    emit sig(1,strValue,0); // 发送帧率

    QFileInfo fileInfo;
    fileInfo.setFile(url);
    if(!fileInfo.isFile()) return -4;
    fileLen=fileInfo.size()/1024;
    strValue=QString::number(fileLen)+" KB";
    emit sig(2,strValue,0);         // 文件大小

    avpicture_alloc(&picture,AV_PIX_FMT_RGB24,width,height);
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    pSwsCtx = sws_getContext(width, height, AV_PIX_FMT_YUV420P, width,
            height, AV_PIX_FMT_RGB24,SWS_BICUBIC, 0, 0, 0);

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
    {
        qDebug()<<"Unable to open codec";
        return -5;
    }
}

void ImgThread::setOutPath(QString path)
{
    outPath=path;
}

void ImgThread::setImgFormat(int flag)
{
    imgFormat=flag;
}

void ImgThread::setIntervalSec(int sec)
{
    intervalSec=sec;
}

void ImgThread::run()
{
    if(NULL==pFormatCtx) return;
    qDebug()<<"Thread start.";

    int frameFinished=0;
    int startFlag=false;
    int imgCount=1;
    int frameCount=0;
    uint32_t sizeCount=0;
    int nRet=0;

    QString imgName;

     while (av_read_frame(pFormatCtx, &packet) >= 0)
     {
         sizeCount+=packet.size/1024;
         emit sig(3,NULL,100*sizeCount/fileLen);

         if(packet.stream_index==videoStreamIdx)
         {
             if(!startFlag)
             {
                 if(packet.flags)
                 {
                     startFlag=true;
                 }
                 else continue;
             }

             frameCount++;
             avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
             if (frameFinished)
             {
                 if((frameCount%(frameRate*intervalSec))) continue;

                 nRet = sws_scale(pSwsCtx, (const uint8_t* const *) pFrame->data,
                                    pFrame->linesize, 0,height, picture.data, picture.linesize);
                 if(-1==nRet)  break;

                 QImage image=QImage(picture.data[0],width,height,QImage::Format_RGB888);

                 if(outPath.isEmpty()) imgName="";
                 else imgName=outPath+"/";

                 if(1==imgFormat)
                    imgName+="img"+QString::number(imgCount)+".jpg";
                 else
                    imgName+="img"+QString::number(imgCount)+".png";

                 image.save(imgName);
                 emit sig(4,imgName,0);
                 imgCount++;
             }
        }
        av_free_packet(&packet);
     }

    avformat_close_input(&pFormatCtx);
    avformat_free_context(pFormatCtx);
    pFormatCtx=NULL;
    av_frame_free(&pFrame);
    pFrame=NULL;

    emit sig(3,NULL,100);

    qDebug()<<"Finish!";
 }
// =============================== img stop =============================

