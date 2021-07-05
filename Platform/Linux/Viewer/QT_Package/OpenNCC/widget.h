    #ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QtSql>
#include <QMessageBox>
#include <QFile>
#include <QTextBrowser>
#include <QPushButton>
#include <QComboBox>
#include <QDebug>
#include <QString>
#include <QThread>
#include <QTimer>
#include "my_class/roi_label.h"
#include "my_class/add_blob.h"
#include "my_class/del_blob.h"
#include "sdk.h"
#include "my_class/mythread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();



signals:
    void clear_blob_path();
    void get_valid_algo_data(int w,int h,char *data,float score);
    void close_mydata_thread();
    void stop_mydata_thread_timer();

public slots:
    void update_model_1st_list();

    void download_model_file(QString model_name);


private slots:
    void on_load_fw_btn_clicked();
    //model type
    void model_1st_changed();
    void model_2nd_changed();
    //stream encoding
    void yuv_selected();
    void h264_selected();
    void h265_selected();
    void mjpeg_selected();
    //update coordinate
    void compare_coordinate(int a,int b);
    void update_StartPoint();
    void update_StopPoint();
    //creat thread to load fw and preview
    void show_valid_data(QString msg);

    void on_Add_widget_clicked();

    void on_Del_widget_clicked();

    void on_check_device_clicked();

    QString current_time();

    void on_two_net_model_clicked();

    void on_scale_value_valueChanged(int value);

    void on_min_score_value_valueChanged(int value);

    void show_2net_model_log(QString msg);

    void choose_2nd_model_status(int type);

    void close_1st_model();

    void on_checkBox_stateChanged(int arg1);

    void on_showstate_stateChanged(int arg1);

    void on_acc_stateChanged(int arg1);

    void on_exposure_value_sliderReleased();


    void on_exposure_mode_currentIndexChanged(int index);

    void on_exp_time_label_valueChanged(int arg1);

    void on_exp_gain_label_valueChanged(int arg1);

    void on_saveAviCheckBox__clicked();

    void on_yuv_stream_stateChanged(int arg1);

private:
    int PostProcessFrame(char* pFrame,int bufsize,float min_score,float scale,char* winhndl);



private:
    Ui::Widget *ui;
    add_blob *add_blob_file;
    del_blob *del_blob_file;

    QThread *thread;
    MyThread *MyT;

    int mvideo_type;
    int mh265en;
    int msensorModeId;
    char m_version[50];

    encodeMode x26mode;
    int h26x_type;

    int mexp_mode;



signals:
    void start_2net_model(int type,int modeId,int h26x_type);
    void close_2net_model();
};
#endif // WIDGET_H
