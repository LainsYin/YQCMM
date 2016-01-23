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

#ifndef MP4PLAYER_H_
#define MP4OPLAYER_H_

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
namespace Ui {
    class MP4Player;
}

class QTimer;
class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;

class MP4Player : public QMainWindow
{
    Q_OBJECT
public:
    explicit MP4Player(QWidget *parent = 0);
    ~MP4Player();

    void closeEvent(QCloseEvent *);

public:
    void playMedia(const QString filePaht);
    ///
    /// \brief playUrlMedia
    /// \param url
    /// \param track  0原唱 1伴唱
    ///
    void playUrlMedia(const QString &url, const int &track);

private:
    void readAndSetStyleSheet();
private slots:
    void playEnd();

    void on_stop_clicked();

    void on_pause_clicked(bool checked);

    void trackChange(const int &current);

    void timeOver();
private:
    Ui::MP4Player *ui;

    VlcInstance *_instance;
    VlcMedia *_media;
    VlcMediaPlayer *_player;

    QTimer *timer;
    int _track;
};

#endif // MP4OPLAYER_H_
