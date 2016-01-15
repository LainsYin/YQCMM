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
class MysqlQuery;
class YunDM;

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
    void initSql(MysqlQuery *sql);
    void paintEvent(QPaintEvent *painter);
signals:
    void sqlValue(QList< QStringList > rowList);

private slots:
    //显示隐藏日历
    void setCalendarStatue_s(bool);
    void setCalendarStatue_e(bool);
    //更新日历
    void updateCaleSta_s();
    void updateCaleSta_e();

    //请求结束
    void replyFinished();
    //读取数据
    void readyRead();
    //更新进度
    void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);
    /*
     * 更求版本列表
     * row 当前行的版本信息列表
    */
    void requestVersionInfo(const int &row);

    void nextPage();
    void prePage();
    void pageTurn(int page);    
    void recordOfPageEditFinish();

private:
    /*
     * 开始请求
     * url 请求的url
    */
    void startRequest(QByteArray url);
    /*
     * 获取未更新的版本列表
    */
    void getUpdateVersionList();
    /*
     * 拼接url
     * path 请求的url除去主机段部分
     * return 返回url
    */
    QByteArray getUrl(QByteArray path);
    /*
     * 请求返回错误对话框提示
     * json 请求获取的json数据，用来获取返回的错误信息
    */
    void errView(QJsonObject json);
    /*
     * 获取data数据
     * return Data的json对象
    */
    QJsonObject getRequestData(QString array);
    /*
     * 设置分页信息
     * json 请求获取到的json信息 用来获取页码信息
    */
    QJsonArray setPagingInfo(QJsonObject json);
    /*
     *  设置版本列表
     * json 请求获取到的json信息
    */
    void setVersionList(QJsonArray json);
    /*
     * 版本信息的歌曲/歌星列表
    */
    void setUpdateList(QJsonObject json);
    /*
     * 获取操作类型
     * sql  操作的sql语句
    */
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
    int request_type;
    MysqlQuery *_sql;
    YunDM *dialog;
};

#endif // YUNCLIENT_H
