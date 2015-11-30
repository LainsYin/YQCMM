#include "songsonlinewidget.h"
#include "songinfowidget.h"
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
#include <QAxObject>
#include <QFileDialog>
#include <QProcessEnvironment>
#include <QDebug>
#include <QTimer>

SongsOnlineWidget::SongsOnlineWidget(QWidget *parent)
    : QWidget(parent)
{
    initWidget();
    readAndSetStyleSheet();
    initWidgetValue();   
    tableView_songsOnline->setMediaOnlineDelegate();

    pushButton_hide->setCheckable(true);
    connect(pushButton_preview, &QPushButton::clicked, this, &SongsOnlineWidget::preview);
    connect(pushButton_upload, &QPushButton::clicked, this, &SongsOnlineWidget::upload);
    connect(pushButton_save, &QPushButton::clicked, this, &SongsOnlineWidget::save);
    connect(pushButton_uploadServer, &QPushButton::clicked, this, &SongsOnlineWidget::upload_server);
    connect(pushButton_hide, &QPushButton::clicked, this, &SongsOnlineWidget::hideWidget);

    connect(tableView_songsOnline, &PagingTableView::currentRow, this, &SongsOnlineWidget::cancelMedia);
    connect(tableView_songsOnline, &PagingTableView::selectRow, this, &SongsOnlineWidget::setValue);

    max_serial_id = 0;
    retSize = "";
    row = -1;
    timer = NULL;

    widget_info->set_Tooltip_image();
    widget_info->setStrckCurrentIndex();
    tableView_songsOnline->setMedia_black_CW();

    setTab_order();


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

SongsOnlineWidget::~SongsOnlineWidget()
{

}

void SongsOnlineWidget::readAndSetStyleSheet()
{
    QFile qss(":/qss/songonline.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());
    qss.close();

    this->setObjectName("Widget");
    widgetTitle->setObjectName("TopWidget");
    widgetCenter->setObjectName("CenterWidget");
    label_title->setObjectName("TitleLabel");
    label_select->setObjectName("SelectLabel");

    lineEdit_upload->setObjectName("LineEdit");

    pushButton_preview->setObjectName("Button");
    pushButton_upload->setObjectName("Button");
    pushButton_save->setObjectName("Button");
    pushButton_uploadServer->setObjectName("Button");
    pushButton_hide->setObjectName("HiddenButton");
}

void SongsOnlineWidget::initWidget()
{
    widgetTitle = new QWidget(this);
    widgetCenter = new QWidget(this);

    widget_info = new SongInfoWidget(widgetCenter);
    label_title = new QLabel(widgetTitle);
    label_select = new QLabel(widgetCenter);

    lineEdit_upload = new QLineEdit(widgetTitle);
    pushButton_upload = new QPushButton(widgetTitle);
    pushButton_preview = new QPushButton(widgetTitle);
    pushButton_save = new QPushButton(widgetCenter);
    pushButton_uploadServer = new QPushButton(widgetCenter);
    pushButton_hide = new QPushButton(widgetCenter);

    tableView_songsOnline = new PagingTableView(this);

    previewLayout = new QHBoxLayout();
    uploadLayout = new QHBoxLayout();
    titleLayout = new QHBoxLayout();

    lineEdit_upload->setFixedHeight(36);
    lineEdit_upload->setFixedWidth(260);
    pushButton_preview->setFixedHeight(36);
    pushButton_preview->setFixedWidth(60);
    pushButton_upload->setFixedSize(90, 36);
    pushButton_save->setFixedSize(60, 36);
    pushButton_uploadServer->setFixedSize(60, 36);
    pushButton_hide->setFixedSize(14, 14);

    label_select->setFixedHeight(36);
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
    saveLayout->addWidget(pushButton_uploadServer);
    saveLayout->addStretch();
    saveLayout->addWidget(pushButton_hide);
    saveLayout->setContentsMargins(0, 15, 20, 10);
    saveLayout->setSpacing(30);

    infoSavelayout->addWidget(widget_info);
    infoSavelayout->addLayout(saveLayout);
    infoSavelayout->setContentsMargins(26, 0, 0, 0);
    infoSavelayout->setSpacing(0);

    centerLayout->addWidget(label_select);    
    centerLayout->addLayout(infoSavelayout);
    centerLayout->setContentsMargins(20, 0, 0, 0);
    centerLayout->setSpacing(0);
    widgetCenter->setLayout(centerLayout);

    tableViewLayout = new QVBoxLayout();
    vLayout = new QVBoxLayout();
    tableViewLayout->addWidget(tableView_songsOnline);
    tableViewLayout->setContentsMargins(20, 15, 12, 3);

    vLayout->addWidget(widgetTitle);
    vLayout->addWidget(widgetCenter);
    vLayout->addLayout(tableViewLayout);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);

    this->setLayout(vLayout);
}

void SongsOnlineWidget::initWidgetValue()
{
    label_title->setText("歌曲上线");
    label_select->setText("歌曲添加");
    lineEdit_upload->setPlaceholderText("表格内容必须与数据库字段一致");
    pushButton_preview->setText("浏览");
    pushButton_upload->setText("批量上传");
    pushButton_save->setText("保存");
    pushButton_uploadServer->setText("上传");

    if(!rowList.isEmpty())
        rowList.clear();

    if(!medias.isEmpty())
        medias.clear();;
}

void SongsOnlineWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void SongsOnlineWidget::initComboboxValue(MysqlQuery *sql)
{
    _sql = sql;

    _sql->getMediaLanguage(language);
    _sql->getMediaType(type);
    _sql->getMediaEffect(effect);
    _sql->getMediaResolution(resolution);
    _sql->getMediaQuality(quality);
    _sql->getMediaSource(source);
    _sql->getMediaVersion(version);
    _sql->getMediaRthym(rthym);
    _sql->getMediaPitch(pitch);

    qint64 _id = -1;
#ifndef YQC_TECH
    _sql->getMediaMaxMid(_id);
#else
    _sql->getMediaMaxSerial_id(_id);
#endif

    widget_info->setPlaceholder_serial_id(_id);
    widget_info->set_comboBox_Language(language);
    widget_info->set_comboBox_type(type);
    widget_info->set_comboBox_effect(effect);
    widget_info->set_comboBox_resolution(resolution);
    widget_info->set_comboBox_quality(quality);
    widget_info->set_comboBox_source(source);
    widget_info->set_comboBox_version(version);
    widget_info->set_comboBox_rthym(rthym);
    widget_info->set_comboBox_pitch(pitch);
    widget_info->set_comboBox_Remain();
}

void SongsOnlineWidget::setMediasValue(QList<Media> _medias)
{
    medias = _medias;
}

void SongsOnlineWidget::preview()
{
//    QMessageBox::information(this, "提示", "浏览");

    QString fileFormat("视频文件(*.xls *.xlsx)");
    QString document = QProcessEnvironment::systemEnvironment().value("USERPROFILE")+"\\Desktop";
    QString pathStr = QFileDialog::getOpenFileName(this,
                                                     "批量上传",
                                                     document,
                                                     fileFormat
                                                     );

    if(pathStr.isEmpty())
        return;

    lineEdit_upload->setText(pathStr);
}

void SongsOnlineWidget::upload()
{
    this->setCursor(Qt::WaitCursor);

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
        if(work_sheet_name.compare("新增歌曲") == 0)
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
            Media media;
            for(int j=column_start; j<=column_count; j++)
            {
                QAxObject *cell = work_sheet->querySubObject("Cells(int,int)", i, j);
                QAxObject *cell_01 = work_sheet->querySubObject("Cells(int)", i);
                QVariant value = cell_01->property("Value");
                QVariant cell_value = cell->property("Value");  //获取单元格内容

               if(j == 1)
                   media.mid = cell_value.toString();
               else if(j == 2)
                   media.serial_id = cell_value.toString();
               else if(j == 3)
                   media.name = cell_value.toString();
               else if(j == 4)
                   media.language = cell_value.toString();
               else if(j == 5)
                   media.type = cell_value.toString();

               else if(j == 6)
                   media.singer = cell_value.toString();
               else if(j == 7)
                   media.artist_sid_1 = cell_value.toString();
               else if(j == 8)
                   media.artist_sid_2 = cell_value.toString();
               else if(j == 9)
                   media.pinyin = cell_value.toString();
               else if(j == 10)
                   media.header = cell_value.toString();

               else if(j == 11)
                   media.head = cell_value.toString();
               else if(j == 12)
                   media.words = cell_value.toString();
               else if(j == 13)
                   media.path = cell_value.toString();
               else if(j == 14)
                   media.lyric = cell_value.toString();
               else if(j == 15)
                   media.original_track = cell_value.toString();

               else if(j == 16)
                   media.sound_track = cell_value.toString();
               else if(j == 17)
                   media.start_volume_1 = cell_value.toString();
               else if(j == 18)
                   media.start_volume_2 = cell_value.toString();
               else if(j == 19)
                   media.prelude = cell_value.toString();
               else if(j == 20)
                   media.effect = cell_value.toString();

               else if(j == 21)
                   media.version = cell_value.toString();
               else if(j == 22)
                   media.create_time = cell_value.toString();
               else if(j == 23)
                   media.stars = cell_value.toString();
               else if(j == 24)
                   media.hot = cell_value.toString();
               else if(j == 25)
                   media.count = cell_value.toString();

               else if(j == 26)
                   media.enabled = cell_value.toString();
               else if(j == 27)
                   media.black = cell_value.toString();
               else if(j == 28)
                   media.match = cell_value.toString();
               else if(j == 29)
                   media.update_time = cell_value.toString();
               else if(j == 30)
                   media.resolution = cell_value.toString();

               else if(j == 31)
                   media.quality = cell_value.toString();
               else if(j == 32)
                   media.source = cell_value.toString();
               else if(j == 33)
                   media.rhythm = cell_value.toString();
               else if(j == 34)
                   media.pitch = cell_value.toString();
               else if(j == 35)
                   media.info = cell_value.toString();

            }


            tempSave(media);
        }
    }

    excel->dynamicCall("Quit (void)");

    this->setCursor(Qt::ArrowCursor);
}

