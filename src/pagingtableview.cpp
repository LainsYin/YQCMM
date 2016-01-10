#include "pagingtableview.h"
#include "yqcdelegate.h"
#include "tablemodel.h"
#include "mysqlquery.h"
#include "player/MP4Player.h"
#include "releasetech.h"
#include "enuminfo.h"
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QValidator>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFile>
#include <QPainter>
#include <QMenu>
#include <QHeaderView>
#include <QTableView>
#include <QPaintEvent>
#include <QSettings>
#include <QSqlQuery>
#include <QCursor>
#include <QDebug>
#include <QAbstractItemView>

#define DEFAULT_MINNUM   5
#define DEFAULT_ROWCOUNT 20  //每页默认行数

#define UPLOAD_TYPE "avatar"

PagingTableView::PagingTableView(QWidget *parent, bool alternate_background_color)
    : QWidget(parent),
    alternate_background_color(alternate_background_color)
{
    initPagingQueryLimitArgu();

    isCheckBox = false;
    showMediaOrActor = false;

    runPath = QCoreApplication::applicationDirPath();
    runPath.append("/platforms/image/" );


    initWidget();
    setTableWidget();
    widgetLayout();
    setWidgetValue();


    connect(m_checkBoxDelegate, &CheckBoxDelegate::success, this, &PagingTableView::update_View);
    connect(this, &PagingTableView::checkBoxIsAll, m_checkBoxDelegate, &CheckBoxDelegate::setAllSelect);
    connect(m_cancelDelegate, SIGNAL(currentRow(int)), this, SIGNAL(currentRow(int)));
    connect(m_playDelegate, SIGNAL(currentRow(int,int)), this, SIGNAL(play(int,int)));
//    connect(m_playDelegate, SIGNAL(currentRow(QString)), this, SLOT(palyVideo(QString)));

    connect(pushButton_upPage, &QPushButton::clicked, this, &PagingTableView::up_page);
    connect(pushButton_nextPage, &QPushButton::clicked, this, &PagingTableView::next_page);
    connect(pushButton_firstPage, &QPushButton::clicked, this, &PagingTableView::first_page);
    connect(pushButton_lastpage, &QPushButton::clicked, this, &PagingTableView::last_page);
    connect(pushButton_jumpPage, &QPushButton::clicked, this, &PagingTableView::jump_page);

    connect(lineEdit_jump, &QLineEdit::editingFinished, this, &PagingTableView::jumpEditFinish);
    connect(pushButton_jumpPage, &QPushButton::clicked, this, &PagingTableView::jumpEditFinish);
    connect(lineEdit_row, &QLineEdit::editingFinished, this, &PagingTableView::rowPageEditFinish);


    m_playDelegate->setPixmapNormal(":/video/images/preview_normal.png");
    m_playDelegate->setPixmapPressed(":/video/images/preview_normal.png"); //preview_pressed
}

PagingTableView::~PagingTableView()
{
    if(curlDownlaod)
    {
        delete curlDownlaod;
        curlDownlaod = NULL;
    }
}


void PagingTableView::initPagingQueryLimitArgu()
{
    limitArgu.tagPage = -1;
    limitArgu.limitOffset = 0;
    limitArgu.curMaxVal = 0;
    limitArgu.curMinVal = 0;
    limitArgu.limitStartVal = 0;
    limitArgu.totalPage = 0;
    limitArgu.currentpage = 0;
    limitArgu.totalRow = 0;
    limitArgu.rowNum = DEFAULT_ROWCOUNT;
}

void PagingTableView::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void PagingTableView::initWidget()
{
    curlDownlaod = new CurlUpload();
    widget_bottom = new QWidget(this);
    tableView = new TableView(this);
    model = new TableModel(this);
    tableView->setModel(model);

    label_totalNum = new QLabel(widget_bottom);
    label_showPage = new QLabel(widget_bottom);
    label_toopTipLeft = new QLabel(widget_bottom);
    label_toopTipRight = new QLabel(widget_bottom);

    lineEdit_jump = new QLineEdit(widget_bottom);
    lineEdit_row = new QLineEdit(widget_bottom);

    pushButton_upPage = new QPushButton(widget_bottom);
    pushButton_nextPage = new QPushButton(widget_bottom);
    pushButton_firstPage = new QPushButton(widget_bottom);
    pushButton_lastpage = new QPushButton(widget_bottom);
    pushButton_jumpPage = new QPushButton(widget_bottom);


    m_cancelDelegate = new ButtonDelegate(this);
    m_playDelegate = new IconDelegate(this, true);
    m_imageDelegate = new ImageDelegate(this);
    m_checkBoxDelegate = new CheckBoxDelegate(this);
    m_cancelDelegate->setButtonText("取消");
    m_moveDelegate = new MoveDelegate(this);
}

void PagingTableView::setTableWidget()
{
    tableView->setAlternatingRowColors(true);

    tableView->setItemDelegate(new NoFocusDelegate());
    tableView->setSelectionMode(QAbstractItemView::NoSelection);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->horizontalHeader()->setHighlightSections(false);
    tableView->verticalHeader()->setVisible(false);
    tableView->setShowGrid(false);
    tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableView->verticalHeader()->setDefaultSectionSize(38);
    QHeaderView *headerView = tableView->horizontalHeader();
    headerView->setStretchLastSection(true);  ////最后一行适应空余部分
//    headerView->setSectionResizeMode(QHeaderView::Stretch); //平均列宽

    tableView->show();
    tableView->setContextMenuPolicy(Qt::CustomContextMenu);   
}

