#ifndef MPEG2AVI_H
#define MPEG2AVI_H

#include <QObject>
#include "media2Avi.h"

class Mpeg2Avi : public Media2Avi
{
    Q_OBJECT
public:
    Mpeg2Avi();

    bool initVideoCodec(const int codeType,int width,int height);

    bool encode(unsigned char* data,int size);

    bool endEncode();

private:
    AVStream*               outStream_;
    AVPacket*               pkt_;
    AVFormatContext *       ofmtCtx_;
};

#endif // MPEG2AVI_H
