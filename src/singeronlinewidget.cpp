#include "singeronlinewidget.h"
#include "singerinfowdiget.h"
#include "pagingtableview.h"
#include "mysqlquery.h"
#include "thread.h"
#include "releasetech.h"
#include <QFile>
#include <QLabel>
#include <QPainter>
#include <QLineEdit>
#include <QTableView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QCoreApplication>
#include <QProcessEnvironment>
#include <QFileDialog>
#include <QAxObject>
#include <QTimer>
#include <QDebug>
SingerOnlineWidget::SingerOnlineWidget(QWidget *parent)
    : QWidget(parent)
{
    initWidget();
    readAndSetStyleSheet();
    initWidgetValue();

    widget_info->isSingerOnline();
    tableView_singerOnline->setActorOnlineDelegate();
    connect(pushButton_upload, &QPushButton::clicked, this, &SingerOnlineWidget::upload);
    connect(pushButton_preview, &QPushButton::clicked, this, &SingerOnlineWidget::preview);
    connect(pushButton_save, &QPushButton::clicked, this, &SingerOnlineWidget::save);

    pushButton_hide->setCheckable(true);
    connect(pushButton_hide, &QPushButton::clicked, this, &SingerOnlineWidget::hideWidget);
    connect(pushButton_uplosdServer, &QPushButton::clicked, this, &SingerOnlineWidget::upload_server);

    connect(tableView_singerOnline, &PagingTableView::currentRow, this, &SingerOnlineWidget::cancelActor);
    connect(tableView_singerOnline, &PagingTableView::selectRow, this, &SingerOnlineWidget::setValue);
//    connect(tableView_singerOnline, &PagingTableView::pointRow, this, &SingerOnlineWidget::setValuePos);


    retSize = "";
    row = -1;
    timer = NULL;
    widget_info->set_tooptip_image();
    tableView_singerOnline->setActorColumnWidth(7);

#ifndef YQC_TECH
    lineEdit_upload->setHidden(true);
    pushButton_preview->setHidden(true);
    pushButton_upload->setHidden(true);
#else
    lineEdit_upload->setHidden(false);
    pushButton_preview->setHidden(false);
    pushButton_upload->setHidden(false);
#endif
}

SingerOnlineWidget::~SingerOnlineWidget()
{

}

