#include "singerquerywidget.h"
#include "pagingtableview.h"
#include "mysqlquery.h"
#include "tablemodel.h"
#include "singerinfodialog.h"
#include "yqcdelegate.h"
#include "releasetech.h"

#include <QFile>
#include <QLabel>
#include <QPainter>
#include <QComboBox>
#include <QLineEdit>
#include <QTableView>
#include <QPushButton>
#include <QListView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QAxObject>
#include <QSqlQueryModel>
#include <QDesktopServices>
#include <QProcessEnvironment>
#include <QProgressDialog>
#include <QDateTime>
#include <QDebug>

#define  MaxItems 5

SingerQueryWidget::SingerQueryWidget(QWidget *parent)
    : QWidget(parent),
      _sql(NULL)
{
    initWidget();
    readAndSetStyleSheet();
    initWidgetValue();


    connect(pushButton_export_excel, &QPushButton::clicked, this, &SingerQueryWidget::export_excel_clicked);
    connect(pushButton_search, &QPushButton::clicked, this, &SingerQueryWidget::search_singer_clicked);
    connect(lineEdit_search, &QLineEdit::editingFinished, this, &SingerQueryWidget::setActorValue);

    connect(tableView_singerQuery, &PagingTableView::updateView, this, &SingerQueryWidget::setActorValue);
    connect(tableView_singerQuery, &PagingTableView::currentRow, this, &SingerQueryWidget::setActorInfo);
    connect(tableView_singerQuery, &PagingTableView::dele, this, &SingerQueryWidget::deleteActor);

    connect(comboBox_sex, SIGNAL(currentIndexChanged(QString)), this, SLOT(combobox_currentIndexChanged(QString)));
    connect(comboBox_nation, SIGNAL(currentIndexChanged(QString)), this, SLOT(combobox_currentIndexChanged(QString)));
    connect(comboBox_stars, SIGNAL(currentIndexChanged(QString)), this, SLOT(combobox_currentIndexChanged(QString)));
    connect(comboBox_enabled, SIGNAL(currentIndexChanged(QString)), this, SLOT(combobox_currentIndexChanged(QString)));

    initFlag = false;
    pushButton_search->setDefault(true);


    tableView_singerQuery->setShowCheckBox();
    tableView_singerQuery->setQueryActorDelegate();
    pushButton_checkBox = new QPushButton(tableView_singerQuery);
    pushButton_checkBox->setCheckable(true);
    connect(pushButton_checkBox, &QPushButton::clicked, tableView_singerQuery, &PagingTableView::checkBoxIsAll);
    QHeaderView *headerView = tableView_singerQuery->tableView->horizontalHeader();
    QRect rect = headerView->geometry();
    QRect checkBoxRect = QRect(rect.x()+8, rect.y()+12, 16, 16);
    pushButton_checkBox->setGeometry(checkBoxRect);
    pushButton_checkBox->setObjectName("CheckBoxButton");

    connect(pushButton_addBlack, &QPushButton::clicked, this, &SingerQueryWidget::add_black);

#ifndef YQC_TECH
    pushButton_export_excel->setHidden(true);
#else
    pushButton_export_excel->setHidden(false);
#endif
}

SingerQueryWidget::~SingerQueryWidget()
{

}

