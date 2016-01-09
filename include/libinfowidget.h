#ifndef LIBINFOWIDGET_H
#define LIBINFOWIDGET_H

#include <QMap>
#include <QWidget>
#include <QSqlQuery>
class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class QPushButton;
class QTableView;
class MysqlQuery;
class PagingTableView;
class LibDelegate;
/*
 * 曲库统计界面
*/
class LibInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LibInfoWidget(QWidget *parent = 0);
    ~LibInfoWidget();

    void readAndSetStyleSheet();
    void initWidget();
    void initWidgetValue();
    void paintEvent(QPaintEvent *);

    void initSqlPointer( MysqlQuery *sql);
    void initQueryCondition();
    void setTableModelValue( QList< QPair<QString, qint64> > listA,
                             QList< QPair<QString, qint64> > listM);
    /*
     * 歌星统计双击显示内容
    */
    void singerDoubleClicked(const QModelIndex & index);
    /*
     * 歌曲统计双击显示内容
    */
    void songDoubleClicked(const QModelIndex & index);

private slots:
    void release();

signals:

private:
   QVBoxLayout *musicLayout;
   QVBoxLayout *singerLayout;
   QVBoxLayout *centerLayout;
   QVBoxLayout *vLayout;
   QWidget *widget_libInfo;
   QWidget *widget_title;
   PagingTableView *tableView_songsTotal;
   PagingTableView *tableView_singerTotal;
   QLabel *label_libTitle;
   QLabel *label_musicTotal;
   QLabel *label_singerTotal;

   QPushButton  *button_release;

   MysqlQuery    *_sql;
   QSqlQuery query;
   LibDelegate *m_libDelegateM;
   LibDelegate *m_libDelegateA;
   QList< QStringList > rowListSinger;
   QList< QStringList > rowListSong;


   QMap<int, QString> nation; // 国籍
   QMap<int, QString> sex; // 性别
};

#endif // LIBINFOWIDGET_H
