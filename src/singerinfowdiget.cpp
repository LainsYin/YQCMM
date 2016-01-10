#include "singerinfowdiget.h"
#include "curlupload.h"
#include "pinyinall.h"
#include "releasetech.h"
#include <QFile>
#include <QLabel>
#include <QPainter>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QFileDialog>
#include <QProcessEnvironment>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QListView>
#define  MaxItems 5

SingerInfoWdiget::SingerInfoWdiget(QWidget *parent)
    : QWidget(parent)
{
    initWidget();
    setWidgetSize();
    readAndSetStyleSheet();
    initWidgetValue();

    isRequired();

    enabled.insert(1, "启用");
    enabled.insert(0, "禁用");

    stars.insert(1,"5.0");
    stars.insert(2,"4.5");
    stars.insert(3,"4.0");
    stars.insert(4,"3.5");
    stars.insert(5,"3.0");
    stars.insert(6,"2.5");
    stars.insert(7,"2.0");
    stars.insert(8,"1.5");
    stars.insert(9,"1.0");
    stars.insert(10,"0.5");
    stars.insert(11,"0.0");

    label_show_image->setScaledContents(true); //图片随label调整大小
    label_show_image->setAlignment(Qt::AlignCenter);

    connect(pushButton_upload, &QPushButton::clicked, this, &SingerInfoWdiget::uploadImage);
    connect(pushButton_upload_online, &QPushButton::clicked, this, &SingerInfoWdiget::uploadImageOnline);
    connect(pushButton_preview_online, &QPushButton::clicked, this, &SingerInfoWdiget::previewOnline);



    border = QString("border: 1px solid rgb(255, 0, 0)");
    border_yellow = QString("border: 1px solid rgb(0, 0, 255)");
    connect(lineEdit_name, &QLineEdit::editingFinished, this, &SingerInfoWdiget::setWords);
    connect(lineEdit_name, &QLineEdit::editingFinished, this, &SingerInfoWdiget::name_editFinish);
    connect(lineEdit_serial_id, &QLineEdit::editingFinished, this, &SingerInfoWdiget::serial_id_editFinish);
    connect(lineEdit_header, &QLineEdit::editingFinished, this, &SingerInfoWdiget::header_editFinish);
    connect(lineEdit_head, &QLineEdit::editingFinished, this, &SingerInfoWdiget::head_editFinish);
    connect(lineEdit_pinyin, &QLineEdit::editingFinished, this, &SingerInfoWdiget::pinyin_editFinish);
    connect(lineEdit_words, &QLineEdit::editingFinished, this, &SingerInfoWdiget::words_editFinish);
    connect(lineEdit_info_online, &QLineEdit::editingFinished, this, &SingerInfoWdiget::info_editFinish);
    connect(lineEdit_image_online, &QLineEdit::editingFinished, this, &SingerInfoWdiget::image_editFinish);

    connect(comboBox_enabled, SIGNAL(activated(QString)), this, SLOT(enabled_activated(QString)));
    connect(comboBox_nation, SIGNAL(activated(QString)), this, SLOT(nation_activated(QString)));
    connect(comboBox_sex, SIGNAL(activated(QString)), this, SLOT(sex_activated(QString)));
    connect(comboBox_stars, SIGNAL(activated(QString)), this, SLOT(stars_activated(QString)));

}

SingerInfoWdiget::~SingerInfoWdiget()
{

}

