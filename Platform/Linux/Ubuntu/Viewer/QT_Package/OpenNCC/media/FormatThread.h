#ifndef FORMATTHREAD_H
#define FORMATTHREAD_H

#include <QThread>
#include <QString>

#define __STDC_CONSTANT_MACROS

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

class FormatThread : public QThread
{
     Q_OBJECT

public:
     explicit FormatThread();
     ~FormatThread();

     void run();

signals:
    void sig(int flag,QString strValue,int value);

private:
    AVFormatContext *pFormatCtx;
    AVPacket packet;

    int videoStreamIdx;
    uint64_t fileLen;
    QString outFileName;
    QString inFileName;
public:
    int openUrl(QString url);
    void setOutFileName(QString fileName);
};


class ImgThread : public QThread
{
     Q_OBJECT

public:
     explicit ImgThread();
     ~ImgThread();

     void run();

signals:
    void sig(int flag,QString strValue,int value);

private:
    AVFormatContext *pFormatCtx;
    AVFrame *pFrame;
    AVCodecContext *pCodecCtx;
    AVPicture  picture;
    SwsContext *pSwsCtx;
    AVCodec *pCodec;
    AVPacket packet;
    int frameRate;

    int videoStreamIdx;
    uint64_t fileLen;
    QString outPath;
    int imgFormat;
    int intervalSec;

    int width;
    int height;

public:
    int openUrl(QString url);
    void setOutPath(QString path);
    void setImgFormat(int flag);
    void setIntervalSec(int sec);
};

#endif // FORMATTHREAD_H
