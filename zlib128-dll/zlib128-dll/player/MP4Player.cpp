/****************************************************************************
* VLC-Qt Demo Player
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <QFileDialog>
#include <QInputDialog>
#include <QListView>
#include <QTimer>
#include <Common.h>
#include "Instance.h"
#include "Media.h"
#include "MediaPlayer.h"

#include "MP4Player.h"
#include "ui_MP4Player.h"

#define STARTSTR "开始"
#define STOPSTR  "暂停"

#include <QDebug>

MP4Player::MP4Player(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MP4Player),
      _media(0)
{
    ui->setupUi(this);

    _instance = new VlcInstance(VlcCommon::args(), this);
    _player = new VlcMediaPlayer(_instance);
    _player->setVideoWidget(ui->video);

    ui->video->setMediaPlayer(_player);
    ui->volume->setMediaPlayer(_player);
    ui->volume->setVolume(50);
    ui->seek->setMediaPlayer(_player);
    connect(_player, &VlcMediaPlayer::end, this, &MP4Player::playEnd);
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(trackChange(int)));

    ui->stop->setHidden(true);
    QStringList tracks;
    tracks  << " 原唱" << " 伴唱";
    ui->comboBox->addItems(tracks);
    ui->pause->setFixedSize(50, 30);
    ui->comboBox->setFixedSize(80, 30);
    QListView *view = new QListView();
    ui->comboBox->setView(view);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MP4Player::timeOver);
    _track = 0;
}

MP4Player::~MP4Player()
{
    delete _player;
    delete _media;
    delete _instance;
    delete ui;
}

void MP4Player::readAndSetStyleSheet()
{
    QFile qss(":/qss/songquery.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());
    qss.close();

    ui->pause->setObjectName("PushButton");
    ui->comboBox->setObjectName("Combobox");
}

void MP4Player::closeEvent(QCloseEvent *)
{
    if(_player)
        _player->stop();
}

///local file
///\param location location of the media (QString)
///\param localFile true, if media is local file (bool)
///\param instance main libvlc instance (VlcInstance *)
void MP4Player::playMedia(const QString filePaht)
{
    if (filePaht.isEmpty())
        return;

    _media = new VlcMedia(filePaht, true, _instance);

    _player->open(_media);

//     timer->start(500);
}

///url file
void MP4Player::playUrlMedia(const QString &url, const int &track)
{
    if (url.isEmpty())
        return;

    _media = new VlcMedia(url, _instance);

    _player->open(_media);

    _track = track;
    timer->start(500);
}

void MP4Player::playEnd()
{
    ui->pause->setText(STARTSTR);
    ui->pause->setDown(true); //按键有个凹下的效果
    ui->pause->setChecked(true);
    _player->open(_media);
    _player->stop();
}

void MP4Player::on_stop_clicked()
{

    this->close();
}

void MP4Player::on_pause_clicked(bool checked)
{
    if(checked){
        if(ui->pause->text().compare(STOPSTR) != 0)
            return;
        _player->pause();
        ui->pause->setText(STARTSTR);
    }else{
        if(ui->pause->text().compare(STARTSTR) != 0)
            return;
        _player->play();
        ui->pause->setText(STOPSTR);
    }
}

void MP4Player::trackChange(const int &current)
{
    int currTrack = ui->volume->track();
//    if(currTrack == -1)
//        return;
//    QList<int> tracks = ui->volume->trackIds();
//    QStringList lists = ui->volume->trackDescription();
//    qDebug() << " current track " << currTrack;
//    qDebug() << " current track " << tracks;
//    qDebug() << " list s " << lists;

    int track = 1;
    if(current == 0)
        track = 1;
    else
        track = 2;

    ui->volume->setTrack(track);
}

void MP4Player::timeOver()
{
   if(_track)
   {
       ui->volume->setTrack(2);
       ui->comboBox->setCurrentIndex(1);
   }
   else
   {
       ui->volume->setTrack(1);
       ui->comboBox->setCurrentIndex(0);
   }

   if(timer->isActive())
   {
       timer->stop();

       qDebug() << " timer stop ";
   }
}