void SingerQueryWidget::readAndSetStyleSheet()
{
    QFile qss(":/qss/singerquery.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());
    qss.close();

    this->setObjectName("Widget");
    widgetTitle->setObjectName("TopWidget");
    widgetCenter->setObjectName("CenterWidget");
    label_title->setObjectName("TitleLabel");
    label_select->setObjectName("SelectLabel");
    label_result->setObjectName("ResultLabel");

    label_sex->setObjectName("Label");
    label_nation->setObjectName("Label");
    label_stars->setObjectName("Label");
    label_enabled->setObjectName("Label");
    label_search->setObjectName("Label");

    pushButton_addBlack->setObjectName("Button");
    pushButton_export_excel->setObjectName("Button");
    pushButton_search->setObjectName("SerachButton");
    lineEdit_search->setObjectName("SerachEdit");

    comboBox_sex->setObjectName("Combobox");
    comboBox_nation->setObjectName("Combobox");
    comboBox_stars->setObjectName("Combobox");
    comboBox_enabled->setObjectName("Combobox");
}

void SingerQueryWidget::initWidget()
{
    widgetTitle = new QWidget(this);
    widgetCenter = new QWidget(this);

    lineEdit_search = new QLineEdit(widgetTitle);
    pushButton_search = new QPushButton(lineEdit_search);
    pushButton_export_excel = new QPushButton(widgetTitle);
    pushButton_addBlack = new QPushButton(widgetCenter);

    label_search = new QLabel(widgetTitle);
    label_title = new QLabel(widgetTitle);
    label_select = new QLabel(widgetCenter);
    label_sex = new QLabel(widgetCenter);
    label_nation = new QLabel(widgetCenter);
    label_stars = new QLabel(widgetCenter);
    label_enabled = new QLabel(widgetCenter);
    label_result = new QLabel(this);

    comboBox_sex = new QComboBox(widgetCenter);
    comboBox_nation = new QComboBox(widgetCenter);
    comboBox_stars = new QComboBox(widgetCenter);
    comboBox_enabled = new QComboBox(widgetCenter);

    QListView *view_sex = new QListView();
    QListView *view_nation = new QListView();
    QListView *view_stars = new QListView();
    QListView *view_enabled = new QListView();

    comboBox_sex->setView(view_sex);
    comboBox_nation->setView(view_nation);
    comboBox_stars->setView(view_stars);
    comboBox_enabled->setView(view_enabled);
    comboBox_sex->setMaxVisibleItems(MaxItems);
    comboBox_nation->setMaxVisibleItems(MaxItems);
    comboBox_stars->setMaxVisibleItems(MaxItems);
    comboBox_enabled->setMaxVisibleItems(MaxItems);


    tableView_singerQuery = new PagingTableView(this);
    tableView_singerQuery->model->setMediaOrActor(false);

    pushButton_addBlack->setFixedSize(90, 36);
    label_search->setFixedWidth(56);
    pushButton_export_excel->setFixedHeight(36);
    pushButton_export_excel->setFixedWidth(90);
    lineEdit_search->setFixedWidth(260);
    lineEdit_search->setFixedHeight(36);

    comboBox_sex->setFixedHeight(36);
    comboBox_sex->setFixedWidth(110);
    comboBox_nation->setFixedHeight(36);
    comboBox_nation->setFixedWidth(110);
    comboBox_stars->setFixedHeight(36);
    comboBox_stars->setFixedWidth(110);
    comboBox_enabled->setFixedHeight(36);
    comboBox_enabled->setFixedWidth(110);

    searchSingerLayout = new QHBoxLayout();
    searchLayout = new QHBoxLayout();
    titleLayout = new QHBoxLayout();

    searchSingerLayout->addWidget(label_search);
    searchSingerLayout->addWidget(lineEdit_search);
    searchSingerLayout->setContentsMargins(0, 0, 0, 0);
    searchSingerLayout->setSpacing(12);

    searchLayout->addLayout(searchSingerLayout);
    searchLayout->addWidget(pushButton_export_excel);
    searchLayout->setContentsMargins(0, 0, 0, 0);
    searchLayout->setSpacing(20);

    titleLayout->addWidget(label_title);
    titleLayout->addStretch();
    titleLayout->addLayout(searchLayout);
    titleLayout->setContentsMargins(20, 10, 12, 10);
    widgetTitle->setLayout(titleLayout);
    widgetTitle->setFixedHeight(60);

    sexLayout = new QHBoxLayout();
    nationLayout = new QHBoxLayout();
    starsLayout = new QHBoxLayout();
    enabledLayout = new QHBoxLayout();
    conditionLayout = new QHBoxLayout();
    centerLayout = new QVBoxLayout();

    sexLayout->addWidget(label_sex);
    sexLayout->addWidget(comboBox_sex);
    sexLayout->setContentsMargins(0, 0, 0, 0);
    sexLayout->setSpacing(6);

    nationLayout->addWidget(label_nation);
    nationLayout->addWidget(comboBox_nation);
    nationLayout->setContentsMargins(0, 0, 0, 0);
    nationLayout->setSpacing(8);

    starsLayout->addWidget(label_stars);
    starsLayout->addWidget(comboBox_stars);
    starsLayout->setContentsMargins(0, 0, 0, 0);
    starsLayout->setSpacing(8);

    enabledLayout->addWidget(label_enabled);
    enabledLayout->addWidget(comboBox_enabled);
    enabledLayout->setContentsMargins(0, 0, 0, 0);
    enabledLayout->setSpacing(8);

    conditionLayout->addLayout(sexLayout);
    conditionLayout->addLayout(nationLayout);
    conditionLayout->addLayout(starsLayout);
    conditionLayout->addLayout(enabledLayout);
    conditionLayout->addStretch();
    conditionLayout->setContentsMargins(26, 0, 0, 0);
    conditionLayout->setSpacing(30);

    centerLayout->addWidget(label_select);
    centerLayout->addLayout(conditionLayout);
    centerLayout->setContentsMargins(20, 15, 12, 20);
    widgetCenter->setLayout(centerLayout);


    blackLayout = new QHBoxLayout();
    blackLayout->addWidget(label_result);
    blackLayout->addStretch();
    blackLayout->addWidget(pushButton_addBlack);
    blackLayout->setContentsMargins(0, 15, 0, 0);

    tableViewLayout = new QVBoxLayout();
//    tableViewLayout->addWidget(label_result);
    tableViewLayout->addLayout(blackLayout);
    tableViewLayout->addWidget(tableView_singerQuery);
    tableViewLayout->setContentsMargins(20, 0, 12, 9);
    tableViewLayout->setSpacing(10);

    vLayout = new QVBoxLayout();
    vLayout->addWidget(widgetTitle);
    vLayout->addWidget(widgetCenter);
    vLayout->addLayout(tableViewLayout);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);

    this->setLayout(vLayout);
}

