/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <my_class/browserlabel.h>
#include <my_class/roi_label.h>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QWidget *widget_12;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_15;
    browserlabel *web_label1;
    QSpacerItem *horizontalSpacer_4;
    browserlabel *web_label3;
    QSpacerItem *horizontalSpacer_6;
    QLabel *version_label;
    QWidget *widget_8;
    QGridLayout *gridLayout_4;
    QLabel *label_9;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QHBoxLayout *horizontalLayout_5;
    QTextBrowser *algo_area;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QHBoxLayout *horizontalLayout_4;
    QTextBrowser *log_area;
    QFrame *line_10;
    QLabel *label_10;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QPushButton *check_device;
    QFrame *line;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QCheckBox *yuv_stream;
    QCheckBox *h264_stream;
    QCheckBox *h265_stream;
    QCheckBox *mjpeg_stream;
    QFrame *line_3;
    QHBoxLayout *horizontalLayout_16;
    QLabel *label_13;
    QComboBox *outflow_mode;
    QFrame *line_4;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_2;
    QComboBox *model_1st;
    QFrame *line_5;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_3;
    QComboBox *model_2nd;
    QFrame *line_2;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_12;
    QSpinBox *min_score_spinbox;
    QSlider *min_score_value;
    QFrame *line_6;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_11;
    QSpinBox *scale_spinbox;
    QSlider *scale_value;
    QHBoxLayout *horizontalLayout_19;
    QLabel *label_4;
    QComboBox *exposure_mode;
    QLabel *label_exptime;
    QSlider *exposure_value;
    QSpinBox *exp_time_label;
    QLabel *label_iso;
    QSpinBox *exp_gain_label;
    QHBoxLayout *horizontalLayout_10;
    QPushButton *load_fw_btn;
    QPushButton *two_net_model;
    QVBoxLayout *verticalLayout_5;
    QWidget *widget_6;
    QGridLayout *gridLayout_2;
    QPushButton *Add_widget;
    QPushButton *Del_widget;
    QFrame *line_9;
    roi_label *roi_setting_area;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_3;
    QCheckBox *acc;
    QCheckBox *showstate;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *checkBox;
    QCheckBox *saveAviCheckBox_;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(1080, 683);
        Widget->setMinimumSize(QSize(781, 666));
        Widget->setMaximumSize(QSize(10000, 10000));
        widget_12 = new QWidget(Widget);
        widget_12->setObjectName(QStringLiteral("widget_12"));
        widget_12->setGeometry(QRect(20, 620, 1021, 38));
        horizontalLayout_9 = new QHBoxLayout(widget_12);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label_15 = new QLabel(widget_12);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setMinimumSize(QSize(0, 60));
        label_15->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        horizontalLayout_9->addWidget(label_15);

        web_label1 = new browserlabel(widget_12);
        web_label1->setObjectName(QStringLiteral("web_label1"));
        web_label1->setMinimumSize(QSize(0, 60));
        web_label1->setCursor(QCursor(Qt::PointingHandCursor));
        web_label1->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        horizontalLayout_9->addWidget(web_label1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_4);

        web_label3 = new browserlabel(widget_12);
        web_label3->setObjectName(QStringLiteral("web_label3"));
        web_label3->setMinimumSize(QSize(0, 60));
        web_label3->setCursor(QCursor(Qt::PointingHandCursor));
        web_label3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        horizontalLayout_9->addWidget(web_label3);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_6);

        version_label = new QLabel(widget_12);
        version_label->setObjectName(QStringLiteral("version_label"));
        version_label->setMinimumSize(QSize(0, 60));
        version_label->setAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);

        horizontalLayout_9->addWidget(version_label);

        horizontalLayout_9->setStretch(0, 1);
        horizontalLayout_9->setStretch(2, 1);
        horizontalLayout_9->setStretch(3, 1);
        horizontalLayout_9->setStretch(4, 2);
        horizontalLayout_9->setStretch(5, 1);
        widget_8 = new QWidget(Widget);
        widget_8->setObjectName(QStringLiteral("widget_8"));
        widget_8->setGeometry(QRect(11, 380, 1058, 231));
        gridLayout_4 = new QGridLayout(widget_8);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        label_9 = new QLabel(widget_8);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_4->addWidget(label_9, 1, 0, 1, 1);

        scrollArea_2 = new QScrollArea(widget_8);
        scrollArea_2->setObjectName(QStringLiteral("scrollArea_2"));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QStringLiteral("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 515, 186));
        horizontalLayout_5 = new QHBoxLayout(scrollAreaWidgetContents_2);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        algo_area = new QTextBrowser(scrollAreaWidgetContents_2);
        algo_area->setObjectName(QStringLiteral("algo_area"));

        horizontalLayout_5->addWidget(algo_area);

        scrollArea_2->setWidget(scrollAreaWidgetContents_2);

        gridLayout_4->addWidget(scrollArea_2, 2, 1, 1, 1);

        scrollArea = new QScrollArea(widget_8);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 515, 186));
        horizontalLayout_4 = new QHBoxLayout(scrollAreaWidgetContents);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        log_area = new QTextBrowser(scrollAreaWidgetContents);
        log_area->setObjectName(QStringLiteral("log_area"));

        horizontalLayout_4->addWidget(log_area);

        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout_4->addWidget(scrollArea, 2, 0, 1, 1);

        line_10 = new QFrame(widget_8);
        line_10->setObjectName(QStringLiteral("line_10"));
        line_10->setFrameShape(QFrame::HLine);
        line_10->setFrameShadow(QFrame::Sunken);

        gridLayout_4->addWidget(line_10, 0, 0, 1, 2);

        label_10 = new QLabel(widget_8);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout_4->addWidget(label_10, 1, 1, 1, 1);

        layoutWidget = new QWidget(Widget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 0, 1028, 376));
        horizontalLayout_6 = new QHBoxLayout(layoutWidget);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(5, 5, 5, 5);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        check_device = new QPushButton(layoutWidget);
        check_device->setObjectName(QStringLiteral("check_device"));
        check_device->setMinimumSize(QSize(420, 25));

        verticalLayout->addWidget(check_device);

        line = new QFrame(layoutWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(120, 25));

        horizontalLayout->addWidget(label);

        yuv_stream = new QCheckBox(layoutWidget);
        yuv_stream->setObjectName(QStringLiteral("yuv_stream"));
        yuv_stream->setEnabled(true);
        yuv_stream->setMinimumSize(QSize(100, 25));
        yuv_stream->setChecked(false);

        horizontalLayout->addWidget(yuv_stream);

        h264_stream = new QCheckBox(layoutWidget);
        h264_stream->setObjectName(QStringLiteral("h264_stream"));
        h264_stream->setEnabled(true);
        h264_stream->setMinimumSize(QSize(100, 25));

        horizontalLayout->addWidget(h264_stream);

        h265_stream = new QCheckBox(layoutWidget);
        h265_stream->setObjectName(QStringLiteral("h265_stream"));
        h265_stream->setEnabled(true);
        h265_stream->setMinimumSize(QSize(100, 25));

        horizontalLayout->addWidget(h265_stream);

        mjpeg_stream = new QCheckBox(layoutWidget);
        mjpeg_stream->setObjectName(QStringLiteral("mjpeg_stream"));
        mjpeg_stream->setEnabled(true);
        mjpeg_stream->setMinimumSize(QSize(100, 25));

        horizontalLayout->addWidget(mjpeg_stream);


        verticalLayout->addLayout(horizontalLayout);

        line_3 = new QFrame(layoutWidget);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_3);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));
        label_13 = new QLabel(layoutWidget);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setMinimumSize(QSize(120, 25));

        horizontalLayout_16->addWidget(label_13);

        outflow_mode = new QComboBox(layoutWidget);
        outflow_mode->setObjectName(QStringLiteral("outflow_mode"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(outflow_mode->sizePolicy().hasHeightForWidth());
        outflow_mode->setSizePolicy(sizePolicy);
        outflow_mode->setMinimumSize(QSize(430, 25));
        outflow_mode->setMaximumSize(QSize(167777, 16777215));

        horizontalLayout_16->addWidget(outflow_mode);


        verticalLayout->addLayout(horizontalLayout_16);

        line_4 = new QFrame(layoutWidget);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_4);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMinimumSize(QSize(120, 25));

        horizontalLayout_15->addWidget(label_2);

        model_1st = new QComboBox(layoutWidget);
        model_1st->setObjectName(QStringLiteral("model_1st"));
        sizePolicy.setHeightForWidth(model_1st->sizePolicy().hasHeightForWidth());
        model_1st->setSizePolicy(sizePolicy);
        model_1st->setMinimumSize(QSize(430, 25));
        model_1st->setMaximumSize(QSize(167777, 16777215));

        horizontalLayout_15->addWidget(model_1st);


        verticalLayout->addLayout(horizontalLayout_15);

        line_5 = new QFrame(layoutWidget);
        line_5->setObjectName(QStringLiteral("line_5"));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_5);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setEnabled(true);
        label_3->setMinimumSize(QSize(120, 25));

        horizontalLayout_14->addWidget(label_3);

        model_2nd = new QComboBox(layoutWidget);
        model_2nd->setObjectName(QStringLiteral("model_2nd"));
        model_2nd->setEnabled(true);
        sizePolicy.setHeightForWidth(model_2nd->sizePolicy().hasHeightForWidth());
        model_2nd->setSizePolicy(sizePolicy);
        model_2nd->setMinimumSize(QSize(430, 25));
        model_2nd->setMaximumSize(QSize(167777, 16777215));

        horizontalLayout_14->addWidget(model_2nd);


        verticalLayout->addLayout(horizontalLayout_14);

        line_2 = new QFrame(layoutWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_2);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        label_12 = new QLabel(layoutWidget);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setMinimumSize(QSize(120, 25));

        horizontalLayout_12->addWidget(label_12);

        min_score_spinbox = new QSpinBox(layoutWidget);
        min_score_spinbox->setObjectName(QStringLiteral("min_score_spinbox"));
        min_score_spinbox->setMinimumSize(QSize(100, 25));
        min_score_spinbox->setMinimum(30);
        min_score_spinbox->setMaximum(100);

        horizontalLayout_12->addWidget(min_score_spinbox);

        min_score_value = new QSlider(layoutWidget);
        min_score_value->setObjectName(QStringLiteral("min_score_value"));
        min_score_value->setMinimumSize(QSize(200, 0));
        min_score_value->setMinimum(30);
        min_score_value->setMaximum(100);
        min_score_value->setOrientation(Qt::Horizontal);

        horizontalLayout_12->addWidget(min_score_value);


        verticalLayout->addLayout(horizontalLayout_12);

        line_6 = new QFrame(layoutWidget);
        line_6->setObjectName(QStringLiteral("line_6"));
        line_6->setFrameShape(QFrame::HLine);
        line_6->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_6);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_11 = new QLabel(layoutWidget);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setMinimumSize(QSize(120, 25));

        horizontalLayout_2->addWidget(label_11);

        scale_spinbox = new QSpinBox(layoutWidget);
        scale_spinbox->setObjectName(QStringLiteral("scale_spinbox"));
        scale_spinbox->setMinimumSize(QSize(100, 25));
        scale_spinbox->setMinimum(30);
        scale_spinbox->setMaximum(100);

        horizontalLayout_2->addWidget(scale_spinbox);

        scale_value = new QSlider(layoutWidget);
        scale_value->setObjectName(QStringLiteral("scale_value"));
        scale_value->setMinimumSize(QSize(200, 0));
        scale_value->setMinimum(30);
        scale_value->setMaximum(100);
        scale_value->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(scale_value);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setObjectName(QStringLiteral("horizontalLayout_19"));
        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_19->addWidget(label_4);

        exposure_mode = new QComboBox(layoutWidget);
        exposure_mode->setObjectName(QStringLiteral("exposure_mode"));

        horizontalLayout_19->addWidget(exposure_mode);

        label_exptime = new QLabel(layoutWidget);
        label_exptime->setObjectName(QStringLiteral("label_exptime"));

        horizontalLayout_19->addWidget(label_exptime);

        exposure_value = new QSlider(layoutWidget);
        exposure_value->setObjectName(QStringLiteral("exposure_value"));
        exposure_value->setMinimum(1);
        exposure_value->setMaximum(330);
        exposure_value->setSingleStep(1);
        exposure_value->setOrientation(Qt::Horizontal);

        horizontalLayout_19->addWidget(exposure_value);

        exp_time_label = new QSpinBox(layoutWidget);
        exp_time_label->setObjectName(QStringLiteral("exp_time_label"));
        exp_time_label->setMaximum(16666);
        exp_time_label->setSingleStep(100);

        horizontalLayout_19->addWidget(exp_time_label);

        label_iso = new QLabel(layoutWidget);
        label_iso->setObjectName(QStringLiteral("label_iso"));

        horizontalLayout_19->addWidget(label_iso);

        exp_gain_label = new QSpinBox(layoutWidget);
        exp_gain_label->setObjectName(QStringLiteral("exp_gain_label"));
        exp_gain_label->setMinimum(100);
        exp_gain_label->setMaximum(1600);
        exp_gain_label->setSingleStep(100);
        exp_gain_label->setValue(800);

        horizontalLayout_19->addWidget(exp_gain_label);


        verticalLayout->addLayout(horizontalLayout_19);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        load_fw_btn = new QPushButton(layoutWidget);
        load_fw_btn->setObjectName(QStringLiteral("load_fw_btn"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(load_fw_btn->sizePolicy().hasHeightForWidth());
        load_fw_btn->setSizePolicy(sizePolicy1);
        load_fw_btn->setMinimumSize(QSize(200, 30));

        horizontalLayout_10->addWidget(load_fw_btn);

        two_net_model = new QPushButton(layoutWidget);
        two_net_model->setObjectName(QStringLiteral("two_net_model"));
        two_net_model->setMinimumSize(QSize(200, 30));

        horizontalLayout_10->addWidget(two_net_model);


        verticalLayout_2->addLayout(horizontalLayout_10);


        horizontalLayout_6->addLayout(verticalLayout_2);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        widget_6 = new QWidget(layoutWidget);
        widget_6->setObjectName(QStringLiteral("widget_6"));
        gridLayout_2 = new QGridLayout(widget_6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        Add_widget = new QPushButton(widget_6);
        Add_widget->setObjectName(QStringLiteral("Add_widget"));

        gridLayout_2->addWidget(Add_widget, 1, 0, 1, 1);

        Del_widget = new QPushButton(widget_6);
        Del_widget->setObjectName(QStringLiteral("Del_widget"));

        gridLayout_2->addWidget(Del_widget, 1, 1, 1, 1);

        line_9 = new QFrame(widget_6);
        line_9->setObjectName(QStringLiteral("line_9"));
        line_9->setFrameShape(QFrame::HLine);
        line_9->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line_9, 2, 0, 1, 2);

        roi_setting_area = new roi_label(widget_6);
        roi_setting_area->setObjectName(QStringLiteral("roi_setting_area"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Ignored);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(roi_setting_area->sizePolicy().hasHeightForWidth());
        roi_setting_area->setSizePolicy(sizePolicy2);
        roi_setting_area->setMinimumSize(QSize(384, 216));
        roi_setting_area->setMaximumSize(QSize(384, 216));
        roi_setting_area->setCursor(QCursor(Qt::ArrowCursor));
        roi_setting_area->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px solid rgb(71, 169, 255);"));
        roi_setting_area->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(roi_setting_area, 0, 0, 1, 2);


        verticalLayout_5->addWidget(widget_6);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        acc = new QCheckBox(layoutWidget);
        acc->setObjectName(QStringLiteral("acc"));

        verticalLayout_3->addWidget(acc);

        showstate = new QCheckBox(layoutWidget);
        showstate->setObjectName(QStringLiteral("showstate"));

        verticalLayout_3->addWidget(showstate);


        horizontalLayout_3->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        checkBox = new QCheckBox(layoutWidget);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setMinimumSize(QSize(200, 30));

        verticalLayout_4->addWidget(checkBox);

        saveAviCheckBox_ = new QCheckBox(layoutWidget);
        saveAviCheckBox_->setObjectName(QStringLiteral("saveAviCheckBox_"));

        verticalLayout_4->addWidget(saveAviCheckBox_);


        horizontalLayout_3->addLayout(verticalLayout_4);


        verticalLayout_5->addLayout(horizontalLayout_3);


        horizontalLayout_6->addLayout(verticalLayout_5);


        retranslateUi(Widget);
        QObject::connect(model_1st, SIGNAL(currentTextChanged(QString)), Widget, SLOT(model_1st_changed()));
        QObject::connect(yuv_stream, SIGNAL(clicked(bool)), Widget, SLOT(yuv_selected()));
        QObject::connect(h264_stream, SIGNAL(clicked(bool)), Widget, SLOT(h264_selected()));
        QObject::connect(mjpeg_stream, SIGNAL(clicked(bool)), Widget, SLOT(mjpeg_selected()));
        QObject::connect(scale_spinbox, SIGNAL(valueChanged(int)), scale_value, SLOT(setValue(int)));
        QObject::connect(model_2nd, SIGNAL(currentTextChanged(QString)), Widget, SLOT(model_2nd_changed()));
        QObject::connect(min_score_value, SIGNAL(valueChanged(int)), min_score_spinbox, SLOT(setValue(int)));
        QObject::connect(scale_value, SIGNAL(valueChanged(int)), scale_spinbox, SLOT(setValue(int)));
        QObject::connect(min_score_spinbox, SIGNAL(valueChanged(int)), min_score_value, SLOT(setValue(int)));
        QObject::connect(h265_stream, SIGNAL(clicked(bool)), Widget, SLOT(h265_selected()));

        exposure_mode->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", Q_NULLPTR));
        label_15->setText(QApplication::translate("Widget", "Support:", Q_NULLPTR));
        web_label1->setText(QApplication::translate("Widget", "<html><head/><body><p><span style=\" text-decoration: underline;\">OpenVINO Introduction</span></p></body></html>", Q_NULLPTR));
        web_label3->setText(QApplication::translate("Widget", "<html><head/><body><p><span style=\" text-decoration: underline;\">Get online supported</span></p></body></html>", Q_NULLPTR));
        version_label->setText(QApplication::translate("Widget", "Version:1.1.5", Q_NULLPTR));
        label_9->setText(QApplication::translate("Widget", "Log:", Q_NULLPTR));
        label_10->setText(QApplication::translate("Widget", "Algorithm output results:", Q_NULLPTR));
        check_device->setText(QApplication::translate("Widget", "Get device info", Q_NULLPTR));
        label->setText(QApplication::translate("Widget", "Stream Format:", Q_NULLPTR));
        yuv_stream->setText(QApplication::translate("Widget", "YUV420P", Q_NULLPTR));
        h264_stream->setText(QApplication::translate("Widget", "H.264", Q_NULLPTR));
        h265_stream->setText(QApplication::translate("Widget", "H.265", Q_NULLPTR));
        mjpeg_stream->setText(QApplication::translate("Widget", "MJPEG", Q_NULLPTR));
        label_13->setText(QApplication::translate("Widget", "Stream Resolution:  ", Q_NULLPTR));
        label_2->setText(QApplication::translate("Widget", "1st network model: ", Q_NULLPTR));
        label_3->setText(QApplication::translate("Widget", "2nd network model:", Q_NULLPTR));
        label_12->setText(QApplication::translate("Widget", "Model Score:     ", Q_NULLPTR));
        label_11->setText(QApplication::translate("Widget", "Display Scaler:  ", Q_NULLPTR));
        label_4->setText(QApplication::translate("Widget", "Exposure control:", Q_NULLPTR));
        label_exptime->setText(QApplication::translate("Widget", "time(us):", Q_NULLPTR));
        label_iso->setText(QApplication::translate("Widget", "iso", Q_NULLPTR));
        load_fw_btn->setText(QApplication::translate("Widget", "Start running model", Q_NULLPTR));
        two_net_model->setText(QApplication::translate("Widget", "Start running models", Q_NULLPTR));
        Add_widget->setText(QApplication::translate("Widget", "Add model", Q_NULLPTR));
        Del_widget->setText(QApplication::translate("Widget", "Del model", Q_NULLPTR));
        roi_setting_area->setText(QApplication::translate("Widget", " Region of interest for model", Q_NULLPTR));
        acc->setText(QApplication::translate("Widget", "inference accelerate", Q_NULLPTR));
        showstate->setText(QApplication::translate("Widget", "show state", Q_NULLPTR));
        checkBox->setText(QApplication::translate("Widget", "save_img", Q_NULLPTR));
        saveAviCheckBox_->setText(QApplication::translate("Widget", "save_avi", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
