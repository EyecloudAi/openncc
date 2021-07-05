#include "roi_label.h"

roi_label::roi_label(QWidget *parent) : QLabel(parent)
{
    x1 = 0;
    x2 = 0;
    y1 = 0;
    y2 = 0;
}

void roi_label::paintEvent(QPaintEvent *ev){
    QLabel::paintEvent(ev);
    QPainter painter(this);
    painter.setPen(QPen(Qt::red,1));
    painter.drawRect(x1,y1,x2-x1,y2-y1);
}

void roi_label::enterEvent(QEvent *)
{
    this->setCursor(Qt::CrossCursor);
}

void roi_label::leaveEvent(QEvent *)
{
    this->setCursor(Qt::ArrowCursor);
}

void roi_label::mouseMoveEvent(QMouseEvent *ev){
    if (ev->buttons() & Qt::LeftButton){
        int temp_x = ev->x();
        int temp_y = ev->y();
        if (temp_x<0){temp_x= 0;}
        else if (temp_x>this->width()) {
            temp_x = this->width();
        }
        if (temp_y<0) {temp_y = 0;}
        else if (temp_y>this->height()) {
            temp_y = this->height();
        }

        x2 = temp_x;
        y2 = temp_y;
        emit StopPointSignal();
        update();
    }

}

void roi_label::mousePressEvent(QMouseEvent *ev){
    x1 = ev->x();
    y1 = ev->y();
    emit StartPointSignal();
}

void roi_label::mouseReleaseEvent(QMouseEvent *ev){
    int temp_x = ev->x();
    int temp_y = ev->y();
    if (temp_x<0){temp_x= 0;}
    else if (temp_x>this->width()) {
        temp_x = this->width();
    }
    if (temp_y<0) {temp_y = 0;}
    else if (temp_y>this->height()) {
        temp_y = this->height();
    }

    x2 = temp_x;
    y2 = temp_y;
    update();
}