void SingerQueryWidget::initWidgetValue()
{
    label_title->setText("歌星查询");
    label_select->setText("筛选条件");
    label_sex->setText("歌星性别");
    label_nation->setText("歌星区域");
    label_stars->setText("歌星评星");
    label_enabled->setText("歌星状态");
    label_result->setText("搜索结果");
    label_search->setText("歌星搜索");

    lineEdit_search->setPlaceholderText("歌星名称");
    pushButton_export_excel->setText("导出总表");
    pushButton_addBlack->setText("加入黑名单");

//    tableView_singerQuery->setQueryActorDelegate();
    QRect rect = lineEdit_search->geometry();
    lineEdit_search->setTextMargins(0, 0, 20, 0);
    pushButton_search->setGeometry(rect.width()-20, 10, 14, 14);
    pushButton_search->setCursor(Qt::PointingHandCursor);
}

void SingerQueryWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void SingerQueryWidget::search_singer_clicked()
{
    comboBox_enabled->setCurrentIndex(0);
    comboBox_nation->setCurrentIndex(0);
    comboBox_sex->setCurrentIndex(0);
    comboBox_stars->setCurrentIndex(0);

    tableView_singerQuery->initPagingQueryLimitArgu();
    setActorValue();
}

void SingerQueryWidget::setActorValue()
{

    ActorPagingQuery argu;
    getQueryCondition(argu);

    qint64 total = 0;
    _sql->queryCountActor_A(argu, total);
    tableView_singerQuery->setTotal_Rows_Pages(total);

    query.clear();
    tableView_singerQuery->tableView->clear();

    _sql->queryActor(argu, tableView_singerQuery->limitArgu, query);
//    tableView_singerQuery->showUploadData(query, false);
    tableView_singerQuery->showQuerySingerData(query);
}

