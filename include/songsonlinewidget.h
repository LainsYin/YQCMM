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
    void updateView(const Media &media);
    QStringList getUpdateValue(const Media &media);
    int isExsitOfSerial_id(const qint64 &serial_id);
    int isExsidOfMid(const qint64 &mid);

    qint64 getMaxMediasSerial_id();
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