void PagingTableView::widgetLayout()
{
    QHBoxLayout *showPage_layout = new QHBoxLayout();
    showPage_layout->addWidget(label_totalNum);
    showPage_layout->addWidget(label_toopTipLeft);
    showPage_layout->addWidget(lineEdit_row);
    showPage_layout->addWidget(label_toopTipRight);
    showPage_layout->addStretch();
    showPage_layout->setContentsMargins(5, 0, 0, 0);
    showPage_layout->setSpacing(8);

    QHBoxLayout *chanagePage_layout = new QHBoxLayout();
    chanagePage_layout->addStretch();
    chanagePage_layout->addWidget(pushButton_firstPage);
    chanagePage_layout->addWidget(pushButton_upPage);
    chanagePage_layout->addWidget(label_showPage);
    chanagePage_layout->addWidget(pushButton_nextPage);
    chanagePage_layout->addWidget(pushButton_lastpage);
    chanagePage_layout->setContentsMargins(0, 0, 0, 0);
    chanagePage_layout->setSpacing(8);

    QHBoxLayout *jumpPage = new QHBoxLayout();
    jumpPage->addWidget(lineEdit_jump);
    jumpPage->addWidget(pushButton_jumpPage);
    jumpPage->setContentsMargins(0, 0, 0, 0);
    jumpPage->setSpacing(8);

    QHBoxLayout *chanage_jump_layout = new QHBoxLayout();
    chanage_jump_layout->addLayout(chanagePage_layout);
    chanage_jump_layout->addLayout(jumpPage);
    chanage_jump_layout->setContentsMargins(0, 0, 0, 0);
    chanage_jump_layout->setSpacing(20);

    QHBoxLayout *bottom_Layout = new QHBoxLayout();
    bottom_Layout->addLayout(showPage_layout);
    bottom_Layout->addStretch();
    bottom_Layout->addLayout(chanage_jump_layout);
    bottom_Layout->setContentsMargins(0, 0, 0, 0);
    bottom_Layout->setSpacing(0);

    widget_bottom->setLayout(bottom_Layout);

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget(tableView);
    vLayout->addWidget(widget_bottom);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    this->setLayout(vLayout);
}

void PagingTableView::setWidgetValue()
{
    pushButton_upPage->setText("上一页");
    pushButton_nextPage->setText("下一页");
    pushButton_firstPage->setText("首页");
    pushButton_lastpage->setText("末页");
    pushButton_jumpPage->setText("跳转");
    label_showPage->setText(" / ");

    label_toopTipLeft->setText("每页显示");
    label_toopTipRight->setText("条");

    lineEdit_row->setText(QString::number(DEFAULT_ROWCOUNT));
    QRegExp regx("[0-9]+$");
    QValidator *validate = new QRegExpValidator(regx, this);
    lineEdit_jump->setValidator(validate);
    lineEdit_row->setValidator(validate);

    widget_bottom->setFixedHeight(48);
    pushButton_upPage->setFixedHeight(30);
    pushButton_nextPage->setFixedHeight(30);
    pushButton_firstPage->setFixedHeight(30);
    pushButton_lastpage->setFixedHeight(30);
    pushButton_jumpPage->setFixedHeight(30);
    pushButton_upPage->setFixedWidth(60);
    pushButton_nextPage->setFixedWidth(60);
    pushButton_firstPage->setFixedWidth(50);
    pushButton_lastpage->setFixedWidth(50);
    pushButton_jumpPage->setFixedWidth(50);

    label_showPage->setFixedHeight(30);
    label_showPage->setFixedWidth(60);

    lineEdit_jump->setFixedHeight(30);
    lineEdit_jump->setFixedWidth(50);
    lineEdit_row->setFixedHeight(30);
    lineEdit_row->setFixedWidth(50);

    label_showPage->setObjectName("Label");
    label_toopTipLeft->setObjectName("Label");
    label_toopTipRight->setObjectName("Label");
    lineEdit_jump->setObjectName("LineEdit");
    lineEdit_row->setObjectName("LineEdit");
    pushButton_upPage->setObjectName("Button");
    pushButton_nextPage->setObjectName("Button");
    pushButton_firstPage->setObjectName("Button");
    pushButton_lastpage->setObjectName("Button");
    pushButton_jumpPage->setObjectName("Button");

    if(alternate_background_color)
        tableView->setObjectName("LibPagingTable");
    else
        tableView->setObjectName("PagingTable");

    QString strStyle("border: 1px solid rgb(170, 170, 170);border-radius:5px;");
    this->setStyleSheet("border: 0px;   background:transparent;");
    tableView->setStyleSheet("border: 0px;");

    label_showPage->setAlignment(Qt::AlignCenter);
    lineEdit_jump->setAlignment(Qt::AlignCenter);
    lineEdit_row->setAlignment(Qt::AlignCenter);
    label_showPage->setStyleSheet(strStyle);
    lineEdit_row->setStyleSheet(strStyle);
    lineEdit_jump->setStyleSheet(strStyle);
}


void PagingTableView::update_View()
{
    tableView->setFocus();
}

void PagingTableView::palyVideo(const QString &path, const int &track)
{
    QSettings *readConfig = new QSettings("SongsMaintain.conf", QSettings::IniFormat);
    readConfig->setIniCodec("UTF-8");

    QString hostName = readConfig->value("/SERVER/hostname", "http://192.168.1.243").toString();

    QString url(hostName);
    if (path.isEmpty()){
        return;
    }

    url += path;
    MP4Player *player = new MP4Player(this);
    player->playUrlMedia(url, track); //"http://192.168.1.199/08/08/260928.mp4"
    player->show();
}

