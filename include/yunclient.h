#ifndef YUNCLIENT_H
#define YUNCLIENT_H

#include <QWidget>
class QPushButton;
class QLabel;
class QLineEdit;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class PagingTableView;
class QCalendarWidget;

/*
 * 云更新主界面
*/
class YunClient : public QWidget
{
    Q_OBJECT
public:
    explicit YunClient(QWidget *parent = 0);
    ~YunClient();

    void readAndSetStyleSheet();
    void initWidget();
    void initWidgetValue();
    void paintEvent(QPaintEvent *);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *mouse);

signals:

private slots:
    void setCalendarStatue_s(bool);
    void setCalendarStatue_e(bool);
    void updateCaleSta_s();
    void updateCaleSta_e();

    void getUpdateVersionList();

private:
    QWidget *widget_top;
    QWidget *widget_center;
    QWidget *widget_bottom;

    QLineEdit *lineedit_start;
    QLineEdit *lineedit_end;

    QPushButton *pushbutton_query;
    QPushButton *pushbutton_start;
    QPushButton *pushbutton_end;

    QLabel *label_cen;
    QLabel *label_progress;
    QLabel *label_speed;
    PagingTableView *update_list;
    QCalendarWidget *calendar_start;
    QCalendarWidget *calendar_end;

    QHBoxLayout *layout_select;
    QHBoxLayout *layout_top;
    QHBoxLayout *layout_list;
    QHBoxLayout *layout_pro;
    QVBoxLayout *vLayout;

};

#endif // YUNCLIENT_H
