#include "songinfowidget.h"
#include "curlupload.h"
#include "pinyinall.h"
#include "singeraddmodifydialog.h"
#include "releasetech.h"
#include <QFile>
#include <QLabel>
#include <QPainter>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QFileDialog>
#include <QCalendarWidget>
#include <QProcessEnvironment>
#include <QCursor>
#include <QListView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>

#include <QDebug>
#define  MaxItems 5

SongInfoWidget::SongInfoWidget(QWidget *parent)
    : QWidget(parent)
{
    initWidget();
    readAndSetStyleSheet();
    setWidgetSize();
    initWidgetValue();
    isRequired();

    match.insert(1, "是");
    match.insert(0, "否");

    enabled.insert(1, "启用");
    enabled.insert(0, "禁用");

    track.insert(0, "0");
    track.insert(1, "1");

    connect(PushButton_question, &QPushButton::pressed, this, &SongInfoWidget::PushButton_question_pressed);
    connect(PushButton_question, &QPushButton::released, this, &SongInfoWidget::PushButton_question_released);
    connect(pushButton_calendar, &QPushButton::clicked, this, &SongInfoWidget::getCalendarDate);
    connect(calendar, &QCalendarWidget::selectionChanged, this, &SongInfoWidget::setDateTime);

    connect(pushButton_addSinger, &QPushButton::clicked, this, &SongInfoWidget::addSinger);
    connect(pushButton_upload_video, &QPushButton::clicked, this, &SongInfoWidget::uploadVideo);
    connect(pushButton_upload_lyric, &QPushButton::clicked, this, &SongInfoWidget::uploadLyric);



    border_red = QString("border: 1px solid rgb(255, 0, 0)");
    border_yellow = QString("border: 1px solid rgb(0, 0, 255)");
    connect(lineEdit_name, &QLineEdit::editingFinished, this, &SongInfoWidget::setWords);
    connect(lineEdit_name, &QLineEdit::editingFinished, this, &SongInfoWidget::name_ef);
    connect(lineEdit_serial_id, &QLineEdit::editingFinished, this, &SongInfoWidget::serial_id_ef);
    connect(lineEdit_upload_video, &QLineEdit::editingFinished, this, &SongInfoWidget::video_ef);
    connect(lineEdit_pinyin, &QLineEdit::editingFinished, this, &SongInfoWidget::pinyin_ef);
    connect(lineEdit_head, &QLineEdit::editingFinished, this, &SongInfoWidget::head_ef);
    connect(lineEdit_lyric, &QLineEdit::editingFinished, this, &SongInfoWidget::lyric_ef);
    connect(lineEdit_header, &QLineEdit::editingFinished, this, &SongInfoWidget::header_ef);
    connect(lineEdit_words, &QLineEdit::editingFinished, this, &SongInfoWidget::words_ef);
    connect(lineEdit_prelude, &QLineEdit::editingFinished, this, &SongInfoWidget::prelude_ef);
    connect(lineEdit_addSinger, &QLineEdit::editingFinished, this, &SongInfoWidget::singer_ef);
    connect(lineEdit_info, &QLineEdit::editingFinished, this, &SongInfoWidget::info_ef);
    connect(lineEdit_create_time, &QLineEdit::editingFinished, this, &SongInfoWidget::time_ef);

    connect(comboBox_language, SIGNAL(activated(QString)), this, SLOT(language_act(QString)));
    connect(comboBox_version, SIGNAL(activated(QString)), this, SLOT(version_act(QString)));
    connect(comboBox_source, SIGNAL(activated(QString)), this, SLOT(source_act(QString)));
    connect(comboBox_type, SIGNAL(activated(QString)), this, SLOT(type_act(QString)));
    connect(comboBox_resolution, SIGNAL(activated(QString)), this, SLOT(resolution_act(QString)));
    connect(comboBox_rthym, SIGNAL(activated(QString)), this, SLOT(rthym_act(QString)));
    connect(comboBox_match, SIGNAL(activated(QString)), this, SLOT(match_act(QString)));
    connect(comboBox_enabled, SIGNAL(activated(QString)), this, SLOT(enabled_act(QString)));
    connect(comboBox_original_track, SIGNAL(activated(QString)), this, SLOT(original_track_act(QString)));
    connect(comboBox_sound_track, SIGNAL(activated(QString)), this, SLOT(sound_track_act(QString)));
    connect(comboBox_quality, SIGNAL(activated(QString)), this, SLOT(quality_act(QString)));
    connect(comboBox_effect, SIGNAL(activated(QString)), this, SLOT(effect_act(QString)));
    connect(comboBox_pitch, SIGNAL(activated(QString)), this, SLOT(pitch_act(QString)));


    setTab_order();
}

SongInfoWidget::~SongInfoWidget()
{

}

void SongInfoWidget::setStrckCurrentIndex()
{
    connect(comboBox_sound_track, SIGNAL(currentIndexChanged(int)), this, SLOT(change_sound_track(int)));
    connect(comboBox_original_track, SIGNAL(currentIndexChanged(int)), this, SLOT(change_original_track(int)));
}

