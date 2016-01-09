#ifndef ADWIDGET_H
#define ADWIDGET_H

#include <QWidget>
#include <QSqlQuery>
#include "defistruct.h"
class QLabel;
class QComboBox;
class MediaList;
class MysqlQuery;
class QTableView;
class QPushButton;
class PagingTableView;
class QVBoxLayout;
class QHBoxLayout;
class AdWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdWidget(QWidget *parent = 0);
    ~AdWidget();

    void readAndSetStyleSheet();
    void initWidget();
    void initWidgetValue();
    void setWidgetSize();
    void paintEvent(QPaintEvent *);

    /*
     * 初始化数据指针
    */
    void initSql(MysqlQuery *sql);
    /*
     * 获取广告类型和公播类型
    */
    QString getType();
    void getAdValue(const QSqlQuery &_query, MediaList &_list);

signals:

public slots:
    /*
     * 根据广告类型获取歌曲数据
    */
    void setAdValue();
    /*
     * combobox值改变调用
    */
    void combobox_IndexChanged(const int &row);

    void moveUp(const int &row);
    void moveDown(const int &row);
    void deleteMusic(const int &row);
    void addMusic();

    /*
     * 添加公播歌曲路口函数
    */
    void insertMediaList(QList<Media> _media);
    /*
     * 视频预览
    */
    void play(const int &row, const int &);

    /*
     * 上传公播歌曲
    */
    void uploadPublicSong();

private:
    /*
     * 添加公播类型 初始化combobox
    */
    void addPublicSongType();

private:
    QHBoxLayout *title_layout;
    QHBoxLayout *type_layout;

    QVBoxLayout *tableView_layout;
    QVBoxLayout *vLayout;

    QWidget *widgetTitle;
    QWidget *widgetCenter;
    QLabel *label_title;
    QLabel *label_type;
    QPushButton *pushButton_add;
    QPushButton *pushButton_publicsong;
    QComboBox *combobox_type;

    PagingTableView *tableView_Ad;
    MysqlQuery *_sql;
    QSqlQuery query;
};

#endif // ADWIDGET_H
