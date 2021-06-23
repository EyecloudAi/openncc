/********************************************************************************
** Form generated from reading UI file 'add_blob.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADD_BLOB_H
#define UI_ADD_BLOB_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_add_blob
{
public:
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *blob_btn;
    QLineEdit *blob_path;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *confirm_btn;

    void setupUi(QWidget *add_blob)
    {
        if (add_blob->objectName().isEmpty())
            add_blob->setObjectName(QStringLiteral("add_blob"));
        add_blob->resize(397, 174);
        widget = new QWidget(add_blob);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(9, 9, 350, 114));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(20);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(300, 0));

        horizontalLayout->addWidget(label);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        blob_btn = new QPushButton(widget);
        blob_btn->setObjectName(QStringLiteral("blob_btn"));
        blob_btn->setMinimumSize(QSize(100, 0));

        horizontalLayout_2->addWidget(blob_btn);

        blob_path = new QLineEdit(widget);
        blob_path->setObjectName(QStringLiteral("blob_path"));
        blob_path->setMinimumSize(QSize(200, 0));

        horizontalLayout_2->addWidget(blob_path);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        confirm_btn = new QPushButton(widget);
        confirm_btn->setObjectName(QStringLiteral("confirm_btn"));
        confirm_btn->setMinimumSize(QSize(200, 0));

        horizontalLayout_3->addWidget(confirm_btn);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(add_blob);

        QMetaObject::connectSlotsByName(add_blob);
    } // setupUi

    void retranslateUi(QWidget *add_blob)
    {
        add_blob->setWindowTitle(QApplication::translate("add_blob", "Form", Q_NULLPTR));
        label->setText(QApplication::translate("add_blob", "Please select a blob modle file:", Q_NULLPTR));
        blob_btn->setText(QApplication::translate("add_blob", "Open...", Q_NULLPTR));
        confirm_btn->setText(QApplication::translate("add_blob", "Confirm", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class add_blob: public Ui_add_blob {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADD_BLOB_H
