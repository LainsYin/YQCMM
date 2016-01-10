#ifndef THREAD_H
#define THREAD_H

#include <QThread>
class MysqlQuery;
class SongInfoWidget;
class SingerInfoWdiget;
class Media;
class Actor;

/*
 * 线程
*/
class Thread : public QThread
{
    Q_OBJECT
public:
    Thread(QObject *parent = 0, int _isSong = 0);
    ~Thread();

    void run();

    //上传歌曲
    void uploadMedia();
    //上传歌星
    void uploadActor();

    /*
     * 上传云更新数据
     * filename 压缩包名
     * localpath 当前路径
     * _retSize 实时上传数据大小
    */
    void setUploadYun(const QString &filename , const QString &localpath, QString *_retSize = NULL);
    /*
     * 上传云更新压缩包
    */
    void uploadYun();

    void setSongInfoWidgetPointer( SongInfoWidget *_song,  MysqlQuery *_sql, QString *_retSize = NULL);
    void setSingerInfoWidgetPointer( SingerInfoWdiget *_singer,  MysqlQuery *_sql, QString *_retSize = NULL);

    //设置歌星和歌曲值
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