void SongInfoWidget::readAndSetStyleSheet()
{
    QFile qss(":/qss/songinfo.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());
    qss.close();

    this->setObjectName("Widget");    

    lineEdit_serial_id->setObjectName("LineEdit");
    lineEdit_upload_video->setObjectName("LineEdit");
    lineEdit_pinyin->setObjectName("LineEdit");
    lineEdit_head->setObjectName("LineEdit");
    lineEdit_create_time->setObjectName("LineEdit");

    lineEdit_name->setObjectName("LineEdit");
    lineEdit_lyric->setObjectName("LineEdit");
    lineEdit_header->setObjectName("LineEdit");
    lineEdit_words->setObjectName("LineEdit");
    lineEdit_prelude->setObjectName("LineEdit");

    lineEdit_addSinger->setObjectName("LineEdit");
    lineEdit_info->setObjectName("LineEdit");


    comboBox_language->setObjectName("Combobox");
    comboBox_version->setObjectName("Combobox");
    comboBox_source->setObjectName("Combobox");

    comboBox_type->setObjectName("Combobox");
    comboBox_resolution->setObjectName("Combobox");
    comboBox_rthym->setObjectName("Combobox");

    comboBox_match->setObjectName("Combobox");
    comboBox_enabled->setObjectName("Combobox");
    comboBox_original_track->setObjectName("Combobox");
    comboBox_sound_track->setObjectName("Combobox");
    comboBox_quality->setObjectName("Combobox");
    comboBox_effect->setObjectName("Combobox");
    comboBox_pitch->setObjectName("Combobox");

    comboBox_attribute01->setObjectName("Combobox");
    comboBox_attribute02->setObjectName("Combobox");
    comboBox_attribute03->setObjectName("Combobox");
    comboBox_attribute04->setObjectName("Combobox");
    comboBox_attribute05->setObjectName("Combobox");
    comboBox_attribute06->setObjectName("Combobox");

    pushButton_calendar->setObjectName("CalendarButton");
    pushButton_addSinger->setObjectName("Button");
    pushButton_upload_video->setObjectName("Button");
    pushButton_upload_lyric->setObjectName("Button");
    PushButton_question->setObjectName("AnnotationButton");
    PushButton_attribute->setObjectName("PlusButton");
}

void SongInfoWidget::initWidget()
{
    widgetLeft = new QWidget(this);
    widgetCenter = new QWidget(this);
    widgetRight = new QWidget(this);
    widgetTrack = new QWidget(widgetRight);        

    calendar = new QCalendarWidget(this);
    calendar->resize(280, 180);

    label_serial_id = new QLabel(widgetLeft);
    label_upload_video = new QLabel(widgetLeft);
    label_language = new QLabel(widgetLeft);
    label_version = new QLabel(widgetLeft);
    label_pinyin = new QLabel(widgetLeft);
    label_head = new QLabel(widgetLeft);
    label_source = new QLabel(widgetLeft);
    label_create_time = new QLabel(widgetLeft);

    label_name = new QLabel(widgetCenter);
    label_lyric = new QLabel(widgetCenter);
    label_type = new QLabel(widgetCenter);
    label_resolution = new QLabel(widgetCenter);
    label_header = new QLabel(widgetCenter);
    label_words = new QLabel(widgetCenter);
    label_prelude = new QLabel(widgetCenter);
    label_prelude_unit = new QLabel(widgetCenter);
    label_rthym = new QLabel(widgetCenter);

    label_addSinger = new QLabel(widgetRight);
    label_match = new QLabel(widgetCenter);
    label_enabled = new QLabel(widgetCenter);
    label_track = new QLabel(widgetCenter);
    label_original_track = new QLabel(widgetCenter);
    label_sound_track = new QLabel(widgetCenter);
    label_quality = new QLabel(widgetCenter);
    label_effect = new QLabel(widgetCenter);
    label_info = new QLabel(widgetCenter);
    label_pitch = new QLabel(widgetCenter);

    label_attribute = new QLabel(this);

    label_serial_id_tooptip = new QLabel(widgetLeft);
    label_upload_video_tooptip = new QLabel(widgetLeft);
    label_language_tooptip = new QLabel(widgetLeft);
    label_version_tooptip = new QLabel(widgetLeft);
    label_pinyin_tooptip = new QLabel(widgetLeft);
    label_head_tooptip = new QLabel(widgetLeft);
    label_source_tooptip = new QLabel(widgetLeft);
    label_create_time_tooptip = new QLabel(widgetLeft);

    label_name_tooptip = new QLabel(widgetCenter);
    label_lyric_tooptip = new QLabel(widgetCenter);
    label_type_tooptip = new QLabel(widgetCenter);
    label_resolution_tooptip = new QLabel(widgetCenter);
    label_header_tooptip = new QLabel(widgetCenter);
    label_words_tooptip = new QLabel(widgetCenter);
    label_prelude_tooptip = new QLabel(widgetCenter);
    label_rthym_tooptip = new QLabel(widgetCenter);

    label_addSinger_tooptip = new QLabel(widgetRight);
    label_match_tooptip = new QLabel(widgetRight);
    label_enabled_tooptip = new QLabel(widgetRight);
    label_track_tooptip = new QLabel(widgetRight);
    label_quality_tooptip = new QLabel(widgetRight);
    label_effect_tooptip = new QLabel(widgetRight);
    label_info_tooptip = new QLabel(widgetRight);
    label_pitch_tooptip = new QLabel(widgetRight);

    label_attribute_tooptip = new QLabel(this);

    lineEdit_serial_id = new QLineEdit(widgetLeft);
    lineEdit_upload_video = new QLineEdit(widgetLeft);
    lineEdit_pinyin = new QLineEdit(widgetLeft);
    lineEdit_head = new QLineEdit(widgetLeft);
    lineEdit_create_time = new QLineEdit(widgetLeft);

    lineEdit_name = new QLineEdit(widgetCenter);
    lineEdit_lyric = new QLineEdit(widgetCenter);
    lineEdit_header = new QLineEdit(widgetCenter);
    lineEdit_words = new QLineEdit(widgetCenter);
    lineEdit_prelude = new QLineEdit(widgetCenter);

    lineEdit_addSinger = new QLineEdit(widgetRight);
    lineEdit_info = new QLineEdit(widgetRight);

    comboBox_language = new QComboBox(widgetLeft);
    comboBox_version = new QComboBox(widgetLeft);
    comboBox_source = new QComboBox(widgetLeft);

    QListView *view_lan = new QListView();
    QListView *view_ver = new QListView();
    QListView *view_sou = new QListView();
    comboBox_language->setView(view_lan);
    comboBox_version->setView(view_ver);
    comboBox_source->setView(view_sou);
    comboBox_language->setMaxVisibleItems(MaxItems);
    comboBox_version->setMaxVisibleItems(MaxItems);
    comboBox_source->setMaxVisibleItems(MaxItems);


    comboBox_type = new QComboBox(widgetCenter);
    comboBox_resolution = new QComboBox(widgetCenter);
    comboBox_rthym = new QComboBox(widgetCenter);

    QListView *view_typ = new QListView();
    QListView *view_res = new QListView();
    QListView *view_rth = new QListView();
    comboBox_type->setView(view_typ);
    comboBox_resolution->setView(view_res);
    comboBox_rthym->setView(view_rth);
    comboBox_type->setMaxVisibleItems(MaxItems);
    comboBox_resolution->setMaxVisibleItems(MaxItems);
    comboBox_rthym->setMaxVisibleItems(MaxItems);


    comboBox_match = new QComboBox(widgetRight);
    comboBox_enabled = new QComboBox(widgetRight);
    comboBox_original_track = new QComboBox(widgetRight);
    comboBox_sound_track = new QComboBox(widgetRight);
    comboBox_quality = new QComboBox(widgetRight);
    comboBox_effect = new QComboBox(widgetRight);
    comboBox_pitch = new QComboBox(widgetRight);

    QListView *view_mat = new QListView();
    QListView *view_ena = new QListView();
    QListView *view_ori = new QListView();
    QListView *view_sound = new QListView();
    QListView *view_qua = new QListView();
    QListView *view_eff = new QListView();
    QListView *view_pit = new QListView();
    comboBox_match->setView(view_mat);
    comboBox_enabled->setView(view_ena);
    comboBox_original_track->setView(view_ori);
    comboBox_sound_track->setView(view_sound);
    comboBox_quality->setView(view_qua);
    comboBox_effect->setView(view_eff);
    comboBox_pitch->setView(view_pit);

    comboBox_match->setMaxVisibleItems(MaxItems);
    comboBox_enabled->setMaxVisibleItems(MaxItems);
    comboBox_original_track->setMaxVisibleItems(MaxItems);
    comboBox_sound_track->setMaxVisibleItems(MaxItems);
    comboBox_quality->setMaxVisibleItems(MaxItems);
    comboBox_effect->setMaxVisibleItems(MaxItems);
    comboBox_pitch->setMaxVisibleItems(MaxItems);


    comboBox_attribute01 = new QComboBox(this);
    comboBox_attribute02 = new QComboBox(this);
    comboBox_attribute03 = new QComboBox(this);
    comboBox_attribute04 = new QComboBox(this);
    comboBox_attribute05 = new QComboBox(this);
    comboBox_attribute06 = new QComboBox(this);

    pushButton_calendar = new QPushButton(lineEdit_create_time);
    pushButton_addSinger = new QPushButton(widgetRight);
    pushButton_upload_video = new QPushButton(widgetLeft);
    pushButton_upload_lyric = new QPushButton(widgetCenter);
    PushButton_question = new QPushButton(widgetRight);
    PushButton_attribute = new QPushButton(this);


    serial_id_layout = new QHBoxLayout();
    upload_video_pushbutton_layout = new QHBoxLayout();
    upload_video_layout = new QHBoxLayout();
    language_layout = new QHBoxLayout();
    version_layout = new QHBoxLayout();
    pinyin_layout = new QHBoxLayout();
    head_layout = new QHBoxLayout();
    source_layout = new QHBoxLayout();
    create_time_layout = new QHBoxLayout();
    leftLayout = new QVBoxLayout();

    serial_id_layout->addWidget(label_serial_id_tooptip);
    serial_id_layout->addWidget(label_serial_id);
    serial_id_layout->addWidget(lineEdit_serial_id);
    serial_id_layout->setContentsMargins(0, 0, 0, 0);
    serial_id_layout->setSpacing(0);

    upload_video_pushbutton_layout->addWidget(lineEdit_upload_video);
    upload_video_pushbutton_layout->addWidget(pushButton_upload_video);
    upload_video_pushbutton_layout->setContentsMargins(0, 0, 0, 0);
    upload_video_pushbutton_layout->setSpacing(10);
    upload_video_layout->addWidget(label_upload_video_tooptip);
    upload_video_layout->addWidget(label_upload_video);
    upload_video_layout->addLayout(upload_video_pushbutton_layout);
    upload_video_layout->addStretch();
    upload_video_layout->setContentsMargins(0, 0, 0, 0);
    upload_video_layout->setSpacing(0);

    language_layout->addWidget(label_language_tooptip);
    language_layout->addWidget(label_language);
    language_layout->addWidget(comboBox_language);
    language_layout->setContentsMargins(0, 0, 0, 0);
    language_layout->setSpacing(0);

    version_layout->addWidget(label_version_tooptip);
    version_layout->addWidget(label_version);
    version_layout->addWidget(comboBox_version);
    version_layout->setContentsMargins(0, 0, 0, 0);
    version_layout->setSpacing(0);

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

    source_layout->addWidget(label_source_tooptip);
    source_layout->addWidget(label_source);
    source_layout->addWidget(comboBox_source);
    source_layout->setContentsMargins(0, 0, 0, 0);
    source_layout->setSpacing(0);

    create_time_layout->addWidget(label_create_time_tooptip);
    create_time_layout->addWidget(label_create_time);
    create_time_layout->addWidget(lineEdit_create_time);
    create_time_layout->setContentsMargins(0, 0, 0, 0);
    create_time_layout->setSpacing(0);

    leftLayout->addLayout(serial_id_layout);
    leftLayout->addLayout(upload_video_layout);
    leftLayout->addLayout(language_layout);
    leftLayout->addLayout(version_layout);
    leftLayout->addLayout(pinyin_layout);
    leftLayout->addLayout(head_layout);
    leftLayout->addLayout(source_layout);
    leftLayout->addLayout(create_time_layout);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(10);
    widgetLeft->setLayout(leftLayout);



    name_layout = new QHBoxLayout();
    upload_lyric_pushbutton_layout = new QHBoxLayout();
    upload_lyric_layout = new QHBoxLayout();
    type_layout = new QHBoxLayout();
    resolution_layout = new QHBoxLayout();
    header_layout = new QHBoxLayout();
    words_layout = new QHBoxLayout();
    prelude_layout = new QHBoxLayout();
    prelude_unit_layout = new QHBoxLayout();
    rthym_layout = new QHBoxLayout();
    centerLayout = new QVBoxLayout();

    name_layout->addWidget(label_name_tooptip);
    name_layout->addWidget(label_name);
    name_layout->addWidget(lineEdit_name);
    name_layout->setContentsMargins(0, 0, 0, 0);
    name_layout->setSpacing(0);

    upload_lyric_pushbutton_layout->addWidget(lineEdit_lyric);
    upload_lyric_pushbutton_layout->addWidget(pushButton_upload_lyric);
    upload_lyric_pushbutton_layout->setContentsMargins(0, 0, 0, 0);
    upload_lyric_pushbutton_layout->setSpacing(10);
    upload_lyric_layout->addWidget(label_lyric_tooptip);
    upload_lyric_layout->addWidget(label_lyric);
    upload_lyric_layout->addLayout(upload_lyric_pushbutton_layout);
    upload_lyric_layout->addStretch();
    upload_lyric_layout->setContentsMargins(0, 0, 0, 0);
    upload_lyric_layout->setSpacing(0);

    type_layout->addWidget(label_type_tooptip);
    type_layout->addWidget(label_type);
    type_layout->addWidget(comboBox_type);
    type_layout->setContentsMargins(0, 0, 0, 0);
    type_layout->setSpacing(0);

    resolution_layout->addWidget(label_resolution_tooptip);
    resolution_layout->addWidget(label_resolution);
    resolution_layout->addWidget(comboBox_resolution);
    resolution_layout->setContentsMargins(0, 0, 0, 0);
    resolution_layout->setSpacing(0);

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

    prelude_unit_layout->addWidget(lineEdit_prelude);
    prelude_unit_layout->addWidget(label_prelude_unit);
    prelude_unit_layout->setContentsMargins(0, 0, 0, 0);
    prelude_unit_layout->setSpacing(10);
    prelude_layout->addWidget(label_prelude_tooptip);
    prelude_layout->addWidget(label_prelude);
    prelude_layout->addLayout(prelude_unit_layout);
    prelude_layout->addStretch();
    prelude_layout->setContentsMargins(0, 0, 0, 0);
    prelude_layout->setSpacing(0);

    rthym_layout->addWidget(label_rthym_tooptip);
    rthym_layout->addWidget(label_rthym);
    rthym_layout->addWidget(comboBox_rthym);
    rthym_layout->setContentsMargins(0, 0, 0, 0);
    rthym_layout->setSpacing(0);

    centerLayout->addLayout(name_layout);
    centerLayout->addLayout(upload_lyric_layout);
    centerLayout->addLayout(type_layout);
    centerLayout->addLayout(resolution_layout);
    centerLayout->addLayout(header_layout);
    centerLayout->addLayout(words_layout);
    centerLayout->addLayout(prelude_layout);
    centerLayout->addLayout(rthym_layout);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->setSpacing(10);
    widgetCenter->setLayout(centerLayout);


    addSinger_pushbutton_layout = new QHBoxLayout();
    addSinger_layout = new QHBoxLayout();
    match_layout = new QHBoxLayout();
    enabled_layout = new QHBoxLayout();
    track_layout = new QHBoxLayout();
    track_help_layout = new QHBoxLayout();
    original_track_layout = new QHBoxLayout();
    sound_track_layout = new QHBoxLayout();
    quality_layout = new QHBoxLayout();
    effect_layout = new QHBoxLayout();
    info_layout = new QHBoxLayout();
    pitch_layout = new QHBoxLayout();
    rightLayout = new QVBoxLayout();

    addSinger_pushbutton_layout->addWidget(pushButton_addSinger);
    addSinger_pushbutton_layout->addWidget(lineEdit_addSinger);
    addSinger_pushbutton_layout->setContentsMargins(0, 0, 0, 0);
    addSinger_pushbutton_layout->setSpacing(10);
    addSinger_layout->addWidget(label_addSinger_tooptip);
    addSinger_layout->addWidget(label_addSinger);
    addSinger_layout->addLayout(addSinger_pushbutton_layout);
    addSinger_layout->addStretch();
    addSinger_layout->setContentsMargins(0, 0, 0, 0);
    addSinger_layout->setSpacing(0);

    match_layout->addWidget(label_match_tooptip);
    match_layout->addWidget(label_match);
    match_layout->addWidget(comboBox_match);
    match_layout->setContentsMargins(0, 0, 0, 0);
    match_layout->setSpacing(0);

    enabled_layout->addWidget(label_enabled_tooptip);
    enabled_layout->addWidget(label_enabled);
    enabled_layout->addWidget(comboBox_enabled);
    enabled_layout->setContentsMargins(0, 0, 0, 0);
    enabled_layout->setSpacing(0);

    original_track_layout->addWidget(label_original_track);
    original_track_layout->addWidget(comboBox_original_track);
    original_track_layout->setContentsMargins(0, 0, 0, 0);
    original_track_layout->setSpacing(8);
    sound_track_layout->addWidget(label_sound_track);
    sound_track_layout->addWidget(comboBox_sound_track);
    sound_track_layout->setContentsMargins(0, 0, 0, 0);
    sound_track_layout->setSpacing(8);
    track_help_layout->addLayout(original_track_layout);
    track_help_layout->addLayout(sound_track_layout);
    track_help_layout->addWidget(PushButton_question);    
    track_help_layout->setContentsMargins(0, 0, 0, 0);
    track_help_layout->setSpacing(10);
    widgetTrack->setLayout(track_help_layout);

    track_layout->addWidget(label_track_tooptip);
    track_layout->addWidget(label_track);
    track_layout->addWidget(widgetTrack);
    track_layout->addStretch();
    track_layout->setContentsMargins(0, 0, 0, 0);
    track_layout->setSpacing(0);


    quality_layout->addWidget(label_quality_tooptip, 0, Qt::AlignLeft);
    quality_layout->addWidget(label_quality, 0, Qt::AlignLeft);
    quality_layout->addWidget(comboBox_quality);
    quality_layout->setContentsMargins(0, 0, 0, 0);
    quality_layout->setSpacing(0);

    effect_layout->addWidget(label_effect_tooptip);
    effect_layout->addWidget(label_effect);
    effect_layout->addWidget(comboBox_effect);
    effect_layout->setContentsMargins(0, 0, 0, 0);
    effect_layout->setSpacing(0);

    info_layout->addWidget(label_info_tooptip);
    info_layout->addWidget(label_info);
    info_layout->addWidget(lineEdit_info);
    info_layout->setContentsMargins(0, 0, 0, 0);
    info_layout->setSpacing(0);

    pitch_layout->addWidget(label_pitch_tooptip);
    pitch_layout->addWidget(label_pitch);
    pitch_layout->addWidget(comboBox_pitch);
    pitch_layout->setContentsMargins(0, 0, 0, 0);
    pitch_layout->setSpacing(0);

    rightLayout->addLayout(addSinger_layout);
    rightLayout->addLayout(match_layout);
    rightLayout->addLayout(enabled_layout);
    rightLayout->addLayout(track_layout);
    rightLayout->addLayout(quality_layout);
    rightLayout->addLayout(effect_layout);
    rightLayout->addLayout(info_layout);
    rightLayout->addLayout(pitch_layout);

    widgetRight->setLayout(rightLayout);

    infoLayout = new QHBoxLayout();
    infoLayout->addWidget(widgetLeft);
    infoLayout->addWidget(widgetCenter);
    infoLayout->addWidget(widgetRight);
    infoLayout->addStretch();
    infoLayout->setContentsMargins(0, 0, 0, 0);
    infoLayout->setSpacing(36);

    attributeAddLayout = new QHBoxLayout();
    attributeLayout = new QHBoxLayout();
    attributeAddLayout->addWidget(comboBox_attribute01);
    attributeAddLayout->addWidget(comboBox_attribute02);
    attributeAddLayout->addWidget(comboBox_attribute03);
    attributeAddLayout->addWidget(comboBox_attribute04);
    attributeAddLayout->addWidget(comboBox_attribute05);
    attributeAddLayout->addWidget(comboBox_attribute06);
    attributeAddLayout->addWidget(PushButton_attribute);
    attributeAddLayout->addStretch();
    attributeAddLayout->setContentsMargins(0, 0, 0, 0);
    attributeAddLayout->setSpacing(10);

    attributeLayout->addWidget(label_attribute_tooptip);
    attributeLayout->addWidget(label_attribute);
    attributeLayout->addLayout(attributeAddLayout);
    attributeLayout->setContentsMargins(0, 0, 0, 0);
    attributeLayout->setSpacing(0);

    vLayout = new QVBoxLayout();
    vLayout->addLayout(infoLayout);
    vLayout->addLayout(attributeLayout);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(10);

    this->setLayout(vLayout);
}

void SongInfoWidget::initWidgetValue()
{
#ifndef YQC_TECH
        label_serial_id->setText("MID");
        label_serial_id->setEnabled(false);
        lineEdit_serial_id->setEnabled(false);
#else
        label_serial_id->setText("SERIAL_ID");
#endif

    label_upload_video->setText("上传视频");
    label_language->setText("歌曲语种");
    label_version->setText("版本类型");
    label_pinyin->setText("名称全拼");
    label_head->setText("名称第一个\n字首字母");
    label_source->setText("歌曲来源");
    label_create_time->setText("发布时间");


    label_name->setText("歌曲名称");
    label_lyric->setText("上传歌词");
    label_type->setText("歌曲分类");
    label_resolution->setText("分 辨 率");
    label_header->setText("名称首拼");
    label_words->setText("名称字数");
    label_prelude->setText("前奏时间");
    label_prelude_unit->setText("S");
    label_rthym->setText("歌曲节奏");



#ifndef YQC_TECH
    lineEdit_prelude->setPlaceholderText("输入歌词第一个字开始前5秒的数字");
    lineEdit_prelude->setStyleSheet("font-size: 14px;");
#else
#endif


    label_addSinger->setText("添加歌星");
    label_match->setText("是否K歌");
    label_enabled->setText("是否启用");
    label_track->setText("音    轨");
    label_original_track->setText("原唱");
    label_sound_track->setText("伴唱");
    label_quality->setText("音    质");
    label_effect->setText("音    效");
    label_info->setText("备    注");
    label_pitch->setText("音高类型");

    label_attribute->setText("标签属性");

    pushButton_upload_video->setText("上传");
    pushButton_upload_lyric->setText("上传");
    pushButton_addSinger->setText("选择");   
}

void SongInfoWidget::setWidgetSize()
{
    widgetTrack->setFixedWidth(250);    

    label_serial_id->setFixedWidth(80);
    label_upload_video->setFixedWidth(80);
    label_language->setFixedWidth(80);
    label_version->setFixedWidth(80);
    label_pinyin->setFixedWidth(80);
    label_head->setFixedWidth(80);
    label_source->setFixedWidth(80);
    label_create_time->setFixedWidth(80);

    label_name->setFixedWidth(80);
    label_lyric->setFixedWidth(80);
    label_type->setFixedWidth(80);
    label_resolution->setFixedWidth(80);
    label_header->setFixedWidth(80);
    label_words->setFixedWidth(80);
    label_prelude->setFixedWidth(80);
    label_prelude_unit->setFixedWidth(10);
    label_rthym->setFixedWidth(80);

    label_addSinger->setFixedWidth(80);
    label_match->setFixedWidth(80);
    label_enabled->setFixedWidth(80);
    label_track->setFixedWidth(80);
//    label_original_track->setFixedWidth(80);
//    label_sound_track->setFixedWidth(80);
    label_quality->setFixedWidth(80);
    label_effect->setFixedWidth(80);
    label_info->setFixedWidth(80);
    label_pitch->setFixedWidth(80);

    label_attribute->setFixedWidth(80);


    label_serial_id_tooptip->setFixedWidth(8);
    label_upload_video_tooptip->setFixedWidth(8);
    label_language_tooptip->setFixedWidth(8);
    label_version_tooptip->setFixedWidth(8);
    label_pinyin_tooptip->setFixedWidth(8);
    label_head_tooptip->setFixedWidth(8);
    label_source_tooptip->setFixedWidth(8);
    label_create_time_tooptip->setFixedWidth(8);

    label_name_tooptip->setFixedWidth(8);
    label_lyric_tooptip->setFixedWidth(8);
    label_type_tooptip->setFixedWidth(8);
    label_resolution_tooptip->setFixedWidth(8);
    label_header_tooptip->setFixedWidth(8);
    label_words_tooptip->setFixedWidth(8);
    label_prelude_tooptip->setFixedWidth(8);
    label_rthym_tooptip->setFixedWidth(8);

    label_addSinger_tooptip->setFixedWidth(8);
    label_match_tooptip->setFixedWidth(8);
    label_enabled_tooptip->setFixedWidth(8);
    label_track_tooptip->setFixedWidth(8);
    label_quality_tooptip->setFixedWidth(8);
    label_effect_tooptip->setFixedWidth(8);
    label_info_tooptip->setFixedWidth(8);
    label_pitch_tooptip->setFixedWidth(8);

    label_attribute_tooptip->setFixedWidth(8);


    lineEdit_serial_id->setFixedHeight(36);
    lineEdit_serial_id->setFixedWidth(250);
    lineEdit_upload_video->setFixedHeight(36);
    lineEdit_upload_video->setFixedWidth(180);
    lineEdit_pinyin->setFixedHeight(36);
    lineEdit_pinyin->setFixedWidth(250);
    lineEdit_head->setFixedHeight(36);
    lineEdit_head->setFixedWidth(250);
    lineEdit_create_time->setFixedHeight(36);
    lineEdit_create_time->setFixedWidth(250);

    lineEdit_name->setFixedHeight(36);
    lineEdit_name->setFixedWidth(250);
    lineEdit_lyric->setFixedHeight(36);
    lineEdit_lyric->setFixedWidth(180);
    lineEdit_header->setFixedHeight(36);
    lineEdit_header->setFixedWidth(250);
    lineEdit_words->setFixedHeight(36);
    lineEdit_words->setFixedWidth(250);
    lineEdit_prelude->setFixedHeight(36);
    lineEdit_prelude->setFixedWidth(230);
    lineEdit_addSinger->setFixedHeight(36);
    lineEdit_addSinger->setFixedWidth(180);
    lineEdit_info->setFixedHeight(36);
    lineEdit_info->setFixedWidth(250);


    comboBox_language->setFixedHeight(36);
    comboBox_language->setFixedWidth(250);
    comboBox_version->setFixedHeight(36);
    comboBox_version->setFixedWidth(250);
    comboBox_source->setFixedHeight(36);
    comboBox_source->setFixedWidth(250);

    comboBox_type->setFixedHeight(36);
    comboBox_type->setFixedWidth(250);
    comboBox_resolution->setFixedHeight(36);
    comboBox_resolution->setFixedWidth(250);
    comboBox_rthym->setFixedHeight(36);
    comboBox_rthym->setFixedWidth(250);

    comboBox_match->setFixedHeight(36);
    comboBox_match->setFixedWidth(250);
    comboBox_enabled->setFixedHeight(36);
    comboBox_enabled->setFixedWidth(250);
    comboBox_original_track->setFixedHeight(36);
    comboBox_original_track->setFixedWidth(60);
    comboBox_sound_track->setFixedHeight(36);
    comboBox_sound_track->setFixedWidth(60);
    comboBox_quality->setFixedHeight(36);
    comboBox_quality->setFixedWidth(250);
    comboBox_effect->setFixedHeight(36);
    comboBox_effect->setFixedWidth(250);
    comboBox_pitch->setFixedHeight(36);
    comboBox_pitch->setFixedWidth(250);

    comboBox_attribute01->setFixedHeight(36);
    comboBox_attribute01->setFixedWidth(120);
    comboBox_attribute02->setFixedHeight(36);
    comboBox_attribute02->setFixedWidth(120);
    comboBox_attribute03->setFixedHeight(36);
    comboBox_attribute03->setFixedWidth(120);
    comboBox_attribute04->setFixedHeight(36);
    comboBox_attribute04->setFixedWidth(120);
    comboBox_attribute05->setFixedHeight(36);
    comboBox_attribute05->setFixedWidth(120);
    comboBox_attribute06->setFixedHeight(36);
    comboBox_attribute06->setFixedWidth(120);


    pushButton_addSinger->setFixedHeight(36);
    pushButton_addSinger->setFixedWidth(60);
    pushButton_upload_video->setFixedHeight(36);
    pushButton_upload_video->setFixedWidth(60);
    pushButton_upload_lyric->setFixedHeight(36);
    pushButton_upload_lyric->setFixedWidth(60);
    PushButton_question->setFixedHeight(30);
    PushButton_question->setFixedWidth(30);
    PushButton_attribute->setFixedHeight(36);
    PushButton_attribute->setFixedWidth(36);

    QRect rect = lineEdit_create_time->geometry();
    lineEdit_create_time->setTextMargins(0, 0, 20, 0);
    pushButton_calendar->setGeometry(rect.width()-25, 10, 20, 20);
    pushButton_calendar->setCursor(Qt::PointingHandCursor);

    pushButton_calendar->setCheckable(true);
    calendar->setGeometry(this->x() + 180, this->y()+140, 280, 180);
    calendar->setHidden(true);
}

void SongInfoWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);   
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void SongInfoWidget::set_enabled(bool status)
{
    lineEdit_serial_id->setEnabled(status);
    lineEdit_upload_video->setEnabled(status);
    lineEdit_pinyin->setEnabled(status);
    lineEdit_head->setEnabled(status);
    lineEdit_create_time->setEnabled(status);

    lineEdit_name->setEnabled(status); //lineEdit_name->setDisabled();
    lineEdit_lyric->setEnabled(status);
    lineEdit_header->setEnabled(status);
    lineEdit_words->setEnabled(status);
    lineEdit_prelude->setEnabled(status);

    lineEdit_addSinger->setEnabled(status);
    lineEdit_info->setEnabled(status);

    comboBox_language->setEnabled(status);
    comboBox_version->setEnabled(status);
    comboBox_source->setEnabled(status);

    comboBox_type->setEnabled(status);
    comboBox_resolution->setEnabled(status);
    comboBox_rthym->setEnabled(status);

    comboBox_match->setEnabled(status);
    comboBox_enabled->setEnabled(status);
    comboBox_original_track->setEnabled(status);
    comboBox_sound_track->setEnabled(status);
    comboBox_quality->setEnabled(status);
    comboBox_effect->setEnabled(status);
    comboBox_pitch->setEnabled(status);

    comboBox_attribute01->setEnabled(status);
    comboBox_attribute02->setEnabled(status);
    comboBox_attribute03->setEnabled(status);
    comboBox_attribute04->setEnabled(status);
    comboBox_attribute05->setEnabled(status);
    comboBox_attribute06->setEnabled(status);


    pushButton_calendar->setHidden(!status);
    pushButton_addSinger->setHidden(!status);
    pushButton_upload_video->setHidden(!status);
    pushButton_upload_lyric->setHidden(!status);
    PushButton_question->setHidden(!status);
    PushButton_attribute->setHidden(!status);
}

