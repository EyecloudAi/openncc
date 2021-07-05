#include "del_blob.h"
#include "ui_del_blob.h"
#include <QSqlDatabase>
#include <QtSql>
#include <QMessageBox>

del_blob::del_blob(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::del_blob)
{
    ui->setupUi(this);
    QSqlQuery query;
    query.exec("SELECT * FROM ncc_model where parentId = 3");
    while (query.next()){
        ui->del_file->addItem(QString(query.value("model_name").toString()));
    }
}

del_blob::~del_blob()
{
    delete ui;
}



void del_blob::on_comfirm_btn_clicked()
{
    QString msg = tr("Do you want to delete %1?").arg(ui->del_file->currentText());
    int choose= QMessageBox::question(this, tr("comfirm"),
                                       QString(msg),
                                       QMessageBox::Yes | QMessageBox::No);
    if (choose== QMessageBox::No){
    }
    else if (choose== QMessageBox::Yes)
    {
        QSqlQuery query;
        QString del_file = tr("DELETE FROM ncc_model where model_name = '%1'").arg(ui->del_file->currentText());
        query.exec(del_file);
        ui->del_file->clear();
        query.exec("SELECT * FROM ncc_model where parentId = 3");
        while (query.next()){
            ui->del_file->addItem(QString(query.value("model_name").toString()));
        }
        emit updatemodel_1st_list_signal();
    }
}
