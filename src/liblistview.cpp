#include "liblistview.h"
#include "itemdelegate.h"
#include "enuminfo.h"
#include "yqcdelegate.h"
#include "releasetech.h"
#include <QHeaderView>
#include <QFile>
#include <QHBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QListView>
#include <WindowsX.h>
#include <QDebug>
#include <QStandardItemModel>

LibListView::LibListView(QWidget *parent) :
    QWidget(parent),        
    m_menu(NULL)
{
    setFixedWidth(232);
    setMinimumHeight(360);
    QFile qss(":/qss/libitem.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());
    qss.close();

    m_listView = new QListView();
    m_listView->setFocusPolicy(Qt::NoFocus);
    ItemDelegate *delegate = new ItemDelegate(this);  //LeftItemDelegate
    model = new QStandardItemModel(this);

#ifndef YQC_TECH
    toolList = Util::parse(":/xml/Config.xml");
#else
    toolList = Util::parse(":/xml/Config_Tech.xml");
#endif

    setupModelData();

    m_listView->setIconSize(QSize(0, 0));
    m_listView->setMouseTracking(true);
    m_listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_listView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_listView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_listView->setModel(model);    
    m_listView->setItemDelegate(delegate);

    connect(m_listView, &QListView::clicked, this, &LibListView::clickedRow);
//    connect(delegate, SIGNAL(expanded(const QModelIndex &)), this, SLOT(expanded(const QModelIndex &)));

    QHBoxLayout *centerLayout = new QHBoxLayout();
    centerLayout->addWidget(m_listView);
    centerLayout->setSpacing(0);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(centerLayout);

//    connect(m_listView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(treeViewCustomContextMenuRequested(QPoint)));
    connect(delegate, &ItemDelegate::expanded, this, &LibListView::setHiddenItem);
    m_listView->setFrameShape(QFrame::NoFrame);

    this->setObjectName("LibItemWidget");
    m_listView->setObjectName("LibItemView");

    initHiddenItem();
}

LibListView::~LibListView()
{

}

void LibListView::setupModelData()
{    
    for (int i = 0; i < toolList.count(); ++i)
    {
        ToolUtil tool = toolList.at(i);
        QList<QVariant> columnData;
        columnData << tool.id;

        model->appendRow(new QStandardItem(tool.text));
    }
}

void LibListView::initHiddenItem()
{

    QVector<int> widgetlist;
    widgetlist.append(widItem_songs);
    widgetlist.append(widItem_singer);
    widgetlist.append(widItem_songsOnline);
    widgetlist.append(widItem_singerOnline);

    widgetlist.append(widItem_listNew);
    widgetlist.append(widItem_listChinese);
    widgetlist.append(widItem_listHot);
    widgetlist.append(widItem_listNet);
    widgetlist.append(widItem_listHi);
    widgetlist.append(widItem_listbillboard);

#ifndef YQC_TECH
    widgetlist.append(widItem_blackMusic - 1);
    widgetlist.append(widItem_blackSinger - 1);
#else
    widgetlist.append(widItem_blackMusic);
    widgetlist.append(widItem_blackSinger);
#endif
    widgetlist.append(widItem_songsInfo);
    widgetlist.append(widItem_singerInfo);
    widgetlist.append(widItem_listInfo);

    for(int i=0; i<widgetlist.size(); i++){
        m_listView->setRowHidden(widgetlist.value(i), true);
    }
}

void LibListView::setHiddenItem(int row, bool isexpanded)
{
#ifndef YQC_TECH
        if(row >= 14)
            row++;
#else
#endif

    QVector<int> itemList;
    if (row == widItem_songsLib){
        itemList.clear();
        itemList.append(widItem_songs);
        itemList.append(widItem_singer);
    }else if(row == widItem_songsLibOnline){
        itemList.clear();
        itemList.append(widItem_songsOnline);
        itemList.append(widItem_singerOnline);
    }else if(row == widItem_list){
        itemList.clear();
        itemList.append(widItem_listNew);
        itemList.append(widItem_listChinese);
        itemList.append(widItem_listHot);
        itemList.append(widItem_listNet);
        itemList.append(widItem_listHi);
        itemList.append(widItem_listbillboard);
    }else if(row == widItem_black){
        itemList.clear();

#ifndef YQC_TECH
        itemList.append(widItem_blackMusic - 1);
        itemList.append(widItem_blackSinger - 1);
#else
        itemList.append(widItem_blackMusic);
        itemList.append(widItem_blackSinger);
#endif

    }else if(row == widItem_setInfo){
        itemList.clear();
        itemList.append(widItem_songsInfo);
        itemList.append(widItem_singerInfo);
        itemList.append(widItem_listInfo);
    }

    if( isexpanded){

        for(int i=0; i<itemList.size(); i++){
            m_listView->setRowHidden(itemList.value(i), true);
           //248, 248, 248
        }
        int height = m_listView->height() - (itemList.size()*40);
        if(height <= 360)
            height = 360;
        setFixedHeight(height);
    }else {

        LibListDelegate *delegate = new LibListDelegate(this);
        delegate->setRGB(250, 0, 0);
        for(int i=0; i<itemList.size(); i++){
            m_listView->setRowHidden(itemList.value(i), false);
//            m_listView->setItemDelegateForRow(itemList.value(i), delegate); //235, 235, 235
        }
        int height = m_listView->height() + (itemList.size()*40);
        if(height >= 830)
            height = 760;
        setFixedHeight(height);
    }
}


void LibListView::paintEvent(QPaintEvent *)
{    
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}


//void LibListView::treeViewCustomContextMenuRequested(const QPoint &pos)
//{
//}

void LibListView::clickedRow(const QModelIndex & index)
{
    int currentRow = index.row();
    if (currentRow != -1)
    {
#ifndef YQC_TECH
        if(currentRow >= 14)
            currentRow++;
#else
#endif
        emit currentPage(currentRow);
    }

}
