#include "widget.h"
#include "ui_widget.h"
#include <QDesktopServices>
#include <QTextBrowser>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QStringList>
#include "cameraCtrl.h"
#include <opencv2/highgui/highgui.hpp>
#include "decodevideo.h"
#include "utils.h"
#include "media/media2Avi.h"
//#include <sys/time.h>
#include<QElapsedTimer>



#include <QDebug>
#define printf qDebug

#ifndef _WINDOWS
#include "Fp16Convert.h"
#else
#include <windows.h>
extern "C" int gettimeofday(struct timeval *tp, void *tzp);
#pragma warning(disable:4996)
#endif


//#define CALL_BACK
#define READ_DATA
//#define DL_IMG

/* 1. 物体分类demo程序 */
//#define  DEMO_CLS   1
/* 2. 人脸检测demo程序 */
#define  DEMO_FACE_DETECTION  1

#define APP_VERSION "NV02.101.81"

#define  OPENCV_SHOW_SCALE    (0.8)    /* 显示缩放系数 */
extern "C"  void os_sleep(int ms);
extern bool two_net_model;
static char cvname[64];
bool g_run = false;
frameSpecOut* temp;
QString usb_version;
float scale;
float min_score;
int RES;
int ftime;
//char *id;
bool showstate;
bool acc=0;
QDir dir;
QString current_t;
char m_id[50];
//double T=0;
double history_T_sum,Tmean,FPSmean;
int flow_fps,fps_log,history_size;
QList<long long> history_T;
long long T;
//struct timeval tpstart,tpend,tpend2;



#define MAX_FRAME_ROWS  2180
#define MAX_FRAME_COLS  3872
#define BYTES_PIXELS    (2)
#define NCC_HEADER_SIZE  sizeof(frameSpecOut)
#define MAX_FRAME_BUF_SIZE  (NCC_HEADER_SIZE + MAX_FRAME_ROWS*MAX_FRAME_COLS*BYTES_PIXELS)
static char max_frame_buffer[MAX_FRAME_BUF_SIZE]; //TODO: This buffer need malloc after call camera_control_get_features
#define MAX_STREAM_BUF_SIZE (10000000   /8) // 10MB, for H.264 H.265
static char max_stream_buffer[MAX_STREAM_BUF_SIZE]; //TODO: This buffer need malloc after call camera_control_get_features
static char metadata[1024*1024]; //TODO: depend on model's output format;Could us get size as call camera_control_get_features?

static SensorModesConfig cameraCfg;
static SensorModesList   list;
QStringList valid_data_list;
QString cropx1,cropy1,cropx2,cropy2;
extern encodeMode x26mode;
extern int h26x_type;


//static CameraInfo cam_info;
static Network1Par cam_info;


extern void  fd_show_img_func(void *data, int w, int h, float scale, char *name, int nn_fov_show, Network1Par *nnparm, char *nnret,float min_score,int ftime,int RES,char *id,bool showstate,int flow_fps);
extern void  cls_show_img_func(void *data, int w, int h, float scale, char *name, int nn_fov_show, Network1Par *nnparm, char *nnret,float min_score,int ftime,int RES,char *id,bool showstate,int flow_fps);
extern void  obj_show_img_func(void *data, int w, int h, float scale, char *name, int nn_fov_show, Network1Par *nnparm, char *nnret,float min_score,int ftime,int RES,char *id,bool showstate,int flow_fps);
//extern void  mask_show_img_func(void *data, int w, int h, float scale, char *name, int nn_fov_show, CameraInfo *nnparm, char *nnret,float min_score);

typedef void  (*analyzeMetedata)(void *data, int w, int h, float scale, char *name, int nn_fov_show, Network1Par *nnparm, char *nnret,float min_score,int ftime,int RES,char *id,bool showstate,int flow_fps);
static analyzeMetedata fun;