void SingerOnlineWidget::readAndSetStyleSheet()
{
    QFile qss(":/qss/songonline.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());
    qss.close();

    this->setObjectName("Widget");
    widgetTitle->setObjectName("TopWidget");
    widgetCenter->setObjectName("CenterWidget");
    label_title->setObjectName("TitleLabel");
    label_addSinger->setObjectName("SelectLabel");

    lineEdit_upload->setObjectName("LineEdit");

    pushButton_preview->setObjectName("Button");
    pushButton_upload->setObjectName("Button");
    pushButton_save->setObjectName("Button");
    pushButton_uplosdServer->setObjectName("Button");
    pushButton_hide->setObjectName("HiddenButton");
}

void SingerOnlineWidget::initWidget()
{
    widgetTitle = new QWidget(this);
    widgetCenter = new QWidget(this);

    widget_info = new SingerInfoWdiget(widgetCenter);

    label_title = new QLabel(widgetTitle);
    label_addSinger = new QLabel(widgetCenter);

    lineEdit_upload = new QLineEdit(widgetTitle);
    pushButton_upload = new QPushButton(widgetTitle);
    pushButton_preview = new QPushButton(widgetTitle);
    pushButton_save = new QPushButton(widgetCenter);
    pushButton_uplosdServer = new QPushButton(widgetCenter);
    pushButton_hide = new QPushButton(widgetCenter);

    tableView_singerOnline = new PagingTableView(this);

    previewLayout = new QHBoxLayout();
    uploadLayout = new QHBoxLayout();
    titleLayout = new QHBoxLayout();

    lineEdit_upload->setFixedHeight(36);
    lineEdit_upload->setFixedWidth(260);
    pushButton_preview->setFixedHeight(36);
    pushButton_preview->setFixedWidth(60);
    pushButton_upload->setFixedHeight(36);
    pushButton_upload->setFixedWidth(90);
    pushButton_save->setFixedHeight(36);
    pushButton_save->setFixedWidth(90);

    pushButton_uplosdServer->setFixedSize(60, 36);
    pushButton_hide->setFixedSize(14, 14);

    label_addSinger->setFixedHeight(36);
    widgetTitle->setFixedHeight(60);

    previewLayout->addWidget(lineEdit_upload);
    previewLayout->addWidget(pushButton_preview);
    previewLayout->setContentsMargins(0, 0, 0, 0);
    previewLayout->setSpacing(10);

    uploadLayout->addLayout(previewLayout);
    uploadLayout->addWidget(pushButton_upload);
    uploadLayout->setContentsMargins(0, 0, 0, 0);
    uploadLayout->setSpacing(30);

    titleLayout->addWidget(label_title);
    titleLayout->addStretch();
    titleLayout->addLayout(uploadLayout);
    titleLayout->setContentsMargins(20, 0, 24, 0);
    titleLayout->setSpacing(0);
    widgetTitle->setLayout(titleLayout);

    saveLayout = new QHBoxLayout();
    infoSavelayout = new QVBoxLayout();
    centerLayout = new QVBoxLayout();

    saveLayout->addStretch();
    saveLayout->addWidget(pushButton_save);
    saveLayout->addWidget(pushButton_uplosdServer);
    saveLayout->addStretch();
    saveLayout->addWidget(pushButton_hide);
    saveLayout->setContentsMargins(0, 15, 20, 10);
    saveLayout->setSpacing(30);

    infoSavelayout->addWidget(widget_info);
    infoSavelayout->addLayout(saveLayout);
    infoSavelayout->setContentsMargins(26, 0, 0, 0);
    infoSavelayout->setSpacing(0);

    centerLayout->addWidget(label_addSinger);
    centerLayout->addLayout(infoSavelayout);
    centerLayout->setContentsMargins(20, 0, 0, 0);
    centerLayout->setSpacing(0);
    widgetCenter->setLayout(centerLayout);

    tableViewLayout = new QVBoxLayout();
    vLayout = new QVBoxLayout();
    tableViewLayout->addWidget(tableView_singerOnline);
    tableViewLayout->setContentsMargins(20, 15, 12, 3);

    vLayout->addWidget(widgetTitle);
    vLayout->addWidget(widgetCenter);
    vLayout->addLayout(tableViewLayout);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);

    this->setLayout(vLayout);
}

void SingerOnlineWidget::initWidgetValue()
{
    label_title->setText("歌星上线");
    label_addSinger->setText("歌星添加");
    lineEdit_upload->setPlaceholderText("表格内容必须与数据库字段一致");
    pushButton_preview->setText("浏览");
    pushButton_upload->setText("批量上传");
    pushButton_save->setText("保存");
    pushButton_uplosdServer->setText("上传");


    QString runPath = QCoreApplication::applicationDirPath();
    runPath.append("/platforms/image/" );
}

void SingerOnlineWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void SingerOnlineWidget::initComboboxValue(MysqlQuery *sql)
{
    _sql = sql;

    _sql->getSex(sex);
    _sql->getNation(nation);

    qint64 _id = -1;
#ifndef YQC_TECH
    _sql->getActorMaxSid(_id);
#else
    _sql->getActorMaxSerial_id(_id);
#endif

    widget_info->setPlaceholder_serial_id(_id);
    widget_info->set_comboBox_sex(sex);
    widget_info->set_comboBox_nation(nation);
    widget_info->set_comboBox_Remain();
}

