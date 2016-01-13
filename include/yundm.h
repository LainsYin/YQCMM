#ifndef YUNDM_H
#define YUNDM_H

#include "dropshadowwidget.h"

#include <QMap>
#include <QProgressBar>
class QFile;
class QLabel;
class QProgressDialog;
class QPushButton;
class PagingTableView;
class QVBoxLayout;
class QHBoxLayout;
class QNetworkReply;
class QNetworkAccessManager;

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


public slots:
    void setSqlValue(QList< QStringList > rowVal, QMap<int, QString> sqlVal);
    void updateInfo();

private slots:
//    void replyFinished();
//    void readyRead();
//    void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);

private:
//    void startRequest(QString url);
    QString downloadFile(const QString &type, const QString &name, const QString &url);
    bool uploadFile(const QString &type, const QString &filePath);

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
    QMap<int, QString> sqlList;

    QNetworkReply *reply;
    QNetworkAccessManager *manager;
    QString savePath;
    QFile *file;
    int type;
    bool requestFinished;
};

#endif // YUNDM_H
