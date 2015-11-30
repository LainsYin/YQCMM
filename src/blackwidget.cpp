#include "blackwidget.h"
#include "pagingtableview.h"
#include "tablemodel.h"
#include "mysqlquery.h"
#include "yqcdelegate.h"
#include "dropshadowwidget.h"
#include <QLabel>
#include <QList>
#include <QFile>
#include <QPainter>
#include <QCheckBox>
#include <QPushButton>
#include <QTableView>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#define DEFAULT_ROWCOUNT 20  //每页默认行数

BlackWidget::BlackWidget(QWidget *parent, int itemIndex)
    : QWidget(parent),
    _itemIndex(itemIndex)
{
    initWidget();
    readAndSetStyleSheet();

    setWidgetValue(_itemIndex);

}

BlackWidget::~BlackWidget()
{
    if(curlDownlaod){
        delete curlDownlaod;
        curlDownlaod = NULL;
    }
}

void BlackWidget::readAndSetStyleSheet()
{
    QFile qss(":/qss/songquery.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());
    qss.close();

    this->setObjectName("Widget");
    widgetTitle->setObjectName("TopWidget");
    label_title->setObjectName("TitleLabel");
    pushButton_cancel->setObjectName("Button");
    pushButton_checkBox->setObjectName("CheckBoxButton");
}

void BlackWidget::initWidget()
{
    curlDownlaod = new CurlUpload();
    runPath = QCoreApplication::applicationDirPath();
    runPath.append("/platforms/image/" );

    widgetTitle = new QWidget(this);
    label_title = new QLabel(widgetTitle);
    pushButton_cancel = new QPushButton(widgetTitle);
    tableView = new PagingTableView(this);    
    pushButton_checkBox = new QPushButton(tableView);
    pushButton_checkBox->setCheckable(true);

    connect(tableView, &PagingTableView::updateView, this, &BlackWidget::show_black);
    connect(pushButton_checkBox, &QPushButton::clicked, tableView, &PagingTableView::checkBoxIsAll);
    connect(pushButton_cancel, &QPushButton::clicked, this, &BlackWidget::cancelBlackMush);
    connect(tableView, &PagingTableView::currentRow, this, &BlackWidget::cancelBlack);

    connect(tableView, &PagingTableView::play, this, &BlackWidget::play);

    QHeaderView *headerView = tableView->tableView->horizontalHeader();
    QRect rect = headerView->geometry();
    QRect checkBoxRect = QRect(rect.x()+8, rect.y()+12, 16, 16);
    pushButton_checkBox->setGeometry(checkBoxRect);

    widgetTitle->setFixedHeight(60);
    pushButton_cancel->setFixedWidth(90);
    pushButton_cancel->setFixedHeight(36);
    pushButton_cancel->setText("批量取消");
    widgetTitle->setStyleSheet("border-bottom: 1px solid rgb(201, 201, 201);");

    title_layout = new QHBoxLayout();
    title_layout->addWidget(label_title);
    title_layout->addStretch();
    title_layout->addWidget(pushButton_cancel);
    title_layout->setContentsMargins(20, 0, 24, 0);
    title_layout->setSpacing(0);

    widgetTitle->setLayout(title_layout);

    tableView_layout = new QVBoxLayout();
    vLayout = new QVBoxLayout();

    tableView_layout->addWidget(tableView);
    tableView_layout->setContentsMargins(12, 0, 12, 0);
    tableView_layout->setSpacing(0);

    vLayout->addWidget(widgetTitle);
    vLayout->addLayout(tableView_layout);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);

    this->setLayout(vLayout);
}

void BlackWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void BlackWidget::setWidgetValue(int  itemIndex)
{
    if (itemIndex == stacked_mediablack)
        label_title->setText("歌曲黑名单");
    else if (itemIndex == stacked_actorblack)
        label_title->setText("歌星黑名单");
}

void BlackWidget::initSqlQuery(MysqlQuery *sql)
{
    _sql = sql;

    if(_sql)
    {
        if(!sex.isEmpty())
            sex.clear();

        if(!nation.isEmpty())
            nation.clear();

        _sql->getSex(sex);
        _sql->getNation(nation);
    }
}

void BlackWidget::initBlack_Media()
{
    tableView->setShowCheckBox();
    tableView->initMediaDelegate();

    tableView->setMedia_black_CW();
}

