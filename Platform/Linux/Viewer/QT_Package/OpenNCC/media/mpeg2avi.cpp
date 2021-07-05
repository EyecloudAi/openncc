#include "mpeg2avi.h"
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QDateTime>

Mpeg2Avi::Mpeg2Avi()
{
    ofmtCtx_            = nullptr;
    outStream_          = nullptr;
    pkt_                = nullptr;
}

bool Mpeg2Avi::initVideoCodec(const int codeType,int width,int height){
    static char errorBuf[10240];
    memset(errorBuf,0,10240);

    status_ = CodecStatus::failed;

    av_register_all();

    QString workDir = QCoreApplication::applicationDirPath();

    QString pPath = workDir + "/avi";

    QDir dir(pPath);
    if(!dir.exists()){
        if(!dir.mkpath(pPath)){
            qInfo()<<"Code avi: initVideoCodec, Failed mkpath "
                  <<pPath
                  <<", save avi in workdir, workdir="
                 << workDir;
            pPath = workDir;
        }
    }

    QDateTime currentDateTime =QDateTime::currentDateTime();
    QString currentDate =currentDateTime.toString("yyyy-MM-dd-hh-mm-ss-zzz");
    QString fileName = pPath + "/"
            + currentDate + ".avi";

    qInfo()<<"Save Avi: init video codec, fileName="<<fileName;

    avformat_alloc_output_context2(&ofmtCtx_,
                                    NULL,
                                    NULL,
                                 fileName.toStdString().c_str());
    int ret = 0;
    if(!ofmtCtx_){
        ret = avformat_alloc_output_context2(&ofmtCtx_,
                                    NULL,
                                    "avi",
                                    fileName.toStdString().c_str());
    }
    if(!ofmtCtx_){
        av_strerror(ret, errorBuf, 102400);
        printf("save avi: Failed initVideoCodec,avformat_alloc_output_context2 err=%s\n",
               errorBuf);
        return false;
    }

    outStream_ = avformat_new_stream(ofmtCtx_,NULL);
    if(!outStream_){
        av_strerror(ret, errorBuf, 102400);
        printf("save avi: Failed initVideoCodec,avformat_new_stream err=%s\n",
               errorBuf);
        return false;
    }

    AVCodecContext *avctx = outStream_->codec;
    avctx->codec_type = AVMEDIA_TYPE_VIDEO; /*此处,需指定编码后的H264数据的分辨率、帧率及码率*/
    if(codeType == JPEG){
        avctx->codec_id = AV_CODEC_ID_MJPEG;
    }else{
        avctx->codec_id = AV_CODEC_ID_H265;
    }
    avctx->bit_rate = 4000000;
    avctx->width = 1920; //
    avctx->height = 1080; //
    avctx->time_base.num = 1; //
    avctx->time_base.den = 60; //
    //avctx->pix_fmt = PIX_FMT_YUV422P;
    //avctx->gop_size=1;
    //avctx->pix_fmt = AV_PIX_FMT_YUV420P;

    qInfo()<<"save avi: initVideoCodec, AVCodecContext paramas: width="<<width
          <<",height="<<height;

    /* print output stream information*/
    av_dump_format(ofmtCtx_,
                   0,
                   fileName.toStdString().c_str(),
                   1);

    if (!(ofmtCtx_->flags & AVFMT_NOFILE)){
         // first write file
         ret = avio_open(&(ofmtCtx_->pb),
                         fileName.toStdString().c_str(),
                         AVIO_FLAG_WRITE);
         if(ret < 0){
             av_strerror(ret, errorBuf, 102400);
             printf("save avi: Failed initVideoCodec,avio_open err=%s\n",
                    errorBuf);
             return false;
         }
    }

    ret = avformat_write_header(ofmtCtx_, NULL);
    if(ret < 0){
        av_strerror(ret, errorBuf, 102400);
        printf("save avi: Failed initVideoCodec,avformat_write_header err=%s\n",
               errorBuf);
        return false;
    }

    status_ = CodecStatus::successed;
    qInfo()<<"Successed initVideoCodec...!";
    return true;
}

bool Mpeg2Avi::encode(unsigned char* data,int size){
    unsigned char* ptr = data;

    qInfo()<<"pkt size="<<size;
    AVPacket pkt;
    av_init_packet(&pkt);
    if(size > 30000){
        pkt.flags |= AV_PKT_FLAG_KEY;
    }else{
        pkt.flags |= 0;
    }
    pkt.size = size;
    pkt.data = data;
    int ret = av_interleaved_write_frame(ofmtCtx_, &pkt);
    if (ret < 0) {
      return false;
    }
    return true;
}

bool Mpeg2Avi::endEncode(){
    if(ofmtCtx_){
        av_write_trailer(ofmtCtx_);
    }

    if(ofmtCtx_ && !(ofmtCtx_->oformat->flags & AVFMT_NOFILE)){
        avio_close(ofmtCtx_->pb);
    }

    if(ofmtCtx_){
        avformat_free_context(ofmtCtx_);
    }
    return true;
}
