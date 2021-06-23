#include "mythread.h"
#include "widget.h"
#include <QThread>
#include <QDebug>
#include <QDateTime>
#include <QApplication>
#include "ncc_sdk/Fp16Convert.h"
#include "ncc_sdk/cameraCtrl.h"
#include "ncc_sdk/sdk.h"
#include "decodevideo.h"
#include <opencv2/highgui/highgui.hpp>
#include "utils.h"
#include <QDir>

#include "ui_widget.h"
#include "media/media2Avi.h"

#include <QDebug>
#define MAX_FRAME_ROWS  2180
#define MAX_FRAME_COLS  3872
#define BYTES_PIXELS    (2)
#define NCC_HEADER_SIZE  sizeof(frameSpecOut)
#define MAX_FRAME_BUF_SIZE  (NCC_HEADER_SIZE + MAX_FRAME_ROWS*MAX_FRAME_COLS*BYTES_PIXELS)
#define printf qDebug
#define MAX_STREAM_BUF_SIZE (10000000/8) // 10MB, for H.264 H.265


extern void  vehicle_license_plate_detection_barrier(void *data, int w, int h, float scale, char *name, int nn_fov_show, Network1Par *nnParm1,Network2Par *nnParm2, char *nnret, float min_score,int ftime,int RES,char* id,bool showstate,int flow_fps);
extern float scale;
extern float min_score;
extern bool g_run;
extern bool showstate;
extern char m_id[50];
extern bool acc;
extern int ftime;
extern double history_T_sum,Tmean,FPSmean;
extern int flow_fps,fps_log,history_size;
QList<long long> history_t;
bool two_net_model = true;
extern long long T;




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

MyThread::MyThread(QObject *parent) : QObject(parent)
{

}