void SongInfoWidget::isRequired()
{
    label_serial_id_tooptip->setObjectName("TooptipLabel");
    label_upload_video_tooptip->setObjectName("TooptipLabel");
    label_language_tooptip->setObjectName("TooptipLabel");
//    label_version_tooptip->setObjectName("TooptipLabel");
    label_pinyin_tooptip->setObjectName("TooptipLabel");
    label_head_tooptip->setObjectName("TooptipLabel");
//    label_source_tooptip->setObjectName("TooptipLabel");
//    label_create_time_tooptip->setObjectName("TooptipLabel");

    label_name_tooptip->setObjectName("TooptipLabel");
//    label_lyric_tooptip->setObjectName("TooptipLabel");
    label_type_tooptip->setObjectName("TooptipLabel");
//    label_resolution_tooptip->setObjectName("TooptipLabel");
    label_header_tooptip->setObjectName("TooptipLabel");
    label_words_tooptip->setObjectName("TooptipLabel");
//    label_prelude_tooptip->setObjectName("TooptipLabel");
//    label_rthym_tooptip->setObjectName("TooptipLabel");

    label_addSinger_tooptip->setObjectName("TooptipLabel");
    label_match_tooptip->setObjectName("TooptipLabel");
    label_enabled_tooptip->setObjectName("TooptipLabel");
    label_track_tooptip->setObjectName("TooptipLabel");
//    label_quality_tooptip->setObjectName("TooptipLabel");
//    label_effect_tooptip->setObjectName("TooptipLabel");
//    label_pitch_tooptip->setObjectName("TooptipLabel");

//    label_attribute_tooptip->setObjectName("TooptipLabel");
}

