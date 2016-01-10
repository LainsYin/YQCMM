#ifndef SINGERINFOWDIGET_H
#define SINGERINFOWDIGET_H

#include "defistruct.h"
#include <QWidget>
#include <QMap>
class QLabel;
class QLineEdit;
class QComboBox;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
/*
 * 歌星通用信息界面
*/
class SingerInfoWdiget : public QWidget
{
    Q_OBJECT
public:
    explicit SingerInfoWdiget(QWidget *parent = 0);
    ~SingerInfoWdiget();

    void readAndSetStyleSheet();
    void initWidget();
    void initWidgetValue();
    void setWidgetSize();
    void paintEvent(QPaintEvent *);

    /*
     * 歌星上线隐藏界面最下面widget
     * 歌星上线界面隐藏界面最下面widgetbottom
     * 显示widgetbottom_online
    */
    void isSingerOnline();
    /*
     *歌星预览或修改
     * 隐藏widgetbottom_online
     * 显示wigetbottom
    */
    void isPreviewAndModify();
    /*
     * 星号提示
    */
    void isRequired();
    /*
     * 是否为空
    */
    bool isEmpty();
    /*
     * 必填项是否填写
    */
    bool isRequiredOption();
    /*
     * 设置控件状态
     * status true 控件可编辑 false 控件不可编辑
    */
    void set_enabled(bool status);

    /*
     * 获取歌星信息数据
     * _actor 待赋值数据结构
    */
    void getActorValue(Actor &_actor);
    /*
     * 获取歌星上线数据
     * _actor 待赋值数据结构
    */
    void getActorOnlineValue(Actor &_actor);
    /*
     * 设置界面歌星值
     * _actor 歌星数据
    */
    void setActorValue(const Actor &_actor);
    /*
     * 设置歌星上线界面歌星值
     * _actor 歌星数据
    */
    void setActorValue_online(const Actor &_actor);
    /*
     * 获取歌星头像路径
     * return 返回歌星头像路径
    */
    QString getImagePath();

    /*
     * 设置sex
     * _sex 性别数据
    */
    void set_comboBox_sex(QMap<int, QString> &_sex);
    /*
     * 设置国籍
     * _nat 国籍数据
    */
    void set_comboBox_nation(QMap<int, QString> &_nat);
    /*
     * 设置starts enabled 值
    */
    void set_comboBox_Remain();
    /*
     * 设置歌星头像数据
     * path 歌星头像路径
    */
    void setImageOnlineRetPath(const QString &path);

    /*
     * 设置serial_id值
    */
    void setPlaceholder_serial_id(qint64 &serial_id);
    /*
     * 设置星号标识
    */
    void set_tooptip_image();

    void setOldActorValue(Actor _actor);
    void setComboBoxValue(QComboBox *combobox, QList<QString> list);
    void setSexValue();
    void setNationValue();
    void setStarsValue();
    void setEnabledValue();
signals:

public slots:
    void uploadImage();
    void uploadImageOnline();
    void previewOnline();

    void setWords();

private slots:
    void name_editFinish();
    void serial_id_editFinish();
    void pinyin_editFinish();
    void header_editFinish();
    void head_editFinish();
    void words_editFinish();
    void info_editFinish();
    void image_editFinish();
    void enabled_activated(const QString &val);
    void nation_activated(const QString &val);
    void sex_activated(const QString &val);
    void stars_activated(const QString &val);
private:
    QString border;
    QString border_yellow;
public:
    void setDefaultBorder();
    void setDefaultValue();

private:
    QHBoxLayout *serial_id_layout;
    QHBoxLayout *sex_layout;
    QHBoxLayout *stars_layout;
    QHBoxLayout *pinyin_layout;
    QHBoxLayout *head_layout;
    QHBoxLayout *count_layout;
    QVBoxLayout *left_layout;

    QHBoxLayout *name_layout;
    QHBoxLayout *nation_layout;
    QHBoxLayout *enabled_layout;
    QHBoxLayout *header_layout;
    QHBoxLayout *words_layout;
    QHBoxLayout *info_layout;
    QHBoxLayout *info_online_layout;
    QVBoxLayout *center_layout;

    QHBoxLayout *upload_layout;
    QHBoxLayout *bottom_layout;
    QHBoxLayout *upload_online_layout;
    QHBoxLayout *bottom_online_layout;

    QHBoxLayout *image_tooltip_layout;
    QVBoxLayout *right_layout;
    QHBoxLayout *singerInfo_layout;
    QVBoxLayout *vLayout;

    QWidget *widgetLeft;
    QWidget *widgetCenter;
    QWidget *widgetRight;
    QWidget *widgetBottom;
    QWidget *widgetBottom_online;


    QLabel *label_serial_id;
    QLabel *label_sex;
    QLabel *label_stars;
    QLabel *label_pinyin;
    QLabel *label_head;
    QLabel *label_count;

    QLabel *label_name;
    QLabel *label_nation;
    QLabel *label_enabled;
    QLabel *label_header;
    QLabel *label_words;
    QLabel *label_info;
    QLabel *label_info_online;

    QLabel *label_image;
    QLabel *label_show_image;

private:
    ///星号提示
    QLabel *label_serial_id_tooptip;
    QLabel *label_sex_tooptip;
    QLabel *label_stars_tooptip;
    QLabel *label_pinyin_tooptip;
    QLabel *label_head_tooptip;
    QLabel *label_count_tooptip;

    QLabel *label_name_tooptip;
    QLabel *label_nation_tooptip;
    QLabel *label_enabled_tooptip;
    QLabel *label_header_tooptip;
    QLabel *label_words_tooptip;
    QLabel *label_info_tooptip;
    QLabel *label_info_online_tooptip;

    QLabel *label_image_tooptip;

    QLineEdit *lineEdit_serial_id;
    QLineEdit *lineEdit_pinyin;
    QLineEdit *lineEdit_head;
    QLineEdit *lineEdit_count;

    QLineEdit *lineEdit_name;
    QLineEdit *lineEdit_header;
    QLineEdit *lineEdit_words;
    QLineEdit *lineEdit_info;
    QLineEdit *lineEdit_image;
    QLineEdit *lineEdit_info_online;
    QLineEdit *lineEdit_image_online;

public:
    QPushButton *pushButton_upload;
    QPushButton *pushButton_upload_online;
    QPushButton *pushButton_preview_online;

    QComboBox *comboBox_sex;
    QComboBox *comboBox_stars;

    QComboBox *comboBox_nation;
    QComboBox *comboBox_enabled;


    Actor actor;
    QMap<int, QString> sex, nation, enabled, stars;
};

#endif // SINGERINFOWDIGET_H