static QSqlDatabase loadDb(const QString& dbName){
    QString exePath = QCoreApplication::applicationDirPath();

    QString dbPath = exePath + dbName;

#ifdef Q_OS_WIN
    // 默认使用linux风格，当系统为win时，转成win风格的路径
    dbPath = QDir::toNativeSeparators(dbPath);
#endif

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);
    return db;
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //样式初始化
    setWindowIcon(QIcon(":/image/Window_icon.png"));
    setWindowTitle("OpenNCC View");
    ui->scale_spinbox->setValue(50);
    ui->min_score_spinbox->setValue(50);
    ui->outflow_mode->addItem("1080P");
    ui->yuv_stream->hide();
    ui->two_net_model->hide();
    ui->version_label->setText(APP_VERSION);
    ui->checkBox->hide();
    ui->exposure_mode->addItem("Auto");
    ui->exposure_mode->addItem("Manual");
    ui->h264_stream->setEnabled(false);
    ui->h265_stream->setEnabled(false);
    ui->mjpeg_stream->setEnabled(false);

    //模型选项初始化
    ui->model_1st->addItem("None");
    //    ui->model_2nd->addItem("None");

    QSqlDatabase db = loadDb("/Configuration/NCC.db");
    if (!db.open())
    {
        QMessageBox::warning(this,"Loading database error","Failed to load default sqlite database!\n\n"
                                                           "You need reinstall your NCC View or add a modle first.");
    }
    else {
        QSqlQuery query;
        query.exec("SELECT * FROM ncc_model where parentId = 1 or parentId = 3");
        while (query.next())    {
            ui->model_1st->addItem(QString(query.value("model_name").toString()));
        }
    }

    thread = new QThread(this);
    MyT = new MyThread();
    MyT->moveToThread(thread);
    thread->start();

    add_blob_file = new add_blob();
    if(add_blob_file == NULL)
        return;
    //singals and slots
    connect(ui->roi_setting_area,&roi_label::StartPointSignal,this,&Widget::update_StartPoint);
    connect(ui->roi_setting_area,&roi_label::StopPointSignal,this,&Widget::update_StopPoint);

    //add/del blob
    connect(add_blob_file,&add_blob::update_model_1st_list_signal,this,&Widget::update_model_1st_list);
    connect(this,&Widget::clear_blob_path,add_blob_file,&add_blob::clear_blob_path_record);

    //open browser
    connect(ui->web_label1,&browserlabel::OpenBrowser,[=](){
        QDesktopServices::openUrl(QUrl(QString("https://docs.openvinotoolkit.org/2019_R1.1/usergroup1.html")));
    });
    connect(ui->web_label3,&browserlabel::OpenBrowser,[=](){
        QDesktopServices::openUrl(QUrl(QString("https://www.openncc.com.cn/")));
    });

    connect(this,&Widget::start_2net_model,MyT,&MyThread::load_2net_model);
    connect(MyT,&MyThread::send_log_info_to_UI,this,&Widget::show_2net_model_log);
    connect(MyT,&MyThread::close_1st_net_model,this,&Widget::close_1st_model);
    connect(this,&Widget::close_2net_model,MyT,&MyThread::close_2nd_model);

}

Widget::~Widget()
{
    if(thread->isRunning())
    {
        thread->quit();
        thread->wait();
    }
    delete ui;
}

void Widget::model_1st_changed()
{
    if (ui->model_1st->currentText() == "None" || ui->model_1st->currentText() == NULL)
    {
        ui->model_2nd->clear();
        ui->model_2nd->addItem("None");
    }
    else
    {
        QString exePath = QCoreApplication::applicationDirPath();
        QString check_file = exePath +"/Configuration/blob/" +ui->model_1st->currentText() +".blob";
#ifdef Q_OS_WIN
        // 默认使用linux风格，当系统为win时，转成win风格的路径
        check_file = QDir::toNativeSeparators(check_file);
#endif
        qDebug()<<check_file;
        if (!QFile::exists(check_file)){
            //            QString link =
            //            QString msg = tr("No blob file corresponding to the option was found! please visit the link as below to get files:<br><a href='%1'>Intel models</a>").arg(QDesktopServices::openUrl(QUrl(QString("http://aiyo.eyecloud.tech")));
            QMessageBox::critical(this,"option error","No blob file corresponding to the option was found! please visit the link as below to get files");
            ui->model_1st->setCurrentIndex(0);
        }
        else {
            //按选项查询数据库，获取对应模型信息
            QSqlQuery query;
            QString str= QString("SELECT * FROM ncc_model where model_name='%1'").arg(ui->model_1st->currentText());
            query.exec(str);
            query.first();
            //ui->model_info1->setText(query.value("model_info").toString());
            int model_id = query.value("id").toInt();
            //根据查到的模型id，获取其关联的子模型
            QString str2 = QString("select ncc_model.* from test inner join ncc_model on test.child_id = ncc_model.id where test.id=%1;").arg(model_id);
            query.exec(str2);
            ui->model_2nd->clear();
            ui->model_2nd->addItem("None");
            while (query.next())
            {
                ui->model_2nd->addItem(QString(query.value("model_name").toString()));
            }
        }
    }

    if(ui->model_1st->currentIndex() != 9)
    {
        ui->model_2nd->setDisabled(true);
        ui->label_3->setDisabled(true);
    }
    else
    {
        ui->model_2nd->setDisabled(false);
        ui->label_3->setDisabled(true);
    }
}

void Widget::download_model_file(QString model_name)
{

}

void Widget::model_2nd_changed()
{
    if (ui->model_2nd->currentText()=="None")
    {
        choose_2nd_model_status(0);
        //ui->label_2->setText("please select your 2nd model");
    }
    else
    {
        if(ui->h264_stream->isChecked() || ui->h265_stream->isChecked() || ui->mjpeg_stream->isChecked())
        {
            ui->two_net_model->setEnabled(false);
        }
        choose_2nd_model_status(1);
        QSqlQuery query;
        QString str= QString("SELECT * FROM ncc_model where model_name='%1'").arg(ui->model_2nd->currentText());
        query.exec(str);
        query.first();
        //ui->label_2->setText(query.value("model_info").toString());
    }
}

void Widget::choose_2nd_model_status(int type)
{
    if(type == 0)
    {
        ui->two_net_model->hide();
        ui->load_fw_btn->show();
    }
    else
    {
        ui->load_fw_btn->hide();
        ui->two_net_model->show();
    }
}