void PagingTableView::setShowCheckBox()
{
    isCheckBox = true;
}

void PagingTableView::initMediaDelegate(bool isBlack)
{
    if(isCheckBox)
    {
#ifndef YQC_TECH
        tableView->setItemDelegateForColumn(0, m_checkBoxDelegate);
#else
        tableView->setItemDelegateForColumn(1, m_checkBoxDelegate);
#endif
    }
    m_playDelegate->setPixmapNormal(":/video/images/preview_normal.png");
    m_playDelegate->setPixmapPressed(":/video/images/preview_normal.png"); //preview_pressed
    tableView->setItemDelegateForColumn(6, m_playDelegate);
    tableView->setItemDelegateForColumn(7, m_cancelDelegate);

    QStringList headerList;
    headerList << "MID" << "SERIAL_ID" << "歌曲名" << "歌星名" << "语种" << "分类" << "预览";
    if(isBlack){
        headerList << "编辑";
        m_cancelDelegate->setButtonText("取消");
    }
    else{
        headerList << "详情";
        m_cancelDelegate->setButtonText("详情");
    }
    model->setHorizontalHeaderList(headerList);
    model->refrushModel();

#ifndef YQC_TECH
    tableView->setColumnHidden(1, true);
#else
    tableView->setColumnHidden(0, true);
#endif
}

void PagingTableView::initActorDelegate(bool isBlack)
{
    if(isCheckBox)
    {
#ifndef YQC_TECH
        tableView->setItemDelegateForColumn(0, m_checkBoxDelegate);
#else
        tableView->setItemDelegateForColumn(1, m_checkBoxDelegate);
#endif
    }
    m_imageDelegate = new ImageDelegate(this);
    tableView->verticalHeader()->setDefaultSectionSize(70);
    tableView->setItemDelegateForColumn(2, m_imageDelegate);
    tableView->setItemDelegateForColumn(6, m_cancelDelegate);

    QStringList headerList;    
    headerList <<"SID" << "SERIAL_ID" << "头像" << "歌星" << "性别" << "区域";
    if(isBlack){
        headerList << "编辑";
        m_cancelDelegate->setButtonText("取消");
    }
    else{
        headerList << "详情";
        m_cancelDelegate->setButtonText("详情");
    }
    model->setHorizontalHeaderList(headerList);
    model->refrushModel();

#ifndef YQC_TECH
    tableView->setColumnHidden(1, true);
#else
    tableView->setColumnHidden(0, true);
#endif
}

void PagingTableView::setQueryActorDelegate()
{
    if(isCheckBox)
    {
#ifndef YQC_TECH
        tableView->setItemDelegateForColumn(0, m_checkBoxDelegate);
#else
        tableView->setItemDelegateForColumn(1, m_checkBoxDelegate);
#endif
    }


    m_imageDelegate = new ImageDelegate(this);
    ButtonDelegate *m_deleteDelegate = new ButtonDelegate(this);
    m_deleteDelegate->setTextColor(254, 107, 107);
    m_deleteDelegate->setButtonText("删除");
    m_cancelDelegate->setButtonText("详情");
    tableView->verticalHeader()->setDefaultSectionSize(70);
    tableView->setItemDelegateForColumn(2, m_imageDelegate);
    tableView->setItemDelegateForColumn(7, m_cancelDelegate);
    tableView->setItemDelegateForColumn(8, m_deleteDelegate);
    connect(m_deleteDelegate, SIGNAL(currentRow(int)), this, SIGNAL(dele(int)));

    QStringList headerList;
    headerList << "SID" << "SERIAL_ID" << "头像" << "歌星" << "性别" << "区域" << "歌曲数" << "详情" << "";

    model->setHorizontalHeaderList(headerList);
    model->refrushModel();

#ifndef YQC_TECH
    tableView->setColumnHidden(1, true);
#else
    tableView->setColumnHidden(0, true);
#endif
}

void PagingTableView::setQueryMediaDelegate()
{
    IconDelegate *matchDelegate = new IconDelegate(this);
    matchDelegate->setPixmapNormal(":/switch/images/switch_off.png");
    matchDelegate->setPixmapPressed(":/switch/images/switch_on.png");
    matchDelegate->setXY(20, 10);

     ButtonDelegate *m_deleteDelegate = new ButtonDelegate(this);
     m_deleteDelegate->setTextColor(254, 107, 107);
     m_deleteDelegate->setButtonText("删除");

    if(isCheckBox)
    {
#ifndef YQC_TECH
        tableView->setItemDelegateForColumn(0, m_checkBoxDelegate);
#else
        tableView->setItemDelegateForColumn(1, m_checkBoxDelegate);
#endif
    }

    tableView->setItemDelegateForColumn(6, matchDelegate);
    tableView->setItemDelegateForColumn(7, m_playDelegate);
    tableView->setItemDelegateForColumn(8, m_cancelDelegate);
    tableView->setItemDelegateForColumn(9, m_deleteDelegate);
    m_cancelDelegate->setButtonText("详情");
//    connect(matchDelegate, &IconDelegate::currentRow, this, &PagingTableView::matchMusic);
    connect(matchDelegate, SIGNAL(currentRow(int,int)), this, SIGNAL(matchMusic(int,int)));
    connect(m_deleteDelegate, SIGNAL(currentRow(int)), this, SIGNAL(dele(int)));

    QStringList headerList;
    headerList << "MID" << "SERIAL_ID" << "歌曲名" << "歌星名" << "语种" << "分类" << "是否K歌" << "预览" << "详情" << "";

    model->setHorizontalHeaderList(headerList);
    model->refrushModel();

#ifndef YQC_TECH
    tableView->setColumnHidden(1, true);
#else
    tableView->setColumnHidden(0, true);
#endif
}