bool SongInfoWidget::isEmpty()
{
    if ( lineEdit_serial_id->text().isEmpty()
         || lineEdit_upload_video->text().isEmpty()
         || lineEdit_pinyin->text().isEmpty()
         || lineEdit_head->text().isEmpty()
         || lineEdit_create_time->text().isEmpty()
         || lineEdit_name->text().isEmpty()
         || lineEdit_lyric->text().isEmpty()
         || lineEdit_header->text().isEmpty()
         || lineEdit_words->text().isEmpty()
         || lineEdit_prelude->text().isEmpty()
         || lineEdit_addSinger->text().isEmpty()
         )
        return true;

    if(comboBox_language->currentText().isEmpty()
       || comboBox_version->currentText().isEmpty()
       || comboBox_source->currentText().isEmpty()
       || comboBox_type->currentText().isEmpty()
       || comboBox_resolution->currentText().isEmpty()
       || comboBox_rthym->currentText().isEmpty()
       || comboBox_match->currentText().isEmpty()
       || comboBox_enabled->currentText().isEmpty()
       || comboBox_original_track->currentText().isEmpty()
       || comboBox_sound_track->currentText().isEmpty()
       || comboBox_quality->currentText().isEmpty()
       || comboBox_effect->currentText().isEmpty()
       || comboBox_pitch->currentText().isEmpty()
       )
       return true;

    return false;
}

