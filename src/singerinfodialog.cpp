﻿#include "singerinfodialog.h"
#include "mysqlquery.h"
#include "curlupload.h"
#include "releasetech.h"
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QStyleOption>
#include <QMessageBox>
#include <QTextCodec>
#include <QHBoxLayout>
#include <QVBoxLayout>
SingerInfoDialog::SingerInfoDialog(QWidget *parent)
    :DropShadowWidget(parent)
{
    setFixedHeight(402);
    setFixedWidth(1108);

    _sql = NULL;
    sid = -1;

    initWidget();
    initWidgetValue();
}

SingerInfoDialog::~SingerInfoDialog()
{

}

void SingerInfoDialog::initWidget()
{
    label = new QLabel(this);
    widgetTitle = new QWidget(this);
    widgetCenter = new QWidget(this);
    widgetUpdate = new QWidget(this);
    widgetSave = new QWidget(this);
    label_title = new QLabel(widgetTitle);
    pushButton_close = new QPushButton(widgetTitle);

    info = new SingerInfoWdiget(widgetCenter);
    info->isPreviewAndModify();

    pushButton_Yes = new QPushButton(widgetUpdate);
    pushButton_update = new QPushButton(widgetUpdate);
    pushButton_save = new QPushButton(widgetSave);

    connect(pushButton_close, &QPushButton::clicked, this, &SingerInfoDialog::close);
    connect(pushButton_Yes, &QPushButton::clicked, this, &SingerInfoDialog::close);
    connect(pushButton_update, &QPushButton::clicked, this, &SingerInfoDialog::updateActorValue);
    connect(pushButton_save, &QPushButton::clicked, this, &SingerInfoDialog::save);

    label_prompt = new QLabel(widgetSave);
    label_prompt_tooptip = new QLabel(widgetSave);

    hLayout = new QHBoxLayout();
    hLayout->addWidget(label_title);
    hLayout->addStretch();
    hLayout->addWidget(pushButton_close);
    hLayout->setContentsMargins(20, 0, 20, 0);
    hLayout->setSpacing(0);
    widgetTitle->setLayout(hLayout);

    updateLayout = new QHBoxLayout();
    updateLayout->addStretch();
    updateLayout->addWidget(pushButton_Yes);
    updateLayout->addWidget(pushButton_update);
    updateLayout->addStretch();
    updateLayout->setContentsMargins(0, 0, 0, 0);
    updateLayout->setSpacing(20);
    widgetUpdate->setLayout(updateLayout);

    promptLayout = new QHBoxLayout();
    saveLayout = new QHBoxLayout();
    promptLayout->addStretch();
    promptLayout->addWidget(label_prompt_tooptip);
    promptLayout->addWidget(label_prompt);
    promptLayout->setContentsMargins(0, 0, 10, 10);
    promptLayout->setSpacing(0);
    widgetSave->setLayout(promptLayout);


    infoLayout = new QVBoxLayout();
    infoLayout->addWidget(label);
    infoLayout->addWidget(info);
    infoLayout->setContentsMargins(20, 0, 20, 0);
    infoLayout->setSpacing(0);

    wbLayout = new QVBoxLayout();
    vLayout = new QVBoxLayout();

    wbLayout->addLayout(infoLayout);
    wbLayout->addStretch();
    wbLayout->addWidget(widgetUpdate);
    wbLayout->addWidget(widgetSave);
    wbLayout->setContentsMargins(0, 0, 0, 10);
    wbLayout->setSpacing(0);
    widgetCenter->setLayout(wbLayout);

    vLayout->addWidget(widgetTitle);
    vLayout->addWidget(widgetCenter);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);


    this->setLayout(vLayout);
}