void PagingTableView::setFmDelegate()
{
    tableView->setItemDelegateForColumn(6, m_playDelegate);

    QStringList headerList;
    headerList  << "MID" << "SERIAL_ID" << "歌曲名" << "歌星名" << "语种" << "分类"  << "预览";

    model->setHorizontalHeaderList(headerList);
    model->refrushModel();

#ifndef YQC_TECH
    tableView->setColumnHidden(1, true);
#else
    tableView->setColumnHidden(0, true);
#endif
}

void PagingTableView::setFmAddMusicDelegate(const QString &add_dele)
{
    tableView->setItemDelegateForColumn(6, m_playDelegate);
    tableView->setItemDelegateForColumn(7, m_cancelDelegate);
    QStringList headerList;
    headerList << "MID" << "SERIAL_ID" << "歌曲名" << "歌星名" << "语种" << "分类"  << "预览" << add_dele;

    m_cancelDelegate->setTextColor(254, 107, 107);
    m_cancelDelegate->setButtonText(add_dele);

    model->setHorizontalHeaderList(headerList);
    model->refrushModel();

#ifndef YQC_TECH
    tableView->setColumnHidden(1, true);
#else
    tableView->setColumnHidden(0, true);
#endif
}

void PagingTableView::setFmShowMusicDelegate()
{
    QStringList headerList;
    headerList << "MID" << "SERIAL_ID" << "歌曲名" << "歌星名" << "语种" << "分类"
               << "预览" << "位置" << "替换" << "删除";


    ButtonDelegate *deleteDelegate = new ButtonDelegate(this);
    deleteDelegate->setTextColor(254, 107, 107);
    deleteDelegate->setButtonText("删除");

    ButtonDelegate *replaceDelegate = new ButtonDelegate(this);
    replaceDelegate->setTextColor(108, 162, 234);
    replaceDelegate->setButtonText("替换");

    tableView->setItemDelegateForColumn(6, m_playDelegate);
    tableView->setItemDelegateForColumn(7, m_moveDelegate);
    tableView->setItemDelegateForColumn(8, replaceDelegate);
    tableView->setItemDelegateForColumn(9, deleteDelegate);

    connect(deleteDelegate, &ButtonDelegate::currentRow, this, &PagingTableView::currentRow);
    connect(m_moveDelegate, &MoveDelegate::left, this, &PagingTableView::moveUp);
    connect(m_moveDelegate, &MoveDelegate::right, this, &PagingTableView::moveDown);
    connect(replaceDelegate, &ButtonDelegate::currentRow, this, &PagingTableView::replace);

    model->setHorizontalHeaderList(headerList);
    model->refrushModel();

#ifndef YQC_TECH
    tableView->setColumnHidden(1, true);
#else
    tableView->setColumnHidden(0, true);
#endif
}

void PagingTableView::setAdDelegate()
{
    QStringList headerList;
    headerList << "MID" << "SERIAL_ID" << "歌曲名" << "歌星名" << "语种" << "分类"
               << "是否切歌" << "预览" << "位置" << "删除";


    ButtonDelegate *deleteDelegate = new ButtonDelegate(this);
    deleteDelegate->setTextColor(254, 107, 107);
    deleteDelegate->setButtonText("删除");

    IconDelegate *skipDelegate = new IconDelegate(this);
    skipDelegate->setPixmapNormal(":/switch/images/switch_off.png");
    skipDelegate->setPixmapPressed(":/switch/images/switch_on.png");
    skipDelegate->setXY(20, 10);

    tableView->setItemDelegateForColumn(6, skipDelegate);
    tableView->setItemDelegateForColumn(7, m_playDelegate);
    tableView->setItemDelegateForColumn(8, m_moveDelegate);
    tableView->setItemDelegateForColumn(9, deleteDelegate);


    connect(deleteDelegate, &ButtonDelegate::currentRow, this, &PagingTableView::currentRow);
    connect(m_moveDelegate, &MoveDelegate::left, this, &PagingTableView::moveUp);
    connect(m_moveDelegate, &MoveDelegate::right, this, &PagingTableView::moveDown);
//    connect(skipDelegate, &IconDelegate::currentRow, this, &PagingTableView::skipMusic);
    connect(skipDelegate, SIGNAL(currentRow(int,int)), this, SIGNAL(skipMusic(int,int)));

    model->setHorizontalHeaderList(headerList);
    model->refrushModel();

#ifndef YQC_TECH
    tableView->setColumnHidden(1, true);
#else
    tableView->setColumnHidden(0, true);
#endif
}

void PagingTableView::setMediaListDelegate()
{
    setFmShowMusicDelegate();
}

