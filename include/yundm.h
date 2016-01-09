#ifndef YUNDM_H
#define YUNDM_H

#include "dropshadowwidget.h"

class QLabel;
class QPushButton;
class PagingTableView;
class QVBoxLayout;
class QHBoxLayout;

/*
 * 云更新对话框
*/
class YunDM : public DropShadowWidget
{
public:
    YunDM(QWidget *parent = 0);
    ~YunDM();

    void readAndSetStyleSheet();
    void initWidget();
    void initWidgetValue();
    void paintEvent(QPaintEvent *);


private:
    QLabel *label_title;
    QPushButton *pushbutton_close;

    QWidget *widget_top;
    QWidget *widget_opt;
    QWidget *widget_list;

    PagingTableView *down_list;

    QHBoxLayout *layout_title;
    QHBoxLayout *layout_list;
    QVBoxLayout *vLayout;
};

#endif // YUNDM_H
