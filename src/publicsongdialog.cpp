#include "publicsongdialog.h"

#include "tablemodel.h"
#include "enuminfo.h"
#include "defistruct.h"
#include "thread.h"
#include "songsonlinewidget.h"
#include "songinfowidget.h"
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QPainter>
#include <QFileDialog>
#include <QStyleOption>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QProgressBar>
#include <QDateTime>
#include <QComboBox>
#include <QTimer>
#include <QListView>
#include <QProcessEnvironment>
#include <QDebug>


PublicSongDialog::PublicSongDialog(QWidget *parent)
    : DropShadowWidget(parent)
{
    setFixedHeight(350);
    setFixedWidth(400);
    _sql = NULL;

    initWidget();
    initWidgetValue();

    connect(pushbutton_close, &QPushButton::clicked, this, &PublicSongDialog::close);
    connect(pushbutton_preview, &QPushButton::clicked, this, &PublicSongDialog::preview);
    connect(pushbutton_yes, &QPushButton::clicked, this, &PublicSongDialog::yes);

    retSize = "";
    timer = NULL;

    pushbutton_yes->setDefault(true);
    combobox_type->setHidden(true);
}

PublicSongDialog::~PublicSongDialog()
{

}

void PublicSongDialog::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void PublicSongDialog::initWidget()
{
    widgetTitle = new QWidget(this);
    widgetBottom = new QWidget(this);

    label_title = new QLabel(widgetTitle);
    label_tip = new QLabel(widgetBottom);
//    label_musicName = new QLabel(widgetBottom);
//    label_video = new QLabel(widgetBottom);

    pushbutton_close = new QPushButton(widgetTitle);
    pushbutton_preview = new QPushButton(widgetBottom);
    pushbutton_yes = new QPushButton(widgetBottom);

    lineedit_name = new QLineEdit(widgetBottom);
    lineedit_path = new QLineEdit(widgetBottom);

    combobox_type = new QComboBox(widgetBottom);
    progress = new QProgressBar(widgetBottom);

    QHBoxLayout *title_layout = new QHBoxLayout();
    title_layout->addWidget(label_title);
    title_layout->addStretch();
    title_layout->addWidget(pushbutton_close);
    title_layout->setContentsMargins(20, 0, 20, 0);
    title_layout->setSpacing(0);
    widgetTitle->setLayout(title_layout);

    QHBoxLayout *path_layout = new QHBoxLayout();
    path_layout->addWidget(lineedit_path);
    path_layout->addWidget(pushbutton_preview);
    path_layout->addWidget(pushbutton_yes);
    path_layout->setSpacing(10);
    path_layout->setMargin(0);

    QVBoxLayout *bottom_layout = new QVBoxLayout();
    bottom_layout->addWidget(label_tip);
    bottom_layout->addWidget(combobox_type);
    bottom_layout->addWidget(lineedit_name);
    bottom_layout->addLayout(path_layout);
    bottom_layout->addWidget(progress);
    bottom_layout->setSpacing(10);
    bottom_layout->setContentsMargins(20, 0, 20, 50);
    widgetBottom->setLayout(bottom_layout);

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget(widgetTitle);
    vLayout->addWidget(widgetBottom);
    vLayout->setSpacing(0);
    vLayout->setMargin(0);

    this->setLayout(vLayout);
}