void Widget::compare_coordinate(int a,int b){
    if (a<b){
        int temp = a;
        a = b;
        b = temp;
    }
}

void Widget::on_load_fw_btn_clicked()
{



    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    if(two_net_model)
    {
        two_net_model = false;
        QThread::sleep(1);
    }

    cam_info =
    {
        -1,  //imageWidth
        -1,  //imageHeight
        -1,  //startX
        -1,  //startY
        -1,   //endX
        -1,   //endY
        0,  //inputDimWidth
        0,   //inputDimHeight                   /* <dim>300</dim>  <dim>300</dim> */
        IMG_FORMAT_BGR_PLANAR,      //IMAGE_FORMAT
        0,                    //meanValue
        0,
        0 ,
        1,                         //stdValue
        1,                           /*打开YUV420输出功能*/
        1,                           /*打开H26X编码功能*/
        1,                           /*打开MJPEG编码功能*/
        x26mode,            /* 使用H264编码格式 */
        NULL,
        0,
        0
    };

    if(!ui->mjpeg_stream->isChecked()&&!ui->yuv_stream->isChecked()&&!ui->h264_stream->isChecked()&&!ui->h265_stream->isChecked()){
        QMessageBox::warning(this,"please select your Stream Format","Please select Stream Format first.");
        QApplication::restoreOverrideCursor();
    }

    else {
        if (g_run){
            g_run = 0;
            cv::destroyWindow(cvname);
            ui->log_area->append(tr("exit show video"));
            sdk_uninit();
            UninitDecoder();
        }
        if (ui->yuv_stream->isChecked()){
            mvideo_type = YUV420p;
        }
        else if (ui->h264_stream->isChecked()||ui->h265_stream->isChecked()) {
            mvideo_type = H26X;
        }
        else if (ui->mjpeg_stream->isChecked()){
            mvideo_type = JPEG;
        }

        int ret = 0;
        memset(metadata, 0, sizeof(metadata));



        ret = loadFw("/Configuration/moviUsbBoot","/Configuration/fw/OpenNcc.mvcmd");


        if (ret<0)
        {
            QApplication::restoreOverrideCursor();
            ui->log_area->append("load firmware to OpenNCC failed!");
        }
        else {
            ui->log_area->append(tr("usb sersion: %1").arg(get_usb_version()));

            //2. 获取camera参数
            camera_get_fw_version(m_version);
            camera_get_ncc_id(m_id);
            camera_control_get_features(&list);
            for(int i=0;i<list.num;i++)
            {
                SensorModesConfig features;
                memcpy(&features, &list.mode[i], sizeof(features));
                ui->log_area->append("========================");
                ui->log_area->append(tr("modelIndex: %1,moduleName: %2").arg(i).arg(features.moduleName));
                ui->log_area->append(tr("camWidth: %1,camHeight: %2").arg(features.camWidth).arg(features.camHeight));
                ui->log_area->append(tr("camFps: %1").arg(features.camFps));
            }
            //设置曝光模式
            if(manualset_exp==1)
            {
                camera_control_ae_set_exp(mexp_time);
                camera_control_ae_set_gain(mexp_gain);
            }
            else if(manualset_exp==0)
            {
                camera_control_ae_mode(CAMERA_CONTROL__AE_AUTO__FLASH_MODE__AUTO);
            }


            printf("\n\n manualset_exp is %d\n\n",manualset_exp);

            msensorModeId = ui->outflow_mode->currentIndex(); //1080P模式
            //msensorModeId = 1; //4K模式
            camera_select_sensor(msensorModeId);
            memcpy(&cameraCfg, &list.mode[msensorModeId], sizeof(cameraCfg));//select camera info

            cam_info.imageWidth  = cameraCfg.camWidth;
            cam_info.imageHeight = cameraCfg.camHeight;

            // 3. 算法有效区域初始冿
            if (cropx1.isEmpty()||cropx2.isEmpty()||cropy1.isEmpty()||cropy2.isEmpty()||cropx1==cropx2||cropy1==cropy2)
            {
                cam_info.startX      = 0;
                cam_info.startY      = 0;
                cam_info.endX        = cameraCfg.camWidth;
                cam_info.endY        = cameraCfg.camHeight;
            }
            else {
                int x1,y1,x2,y2;
                switch (ui->outflow_mode->currentIndex()) {
                case(0):
                    x1 = cropx1.toInt()*5;
                    x2 = cropx2.toInt()*5;
                    y1 = cropy1.toInt()*5;
                    y2 = cropy2.toInt()*5;
                    break;
                case(1):
                    x1 = cropx1.toInt()*10;
                    x2 = cropx2.toInt()*10;
                    y1 = cropy1.toInt()*10;
                    y2 = cropy2.toInt()*10;
                    break;
                }
                compare_coordinate(x1,x2);
                compare_coordinate(y1,y2);
                cam_info.startX      = x1;
                cam_info.startY      = y1;
                cam_info.endX        = x2;
                cam_info.endY        = y2;
            }
            ui->log_area->append(tr("ROI Region(Right Top:(%1,%2) Left Bottom:(%3,%4))").arg(cam_info.startX).arg(cam_info.startY).arg(cam_info.endX).arg(cam_info.endY));



            static std::vector<QString> blobVecs={
                "",
                "/Configuration/blob/classification-fp16.blob",
                "/Configuration/blob/face-detection-retail-0004-fp16.blob",
                "/Configuration/blob/face-detection-adas-0001-fp16.blob",

                "/Configuration/blob/person-detection-retail-0013-fp16.blob",
                "/Configuration/blob/person-vehicle-bike-detection-crossroad-0078-fp16.blob",
                "/Configuration/blob/face-person-detection-retail-0002-fp16.blob",

                "/Configuration/blob/pedestrian-detection-adas-0002-fp16.blob",
                "/Configuration/blob/vehicle-detection-adas-0002-fp16.blob",
                "/Configuration/blob/vehicle-license-plate-detection-barrier-0106-fp16.blob",

                "/Configuration/blob/pedestrian-and-vehicle-detector-adas-0001-fp16.blob"
            };

            QString exePath = QCoreApplication::applicationDirPath();
            // 4. sdk初始冿

            int index = ui->model_1st->currentIndex();
            QString modelSelectLog("No blob has been selected");

            cam_info.inferenceACC=acc?1:0;
            //printf("inferenceACC %d\n",acc);

            if(index > 0 && index <= 10){

                QString blobFile = blobVecs[index];
                char blob[256];

                blobFile = exePath + blobFile;
#ifdef Q_OS_WIN
                blobFile = QDir::toNativeSeparators(blobFile);
#endif
                strcpy(blob,blobFile.toStdString().c_str());

                modelSelectLog = QString("model %1").arg(index);


                if(cam_info.mode==ENCODE_H264_MODE)
                    printf("\n cam_info.mode==ENCODE_H264_MODE \n ");
                else if(cam_info.mode==ENCODE_H265_MODE)
                    printf("\n cam_info.mode==ENCODE_H265_MODE \n ");


                ret = sdk_init_ex(NULL,NULL,blob, &cam_info, sizeof(cam_info));

            }else{
                ret = sdk_init_ex(NULL,NULL,NULL, &cam_info, sizeof(cam_info));
            }

            ui->log_area->append(modelSelectLog);

            if(1 == index){
                fun = cls_show_img_func;
            }else if(2 == index){
                fun = fd_show_img_func;
            }else{
                fun = obj_show_img_func;
            }

            if (ret<0){
                QApplication::restoreOverrideCursor();
                QString log= QString("failed to init the device,ret=%1").arg(ret);
                ui->log_area->append(log);
            }
            else {
                QApplication::restoreOverrideCursor();
                //5. 输出配置
                int get_log = 0;

                NCCVideoCtrl(mvideo_type,2,h26x_type);
                g_run = true;
                ftime=1;
                history_T_sum=0;
                history_T.clear();

                streamrun=1;

                //TODO:jonzhao 数据获取放到子线程去，UI这里不应该死循环

                while(g_run)
                {
                    get_log++;
                    ret = PostProcessFrame(max_frame_buffer,MAX_FRAME_BUF_SIZE,min_score,scale,cvname);
                    switch (ret) {
                    case(2):
                        os_sleep(1);
                        continue;
                        break;
                    case(3):
                        g_run = 0;
                        cv::destroyWindow(cvname);
                    }
                    if (valid_data_list.size()>0&&get_log%20==0){
                        ui->algo_area->append(valid_data_list.at(valid_data_list.size()-1));
                        valid_data_list.clear();
                    }


                    if (get_log%30==0)
                    {


                        QString temp_info = tr("Temperature CSS = %1 MSS = %2 UPA0 = %3 UPA1 =%4 ID:%5 ver:%6")
                                .arg(temp->res[9]).arg(temp->res[10]).arg(temp->res[11]).arg(temp->res[12])
                                .arg(QString(m_id)).arg(QString(m_version));
                        ui->log_area->append(temp_info);
                    }

                    if (!cvGetWindowHandle(cvname))
                    {
                        qInfo()<<"cvGetWindowHandle close()";
                        g_run = false;
                        cv::destroyWindow(cvname);
                        // emit stop_mydata_thread_timer();
                        break;
                    }
                }
                streamrun=0;
                //run finsh
                ui->log_area->append(tr("exit show video"));
                if(cvGetWindowHandle(cvname))
                    cv::destroyWindow(cvname);
                sdk_uninit();
                UninitDecoder();
                Media2AviAgent::instance().endEncode();
            }
        }
    }
    qInfo()<<"end loop!";
}

