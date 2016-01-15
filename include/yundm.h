#ifndef YUNDM_H
#define YUNDM_H

#include "dropshadowwidget.h"

#include <QMap>
#include <QProgressBar>
class QTimer;
class QFile;
class QLabel;
class QProgressDialog;
class QPushButton;
class PagingTableView;
class QVBoxLayout;
class QHBoxLayout;
class QNetworkReply;
class QNetworkAccessManager;
class MysqlQuery;

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
    void initSqlAndVersion(MysqlQuery *sql, const QString &verId, const QString &verName);
    void paintEvent(QPaintEvent *);


public slots:
    void setSqlValue(QList< QStringList > rowVal);
    void updateInfo();
    void updateInfo1();

private slots:
//    void replyFinished();
//    void readyRead();
//    void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);

    void timeOver();
    void updateDialogView(QStringList row);
    void updateDownLoadStatus(QString status);
    void updateStoreStatus();

private:
//    void startRequest(QString url);
    QString downloadFile(const QString &type, const QString &name, const QString &url);
    bool uploadFile(const QString &type, const QString &filePath);

    void timeStart();
    bool execSql(const QStringList &info);
    void writeLogging(const QString &str);
    void setTitleText(const QString &text="下载管理");

private:
    QLabel *label_title;
    QPushButton *pushbutton_close;
    QPushButton *pushbutton_update_select;

    QWidget *widget_top;
    QWidget *widget_opt;
    QWidget *widget_list;

    PagingTableView *down_list;
    QProgressBar *progress;

    QHBoxLayout *layout_title;
    QHBoxLayout *layout_opt;
    QVBoxLayout *layout_list;
    QVBoxLayout *vLayout;

    QList<QStringList> rowList;

    QNetworkReply *reply;
    QNetworkAccessManager *manager;
    QString savePath;
    QFile *file;
    int type;
    bool requestFinished;

    MysqlQuery *_sql;
    QString versionId;
    QString versionName;

    QString retSize;
    QTimer *timer;
};

#endif // YUNDM_H