bool SongInfoWidget::isRequiredOption()
{
    if ( lineEdit_serial_id->text().isEmpty()
         || lineEdit_upload_video->text().isEmpty()
         || lineEdit_pinyin->text().isEmpty()
//         || lineEdit_head->text().isEmpty()
         || lineEdit_name->text().isEmpty()
         || lineEdit_header->text().isEmpty()
         || lineEdit_words->text().isEmpty()
         || lineEdit_addSinger->text().isEmpty()
         )
        return true;

    if(comboBox_language->currentText().isEmpty()
       || comboBox_type->currentText().isEmpty()
       || comboBox_enabled->currentText().isEmpty()
       || comboBox_original_track->currentText().isEmpty()
       || comboBox_sound_track->currentText().isEmpty()
       )
       return true;

    return false;
}

void SongInfoWidget::getMediaValue(Media &_media)
{
#ifndef YQC_TECH
        _media.mid = lineEdit_serial_id->text();
        _media.serial_id = "";
#else
        _media.serial_id = lineEdit_serial_id->text();
#endif

//    _media.serial_id = lineEdit_serial_id->text();
    _media.path = lineEdit_upload_video->text();
    _media.pinyin = lineEdit_pinyin->text();
    _media.head = lineEdit_head->text();
    _media.create_time = lineEdit_create_time->text();

    _media.name = lineEdit_name->text();
    _media.lyric = lineEdit_lyric->text();
    _media.header = lineEdit_header->text();
    _media.words = lineEdit_words->text();
    _media.prelude = lineEdit_prelude->text();

    _media.singer = lineEdit_addSinger->text();
    _media.info = lineEdit_info->text();


    int tempValue;
                     tempValue = language.key(comboBox_language->currentText(), 0);
    _media.language = tempValue == 0?"":QString::number(tempValue);
                     tempValue = version.key(comboBox_version->currentText(), 0);
    _media.version = tempValue == 0?"":QString::number(tempValue);
                     tempValue = source.key(comboBox_source->currentText(), 0);
    _media.source = tempValue == 0?"":QString::number(tempValue);

                    tempValue = type.key(comboBox_type->currentText(), 0);
    _media.type = tempValue == 0?"":QString::number(tempValue);
                    tempValue = resolution.key(comboBox_resolution->currentText(), 0);
    _media.resolution = tempValue == 0?"":QString::number(tempValue);
                    tempValue = rthym.key(comboBox_rthym->currentText(), 0);
    _media.rhythm = tempValue == 0?"":QString::number(tempValue);

    _media.match = QString::number( match.key(comboBox_match->currentText(), 0));
    _media.enabled = QString::number( enabled.key(comboBox_enabled->currentText(), 0));
    _media.original_track = QString::number(track.key(comboBox_original_track->currentText(), 0));
    _media.sound_track = QString::number(track.key(comboBox_sound_track->currentText(), 0));
                    tempValue = quality.key(comboBox_quality->currentText(), 0);
    _media.quality = tempValue == 0?"":QString::number(tempValue);
                    tempValue = effect.key(comboBox_effect->currentText(), 0);
    _media.effect = tempValue == 0?"":QString::number(tempValue);
                    tempValue = pitch.key(comboBox_pitch->currentText(), 0);
    _media.pitch = tempValue == 0?"":QString::number(tempValue);

    _media.artist_sid_1 = media.artist_sid_1;
    if(actor.size() >= 1)
        _media.artist_sid_2 = media.artist_sid_2;
    else
        _media.artist_sid_2 = "";

    if(lineEdit_info->text().isEmpty())
        _media.info  = "";
}