void BlackWidget::initBlack_Actor()
{
    tableView->setShowCheckBox();
    tableView->initActorDelegate();

    tableView->setActor_black_CW();
}

void BlackWidget::initBlack_Total()
{
    qint64 total=0;
    if(_itemIndex == stacked_mediablack){
        if(!_sql->queryCountBlack_M(total)) return;
    }
    if(_itemIndex == stacked_actorblack){
        if(!_sql->queryCountBlack_A(total)) return;
    }

    tableView->setTotal_Rows_Pages(total);
}

void BlackWidget::show_black()
{    
    if (_itemIndex != stacked_mediablack && _itemIndex != stacked_actorblack)
        return;

    initBlack_Total();
    query.clear();
    tableView->tableView->clear();    

    if(_itemIndex == stacked_mediablack){
        if(_sql->queryMediaBlack(tableView->limitArgu, query))
            tableView->showUploadData(query, true);

    }else if(_itemIndex == stacked_actorblack){
        if(_sql->queryActorBlack(tableView->limitArgu, query))
        {
            tableView->setSexNation(nation, sex);
//            tableView->showUploadData(query, false);
            tableView->showUploadData_blackActor(query);
            tableView->setActor_black_CW();
        }

    }else
        return;
}


void BlackWidget::addBlackRightMenu()
{

}

void BlackWidget::cancelBlack(const int &row)
{
//    query.seek(row);
//    qint64 _id = -1;
//    QVector<qint64> _mids;
//    if(_itemIndex == stacked_mediablack){
//        _id = query.value("_mid").toLongLong();
//        if(!_sql->updateMediaBlack(_id, 0)) return;
//    }
//    if(_itemIndex == stacked_actorblack){
//        _id = query.value("_sid").toLongLong();
//        _sql->getMidsOfSid(_id, _mids);
//        if (_sql->updateMediaBlackMush(_mids, 0))
//            _sql->updateActorBlack(_id, 0);
//    }
//    show_black();

    QVector<int> rows;
    rows.append(row);
    bool ret = cancleBlack_J(rows);
    if(ret){
        tableView->m_checkBoxDelegate->setIconStatus(rows);

        show_black();
    }
}

void BlackWidget::cancelBlackMush()
{
    QVector<int> rows;
    tableView->m_checkBoxDelegate->getIconStatus(rows);

    if(rows.size() <= 0)
        return;

    bool ret = cancleBlack_J(rows);
    if(ret){
        tableView->m_checkBoxDelegate->setIconStatus(rows);

        show_black();
    }
}

void BlackWidget::play(const int &row, const int &)
{
    query.seek(row);
    QString path = query.value("_path").toString();
    int track = query.value("_original_track").toInt();
    tableView->palyVideo(path, track);
}