void PagingTableView::setInfoDelegate(int rows)
{
    SetInfoDelegate *rowDelegate = new SetInfoDelegate(this);
    rowDelegate->setRowNum(rows);

    tableView->setItemDelegateForColumn(2, rowDelegate);
    tableView->setColumnWidth(0, 200);
    tableView->setColumnWidth(1, 200);
    tableView->setColumnWidth(2, 200);
    QHeaderView *headerView = tableView->horizontalHeader();
    headerView->setStretchLastSection(true);  ////最后一行适应空余部分

    connect(rowDelegate, &SetInfoDelegate::left, this, &PagingTableView::modify);
    connect(rowDelegate, &SetInfoDelegate::right, this, &PagingTableView::dele);
    connect(rowDelegate, &SetInfoDelegate::currentRow, this, &PagingTableView::add);

    QStringList headerList;
    headerList << "编码" << "名称" << "编辑" << "" << "" << "";
    model->setHorizontalHeaderList(headerList);
    model->refrushModel();

    //    headerView->setSectionResizeMode(QHeaderView::Stretch);
}

void PagingTableView::setAddModifyDelegate(const QString &add_dele)
{
    m_cancelDelegate->setTextColor(254, 107, 107);
    m_cancelDelegate->setButtonText(add_dele);
    m_imageDelegate = new ImageDelegate(this);
    tableView->verticalHeader()->setDefaultSectionSize(70);
    tableView->setItemDelegateForColumn(2, m_imageDelegate);
    tableView->setItemDelegateForColumn(6, m_cancelDelegate);
    QStringList headerList;
    headerList << "SID" << "SERIAL_ID" << "头像" << "歌星" << "性别" << "区域" << "编辑";

    model->setHorizontalHeaderList(headerList);
    model->refrushModel();

#ifndef YQC_TECH
    tableView->setColumnHidden(1, true);
#else
    tableView->setColumnHidden(0, true);
#endif
}

void PagingTableView::setActorOnlineDelegate()
{
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
//    tableView->setSelectionBehavior(QAbstractItemView::SelectedClicked);
    m_imageDelegate = new ImageDelegate(this);
    DelReconQueueDelegate *progress = new DelReconQueueDelegate(this);
    tableView->verticalHeader()->setDefaultSectionSize(70);
    tableView->setItemDelegateForColumn(2, m_imageDelegate);
    tableView->setItemDelegateForColumn(6, progress);
    QStringList headerList;
    headerList << "SID" << "SERIAL_ID" << "头像" << "歌星" << "性别" << "区域" << "进度" << "编辑";

    ButtonDelegate *cancel = new ButtonDelegate(this);
    cancel->setTextColor(254, 107, 107);
    cancel->setButtonText("取消");
    tableView->setItemDelegateForColumn(7, cancel);

//    connect(tableView, &TableView::customContextMenuRequested, this, &PagingTableView::returnRow);
    connect(tableView, &TableView::pressed  , this, &PagingTableView::selectRow);
    connect(cancel, &ButtonDelegate::currentRow, this, &PagingTableView::currentRow);

    model->setHorizontalHeaderList(headerList);
    model->refrushModel();

#ifndef YQC_TECH
    tableView->setColumnHidden(1, true);
#else
    tableView->setColumnHidden(0, true);
#endif
}
void PagingTableView::returnRow(const QPoint &pos)
{
//    mapFromGlobal(QCursor::pos());
//    QPoint point = this->mapFromGlobal(QCursor::pos());
//    int row = tableView->rowAt(tableView->mapFromGlobal(point.y()));
    int row = tableView->rowAt(pos.y());
    emit(pointRow(row));
    return;
}


void PagingTableView::setMediaOnlineDelegate()
{

    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    QStringList headerList;
    headerList  << "MID" << "SERIAL_ID" << "歌曲名" << "歌星名" << "语种" << "分类"  << "进度" << "编辑";

    ButtonDelegate *cancel = new ButtonDelegate(this);
    cancel->setTextColor(254, 107, 107);
    cancel->setButtonText("取消");
    DelReconQueueDelegate *progress = new DelReconQueueDelegate(this);
    tableView->setItemDelegateForColumn(7, cancel);
    tableView->setItemDelegateForColumn(6, progress);

    connect(tableView, &TableView::clicked, this, &PagingTableView::selectRow);
    connect(cancel, &ButtonDelegate::currentRow, this, &PagingTableView::currentRow);

    model->setHorizontalHeaderList(headerList);
    model->refrushModel();

#ifndef YQC_TECH
    tableView->setColumnHidden(1, true);
#else
    tableView->setColumnHidden(0, true);
#endif
}

void PagingTableView::setYunDelegate()
{

    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    QStringList headerList;
    headerList  << "序号" << "版本" << "大小" << "版本信息"  << "发布时间" << "更新";

    ButtonDelegate *update = new ButtonDelegate(this);
    update->setTextColor(254, 107, 107);
    update->setButtonText("更新");
//    DelReconQueueDelegate *progress = new DelReconQueueDelegate(this);
//    tableView->setItemDelegateForColumn(6, progress);
    tableView->setItemDelegateForColumn(5, update);

//    connect(tableView, &TableView::clicked, this, &PagingTableView::selectRow);
    connect(update, &ButtonDelegate::currentRow, this, &PagingTableView::currentRow);

    model->setHorizontalHeaderList(headerList);
    model->refrushModel();

//#ifndef YQC_TECH
//    tableView->setColumnHidden(1, true);
//#else
//    tableView->setColumnHidden(0, true);
//#endif
}