void SongsOnlineWidget::save()
{
    Media media;
    widget_info->getMediaValue(media);

    QString title("保存提示");
    QString content("必填项未填写完整！");
    QMessageBox box(QMessageBox::Warning, title, content);
    box.setStandardButtons(QMessageBox::Ok); //| QMessageBox::Cancel);
    box.setButtonText(QMessageBox::Ok, "确定");
//    box.setButtonText(QMessageBox::Cancel, "取消");

    if(widget_info->isRequiredOption()){
        box.exec();
        return;
    }

    tempSave(media);


    widget_info->setDefaultBorder();
    widget_info->setDefaultValue();


#ifndef YQC_TECH
    qint64 mid = getMaxMediaMid();
    if(mid != 0)
    {
        widget_info->setPlaceholder_serial_id(mid);
    }
    else
    {
        qint64 _mid = media.mid.toLongLong();
        widget_info->setPlaceholder_serial_id(_mid);
    }
#else
    qint64 serial_id = getMaxMediasSerial_id();
    if(serial_id != 0)
    {
        widget_info->setPlaceholder_serial_id(serial_id);
    }
    else
    {
        qint64 _serial_id = media.serial_id.toLongLong();
        widget_info->setPlaceholder_serial_id(_serial_id);
    }
#endif
}

