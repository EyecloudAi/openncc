#include "browserlabel.h"

browserlabel::browserlabel(QWidget *parent) : QLabel(parent)
{

}

void browserlabel::mousePressEvent(QMouseEvent *ev){

    if (ev->button()== Qt::LeftButton)
    {
    emit OpenBrowser();
    }
}
