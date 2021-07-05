#include "widget.h"

#include <QApplication>
#include "utils.h"
#include "helper.h"

void test2Net(){

    int ret = loadFw("/Configuration/moviUsbBoot","/Configuration/fw/OpenNcc.mvcmd");
    if(ret < 0){
        qInfo()<<"Failed loadFw!";
        return;
    }

    Network1Par cnn1PrmSet =
    {
        -1, -1,                     /* 动态获取 */
        -1, -1,                                          /* 根据获取的sensor分辨率设置 */
        -1, -1,                                            /*  根据获取的sensor分辨率设置 */
        -1, -1,            /* 根据获取的模型参数设置 */
        IMG_FORMAT_BGR_PLANAR,      /*  默认为BRG输入 */
        {0,0,0},
        1,
        1,                                               /*打开YUV420输出功能*/
        1,                                             /*打开H26X编码功能*/
        1,                                              /*打开MJPEG编码功能*/
        ENCODE_H264_MODE,                          /* 使用H264编码格式 */
        {0},                                                  /* model多输入，第二个输入参数 */
        0,                                           /* 默认不级联下一级模型 */
        0,
    };

    /* 第二级模型默认参数 */
    Network2Par cnn2PrmSet =
    {
        0,
        0,
        0,
        0,
        {0},                                                 /* 需要处理的label掩码，如果对应位置至1才会处理 */
        0.99,                                                /* 检测目标的置信度大于该值才会处理 */
        -1,
        -1,            /* 根据获取的模型参数设置 */
        IMG_FORMAT_BGR_PLANAR,      /*  默认为BRG输入 */
        {0,0,0},
        1,
        {0},                                                  /* model多输入，第二个输入参数 */
        0                                             /* 默认不级联下一级模型 */
    };

    QString exePath = QCoreApplication::applicationDirPath();

    QString blob1File = exePath + "/Configuration/blob/vehicle-license-plate-detection-barrier-0106.blob";
    QString blob2File = exePath + "/Configuration/blob/license-plate-recognition-barrier-0001.blob";

#ifdef Q_OS_WIN
    blob1File = QDir::toNativeSeparators(blob1File);
    blob2File = QDir::toNativeSeparators(blob2File);
#endif

    qInfo()<<"blob1File="<<blob1File;
    qInfo()<<"blob2File="<<blob2File;

    char blob1Array[256];
    char blob2Array[256];

     strcpy(blob1Array,blob1File.toStdString().c_str());
     strcpy(blob2Array,blob2File.toStdString().c_str());

     ret = sdk_net2_init(0,
                        0,
                        blob1Array,
                        &cnn1PrmSet,
                        sizeof(cnn1PrmSet),
                        blob2Array,
                        &cnn2PrmSet,
                        sizeof(cnn2PrmSet));
   if(ret < 0){
       qInfo()<<"Failed sdk_net2_init!";
   }else{
       qInfo()<<"Successed sdk_net2_init!";
   }

}


int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    SetUnhandledExceptionFilter(ExceptionFilter);
#endif
    QApplication a(argc, argv);

    setStyleByCss();
  //  test2Net();
      Widget w;
    w.show();
    return a.exec();
}