void SingerInfoWdiget::readAndSetStyleSheet()
{
    QFile qss(":/qss/songinfo.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());
    qss.close();

    this->setObjectName("Widget");

    label_count->setObjectName("SingerInfoLabel");
    label_info->setObjectName("SingerInfoLabel");

    lineEdit_count->setObjectName("LineEdit");
    lineEdit_head->setObjectName("LineEdit");
    lineEdit_header->setObjectName("LineEdit");
    lineEdit_image->setObjectName("LineEdit");
    lineEdit_image_online->setObjectName("LineEdit");
    lineEdit_info->setObjectName("LineEdit");
    lineEdit_info_online->setObjectName("LineEdit");
    lineEdit_name->setObjectName("LineEdit");
    lineEdit_pinyin->setObjectName("LineEdit");
    lineEdit_serial_id->setObjectName("LineEdit");
    lineEdit_words->setObjectName("LineEdit");

    comboBox_enabled->setObjectName("Combobox");
    comboBox_nation->setObjectName("Combobox");
    comboBox_sex->setObjectName("Combobox");
    comboBox_stars->setObjectName("Combobox");

    pushButton_preview_online->setObjectName("Button");
    pushButton_upload->setObjectName("Button");
    pushButton_upload_online->setObjectName("Button");   
}

void SingerInfoWdiget::initWidget()
{
    widgetLeft = new QWidget(this);
    widgetCenter = new QWidget(this);
    widgetRight = new QWidget(this);
    widgetBottom = new QWidget(this);
    widgetBottom_online = new QWidget(this);

    label_serial_id = new QLabel(widgetLeft);
    label_sex = new QLabel(widgetLeft);
    label_stars = new QLabel(widgetLeft);
    label_pinyin = new QLabel(widgetLeft);
    label_head = new QLabel(widgetLeft);    

    label_name = new QLabel(widgetCenter);
    label_nation = new QLabel(widgetCenter);
    label_enabled = new QLabel(widgetCenter);
    label_header = new QLabel(widgetCenter);
    label_words = new QLabel(widgetCenter);

    label_image = new QLabel(widgetRight);
    label_show_image = new QLabel(widgetRight);

    label_count = new QLabel(this);
    label_info = new QLabel(this);
    label_info_online = new QLabel(this);

    ///星号提示
    label_serial_id_tooptip = new QLabel(widgetLeft);
    label_sex_tooptip = new QLabel(widgetLeft);
    label_stars_tooptip = new QLabel(widgetLeft);
    label_pinyin_tooptip = new QLabel(widgetLeft);
    label_head_tooptip = new QLabel(widgetLeft);
    label_count_tooptip = new QLabel(this);

    label_name_tooptip = new QLabel(widgetCenter);
    label_nation_tooptip = new QLabel(widgetCenter);
    label_enabled_tooptip = new QLabel(widgetCenter);
    label_header_tooptip = new QLabel(widgetCenter);
    label_words_tooptip = new QLabel(widgetCenter);
    label_info_tooptip = new QLabel(this);
    label_info_online_tooptip = new QLabel(this);
    label_image_tooptip = new QLabel(widgetRight);   

    lineEdit_serial_id = new QLineEdit(widgetLeft);
    lineEdit_pinyin = new QLineEdit(widgetLeft);
    lineEdit_head = new QLineEdit(widgetLeft);

    lineEdit_name = new QLineEdit(widgetCenter);
    lineEdit_header = new QLineEdit(widgetCenter);
    lineEdit_words = new QLineEdit(widgetCenter);

    lineEdit_count = new QLineEdit(this);
    lineEdit_info = new QLineEdit(this);
    lineEdit_image = new QLineEdit(widgetBottom);

    lineEdit_info_online = new QLineEdit(widgetBottom_online);
    lineEdit_image_online = new QLineEdit(widgetBottom_online);

    pushButton_upload = new QPushButton(widgetBottom);
    pushButton_upload_online = new QPushButton(widgetBottom_online);
    pushButton_preview_online = new QPushButton(widgetBottom_online);

    comboBox_sex = new QComboBox(widgetLeft);
    comboBox_stars = new QComboBox(widgetLeft);

    QListView *view_sex = new QListView();
    QListView *view_sta = new QListView();
    comboBox_sex->setView(view_sex);
    comboBox_stars->setView(view_sta);
    comboBox_sex->setMaxVisibleItems(MaxItems);
    comboBox_stars->setMaxVisibleItems(MaxItems);

    comboBox_nation = new QComboBox(widgetCenter);
    comboBox_enabled = new QComboBox(widgetCenter);

    QListView *view_nat = new QListView();
    QListView *view_ena = new QListView();
    comboBox_nation->setView(view_nat);
    comboBox_enabled->setView(view_ena);
    comboBox_nation->setMaxVisibleItems(MaxItems);
    comboBox_enabled->setMaxVisibleItems(MaxItems);

    serial_id_layout = new QHBoxLayout();
    sex_layout = new QHBoxLayout();
    stars_layout = new QHBoxLayout();
    pinyin_layout = new QHBoxLayout();
    head_layout = new QHBoxLayout();
    left_layout = new QVBoxLayout();

    serial_id_layout->addWidget(label_serial_id_tooptip);
    serial_id_layout->addWidget(label_serial_id);
    serial_id_layout->addWidget(lineEdit_serial_id);
    serial_id_layout->setContentsMargins(0, 0, 0, 0);
    serial_id_layout->setSpacing(0);

    sex_layout->addWidget(label_sex_tooptip);
    sex_layout->addWidget(label_sex);
    sex_layout->addWidget(comboBox_sex);
    sex_layout->setContentsMargins(0, 0, 0, 0);
    sex_layout->setSpacing(0);

    stars_layout->addWidget(label_stars_tooptip);
    stars_layout->addWidget(label_stars);
    stars_layout->addWidget(comboBox_stars);
    stars_layout->setContentsMargins(0, 0, 0, 0);
    stars_layout->setSpacing(0);

    pinyin_layout->addWidget(label_pinyin_tooptip);
    pinyin_layout->addWidget(label_pinyin);
    pinyin_layout->addWidget(lineEdit_pinyin);
    pinyin_layout->setContentsMargins(0, 0, 0, 0);
    pinyin_layout->setSpacing(0);

    head_layout->addWidget(label_head_tooptip);
    head_layout->addWidget(label_head);
    head_layout->addWidget(lineEdit_head);
    head_layout->setContentsMargins(0, 0, 0, 0);
    head_layout->setSpacing(0);

    left_layout->addLayout(serial_id_layout);
    left_layout->addLayout(sex_layout);
    left_layout->addLayout(stars_layout);
    left_layout->addLayout(pinyin_layout);
    left_layout->addLayout(head_layout);
    left_layout->setContentsMargins(0, 0, 0, 0);
    left_layout->setSpacing(10);

    name_layout = new QHBoxLayout();
    nation_layout = new QHBoxLayout();
    enabled_layout = new QHBoxLayout();
    header_layout = new QHBoxLayout();
    words_layout = new QHBoxLayout();
    center_layout = new QVBoxLayout();

    name_layout->addWidget(label_name_tooptip);
    name_layout->addWidget(label_name);
    name_layout->addWidget(lineEdit_name);
    name_layout->setContentsMargins(0, 0, 0, 0);
    name_layout->setSpacing(0);

    nation_layout->addWidget(label_nation_tooptip);
    nation_layout->addWidget(label_nation);
    nation_layout->addWidget(comboBox_nation);
    nation_layout->setContentsMargins(0, 0, 0, 0);
    nation_layout->setSpacing(0);

    enabled_layout->addWidget(label_enabled_tooptip);
    enabled_layout->addWidget(label_enabled);
    enabled_layout->addWidget(comboBox_enabled);
    enabled_layout->setContentsMargins(0, 0, 0, 0);
    enabled_layout->setSpacing(0);

    header_layout->addWidget(label_header_tooptip);
    header_layout->addWidget(label_header);
    header_layout->addWidget(lineEdit_header);
    header_layout->setContentsMargins(0, 0, 0, 0);
    header_layout->setSpacing(0);

    words_layout->addWidget(label_words_tooptip);
    words_layout->addWidget(label_words);
    words_layout->addWidget(lineEdit_words);
    words_layout->setContentsMargins(0, 0, 0, 0);
    words_layout->setSpacing(0);

    center_layout->addLayout(name_layout);
    center_layout->addLayout(nation_layout);
    center_layout->addLayout(enabled_layout);
    center_layout->addLayout(header_layout);
    center_layout->addLayout(words_layout);
    center_layout->setContentsMargins(0, 0, 0, 0);
    center_layout->setSpacing(10);

    info_layout = new QHBoxLayout();
    info_online_layout = new QHBoxLayout();
    count_layout = new QHBoxLayout();

    info_layout->addWidget(label_info_tooptip);
    info_layout->addWidget(label_info);
    info_layout->addWidget(lineEdit_info);
    info_layout->setContentsMargins(8, 0, 0, 0);
    info_layout->setSpacing(0);

    count_layout->addWidget(label_count_tooptip);
    count_layout->addWidget(label_count);
    count_layout->addWidget(lineEdit_count);
    count_layout->setContentsMargins(0, 0, 0, 0);
    count_layout->setSpacing(0);

    info_online_layout->addWidget(label_info_online_tooptip);
    info_online_layout->addWidget(label_info_online);
    info_online_layout->addWidget(lineEdit_info_online);
    info_online_layout->setContentsMargins(0, 0, 0, 0);
    info_online_layout->setSpacing(0);

    upload_layout = new QHBoxLayout();
    bottom_layout = new QHBoxLayout();
    upload_online_layout = new QHBoxLayout();
    bottom_online_layout = new QHBoxLayout();

    upload_layout->addWidget(lineEdit_image);
    upload_layout->addWidget(pushButton_upload);
    upload_layout->addStretch();
    upload_layout->setContentsMargins(0, 0, 0, 0);
    upload_layout->setSpacing(10);

    bottom_layout->addLayout(count_layout);
    bottom_layout->addLayout(info_layout);
    bottom_layout->addLayout(upload_layout);
    bottom_layout->addStretch();
    bottom_layout->setContentsMargins(0, 0, 0, 0);
    bottom_layout->setSpacing(42);
    widgetBottom->setLayout(bottom_layout);

    upload_online_layout->addWidget(lineEdit_image_online);
    upload_online_layout->addWidget(pushButton_preview_online);
    upload_online_layout->addWidget(pushButton_upload_online);   
    upload_online_layout->setContentsMargins(0, 0, 0, 0);
    upload_online_layout->setSpacing(8);

    bottom_online_layout->addLayout(info_online_layout);
    bottom_online_layout->addLayout(upload_online_layout);
    bottom_online_layout->addStretch();
    bottom_online_layout->setContentsMargins(0, 0, 0, 0);
    bottom_online_layout->setSpacing(42);
    widgetBottom_online->setLayout(bottom_online_layout);

    image_tooltip_layout = new QHBoxLayout();
    right_layout = new QVBoxLayout();
    image_tooltip_layout->addWidget(label_image_tooptip);
    image_tooltip_layout->addWidget(label_image, 0, Qt::AlignLeft);
    image_tooltip_layout->setContentsMargins(0, 0, 0, 0);
    image_tooltip_layout->setSpacing(0);

    right_layout->addLayout(image_tooltip_layout);
    right_layout->addWidget(label_show_image, 0, Qt::AlignLeft);
    right_layout->addStretch();
    right_layout->setContentsMargins(0, 0, 0, 0);
    right_layout->setSpacing(10);


    singerInfo_layout = new QHBoxLayout();
    singerInfo_layout->addLayout(left_layout);
    singerInfo_layout->addLayout(center_layout);
    singerInfo_layout->addLayout(right_layout);
    singerInfo_layout->addStretch();
    singerInfo_layout->setContentsMargins(0, 0, 0, 0);
    singerInfo_layout->setSpacing(42);

    vLayout = new QVBoxLayout();
    vLayout->addLayout(singerInfo_layout);
    vLayout->addWidget(widgetBottom);
    vLayout->addWidget(widgetBottom_online);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(10);

    this->setLayout(vLayout);
}

void SingerInfoWdiget::initWidgetValue()
{
#ifndef YQC_TECH
        label_serial_id->setText("MID");
        label_serial_id->setEnabled(false);
        lineEdit_serial_id->setEnabled(false);
#else
        label_serial_id->setText("SERIAL_ID");
#endif
    label_sex->setText("歌星性别");
    label_stars->setText("歌星评分");
    label_pinyin->setText("名称全拼");
    label_head->setText("名称第一个\n字首字母");

    label_name->setText("歌星名称");
    label_nation->setText("歌星区域");
    label_enabled->setText("是否启用");
    label_header->setText("名称首字母");
    label_words->setText("名称字数");
    label_image->setText("歌星头像");

    label_count->setText("歌曲数量");
    label_info->setText("备    注");
    label_info_online->setText("备    注");

    pushButton_upload->setText("浏览");
    pushButton_preview_online->setText("浏览");
    pushButton_upload_online->setText("上传");
}

void SingerInfoWdiget::setWidgetSize()
{
    label_serial_id->setFixedWidth(80);
    label_sex->setFixedWidth(80);
    label_stars->setFixedWidth(80);
    label_pinyin->setFixedWidth(80);
    label_head->setFixedWidth(80);

    label_name->setFixedWidth(80);
    label_nation->setFixedWidth(80);
    label_enabled->setFixedWidth(80);
    label_header->setFixedWidth(80);
    label_words->setFixedWidth(80);

    label_image->setFixedWidth(80);
    label_show_image->setFixedWidth(164);
    label_show_image->setFixedHeight(164);

    label_count->setFixedWidth(80);
    label_info->setFixedWidth(80);
    label_info_online->setFixedWidth(80);    
    label_count_tooptip->setFixedWidth(8);
    label_info_tooptip->setFixedWidth(8);
    label_info_online_tooptip->setFixedWidth(8);

    label_serial_id_tooptip->setFixedWidth(8);
    label_sex_tooptip->setFixedWidth(8);
    label_stars_tooptip->setFixedWidth(8);
    label_pinyin_tooptip->setFixedWidth(8);
    label_head_tooptip->setFixedWidth(8);

    label_name_tooptip->setFixedWidth(8);
    label_nation_tooptip->setFixedWidth(8);
    label_enabled_tooptip->setFixedWidth(8);
    label_header_tooptip->setFixedWidth(8);
    label_words_tooptip->setFixedWidth(8);

    label_image_tooptip->setFixedWidth(8);


    lineEdit_serial_id->setFixedWidth(250);
    lineEdit_serial_id->setFixedHeight(36);
    lineEdit_pinyin->setFixedWidth(250);
    lineEdit_pinyin->setFixedHeight(36);
    lineEdit_head->setFixedWidth(250);
    lineEdit_head->setFixedHeight(36);

    lineEdit_name->setFixedWidth(250);
    lineEdit_name->setFixedHeight(36);
    lineEdit_header->setFixedWidth(250);
    lineEdit_header->setFixedHeight(36);
    lineEdit_words->setFixedWidth(250);
    lineEdit_words->setFixedHeight(36);

    lineEdit_count->setFixedWidth(250);
    lineEdit_count->setFixedHeight(36);
    lineEdit_info->setFixedWidth(250);
    lineEdit_info->setFixedHeight(36);
    lineEdit_image->setFixedWidth(250);
    lineEdit_image->setFixedHeight(36);

    lineEdit_info_online->setFixedWidth(620+9);
    lineEdit_info_online->setFixedHeight(36);
    lineEdit_image_online->setFixedWidth(192);
    lineEdit_image_online->setFixedHeight(36);


    pushButton_upload->setFixedWidth(60);
    pushButton_upload->setFixedHeight(36);
    pushButton_upload_online->setFixedWidth(60);
    pushButton_upload_online->setFixedHeight(36);
    pushButton_preview_online->setFixedWidth(60);
    pushButton_preview_online->setFixedHeight(36);

    comboBox_sex->setFixedWidth(250);
    comboBox_sex->setFixedHeight(36);
    comboBox_stars->setFixedWidth(250);
    comboBox_stars->setFixedHeight(36);

    comboBox_nation->setFixedWidth(250);
    comboBox_nation->setFixedHeight(36);
    comboBox_enabled->setFixedWidth(250);
    comboBox_enabled->setFixedHeight(36);
}

void SingerInfoWdiget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void SingerInfoWdiget::isSingerOnline()
{
    widgetBottom->setHidden(true);
    widgetBottom_online->setHidden(false);
}

void SingerInfoWdiget::isPreviewAndModify()
{
    widgetBottom->setHidden(false);
    widgetBottom_online->setHidden(true);
}

void SingerInfoWdiget::isRequired()
{
    label_serial_id_tooptip->setObjectName("TooptipLabel");
    label_sex_tooptip->setObjectName("TooptipLabel");
    label_stars_tooptip->setObjectName("TooptipLabel");
    label_pinyin_tooptip->setObjectName("TooptipLabel");
    label_head_tooptip->setObjectName("TooptipLabel");

    label_name_tooptip->setObjectName("TooptipLabel");
    label_nation_tooptip->setObjectName("TooptipLabel");
    label_enabled_tooptip->setObjectName("TooptipLabel");
    label_header_tooptip->setObjectName("TooptipLabel");
    label_words_tooptip->setObjectName("TooptipLabel");
    label_image_tooptip->setObjectName("TooptipLabel");
}

bool SingerInfoWdiget::isEmpty()
{
    if ( lineEdit_serial_id->text().isEmpty()
         || lineEdit_pinyin->text().isEmpty()
         || lineEdit_head->text().isEmpty()
         || lineEdit_name->text().isEmpty()
         || lineEdit_header->text().isEmpty()
         || lineEdit_words->text().isEmpty()
         )
        return true;

    if(comboBox_sex->currentText().isEmpty()
       || comboBox_stars->currentText().isEmpty()
       || comboBox_nation->currentText().isEmpty()
       || comboBox_enabled->currentText().isEmpty()

       )
       return true;

    return false;
}

bool SingerInfoWdiget::isRequiredOption()
{
    if ( lineEdit_serial_id->text().isEmpty()
         || lineEdit_pinyin->text().isEmpty()
         || lineEdit_head->text().isEmpty()
         || lineEdit_name->text().isEmpty()
         || lineEdit_header->text().isEmpty()
         || lineEdit_words->text().isEmpty()
         )
        return true;

    if(comboBox_sex->currentText().isEmpty()
       || comboBox_stars->currentText().isEmpty()
       || comboBox_nation->currentText().isEmpty()
       || comboBox_enabled->currentText().isEmpty()

       )
       return true;

    return false;
}

void SingerInfoWdiget::set_enabled(bool status)
{
    lineEdit_serial_id->setEnabled(status);
    lineEdit_pinyin->setEnabled(status);
    lineEdit_head->setEnabled(status);
    lineEdit_count->setEnabled(status);

    lineEdit_name->setEnabled(status); //lineEdit_name->setDisabled();
    lineEdit_header->setEnabled(status);
    lineEdit_words->setEnabled(status);
    lineEdit_info->setEnabled(status);


    comboBox_sex->setEnabled(status);
    comboBox_stars->setEnabled(status);
    comboBox_nation->setEnabled(status);
    comboBox_enabled->setEnabled(status);

    pushButton_upload->setHidden(!status);
}

void SingerInfoWdiget::getActorValue(Actor &_actor)
{
#ifndef YQC_TECH
        _actor.serial_id = "";
        _actor.sid = lineEdit_serial_id->text();
#else
        _actor.serial_id = lineEdit_serial_id->text();
#endif

    _actor.pinyin = lineEdit_pinyin->text();
    _actor.head = lineEdit_head->text();
    _actor.count = lineEdit_count->text();

    _actor.name = lineEdit_name->text();
    _actor.header = lineEdit_header->text();
    _actor.words = lineEdit_words->text();
    _actor.info = lineEdit_info->text();

    _actor.sex = QString::number( sex.key(comboBox_sex->currentText(), 1));
    _actor.nation = QString::number(nation.key(comboBox_nation->currentText(), 1));
    _actor.enabled = QString::number(enabled.key(comboBox_enabled->currentText(), 1));

    _actor.stars = QString::number(comboBox_stars->currentText().toFloat());
    //    _actor.stars = comboBox_stars->currentText();
}

void SingerInfoWdiget::getActorOnlineValue(Actor &_actor)
{
#ifndef YQC_TECH
        _actor.serial_id = "";
        _actor.sid = lineEdit_serial_id->text();
#else
        _actor.serial_id = lineEdit_serial_id->text();
#endif

//    _actor.serial_id = lineEdit_serial_id->text();
    _actor.pinyin = lineEdit_pinyin->text();
    _actor.head = lineEdit_head->text();
    _actor.count = lineEdit_count->text();

    _actor.name = lineEdit_name->text();
    _actor.header = lineEdit_header->text();
    _actor.words = lineEdit_words->text();
    _actor.info = lineEdit_info->text();

    _actor.sex = QString::number(sex.key(comboBox_sex->currentText()));
    _actor.nation = QString::number(nation.key(comboBox_nation->currentText()));
    _actor.enabled = QString::number(enabled.key(comboBox_enabled->currentText()));

    _actor.stars = comboBox_stars->currentText();
    _actor.image = lineEdit_image_online->text();
}

void SingerInfoWdiget::setActorValue(const Actor &_actor)
{
    actor = _actor;

#ifndef YQC_TECH
        lineEdit_serial_id->setText(_actor.sid);
#else
        lineEdit_serial_id->setText(_actor.serial_id);
#endif


    comboBox_sex->insertItem(0, sex.value(_actor.sex.toInt()));
    comboBox_stars->insertItem(0, _actor.stars);
    lineEdit_pinyin->setText(_actor.pinyin);
    lineEdit_head->setText(_actor.head);
    lineEdit_count->setText(_actor.count);

    lineEdit_name->setText(_actor.name);
    comboBox_nation->insertItem(0, nation.value(_actor.nation.toInt()));
    comboBox_enabled->insertItem(0, enabled.value(_actor.enabled.toInt()));
    lineEdit_header->setText(_actor.header);
    lineEdit_words->setText(_actor.words);
    lineEdit_info->setText(_actor.info);

    QFile file(_actor.image);
    if(!file.exists()){
        return;
    }

    if(!_actor.image.isEmpty()){

        QImage image;
        image.load(_actor.image);
        label_show_image->setPixmap(QPixmap::fromImage(image));
        QStringList strList = _actor.image.split("/");
        lineEdit_image->setText(strList.last());
        pushButton_upload->setText("修改");
    }
}

void SingerInfoWdiget::setActorValue_online(const Actor &_actor)
{
//    actor = _actor;
    QString temp;

#ifndef YQC_TECH
        lineEdit_serial_id->setText(_actor.sid);
#else
        lineEdit_serial_id->setText(_actor.serial_id);
#endif

    lineEdit_serial_id->setText(_actor.serial_id);
    temp = _actor.sex.isEmpty()? "": sex.value(_actor.sex.toInt());
    comboBox_sex->setCurrentIndex(sex.key(temp)); //insertItem(0, value.sex.value(_actor.sex.toInt()));

    comboBox_stars->setCurrentIndex(stars.key(_actor.stars)); //insertItem(0, _actor.stars);
    lineEdit_pinyin->setText(_actor.pinyin);
    lineEdit_head->setText(_actor.head);
    lineEdit_count->setText(_actor.count);

    lineEdit_name->setText(_actor.name);
    temp = _actor.nation.isEmpty()?"": nation.value(_actor.nation.toInt());
    comboBox_nation->setCurrentIndex(nation.key(temp)); //insertItem(0, value.nation.value(_actor.nation.toInt()));
    temp = _actor.enabled.isEmpty()?"":enabled.value(_actor.enabled.toInt());
    comboBox_enabled->setCurrentIndex(enabled.key(temp)); //insertItem(0, value.enabled.value(_actor.enabled.toInt()));
    lineEdit_header->setText(_actor.header);
    lineEdit_words->setText(_actor.words);
    lineEdit_info->setText(_actor.info);

    QFile file(_actor.image);
    if(!file.exists()){
        return;
    }

    if(!_actor.image.isEmpty()){

        QImage image;
        image.load(_actor.image);
        label_show_image->setPixmap(QPixmap::fromImage(image));
        QStringList strList = _actor.image.split("/");
        lineEdit_image->setText(strList.last());
        pushButton_upload->setText("修改");
    }
}

QString SingerInfoWdiget::getImagePath()
{
    return lineEdit_image->text();
}

void SingerInfoWdiget::set_comboBox_sex(QMap<int, QString> &_sex)
{
    QStringList ss(_sex.values());
    sex = _sex;
    comboBox_sex->clear();
    comboBox_sex->addItems(ss);
    comboBox_sex->insertItem(0, "");
    comboBox_sex->setCurrentIndex(0);
}

void SingerInfoWdiget::set_comboBox_nation(QMap<int, QString> &_nat)
{
    QStringList nn(_nat.values());
    nation = _nat;
    comboBox_nation->clear();
    comboBox_nation->addItems(nn);
    comboBox_nation->insertItem(0, "");
    comboBox_nation->setCurrentIndex(0);
}

void SingerInfoWdiget::set_comboBox_Remain()
{
    QStringList ss(stars.values());
    comboBox_stars->clear();
    comboBox_stars->addItems(ss);
    comboBox_stars->setCurrentIndex(comboBox_stars->count()-1);

    QStringList ee(enabled.values());
    comboBox_enabled->clear();
    comboBox_enabled->addItems(ee);
    comboBox_enabled->setCurrentIndex(1);
}

void SingerInfoWdiget::setImageOnlineRetPath(const QString &path)
{
    lineEdit_image_online->setText(path);
}

void SingerInfoWdiget::setPlaceholder_serial_id(qint64 &serial_id)
{
//    QString str = QString("最大serial_id:%1").arg(serial_id);
//    lineEdit_serial_id->setPlaceholderText(QString::number(str));
    serial_id++;
    lineEdit_serial_id->setText(QString::number(serial_id));
}

void SingerInfoWdiget::set_tooptip_image()
{
    QImage image;
    image.load(":/other/images/required.png");

    label_serial_id_tooptip->setPixmap(QPixmap::fromImage(image));
    label_sex_tooptip->setPixmap(QPixmap::fromImage(image));
    label_stars_tooptip->setPixmap(QPixmap::fromImage(image));
    label_pinyin_tooptip->setPixmap(QPixmap::fromImage(image));
    label_head_tooptip->setPixmap(QPixmap::fromImage(image));

    label_name_tooptip->setPixmap(QPixmap::fromImage(image));
    label_nation_tooptip->setPixmap(QPixmap::fromImage(image));
    label_enabled_tooptip->setPixmap(QPixmap::fromImage(image));
    label_header_tooptip->setPixmap(QPixmap::fromImage(image));
    label_words_tooptip->setPixmap(QPixmap::fromImage(image));
    //    label_image_tooptip->setPixmap(QPixmap::fromImage(image));
}

void SingerInfoWdiget::setOldActorValue(Actor _actor)
{
    actor = _actor;
}

void SingerInfoWdiget::setComboBoxValue(QComboBox *combobox, QList<QString> list)
{
    QStringList items(list);
    if(items.isEmpty())
        return;
    QString currentStr = combobox->currentText();
    int index = list.indexOf(currentStr);

    combobox->clear();
    combobox->insertItems(0, items);
    combobox->setCurrentIndex(index);
}

void SingerInfoWdiget::setSexValue()
{
    setComboBoxValue(comboBox_sex, sex.values());
}

void SingerInfoWdiget::setNationValue()
{
    setComboBoxValue(comboBox_nation, nation.values());
}

void SingerInfoWdiget::setStarsValue()
{
    setComboBoxValue(comboBox_stars, stars.values());
}

void SingerInfoWdiget::setEnabledValue()
{
   setComboBoxValue(comboBox_enabled, enabled.values());
}

void SingerInfoWdiget::uploadImage()
{
    QString fileFormat("图片文件(*.jpg)");
    QString document = QProcessEnvironment::systemEnvironment().value("USERPROFILE")+"\\Desktop";
    QString pathStr = QFileDialog::getOpenFileName(this,
                                                tr("上传头像"),
                                                document,
                                                fileFormat
                                                );
    if(pathStr.isEmpty())
        return;

    QImage image;
    image.load(pathStr);
    label_show_image->setPixmap(QPixmap::fromImage(image));
    lineEdit_image->setText(pathStr);
}

void SingerInfoWdiget::uploadImageOnline()
{
    QString path = lineEdit_image_online->text();
    QString retPath;
    if(!path.isEmpty())
    {
        CurlUpload *curlUpload = new CurlUpload();
        retPath = curlUpload->uploadActorImage(path, NULL);
        lineEdit_image_online->setText(retPath);

        delete curlUpload;
    }
}

void SingerInfoWdiget::previewOnline()
{
    QString fileFormat("图片文件(*.jpg)");
    QString document = QProcessEnvironment::systemEnvironment().value("USERPROFILE")+"\\Desktop";
    QString pathStr = QFileDialog::getOpenFileName(this,
                                                tr("上传头像"),
                                                document,
                                                fileFormat
                                                );
    if(pathStr.isEmpty())
        return;

    QImage image;
    image.load(pathStr);
    label_show_image->setPixmap(QPixmap::fromImage(image));
    lineEdit_image_online->setText(pathStr);
}

void SingerInfoWdiget::setWords()
{
    QString name = lineEdit_name->text();
    if(name.isEmpty())
        return;
    lineEdit_words->setText(QString::number(name.size()));

    QString pinyin = PinyinAll::getInstance()->toQPinyin(name);
    QStringList singles = pinyin.split("_");

    QString head, header, py;
    py = pinyin;
    py.replace("_", "");
    for(int i=0; i<singles.size(); i++)
    {
        if(singles.at(0).isEmpty())
            continue;
        if(i == 0)
        {
            head = singles.at(0).at(0);
            header = head;
        }
        else
        {
            header.append(singles.at(i).at(0));
        }
    }

    lineEdit_head->setText(head.toUpper());
    lineEdit_header->setText(header.toUpper());
    lineEdit_pinyin->setText(py);


}

void SingerInfoWdiget::name_editFinish()
{
    if(actor.name.compare(lineEdit_name->text()) == 0)
        lineEdit_name->setStyleSheet(border_yellow);
    else
        lineEdit_name->setStyleSheet(border);
}

void SingerInfoWdiget::serial_id_editFinish()
{
    if(actor.serial_id.compare(lineEdit_serial_id->text()) == 0)
        lineEdit_serial_id->setStyleSheet(border_yellow);
    else
        lineEdit_serial_id->setStyleSheet(border);
}

void SingerInfoWdiget::pinyin_editFinish()
{
    if(actor.pinyin.compare(lineEdit_pinyin->text()) == 0)
        lineEdit_pinyin->setStyleSheet(border_yellow);
    else
        lineEdit_pinyin->setStyleSheet(border);
}

void SingerInfoWdiget::header_editFinish()
{
    if(actor.header.compare(lineEdit_header->text()) == 0)
        lineEdit_header->setStyleSheet(border_yellow);
    else
        lineEdit_header->setStyleSheet(border);
}

void SingerInfoWdiget::head_editFinish()
{
    if(actor.head.compare(lineEdit_head->text()) == 0)
        lineEdit_head->setStyleSheet(border_yellow);
    else
        lineEdit_head->setStyleSheet(border);
}

void SingerInfoWdiget::words_editFinish()
{
    if(actor.words.compare(lineEdit_words->text()) == 0)
        lineEdit_words->setStyleSheet(border_yellow);
    else
        lineEdit_words->setStyleSheet(border);
}

void SingerInfoWdiget::info_editFinish()
{
    if(actor.info.compare(lineEdit_info->text()) == 0)
        lineEdit_info->setStyleSheet(border_yellow);
    else
        lineEdit_info_online->setStyleSheet(border);
}

void SingerInfoWdiget::image_editFinish()
{
    if(actor.image.compare(lineEdit_image->text()) == 0)
        lineEdit_image_online->setStyleSheet(border_yellow);
    else
        lineEdit_image_online->setStyleSheet(border);
}

void SingerInfoWdiget::enabled_activated(const QString &val )
{
    int index = actor.enabled.toInt();
    if(index == enabled.key(val, 0))
        comboBox_enabled->setStyleSheet(border_yellow);
    else
        comboBox_enabled->setStyleSheet(border);
}

void SingerInfoWdiget::nation_activated(const QString &val)
{
   int index = actor.nation.toInt();
   if(index == nation.key(val, 0))
       comboBox_nation->setStyleSheet(border_yellow);
   else
       comboBox_nation->setStyleSheet(border);
}

void SingerInfoWdiget::sex_activated(const QString &val)
{
    int index = actor.sex.toInt();
    if(index == sex.key(val, 0))
        comboBox_sex->setStyleSheet(border_yellow);
    else
        comboBox_sex->setStyleSheet(border);
}

void SingerInfoWdiget::stars_activated(const QString &val)
{
    int index = actor.stars.toInt();
    if(index == stars.key(val, 0))
        comboBox_stars->setStyleSheet(border_yellow);
    else
        comboBox_stars->setStyleSheet(border);
}

void SingerInfoWdiget::setDefaultBorder()
{
    QString  border = QString("border: 1px solid rgb(201, 201, 201)");

    comboBox_stars->setStyleSheet(border);
    comboBox_sex->setStyleSheet(border);
    comboBox_nation->setStyleSheet(border);
    comboBox_enabled->setStyleSheet(border);

    lineEdit_image_online->setStyleSheet(border);
    lineEdit_info_online->setStyleSheet(border);
    lineEdit_words->setStyleSheet(border);
    lineEdit_head->setStyleSheet(border);
    lineEdit_header->setStyleSheet(border);
    lineEdit_pinyin->setStyleSheet(border);
    lineEdit_serial_id->setStyleSheet(border);
    lineEdit_name->setStyleSheet(border);
}

void SingerInfoWdiget::setDefaultValue()
{
    comboBox_stars->setCurrentIndex(comboBox_stars->count() -1 );
    comboBox_sex->setCurrentIndex(0);
    comboBox_nation->setCurrentIndex(0);
    comboBox_enabled->setCurrentIndex(1);

    lineEdit_image_online->clear();
    lineEdit_info_online->clear();
    lineEdit_words->clear();
    lineEdit_head->clear();
    lineEdit_header->clear();
    lineEdit_pinyin->clear();
    lineEdit_serial_id->clear();
    lineEdit_name->clear();

    label_show_image->clear();
}
