#ifndef BROWSERLABEL_H
#define BROWSERLABEL_H

#include <QObject>
#include <QLabel>
#include <QMouseEvent>

class browserlabel : public QLabel
{
    Q_OBJECT
public:
    explicit browserlabel(QWidget *parent = nullptr);


public:
    void mousePressEvent(QMouseEvent *ev);

signals:
    void OpenBrowser();

};

#endif // BROWSERLABEL_H
