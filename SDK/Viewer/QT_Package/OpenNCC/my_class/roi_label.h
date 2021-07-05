#ifndef ROI_LABEL_H
#define ROI_LABEL_H
#include <QObject>
#include <QLabel>
#include <QPainter>
#include <QPoint>
#include <QColor>
#include <QPen>
#include <QMouseEvent>

class roi_label : public QLabel
{
    Q_OBJECT
public:
    explicit roi_label(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *ev);
    //鼠标进入事件，更改鼠标形状
    void enterEvent(QEvent *);
    //鼠标离开事件，更改鼠标形状
    void leaveEvent(QEvent *);
    //鼠标移动事件
    void mouseMoveEvent(QMouseEvent *ev);
    //鼠标按下事件
    void mousePressEvent(QMouseEvent *ev);
    //鼠标抬起事件
    void mouseReleaseEvent(QMouseEvent *ev);
    //画框起点和终点
    int x1,y1,x2,y2;

public:
    int getx1(){return x1;}
    int gety1(){return y1;}
    int getx2(){return x2;}
    int gety2(){return y2;}
//private:
//    QPoint StartPoint;          //画线起点
//    QPoint EndPoint;            //画线终点

signals:
    void StartPointSignal();  //开始坐标的信号
    void StopPointSignal();  //结束坐标的信号

};

#endif // ROI_LABEL_H