void SongInfoWidget::setMediaValue(const Media &_media)
{
    media = _media;
    QString temp;

#ifndef YQC_TECH
        lineEdit_serial_id->setText(_media.mid);
#else
        lineEdit_serial_id->setText(_media.serial_id);
#endif

//    lineEdit_serial_id->setText(_media.serial_id);
    lineEdit_upload_video->setText(_media.path);
    temp = _media.language.isEmpty()? "": language.value(_media.language.toInt());
    comboBox_language->insertItem(0, temp);
    temp = _media.version.isEmpty()? "" : temp = version.value(_media.version.toInt());
    comboBox_version->insertItem(0, temp);
    lineEdit_pinyin->setText(_media.pinyin);
    lineEdit_head->setText(_media.head);
    temp = _media.source.isEmpty()? QString(""): source.value(_media.source.toInt());
    comboBox_source->insertItem(0, temp);
    lineEdit_create_time->setText(_media.create_time);

    lineEdit_name->setText(_media.name);
    lineEdit_lyric->setText(_media.lyric);
    temp = _media.type.isEmpty()? "": type.value(_media.type.toInt());
    comboBox_type->insertItem(0, temp);
    temp = _media.resolution.isEmpty()? "": resolution.value(_media.resolution.toInt());
    comboBox_resolution->insertItem(0, temp);
    lineEdit_header->setText(_media.header);
    lineEdit_words->setText(_media.words);
    lineEdit_prelude->setText(_media.prelude);
    temp = _media.rhythm.isEmpty()? "": rthym.value(_media.rhythm.toInt());
    comboBox_rthym->insertItem(0, temp);

    lineEdit_addSinger->setText(_media.singer);
    temp = _media.match.isEmpty()? "": match.value(_media.match.toInt());
    comboBox_match->insertItem(0, temp);
    temp = _media.enabled.isEmpty()? "": enabled.value(_media.enabled.toInt());
    comboBox_enabled->insertItem(0, temp);
    qDebug() << " original : " << _media.original_track << " sound : " << _media.sound_track;
    comboBox_original_track->insertItem(0, _media.original_track); //setCurrentIndex(track.key(_media.original_track)); //
    comboBox_sound_track->insertItem(0, _media.sound_track); //setCurrentIndex(track.key(_media.sound_track));//
    temp = _media.quality.isEmpty()? "": quality.value(_media.quality.toInt());
    comboBox_quality->insertItem(0, temp);
    temp = _media.effect.isEmpty()? "": effect.value(_media.effect.toInt());
    comboBox_effect->insertItem(0, temp);
    lineEdit_info->setText(_media.info);
    temp = _media.pitch.isEmpty()? "": pitch.value(_media.pitch.toInt());
    comboBox_pitch->insertItem(0, temp);


//    comboBox_original_track->setCurrentIndex(0);
    //    for (int i=0; i<_media.musicTag.size(); i++)
}

void SongInfoWidget::setMediavalue_online(const Media &_media)
{
//    media = _media;
    QString temp;

#ifndef YQC_TECH
        lineEdit_serial_id->setText(_media.mid);
#else
        lineEdit_serial_id->setText(_media.serial_id);
#endif

//    lineEdit_serial_id->setText(_media.serial_id);
    lineEdit_upload_video->setText(_media.path);
    temp = _media.language.isEmpty()? "": language.value(_media.language.toInt());
    comboBox_language->setCurrentIndex(language.key(temp));
    temp = _media.version.isEmpty()? "" : temp = version.value(_media.version.toInt());
    comboBox_version->setCurrentIndex(version.key(temp)); //insertItem(0, temp);
    lineEdit_pinyin->setText(_media.pinyin);
    lineEdit_head->setText(_media.head);
    temp = _media.source.isEmpty()? QString(""): source.value(_media.source.toInt());
    comboBox_source->setCurrentIndex(source.key(temp)); //insertItem(0, temp);
    lineEdit_create_time->setText(_media.create_time);

    lineEdit_name->setText(_media.name);
    lineEdit_lyric->setText(_media.lyric);
    temp = _media.type.isEmpty()? "": type.value(_media.type.toInt());
    comboBox_type->setCurrentIndex(type.key(temp));  //insertItem(0, temp);
    temp = _media.resolution.isEmpty()? "": resolution.value(_media.resolution.toInt());
    comboBox_resolution->setCurrentIndex(resolution.key(temp)); //insertItem(0, temp);
    lineEdit_header->setText(_media.header);
    lineEdit_words->setText(_media.words);
    lineEdit_prelude->setText(_media.prelude);
    temp = _media.rhythm.isEmpty()? "": rthym.value(_media.rhythm.toInt());
    comboBox_rthym->setCurrentIndex(rthym.key(temp)); //insertItem(0, temp);

    lineEdit_addSinger->setText(_media.singer);
    temp = _media.match.isEmpty()? "": match.value(_media.match.toInt());
    comboBox_match->setCurrentIndex(match.key(temp)); //insertItem(0, temp);
    temp = _media.enabled.isEmpty()? "": enabled.value(_media.enabled.toInt());
    comboBox_enabled->setCurrentIndex(enabled.key(temp)); //insertItem(0, temp);

    comboBox_original_track->setCurrentIndex(track.key(_media.original_track)); //insertItem(0, _media.original_track);
    comboBox_sound_track->setCurrentIndex(track.key(_media.sound_track)); //insertItem(0, _media.sound_track);

    temp = _media.quality.isEmpty()? "": quality.value(_media.quality.toInt());
    comboBox_quality->setCurrentIndex(quality.key(temp)); //insertItem(0, temp);
    temp = _media.effect.isEmpty()? "": effect.value(_media.effect.toInt());
    comboBox_effect->setCurrentIndex(effect.key(temp)); //insertItem(0, temp);
    lineEdit_info->setText(_media.info);
    temp = _media.pitch.isEmpty()? "": pitch.value(_media.pitch.toInt());
    comboBox_pitch->setCurrentIndex(pitch.key(temp)); //insertItem(0, temp);
}

void SongInfoWidget::set_comboBox_Language(QMap<int, QString> &lan)
{
    QStringList ll(lan.values());
    language = lan;
    comboBox_language->clear();
    comboBox_language->addItems(ll);
    comboBox_language->insertItem(0, "");
    comboBox_language->setCurrentIndex(0);
}

void SongInfoWidget::set_comboBox_type(QMap<int, QString> &typ)
{
    QStringList tt(typ.values());
    type = typ;
    comboBox_type->clear();
    comboBox_type->addItems(tt);
    comboBox_type->insertItem(0, "");
    comboBox_type->setCurrentIndex(0);
}

void SongInfoWidget::set_comboBox_version(QMap<int, QString>&ver)
{
    QStringList vv(ver.values());
    version = ver ;
    comboBox_version->clear();
    comboBox_version->addItems(vv);
    comboBox_version->insertItem(0, "");
    comboBox_version->setCurrentIndex(0);
}

void SongInfoWidget::set_comboBox_effect(QMap<int, QString> &eff)
{
    QStringList ee(eff.values());
    effect = eff;
    comboBox_effect->clear();
    comboBox_effect->addItems(ee);
    comboBox_effect->insertItem(0, "");
    comboBox_effect->setCurrentIndex(0);
}

void SongInfoWidget::set_comboBox_resolution(QMap<int, QString> &res)
{
    QStringList rr(res.values());
    resolution = res;
    comboBox_resolution->clear();
    comboBox_resolution->addItems(rr);
    comboBox_resolution->insertItem(0, "");
    comboBox_resolution->setCurrentIndex(0);
}

void SongInfoWidget::set_comboBox_quality(QMap<int, QString> &qua)
{
    QStringList qq(qua.values());
    quality = qua;
    comboBox_quality->clear();
    comboBox_quality->addItems(qq);
    comboBox_quality->insertItem(0, "");
    comboBox_quality->setCurrentIndex(0);
}

void SongInfoWidget::set_comboBox_source(QMap<int, QString> &sou)
{
    QStringList ss(sou.values());
    source = sou;
    comboBox_source->clear();
    comboBox_source->addItems(ss);
    comboBox_source->insertItem(0, "");
    comboBox_source->setCurrentIndex(0);
}

void SongInfoWidget::set_comboBox_rthym(QMap<int, QString> &rth)
{
    QStringList aa(rth.values());
    rthym = rth;
    comboBox_rthym->clear();
    comboBox_rthym->addItems(aa);
    comboBox_rthym->insertItem(0, "");
    comboBox_rthym->setCurrentIndex(0);
}

void SongInfoWidget::set_comboBox_pitch(QMap<int, QString> &pit)
{
    QStringList pp(pit.values());
    pitch = pit;
    comboBox_pitch->clear();
    comboBox_pitch->addItems(pp);
    comboBox_pitch->insertItem(0, "");
    comboBox_pitch->setCurrentIndex(0);
}

void SongInfoWidget::set_comboBox_track()
{
    QStringList list;
    list << "0" << "1";

    comboBox_original_track->clear();
    comboBox_sound_track->clear();
    comboBox_original_track->addItems(list);
    comboBox_sound_track->addItems(list);
}

void SongInfoWidget::set_comboBox_Remain()
{
    QStringList matchList, enabledList, trackList;

    matchList = QStringList(match.values());
    enabledList = QStringList(enabled.values());
    trackList = QStringList(track.values());

    comboBox_match->clear();
    comboBox_enabled->clear();
    comboBox_sound_track->clear();
    comboBox_original_track->clear();

    comboBox_match->addItems(matchList);
    comboBox_enabled->addItems(enabledList);
    comboBox_original_track->addItems(trackList);
    comboBox_sound_track->addItems(trackList);

    comboBox_enabled->setCurrentIndex(1);
    comboBox_sound_track->setCurrentIndex(1);

    QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    lineEdit_create_time->setText(date);
}