void SingerOnlineWidget::updateView(Actor actor)
{
//    QString filePath;
//    filePath = runPath + actor.name;
//    filePath += ".jpg";

//    QFile file(filePath);
//    if (!file.exists()){
//        CurlUpload *curlDownlaod = new CurlUpload();
//        curlDownlaod->downloadActorImage(actor.name, filePath);

//        delete curlDownlaod;
//    }

//    QStringList list;
//    list.append(actor.sid);
//    list.append(actor.serial_id);
//    if(!filePath.isEmpty())
//        list.append(filePath);
//    else
//        list.append("");
//    list.append(actor.name);
//    list.append(sex.value(actor.sex.toInt()));
//    list.append(nation.value(actor.nation.toInt()));

    QStringList list = getUpdateValue(actor);
    rowList.append(list);
    tableView_singerOnline->setModelValue(rowList);
}

QStringList SingerOnlineWidget::getUpdateValue(const Actor &actor)
{
//    QString filePath;
//    filePath = runPath + actor.name;
//    filePath += ".jpg";

//    QFile file(filePath);
//    if (!file.exists()){
//        CurlUpload *curlDownlaod = new CurlUpload();
//        curlDownlaod->downloadActorImage(actor.name, filePath);

//        delete curlDownlaod;
//    }

    QStringList list;
    list.append(actor.sid);
    list.append(actor.serial_id);
    list.append(actor.image);
//    if(!filePath.isEmpty())
//        list.append(filePath);
//    else
//        list.append("");
    list.append(actor.name);
    list.append(sex.value(actor.sex.toInt()));
    list.append(nation.value(actor.nation.toInt()));

    return list;
}




void SingerOnlineWidget::upload()
{
    QAxObject *excel = NULL;
    QAxObject *work_books = NULL;
    QAxObject *work_book = NULL;
    excel = new QAxObject("Excel.Application");
    if (excel->isNull()) {//网络中很多使用excel==NULL判断，是错误的
        QMessageBox::critical(0, "错误信息", "没有找到EXCEL应用程序");
        return;
    }

    excel->setProperty("Visible", false);
    work_books = excel->querySubObject("WorkBooks");
    QString path = lineEdit_upload->text();
    if(path.isEmpty())
    {
        QMessageBox::warning(NULL, "提示", "批量上传路径不能为空,\n点击浏览选择批量上传文件。");
        return;
    }
    work_books->dynamicCall("Open (const QString&)", QString(path));
    QVariant title_value = excel->property("Caption");  //获取标题
    qDebug()<<QString("excel title : ")<<title_value;
    work_book = excel->querySubObject("ActiveWorkBook");
    QAxObject *work_sheets = work_book->querySubObject("WorkSheets");  //Sheets也可换用WorkSheets

    int sheet_count = work_sheets->property("Count").toInt();  //获取工作表数目
    qDebug()<<QString("sheet count : ")<<sheet_count;

    int newsong_wheet_index = 0;
    for(int i=1; i<=sheet_count; i++)
    {
        QAxObject *work_sheet = work_book->querySubObject("Sheets(int)", i);  //Sheets(int)也可换用Worksheets(int)
        QString work_sheet_name = work_sheet->property("Name").toString();  //获取工作表名称
        QString message = QString("sheet ")+QString::number(i, 10)+ QString(" name");
        if(work_sheet_name.compare("新增歌手") == 0)
        {
            newsong_wheet_index = i;
            break;
        }
        qDebug()<<message<<work_sheet_name;
    }

    if(newsong_wheet_index != 0)
    {
        QAxObject *work_sheet = work_book->querySubObject("Sheets(int)", newsong_wheet_index);
        QAxObject *used_range = work_sheet->querySubObject("UsedRange");
        QAxObject *rows = used_range->querySubObject("Rows");
        QAxObject *columns = used_range->querySubObject("Columns");
        int row_start = used_range->property("Row").toInt();  //获取起始行
        int column_start = used_range->property("Column").toInt();  //获取起始列
        int row_count = rows->property("Count").toInt();  //获取行数
        int column_count = columns->property("Count").toInt();  //获取列数
        for(int i=row_start+1; i<=row_count; i++)
        {
            Actor actor;
            for(int j=column_start; j<=column_count; j++)
            {
                QAxObject *cell = work_sheet->querySubObject("Cells(int,int)", i, j);
                QAxObject *cell_01 = work_sheet->querySubObject("Cells(int)", i);
                QVariant value = cell_01->property("Value");
                QVariant cell_value = cell->property("Value");  //获取单元格内容

               if(j == 1)
                   actor.sid = cell_value.toString();
               else if(j == 2)
                   actor.serial_id = cell_value.toString();
               else if(j == 3)
                   actor.name = cell_value.toString();
               else if(j == 4)
                   actor.nation = cell_value.toString();
               else if(j == 5)
                   actor.sex = cell_value.toString();

               else if(j == 6)
                   actor.pinyin = cell_value.toString();
               else if(j == 7)
                   actor.header = cell_value.toString();
               else if(j == 8)
                   actor.head = cell_value.toString();
               else if(j == 9)
                   actor.words = cell_value.toString();
               else if(j == 10)
                   actor.song_count = cell_value.toString();

               else if(j == 11)
                   actor.stars = cell_value.toString();
               else if(j == 12)
                   actor.count = cell_value.toString();
               else if(j == 13)
                   actor.order = cell_value.toString();
               else if(j == 14)
                   actor.enabled = cell_value.toString();
               else if(j == 15)
                   actor.black = cell_value.toString();

               else if(j == 16)
                   actor.info = cell_value.toString();

            }

            tempSave(actor);
        }
    }

    excel->dynamicCall("Quit (void)");
}