void SongsOnlineWidget::tempSave(Media &media)
{
    QString title("保存提示");
    QString content;

#ifndef YQC_TECH
#else
    qint64 ser_id = _sql->isMediaSerialId(media.serial_id.toLongLong());
    if(ser_id > 0)
    {
        content = QString("serial_id : %1已经存在!\n 最大serial_id : %2").arg(media.serial_id).arg(ser_id);
        QMessageBox box(QMessageBox::Warning, title, content);
        box.setStandardButtons(QMessageBox::Ok);// | QMessageBox::Cancel);
        box.setButtonText(QMessageBox::Ok, "确定");
//        box.setButtonText(QMessageBox::Cancel, "取消");
        box.exec();
        return;
    }
#endif


    ////歌名,歌星名,语重复判断    关系与  0不存在 返回存在数量
    int count = _sql->mediaOnlineRepeat(media.name, media.singer, media.language.toInt());
    if(count != 0)
    {
        ///有重复情况提示继续保存还是取消
        int lankey = media.language.toInt();
        QString lan = language.value(lankey);
        title = QString("歌曲重复提示");
        content = QString("歌曲名:%1 \n"
                          "歌星名:%2 \n"
                          "语种:%3 \n"
                          "继续保存或取消?").arg(media.name).arg(media.singer).arg(lan);
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
    row = isExsidOfMid(media.mid.toLongLong());
    contentText = QString("mid:%1\n已存在是否覆盖?").arg(media.mid);
#else
    row = isExsitOfSerial_id(media.serial_id.toLongLong());
    contentText = QString("serial_id:%1\n已存在是否覆盖?").arg(media.serial_id);
#endif
    if(row != -1)
    {
        QMessageBox box(QMessageBox::Warning, "提示", contentText);
        box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        box.setButtonText(QMessageBox::Ok, "确定");
        box.setButtonText(QMessageBox::Cancel, "取消");
        if(box.exec() == QMessageBox::Ok)
        {
            QStringList list = getUpdateValue(media);
            rowList.replace(row, list);
            medias.replace(row, media);
        }
        else
            return;
    }
    else
    {
        updateView(media);
        medias.append(media);
    }
}

void SongsOnlineWidget::updateView(const Media &media)
{
//    QStringList list;
//    list.append(media.mid);
//    list.append(media.serial_id);
//    list.append(media.name);
//    list.append(media.singer);
//    list.append(language.value(media.language.toInt()));
//    list.append(type.value(media.type.toInt()));

    QStringList list = getUpdateValue(media);
    rowList.append(list);
    tableView_songsOnline->setModelValue(rowList);
}

QStringList SongsOnlineWidget::getUpdateValue(const Media &media)
{
    QStringList list;
    list.append(media.mid);
    list.append(media.serial_id);
    list.append(media.name);
    list.append(media.singer);
    list.append(language.value(media.language.toInt()));
    list.append(type.value(media.type.toInt()));
    list.append("");

    return list;
}

void SongsOnlineWidget::upload_server()
{
//    QFile file(media.path);/*c:/test/12345678.mp4*/
//    QString path = media.path;
//    int before = path.lastIndexOf("/");
//    int after = path.lastIndexOf(".");

//    QString mediaName = media.serial_id.right(6);
//    path = path.replace(before+1, after-before-1, mediaName);
//    file.rename(path);
//    if(file.exists())
//        media.path = widget_info->uploadV(path);
////    file.rename(media.path);

//    file.setFileName(media.lyric);
//    if(file.exists())
//            media.lyric = widget_info->uploadL(media.lyric);

//    if(_sql->insertMedia(media))
//            updateView(media);


    retSize = "-2";
    Thread *workerThread = new Thread(this, 1);
    connect(workerThread, &Thread::result, this, &SongsOnlineWidget::uploadsuccess);
    connect(workerThread, &Thread::start_upload, this, &SongsOnlineWidget::timeStart);
    connect(workerThread, &Thread::finished, workerThread, &QObject::deleteLater);
    workerThread->setSongInfoWidgetPointer(widget_info, _sql, &retSize);
    workerThread->setSongValue(medias);
    workerThread->start();
}

void SongsOnlineWidget::timeStart(qint64 serial_id)
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
    connect(timer, &QTimer::timeout, this, &SongsOnlineWidget::timeOver);
//    timer->start(1000);

    retSize = "-2";
    row = -1;
    if(serial_id != -1)
    {
        for(int i=0; i<rowList.count(); i++)
        {
            QStringList rowValue = rowList.at(i);
#ifndef YQC_TECH
            if(rowValue.at(0).toLongLong() == serial_id)
#else
            if(rowValue.at(1).toLongLong() == serial_id)
#endif
            {
                row = i;
                timer->start(1000);
                break;
            }

        }
    }
}

