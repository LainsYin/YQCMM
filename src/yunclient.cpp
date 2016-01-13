#include "yunclient.h"

#include "pagingtableview.h"
#include "yundm.h"
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QPainter>
#include <QStyleOption>
#include <QCalendarWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateTime>
#include <QFileInfo>
#include <QSettings>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QMessageBox>
#include <QDebug>

#define RECORD_PAGE 10

YunClient::YunClient(QWidget *parent) : QWidget(parent)
{
    initWidget();
    initWidgetValue();
    readAndSetStyleSheet();

    connect(pushbutton_start, &QPushButton::clicked, this, &YunClient::setCalendarStatue_s);
    connect(pushbutton_end, &QPushButton::clicked, this, &YunClient::setCalendarStatue_e);
    connect(calendar_start, &QCalendarWidget::selectionChanged, this, &YunClient::updateCaleSta_s);
    connect(calendar_end, &QCalendarWidget::selectionChanged, this, &YunClient::updateCaleSta_e);

    connect(pushbutton_query, &QPushButton::clicked, this, &YunClient::getUpdateVersionList);
    connect(update_list, &PagingTableView::currentRow, this, &YunClient::requestVersionInfo);

    QSettings *initConfig = new QSettings("SongsMaintain.conf", QSettings::IniFormat);
    initConfig->setIniCodec("UTF-8");
    host = initConfig->value("UPDATE/hostport").toString();
}

YunClient::~YunClient()
{

}

void YunClient::readAndSetStyleSheet()
{
    QFile qss(":/qss/songinfo.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());
    qss.close();

    this->setObjectName("Widget");
    widget_top->setObjectName("TopWidget");
    widget_center->setObjectName("CenterWidget");
    widget_bottom->setObjectName("TopWidget");

    label_cen->setObjectName("Label");
    label_progress->setObjectName("Label");
    label_speed->setObjectName("Label");
    label_recordPage->setObjectName("Label");
    label_totalRecords->setObjectName("Label");
    label_page->setObjectName("Label");
    lineedit_end->setObjectName("LineEdit");
    lineedit_start->setObjectName("LineEdit");
    lineedit_recordPage->setObjectName("LineEdit");

    pushbutton_query->setObjectName("Button");
    pushbutton_next->setObjectName("Button");
    pushbutton_up->setObjectName("Button");
    pushbutton_end->setObjectName("CalendarButton");
    pushbutton_start->setObjectName("CalendarButton");
}

void YunClient::initWidget()
{
    widget_top = new QWidget(this);
    widget_center = new QWidget(this);
    widget_bottom = new QWidget(this);

    lineedit_start = new QLineEdit(widget_top);
    lineedit_end = new QLineEdit(widget_top);
    lineedit_recordPage = new QLineEdit(widget_bottom);

    pushbutton_query = new QPushButton(widget_top);
    pushbutton_start = new QPushButton(lineedit_start);
    pushbutton_end = new QPushButton(lineedit_end);
    pushbutton_up = new QPushButton(widget_bottom);
    pushbutton_next = new QPushButton(widget_bottom);

    label_cen = new QLabel(widget_top);
    label_progress = new QLabel(widget_bottom);
    label_speed = new QLabel(widget_bottom);
    label_totalRecords = new QLabel(widget_bottom);
    label_recordPage = new QLabel(widget_bottom);
    label_page = new QLabel(widget_bottom);
    update_list = new PagingTableView(widget_center);

    calendar_start = new QCalendarWidget(this);
    calendar_start->resize(280, 180);
    calendar_end = new QCalendarWidget(this);
    calendar_end->resize(280, 180);

    layout_select = new QHBoxLayout();
    layout_top = new QHBoxLayout();

    layout_select->addWidget(lineedit_start);
    layout_select->addWidget(label_cen);
    layout_select->addWidget(lineedit_end);
    layout_select->setContentsMargins(20, 0, 20, 0);
    layout_select->setSpacing(20);

    layout_top->addLayout(layout_select);
    layout_top->addWidget(pushbutton_query);
    layout_top->addStretch();
    widget_top->setLayout(layout_top);

    layout_list = new QHBoxLayout();
    layout_list->addWidget(update_list);
    layout_list->setContentsMargins(0, 0, 0, 0);
    widget_center->setLayout(layout_list);

    layout_paging = new QHBoxLayout();
    layout_paging->addWidget(label_totalRecords);
    layout_paging->addWidget(label_recordPage);
    layout_paging->addWidget(lineedit_recordPage);
    layout_paging->addWidget(pushbutton_next);
    layout_paging->addWidget(label_page);
    layout_paging->addWidget(pushbutton_up);
    layout_paging->addStretch();
    layout_paging->setContentsMargins(20, 6, 10, 6);
    layout_paging->setSpacing(15);

    layout_pro = new QHBoxLayout();
    layout_pro->addLayout(layout_paging);
    layout_pro->addStretch();
    layout_pro->addWidget(label_progress);
    layout_pro->addWidget(label_speed);
    layout_pro->setContentsMargins(0, 6, 20, 6);
    layout_pro->setSpacing(20);
    widget_bottom->setLayout(layout_pro);

    vLayout = new QVBoxLayout();
    vLayout->addWidget(widget_top);
    vLayout->addWidget(widget_center);
    vLayout->addWidget(widget_bottom);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);

    this->setLayout(vLayout);

    lineedit_recordPage->setFixedSize(50, 36);
    lineedit_start->setFixedSize(260, 36);
    lineedit_end->setFixedSize(260, 36);
    pushbutton_query->setFixedSize(90, 36);
    pushbutton_end->setFixedSize(20, 20);
    pushbutton_start->setFixedSize(20, 20);
    pushbutton_next->setFixedSize(90, 36);
    pushbutton_up->setFixedSize(90, 36);

    label_cen->setMinimumHeight(36);
    label_progress->setMinimumHeight(36);
    label_speed->setMinimumHeight(36);

    QRect rect = lineedit_start->geometry();
    lineedit_start->setTextMargins(0, 0, 20, 0);
    pushbutton_start->setGeometry(rect.width()-25, 10, 20, 20);
    pushbutton_start->setCursor(Qt::PointingHandCursor);

    pushbutton_start->setCheckable(true);
    calendar_start->setGeometry(this->x() + 20, this->y()+55, 280, 180);
    calendar_start->setHidden(true);

    QRect rect1 = lineedit_end->geometry();
    lineedit_end->setTextMargins(0, 0, 20, 0);
    pushbutton_end->setGeometry(rect1.width() - 25, 10, 20, 20);
    pushbutton_end->setCursor(Qt::PointingHandCursor);
    pushbutton_end->setCheckable(true);
    calendar_end->setGeometry(this->x() + 340, this->y()+55, 280, 180);
    calendar_end->setHidden(true);

    update_list->widget_bottom->setHidden(true);
    update_list->setYunDelegate();
    update_list->setYunUpdateColumnWidget();
}