void SingerOnlineWidget::preview()
{
    QString fileFormat(QStringLiteral("视频文件(*.xls *.xlsx)"));
    QString document = QProcessEnvironment::systemEnvironment().value("USERPROFILE")+"\\Desktop";
    QString pathStr = QFileDialog::getOpenFileName(this,
                                                     QStringLiteral("批量上传"),
                                                     document,
                                                     fileFormat
                                                     );

    if(pathStr.isEmpty())
        return;

    lineEdit_upload->setText(pathStr);
}

void SingerOnlineWidget::save()
{

    if(widget_info->isRequiredOption()){
        QMessageBox box(QMessageBox::Warning, "保存提示", "必填项未填写完整！");
        box.setStandardButtons(QMessageBox::Ok);// | QMessageBox::Cancel);
        box.setButtonText(QMessageBox::Ok, "确定");
//        box.setButtonText(QMessageBox::Cancel, "取消");
        box.exec();
        return;
    }

    Actor actor;
    widget_info->getActorOnlineValue(actor);

    tempSave(actor);

    widget_info->setDefaultBorder();
    widget_info->setDefaultValue();

#ifndef YQC_TECH
    qint64 sid = getMaxActorSid();
    if(sid != 0)
    {
        widget_info->setPlaceholder_serial_id(sid);
    }
    else
    {
        qint64 _sid = actor.sid.toLongLong();
        widget_info->setPlaceholder_serial_id(_sid);
    }
#else
    qint64 serial_id = getMaxActorSerial_id();
    if(serial_id != 0)
    {
        widget_info->setPlaceholder_serial_id(serial_id);
    }
    else
    {
        qint64 _serial_id = actor.serial_id.toLongLong();
        widget_info->setPlaceholder_serial_id(_serial_id);
    }
#endif

}