void MyThread::load_2net_model(int Video_type,int modeId,int h26x_type)
{
    int continueReadedFailedCount = 0;
    if(g_run)
    {
        g_run = false;
        QThread::sleep(1);
    }
    int ret = loadFw("/Configuration/moviUsbBoot","/Configuration/fw/flicRefApp.mvcmd");
    if (ret<0)
    {
        QApplication::restoreOverrideCursor();
        emit send_log_info_to_UI(tr("load firmware to OpenNCC failed! status:%1").arg(ret));
        return;
    }
    else {
        qInfo()<<"Successed loadfw 2net!";
        emit send_log_info_to_UI(tr("usb sersion: %1").arg(get_usb_version()));
        //2. 获取camera参数
        SensorModesConfig cameraCfg;
        SensorModesList list;
        camera_control_get_features(&list);
        for(int i=0;i<list.num;i++)
        {
            SensorModesConfig features;
            memcpy(&features, &list.mode[i], sizeof(features));
            emit send_log_info_to_UI("========================");
            emit send_log_info_to_UI(tr("modelIndex: %1,moduleName: %2").arg(i).arg(features.moduleName));
            emit send_log_info_to_UI(tr("camWidth: %1,camHeight: %2").arg(features.camWidth).arg(features.camHeight));
            emit send_log_info_to_UI(tr("camFps: %1").arg(features.camFps));
        }

        if(manualset_exp==1)
        {
            camera_control_ae_set_exp(mexp_time);
            camera_control_ae_set_gain(mexp_gain);
        }
        else if(manualset_exp==0)
        {
            camera_control_ae_mode(CAMERA_CONTROL__AE_AUTO__FLASH_MODE__AUTO);
        }

        //            msensorModeId = ui->outflow_mode->currentIndex(); //1080P模式
        int sensorModeId = modeId; //4K模式
        camera_select_sensor(sensorModeId);
        memcpy(&cameraCfg, &list.mode[sensorModeId], sizeof(cameraCfg));//select camera info
        cnn1PrmSet.imageWidth  = cameraCfg.camWidth;
        cnn1PrmSet.imageHeight = cameraCfg.camHeight;



        //4. 配置模型参数
        if(h26x_type==2)
            cnn1PrmSet.mode=ENCODE_H265_MODE;
        else if(h26x_type==1)
            cnn1PrmSet.mode=ENCODE_H264_MODE;


        //////////2级模型demo/////////
        // 4. 1 第一级模型初始化初始化
        cnn1PrmSet.startX      = 0;
        cnn1PrmSet.startY      = 0;
        cnn1PrmSet.endX        = cameraCfg.camWidth;
        cnn1PrmSet.endY        = cameraCfg.camHeight;
        cnn1PrmSet.inputDimWidth  = 300;
        cnn1PrmSet.inputDimHeight = 300;
        /* 级联下一级模型 */
        cnn1PrmSet.modelCascade = 1;

        cnn1PrmSet.inferenceACC=acc?1:0;

        //4.2 第二级模型初始化
        if(cnn1PrmSet.modelCascade !=0)
        {
            /* 基于第一级的检测结果，适当微调第一级的检测坐标，有利于识别 */
            /*
                      *起点向左上方微调（startXAdj，startYAdj ）
                      *底点向右下方微调（endXAdj，endYAdj） */
            cnn2PrmSet.startXAdj  = -5;
            cnn2PrmSet.startYAdj  = -5;
            cnn2PrmSet.endXAdj   = 5;
            cnn2PrmSet.endYAdj   = 5;


            cnn2PrmSet.minConf= min_score;  // 置信度设置，第一级目标检测大于这个值，才进入第二级模型处理
            cnn2PrmSet.labelMask[2] = 1;     // 车牌 label id = 2，车辆 label id = 1, 只处理id=2

            /*name1: "data" , shape: [1x3x24x94] - An input image in following format [1xCxHxW]. Expected color order is BGR.*/
            cnn2PrmSet.inputDimWidth  = 94;
            cnn2PrmSet.inputDimHeight = 24;
            /*
                      *URL:  https://docs.openvinotoolkit.org/2019_R1.1/_license_plate_recognition_barrier_0001_description_license_plate_recognition_barrier_0001.html
                      * name2: "seq_ind" , shape: [88,1] - An auxiliary blob that is needed for correct decoding. Set this to [0, 1, 1, ..., 1]. */
            //多输入初始化，转换成FP16
            uint16_t *p = (uint16_t *)cnn2PrmSet.extInputs;
            p[0] = (uint16_t)f32Tof16(0);
            for(int i=1;i<88;i++)
            {
                p[i] = (uint16_t)f32Tof16(1.0);
            }
        }

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
        //////////////////////////////////////模型参数初始化 end///////////////////////////////////////////////////
        //5. sdk初始化
        //             struct timeval tpstart,tpend;
        //             float timeuse;
        //             gettimeofday(&tpstart,NULL);
        if(cnn1PrmSet.mode==ENCODE_H264_MODE)
            printf("\n cam_info.mode==ENCODE_H264_MODE \n ");
        else if(cnn1PrmSet.mode==ENCODE_H265_MODE)
            printf("\n cam_info.mode==ENCODE_H265_MODE \n ");
        ret = sdk_net2_init(0,
                            0,
                            blob1Array,
                            &cnn1PrmSet,
                            sizeof(cnn1PrmSet),
                            blob2Array,
                            &cnn2PrmSet,
                            sizeof(cnn2PrmSet));

        //             gettimeofday(&tpend,NULL);
        //             timeuse=(1000000*(tpend.tv_sec-tpstart.tv_sec) + tpend.tv_usec-tpstart.tv_usec)/1000.0;
        //             qDebug()<<timeuse<<"ms";

        if (ret<0)
        {
            qInfo()<<"Failed to sdk_net2_init";
            QApplication::restoreOverrideCursor();
            emit send_log_info_to_UI(tr("failed to init the device! status:%1").arg(ret));
            return;
        }
        else {
            qInfo()<<"Successed to sdk_net2_init";
            //5. 输出配置
            char *recvImageData     = (char*) malloc(sizeof(frameSpecOut) + cameraCfg.camWidth * cameraCfg.camHeight * 3 / 2);
            int  meteDataSize  = 5*1024*1024;
            char *recvMetaData = (char*) malloc(meteDataSize);
            if ((recvImageData == 0) || (recvMetaData ==0))
                return;


            NCCVideoCtrl(Video_type,2,h26x_type);
            QApplication::restoreOverrideCursor();
            char src[64];
            two_net_model = true;
            history_t.clear();
            history_T_sum=0;

            streamrun=1;
            while(two_net_model)
            {
                frameSpecOut hdr;
                //                    scale = 960 * 1.0 / cameraCfg.camWidth;
                int maxReadSize;
                char *pFrame=NULL, *metaData;
                int w,h,pixfmt;

                int RES=0;

                if(sensorModeId==0)
                    RES=1;
                else if(sensorModeId==1)
                    RES=2;

                switch(Video_type)
                {
                case YUV420p:
                {
                    //阻塞读取yuv420数据
                    maxReadSize  = sizeof(frameSpecOut)+ cameraCfg.camWidth * cameraCfg.camHeight * 3 / 2;
                    if (read_yuv_data(recvImageData, &maxReadSize, 0) < 0)
                    {
                        QThread::sleep(1);
                        continueReadedFailedCount++;
                        break;
                    }
                    continueReadedFailedCount = 0;
                    memcpy(&hdr, recvImageData, sizeof(frameSpecOut));
                    QString model_name = tr("%1").arg(cameraCfg.moduleName);
                    if (model_name == "ar0234")
                    {
                        RES=1;
                    }
                }
                    break;
                case H26X:
                {

                     maxReadSize  = MAX_FRAME_BUF_SIZE;
                    if (read_26x_data(recvImageData, &maxReadSize, 0) < 0)
                    {
                        QThread::sleep(1);
                        continueReadedFailedCount++;
                        break;
                    }
                    continueReadedFailedCount = 0;
                    DecodeFrame((unsigned char *)(recvImageData +sizeof(frameSpecOut)),maxReadSize,(unsigned char *)recvImageData,&w,&h,&pixfmt);
                    memcpy(&hdr, recvImageData , sizeof(frameSpecOut));
                    QString model_name = tr("%1").arg(cameraCfg.moduleName);
                    if (model_name == "ar0234"&&RES==2)
                    {
                        cameraCfg.camWidth=1280;
                        cameraCfg.camHeight=720;
                        RES=3;
                    }
                    Media2AviAgent::instance().encode(Video_type,
                                                      w,
                                                      h,
                                                      (unsigned char *)(recvImageData  +sizeof(frameSpecOut)),
                                                      maxReadSize);
                }
                    break;
                case JPEG:
                {

                     maxReadSize  = MAX_FRAME_BUF_SIZE;
                    if (read_jpg_data(recvImageData, &maxReadSize, 0) < 0)
                    {
                        QThread::sleep(1);
                        continueReadedFailedCount++;
                        break;
                    }
                    continueReadedFailedCount = 0;

                    DecodeFrame((unsigned char *)(recvImageData  +sizeof(frameSpecOut)),maxReadSize,(unsigned char *)recvImageData,&w,&h,&pixfmt);
                    memcpy(&hdr, recvImageData, sizeof(frameSpecOut));
                    QString model_name = tr("%1").arg(cameraCfg.moduleName);
                    if (model_name == "ar0234")
                    {
                        RES=1;
                    }
                    Media2AviAgent::instance().encode(Video_type,
                                                      w,
                                                      h,
                                                      (unsigned char *)(recvImageData  +sizeof(frameSpecOut)),
                                                      maxReadSize);
                }
                    break;
                default:
                    break;

                }



                //非阻塞读取metedata数据
                maxReadSize = meteDataSize;
                if (read_meta_data(recvMetaData, &maxReadSize, false) == 0)
                {
                    memcpy(&hdr, recvMetaData, sizeof(frameSpecOut));
                    ftime=hdr.res[8];
                }

                if(continueReadedFailedCount != 0){
                    if(continueReadedFailedCount > 1500){
                        continueReadedFailedCount = 0;
                        break;
                    }
                    continue;
                }

                fps_log++;
                if(fps_log%10==0)
                {
                    static QElapsedTimer*lostTimer=NULL;
                    if(NULL==lostTimer)
                    {
                        lostTimer=new QElapsedTimer;
                    }
                    else
                    {
                        T=lostTimer->elapsed();
                    }
                    lostTimer->restart();

                    if(T>1000)
                        T=1000;


                    history_t.append(T);
                    history_size=history_t.size();
                    history_T_sum+=T;

                    if(history_size>4)
                    {
                        history_T_sum-=history_t.at(0);
                        //printf("%f",history_T.at(0));

                        history_t.removeFirst();
                        history_size=history_t.size();
                    }

                    Tmean=history_T_sum/(history_size);
                    FPSmean=10000/Tmean;

                    if(FPSmean-floor(FPSmean)>=0.5)
                        flow_fps=ceil(FPSmean);
                    else
                        flow_fps=floor(FPSmean);
                    //printf("%f  %d",Tmean,flow_fps);
                }

                //提取YUV数据和metedata数据
                pFrame  = (char*) recvImageData;
                if(YUV420p == Video_type){
                    pFrame = recvImageData + sizeof(frameSpecOut);
                }
                metaData = (char*) recvMetaData + sizeof(frameSpecOut);
                //测试显示
                sprintf(src, "demo_video_%dx%d@%dfps", cameraCfg.camWidth, cameraCfg.camHeight, cameraCfg.camFps);


                vehicle_license_plate_detection_barrier( pFrame , cameraCfg.camWidth, cameraCfg.camHeight, scale,src,true, &cnn1PrmSet,&cnn2PrmSet, metaData, min_score,ftime,RES,m_id,showstate,flow_fps);
                if (!cvGetWindowHandle(src))
                {
                    two_net_model = false;
                    cv::destroyWindow(src);
                    break;
                }
            }
            streamrun=0;
            //run finsh
            emit send_log_info_to_UI("exit show video");
            if(cvGetWindowHandle(src))
                cv::destroyWindow(src);
            sdk_uninit();
            UninitDecoder();
            Media2AviAgent::instance().endEncode();
        }
    }
}

void MyThread::close_2nd_model()
{
    two_net_model = false;
}

