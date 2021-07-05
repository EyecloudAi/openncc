#include "media/media2Avi.h"
#include <QDebug>
#include <QUuid>
#include <QDateTime>
#include <QCoreApplication>
#include <QDir>
#include "h26x2avi.h"
#include "mpeg2avi.h"

Media2Avi::Media2Avi(){
    status_ = CodecStatus::uninit;
}

Media2Avi::CodecStatus Media2Avi::codecStatus(){
    return status_;
}

Media2AviAgent::Media2AviAgent(){
    media2Avi_          = nullptr;
    clickState_         = false;
}

void Media2AviAgent::setClickState(bool clicked){
    clickState_ = clicked;
}

bool Media2AviAgent::encode(const int codeType,
                            int h,
                            int w,
                            unsigned char* data,
                            int size){

    static long long kFrameCount = 0;

    bool ret = true;

//    qInfo()<<__FUNCTION__<<",clickState_="<<clickState_
//          <<",media2Avi_="<<media2Avi_;

    if(clickState_){
        if(media2Avi_ == nullptr){

            media2Avi_ = initMedia(codeType,w,h);
            if(!media2Avi_){
                qInfo()<<__FUNCTION__<<",Failed init video codec";
                return false;
            }else{
                qInfo()<<__FUNCTION__<<",Successed init video codec";
            }
        }
    }else{
        if(media2Avi_ != nullptr){
            endEncode();
            qInfo()<<__FUNCTION__<<",Successed stop media2avi";
        }
        return true;
    }

    if(media2Avi_){
        if(kFrameCount++ % 200== 0){
            qInfo()<<"encode...";
        }
        ret = media2Avi_->encode(data,size);
    }
    return ret;
}

Media2Avi* Media2AviAgent::initMedia(const int codeType, int w, int h){
    Media2Avi* media = nullptr;

    if(codeType == H26X){
        media = new H26x2Avi;
    }else if (codeType == JPEG) {
        media = new H26x2Avi;
    }

    if(media){
        if(!media->initVideoCodec(codeType,w,h)){
            media->endEncode();
            delete media;
            media = nullptr;
        }
    }

    return media;
}

void Media2AviAgent::endEncode(){
    if(media2Avi_){
       media2Avi_->endEncode();
       delete media2Avi_;
       media2Avi_ = nullptr;
    }
}
