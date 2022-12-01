#include "utils.h"
#include <QDesktopServices>
#include <QTextBrowser>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QStringList>
#include <QCoreApplication>
#include <QDir>
#include "sdk.h"
#include <QApplication>

int loadFw(const QString& bootPath,const QString& fwPath){

    QString exePath = QCoreApplication::applicationDirPath();

    QString bootExePath = exePath + bootPath;
    QString mvcmdPath = exePath + fwPath;

#ifdef Q_OS_WIN
    // 默认使用linux风格，当系统为win时，转成win风格的路径
    bootExePath += ".exe";
    bootExePath = QDir::toNativeSeparators(bootExePath);
    mvcmdPath = QDir::toNativeSeparators(mvcmdPath);
#endif
    return load_fw(bootExePath.toStdString().c_str(),mvcmdPath.toStdString().c_str());
}


void setStyleByCss(){
    QFile styleSheet;
       styleSheet.setFileName(":/res/style.css");
       if(styleSheet.open(QFile::ReadOnly)) {
           QString styleString = styleSheet.readAll();
           styleSheet.close();
           static_cast<QApplication*>(QApplication::instance())->setStyleSheet(styleString);
       }
}
int manualset_exp=0;
int streamrun=0;
int mexp_gain=0;
int mexp_time=0;