void SongInfoWidget::setPlaceholder_serial_id(qint64 &serial_id)
{
//    QString str = QString("最大serial_id:%1").arg(serial_id);
//    lineEdit_serial_id->setPlaceholderText(QString::number(str));
    serial_id++;
    lineEdit_serial_id->setText(QString::number(serial_id));
}


void SongInfoWidget::PushButton_question_pressed()
{
    label = new QLabel(widgetRight);
    label->setFixedWidth(250);
    label->setObjectName("HelpLabel");
    QString str = QString(" 打开视频时为原唱，原唱填0，伴唱填1\n 打开视频时为伴唱，原唱填1，伴唱填0");
    label->setText(str);
    QRect rect = comboBox_quality->geometry();
    label->setGeometry(rect.x(), rect.y()-8, 250, rect.height()+8);
    label->show();
}

void SongInfoWidget::PushButton_question_released()
{
    label->deleteLater();
}

void SongInfoWidget::getCalendarDate(bool checked)
{
    if(checked){
        calendar->setHidden(false);
        calendar->setFocus();
    }else{
        calendar->setHidden(true);
    }
}

void SongInfoWidget::setDateTime()
{
    QString date = calendar->selectedDate().toString("yyyy-MM-dd");
    lineEdit_create_time->setText(date);
}

void SongInfoWidget::addSinger()
{
    SingerAddModifyDialog *dialog = new SingerAddModifyDialog();

    connect(dialog, &SingerAddModifyDialog::update_view, this, &SongInfoWidget::setAddSinger);
    dialog->exec();
}

void SongInfoWidget::setAddSinger(QList<Actor> _actor)
{
    if(_actor.count()<1)
    {
        return;
    }

    if(!actor.isEmpty())
        actor.clear();

    actor = _actor;
    if(actor.size() == 1)
    {
        media.artist_sid_2 = "";
        media.artist_sid_1 = actor.at(0).sid;
        lineEdit_addSinger->setText(actor.at(0).name);
    }
    else if(actor.size() >= 2)
    {
        media.artist_sid_1 = actor.at(0).sid;
        media.artist_sid_2 = actor.at(1).sid;
        QString singers;
        singers.append(actor.at(0).name);
        singers.append("_");
        singers.append(actor.at(1).name);
        lineEdit_addSinger->setText(singers);
    }
//    else
//    {
//        lineEdit_addSinger->setText(actor.at(0).name);
//    }
}

void SongInfoWidget::change_sound_track(int index)
{
    if(index == 0)
        comboBox_original_track->setCurrentIndex(1);
    else
        comboBox_original_track->setCurrentIndex(0);
}

void SongInfoWidget::change_original_track(int index)
{
    if(index == 0)
        comboBox_sound_track->setCurrentIndex(1);
    else
        comboBox_sound_track->setCurrentIndex(0);
}


void SongInfoWidget::uploadVideo()
{
    QString fileFormat("视频文件(*.mp4 *.mpg)");
    QString document = QProcessEnvironment::systemEnvironment().value("USERPROFILE")+"\\Desktop";
    QString pathStr = QFileDialog::getOpenFileName(this,
                                                tr("上传视频"),
                                                document,
                                                fileFormat
                                                );
    if(pathStr.isEmpty())
        return;

    lineEdit_upload_video->setText(pathStr);
}

void SongInfoWidget::uploadLyric()
{
    QString fileFormat("歌词文件(*.lrc)");
    QString document = QProcessEnvironment::systemEnvironment().value("USERPROFILE")+"\\Desktop";
    QString pathStr = QFileDialog::getOpenFileName(this,
                                                tr("上传歌词"),
                                                document,
                                                fileFormat
                                                );
    if(pathStr.isEmpty())
        return;

    lineEdit_lyric->setText(pathStr);
}

QString SongInfoWidget::uploadV(const QString path, QString *retValue)
{
    CurlUpload *curlUpload = new CurlUpload();
    QString retPath =  curlUpload->uploadMediaVideo(path, retValue);

#ifndef YQC_TECH

#else
    if(!retPath.isEmpty())
        lineEdit_upload_video->setText(retPath);
#endif


    delete curlUpload;
    return retPath;
}

QString SongInfoWidget::uploadL(const QString path)
{
    CurlUpload *curlUpload = new CurlUpload();
    QString retPath =  curlUpload->uploadMedialyric(path);

    if(!retPath.isEmpty())
        lineEdit_lyric->setText(retPath);

    QStringList list = retPath.split("/");
    delete curlUpload;
    return list.last();
}

void SongInfoWidget::setWords()
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
        if(singles.at(i).isEmpty())
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

void SongInfoWidget::serial_id_ef()
{
    if(media.serial_id.compare(lineEdit_serial_id->text()) == 0)
        lineEdit_serial_id->setStyleSheet(border_yellow);
    else
        lineEdit_serial_id->setStyleSheet(border_red);
}

void SongInfoWidget::video_ef()
{
    if(media.path.compare(lineEdit_upload_video->text()) == 0)
        lineEdit_upload_video->setStyleSheet(border_yellow);
    else
        lineEdit_upload_video->setStyleSheet(border_red);
}

void SongInfoWidget::pinyin_ef()
{
    if(media.pinyin.compare(lineEdit_pinyin->text()) == 0)
        lineEdit_pinyin->setStyleSheet(border_yellow);
    else
        lineEdit_pinyin->setStyleSheet(border_red);
}

void SongInfoWidget::head_ef()
{
    if(media.head.compare(lineEdit_head->text()) == 0)
        lineEdit_head->setStyleSheet(border_yellow);
    else
        lineEdit_head->setStyleSheet(border_red);
}

void SongInfoWidget::name_ef()
{
    if(media.name.compare(lineEdit_name->text()) == 0)
        lineEdit_name->setStyleSheet(border_yellow);
    else
        lineEdit_name->setStyleSheet(border_red);
}

void SongInfoWidget::lyric_ef()
{
    if(media.lyric.compare(lineEdit_lyric->text()) == 0)
        lineEdit_lyric->setStyleSheet(border_yellow);
    else
        lineEdit_lyric->setStyleSheet(border_red);
}

void SongInfoWidget::header_ef()
{
    if(media.header.compare(lineEdit_header->text()) == 0)
        lineEdit_header->setStyleSheet(border_yellow);
    else
        lineEdit_header->setStyleSheet(border_red);
}

void SongInfoWidget::words_ef()
{
    if(media.words.compare(lineEdit_words->text()) == 0)
        lineEdit_words->setStyleSheet(border_yellow);
    else
        lineEdit_words->setStyleSheet(border_red);
}

void SongInfoWidget::prelude_ef()
{
    if(media.prelude.compare(lineEdit_prelude->text()) == 0)
        lineEdit_prelude->setStyleSheet(border_yellow);
    else
        lineEdit_prelude->setStyleSheet(border_red);
}

void SongInfoWidget::singer_ef()
{
    if(media.singer.compare(lineEdit_addSinger->text()) == 0)
        lineEdit_addSinger->setStyleSheet(border_yellow);
    else
        lineEdit_addSinger->setStyleSheet(border_red);
}

void SongInfoWidget::info_ef()
{
    if(media.info.compare(lineEdit_info->text()) == 0)
        lineEdit_info->setStyleSheet(border_yellow);
    else
        lineEdit_info->setStyleSheet(border_red);
}

void SongInfoWidget::time_ef()
{
    if(media.create_time.compare(lineEdit_create_time->text()) == 0)
        lineEdit_create_time->setStyleSheet(border_yellow);
    else
        lineEdit_create_time->setStyleSheet(border_red);
}

void SongInfoWidget::language_act(const QString &val)
{
    int index = media.language.toInt();
    if(index == language.key(val, 0))
        comboBox_language->setStyleSheet(border_yellow);
    else
        comboBox_language->setStyleSheet(border_red);
}

void SongInfoWidget::version_act(const QString &val)
{
    int index = media.version.toInt();
    if(index == version.key(val, 0))
        comboBox_version->setStyleSheet(border_yellow);
    else
        comboBox_version->setStyleSheet(border_red);
}

void SongInfoWidget::source_act(const QString &val)
{
    int index = media.source.toInt();
    if(index == source.key(val, 0))
        comboBox_source->setStyleSheet(border_yellow);
    else
        comboBox_source->setStyleSheet(border_red);
}

void SongInfoWidget::type_act(const QString &val)
{
    int index = media.type.toInt();
    if(index == type.key(val, 0))
        comboBox_type->setStyleSheet(border_yellow);
    else
        comboBox_type->setStyleSheet(border_red);
}

void SongInfoWidget::resolution_act(const QString &val)
{
    int index = media.resolution.toInt();
    if(index == resolution.key(val, 0))
        comboBox_resolution->setStyleSheet(border_yellow);
    else
        comboBox_resolution->setStyleSheet(border_red);
}

void SongInfoWidget::rthym_act(const QString &val)
{
    int index = media.rhythm.toInt();
    if(index == rthym.key(val, 0))
        comboBox_rthym->setStyleSheet(border_yellow);
    else
        comboBox_rthym->setStyleSheet(border_red);
}

