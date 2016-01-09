#ifndef FMADDMODIFYDIALOG_H
#define FMADDMODIFYDIALOG_H

#include "dropshadowwidget.h"
#include "defistruct.h"
#include "mysqlquery.h"
#include <QObject>
#include <QWidget>
#include <QString>
#include <QSqlQuery>
class QLabel;
class QPushButton;
class QLineEdit;
class PagingTableView;

/*
 * fm歌单修改对话框
*/
class FmAddModifyDialog : public DropShadowWidget
{
    Q_OBJECT

public:    
    explicit FmAddModifyDialog(QWidget *parent = 0);
    ~FmAddModifyDialog();
    void paintEvent(QPaintEvent *);

    void initWidget();
    void initWidgetValue();

    void setTitle(const QString &str);
    void initSql(MysqlQuery *sql, QSqlQuery &query);
    void initType(const QString &type);
    void setReplaceRow(const bool &rep, const int &row);

    /*
     *设置fm状态  新fm或修改fm
     * state 状态
    */
    void setFmState(const int &state);
    /*
     *设置view代理
     * item
    */
    void setType_delegate(const int &item);
    /*
     *设置view歌曲信息
     * item
    */
    void setFmViewValue();

    /*
     * 设置view广告歌曲信息 （未使用）
    */
    void setAdViewValue(const QString &type);

    /*
     * 保存歌单修改信息
     * return  成功保存true 否则false
    */
    bool saveModityLid();
    /*
     * 保存新歌单信息
     * return  成功保存true 否则false
    */
    bool saveNewLid();
    /*
     * 保存歌单里增加的歌曲
     * return  成功保存true 否则false
    */
    bool saveMusic();

    /*
     * 隐藏歌单界面信息
    */
    void initMediaList();

    /*
     * 保存歌单信息
    */
    bool saveMediaList();

    /*
     *保存替换热歌和网络歌曲  （未使用）
    */
    bool saveReplaceMediaList(QString hotnet = NULL);
    /*
     *保存替换热歌  （未使用）
    */
    bool saveMediaListHot();
    /*
     *保存网络歌曲  （未使用）
    */
    bool saveMediaListNet();

    void setSavePushbuttonHidden(bool hidden = false);

signals:
    void update_view();

private slots:
    void close_dialog();
    void addMusic(bool checked = false);

    void previewImage();
    void uploadImage();

    void deleteMusic(const int &row);
    void moveUp(const int &row);
    void moveDown(const int &row);
    void replace(const int &row);
    void replaceViewMusic(const int &row);

    void serachMusic();    
    void addMusicDetail(const int &row);
    void save();    

    void play(const int &row, const int &);
    void playAddMusic(const int &row, const int &);

private:
    void getMusicDetail(QSqlQuery &querySearch, QStringList &tempValue);
    void getMediaListValue(const QSqlQuery &_que, MediaList &_list);



private:

    QWidget *widgetTitle;
    QWidget *widgetImage;
    QWidget *widgetAddMusic;

    QLabel *label_title;
    QLabel *label_listName;
    QLabel *label_listImage;
    QLabel *label_music;
    QLabel *label_image;

    QLineEdit *lineEdit_listName;
    QLineEdit *lineEdit_listImage;
    QLineEdit *lineEdit_search;

    QPushButton *pushButton_close;
    QPushButton *pushButton_preview;
    QPushButton *pushButton_upload;
    QPushButton *pushButton_addMusic;
    QPushButton *pushButton_save;
    QPushButton *pushButton_search;

    PagingTableView *view_addMusic;
    PagingTableView *view_music;

    MysqlQuery    *_sql;
    QSqlQuery _query;
    QSqlQuery _queryList;
    QSqlQuery _querySearch;
    int lid;
    QString imagePath;

    QList< QStringList > rowList;
    QList< QString > paths;

    int fmState;
    int _widItem;
    int replaceRow;

    bool replaceFalg;

    QString _type;
    SongList song_list;
    SongList new_song_list;
//    QString runPath;
};

#endif // FMADDMODIFYDIALOG_H