QString Widget::current_time()
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    return current_date;
}


int img_num = 0;
bool save_img = false;
int Widget::PostProcessFrame(char* pFrame,int bufsize,float min_score,float scale,char* winhndl)
{
    static int continueReadedFailedCount = 0;

    int w,h,pixfmt,ret;
    char* Postframe = NULL;
    int size = bufsize;
    if(pFrame == NULL || bufsize <= 0 || winhndl == NULL)
        return 1;
    static int frametick = 0;
    frametick++;
    QString img_name;
    FILE *cvtImg;
    if(msensorModeId==0)
        RES=1;
    else if(msensorModeId==1)
        RES=2;
    switch(mvideo_type)
    {
    case YUV420p:
    {
        //如何先确定开的缓存区够大？如何保证数据不溢出？错误返回值？
        ret = read_yuv_data(pFrame,&size,0);
        if (ret<0){
            if(continueReadedFailedCount++ > 1500){
                continueReadedFailedCount = 0;
                return 3;
            }
            if (ret == USB_ERROR_NO_DEVICE){
                return 3;
            }
            else { 
                return 2;

            }
        }
        continueReadedFailedCount = 0;

        QString model_name = tr("%1").arg(cameraCfg.moduleName);
        if (model_name == "ar0234")
        {
            RES=1;
        }

        temp = (frameSpecOut *)pFrame;

    }

        if(save_img)
        {
            img_num++;
            img_name = QString("./%1/%2.yuv").arg(current_t).arg(img_num);
            std::string str = img_name.toStdString();
            const char* ch = str.c_str();
            cvtImg = fopen(ch,"w");
            fwrite(pFrame+sizeof(frameSpecOut),1,size-sizeof(frameSpecOut),cvtImg);
            fclose(cvtImg);
        }
        Postframe = pFrame+sizeof(frameSpecOut);
        break;
    case H26X:
    {
        ret = read_26x_data(max_stream_buffer,&size,0);
        if (ret<0){
            if(continueReadedFailedCount++ > 1500){
                continueReadedFailedCount = 0;
                return 3;
            }
            if (ret == USB_ERROR_NO_DEVICE){
                return 3;
            }
            else {
                return 2;
            }
        }
        continueReadedFailedCount = 0;
        if(save_img)
        {
            img_num++;
            img_name = QString("./%1/%2.jpg").arg(current_t).arg(img_num);
            std::string str = img_name.toStdString();
            const char* ch = str.c_str();
            cvtImg = fopen(ch,"w");
            fwrite(max_stream_buffer+sizeof(frameSpecOut),1,size-sizeof(frameSpecOut),cvtImg);
            fclose(cvtImg);
        }

        QString model_name = tr("%1").arg(cameraCfg.moduleName);
        if (model_name == "ar0234"&&RES ==2)
        {
            cameraCfg.camWidth=1280;
            cameraCfg.camHeight=720;
            RES=3;
        }

        temp = (frameSpecOut *)max_stream_buffer;
        DecodeFrame((unsigned char *)(max_stream_buffer+sizeof(frameSpecOut)),size,(unsigned char *)pFrame,&w,&h,&pixfmt);
        Postframe = pFrame;
        Media2AviAgent::instance().encode(mvideo_type,
                                          w,
                                          h,
                                          (unsigned char *)(max_stream_buffer+sizeof(frameSpecOut)),
                                          size);
    }

        break;
    case JPEG:
    {
        ret = read_jpg_data(max_stream_buffer,&size,0);
        if (ret<0){
            if(continueReadedFailedCount++ > 1500){
                continueReadedFailedCount = 0;
                return 3;
            }
            if (ret == USB_ERROR_NO_DEVICE){
                return 3;
            }
            else {
                return 2;
            }
        }
        continueReadedFailedCount = 0;
        if(save_img)
        {
            img_num++;
            img_name = QString("./%1/%2.jpg").arg(current_t).arg(img_num);
            std::string str = img_name.toStdString();
            const char* ch = str.c_str();
            cvtImg = fopen(ch,"w");
            fwrite(max_stream_buffer+sizeof(frameSpecOut),1,size-sizeof(frameSpecOut),cvtImg);
            fclose(cvtImg);
        }
        QString model_name = tr("%1").arg(cameraCfg.moduleName);
        if (model_name == "ar0234")
        {
            RES=1;
        }
        temp = (frameSpecOut *)max_stream_buffer;
        //        frameSpecOut* spec =(frameSpecOut*) max_stream_buffer;
        DecodeFrame((unsigned char *)(max_stream_buffer+sizeof(frameSpecOut)),size,(unsigned char *)pFrame,&w,&h,&pixfmt);
        Postframe = pFrame;

        Media2AviAgent::instance().encode(mvideo_type,
                                          w,
                                          h,
                                          (unsigned char *)(max_stream_buffer+sizeof(frameSpecOut)),
                                          size);
    }
        break;
    default:
        break;
    }



    size = sizeof(metadata);

    ret=read_meta_data(metadata,&size,0);


    frameSpecOut* out = (frameSpecOut*)metadata;
    if(ret==0)
    {
        ftime=out->res[8];
    }


    //printf("ftime=%d\n",ftime);
    //  id=m_id;
    sprintf(winhndl, "NCC_View_%dx%d@%dfps", cameraCfg.camWidth, cameraCfg.camHeight, cameraCfg.camFps);
    // 检测


    //display frame rate

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

        history_T.append(T);
        history_size=history_T.size();
        history_T_sum+=T;

        if(history_size>12)
        {
            history_T_sum-=history_T.at(0);

            history_T.removeFirst();
            history_size=history_T.size();
        }

        Tmean=history_T_sum/(history_size);

        FPSmean=10000/Tmean;

        if(FPSmean-floor(FPSmean)>=0.5)
            flow_fps=ceil(FPSmean);
        else
            flow_fps=floor(FPSmean);
        //printf("%f  %d",history_T_sum,history_size);
    }
    //printf("fps=%d\n",flow_fps);

    fun(Postframe, cameraCfg.camWidth, cameraCfg.camHeight, scale,winhndl, true, &cam_info, metadata+sizeof(frameSpecOut)+OUTPUT_INDEX_SIZE,min_score,ftime,RES,m_id,showstate,flow_fps);

    return 0;
}


