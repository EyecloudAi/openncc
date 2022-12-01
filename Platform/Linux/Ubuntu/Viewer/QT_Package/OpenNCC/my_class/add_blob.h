#ifndef ADD_BLOB_H
#define ADD_BLOB_H

#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>
#include <QtSql>

namespace Ui {
class add_blob;
}

class add_blob : public QWidget
{
    Q_OBJECT

public:
    explicit add_blob(QWidget *parent = nullptr);
    ~add_blob();

public:
    void copy_file(QString path1,QString path2);
    void clear_blob_path_record();

signals:
    void update_model_1st_list_signal();

private slots:
    void on_confirm_btn_clicked();
    void on_blob_btn_clicked();

private:
    Ui::add_blob *ui;
};

#endif // ADD_BLOB_H
