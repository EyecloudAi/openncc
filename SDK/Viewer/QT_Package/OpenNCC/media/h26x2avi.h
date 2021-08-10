#ifndef H26X2AVI_H
#define H26X2AVI_H

#include <QObject>
#include "media2Avi.h"
#include "FormatThread.h"
class H26x2Avi : public Media2Avi
{
public:
    H26x2Avi();

    bool initVideoCodec(const int codeType,int width,int height);

    bool encode(unsigned char* data,int size);

    bool endEncode();

private:
    BinaryWriter* bWriter_;

    FormatThread *myformat_thread;
};

#endif // H26X2AVI_H
