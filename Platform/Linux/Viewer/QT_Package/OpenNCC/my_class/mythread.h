#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>

class MyThread : public QObject
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = nullptr);

    void load_2net_model(int type,int modeId,int h26x);

    int Post_ProcessFrame(int type,char* pFrame,int bufsize,char* winhndl);

    void close_2nd_model();

    void h26x_265();


signals:
    void send_log_info_to_UI(QString msg);
    void close_1st_net_model();
};

#endif // MYTHREAD_H