void PublicSongDialog::initWidgetValue()
{
    QFile qss(":/qss/title.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());
    qss.close();

    label_title->setText("添加公播歌曲");
    label_tip->setText("此处仅限上传广告宣传类公播歌曲，\n"
                       "若为其他类型歌曲，\n"
                       "请前往曲库上线管理上传歌曲信息！");

    lineedit_name->setPlaceholderText("歌曲名字");
    lineedit_path->setPlaceholderText("视频路径");

    pushbutton_preview->setText("浏览");
    pushbutton_yes->setText("确定");

    this->setObjectName("WidgetCenter");
    widgetTitle->setObjectName("TitleWidget");
    label_title->setObjectName("TitleLabel");

    label_tip->setObjectName("Label");
    label_tip->setStyleSheet("font-size:16px;");
    lineedit_name->setObjectName("LineEdit");
    lineedit_path->setObjectName("LineEdit");

    pushbutton_preview->setObjectName("Button");
    pushbutton_yes->setObjectName("Button");
    pushbutton_close->setObjectName("CloseButton");

    QListView *view = new QListView();
    combobox_type->setView(view);
    combobox_type->setMaxVisibleItems(5);

    widgetTitle->setFixedHeight(40);
    pushbutton_close->setFixedSize(14, 14);
    pushbutton_preview->setFixedSize(60, 40);
    pushbutton_yes->setFixedSize(60, 40);

    lineedit_name->setMinimumHeight(40);
    lineedit_path->setMinimumHeight(40);
    combobox_type->setMinimumHeight(40);

    progress->setValue(0);
}

void PublicSongDialog::setTitle(const QString &str)
{
    label_title->setText(str);
}

void PublicSongDialog::initSql(MysqlQuery *sql)
{
    _sql = sql;

    QStringList typeV;
    typeV << "宣传歌曲" << "锁屏状态" << "开关机歌曲";
    combobox_type->addItems(typeV);
    QStringList typeP;
    _sql->queryPublicSongIPGroupBy(typeP);
    combobox_type->addItems(typeP);
}

void PublicSongDialog::preview()
{
    QString fileFormat(("视频文件(*.mp4)"));
    QString document = QProcessEnvironment::systemEnvironment().value("USERPROFILE")+"\\Desktop";
    QString pathStr = QFileDialog::getOpenFileName(this,
                                                         ("选择视频"),
                                                         document,
                                                         fileFormat
                                                         );

    if(pathStr.isEmpty())
        return;

    lineedit_path->setText(pathStr);
}

void PublicSongDialog::yes()
{
    QString name = lineedit_name->text();
    QString path = lineedit_path->text();
    if(name.isEmpty() || path.isEmpty())
    {
        QMessageBox::information(NULL, "提示", "文件名和路径不能为空");
        return;
    }

    Media _media;
    qint64 _mid;
    _sql->getMediaMaxMid(_mid);
    _media.name = name;
    _media.mid = QString::number(++_mid);
    _media.path = path;

    _media.language = "1";
    _media.type = "1";
    _media.singer = "佚名";

    _media.pinyin = "yiming";
    _media.header = "YM";
    _media.head = "Y";
    _media.words = "1";
    _media.original_track = "0";
    _media.sound_track = "1";
    _media.artist_sid_1 = "NULL";

    QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    _media.create_time = date;

    if(!medias.isEmpty())
        medias.clear();

    medias.append(_media);

    ///线程处理上传
    retSize = "-2";
    SongInfoWidget *widget_info = new SongInfoWidget();
    Thread *workerThread = new Thread(this);
    connect(workerThread, &Thread::result, this, &PublicSongDialog::uploadsuccess);
    connect(workerThread, &Thread::start_upload, this, &PublicSongDialog::timeStart);
    connect(workerThread, &Thread::finished, workerThread, &QObject::deleteLater);
    workerThread->setSongInfoWidgetPointer(widget_info, _sql, &retSize);
    workerThread->setSongValue(medias);
    workerThread->start();
}

void PublicSongDialog::uploadsuccess(const qint64 &mid)
{

}

void PublicSongDialog::timeStart(qint64 mid)
{
    if(timer != NULL)
    {
        if(timer->isActive())
        {
            timer->stop();
            timer = NULL;
        }
    }
    timer = new QTimer(this);
    disconnect(timer, &QTimer::timeout, 0, 0);
    connect(timer, &QTimer::timeout, this, &PublicSongDialog::timeOver);
//    timer->start(1000);

    retSize = "-2";
    if(mid != -1)
    {
        timer->start(1000);
        size = true;
    }
}

void PublicSongDialog::timeOver()
{
    if(retSize.isEmpty())
    {
         timer->stop();
         qDebug() << " isempty stop  ";
    }

    QStringList list = retSize.split(",");
    if(list.last().compare("-2") == 0)
        return;
    if(!list.last().isEmpty() && list.last().compare("0") != 0)
    {
        QString valueStr;
        if(size){
            valueStr = list.last();
            progress->setRange(0, (int)valueStr.toDouble());
            size = false;
        }
        qDebug() << "fist : " <<  list.first() << " last : " << list.last();

        valueStr = list.first();
        progress->setValue((int)valueStr.toDouble());
        if(list.first().compare(list.last()) == 0)
        {
            timer->stop();
            timer = NULL;
            qDebug() << " 100 stop";
        }
    }
}

