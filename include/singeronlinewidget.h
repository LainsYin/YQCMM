#ifndef SINGERONLINEWIDGET_H
#define SINGERONLINEWIDGET_H

#include <QMap>
#include <QSqlQuery>
#include <QWidget>
class Actor;
class QLabel;
class QLineEdit;
class QPushButton;
class SingerInfoWdiget;
class QVBoxLayout;
class QHBoxLayout;
class QTableView;
class MysqlQuery;
class PagingTableView;
/*
 * 歌星上线界面
*/
class SingerOnlineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SingerOnlineWidget(QWidget *parent = 0);
    ~SingerOnlineWidget();

    void readAndSetStyleSheet();
    void initWidget();
    void initWidgetValue();
    void paintEvent(QPaintEvent *);

    void initComboboxValue(MysqlQuery *sql);
    void updateView(Actor actor);
    QStringList getUpdateValue(const Actor &media);


signals:

public slots:
     void cancelActor(const int &row);

private slots:

    void upload();
    void preview();
    void save();
    void tempSave(Actor &actor);

    void hideWidget(bool checked = false);
    void upload_server();
    void uploadsuccess(const qint64 &serial_id);
    void setValue(const QModelIndex & index);
    void setValuePos(const int &row);
 private:
    int isExsitOfSerial_id(const qint64 &serial_id);
    int isExsitOfSid(const qint64 &sid);
    qint64 getMaxActorSerial_id();
    qint64 getMaxActorSid();

    void timeStart(qint64 serial_id = 0);
    void timeOver();
private:
    QHBoxLayout *previewLayout;
    QHBoxLayout *uploadLayout;
    QHBoxLayout *titleLayout;

    QHBoxLayout *saveLayout;
    QVBoxLayout *infoSavelayout;
    QVBoxLayout *centerLayout;

    QVBoxLayout *tableViewLayout;
    QVBoxLayout *vLayout;

    QWidget *widgetTitle;
    QWidget *widgetCenter;

    SingerInfoWdiget *widget_info;

    QLabel *label_title;
    QLabel *label_addSinger;

    QLineEdit *lineEdit_upload;
    QPushButton *pushButton_upload;
    QPushButton *pushButton_preview;
    QPushButton *pushButton_save;
    QPushButton *pushButton_uplosdServer;
    QPushButton *pushButton_hide;

    QMap<int, QString> sex, nation;
    PagingTableView *tableView_singerOnline;
    QList< QStringList > rowList;
    QList< Actor> actors;
    MysqlQuery *_sql;
    QSqlQuery query;
    QString runPath;
    QString retSize;
    QTimer *timer;
    int row;
};

#endif // SINGERONLINEWIDGET_H