bool BlackWidget::cancleBlack_J(const QVector<int> &rows)
{
    QVector<qint64> _ids;
    QVector<qint64> _sids;
    QVector<Media> _medias;
    QVector<Actor> _actors;
    if(_itemIndex == stacked_mediablack){
        for(int i=0; i<rows.count(); i++){
            query.seek(rows.at(i));
            _ids.append(query.value("_mid").toLongLong());

            Media media;
            media.name = query.value("_name").toString();
            media.singer = query.value("_singer").toString();
            media.mid = query.value("_mid").toString();
            media.serial_id = query.value("_serial_id").toString();
            media.path = query.value("_path").toString();
            media.lyric = query.value("_lyric").toString();
            media.artist_sid_1 = query.value("_artist_sid_1").toString();
            media.artist_sid_2 = query.value("_artist_sid_2").toString();
            QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd"); // hh:mm:ss
            media.update_time = time;
            _medias.append(media);
        }

        if(!_ids.isEmpty())
        {
            QString content = QString("歌曲： \n");
            for (int i=0; i<_medias.size(); i++)
            {
                QString str = QString("歌名：%1，歌手：%2 \n").arg(_medias.at(i).name)
                                                                        .arg(_medias.at(i).singer);
                content.append(str);
            }

            QMessageBox box(QMessageBox::Warning, "取消黑名单提示", content);
            box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            box.setButtonText(QMessageBox::Ok, "确定");
            box.setButtonText(QMessageBox::Cancel, "取消");

            if(box.exec() == QMessageBox::Ok)
            {
                _sql->updateMediaBlackMush_Medias(_ids, _medias, 0);
                return true;
            }
//            _sql->updateMediaBlackMush(_ids, 0);
        }
    }

    if(_itemIndex == stacked_actorblack){
        for(int i=0; i<rows.count(); i++){
            query.seek(rows.at(i));
            _sids.append(query.value("_sid").toLongLong());

            Actor actor;
            actor.name = query.value("_name").toString();
            actor.sid = query.value("_sid").toString();
            actor.serial_id = query.value("_serial_id").toString();
            actor.song_count = query.value("_song_count").toString();

            _actors.append(actor);
        }

        if(!_sids.isEmpty())
        {
            QString content = QString("歌手取消黑名单对应的歌曲会自动取消黑名单。\n 歌手： \n");
            for (int i=0; i<_actors.size(); i++)
            {
                QString str = QString("歌手名：%1，歌曲数：%2 \n").arg(_actors.at(i).name)
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
//                    setMedia_black(ids, medias, _actors.at(i).name);

                    QSqlQuery _query;
                    _sql->queryMediaOfSinger(_actors.at(i).name, _query);
                    while(_query.next())
                    {
                        ids.append(_query.value("_mid").toLongLong());

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
                        medias.append(media);
                    }

                    if(ids.size() > 0 && medias.size() > 0)
                    {
                        _sql->updateMediaBlackMush_Medias(ids, medias, 0);
                    }

                }


                _sql->updateActorBlackMush_Actors(_sids, _actors, 0);
                return true;

            }
        }

//        for(int i=0; i<_sids.size(); i++){
//            _mids.clear();
//            _sql->getMidsOfSid(_ids.at(i), _mids);
//            if (_sql->updateMediaBlackMush(_mids, 0))
//                _sql->updateActorBlack(_sids.at(i), 0);
//        }
    }
}



//void BlackWidget::setBlackColumnWidth_Media(TableView *widget)
//{
//    int width = widget->width();
//    int total = 8;
//    widget->setColumnWidth(0, (width/total));
//    widget->setColumnWidth(1, (width/total));
//    widget->setColumnWidth(2, (width/total));
//    widget->setColumnWidth(3, (width/total));
//    widget->setColumnWidth(4, (width/total));
//    widget->setColumnWidth(5, (width/total));
//    widget->setColumnWidth(6, (width/total));
//    widget->setColumnWidth(7, (width/total));
//}

//void BlackWidget::setBlackColumnWidth_Actor(QTableView *widget)
//{
//    int width = widget->width();
//    int total = 16;
//    widget->setColumnWidth(actBlack_check, (width/total)*1);
//    widget->setColumnWidth(actBlack_image, (width/total)*3);
//    widget->setColumnWidth(actBlack_name, (width/total)*3);
//    widget->setColumnWidth(actBlack_sex, (width/total)*3);
//    widget->setColumnWidth(actBlack_nation, (width/total)*3);
//    widget->setColumnWidth(actBlack_edit, (width/total)*3);
//}

//void BlackWidget::setMediaValue(const QSqlQuery &query, QStringList &rowValue, int index)
//{
//    rowValue.clear();
//    rowValue.append(QString::number(index));
//    rowValue.append(query.value("_name").toString());
//    rowValue.append(query.value("_singer").toString());
//    rowValue.append(query.value("_language").toString());
//    rowValue.append(query.value("_type").toString());
//    QString track = QString("原%1 伴%2")
//                    .arg(query.value("_original_track").toString())
//                    .arg(query.value("_sound_track").toString());
//    rowValue.append(track);
//}

//void BlackWidget::setActorValue(const QSqlQuery &query, const QString &runPath, int index, QStringList &rowValue)
//{
//    rowValue.clear();
//    rowValue.append(QString::number(index));

//    QString filePath;
//    filePath = runPath + query.value("_name").toString();
//    filePath += ".jpg";
//    QFile file(filePath);
//    if (!file.exists()){
//        curlDownlaod->downloadActorImage(query.value("_name").toString(), filePath);
//    }

//    rowValue.append(filePath);
//    rowValue.append(query.value("_name").toString());
//    rowValue.append(query.value("_sex").toString());
//    rowValue.append(query.value("_nation").toString());
//}