void Widget::yuv_selected()
{
    if (ui->yuv_stream->isChecked()){
        ui->h264_stream->setEnabled(false);
        ui->h265_stream->setEnabled(false);
        ui->mjpeg_stream->setEnabled(false);
//        QString model_name = tr("%1").arg(cameraCfg.moduleName);
//        if (model_name == "ar0234")
//        {
//            ui->outflow_mode->clear();
//            ui->outflow_mode->addItem("1080P 60FPS");
//            ui->outflow_mode->addItem("1080P 120FPS");
//        }
    }
    else {
        ui->h264_stream->setEnabled(true);
        ui->h265_stream->setEnabled(true);
        ui->mjpeg_stream->setEnabled(true);
    }
}

void Widget::h264_selected()
{

    if (ui->h264_stream->isChecked()){
        x26mode=ENCODE_H264_MODE;
        h26x_type=1;
        ui->yuv_stream->setEnabled(false);
        ui->h265_stream->setEnabled(false);
        ui->mjpeg_stream->setEnabled(false);

//        QString model_name = tr("%1").arg(cameraCfg.moduleName);
//        qInfo()<<"h264_selected: model_name="<<model_name;
//        if (model_name == "ar0234")
//        {
//            ui->outflow_mode->clear();
//            ui->outflow_mode->addItem("1080P 60FPS");
//            ui->outflow_mode->addItem("720P 120FPS");
//        }
    }
    else {
        ui->yuv_stream->setEnabled(true);
        ui->h265_stream->setEnabled(true);
        ui->mjpeg_stream->setEnabled(true);
    }
}

