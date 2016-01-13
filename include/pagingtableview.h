#ifndef PAGINGTABLEVIEW_H
#define PAGINGTABLEVIEW_H

#include "defistruct.h"
#include "curlupload.h"
#include "typeindexvalue.h"
#include <QWidget>
#include <QMap>
class QSqlQuery;
class QMenu;
class QLabel;
class QCheckBox;
class QLineEdit;
class QPushButton;
class QTableView;
class TableModel;
class TableView;
class ButtonDelegate;
class IconDelegate;
class ImageDelegate;
class CheckBoxDelegate;
class MoveDelegate;
class QHBoxLayout;
/*
 * 分页处理基类
 * 查询出来需要分页显示的可以集成该类
*/
class PagingTableView : public QWidget
{
    Q_OBJECT
public:
    explicit PagingTableView(QWidget *parent = 0, bool alternate_background_color = false);
    ~PagingTableView();
    void paintEvent(QPaintEvent *);
private:
    void initWidget();
    void setTableWidget();
    void widgetLayout();
    void actionMenu();
    void setWidgetValue();
public:
    /*
     * 初始化分页信息
    */
    void initPagingQueryLimitArgu();

signals:
    void checkBoxIsAll(bool checked);
    void currentRow(const int &row);
    void play(const int &row, int isPlay);
    void moveUp(const int &row);
    void moveDown(const int &row);
    void replace(const int &row);
    void skipMusic(const int &row,int isSkip);
    void matchMusic(const int &row, int isMatch);
    void modify(const int &row);
    void dele(const int &row);
    void add(const int &row);    

    void pointRow(const int &row);
    void selectRow(const QModelIndex & index);
    void contentMenu(const QPoint &pos);

public slots:
    void returnRow(const QPoint & pos);
    void update_View();
    void palyVideo(const QString &path, const int &track);
public:
    /*
     * 设置显示勾选框
    */
    void setShowCheckBox();
    /*
     * 初始化歌曲自定义委托
    */
    void initMediaDelegate(bool isBlack = true);
    /*
     * 初始化歌星自定义委托
    */
    void initActorDelegate(bool isBlack = true);
    /*
     * 歌星查询自定义委托
    */
    void setQueryActorDelegate();
    /*
     *歌曲查询自定义委托
    */
    void setQueryMediaDelegate();
    /*
     * fm自定义委托
    */
    void setFmDelegate();
    /*
     * fm添加歌曲自定义委托
     * add_dele 委托按键字符
    */
    void setFmAddMusicDelegate(const QString &add_dele);
    /*
     * fm歌曲显示自定义委托
    */
    void setFmShowMusicDelegate();
    /*
     * 设置广告自定义委托
    */
    void setAdDelegate();
    /*
     * 歌单自定义委托
    */
    void setMediaListDelegate();
    /*
     * 资料设置自定义委托
    */
    void setInfoDelegate(int rows);
    /*
     * 歌星添加修改自定义委托
    */
    void setAddModifyDelegate(const QString &add_dele);
    /*
     * 歌星上线自定义委托
    */
    void setActorOnlineDelegate();
    /*
     * 歌曲上线自定义委托
    */
    void setMediaOnlineDelegate();
    /*
     *云更新列表自定义委托
    */
    void setYunDelegate();
    /*
     * 云更新下载自定义委托
    */
    void setYunDownDelegate();
    /*
     * 查询显示数据用（）
    */
    void showUploadData(QSqlQuery &query, bool showMediaOrActor); ///true歌曲 false歌星
    /*
     * 查询歌星数据显示
    */
    void showQuerySingerData(QSqlQuery &query);
    /*
     * 歌星添加修改数据委托
    */
    void showUploadSingerAddModityData(QSqlQuery &query, QList< QString > &paths);
    void showUploadMediaListData(QSqlQuery &query);
    void showUploadData_blackActor(QSqlQuery &query);

    /*
     * 设置歌星/歌曲值
    */
    void setMediaValue(const QSqlQuery &query, QStringList &rowValue, int );
    void setActorValue(const QSqlQuery &query, const QString &runPath,
                       int , QStringList &rowValue);

    /*
     * 设置歌星黑名单值
    */
    void setActorValue_blackActor(const QSqlQuery &query, const QString &runPath,
                       int , QStringList &rowValue);
    /*
     * 设置最小面翻页信息widget隐藏
    */
    void setWidgetBottomHidden(const bool &hidden = true);

    /*
     * 设置model数值
    */
    void setModelValue(QList<QStringList> &data);
    /*
     * 获取model的值
    */
    void getValue(QList< QStringList > &rows);
    int rowCount();

    void setSexNation(const QMap<int, QString> &_nation, const QMap<int, QString> &_sex);

    /*
     * 设置歌曲列宽
    */
    void setMediaColumnWidth(const int &width = 1150);
    /*
     * 设置歌星列宽
    */
    void setActorColumnWidth(const int &columns, const int &width = 1150);

    void setYunUpdateColumnWidget(const int &width = 1150);

    /*
     * 设置歌曲黑名单列宽
    */
    void setMedia_black_CW(const int &width = 1150);
    /*
     * 设置歌星黑名单列宽
    */
    void setActor_black_CW(const int &columns = 6, const int &width = 1150);

    /*
     * 设置平均列宽
    */
    void setSectionResizeMode();
    /*
     * 设置当前行数
    */
    void setCurrentRowNum(const int &num);

private slots:

signals:
    void updateView(const PagingQueryLimitArgu &limit);
public slots:
    void up_page();
    void next_page();
    void first_page();
    void last_page();
    void jump_page(const int page);
    void show_page();

    void rowPageEditFinish();
    void jumpEditFinish();


public:    
    int setValidRowsCount(); ///获取有效行数
    void setTotal_Rows_Pages(const int rowsSize);

public:
    int total_page; //总页码
    int current_page; //当前页码
    int total_Rows;  //总共行数
    int row_page; //一页行数

    PagingQueryLimitArgu limitArgu;
    TableModel *model;
    QWidget *widget_bottom;
    TableView *tableView;
    QLabel *label_totalNum;
    QLabel *label_showPage;
    QLabel *label_toopTipLeft;
    QLabel *label_toopTipRight;

    QLineEdit *lineEdit_jump;
    QLineEdit *lineEdit_row;

    QPushButton *pushButton_upPage;
    QPushButton *pushButton_nextPage;
    QPushButton *pushButton_firstPage;
    QPushButton *pushButton_lastpage;
    QPushButton *pushButton_jumpPage;

//    TypeIndexValue value;
    bool showMediaOrActor;   //true 歌曲  false 歌星
    bool alternate_background_color;
    bool isCheckBox;    
    QList< QStringList > rowList;
    CurlUpload *curlDownlaod;
    QString runPath;

//    ButtonDelegate *info_Delegate;
    ButtonDelegate *m_cancelDelegate;
    IconDelegate *m_playDelegate;
    ImageDelegate *m_imageDelegate;
    CheckBoxDelegate * m_checkBoxDelegate;
    MoveDelegate *m_moveDelegate;

    QMap<int, QString> nation; // 国籍
    QMap<int, QString> sex; // 性别
};

#endif // PAGINGTABLEVIEW_H
