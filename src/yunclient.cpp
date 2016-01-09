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
#include <QMessageBox>
#include <QDebug>

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
    lineedit_end->setObjectName("LineEdit");
    lineedit_start->setObjectName("LineEdit");

    pushbutton_query->setObjectName("Button");
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

    pushbutton_query = new QPushButton(widget_top);
    pushbutton_start = new QPushButton(lineedit_start);
    pushbutton_end = new QPushButton(lineedit_end);

    label_cen = new QLabel(widget_top);
    label_progress = new QLabel(widget_bottom);
    label_speed = new QLabel(widget_bottom);
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

    layout_pro = new QHBoxLayout();
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

    lineedit_start->setFixedSize(260, 36);
    lineedit_end->setFixedSize(260, 36);
    pushbutton_query->setFixedSize(90, 36);
    pushbutton_end->setFixedSize(20, 20);
    pushbutton_start->setFixedSize(20, 20);
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

    update_list->setYunDelegate();
}

void YunClient::initWidgetValue()
{
//    update_list->widget_bottom->setHidden(true);
    label_cen->setText("至");
    label_progress->setText("进度:");
    label_speed->setText("速度:");

    pushbutton_query->setText("查询");
}

void YunClient::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void YunClient::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    if ( pos.x() > label_progress->geometry().x()
         && pos.x() < label_speed->geometry().x() + label_speed->geometry().width()
         && pos.y() > widget_bottom->geometry().y()
         && pos.y() < widget_bottom->geometry().y() + widget_bottom->geometry().height()){

        qDebug() << " yun dm ";
        YunDM *dm = new YunDM();
        dm->open();
    }
}

void YunClient::mouseReleaseEvent(QMouseEvent *mouse)
{
//    QPoint mouPo = mouse->pos();
//    QRect rectEnd = calendar_end->geometry();
//    if (mouPo.x() < rectEnd.x()
//        && mouPo.x() > (rectEnd.x() + rectEnd.width()
//        && mouPo.y() < rectEnd.y())
//        && mouPo.y() > (rectEnd.y() + rectEnd.height())
//        && !calendar_end->isHidden())
//        calendar_end->setHidden(true);

//    QRect rectStart = calendar_start->geometry();
//    if (mouPo.x() < rectStart.x()
//        && mouPo.x() > (rectStart.x() + rectStart.width()
//        && mouPo.y() < rectStart.y())
//        && mouPo.y() < (rectStart.y() + rectStart.height())
//        && !calendar_start->isHidden())
//        calendar_start->setHidden(true);
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
    if (lineedit_start->text().isEmpty() || lineedit_end->text().isEmpty()){
        QMessageBox::warning(this, "提示", "必须填写日期");
        return;
    }

    if (QDateTime::fromString(lineedit_start->text(), "yyyy-MM-dd")
            > QDateTime::fromString(lineedit_end->text(), "yyyy-MM-dd")){
        QMessageBox::warning(this, "提示", "开始时间不能大于结束时间");
        return;
    }


}