void Widget::h265_selected()
{
    if (ui->h265_stream->isChecked()){
        x26mode=ENCODE_H265_MODE;
        h26x_type=2;
        ui->yuv_stream->setEnabled(false);
        ui->h264_stream->setEnabled(false);
        ui->mjpeg_stream->setEnabled(false);
    }
    else {
        ui->yuv_stream->setEnabled(true);
        ui->h264_stream->setEnabled(true);
        ui->mjpeg_stream->setEnabled(true);
    }
}

void Widget::mjpeg_selected()
{
    if (ui->mjpeg_stream->isChecked()){
        ui->h264_stream->setEnabled(false);
        ui->h265_stream->setEnabled(false);
        ui->yuv_stream->setEnabled(false);

//        QString model_name = tr("%1").arg(cameraCfg.moduleName);
//        if (model_name == "ar0234")
//        {
//            ui->outflow_mode->clear();
//            ui->outflow_mode->addItem("1080P 60FPS");
//            ui->outflow_mode->addItem("1080P 120FPS");
//        }
    }
    else {
        ui->h264_stream->setEnabled(true);
        ui->h265_stream->setEnabled(true);
        ui->yuv_stream->setEnabled(true);
    }
}

void Widget::update_StartPoint(){
    int satrt_point_x = ui->roi_setting_area->getx1();
    int satrt_point_y = ui->roi_setting_area->gety1();
    cropx1 = tr("%1").arg(satrt_point_x);
    cropy1 = tr("%1").arg(satrt_point_y);
}

void Widget::update_StopPoint(){
    int end_point_x = ui->roi_setting_area->getx2();
    int end_point_y = ui->roi_setting_area->gety2();
    cropx2 = tr("%1").arg(end_point_x);
    cropy2 = tr("%1").arg(end_point_y);

}

void Widget::update_model_1st_list(){
    QSqlQuery query;
    ui->model_1st->clear();
    ui->model_1st->addItem("None");
    query.exec("SELECT * FROM ncc_model where parentId = 1 or parentId = 3");
    while (query.next()){
        ui->model_1st->addItem(QString(query.value("model_name").toString()));
    }
}

void Widget::on_Add_widget_clicked()
{
    //    add_blob_file = new add_blob();
    add_blob_file->show();
    emit clear_blob_path();
}

void Widget::show_valid_data(QString msg){

    ui->algo_area->append(msg);

}

void Widget::on_Del_widget_clicked()
{
    del_blob_file = new del_blob();
    del_blob_file->show();
}

