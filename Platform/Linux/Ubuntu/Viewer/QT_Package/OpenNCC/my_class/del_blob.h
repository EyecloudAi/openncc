#ifndef DEL_BLOB_H
#define DEL_BLOB_H

#include <QWidget>

namespace Ui {
class del_blob;
}

class del_blob : public QWidget
{
    Q_OBJECT

public:
    explicit del_blob(QWidget *parent = nullptr);
    ~del_blob();

signals:
    void updatemodel_1st_list_signal();

private slots:
    void on_comfirm_btn_clicked();

private:
    Ui::del_blob *ui;
};

#endif // DEL_BLOB_H