void SingerInfoDialog::initWidgetValue()
{
    QFile qss(":/qss/title.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());
    qss.close();

    pushButton_save->setGeometry(this->width()/2 - 45, widgetSave->rect().y(), 90, 36);
    label->setFixedHeight(20);
    label->setObjectName("Label");
    label_prompt->setFixedHeight(36);
    pushButton_save->setFixedWidth(90);
    pushButton_save->setFixedHeight(36);
    pushButton_Yes->setFixedHeight(36);
    pushButton_Yes->setFixedWidth(90);
    pushButton_update->setFixedHeight(36);
    pushButton_update->setFixedWidth(90);
    pushButton_close->setFixedHeight(14);
    pushButton_close->setFixedWidth(14);
    widgetTitle->setFixedHeight(38);
    widgetTitle->setObjectName("TitleWidget");
    widgetCenter->setObjectName("WidgetCenter");
    widgetUpdate->setObjectName("WidgetCenter");
    widgetSave->setObjectName("WidgetCenter");
    pushButton_close->setObjectName("CloseButton");
    pushButton_Yes->setObjectName("Button");
    pushButton_update->setObjectName("Button");
    pushButton_save->setObjectName("Button");
    label_title->setObjectName("TitleLabel");
    label_prompt_tooptip->setObjectName("TooptipLabelImage");
    label_prompt->setObjectName("LineHidden");



    label_title->setText("歌星详情");
    label_prompt->setText("为必填项目");
    pushButton_Yes->setText("确认");
    pushButton_update->setText("修改");
    pushButton_save->setText("保存");

    widgetSave->setHidden(true);
    widgetUpdate->setHidden(false);
    selectStyleSheet(true);
    info->set_disabled();
}

void SingerInfoDialog::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void SingerInfoDialog::setTitleString(const QString &title)
{
    label_title->setText(title);
}

void SingerInfoDialog::setSqlPointer(MysqlQuery *sql)
{
    _sql = sql;
}

void SingerInfoDialog::setValue(const Actor _actor)
{  
    sid = _actor.sid.toLongLong();
    info->setActorValue(_actor);

#ifndef YQC_TECH
    if(_actor.serial_id.toLongLong() == 0)
        pushButton_update->setHidden(false);
    else
        pushButton_update->setHidden(true);
#else

#endif
}



void SingerInfoDialog::updateActorValue()
{
    selectStyleSheet(false);
    info->set_tooptip_image();
    info->set_enabled();
    info->pushButton_upload->setHidden(false);
    widgetSave->setHidden(false);
    widgetUpdate->setHidden(true);
    setTitleString("歌星修改");

    info->setSexValue();
    info->setNationValue();
    info->setStarsValue();
    info->setEnabledValue();
}

void SingerInfoDialog::save()
{
    if(info->isEmpty()){
        QMessageBox box(QMessageBox::Warning, "保存提示", "必填项未填写完整！");
        box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        box.setButtonText(QMessageBox::Ok, "确定");
        box.setButtonText(QMessageBox::Cancel, "取消");
        box.exec();
        return;
    }
    Actor actor;
    info->getActorValue(actor);

    if(sid != -1){
        QString path = info->getImagePath();
        CurlUpload *curlUpload = new CurlUpload();
        curlUpload->uploadActorImage(path, NULL);

        if(_sql->updateActor(sid, actor, info->actor))
        {
            this->close();
        }

        delete curlUpload;
    }
}

void SingerInfoDialog::selectStyleSheet(bool hidden)
{
    QString sytle = "QLabel{\
                        background: transparent;\
                        font-size: 14px;\
                     }\
                    QLabel#SingerInfoLabel{\
                    background: transparent;\
                    font-size: 14px;\
                    }\
                    \
                    QPushButton#Button{\
                    background-color: rgb(255, 255, 255);\
                    border: 1px solid rgb(170, 170, 170);\
                    color:rgb(88, 88, 88);\
                    font-size:14px;\
                    border-radius:5px;\
                    }\
                    QPushButton#Button:hover{\
                    background-color:rgb(255, 255, 255);\
                    border: 1px solid rgb(42, 42, 42);\
                    color:rgb(42, 42, 42);\
                    }\
                    QPushButton#Button:pressed{\
                    background-color: rgb(255, 146, 62);\
                    border: 1px solid rgb(255, 146, 62);\
                    color:rgb(255, 255, 255);\
                    }\
                    QPushButton#CalendarButton{\
                    border-image:url(:/other/images/calendar_normal.png);\
                    }\
                    QPushButton#CalendarButton:hover{\
                    border-image:url(:/other/images/calendar_hovering.png);\
                    }\
                    \
                    QPushButton#CalendarButton:pressed{\
                    border-image:url(:/other/images/calendar_pressed.png);\
                    }\
                    \
                    QPushButton#AnnotationButton{\
                    border-image:url(:/other/images/annotation_normal.png);\
                    }\
                    QPushButton#AnnotationButton:pressed{\
                    border-image:url(:/other/images/annotation_pressed.png);\
                    }\
                    \
                    QPushButton#PlusButton{\
                    border-image:url(:/other/images/plus_normal.png);\
                    }\
                    \
                    QPushButton#PlusButton:hover{\
                    border-image:url(:/other/images/plus_on.png);\
                    }\
                    \
                    QPushButton#PlusButton:pressed{\
                    border-image:url(:/other/images/plus_pressed.png);\
                    }\
                    \
                    QPushButton#PlusButton:disabled{\
                    border-image:url(:/other/images/plus_gray.png);\
                    }\
                    \
                    QLineEdit#LineEdit{\
                    border: 1px solid rgb(170, 170, 170);\
                    color:rgb(202, 202, 202);\
                    border-radius:5px;\
                    }\
                    QLineEdit#LineEdit:hover{\
                    border: 1px solid rgb(42, 42, 42);\
                    color:rgb(202, 202, 202);\
                    }\
                    QLineEdit#LineEdit:pressed{\
                    border: 1px solid rgb(255, 146, 62);\
                    color:rgb(88, 88, 88);\
                    }\
                    QLineEdit#LineEdit:disabled{\
                    color:rgb(202, 202, 202);\
                    border: 1px solid rgb(170, 170, 170);\
                    }\
                    \
                    \
                    QComboBox#Combobox{\
                    border:1px solid rgb(170, 170, 170);\
                    border-radius:5px;\
                    font-size:14px;\
                    color:rgb(88, 88, 88);\
                    }\
                    QComboBox#Combobox::down-arrow{\
                    image:url(:/arrow/images/combobox_down.png);\
                    }\
                    QComboBox#Combobox::down-arrow:on{\
                    image:url(:/arrow/images/combobox_up.png);\
                    }\
                    QComboBox#Combobox QAbstractItemView{\
                     border:1px solid rgb(255, 146, 62);\
                     selection-color:rgb(255, 255, 255);\
                     selection-background-color:rgb(255, 146, 62);\
                     color:rgb(00, 00, 00);\
                    }\
                    \
                    QComboBox#Combobox QAbstractItemView::item{\
                        height:30px;\
                    }\
                    \
                    QComboBox#Combobox::drop-down{\
                    width:20px;\
                    border:none;\
                    }\
                    QComboBox#Combobox:on{\
                    border:1px solid rgb(255, 146, 62);\
                    border-top-left-radius:5px;\
                    border-top-right-radius:5px;\
                    border-bottom-left-radius:0px;\
                    border-bottom-right-radius:0px;\
                    }\
                    \
                    QComboBox#Combobox:hover{\
                    border:1px solid rgb(42, 42, 42);\
                    color:rgb(42, 42, 42);\
                    }\
                    QComboBox#Combobox:disabled{\
                    border:1px solid rgb(201, 201, 201);\
                    color:rgb(225, 225, 225);\
                    }";


    QString hiddenStyle = "QLabel{\
                    background: transparent;\
                    color:rgb(183, 183, 183);\
                    font-size: 14px;\
                }\
                QLabel#SingerInfoLabel{\
                background: transparent;\
                color:rgb(183, 183, 183);\
                font-size: 14px;\
                }\
                QLineEdit{\
                    background: transparent;\
                    border: 0px;\
                    color:rgb(88, 88, 88);\
                    border-radius:5px;\
                }\
                \
                QComboBox{\
                    background: transparent;\
                    border:0px;\
                    border-radius:5px;\
                    font-size:14px;\
                    color:rgb(88, 88, 88);\
                }\
                \
                QComboBox::down-arrow{\
                    border-style:none;\
                }\
                QComboBox::down-arrow:on{\
                    border-style:none;\
                }\
                QComboBox QAbstractItemView{\
                     border:1px solid rgb(255, 146, 62);\
                     selection-color:rgb(255, 255, 255);\
                     selection-background-color:rgb(255, 146, 62);\
                     color:rgb(00, 00, 00);\
                }\
                \
                QComboBox QAbstractItemView::item{\
                        height:30px;\
                }\
                \
                QComboBox::drop-down{\
                    width:20px;\
                    border:none;\
                }\
                QComboBox:on{\
                    border:0px solid rgb(255, 146, 62);\
                    border-top-left-radius:5px;\
                    border-top-right-radius:5px;\
                    border-bottom-left-radius:0px;\
                    border-bottom-right-radius:0px;\
                }";

    if (hidden){
        info->setStyleSheet(hiddenStyle);
    }
    else{
        info->setStyleSheet(sytle);
    }
}