void Widget::on_check_device_clicked()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    if (g_run){
        g_run = 0;
        cv::destroyWindow(cvname);
        ui->log_area->append(tr("exit show video"));
        sdk_uninit();
        UninitDecoder();
    }
    ui->h264_stream->setEnabled(true);
    ui->h265_stream->setEnabled(true);
    ui->mjpeg_stream->setEnabled(true);

    ui->model_1st->setCurrentIndex(0);
    ui->model_2nd->setCurrentIndex(0);
    int ret = 0;
    //1. 加载固件
    ret = loadFw("/Configuration/moviUsbBoot","/Configuration/fw/OpenNcc.mvcmd");
    if (ret<0)
    {
        QApplication::restoreOverrideCursor();
        ui->log_area->append("load firmware to OpenNCC failed!");
    }
    else {
        usb_version = tr("%1").arg(get_usb_version());
        if (usb_version.toInt()<30){
            ui->yuv_stream->setCheckState(Qt::CheckState(false));
            ui->h264_stream->setCheckState(Qt::CheckState(false));
            ui->h265_stream->setCheckState(Qt::CheckState(false));
            ui->mjpeg_stream->setCheckState(Qt::CheckState(false));
            ui->h264_stream->setEnabled(true);
            ui->h265_stream->setEnabled(true);
            ui->mjpeg_stream->setEnabled(true);
            ui->yuv_stream->hide();
            ui->log_area->append("USB interface is not 3.0, yuv outflow will be disabled");
        }
        else {
            ui->yuv_stream->setCheckState(Qt::CheckState(false));
            ui->h264_stream->setCheckState(Qt::CheckState(false));
            ui->h265_stream->setCheckState(Qt::CheckState(false));
            ui->mjpeg_stream->setCheckState(Qt::CheckState(false));
            ui->h264_stream->setEnabled(true);
            ui->h265_stream->setEnabled(true);
            ui->mjpeg_stream->setEnabled(true);
            ui->yuv_stream->setEnabled(true);
            ui->yuv_stream->show();
            ui->log_area->append("USB interface is 3.0, yuv outflow is currently available");
        }
        //2. 获取camera参数
        camera_control_get_features(&list);
        SensorModesConfig features;
        memcpy(&features, &list.mode[0], sizeof(features));

        ui->outflow_mode->clear();
        for(int i = 0;i < list.num;i++){

            qInfo()<<"features.camWidth="<<list.mode[i].camWidth
                 <<"features.camHeight="<<list.mode[i].camHeight
                   <<"features.camFps="<<list.mode[i].camFps;

            QString modelStr;
            if(list.mode[i].camWidth == 1920 && list.mode[i].camHeight == 1080){

                modelStr = QString("1080P ")
                          + QString::number(list.mode[i].camFps)
                          + QString("FPS");
                ui->outflow_mode->addItem(modelStr);

            }else if(list.mode[i].camWidth == 3840 && list.mode[i].camHeight == 2160){
                modelStr = QString("4K ")
                         + QString::number(list.mode[i].camFps)
                         + QString("FPS");
                ui->outflow_mode->addItem(modelStr);
            }
        }

        ui->exposure_value->setMaximum(features.maxEXP/100);
        ui->exp_time_label->setMaximum(features.maxEXP);
        ui->exp_gain_label->setMaximum(features.maxGain);
        ui->exp_gain_label->setMinimum(features.minGain);



//        QString model_name = tr("%1").arg(features.moduleName);
//        if (model_name == "sc8238"){
//            ui->outflow_mode->clear();
//            ui->outflow_mode->addItem("1080P");
//            ui->outflow_mode->addItem("4K");
//            ui->log_area->append("your device support 1080P and 4K mode.");
//        }
//        else if(model_name == "sc2232"){
//            ui->outflow_mode->clear();
//            ui->outflow_mode->addItem("1080P");
//            ui->log_area->append("your device support 1080P mode only.");
//        }
//        if(0 == model_name.compare("ar0234")){
//            //ui->h265_stream->hide();
//        }else{
//            ui->h265_stream->show();
//        }

        QApplication::restoreOverrideCursor();
        sdk_uninit();
    }
}

void Widget::on_two_net_model_clicked()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    if (ui->yuv_stream->isChecked()){
        mvideo_type = YUV420p;
    }
    else if (ui->h264_stream->isChecked()) {
        mvideo_type = H26X;
        h26x_type=1;
        x26mode=ENCODE_H264_MODE;
    }
    else if (ui->h265_stream->isChecked()) {
        mvideo_type = H26X;
        h26x_type=2;
        x26mode=ENCODE_H265_MODE;
    }
    else if (ui->mjpeg_stream->isChecked()){
        mvideo_type = JPEG;
    }

    int modeId = ui->outflow_mode->currentIndex();

    emit start_2net_model(mvideo_type,modeId,h26x_type);
}

void Widget::on_scale_value_valueChanged(int value)
{
    scale = value*0.01;
}

void Widget::on_min_score_value_valueChanged(int value)
{
    min_score = value*0.01;
}

void Widget::show_2net_model_log(QString msg)
{
    ui->log_area->append(msg);
}

void Widget::close_1st_model()
{
    g_run = false;
}

void Widget::on_checkBox_stateChanged(int arg1)
{
    if(arg1)
    {
        save_img = true;
        current_t = QString("%1").arg(QDateTime::currentDateTime().toString("MMddHHmmss"));
        QString file_path = QCoreApplication::applicationDirPath()
                + "/" + current_t;
        dir = QDir(file_path);
        if(!dir.exists())
        {
            dir.mkpath(file_path);
        }
    }
    else
    {
        save_img =false;
    }
}
void Widget::on_showstate_stateChanged(int arg1)
{
    if(arg1)
    {
        showstate=true;
    }
    else
    {
        showstate=false;
    }
}

