#ifndef YUNCLIENT_H
#define YUNCLIENT_H

#include <QMap>
#include <QWidget>
#include <QJsonObject>
class QPushButton;
class QLabel;
class QLineEdit;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class PagingTableView;
class QCalendarWidget;
class QNetworkReply;
class QNetworkAccessManager;

/*
 * 云更新主界面
*/

enum REPLY_READ{
    get_version_list = 1,
    get_media_list = 2,
};
class YunClient : public QWidget
{
    Q_OBJECT
public:
    explicit YunClient(QWidget *parent = 0);
    ~YunClient();

    void readAndSetStyleSheet();
    void initWidget();
    void initWidgetValue();
    void paintEvent(QPaintEvent *painter);
signals:
    void sqlValue(QList< QStringList > rowList, QMap<int, QString> sqlList);

private slots:
    void setCalendarStatue_s(bool);
    void setCalendarStatue_e(bool);
    void updateCaleSta_s();
    void updateCaleSta_e();

    void replyFinished();
    void readyRead();
    void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);
    void requestVersionInfo(const int &row);

private:
    void startRequest(QByteArray url);
    void getUpdateVersionList();
    QByteArray getUrl(QByteArray path);
    void errView(QJsonObject json);
    QJsonObject getRequestData(QString array);
    QJsonArray setPagingInfo(QJsonObject json);
    void setVersionList(QJsonArray json);
    void setUpdateList(QJsonObject json);
    QString getOptType(const QString &sql);

private:
    QWidget *widget_top;
    QWidget *widget_center;
    QWidget *widget_bottom;

    QLineEdit *lineedit_start;
    QLineEdit *lineedit_end;
    QLineEdit *lineedit_recordPage;

    QPushButton *pushbutton_query;
    QPushButton *pushbutton_start;
    QPushButton *pushbutton_end;
    QPushButton *pushbutton_up;
    QPushButton *pushbutton_next;

    QLabel *label_cen;
    QLabel *label_progress;
    QLabel *label_speed;
    QLabel *label_totalRecords;
    QLabel *label_recordPage;
    QLabel *label_page;
    PagingTableView *update_list;
    QCalendarWidget *calendar_start;
    QCalendarWidget *calendar_end;

    QHBoxLayout *layout_select;
    QHBoxLayout *layout_top;
    QHBoxLayout *layout_list;
    QHBoxLayout *layout_pro;
    QHBoxLayout *layout_paging;
    QVBoxLayout *vLayout;

    QString host;
    QNetworkReply *reply;
    QNetworkAccessManager *manager;
    QList< QStringList > rowList;
    QMap<int, QString> sqlList;
    int request_type;
};

#endif // YUNCLIENT_H
