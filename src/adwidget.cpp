#include "adwidget.h"
#include "pagingtableview.h"
#include "mysqlquery.h"
#include "tablemodel.h"
#include "defistruct.h"
#include "fmaddmodifydialog.h"
#include "enuminfo.h"
#include "songslistaddmodifydialog.h"
#include "publicsongdialog.h"
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QFile>
#include <QPainter>
#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

#include <QListView>
#define  MaxItems 5

AdWidget::AdWidget(QWidget *parent)
    : QWidget(parent)
{
    initWidget();
    readAndSetStyleSheet();
    initWidgetValue();
    setWidgetSize();

    connect(tableView_Ad, &PagingTableView::updateView, this, &AdWidget::setAdValue);    
    connect(combobox_type, SIGNAL(currentIndexChanged(int)), this, SLOT(combobox_IndexChanged(int)));

    connect(tableView_Ad, &PagingTableView::moveUp, this, &AdWidget::moveUp);
    connect(tableView_Ad, &PagingTableView::moveDown, this, &AdWidget::moveDown);
    connect(tableView_Ad, &PagingTableView::currentRow, this, &AdWidget::deleteMusic);
    connect(tableView_Ad, &PagingTableView::play, this, &AdWidget::play);

    connect(pushButton_add, &QPushButton::clicked, this, &AdWidget::addMusic);
    connect(pushButton_publicsong, &QPushButton::clicked, this, &AdWidget::uploadPublicSong);

    tableView_Ad->setMediaColumnWidth();
}

AdWidget::~AdWidget()
{

}

void AdWidget::readAndSetStyleSheet()
{
    QFile qss(":/qss/songquery.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());
    qss.close();

    this->setObjectName("Widget");
    widgetTitle->setObjectName("TopWidget");
    widgetCenter->setObjectName("CenterWidget");
    label_title->setObjectName("TitleLabel");
    pushButton_add->setObjectName("Button");
    pushButton_publicsong->setObjectName("Button");
    combobox_type->setObjectName("Combobox");

    label_type->setObjectName("Label");    
}

void AdWidget::initWidget()
{
    widgetTitle = new QWidget(this);
    widgetCenter = new QWidget(this);
    label_title = new QLabel(widgetTitle);
    label_type = new QLabel(widgetCenter);
    pushButton_add = new QPushButton(widgetTitle);
    pushButton_publicsong = new QPushButton(widgetTitle);
    combobox_type = new QComboBox(widgetCenter);

    QListView *view_type = new QListView();
    combobox_type->setView(view_type);
    combobox_type->setMaxVisibleItems(MaxItems);

    tableView_Ad = new PagingTableView(this);

    title_layout = new QHBoxLayout();
    title_layout->addWidget(label_title);
    title_layout->addStretch();
//    title_layout->addWidget(pushButton_add);
    title_layout->addWidget(pushButton_publicsong);
    title_layout->setContentsMargins(20, 0, 24, 0);
    title_layout->setSpacing(0);

    widgetTitle->setLayout(title_layout);

    type_layout = new QHBoxLayout();
    type_layout->addWidget(label_type);
    type_layout->addWidget(combobox_type);
    type_layout->addStretch();
    type_layout->addWidget(pushButton_add);
    type_layout->setContentsMargins(46, 20, 24, 20);
    type_layout->setSpacing(10);

    widgetCenter->setLayout(type_layout);

    tableView_layout = new QVBoxLayout();
    vLayout = new QVBoxLayout();

    tableView_layout->addWidget(tableView_Ad);
    tableView_layout->setContentsMargins(12, 20, 12, 3);
    tableView_layout->setSpacing(0);

    vLayout->addWidget(widgetTitle);
    vLayout->addWidget(widgetCenter);
    vLayout->addLayout(tableView_layout);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);

    this->setLayout(vLayout);
}

void AdWidget::initWidgetValue()
{
    label_title->setText("广告歌曲");
    label_type->setText("歌曲类型");
    pushButton_add->setText("添加歌曲");
    pushButton_publicsong->setText("上传歌曲资源");

//    QStringList type;
//    type << "宣传歌曲" << "锁屏状态"; // << "开关机歌曲" << "大堂歌曲";
//    combobox_type->addItems(type);
//    combobox_type->setCurrentIndex(0);

    tableView_Ad->setAdDelegate();
}