void SongsOnlineWidget::timeOver()
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
        tableView_songsOnline->setModelValue(rowList);

        QStringList list = retSize.split(",");
        if(list.last().compare("-2") == 0)
            return;
        if(!list.last().isEmpty() && list.last().compare("0") != 0)
        {
            qDebug() << "fist : " <<  list.first() << " last : " << list.last();
            //            if(list.first().compare(list.last()) == 0)
            //            {
            //                timer->stop();
            //                timer = NULL;
            //                qDebug() << " 100 stop";
            //            }

            if(list.first().compare("-1") == 0 && list.last().compare("-1") == 0)
            {
                timer->stop();
                timer = NULL;
                qDebug() << " 100 stop";
            }

        }
    }
}


void SongsOnlineWidget::cancelMedia(const int &row)
{
    QString name = rowList.at(row).at(2);
    QMessageBox box(QMessageBox::Warning, "取消提示", QString("确认取消:%1").arg(name));
    box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    box.setButtonText(QMessageBox::Ok, "确定");
    box.setButtonText(QMessageBox::Cancel, "取消");
    if(box.exec() == QMessageBox::Ok)
    {
        rowList.removeAt(row);
        medias.removeAt(row);
    }

    widget_info->setDefaultValue();
    widget_info->setDefaultBorder();
//    qint64 serial_id = getMaxMediasSerial_id();
//    if(serial_id != 0)
//        widget_info->setPlaceholder_serial_id(serial_id);
}