void SingerQueryWidget::setActorInfo(const int &row)
{
    SingerInfoDialog *dialog = new SingerInfoDialog();

    query.seek(row);
    qint64 sid = query.value("_sid").toLongLong();
    QModelIndex index = tableView_singerQuery->model->index(row, 1);

    QSqlQuery _query;
    Actor _actor;
    _actor.image = index.data().toString();
//    if(!_sql->queryActorOfSerialId(serial, _query))
//        return;
    if(!_sql->queryActorOfSid(sid, _query))
        return;

    dialog->info->sex = sex;
    dialog->info->nation = nation;

    getValue(_query, _actor);
    dialog->setValue(_actor);

    dialog->exec();
    setActorValue();
}

void SingerQueryWidget::deleteActor(const int &row)
{
    query.seek(row);
    qint64 serial = query.value("_serial_id").toLongLong();
    QString name = query.value("_name").toString();
    int sid1 = query.value("_sid").toInt();
    int songCount = _sql->isSongOfSid(sid1);

#ifndef YQC_TECH
    if(serial != 0)
    {
        QMessageBox::warning(NULL, "提示", "基础库歌星不能删除");
        return;
    }
#else
#endif

    if(songCount != -1 && songCount != 0)
    {
        QString content = QString("歌星名：%1 \n"
                                  "对应歌曲有%2，不能删除该歌星。").arg(name).arg(songCount);
        QMessageBox box(QMessageBox::Warning, "删除错误", content);
        box.setStandardButtons(QMessageBox::Ok);
        box.setButtonText(QMessageBox::Ok, "确定");
        box.exec();
        return;
    }
    else
    {
        QMessageBox box(QMessageBox::Warning, "删除提示", QString("确认删除歌手:%1?").arg(name));
        box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        box.setButtonText(QMessageBox::Ok, "确定");
        box.setButtonText(QMessageBox::Cancel, "取消");
        if(box.exec() == QMessageBox::Ok)
        {
            _sql->deleteActor(sid1, serial, name);
            setActorValue();
        }
    }
}

void SingerQueryWidget::getQueryCondition(ActorPagingQuery &argu)
{
    argu._sex = sex.key(comboBox_sex->currentText(), -1);
    argu._nation = nation.key(comboBox_nation->currentText(), -1);
    argu._enabled = enabled.key(comboBox_enabled->currentText(), -1);

    argu._stars = comboBox_stars->currentText().compare("全部") == 0?"":comboBox_stars->currentText();
    argu.singerName = lineEdit_search->text();
}

void SingerQueryWidget::getValue(QSqlQuery &_query, Actor &_actor)
{
    _query.first();
    _actor.sid = _query.value("sid").toString();
    _actor.serial_id = _query.value("serial_id").toString();
    _actor.name = _query.value("name").toString();
    _actor.nation = _query.value("nation").isNull()?"":_query.value("nation").toString();
    _actor.sex = _query.value("sex").isNull()?"":_query.value("sex").toString();

    _actor.pinyin = _query.value("pinyin").toString();
    _actor.header = _query.value("header").toString();
    _actor.head = _query.value("head").toString();
    _actor.words = _query.value("words").toString();
    _actor.song_count = _query.value("song_count").toString();

    QString sta =  _query.value("stars").toString();
    int index = sta.indexOf(".");
    if(index == -1)
        sta.append(".0");

    _actor.stars = sta;
    _actor.count = _query.value("count").toString();
    _actor.order = _query.value("order").isNull()?"":_query.value("order").toString();
    _actor.enabled = _query.value("enabled").toString();
    _actor.black = _query.value("black").toString();

    _actor.info = _query.value("info").isNull()?"":_query.value("info").toString();
}

void SingerQueryWidget::initSql(MysqlQuery *sql)
{
    initFlag = false;
    _sql = sql;
    initCombobox();
    lineEdit_search->clear();
    initFlag = true;

    tableView_singerQuery->setSexNation(nation, sex);
    tableView_singerQuery->setActor_black_CW(8);
}