void PagingTableView::setYunDownDelegate()
{
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    QStringList headerList;
    headerList  << "MID" << "SERIAL_ID" << "歌曲名" << "歌星名" << "语种" << "分类"  << "进度" << "编辑";

    ButtonDelegate *cancel = new ButtonDelegate(this);
    cancel->setTextColor(254, 107, 107);
    cancel->setButtonText("取消");
    DelReconQueueDelegate *progress = new DelReconQueueDelegate(this);
    tableView->setItemDelegateForColumn(7, cancel);
    tableView->setItemDelegateForColumn(6, progress);

    connect(tableView, &TableView::clicked, this, &PagingTableView::selectRow);
    connect(cancel, &ButtonDelegate::currentRow, this, &PagingTableView::currentRow);

    model->setHorizontalHeaderList(headerList);
    model->refrushModel();

#ifndef YQC_TECH
    tableView->setColumnHidden(1, true);
#else
    tableView->setColumnHidden(0, true);
#endif
}


void PagingTableView::showUploadData(QSqlQuery &query, bool showMediaOrActor)
{
    label_totalNum->setText(QString("共%1 ").arg(limitArgu.totalRow));
    setTotal_Rows_Pages(limitArgu.totalRow);
    label_showPage->setText(QString("%1/%2")
                           .arg(limitArgu.currentpage + 1)
                           .arg(limitArgu.totalPage));

    int row = 0;
    QStringList rowValue;
    if(!rowList.isEmpty())
        rowList.clear();

    int offset = limitArgu.currentpage * limitArgu.rowNum;
//    if(limitArgu.tagPage == paging_upPage){
//        query.last();
//        query.next();
//        while(query.previous()){
//            if(showMediaOrActor)
//                setMediaValue(query, rowValue, row + 1 + offset);
//            else
//                setActorValue(query, runPath, row+1+offset, rowValue);
//            row++;
//            rowList.append(rowValue);
//        }
//        if(query.last())
//            limitArgu.curMaxVal = query.value("_serial_id").toLongLong();
//        if(query.first())
//            limitArgu.curMinVal = query.value("_serial_id").toLongLong();
//    }else{
        while(query.next()){
            if(showMediaOrActor)
                setMediaValue(query, rowValue, row + 1 + offset);
            else
                setActorValue(query, runPath, row+1+offset, rowValue);
            row++;
            rowList.append(rowValue);
        }
//        if(query.first())
//            limitArgu.curMaxVal = query.value("_serial_id").toLongLong();
//        if(query.last())
//            limitArgu.curMinVal = query.value("_serial_id").toLongLong();
//    }

    model->setModalDatas(&rowList);
}

void PagingTableView::showQuerySingerData(QSqlQuery &query)
{
    label_totalNum->setText(QString("共%1 ").arg(limitArgu.totalRow));
    setTotal_Rows_Pages(limitArgu.totalRow);
    label_showPage->setText(QString("%1/%2")
                           .arg(limitArgu.currentpage + 1)
                           .arg(limitArgu.totalPage));

    int row = 0;
    QStringList rowValue;
    if(!rowList.isEmpty())
        rowList.clear();

    int offset = limitArgu.currentpage * limitArgu.rowNum;
//    if(limitArgu.tagPage == paging_upPage){
//        query.last();
//        query.next();
//        while(query.previous()){

//            setActorValue(query, runPath, row+1+offset, rowValue);
//            rowValue.append(query.value("_song_count").toString());
//            row++;
//            rowList.append(rowValue);
//        }
//        if(query.last())
//            limitArgu.curMaxVal = query.value("_serial_id").toLongLong();
//        if(query.first())
//            limitArgu.curMinVal = query.value("_serial_id").toLongLong();
//    }else{
        while(query.next()){

            setActorValue(query, runPath, row+1+offset, rowValue);
            rowValue.append(query.value("_song_count").toString());
            row++;
            rowList.append(rowValue);
        }
//        if(query.first())
//            limitArgu.curMaxVal = query.value("_serial_id").toLongLong();
//        if(query.last())
//            limitArgu.curMinVal = query.value("_serial_id").toLongLong();
//    }

    model->setModalDatas(&rowList);
}

void PagingTableView::showUploadSingerAddModityData(QSqlQuery &query, QList< QString > &paths)
{    
    QStringList rowValue;
    if(!rowList.isEmpty())
        rowList.clear();

    if(!paths.isEmpty())
        paths.clear();

    while(query.next()){

        rowValue.clear();
        rowValue.append(query.value("_sid").toString());
        rowValue.append(query.value("_serial_id").toString());

        QString filePath;
        filePath = runPath + query.value("_name").toString();
        filePath += ".jpg";
        QFile file(filePath);
        if (!file.exists()){
            curlDownlaod->downloadActorImage(query.value("_name").toString(), filePath);
        }

        rowValue.append(filePath);
        rowValue.append(query.value("_name").toString());
        rowValue.append(query.value("_sex").toString());
        rowValue.append(query.value("_nation").toString());

        paths.append(filePath);
        rowList.append(rowValue);
    }

    model->setModalDatas(&rowList);
}

void PagingTableView::showUploadMediaListData(QSqlQuery &query)
{
    label_totalNum->setText(QString("共%1 ").arg(limitArgu.totalRow));
    setTotal_Rows_Pages(limitArgu.totalRow);
    label_showPage->setText(QString("%1/%2")
                           .arg(limitArgu.currentpage + 1)
                           .arg(limitArgu.totalPage));

    int row = 0;
    QStringList rowValue;
    if(!rowList.isEmpty())
        rowList.clear();

    int offset = limitArgu.currentpage * limitArgu.rowNum;
//    if(limitArgu.tagPage == paging_upPage){
//        query.last();
//        query.next();
//        while(query.previous()){

//            setMediaValue(query, rowValue, row + 1 + offset);

//            row++;
//            rowList.append(rowValue);
//        }
//        if(query.last())
//            limitArgu.curMaxVal = query.value("_index").toLongLong();
//        if(query.first())
//            limitArgu.curMinVal = query.value("_index").toLongLong();
//    }else{
        while(query.next()){

            setMediaValue(query, rowValue, row + 1 + offset);

            row++;
            rowList.append(rowValue);
        }
        if(query.first())
            limitArgu.curMaxVal = query.value("_index").toLongLong();
        if(query.last())
            limitArgu.curMinVal = query.value("_index").toLongLong();
//    }

    model->setModalDatas(&rowList);
}