void YunClient::initWidgetValue()
{
    label_cen->setText("至");
//    label_progress->setText("进度:");
//    label_speed->setText("速度:");
    label_recordPage->setText("每页显示");
    label_totalRecords->setText("共：");
    lineedit_recordPage->setText(QString::number(RECORD_PAGE));
    pushbutton_query->setText("查询");
    pushbutton_next->setText("下一页");
    pushbutton_up->setText("下一页");
}

void YunClient::paintEvent(QPaintEvent *painter)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    painter->ignore();
}

void YunClient::setCalendarStatue_s(bool )
{
    if (calendar_start->isHidden())
        calendar_start->setHidden(false);
    else
        calendar_start->setHidden(true);
}

void YunClient::setCalendarStatue_e(bool )
{
    if (calendar_end->isHidden())
        calendar_end->setHidden(false);
    else
        calendar_end->setHidden(true);
}

void YunClient::updateCaleSta_s()
{
    QString date = calendar_start->selectedDate().toString("yyyy-MM-dd");
    lineedit_start->setText(date);
}

void YunClient::updateCaleSta_e()
{
    QString date = calendar_end->selectedDate().toString("yyyy-MM-dd");
    lineedit_end->setText(date);
}

void YunClient::getUpdateVersionList()
{
    if (QDateTime::fromString(lineedit_start->text(), "yyyy-MM-dd")
            > QDateTime::fromString(lineedit_end->text(), "yyyy-MM-dd")){
        QMessageBox::warning(this, "提示", "开始时间不能大于结束时间");
        return;
    }

    request_type = get_version_list;
    QByteArray url = getUrl("/ml/rs/mediaReleaseVersions/");
    startRequest(url);
}

QByteArray YunClient::getUrl(QByteArray path)
{
    QString page = label_page->text().split(":").last();
    QStringList list_page = page.split("/");
    int cur_page = list_page.first().toInt();
    int record_page = lineedit_recordPage->text().toInt();
    cur_page = cur_page == 0 ? 1 : cur_page;
    record_page = record_page == 0 ? RECORD_PAGE : record_page;
    QByteArray url = host.toLatin1() + path;
    url.append(QString::number(cur_page)).append("/");
    url.append(QString::number(record_page)).append("/");
    url.append(lineedit_start->text().isEmpty() ? "null" : lineedit_start->text()).append("/");
    url.append(lineedit_end->text().isEmpty() ? "null" : lineedit_end->text());

    return url;
}