void SingerOnlineWidget::tempSave(Actor &actor)
{
    ///-1 没有重复
#ifndef YQC_TECH
#else
    qint64 ser_id = _sql->isActorSerialId(actor.serial_id.toLongLong());
    if(ser_id > 0)
    {
        QString content = QString("serial_id : %1已经存在!\n 最大serial_id : %2").arg(actor.serial_id).arg(ser_id);
        QMessageBox box(QMessageBox::Warning, "SERIAL_ID重复提示", content);
        box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        box.setButtonText(QMessageBox::Ok, "确定");
        box.setButtonText(QMessageBox::Cancel, "取消");
        box.exec();
        return;
    }
#endif

    ////歌星名,性别, 地区, 重复判断    关系与  0不存在 返回存在数量
    int count = _sql->actorOnlineRepeat(actor.name, actor.sex.toInt(), actor.nation.toInt());
    if(count != 0)
    {
        ///有重复情况提示继续保存还是取消
        int key = actor.sex.toInt();
        QString _sex = sex.value(key);
        key = actor.nation.toInt();
        QString _nation = nation.value(key);
        QString title = QString("歌重复提示");
        QString content = QString("歌星名:%1 \n"
                          "性别:%2 \n"
                          "地区:%3 \n"
                          "继续保存或取消?").arg(actor.name).arg(_sex).arg(_nation);
        QMessageBox box(QMessageBox::Warning, title, content);
        box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        box.setButtonText(QMessageBox::Ok, "确定");
        box.setButtonText(QMessageBox::Cancel, "取消");
        if(box.exec() == QMessageBox::Cancel)
        {
            return;
        }
    }


    ///
    ///临时库serial_id重复检测
    ///重复返回所在行 否则返回-1
    int row;
    QString contentText;
#ifndef YQC_TECH
    row = isExsitOfSid(actor.sid.toLongLong());
    contentText = QString("sid:%1\n已存在是否覆盖?").arg(actor.sid);
#else
    row = isExsitOfSerial_id(actor.serial_id.toLongLong());
    contentText = QString("serial_id:%1\n已存在是否覆盖?").arg(actor.serial_id);
#endif
    if(row != -1)
    {
        QMessageBox box(QMessageBox::Warning, "提示", contentText);
        box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        box.setButtonText(QMessageBox::Ok, "确定");
        box.setButtonText(QMessageBox::Cancel, "取消");
        if(box.exec() == QMessageBox::Ok)
        {
            QStringList list = getUpdateValue(actor);
            rowList.replace(row, list);
            actors.replace(row, actor);
        }
        else
            return;
    }
    else
    {
        updateView(actor);
        actors.append(actor);
    }
}

void SingerOnlineWidget::hideWidget(bool checked)
{
    if(checked)
        widget_info->setHidden(true);
    else
        widget_info->setHidden(false);
}

void SingerOnlineWidget::upload_server()
{
//    QString path = actor.image;
//    if(path.isEmpty())
//        return;
//    QFile file(path);
//    if(file.exists())
//    {
//        QString retStr = "";
//        CurlUpload *curlUpload = new CurlUpload();
//        retStr = curlUpload->uploadActorImage(path);
//        widget_info->setImageOnlineRetPath(retStr);

//        delete curlUpload;
//    }

//    if(_sql->insertActor(actor))
    //        updateView(actor);


    retSize = "-2";
    Thread *workerThread = new Thread(this, 2);
    connect(workerThread, &Thread::result, this, &SingerOnlineWidget::uploadsuccess);
    connect(workerThread, &Thread::finished, workerThread, &QObject::deleteLater);
    workerThread->setSingerInfoWidgetPointer(widget_info, _sql, &retSize);
    workerThread->setSingerValue(actors);
    workerThread->start();
}

void SingerOnlineWidget::timeStart(qint64 serial_id)
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
    connect(timer, &QTimer::timeout, this, &SingerOnlineWidget::timeOver);
//    timer->start(1000);
    retSize = "-2";

    row = -1;
    if(serial_id != -1)
    {
        for(int i=0; i<rowList.count(); i++)
        {
            QStringList rowValue = rowList.at(i);
            if(rowValue.at(1).toLongLong() == serial_id)
            {
                row = i;
                timer->start(1000);
                break;
            }

        }
    }
}

void SingerOnlineWidget::timeOver()
{
    if(retSize.isEmpty())
    {
         timer->stop();
         qDebug() << " isempty stop  ";
    }

    if(row != -1)
    {
        int count = rowList.count();
        if(count < row || count < 1)
        {
            timer->stop();
            timer = NULL;
            return;
        }

        QStringList rlist = rowList.at(row);
        rlist.replace(6, retSize);
        rowList.replace(row, rlist);
        tableView_singerOnline->setModelValue(rowList);

        QStringList list = retSize.split(",");
        if(list.last().compare("-2") == 0)
            return;
        if(!list.last().isEmpty() && list.last().compare("0") != 0)
        {
            qDebug() << "fist : " <<  list.first() << " last : " << list.last();
            if(list.first().compare(list.last()) == 0)
            {
                timer->stop();
                timer = NULL;
                qDebug() << " 100 stop";
            }
        }
    }
}


