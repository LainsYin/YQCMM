#include "yundm.h"
#include "tablemodel.h"
#include "curlupload.h"
#include "pagingtableview.h"
#include <QDir>
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QPainter>
#include <QStyleOption>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTableView>
#include <QDebug>
#include <QJsonObject>
#include <QCoreApplication>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#define UPANYUN "upaiyun.com"

YunDM::YunDM(QWidget *parent)
    :DropShadowWidget(parent)
{
    setFixedHeight(650);
    setFixedWidth(900);

    initWidget();
    readAndSetStyleSheet();
    initWidgetValue();

    connect(pushbutton_close, &QPushButton::clicked, this, &YunDM::close);
    connect(pushbutton_update_select, &QPushButton::clicked, this, &YunDM::updateInfo);
    this->setStyleSheet("border-bottom: 1px solid rgb(0, 0, 0)");

    down_list->setYunDownDelegate();
    down_list->widget_bottom->setHidden(true);
}

YunDM::~YunDM()
{

}

void YunDM::readAndSetStyleSheet()
{
    QFile qss(":/qss/title.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());
    qss.close();

    this->setObjectName("Widget");
    widget_top->setObjectName("TitleWidget");
    widget_list->setObjectName("WidgetCenter");

    label_title->setObjectName("TitleLabel");
    pushbutton_close->setObjectName("CloseButton");
    pushbutton_update_select->setObjectName("Button");
    widget_opt->setObjectName("UpdateWidget");
}

void YunDM::initWidget()
{
    widget_top = new QWidget(this);
    widget_opt = new QWidget(this);
    widget_list = new QWidget(this);

    label_title = new QLabel(widget_top);
    pushbutton_close = new QPushButton(widget_top);
    pushbutton_update_select = new QPushButton(widget_opt);
    down_list = new PagingTableView(widget_list);
    progress = new QProgressBar(widget_list);


    layout_title = new QHBoxLayout();
    layout_title->addWidget(label_title);
    layout_title->addStretch();
    layout_title->addWidget(pushbutton_close);
    layout_title->setContentsMargins(20, 0, 20, 0);
    layout_title->setSpacing(20);
    widget_top->setLayout(layout_title);

    layout_opt = new QHBoxLayout();
    layout_opt->addStretch();
    layout_opt->addWidget(pushbutton_update_select);
    layout_opt->setContentsMargins(20, 6, 20, 6);
    widget_opt->setLayout(layout_opt);

    layout_list = new QVBoxLayout();
    layout_list->addWidget(down_list);
    layout_list->addWidget(progress);
    layout_list->setContentsMargins(10, 0, 10, 10);
    layout_list->setSpacing(10);
    widget_list->setLayout(layout_list);

    vLayout = new QVBoxLayout();
    vLayout->addWidget(widget_top);
    vLayout->addWidget(widget_opt);
    vLayout->addWidget(widget_list);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);

    this->setLayout(vLayout);
    progress->setHidden(true);
}

void YunDM::initWidgetValue()
{
    requestFinished = false;
    label_title->setText("下载管理");
    widget_top->setFixedHeight(33);
    pushbutton_close->setFixedSize(14, 14);
    pushbutton_update_select->setFixedSize(90, 36);
    pushbutton_update_select->setText("更新");
}

void YunDM::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void YunDM::setSqlValue(QList<QStringList> rowVal, QMap<int, QString> sqlVal)
{
    rowList = rowVal;
    sqlList = sqlVal;

    qDebug() << " version update show record:" << rowList.size();
    down_list->setModelValue(rowList);
}

