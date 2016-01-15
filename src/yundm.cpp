#include "yundm.h"
#include "tablemodel.h"
#include "curlupload.h"
#include "pagingtableview.h"
#include "mysqlquery.h"
#include "yunthread.h"
#include <QDir>
#include <QFile>
#include <QTimer>
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
#include <QSettings>
#include <QJsonObject>
#include <QMessageBox>
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

    retSize = "";
    timer = NULL;
}

void YunDM::initSqlAndVersion(MysqlQuery *sql, const QString &verId, const QString &verName)
{
    _sql = sql;
    versionId = verId;
    versionName = verName;
}

void YunDM::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void YunDM::setSqlValue(QList<QStringList> rowVal)
{
    rowList = rowVal;

    writeLogging(QString(" version update show record : %1").arg(rowList.size()));
    down_list->setModelValue(rowList);
}

void YunDM::updateInfo()
{
    if (rowList.size() < 1)
        return;

    retSize = "-1";
    YunThread *workerThread = new YunThread(this);
    workerThread->initValue(rowList, _sql, &retSize);
    disconnect(workerThread);
    connect(workerThread, &YunThread::updateView, this, &YunDM::updateDialogView);
    connect(workerThread, &YunThread::downloadUpload, this, &YunDM::timeStart);
    connect(workerThread, &YunThread::updateStore, this, &YunDM::updateStoreStatus);
    connect(workerThread, &YunThread::finished, workerThread, &QObject::deleteLater);
    workerThread->start();

    progress->setHidden(false);
    timeStart();
}


void YunDM::timeStart()
{
    if(timer != NULL)
    {
        if(timer->isActive())
        {
            timer->stop();
            timer = NULL;
        }
    }

    timer = new QTimer(this);
    disconnect(timer, &QTimer::timeout, 0, 0);
    connect(timer, &QTimer::timeout, this, &YunDM::timeOver);
    timer->start(1000);

    retSize = "-1";
}

void YunDM::timeOver()
{
    QStringList list = retSize.split(",");
    if (list.last().toDouble() < 0){
        return;
    } else {
        progress->setMaximum((int)list.last().toDouble());
        progress->setValue((int)list.first().toDouble());
    }

    qDebug() << " ******* upload size : " << retSize;
}

void YunDM::updateStoreStatus()
{
    QSettings *initConfig = new QSettings("SongsMaintain.conf", QSettings::IniFormat);
    initConfig->setIniCodec("UTF-8");
    QString storeid = initConfig->value("UPDATE/storeid").toString();
    QString host = initConfig->value("UPDATE/hostport").toString();
    host.append("/ml/rs/ktvVersions");

    QString postStr = QString("{\"versionId\":%1, \"versionName\":\"%2\", \"ktvId\":\"%3\"}")
                             .arg(versionId)
                             .arg(versionName)
                             .arg(storeid);
    CurlUpload *curlDownlaod = new CurlUpload();
    QString retStr = curlDownlaod->uploadStoreUpdateStatus(host, postStr);
    writeLogging(QString(" upate store status return result : %1").arg(retStr));

    progress->setHidden(true);
}

void YunDM::updateDialogView(QStringList row)
{
    rowList.removeOne(row);
    down_list->setModelValue(rowList);
    if (rowList.size() < 1){
        QMessageBox::question(this, tr("提示"),("更新完成..."), QMessageBox::Yes);
        this->close();
    }
}

void YunDM::updateDownLoadStatus(QString status)
{
    QStringList result = status.split(" ");
    qDebug() << " download " << result;
    if (result.size() > 1 && result.last().compare("end") == 0){
        setTitleText("下载管理");
    }

    if (result.first().toLower().compare("downloading") == 0){
        setTitleText(QString("DownLoading %1 ...").arg(result.last()));
    }

    if (result.first().toLower().compare("uploading") == 0){
        setTitleText(QString("DownLoading %1 ...").arg(result.last()));
    }
}


void YunDM::updateInfo1()
{
    savePath = QCoreApplication::applicationDirPath();
    savePath = savePath.append("/update");
    QList<QStringList> rowListTmp = rowList;
    for(int i=0; i<rowListTmp.size(); i++) {
        QStringList rowValue = rowListTmp.at(i);
        QString mv = rowValue.at(rowValue.size() - 3);
        QString lyricImage = rowValue.at(rowValue.size() - 2);

        if (!mv.isEmpty()){
            QString filePath = downloadFile("mp4", savePath, mv);
            if (!(uploadFile("mp4", filePath))){
            }
        }

        if (!lyricImage.isEmpty()){
            QString filePath =  downloadFile("image", savePath, lyricImage);
            if (!(uploadFile("", filePath))){
            }
        }

        if(!execSql(rowValue)){
            rowList.removeOne(rowListTmp.at(i));
            down_list->setModelValue(rowList);
        }
    }

    //门店更新状态接口
    updateStoreStatus();
}

QString YunDM::downloadFile(const QString &type, const QString &dir, const QString &url)
{
    setTitleText("Downloading res……");
    progress->setHidden(true);
    QStringList list = url.split(UPANYUN);
    QString path = dir + list.last();
    QString tempPath = path;
    tempPath = tempPath.remove(tempPath.split("/").last());
    QDir dirf(tempPath);
    if(!dirf.exists()){
        dirf.mkpath(tempPath);
    }

    CurlUpload *curlDownlaod = new CurlUpload();
    if(curlDownlaod->download_yun(type, url, path)){
        writeLogging(QString(" download success : %1").arg(path));
    } else {
        writeLogging(QString(" download failed : %1").arg(path));
    }
    delete curlDownlaod;


    setTitleText();
    return path;
}

bool YunDM::uploadFile(const QString &type, const QString &filePath)
{
    setTitleText("Upload res...");
    progress->setHidden(false);
    bool status = false;
    CurlUpload *curlUpload = new CurlUpload();
    QFile file(filePath);
    if(file.exists()){
        QStringList lists = filePath.split("/");
        QString fileName = lists.last();
        QString dir = lists.at(lists.size() - 2);
        QString ok;
        timeStart();
        progress->setValue(0);
        if (type.compare("mp4") == 0){
            ok = curlUpload->uploadYunVideo(filePath, dir, &retSize);
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
            writeLogging(QString(" upload type error : %1").arg(fileName));
            status = false;
        } else if (ok.compare("1") == 0){
            writeLogging(QString("There already exists a file called %1 in %2 and not Overwrite")
                         .arg(fileName).arg(dir));
            status = true;
        }else if (ok.isEmpty()){
            writeLogging(QString("upload return empty."));
            status = false;
        } else if (ok.compare("-2") == 0){
            writeLogging(QString("upload type error."));
            status = false;
        }else {
            status = true;
            writeLogging(QString("upload return : %1").arg(ok));
        }

        timer->stop();
    }

    if (status){
//        file.remove();
    }

    setTitleText();
    return status;
}

bool YunDM::execSql(const QStringList &info)
{
    bool ret = false;
    if (_sql->executeSql(info.last())){
        rowList.removeOne(info);
        ret = true;
    } else {
        writeLogging(QString(" sql error  : %1").arg(info.last()));
    }

    return ret;
}

void YunDM::writeLogging(const QString &str)
{
    qDebug() << "version id :" << versionId
             << "version name :" << versionName
             << str;
}

void YunDM::setTitleText(const QString &text)
{
    label_title->setText(text);
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



