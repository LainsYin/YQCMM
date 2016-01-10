#ifndef SONGSONLINEWIDGET_H
#define SONGSONLINEWIDGET_H

#include <QMap>
#include <QWidget>
#include <QSqlQuery>
class Media;
class QLabel;
class QLineEdit;
class QPushButton;
class SongInfoWidget;
class QVBoxLayout;
class QHBoxLayout;
class QTableView;
class PagingTableView;
class MysqlQuery;
/*
 * 歌曲上线界面
*/
class SongsOnlineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SongsOnlineWidget(QWidget *parent = 0);
    ~SongsOnlineWidget();

    void readAndSetStyleSheet();
    void initWidget();
    void initWidgetValue();
    void paintEvent(QPaintEvent *);
    void initComboboxValue(MysqlQuery *sql);

    void setMediasValue(QList< Media> _medias);
signals:

public slots:
    void cancelMedia(const int &row);

private slots:
    void preview();
    void upload();
    void save();
    void tempSave(Media &media);
    void upload_server();
    void hideWidget(bool checked = false);
    void uploadsuccess(const qint64 &serial_id);
    void setValue(const QModelIndex & index);

public slots:
    void timeStart(qint64 serial_id = 0);
    void timeOver();
private:
    /*
     * 更新视图
    */
    void updateView(const Media &media);
    QStringList getUpdateValue(const Media &media);
    /*
     * 查询serial_id是否存在
     * mid 要查询的serial_id号
     * return 不存在返回-1 存在返回所在行
    */
    int isExsitOfSerial_id(const qint64 &serial_id);
    /*
     * 查询mid是否存在
     * mid 要查询的mid号
     * return 不存在返回-1 存在返回所在行
    */
    int isExsidOfMid(const qint64 &mid);

    /*
     * 获取最大的serial_id
     * return 返回最大serial_id
    */
    qint64 getMaxMediasSerial_id();
    /*
     * 获取最大的mid
     * return 返回最大的mid
    */
    qint64 getMaxMediaMid();

    void setTab_order();

private:
    QHBoxLayout *previewLayout;
    QHBoxLayout *uploadLayout;
    QHBoxLayout *titleLayout;

    QHBoxLayout *saveLayout;
    QVBoxLayout *centerLayout;
    QVBoxLayout *infoSavelayout;
    QVBoxLayout *tableViewLayout;
    QVBoxLayout *vLayout;

    QWidget *widgetTitle;
    QWidget *widgetCenter;

    SongInfoWidget *widget_info;

    QLabel *label_title;
    QLabel *label_select;

    QLineEdit *lineEdit_upload;
    QPushButton *pushButton_upload;
    QPushButton *pushButton_preview;
    QPushButton *pushButton_save;
    QPushButton *pushButton_uploadServer;
    QPushButton *pushButton_hide;

    PagingTableView *tableView_songsOnline;
    MysqlQuery *_sql;
    QSqlQuery query;
    QMap<int, QString> language, type, effect, resolution, quality;
    QMap<int, QString> source, version, rthym, pitch;
    QList< QStringList > rowList;
    QList< Media> medias;

    qint64 max_serial_id;
    QString retSize;
    QTimer *timer;
    int row;
};


#endif // SONGSONLINEWIDGET_H