void SingerQueryWidget::initCombobox()
{
    _sql->getNation(nation);
    _sql->getSex(sex);

    set_comboBox_value(comboBox_sex, sex);
    set_comboBox_value(comboBox_nation, nation);
    setEnabledStars();
}

void SingerQueryWidget::set_comboBox_value(QComboBox *combobox, QMap<int, QString> &val)
{
    QList<QString> tempVal = val.values();
    tempVal.insert(0, "全部");
    QStringList value(tempVal);
    combobox->clear();
    combobox->addItems(value);
}

void SingerQueryWidget::setEnabledStars()
{
    if(!enabled.isEmpty())
        enabled.clear();
    enabled.insert(0, "全部");
    enabled.insert(1, "启用");
    enabled.insert(2, "禁用");
    comboBox_enabled->clear();
    comboBox_enabled->addItems(enabled.values());

    if(!stars.isEmpty())
        stars.clear();
    stars.insert(0,"全部");
    stars.insert(1,"5.0");
    stars.insert(2,"4.5");
    stars.insert(3,"4.0");
    stars.insert(4,"3.5");
    stars.insert(5,"3.0");
    stars.insert(6,"2.5");
    stars.insert(7,"2.0");
    stars.insert(8,"1.5");
    stars.insert(9,"1.0");
    stars.insert(10,"0.5");
    stars.insert(11,"0.0");
    comboBox_stars->clear();
    comboBox_stars->addItems(stars.values());
}

void SingerQueryWidget::export_excel_clicked()
{
    ///建立临时表映射
    ///
    QSqlQueryModel *sqlquery = new QSqlQueryModel(this);
    sqlquery->setQuery(query);
    QSqlQuery _query;
    ActorPagingQuery argu;
    getQueryCondition(argu);
    if(!_sql->queryActor_All(argu, _query))
        return;
    sqlquery->setQuery(_query);
    int rows = sqlquery->rowCount();
    int columns = sqlquery->columnCount();
    QString desktopPath = QProcessEnvironment::systemEnvironment().value("USERPROFILE")+"\\Desktop";
    QString fileName = QFileDialog::getSaveFileName(tableView_singerQuery, "保存",
                                                    //QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    desktopPath,
                                                    "Excel 文件(*.xls *.xlsx)");
    QProgressDialog *progress = new QProgressDialog(this);
    progress->setLabelText("正在导出表格数据……");
    progress->setRange(0, rows);
    progress->setModal(true);
    progress->setCancelButtonText("取消");
    progress->setMinimumSize(400, 50);
    progress->setAutoClose(true);
    if (fileName!="")
    {
        QAxObject *excel = new QAxObject;
        if (excel->setControl("Excel.Application")) //连接Excel控件
        {
            excel->dynamicCall("SetVisible (bool Visible)","false");//不显示窗体
            excel->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示
            QAxObject *workbooks = excel->querySubObject("WorkBooks");//获取工作簿集合
            workbooks->dynamicCall("Add");//新建一个工作簿
            QAxObject *workbook = excel->querySubObject("ActiveWorkBook");//获取当前工作簿
            QAxObject *worksheet = workbook->querySubObject("Worksheets(int)", 1);
//            //数据区
            for(int i=0; i<rows; i++)
            {
                for (int j=0;j<columns; j++)
                {
                    QModelIndex index = sqlquery->index(i, j);
                    QString text = index.data().toString();
//                    table->item(i,j)?table->item(i,j)->text():""
                    worksheet->querySubObject("Cells(int,int)", i+1, j+1)->dynamicCall("SetValue(const QString&)", text);
                }
                QString label_text = QString("正在导出%1行……").arg(i+1);
                progress->setLabelText(label_text);
                progress->setValue(i+1);
                if(progress->wasCanceled())
                {
                    break;
                }
            }
            workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(fileName));//保存至fileName
            workbook->dynamicCall("Close()");//关闭工作簿
            excel->dynamicCall("Quit (void)");//关闭excel
            delete excel;
            excel=NULL;

            QMessageBox box(QMessageBox::Question, "完成", "文件已经导出，是否现在打开？");
            box.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
            box.setButtonText(QMessageBox::Yes, "确定(&Q)");
            box.setButtonText(QMessageBox::No, "取消(&C)");
            if(box.exec() == QMessageBox::Yes)
//            if (QMessageBox::question(NULL,"完成","文件已经导出，是否现在打开？",QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
            {
                QString local_path = QString("file:///") + fileName;
                qDebug() << "local_path : " << local_path;
                QDesktopServices::openUrl(QUrl(local_path, QUrl::TolerantMode)); //QDir::toNativeSeparators(fileName)));

            }
        }
        else
        {
            QMessageBox::warning(NULL,"错误","未能创建 Excel 对象，请安装 Microsoft Excel。",QMessageBox::Apply);
        }
    }