void PagingTableView::showUploadData_blackActor(QSqlQuery &query)
{
    label_totalNum->setText(QString("共%1 ").arg(limitArgu.totalRow));
    setTotal_Rows_Pages(limitArgu.totalRow);
    label_showPage->setText(QString("%1/%2")
                           .arg(limitArgu.currentpage + 1)
                           .arg(limitArgu.totalPage));

    int row = 0;
    QStringList rowValue;
    if(!rowList.isEmpty())
        rowList.clear();

    int offset = limitArgu.currentpage * limitArgu.rowNum;
//    if(limitArgu.tagPage == paging_upPage){
//        query.last();
//        query.next();
//        while(query.previous()){

//                setActorValue_blackActor(query, runPath, row+1+offset, rowValue);
//            row++;
//            rowList.append(rowValue);
//        }
//        if(query.last())
//            limitArgu.curMaxVal = query.value("_serial_id").toLongLong();
//        if(query.first())
//            limitArgu.curMinVal = query.value("_serial_id").toLongLong();
//    }else{
        while(query.next()){

                setActorValue_blackActor(query, runPath, row+1+offset, rowValue);
            row++;
            rowList.append(rowValue);
        }
//        if(query.first())
//            limitArgu.curMaxVal = query.value("_serial_id").toLongLong();
//        if(query.last())
//            limitArgu.curMinVal = query.value("_serial_id").toLongLong();
//    }

    model->setModalDatas(&rowList);
}

void PagingTableView::setMediaValue(const QSqlQuery &query, QStringList &rowValue, int )
{
    rowValue.clear();
    rowValue.append(query.value("_mid").toString());
    rowValue.append(query.value("_serial_id").toString());
    rowValue.append(query.value("_name").toString());
    rowValue.append(query.value("_singer").toString());
    rowValue.append(query.value("_language").toString());
    rowValue.append(query.value("_type").toString());
    rowValue.append(query.value("_match").toString());

//    qDebug() << "mid : " << query.value("_mid").toString()  << " name : " << query.value("_name").toString();
}

void PagingTableView::setActorValue(const QSqlQuery &query, const QString &runPath, int , QStringList &rowValue)
{
    rowValue.clear();
    rowValue.append(query.value("_sid").toString());
    rowValue.append(query.value("_serial_id").toString());

    QString filePath;
    filePath = runPath + query.value("_name").toString();
    filePath += ".jpg";
    QFile file(filePath);
    if (!file.exists()){
        curlDownlaod->downloadActorImage(query.value("_name").toString(), filePath);
    }

    rowValue.append(filePath);
    rowValue.append(query.value("_name").toString());
    rowValue.append(sex.value(query.value("_sex").toInt()) );
    rowValue.append(nation.value(query.value("_nation").toInt()));
}

void PagingTableView::setActorValue_blackActor(const QSqlQuery &query, const QString &runPath, int, QStringList &rowValue)
{
    rowValue.clear();
    rowValue.append(query.value("_sid").toString());
    rowValue.append(query.value("_serial_id").toString());

    QString filePath;
    filePath = runPath + query.value("_name").toString();
    filePath += ".jpg";
    QFile file(filePath);
    if (!file.exists()){
        curlDownlaod->downloadActorImage(query.value("_name").toString(), filePath);
    }

    rowValue.append(filePath);
    rowValue.append(query.value("_name").toString());
    rowValue.append(query.value("_sex").toString());
    rowValue.append(query.value("_nation").toString());
}

void PagingTableView::setWidgetBottomHidden(const bool &hidden)
{
    widget_bottom->setHidden(hidden);
}

void PagingTableView::setModelValue(QList<QStringList> &data)
{

    model->setModalDatas(&data);
}

void PagingTableView::getValue(QList<QStringList> &rows)
{
    rows = rowList;
}

int PagingTableView::rowCount()
{
    return rowList.count();
}

void PagingTableView::setSexNation(const QMap<int, QString> &_nation, const QMap<int, QString> &_sex)
{
    if(!nation.isEmpty())
        nation.clear();
    nation = _nation;

    if(!sex.isEmpty())
        sex.clear();
    sex = _sex;
}

void PagingTableView::setMediaColumnWidth(const int &width)
{
    int columnWidth = width/9;

    tableView->setColumnWidth(1, columnWidth*1.2);
    tableView->setColumnWidth(2, columnWidth*1.5);
    tableView->setColumnWidth(3, columnWidth*1.5);
    tableView->setColumnWidth(4, columnWidth);
    tableView->setColumnWidth(5, columnWidth);
    tableView->setColumnWidth(6, columnWidth*0.7);
    tableView->setColumnWidth(7, columnWidth*0.7);
    tableView->setColumnWidth(8, columnWidth*0.7);
    tableView->setColumnWidth(9, columnWidth*0.7);
}

