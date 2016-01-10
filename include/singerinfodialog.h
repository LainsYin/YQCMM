#ifndef SINGERINFODIALOG_H
#define SINGERINFODIALOG_H

#include "dropshadowwidget.h"
#include "singerinfowdiget.h"
#include "defistruct.h"
#include <QWidget>
class QLabel;
class MysqlQuery;
class QHBoxLayout;
class QVBoxLayout;
/*
 * 歌星信息对话框
*/
class SingerInfoDialog : public DropShadowWidget
{
public:
    explicit SingerInfoDialog(QWidget *parent = 0);
    ~SingerInfoDialog();

    void initWidget();
    void initWidgetValue();
    void paintEvent(QPaintEvent *);
    /*
     * 设置标题字符串
     * title 标题字符串
    */
    void setTitleString(const QString &title);
    /*
     * 设置数据库指针
     * sql mysql数据库指针
    */
    void setSqlPointer(MysqlQuery *sql);
    /*
     * 选择样式表类型
     * hidden 是否隐藏
    */
    void selectStyleSheet(bool hidden = false);
    /*
     * 设置歌星值
     * _media 歌星数据结构体
    */
    void setValue(const Actor _media);

public slots:
    void updateActorValue();
    void save();
private:
    QHBoxLayout *hLayout;
    QHBoxLayout *updateLayout;
    QHBoxLayout *saveLayout;
    QHBoxLayout *promptLayout;
    QVBoxLayout *infoLayout;
    QVBoxLayout *wbLayout;
    QVBoxLayout *vLayout;

    QWidget *widgetTitle;
    QWidget *widgetCenter;
    QWidget *widgetUpdate;
    QWidget *widgetSave;
    QLabel *label_title;
    QLabel *label;
    QPushButton *pushButton_close;



    QPushButton *pushButton_Yes;
    QPushButton *pushButton_update;
    QPushButton *pushButton_save;

    QLabel *label_prompt;
    QLabel *label_prompt_tooptip;

    MysqlQuery *_sql;
    qint64   sid;

public:
     SingerInfoWdiget *info;
};

#endif // SINGERINFODIALOG_H