void SongsOnlineWidget::hideWidget(bool checked)
{
    if(checked)
        widget_info->setHidden(true);
    else
        widget_info->setHidden(false);
}

void SongsOnlineWidget::uploadsuccess(const qint64 &serial_id)
{
    if(serial_id != -1 && serial_id != 0 && serial_id != 1)
    {
        for(int i=0; i<rowList.count(); i++)
        {
            QStringList rowValue = rowList.at(i);
#ifndef YQC_TECH
            if(rowValue.at(0).toLongLong() == serial_id)
#else
            if(rowValue.at(1).toLongLong() == serial_id)
#endif
            {
                rowList.removeAt(i);
                medias.removeAt(i);
                break;
            }

        }
        tableView_songsOnline->setModelValue(rowList);
    }

    if(serial_id == 0)
    {
        QMessageBox::information(NULL, "提示", "上传失败\n上传歌曲类型不对！");
    }
    else if(serial_id == 1)
    {
        QMessageBox::information(NULL, "提示", QString("上传失败\nSERIAL_ID重复！").arg(serial_id));
    }


    qDebug() << "serial_id ret : " << serial_id;
}

void SongsOnlineWidget::setValue(const QModelIndex &index)
{
    int row = index.row();

    if(row >= 0 && row < medias.count())
    {
        widget_info->setMediavalue_online(medias.at(row));
        widget_info->setOldMediaValue(medias.at(row));
    }
}


int SongsOnlineWidget::isExsitOfSerial_id(const qint64 &serial_id)
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

int SongsOnlineWidget::isExsidOfMid(const qint64 &mid)
{
    int row = -1;
    for(int i=0; i<rowList.size(); i++)
    {
        if(rowList.at(i).isEmpty())
            continue;

        QStringList temp = rowList.at(i);
        qint64 id = temp.at(0).toLongLong();
        if(id == mid)
        {
            row = i;
            break;
        }
    }

    return row;
}

qint64 SongsOnlineWidget::getMaxMediasSerial_id()
{
    qint64 serial_id = 0;
    for (int i=0; i<medias.count(); i++)
    {
        QString idStr = medias.at(i).serial_id;
        qint64 id = idStr.toLongLong();
        if(id > serial_id)
            id = serial_id;
    }

    return serial_id;
}

qint64 SongsOnlineWidget::getMaxMediaMid()
{
    qint64 mid = 0;
    for (int i=0; i<medias.count(); i++)
    {
        QString idStr = medias.at(i).mid;
        qint64 id = idStr.toLongLong();
        if(id > mid)
            id = mid;
    }

    return mid;
}

void SongsOnlineWidget::setTab_order()
{
    this->setTabOrder(lineEdit_upload, pushButton_preview);
    this->setTabOrder(pushButton_preview, pushButton_upload);
}


