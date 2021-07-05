#include "add_blob.h"
#include "ui_add_blob.h"
#include "widget.h"

static QString file_path;

add_blob::add_blob(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_blob)
{
    ui->setupUi(this);
    ui->blob_path->clear();


}

add_blob::~add_blob()
{
    delete ui;
}

void add_blob::clear_blob_path_record(){
    ui->blob_path->clear();
}

void add_blob::copy_file(QString path1,QString path2){
    if (path1==path2){
        return;
    }
    else if (QFile::exists(path2)){
        QFile::remove(path2);
        QFile::copy(path1,path2);
    }
    else {
        QFile::copy(path1,path2);
    }

}

void add_blob::on_confirm_btn_clicked()
{
    if (file_path.isEmpty()){
        QMessageBox::warning(this,"Failed","Please select your blob file.");
    }
    else{
        QFileInfo file_info = QFileInfo(file_path);
        QString file_name = file_info.baseName();
//        qDebug()<<file_name;
        QString files_path = file_info.absolutePath();
//        qDebug()<<files_path;

        QString bin_path = files_path + "/"+file_name +".bin";
        QString xml_path = files_path + "/"+file_name +".xml";
        QString blob_path = files_path + "/"+file_name +".blob";
//        qDebug()<<bin_path
//               <<xml_path
//              <<blob_path;
        if (QFile::exists(bin_path)&&QFile::exists(xml_path)&&QFile::exists(blob_path)){

            QSqlQuery query;
            QString is_blob_exist = tr("SELECT COUNT(*) from ncc_model where model_name = '%1'").arg(file_name);
            query.exec(is_blob_exist);
            query.first();
            if (query.value("count(*)").toInt() == 0){
                QString sqlstr = tr("INSERT INTO ncc_model (model_name,model_info,parentId) VALUES ('%1','Custom model',3)").arg(file_name);
                query.exec(sqlstr);

                QString bin_to_dir = tr("%1/Configuration/blob/%2.bin").arg(QDir::currentPath()).arg(file_name);
                QString xml_to_dir = tr("%1/Configuration/blob/%2.xml").arg(QDir::currentPath()).arg(file_name);
                QString blob_to_dir = tr("%1/Configuration/blob/%2.blob").arg(QDir::currentPath()).arg(file_name);
//        6024019130        qDebug()<<bin_to_dir
//                       <<xml_to_dir
//                      <<blob_to_dir;
                copy_file(bin_path,bin_to_dir);
                copy_file(xml_path,xml_to_dir);
                copy_file(blob_path,blob_to_dir);
                emit update_model_1st_list_signal();
                QMessageBox::information(this,"Success","Enjoy it!");
                this->hide();

            }
            else {
                QMessageBox::warning(this,"Failed","The file already exist! Please rename your file and try again.");
            }
        }
        else{
            QMessageBox::warning(this,"Failed","please confirm your bin/xml/blob file under the same floder with the same name!");
        }
    }
}

void add_blob::on_blob_btn_clicked()
{
    file_path = QFileDialog::getOpenFileName(this,"Please select your blob file","./Configuration/blob","BIN Files(*.blob)");
    if (!file_path.isEmpty()){

        ui->blob_path->setText(file_path);
    }
}