//    progress->close();
}

void SingerQueryWidget::combobox_currentIndexChanged(const QString &)
{
    tableView_singerQuery->limitArgu.currentpage = 0;
    tableView_singerQuery->current_page = 0;
    tableView_singerQuery->limitArgu.tagPage = -1;

    if(initFlag)
        setActorValue();
}

void SingerQueryWidget::add_black()
{
    QVector<int> rows;
    tableView_singerQuery->m_checkBoxDelegate->getIconStatus(rows);

    QVector<qint64> _ids;
    QVector<Actor> _actors;

    for(int i=0; i<rows.count(); i++){
        query.seek(rows.at(i));
        _ids.append(query.value("_sid").toLongLong());

        Actor actor;
        actor.name = query.value("_name").toString();
        actor.sid = query.value("_sid").toString();
        actor.serial_id = query.value("_serial_id").toString();
        actor.song_count = query.value("_song_count").toString();

        _actors.append(actor);
    }

    if(!_ids.isEmpty())
    {
        QString content = QString("歌手加入黑名单对应的歌曲会自动加入黑名单。\n 歌手： \n");

        for (int i=0; i<_actors.size(); i++)
        {
            QString str = QString("歌手名：%1，serial_id：%2，歌曲数：%3 \n").arg(_actors.at(i).name)
                                                                    .arg(_actors.at(i).serial_id)
                                                                    .arg(_actors.at(i).song_count);
            content.append(str);
        }


        QMessageBox box(QMessageBox::Warning, "添加黑名单提示", content);
        box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        box.setButtonText(QMessageBox::Ok, "确定");
        box.setButtonText(QMessageBox::Cancel, "取消");

        if(box.exec() == QMessageBox::Ok)
        {

            for(int i=0; i<_actors.size(); i++)
            {
                QVector<qint64> ids;
                QVector<Media> medias;
                setMedia_black(ids, medias, _actors.at(i).name);
                if(ids.size() > 0 && medias.size() > 0)
                {
                    _sql->updateMediaBlackMush_Medias(ids, medias, 1);
                }
//                else
//                    return;
            }


            bool ret =  _sql->updateActorBlackMush_Actors(_ids, _actors, 1);
            if(ret)
            {
                tableView_singerQuery->m_checkBoxDelegate->setIconStatus(rows);
                setActorValue();
            }
        }
    }
}

void SingerQueryWidget::setMedia_black(QVector<qint64> &_ids, QVector<Media> &_medias, const QString &singer)
{
    QSqlQuery _query;
    _sql->queryMediaOfSinger(singer, _query);
    while(_query.next())
    {
        _ids.append(_query.value("_mid").toLongLong());

        Media media;
        media.name = _query.value("_name").toString();
        media.singer = _query.value("_singer").toString();
        media.mid = _query.value("_mid").toString();
        media.serial_id = _query.value("_serial_id").toString();
        media.path = _query.value("_path").toString();
        media.lyric = _query.value("_lyric").toString();
        media.artist_sid_1 = _query.value("_artist_sid_1").toString();
        media.artist_sid_2 = _query.value("_artist_sid_2").toString();
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd"); // hh:mm:ss
        media.update_time = time;
        _medias.append(media);
    }
}