void YunDM::updateInfo()
{
    savePath = QCoreApplication::applicationDirPath();
    savePath = savePath.append("/update");

    progress->setHidden(false);
    progress->setRange(0, rowList.size());
    for(int i=0; i<rowList.size(); i++) {
        QStringList rowValue = rowList.at(i);
        QString mv = rowValue.at(rowValue.size() - 2);
        QString lyricImage = rowValue.at(rowValue.size() -1 );

        rowValue.replace(rowValue.size() - 3, "更新中……");
        rowList.replace(i, rowValue);
        down_list->setModelValue(rowList);


        if (!mv.isEmpty()){
            QString filePath = downloadFile("mp4", savePath, mv);
            if (uploadFile("mp4", filePath)){
                rowList.removeAt(i);
            } else{
                rowValue.replace(rowValue.size() - 3, "更新失败");
            }
        }

        if (!lyricImage.isEmpty()){
           QString filePath =  downloadFile("image", savePath, lyricImage);
           if (uploadFile("", filePath)){
               rowList.removeAt(i);
           } else {
               rowValue.replace(rowValue.size() - 3, "更新失败");
           }
        }
        down_list->setModelValue(rowList);
        progress->setValue(rowList.size());
    }
}

QString YunDM::downloadFile(const QString &type, const QString &dir, const QString &url)
{
    QStringList list = url.split(UPANYUN);
    QString path = dir + list.last();
    QString tempPath = path;
    tempPath = tempPath.remove(tempPath.split("/").last());
    QDir dirf(tempPath);
    if(dirf.exists()){

    }else{
        dirf.mkpath(tempPath);
    }

    CurlUpload *curlDownlaod = new CurlUpload();
    if(curlDownlaod->download_yun(type, url, path)){
        qDebug() << " download success : " << path;
    } else {
        qDebug() << " download failed : " << path;
    }
    delete curlDownlaod;

    return path;
}

bool YunDM::uploadFile(const QString &type, const QString &filePath)
{
    bool status = false;
    CurlUpload *curlUpload = new CurlUpload();
    QFile file(filePath);
    if(file.exists()){
        QStringList lists = filePath.split("/");
        QString fileName = lists.last();
        QString dir = lists.at(lists.size() - 2);
        QString retSize;
        QString ok;
        if (type.compare("mp4") == 0){
            ok = curlUpload->uploadYunVideo(fileName, dir, &retSize);
        } else {
            if(dir.compare("lyrics") == 0){
                ok = curlUpload->uploadMedialyric(filePath);
            } else if(dir.compare("fm") == 0){
                ok = curlUpload->uploadFmImage(filePath);
            } else if(dir.compare("avator") == 0){
                ok = curlUpload->uploadActorImage(filePath, &retSize);
            }
        }

        if(ok.compare("0") == 0){
            qDebug() << "上传类型不合法";
            status = false;
        } else if (ok.compare("1") == 0){
            qDebug() << "上传文件重复，且不可覆盖";
            status = false;
        }else{
            status = true;
        }

        qDebug() << " return size : " << retSize;
    }

    if (status){
        file.remove();
    }

    return status;
}

//void YunDM::startRequest(QString url)
//{
//    QNetworkRequest request;
//    request.setUrl(QUrl(url));
//    manager = new QNetworkAccessManager(this);
//    //发送GET请求
//    reply = manager->get(request);
//    //连接请求结束信号
//    disconnect(reply);
//    connect(reply, &QNetworkReply::finished,
//            this, &YunDM::replyFinished);
//    //连接响应时返回数据信号
//    connect(reply, &QNetworkReply::readyRead, this, &YunDM::readyRead);

//    //请求更新进度
//    connect(reply, &QNetworkReply::downloadProgress,
//            this,  &YunDM::updateDataReadProgress);
//}

//void YunDM::replyFinished()
//{
//    if (reply->error() == QNetworkReply::NoError){
//        requestFinished = true;
//    } else {
//        //获取响应的信息，状态码为200表示正常
//        QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
//        qDebug() << "request error! return code : " << status_code;
//    }
//}

//void YunDM::readyRead()
//{
//    if(requestFinished){

//    }else{

//    }
//}

//void YunDM::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
//{
////    if(reply->isFinished()){
////        progress->setHidden(true);
////    }
//    progress->setMaximum(totalBytes);
//    progress->setValue(bytesRead);
//    qDebug() << " total bytes :" << totalBytes << "read bytes : " << bytesRead;
//}



