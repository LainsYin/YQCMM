#include "titlewidget.h"
#include <QFile>
#include <QPalette>
#include <QStyleOption>
#include <QMessageBox>
TitleWidget::TitleWidget(QWidget *parent)
    : QWidget(parent)
{
    QFile qss(":/qss/title.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());
    qss.close();

    login_label = new QLabel();
    logo_label = new QLabel();
    title_label = new QLabel();
    min_button = new QPushButton();
    max_button = new QPushButton();
    close_button = new QPushButton();
    about_button = new QPushButton();

    title_label->setEnabled(true);
    logo_label->setScaledContents(true);
    QImage image;
    image.load(":/other/img/logo.ico");
    logo_label->setPixmap(QPixmap::fromImage(image));
    logo_label->setFixedWidth(40);
    logo_label->setFixedHeight(40);
    close_button->setFixedWidth(15);
    close_button->setFixedHeight(15);
    min_button->setFixedWidth(15);
    min_button->setFixedHeight(3);
    max_button->setFixedWidth(15);
    max_button->setFixedHeight(4);

    this->setObjectName("TitleWidget");
    login_label->setObjectName("LoginLabel");
    title_label->setObjectName("TitleLabel");
    close_button->setObjectName("CloseButton");
    min_button->setObjectName("MinButton");
    max_button->setObjectName("MinButton");
    title_label->setAlignment(Qt::AlignCenter);
    title_label->setText("<font size=\"8\">曲库管理系统<br>"
                         "<font size=\"3\">Music library management system ");
    about_button->setObjectName("AboutButton");
    about_button->setText("关于");
//    login_label->setText("admin");
    max_button->setHidden(true);


    connect(min_button, SIGNAL(clicked()), this, SIGNAL(showMin()));
    connect(max_button, &QPushButton::clicked, this, &TitleWidget::showMax);
    connect(close_button, &QPushButton::clicked, this, &TitleWidget::closeWidget);
    connect(about_button, &QPushButton::clicked, this, &TitleWidget::about);

    QHBoxLayout *title_layout = new QHBoxLayout();
    QHBoxLayout *close_layout = new QHBoxLayout();
    close_layout->addWidget(min_button, 0, Qt::AlignVCenter);
    close_layout->addWidget(max_button, 0, Qt::AlignVCenter);
    close_layout->addWidget(close_button, 0, Qt::AlignVCenter);
    close_layout->setContentsMargins(30, 0, 0, 0);
    close_layout->setSpacing(15);
    title_layout->addWidget(logo_label,0,Qt::AlignVCenter);
    title_layout->addWidget(title_label, 0, Qt::AlignVCenter);
    title_layout->addStretch();
    title_layout->addWidget(about_button);
    title_layout->addWidget(login_label, 0, Qt::AlignVCenter);
    title_layout->addLayout(close_layout);
    title_layout->setSpacing(0);
    title_layout->setContentsMargins(20, 0, 20, 0);

    setLayout(title_layout);
    setFixedHeight(60);

//    QPalette palette;
//    palette.setBrush(QPalette::Window,  QBrush(QColor(0, 0, 0 )));
//    this->setPalette(palette);
//    this->setAutoFillBackground(true);
}

void TitleWidget::setLoginUserName(const QString &user)
{
    login_label->setText(user);
}

void TitleWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void TitleWidget::about()
{
    QString title("关于");
    QString content(" <p> <a href=http://www.17chang.com>   北京洛哈网络科技有限公司</a></p>"
                    " <p>公司网址 ：<a href=http://www.17chang.com>   www.yiqiding.com</a></p> "
                    " <p>版本号： 5.1508.0831.02</p>");
    QMessageBox box(QMessageBox::Information,title, content);
    box.setStandardButtons(QMessageBox::Ok);
    box.setButtonText(QMessageBox::Ok, "确定");
    box.exec();
}
