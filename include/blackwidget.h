#ifndef BLACKWIDGET_H
#define BLACKWIDGET_H

#include <QWidget>
#include <QMap>
#include <enuminfo.h>
#include <QSqlQuery>
#include "defistruct.h"
#include "curlupload.h"
class QLabel;
class QCheckBox;
class QTableView;
class QPushButton;
class PagingTableView;
class QVBoxLayout;
class QHBoxLayout;
class TableView;

class MysqlQuery;

/*
 * 黑名单
*/
class BlackWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BlackWidget(QWidget *parent = 0, int itemIndex = 0);
    ~BlackWidget();
    void readAndSetStyleSheet();
    void initWidget();
    void paintEvent(QPaintEvent *);
    void setWidgetValue(int  itemIndex);

    void initSqlQuery(MysqlQuery *sql);
    //初始化歌曲黑名单
    void initBlack_Media();
    //初始化歌星黑名单
    void initBlack_Actor();
    /*
     * 初始化黑名单界面和代理
    */
    void initBlack_Total();
    /*
     *显示查询出来的黑名单数据
    */
    void show_black();
//    void setBlackColumnWidth_Media(TableView *widget);
//    void setBlackColumnWidth_Actor(QTableView *widget);
signals:    

public slots:
    //添加鼠标右键菜单
    void addBlackRightMenu();
    //单个取消黑名单
    void cancelBlack(const int &row);
    //同时取消多个黑名单
    void cancelBlackMush();
    //视频预览
    void play(const int &row, const int &);

private:
    //云更新 信息记录
    bool cancleBlack_J(const QVector<int> &rows);

public:
    PagingTableView *tableView;
private:
    QHBoxLayout *title_layout;
    QVBoxLayout *tableView_layout;
    QVBoxLayout *vLayout;

    QWidget *widgetTitle;
    QLabel *label_title;

    QPushButton *pushButton_cancel;

    int _itemIndex;
    MysqlQuery    *_sql;
    QSqlQuery query;     

    QPushButton *pushButton_checkBox;
public:

    CurlUpload *curlDownlaod;
    QString runPath;

    QMap<int, QString> nation; // 国籍
    QMap<int, QString> sex; // 性别
};

#endif // BLACKWIDGET_H
