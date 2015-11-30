#ifndef THREAD_H
#define THREAD_H

#include <QThread>
class MysqlQuery;
class SongInfoWidget;
class SingerInfoWdiget;
class Media;
class Actor;

class Thread : public QThread
{
    Q_OBJECT
public:
    Thread(QObject *parent = 0, int _isSong = 0);
    ~Thread();

    void run();

    void uploadMedia();
    void uploadActor();

    void setUploadYun(const QString &filename , const QString &localpath, QString *_retSize = NULL);
    void uploadYun();

    void setSongInfoWidgetPointer( SongInfoWidget *_song,  MysqlQuery *_sql, QString *_retSize = NULL);
    void setSingerInfoWidgetPointer( SingerInfoWdiget *_singer,  MysqlQuery *_sql, QString *_retSize = NULL);
    void setSongValue(const QList< Media> _medias);
    void setSingerValue(const QList< Actor> _actors);
signals:
    void result(const qint64 &serial_id);
    void resultYun(const QString url, const int &ok);
    void start_upload(const qint64 &serial_id);
private:
    MysqlQuery *sql;
    SongInfoWidget *song;
    SingerInfoWdiget *singer;
    QString *retSize;
    QList< Media> medias;
    QList< Actor> actors;
//    QList< QStringList > rowList;
    int isSong; ///1mv歌曲  2歌星 3MP3歌曲 4云更新


    QString _filename;
    QString _localpath;
};

#endif // THREAD_H