void YunClient::startRequest(QByteArray url)
{
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    manager = new QNetworkAccessManager(this);
    //发送GET请求
    reply = manager->get(request);
    //连接请求结束信号
    disconnect(reply);
    connect(reply, &QNetworkReply::finished,
            this, &YunClient::replyFinished);
    //连接响应时返回数据信号
//    connect(reply, &QNetworkReply::readyRead, this, &YunClient::readyRead);

    //请求更新进度
//    connect(reply, &QNetworkReply::downloadProgress,
//            this,  &YunClient::updateDataReadProgress);
}

void YunClient::replyFinished()
{
    if (reply->error() == QNetworkReply::NoError){
        QByteArray str = reply->readAll();
        qDebug() << " request return ::: " << str;

        QJsonObject dataJson = getRequestData(QString(str));
        switch (request_type) {
        case get_version_list:{
            QJsonArray listJson = setPagingInfo(dataJson);
            setVersionList(listJson);
            break;
        }
        case get_media_list:{
            setUpdateList(dataJson);
            break;
        }
        default:
            break;
        }
    } else {
        //获取响应的信息，状态码为200表示正常
        QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        qDebug() << "request error! return code : " << status_code;
    }


    reply->deleteLater();
    reply = 0;
}

void YunClient::readyRead()
{
    if (reply->error() == QNetworkReply::NoError){
        QByteArray str = reply->readAll();
        qDebug() << " request return ::: " << str;

        QJsonObject dataJson = getRequestData(QString(str));
        switch (request_type) {
        case get_version_list:{
            QJsonArray listJson = setPagingInfo(dataJson);
            setVersionList(listJson);
            break;
        }
        case get_media_list:{
            setUpdateList(dataJson);
            break;
        }
        default:
            break;
        }
    } else {
        //获取响应的信息，状态码为200表示正常
        QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        qDebug() << "request error! return code : " << status_code;
    }
}

void YunClient::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
    qDebug() << " total bytes :" << totalBytes << "read bytes : " << bytesRead;
}

void YunClient::requestVersionInfo(const int &row)
{
    QStringList rowValue = rowList.at(row);
    QString verId = rowValue.at(1);
    qDebug() << "click row : " << row << "version id :" << verId;
    if(!verId.toInt()){
        qDebug() << "version id error!";
        return;
    }

    request_type = get_media_list;
    QByteArray url =  host.toLatin1() + "/ml/rs/mediaReleaseVersions/" + verId.toLatin1();
    startRequest(url);

    YunDM *dialog = new YunDM();
    connect(this, &YunClient::sqlValue, dialog, &YunDM::setSqlValue);
    dialog->exec();

    getUpdateVersionList();
}

void YunClient::errView(QJsonObject json)
{
    QString errcode, errmsg;
    if(json.contains("errorCode")){
        errcode = json.take("errorCode").toString();
    }
    if (json.contains("errorMsg")){
        errmsg = json.take("errorMsg").toString();
    }
    QMessageBox::critical(this, "HTTP ERROR",
                              QString("request error."
                                      "error code: %1"
                                      "error msg : %s").arg(errcode).arg(errmsg),
                              QMessageBox::Yes, QMessageBox::Yes);
}

QJsonObject YunClient::getRequestData(QString array)
{
    QByteArray byte_array = array.toLocal8Bit();
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array, &json_error);

    QJsonObject dataJson;
    if(json_error.error == QJsonParseError::NoError)
    {
        if(!parse_doucment.isObject())
            return dataJson;

        QJsonObject json = parse_doucment.object();
        if(json.contains("status")){
            QJsonValue value = json.take("status");
            if(!value.toBool()){
                errView(json);
                return dataJson;
            }
        }

        if(json.contains("data")){
            dataJson = json.take("data").toObject();
        }
    }

    return dataJson;
}

QJsonArray YunClient::setPagingInfo(QJsonObject json)
{
    QJsonArray listJson;
    if(json.contains("limit")){
        QJsonValue value = json.take("limit");
        lineedit_recordPage->setText(QString::number(value.toInt()));
    }

    QString total, current;
    if(json.contains("totalPages")){
        QJsonValue value = json.take("totalPages");
        total = QString::number(value.toInt());
    }

    if(json.contains("currentPage")){
        QJsonValue value = json.take("currentPage");
        current = QString::number(value.toInt());
    }
    if (total.compare("0") != 0 && current.compare("0") != 0)
        label_page->setText(QString("共: %1/%2").arg(current).arg(total));

    if(json.contains("totalRecords")){
        QJsonValue value = json.take("totalRecords");
        label_totalRecords->setText(QString("共：%1").arg(value.toInt()));
    }

    if (json.contains("list")){
        listJson = json.take("list").toArray();
    }

    return listJson;
}

