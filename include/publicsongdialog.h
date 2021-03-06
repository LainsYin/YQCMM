#ifndef PUBLICSONGDIALOG_H
#define PUBLICSONGDIALOG_H


#include "dropshadowwidget.h"
#include "defistruct.h"
#include "mysqlquery.h"
#include <QObject>
#include <QWidget>
#include <QSqlQuery>
class QLabel;
class QPushButton;
class QLineEdit;
class QProgressBar;
class QComboBox;
class Media;
/*
 * 公播类型 上传歌曲资源 对话框
*/
class PublicSongDialog : public DropShadowWidget
{
    Q_OBJECT

public:
    explicit PublicSongDialog(QWidget *parent = 0);
    ~PublicSongDialog();
    void paintEvent(QPaintEvent *);

    void initWidget();
    void initWidgetValue();

    void setTitle(const QString &str);
    void initSql(MysqlQuery *sql);

private slots:
    /*
     * 视频预览
    */
    void preview();
    /*
     * 确定上传
    */
    void yes();

    //未实现
    void uploadsuccess(const qint64 &mid);
    /*
     * 更新进度定时开始
    */
    void timeStart(qint64 mid = 0);
    /*
     * 更新进度定时结束
    */
    void timeOver();

private:
    QWidget *widgetTitle;
    QWidget *widgetBottom;

    QLabel *label_title;
    QLabel *label_tip;
//    QLabel *label_musicName;
//    QLabel *label_video;

    QPushButton *pushbutton_close;
    QPushButton *pushbutton_preview;
    QPushButton *pushbutton_yes;

    QLineEdit *lineedit_name;
    QLineEdit *lineedit_path;

    QComboBox *combobox_type;
    QProgressBar *progress;


    MysqlQuery    *_sql;
    QList< Media> medias;
    QString retSize;
    QTimer *timer;

    bool size;
};

#endif // PUBLICSONGDIALOG_H