void SongInfoWidget::match_act(const QString &val)
{
    int index = media.match.toInt();
    if(index == match.key(val, 0))
        comboBox_match->setStyleSheet(border_yellow);
    else
        comboBox_match->setStyleSheet(border_red);
}

void SongInfoWidget::enabled_act(const QString &val)
{
    int index = media.enabled.toInt();
    if(index == enabled.key(val, 0))
        comboBox_enabled->setStyleSheet(border_yellow);
    else
        comboBox_enabled->setStyleSheet(border_red);
}

void SongInfoWidget::sound_track_act(const QString &val)
{
    int index = media.sound_track.toInt();
    if(index == track.key(val, 0))
        comboBox_sound_track->setStyleSheet(border_yellow);
    else
        comboBox_sound_track->setStyleSheet(border_red);
}

void SongInfoWidget::original_track_act(const QString &val)
{
//    comboBox_original_track->setStyleSheet(border_red);
//    comboBox_sound_track->setStyleSheet(border_red);

    int index = media.original_track.toInt();
    if(index == track.key(val, 0))
        comboBox_original_track->setStyleSheet(border_yellow);
    else
        comboBox_original_track->setStyleSheet(border_red);

}

void SongInfoWidget::quality_act(const QString &val)
{
    int index = media.quality.toInt();
    if(index == quality.key(val, 0))
        comboBox_quality->setStyleSheet(border_yellow);
    else
        comboBox_quality->setStyleSheet(border_red);
}

void SongInfoWidget::effect_act(const QString &val)
{
    int index = media.effect.toInt();
    if(index == effect.key(val, 0))
        comboBox_effect->setStyleSheet(border_yellow);
    else
        comboBox_effect->setStyleSheet(border_red);
}

void SongInfoWidget::pitch_act(const QString &val)
{
    int index = media.pitch.toInt();
    if(index == pitch.key(val, 0))
        comboBox_pitch->setStyleSheet(border_yellow);
    else
        comboBox_pitch->setStyleSheet(border_red);
}

void SongInfoWidget::setDefaultBorder()
{
    QString  border = QString("border: 1px solid rgb(201, 201, 201)");

    comboBox_pitch->setStyleSheet(border);
    comboBox_effect->setStyleSheet(border);
    comboBox_quality->setStyleSheet(border);
    comboBox_original_track->setStyleSheet(border);
    comboBox_sound_track->setStyleSheet(border);
    comboBox_enabled->setStyleSheet(border);
    comboBox_match->setStyleSheet(border);
    comboBox_rthym->setStyleSheet(border);
    comboBox_resolution->setStyleSheet(border);
    comboBox_type->setStyleSheet(border);
    comboBox_source->setStyleSheet(border);
    comboBox_version->setStyleSheet(border);
    comboBox_language->setStyleSheet(border);

    lineEdit_create_time->setStyleSheet(border);
    lineEdit_info->setStyleSheet(border);
    lineEdit_addSinger->setStyleSheet(border);
    lineEdit_prelude->setStyleSheet(border);
    lineEdit_words->setStyleSheet(border);
    lineEdit_header->setStyleSheet(border);
    lineEdit_lyric->setStyleSheet(border);
    lineEdit_name->setStyleSheet(border);
    lineEdit_head->setStyleSheet(border);
    lineEdit_pinyin->setStyleSheet(border);
    lineEdit_upload_video->setStyleSheet(border);
    lineEdit_serial_id->setStyleSheet(border);
    lineEdit_name->setStyleSheet(border);
}

void SongInfoWidget::setDefaultValue()
{
    comboBox_pitch->setCurrentIndex(0);
    comboBox_effect->setCurrentIndex(0);
    comboBox_quality->setCurrentIndex(0);
    comboBox_original_track->setCurrentIndex(0);
    comboBox_sound_track->setCurrentIndex(1);
    comboBox_enabled->setCurrentIndex(1);
    comboBox_match->setCurrentIndex(0);
    comboBox_rthym->setCurrentIndex(0);
    comboBox_resolution->setCurrentIndex(0);
    comboBox_type->setCurrentIndex(0);
    comboBox_source->setCurrentIndex(0);
    comboBox_version->setCurrentIndex(0);
    comboBox_language->setCurrentIndex(0);

    lineEdit_create_time->clear();
    lineEdit_info->clear();
    lineEdit_addSinger->clear();
    lineEdit_prelude->clear();
    lineEdit_words->clear();
    lineEdit_header->clear();
    lineEdit_lyric->clear();
    lineEdit_name->clear();
    lineEdit_head->clear();
    lineEdit_pinyin->clear();
    lineEdit_upload_video->clear();
    lineEdit_serial_id->clear();
    lineEdit_name->clear();


    QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    lineEdit_create_time->setText(date);
}

qint64 SongInfoWidget::getSerial_id()
{
    return media.serial_id.toLongLong();
}

QString SongInfoWidget::getVideoPath()
{
    return media.path;
}

int SongInfoWidget::getTrack()
{
    return media.original_track.toInt();
}

void SongInfoWidget::set_Tooltip_image()
{
//    label_serial_id_tooptip->setScaledContents(true); //图片随label调整大小

    QImage image;
    image.load(":/other/images/required.png");
    label_serial_id_tooptip->setPixmap(QPixmap::fromImage(image));

    label_serial_id_tooptip->setPixmap(QPixmap::fromImage(image));
    label_upload_video_tooptip->setPixmap(QPixmap::fromImage(image));
    label_language_tooptip->setPixmap(QPixmap::fromImage(image));
    label_pinyin_tooptip->setPixmap(QPixmap::fromImage(image));
    label_head_tooptip->setPixmap(QPixmap::fromImage(image));

    label_name_tooptip->setPixmap(QPixmap::fromImage(image));
    label_type_tooptip->setPixmap(QPixmap::fromImage(image));
    label_header_tooptip->setPixmap(QPixmap::fromImage(image));
    label_words_tooptip->setPixmap(QPixmap::fromImage(image));

    label_addSinger_tooptip->setPixmap(QPixmap::fromImage(image));
    label_match_tooptip->setPixmap(QPixmap::fromImage(image));
    label_enabled_tooptip->setPixmap(QPixmap::fromImage(image));
    label_track_tooptip->setPixmap(QPixmap::fromImage(image));
}

void SongInfoWidget::setComboBoxValue(QComboBox *combobox, QList<QString> list)
{
    QStringList items(list);
    if(items.isEmpty())
        return;
    QString currentStr = combobox->currentText();
    int index = list.indexOf(currentStr);

    combobox->clear();
    combobox->insertItems(0, items);
    combobox->setCurrentIndex(index);
    combobox->insertItem(0, "");
}

void SongInfoWidget::setComboBoxValueRemain(QComboBox *combobox, QList<QString> list)
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

QString SongInfoWidget::returnOldSerial_id()
{
    return media.serial_id;
}

void SongInfoWidget::setOldMediaValue(Media _media)
{
    media = _media;
}

void SongInfoWidget::setTab_order()
{
    this->setTabOrder(lineEdit_serial_id, lineEdit_name);
    this->setTabOrder(lineEdit_name, pushButton_addSinger);
    this->setTabOrder(pushButton_addSinger, lineEdit_addSinger);
    setTabOrder(lineEdit_addSinger, lineEdit_upload_video);
    setTabOrder(lineEdit_upload_video, pushButton_upload_video);
    setTabOrder(pushButton_upload_video, lineEdit_lyric);
    setTabOrder(lineEdit_lyric, pushButton_upload_lyric);
    setTabOrder(pushButton_upload_lyric, comboBox_match);
    setTabOrder(comboBox_match, comboBox_language);
    setTabOrder(comboBox_language, comboBox_type);
    setTabOrder(comboBox_type, comboBox_enabled);
    setTabOrder(comboBox_enabled, comboBox_version);
    setTabOrder(comboBox_version, comboBox_resolution);
    setTabOrder(comboBox_resolution, comboBox_original_track);
    setTabOrder(comboBox_original_track, comboBox_sound_track);
    setTabOrder(comboBox_sound_track, lineEdit_pinyin);
    setTabOrder(lineEdit_pinyin, lineEdit_header);
    setTabOrder(lineEdit_header, comboBox_quality);
    setTabOrder(comboBox_quality,  lineEdit_head);
    setTabOrder(lineEdit_head, lineEdit_words);
    setTabOrder(lineEdit_words, comboBox_effect);
    setTabOrder(comboBox_effect, comboBox_source);
    setTabOrder(comboBox_source, lineEdit_prelude);
    setTabOrder(lineEdit_prelude, lineEdit_info);
    setTabOrder(lineEdit_info, lineEdit_create_time);
    setTabOrder(lineEdit_create_time, pushButton_calendar);
    setTabOrder(pushButton_calendar, comboBox_rthym);
    setTabOrder(comboBox_rthym, comboBox_pitch);

}