void AdWidget::setWidgetSize()
{
    widgetTitle->setFixedHeight(60);
    pushButton_add->setFixedWidth(90);
    pushButton_add->setFixedHeight(36);

    pushButton_publicsong->setFixedSize(120, 36);

    combobox_type->setFixedWidth(150);
    combobox_type->setFixedHeight(36);
}

void AdWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void AdWidget::initSql(MysqlQuery *sql)
{
    _sql = sql;

    addPublicSongType();
}

void AdWidget::setAdValue()
{
    query.clear();
    tableView_Ad->tableView->clear();
    QString type = getType();

    if(!type.isEmpty())
    {
        if(!_sql->queryMediaList_ad(type, tableView_Ad->limitArgu,  query)) return;
        tableView_Ad->setTotal_Rows_Pages(query.size());
    }

//    if(!type.isEmpty())
//    {
//        if(type.compare("ad") == 0 || type.compare("pro") == 0)
//        {
//            if(!_sql->queryMediaList_ad(type, tableView_Ad->limitArgu,  query)) return;
//            tableView_Ad->setTotal_Rows_Pages(query.size());
//        }
//        else
//        {
//            ////公播初始值
//            if(!_sql->queryMediaList_ad(type, tableView_Ad->limitArgu,  query)) return;
//            tableView_Ad->setTotal_Rows_Pages(query.size());
//        }
//    }
    tableView_Ad->showUploadMediaListData(query);
}

void AdWidget::combobox_IndexChanged(const int &row)
{
//    if(row == 0 || row == 1 || row == 2)
//    {
//        pushButton_add->setHidden(false);
//        pushButton_publicsong->setHidden(true);
//    }
//    else
//    {
//        pushButton_add->setHidden(true);
//        pushButton_publicsong->setHidden(false);
//    }

    setAdValue();
}

/*
 * 第一个是广告类型ad
 * 第二个是广告类型pro
 * 第三个开始是公播类型
*/
QString AdWidget::getType()
{
    QString str = NULL;
    bool enabled = true;
    if(combobox_type->currentIndex() == 0)
    {
        str = "ad";

    }
    else if(combobox_type->currentIndex() == 1)
    {
        str = "pro";

    }
//    else if(combobox_type->currentIndex() == 2)
//    {
//        str = "";
//        enabled = false;
//    }
//    else if(combobox_type->currentIndex() == 3)
//    {
//        str = "";
//        enabled = false;
//    }
    else
        str = combobox_type->currentText();

    pushButton_add->setEnabled(enabled);
    return str;
}

void AdWidget::getAdValue(const QSqlQuery &_query, MediaList &_list)
{
    _list.type =  getType();   //_query.value("_type").toString();
    _list.index = _query.value("_index").toString();
    _list.mid = _query.value("_mid").toString();
    _list.serial_id = _query.value("_serial_id").toString();
}

void AdWidget::moveUp(const int &row)
{
    if(row  <= 0)
        return;

    query.seek(row);

    MediaList ad, adPre;
    getAdValue(query, ad);

    if(!query.previous())
        return;
    getAdValue(query, adPre);
    QString type = getType();

    if(_sql->updateMediaList_ad(ad, adPre, type)){
       if(_sql->updateMediaList_ad(adPre, ad, type));
       else
           _sql->updateMediaList_ad(ad, adPre, type);
    }

    setAdValue();
}

void AdWidget::moveDown(const int &row)
{
    if(row >= query.size() - 1)
        return;
    query.seek(row);

    MediaList ad, adNext;
    getAdValue(query, ad);
    QString type = getType();

    if(!query.next())
        return;
    getAdValue(query, adNext);

    if(_sql->updateMediaList_ad(ad, adNext, type)){
        if(_sql->updateMediaList_ad(adNext, ad, type));
        else
            _sql->updateMediaList_ad(ad, adNext, type);
    }

    setAdValue();
}

