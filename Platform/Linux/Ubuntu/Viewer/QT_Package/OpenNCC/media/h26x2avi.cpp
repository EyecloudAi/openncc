#include "h26x2avi.h"
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <QThread>
QString all_filename;
QString p_path;
QString time_;
H26x2Avi::H26x2Avi(){
    bWriter_ = nullptr;
    myformat_thread = new FormatThread;
}

bool H26x2Avi::initVideoCodec(const int codeType,int width,int height){

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
    p_path = pPath;
    QDateTime currentDateTime =QDateTime::currentDateTime();
    QString currentDate =currentDateTime.toString("yyyy-MM-dd-hh-mm-ss-zzz");
    time_ = currentDate;
    QString fileName = pPath + "/"
            + currentDate + ".avi.1";
    all_filename = fileName;
    if(!bWriter_){
        bWriter_ = new BinaryWriter;
    }

    bool openRet = bWriter_->open(fileName.toStdString());
    if(!openRet){
        delete bWriter_;
        bWriter_ = nullptr;
        return false;
    }

    status_ = CodecStatus::successed;
    return true;
}

bool H26x2Avi::encode(unsigned char* data,int size){
    if(!bWriter_){
        return false;
    }
    bWriter_->append(data,size);
    return true;
}

bool H26x2Avi::endEncode(){
    if(!bWriter_){
        return false;
    }
    bWriter_->close();


    QThread::sleep(2);
    qDebug()<<"end encode";
#if 1
    myformat_thread->openUrl(all_filename);
    QString output_filename = p_path.append(QString("/")).append(time_).append(QString(".avi"));
    //qDebug()<<output_filename;
    myformat_thread->setOutFileName(output_filename);
    myformat_thread->start();
#endif
    return true;
}