void PagingTableView::setActorColumnWidth(const int &columns, const int &width)
{
    int columnWidth = width/columns;

    for (int i=0;  i<columns; i++) {
        tableView->setColumnWidth(i+1, columnWidth);
    }
}

void PagingTableView::setMedia_black_CW(const int &width)
{
    int columnWidth = width/7;

    tableView->setColumnWidth(1, columnWidth);
    tableView->setColumnWidth(2, columnWidth*1.5);
    tableView->setColumnWidth(3, columnWidth);
    tableView->setColumnWidth(4, columnWidth);
    tableView->setColumnWidth(5, columnWidth);
    tableView->setColumnWidth(6, columnWidth*0.75);
    tableView->setColumnWidth(7, columnWidth*0.75);
}

void PagingTableView::setActor_black_CW(const int &columns, const int &width)
{
    int columnWidth = width/columns;

    for (int i=0;  i<columns; i++) {
        tableView->setColumnWidth(i, columnWidth*1);
    }
//        tableView->setColumnWidth(1, columnWidth);
//        tableView->setColumnWidth(2, columnWidth);
//        tableView->setColumnWidth(3, columnWidth);
//        tableView->setColumnWidth(4, columnWidth*0.75);
    //        tableView->setColumnWidth(5, columnWidth*0.75);
}

void PagingTableView::setSectionResizeMode()
{
    QHeaderView *headerView = tableView->horizontalHeader();
    headerView->setSectionResizeMode(QHeaderView::Stretch); //平均列宽
}

void PagingTableView::setCurrentRowNum(const int &num)
{
    lineEdit_row->setText(QString::number(num));
}

void PagingTableView::up_page()
{
    if(limitArgu.currentpage <= 0)
        return;

    limitArgu.currentpage--;
    limitArgu.tagPage = paging_upPage;
    limitArgu.limitStartVal = limitArgu.curMaxVal;
    limitArgu.limitOffset = 0;

    show_page();    
}

void PagingTableView::next_page()
{    
    if(limitArgu.currentpage >= limitArgu.totalPage - 1)
        return;

    limitArgu.currentpage++;
    limitArgu.tagPage = paging_nextPage;
    limitArgu.limitStartVal = limitArgu.curMinVal;
    limitArgu.limitOffset = 0;

    show_page();
}

void PagingTableView::first_page()
{
    limitArgu.tagPage = paging_upPage;
    int tempNum = limitArgu.currentpage - 1;
    int offset = tempNum * limitArgu.rowNum;
    limitArgu.limitStartVal = limitArgu.curMaxVal;
    limitArgu.limitOffset = offset;
    limitArgu.currentpage = 0;
    show_page();
}

void PagingTableView::last_page()
{
    int tempNum = limitArgu.totalPage - 1 - limitArgu.currentpage - 1;
    int offset = (tempNum)*limitArgu.rowNum;
    limitArgu.tagPage = paging_nextPage;
    limitArgu.currentpage = limitArgu.totalPage - 1;
    limitArgu.limitStartVal = limitArgu.curMinVal;
//    limitArgu.limitStartVal = 0;
    limitArgu.limitOffset = offset;

    show_page();
}

void PagingTableView::jump_page(const int page)
{
    int tempCurrentPage = limitArgu.currentpage;
    limitArgu.currentpage = page;
    if(page > tempCurrentPage){
        limitArgu.tagPage = paging_nextPage;
        limitArgu.limitStartVal = limitArgu.curMinVal;
//        qDebug() << " minvalue : " << limitArgu.curMinVal;
        limitArgu.limitOffset = (page - 1 - tempCurrentPage)*limitArgu.rowNum;
    }else if(page < tempCurrentPage){
        limitArgu.tagPage = paging_upPage;
        limitArgu.limitStartVal = limitArgu.curMaxVal;
//        qDebug() << " maxvalue : " << limitArgu.curMaxVal;
        limitArgu.limitOffset = (tempCurrentPage - page - 1)*limitArgu.rowNum;
    }

    show_page();
}

 void PagingTableView::show_page()
 {         
     emit updateView(limitArgu);     
 }


void PagingTableView::rowPageEditFinish()
{
    int num = lineEdit_row->text().toInt();
    if (num < DEFAULT_MINNUM)
       return;

    setTotal_Rows_Pages(limitArgu.totalRow);
    limitArgu.rowNum = num;
    limitArgu.currentpage = 0;
    show_page();
}

void PagingTableView::jumpEditFinish()
{
    if(lineEdit_jump->text().toInt() < 1
        || lineEdit_jump->text().toInt() > limitArgu.totalPage)
        return;
    jump_page(lineEdit_jump->text().toInt() -1);
}

int PagingTableView::setValidRowsCount()
{
    int rowsCount =  limitArgu.rowNum;
    if (limitArgu.totalPage == 0)
        rowsCount = 0;
    if (limitArgu.currentpage + 1 == limitArgu.totalPage){
       rowsCount = limitArgu.totalRow - limitArgu.rowNum * limitArgu.currentpage;
    }

    return rowsCount;
}

void PagingTableView::setTotal_Rows_Pages(const int rowsSize)
{
    limitArgu.totalRow = rowsSize;
    limitArgu.rowNum = lineEdit_row->text().toInt();
    if (limitArgu.rowNum <= 0){
        limitArgu.rowNum = DEFAULT_ROWCOUNT;
        limitArgu.totalPage  = 1;
    }

    limitArgu.totalPage = limitArgu.totalRow/limitArgu.rowNum + (bool)(limitArgu.totalRow%limitArgu.rowNum);
}
