#ifndef CodecFfmpegToAvi_H
#define CodecFfmpegToAvi_H

#include <QObject>
#include <QString>
#include <QSemaphore>
#include <queue>
#include <mutex>
#include <iostream>
#include "sdk.h"
extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libswscale/swscale.h"
    #include <libavutil/imgutils.h>
    #include <libavutil/samplefmt.h>
    #include <libavutil/timestamp.h>
};

#include "utils.h"

class Media2Avi: public QObject
{
    Q_OBJECT
public:
    Media2Avi();
    virtual bool initVideoCodec(const int codeType,int width,int height)=0;

    virtual bool encode(unsigned char* data,int size)=0;

    virtual bool endEncode()=0;

    enum CodecStatus{
        uninit,
        successed,
        failed
    };

    CodecStatus codecStatus();
protected:
    CodecStatus status_;
};

/**
 * @brief The Media2AviAgent class
 */
class Media2AviAgent{
public:
    static Media2AviAgent& instance(){
        static Media2AviAgent inst;
        return inst;
    }

    Media2AviAgent();

    Media2Avi* initMedia(const int codeType, int w, int h);

    bool encode(const int codeType, int w, int h, unsigned char* data,int size);

    void endEncode();

    void setClickState(bool clicked);

    bool clickState_;
private:
    Media2Avi* media2Avi_;
};

#endif // CodecFfmpegToAvi_H