void Widget::on_acc_stateChanged(int arg1)
{
    if(arg1)
    {
        acc=true;
    }
    else
    {
        acc=false;
    }
}
void Widget::on_exposure_value_sliderReleased()
{
    if(streamrun==1)
    {
        mexp_time=ui->exposure_value->value()*100;
        ui->exp_time_label->setValue(mexp_time);
        camera_control_ae_set_exp(mexp_time);
    }
    else if(streamrun==0)
    {
        mexp_time=ui->exposure_value->value()*100;
        ui->exp_time_label->setValue(mexp_time);
    }
}

void Widget::on_exp_time_label_valueChanged(int arg1)
{
    if(streamrun==1)
    {
        mexp_time=ui->exp_time_label->value();
        ui->exposure_value->setValue(mexp_time/100);
        camera_control_ae_set_exp(mexp_time);
    }
    else if(streamrun==0)
    {
        mexp_time=ui->exp_time_label->value();
        ui->exposure_value->setValue(mexp_time/100);
    }
}

void Widget::on_exp_gain_label_valueChanged(int arg1)
{
    if(streamrun==1)
    {
        mexp_gain=ui->exp_gain_label->value();
        camera_control_ae_set_gain(mexp_gain);
    }
    else if(streamrun==0)
    {
        mexp_gain=ui->exp_gain_label->value();
    }
}


void Widget::on_exposure_mode_currentIndexChanged(int index)
{


    mexp_mode= ui->exposure_mode->currentIndex();

    if(mexp_mode==0&&streamrun==1)
    {
        ui->exposure_value->hide();
        ui->exp_time_label->hide();
        ui->exp_gain_label->hide();
        ui->label_iso->hide();
        ui->label_exptime->hide();
        manualset_exp=0;

        camera_control_ae_mode(CAMERA_CONTROL__AE_AUTO__FLASH_MODE__AUTO);
    }
    if(mexp_mode==0&&streamrun==0)
    {
        ui->exposure_value->hide();
        ui->exp_time_label->hide();
        ui->exp_gain_label->hide();
        ui->label_iso->hide();
        ui->label_exptime->hide();
        manualset_exp=0;
    }
    else if(mexp_mode==1&&streamrun==1)
    {
        ui->exposure_value->show();
        ui->exp_time_label->show();
        ui->exp_gain_label->show();
        ui->label_iso->show();
        ui->label_exptime->show();
        manualset_exp=1;
        //camera_control_ae_mode(CAMERA_CONTROL__AE_AUTO__FLASH_MODE__OFF);
        ui->exposure_value->setValue((int)(3*ui->exposure_value->maximum()/4));
        ui->exp_time_label->setValue((int)(3*ui->exp_time_label->maximum()/4));
        mexp_time=(int)(3*ui->exp_time_label->maximum()/4);
        ui->exp_gain_label->setValue((int)(ui->exp_gain_label->maximum()/4));
        mexp_gain=(int)(ui->exp_gain_label->maximum()/4);
        camera_control_ae_set_exp(mexp_time);
        camera_control_ae_set_gain(mexp_gain);
    }
    else if(mexp_mode==1&&streamrun==0)
    {
        ui->exposure_value->show();
        ui->exp_time_label->show();
        ui->exp_gain_label->show();
        ui->label_iso->show();
        ui->label_exptime->show();
        manualset_exp=1;
        ui->exposure_value->setValue((int)(3*ui->exposure_value->maximum()/4));
        ui->exp_time_label->setValue((int)(3*ui->exp_time_label->maximum()/4));
        mexp_time=(int)(3*ui->exp_time_label->maximum()/4);
        ui->exp_gain_label->setValue((int)(ui->exp_gain_label->maximum()/4));
        mexp_gain=(int)(ui->exp_gain_label->maximum()/4);
    }
}

void Widget::on_saveAviCheckBox__clicked()
{
    if(!ui->saveAviCheckBox_->isChecked()){
        qInfo()<<"on_saveAviCheckBox__clicked, Is not checked!";
        Media2AviAgent::instance().setClickState(false);
    }else{
        qInfo()<<"on_saveAviCheckBox__clicked, Is checked!";
        Media2AviAgent::instance().setClickState(true);
        if(g_run){
            on_load_fw_btn_clicked();
        }
    }
}

void Widget::on_yuv_stream_stateChanged(int arg1)
{
    if(arg1 == Qt::CheckState::Checked){
        qInfo()<<"yuv is clicked";
        ui->saveAviCheckBox_->setCheckState(Qt::CheckState::Unchecked);
        on_saveAviCheckBox__clicked();
        ui->saveAviCheckBox_->hide();
        ui->two_net_model->setEnabled(true);
    }else if(arg1 == Qt::CheckState::Unchecked){
         qInfo()<<"yuv is not clicked";
        ui->saveAviCheckBox_->show();
        ui->two_net_model->setEnabled(false);
    }
}
