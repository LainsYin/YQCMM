#ifndef FMINFOWIDGET_H
#define FMINFOWIDGET_H

#include <QWidget>
#include <QSqlQuery>
class QLabel;
class QTableView;
class QPushButton;
class MysqlQuery;
class PagingTableView;
class QVBoxLayout;
class QHBoxLayout;

/*
 * fm歌单基本信息界面
*/
class FmInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FmInfoWidget(QWidget *parent = 0);
    ~FmInfoWidget();

    void readAndSetStyleSheet();
    void initWidget();
    void initWidgetValue();
    void setWidgetSize();
    void paintEvent(QPaintEvent *);

    void initSql(MysqlQuery *sql);

    /*
     * 设置fm歌单名
     * fmname 歌单名
    */
    void setFmName(const QString &fmname);

    /*
     * 设置fm图片
     * path 图片路径
    */
    void setImage(const QString &path);

    /*
     * 设置共有多少歌单
     * total 歌单数
    */
    void setTotal(const int &total);

    /*
     * 设置歌单页数
     * total 歌单数
    */
    void setTotalRows(const int &total);

    /*
     * 设置歌单显示歌曲
     * query 数据库返回数据
    */
    void setTableValue(QSqlQuery &query);

    /*
     * 获取鼠标所在位置
    */
    QRect getRightButtonGeometry();

    /*
     * 清除界面歌单数据
    */
    void clear();

    /*
     * 界面设置fm歌单图片
    */
    void setImage(const QPixmap &pixmap);
signals:
    void  modifyFm();
    void  deleteFm();

public slots:
    void play(const int &row, const int &);

private:
    QVBoxLayout *image_layout;
    QVBoxLayout *imageHeader_layout;
    QHBoxLayout *imageTable_layout;
    QHBoxLayout *fmName_layout;
    QHBoxLayout *fmModify_layout;
    QHBoxLayout *top_layout;

    QVBoxLayout *vLayout;

    QLabel *label_fmName;
    QLabel *label_total;
    QLabel *label_image_header;
    QLabel *label_image;

    QPushButton *pushButton_modify;
    QPushButton *pushButton_delete;

    PagingTableView *tableView_fm;
    MysqlQuery *_sql;
    QSqlQuery _query;
};

#endif // FMINFOWIDGET_H