void YunClient::setVersionList(QJsonArray json)
{
    rowList.clear();
    for (int i=0; i<json.size(); i++)
    {
        QJsonObject vjson = json.at(i).toObject();
        QStringList rowValue;

        rowValue.append(QString::number(i+1));
        if(vjson.contains("versionId")){
            QJsonValue value = vjson.take("versionId");
            rowValue.append(QString::number(value.toInt()));
        }
        if(vjson.contains("versionName")){
            QJsonValue value = vjson.take("versionName");
            rowValue.append(value.toString());
        }
        if(vjson.contains("versionSize")){
            QJsonValue value = vjson.take("versionSize");
            rowValue.append(QString::number(value.toDouble()));
        }
        if(vjson.contains("versionInfo")){
            QJsonValue value = vjson.take("versionInfo");
            rowValue.append(value.toString());
        }
        if(vjson.contains("releaseTime")){
            QJsonValue value = vjson.take("releaseTime");
            rowValue.append(value.toString());
        }

        rowList.append(rowValue);
    }

    update_list->setModelValue(rowList);
}

void YunClient::setUpdateList(QJsonObject json)
{
    if(json.isEmpty()){
        qDebug() << "  json is empty of set version info.";
        return;
    }

    sqlList.clear();
    rowList.clear();
    QStringList rowValue;
    if (json.contains("meidis")){
        QJsonArray mediaArr = json.take("meidis").toArray();
        for (int i=0; i<mediaArr.size(); i++)
        {
            rowValue.clear();
            QString mvPath, lyricPath;
            QJsonObject vjson = mediaArr.at(i).toObject();
            rowValue.append(QString::number(i+1));
            if(vjson.contains("type")){
                QJsonValue value = vjson.take("type");
                rowValue.append(value.toString());
            }
            if(vjson.contains("mname")){
                QJsonValue value = vjson.take("mname");
                rowValue.append(value.toString());
            }
            if(vjson.contains("serialId")){
                QJsonValue value = vjson.take("serialId");
                rowValue.append(QString::number(value.toInt()));
            }
            if(vjson.contains("mediaFilePath")){
                QJsonValue value = vjson.take("mediaFilePath");
                mvPath = value.toString();
                rowValue.append(mvPath.isEmpty() ? "无" : "有");
            }
            if(vjson.contains("lyric")){
                QJsonValue value = vjson.take("lyric");
                lyricPath = value.toString();
                rowValue.append(lyricPath.isEmpty() ? "无" : "有");
            }
            if(vjson.contains("sql")){
                QJsonValue value = vjson.take("sql");
                rowValue.insert(1, getOptType(value.toString()));
                sqlList.insert(rowList.size() + 1, value.toString());
            }

            rowValue.append("未更新");
            rowValue.append(mvPath);
            rowValue.append(lyricPath);
            rowList.append(rowValue);
        }
    }

    if (json.contains("actors")){
        QJsonArray actorArr = json.take("actors").toArray();
        for (int i=0; i<actorArr.size(); i++)
        {
            rowValue.clear();
            QString imagePath;
            QJsonObject vjson = actorArr.at(i).toObject();
            rowValue.append(QString::number(rowList.size() + 1));
            rowValue.append("ACTOR");
            if(vjson.contains("name")){
                QJsonValue value = vjson.take("name");
                rowValue.append(value.toString());
            }
            if(vjson.contains("serialId")){
                QJsonValue value = vjson.take("serialId");
                rowValue.append(QString::number(value.toInt()));
            }
            rowValue.append("无");
            if(vjson.contains("imgFilePath")){
                QJsonValue value = vjson.take("imgFilePath");
                imagePath = value.toString();
                rowValue.append(imagePath.isEmpty() ? "无" : "有");
            }

            if(vjson.contains("sql")){
                QJsonValue value = vjson.take("sql");
                rowValue.insert(1, getOptType(value.toString()));
                sqlList.insert(rowList.size() + 1, value.toString());
            }
            rowValue.append("未更新");
            rowValue.append(imagePath);
            rowValue.append("");
            rowList.append(rowValue);
        }
    }

    qDebug() << "request version record : " << rowList.size();
    emit sqlValue(rowList, sqlList);
}

QString YunClient::getOptType(const QString &sql)
{
    QStringList lists = sql.split(" ");
    QString optStr = lists.first().toLower();
    QString retStr = "NO TYPE";
    if (optStr.compare("insert") == 0){
        retStr = "INSERT";
    } else if (optStr.compare("update") == 0){
        retStr = "UPDATE";
    } else if (optStr.compare("DELETE") == 0){
        retStr = "DALETE";
    }

    return retStr;
}