void SingerOnlineWidget::uploadsuccess(const qint64 &serial_id)
{
    if(serial_id != -1 && serial_id != 0 && serial_id != 1)
    {
        for(int i=0; i<rowList.count(); i++)
        {
            QStringList rowValue = rowList.at(i);
            if(rowValue.at(1).toLongLong() == serial_id)
            {
                rowList.removeAt(i);
                actors.removeAt(i);
                break;
            }

        }
        tableView_singerOnline->setModelValue(rowList);
    }

    if(serial_id == 0)
    {
        QMessageBox::information(NULL, "提示", "上传失败\n上传图片类型不对！");
    }
    else if(serial_id == 1)
    {
        QMessageBox::information(NULL, "提示", QString("上传失败\n资源服务器重复！").arg(serial_id));
    }
}

void SingerOnlineWidget::setValue(const QModelIndex &index)
{
    int row = index.row();
    if(row >= 0 && row < actors.count())
    {
        widget_info->setActorValue_online(actors.at(row));
        widget_info->setOldActorValue(actors.at(row));
    }
}

void SingerOnlineWidget::setValuePos(const int &row)
{
//    if(row == -1)
//    {
//        widget_info->setDefaultBorder();
//        widget_info->setDefaultValue();
//        qint64 serial_id = getMaxActorSerial_id();
//        if(serial_id != 0)
//        {
//            widget_info->setPlaceholder_serial_id(serial_id);
//        }
//        else
//        {
//            qint64 _serial_id = actor.serial_id.toLongLong();
//            widget_info->setPlaceholder_serial_id(_serial_id);
//        }
//    }
}

int SingerOnlineWidget::isExsitOfSerial_id(const qint64 &serial_id)
{
    int row = -1;
    for(int i=0; i<rowList.size(); i++)
    {
        if(rowList.at(i).isEmpty())
            continue;

        QStringList temp = rowList.at(i);
        qint64 id = temp.at(1).toLongLong();
        if(id == serial_id)
        {
            row = i;
            break;
        }
    }

    return row;
}

int SingerOnlineWidget::isExsitOfSid(const qint64 &sid)
{
    int row = -1;
    for(int i=0; i<rowList.size(); i++)
    {
        if(rowList.at(i).isEmpty())
            continue;

        QStringList temp = rowList.at(i);
        qint64 id = temp.at(0).toLongLong();
        if(id == sid)
        {
            row = i;
            break;
        }
    }

    return row;
}

qint64 SingerOnlineWidget::getMaxActorSerial_id()
{
    qint64 serial_id = 0;
    for (int i=0; i<actors.count(); i++)
    {
        QString idStr = actors.at(i).serial_id;
        qint64 id = idStr.toLongLong();
        if(id > serial_id)
            id = serial_id;
    }

    return serial_id;
}

qint64 SingerOnlineWidget::getMaxActorSid()
{
    qint64 sid = 0;
    for (int i=0; i<actors.count(); i++)
    {
        QString idStr = actors.at(i).sid;
        qint64 id = idStr.toLongLong();
        if(id > sid)
            id = sid;
    }

    return sid;
}

void SingerOnlineWidget::cancelActor(const int &row)
{
    QString name = rowList.at(row).at(2);
    QMessageBox box(QMessageBox::Warning, "取消提示", QString("确认取消歌星:%1").arg(name));
    box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    box.setButtonText(QMessageBox::Ok, "确定");
    box.setButtonText(QMessageBox::Cancel, "取消");
    if(box.exec() == QMessageBox::Ok)
    {
        rowList.removeAt(row);
        actors.removeAt(row);
    }

    widget_info->setDefaultValue();
    widget_info->setDefaultBorder();
}