void AdWidget::deleteMusic(const int &row)
{
    query.seek(row);
    MediaList ad;
    getAdValue(query, ad);

    QString type = getType();
    if(type.compare("ad") == 0 || type.compare("pro") == 0)
    {
        _sql->deleteMedialist(ad);
//        if(_sql->deleteMedialist(ad))
//        setAdValue();
    }
    else
    {
        _sql->deletePublicSong(ad, type);
    }

    setAdValue();
}

void AdWidget::addMusic()
{
//    FmAddModifyDialog *ad = new FmAddModifyDialog();
//    connect(ad, SIGNAL(update_view()), this, SLOT(setAdValue()));
//    ad->setTitle("添加歌曲");
//    ad->setType_delegate(widItem_AD);
//    ad->initMediaList();
//    ad->initSql(_sql, query);
//    ad->initType(getType());
////    ad->setAdViewValue(getType());
//    ad->exec();

    SongsListAddModifyDialog *dialog = new SongsListAddModifyDialog();
    connect(dialog, SIGNAL(update_view(QList<Media>)),
            this, SLOT(insertMediaList(QList<Media>)));
    dialog->setTitle("添加歌曲");
    dialog->initSql(_sql, query);
    dialog->initType(getType(), widItem_AD);
    dialog->exec();
}

void AdWidget::insertMediaList(QList<Media> _media)
{
    QList<MediaList> addM;
    QStringList list;
    qint64 maxIndex = -1;

    QString type = getType();
    if(type.compare("ad") == 0 || type.compare("pro") == 0)
    {

        _sql->getMaxMediaListIndex(maxIndex);
        if(maxIndex == -1)
            return;

        for(int i=0; i<_media.size(); i++){
            maxIndex++;
            MediaList media;
            QString tempStr;
            tempStr.append(QString("'%1'").arg(getType()));
            tempStr.append(", ");
            tempStr.append(QString::number(maxIndex));
            tempStr.append(", ");
            tempStr.append(_media.at(i).mid);

            list.append(tempStr);

            ///json使用
            media.index = QString::number(maxIndex);
            media.type = getType();
            media.serial_id = _media.at(i).serial_id;

            addM.append(media);
        }

        _sql->addMediaList(getType(), list, addM);
    }else
    {
        _sql->getMaxIndex_publicsong(maxIndex);
        if(maxIndex == -1)
            return;

        for(int i=0; i<_media.size(); i++){
            maxIndex++;
            QString tempStr;
            tempStr.append(QString("'%1'").arg(getType()));
            tempStr.append(", ");
            tempStr.append(QString::number(maxIndex));
            tempStr.append(", ");
            tempStr.append(_media.at(i).mid);

            list.append(tempStr);
        }
        _sql->addMediaList_publicsong(getType(), list);
    }
    setAdValue();
}

void AdWidget::play(const int &row, const int &)
{
    query.seek(row);
    QString path = query.value("_path").toString();
    int track = query.value("_original_track").toInt();
    tableView_Ad->palyVideo(path, track);
}

void AdWidget::addPublicSongType()
{
    int curIndex = combobox_type->currentIndex();

    combobox_type->clear();
    QStringList typeV;
    typeV << "锁屏状态" << "宣传歌曲" << "开关机歌曲";// << "大堂歌曲";
    combobox_type->addItems(typeV);


    QStringList typeP;
    _sql->queryPublicSongIPGroupBy(typeP);
    combobox_type->addItems(typeP);

    /////删除已删除公播类型对应歌曲
    QStringList typeL;
    _sql->queryPublicSongListGroupBy(typeL);
    foreach (QString _type, typeL) {
        if(typeP.indexOf(_type) == -1)
        {
            _sql->deletePublicSongList(_type);
        }
    }

    if(curIndex == -1)
        combobox_type->setCurrentIndex(0);
    else if(curIndex < (combobox_type->count() - 1))
        combobox_type->setCurrentIndex(curIndex);
}


void AdWidget::uploadPublicSong()
{
    PublicSongDialog *dialog = new PublicSongDialog();
    dialog->setTitle("公播歌曲");
    dialog->initSql(_sql);
    dialog->exec();
}
