#include "yundm.h"
#include "pagingtableview.h"
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QPainter>
#include <QStyleOption>
#include <QCalendarWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

YunDM::YunDM(QWidget *parent)
    :DropShadowWidget(parent)
{
    setFixedHeight(650);
    setFixedWidth(900);

    initWidget();
    readAndSetStyleSheet();
    initWidgetValue();

    connect(pushbutton_close, &QPushButton::clicked, this, &YunDM::close);
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
}

void YunDM::initWidget()
{
    widget_top = new QWidget(this);
    widget_opt = new QWidget(this);
    widget_list = new QWidget(this);

    label_title = new QLabel(widget_top);
    pushbutton_close = new QPushButton();
    down_list = new PagingTableView(widget_list);

    layout_title = new QHBoxLayout();
    layout_title->addWidget(label_title);
    layout_title->addStretch();
    layout_title->addWidget(pushbutton_close);
    layout_title->setContentsMargins(20, 0, 20, 0);
    layout_title->setSpacing(20);
    widget_top->setLayout(layout_title);

    layout_list = new QHBoxLayout();
    layout_list->addWidget(down_list);
    layout_list->setContentsMargins(0, 0, 0, 0);
    layout_list->setSpacing(0);
    widget_list->setLayout(layout_list);

    vLayout = new QVBoxLayout();
    vLayout->addWidget(widget_top);
    vLayout->addWidget(widget_opt);
    vLayout->addWidget(widget_list);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);

    this->setLayout(vLayout);
    widget_opt->setHidden(true);
    down_list->setYunDownDelegate();
}

void YunDM::initWidgetValue()
{
    label_title->setText("下载管理");
    widget_top->setFixedHeight(33);

    pushbutton_close->setFixedSize(14, 14);
}

void YunDM::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

