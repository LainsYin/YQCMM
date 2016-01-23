#include "yunthread.h"
#include "curlupload.h"
#include "mysqlquery.h"
#include <QDir>
#include <QFile>
#include <QThread>
#include <QDebug>
#include <QStringList>
#include <QCoreApplication>

#define UPANYUN "upaiyun.com"

YunThread::YunThread(QObject *parent)
    :QThread(parent)
{

}

YunThread::~YunThread()
{

}

void YunThread::initValue(const QList<QStringList> &value, MysqlQuery *sql,  QString *size)
{
    rowList = value;
    _sql = sql;
    retSize = size;
}

void YunThread::run()
{
    QString savePath = QCoreApplication::applicationDirPath();
    savePath = savePath.append("/update");

    QList<QStringList> rowListTmp = rowList;
    for(int i=0; i<rowListTmp.size(); i++) {
        QStringList rowValue = rowListTmp.at(i);
        QString type = rowValue.at(1);
        QString mv = rowValue.at(rowValue.size() - 3);
        QString lyricImage = rowValue.at(rowValue.size() - 2);

        if (type.toLower().compare("delete") != 0){
            if (!mv.isEmpty()){
                QString filePath = downloadFile("mp4", savePath, mv);
                if (!(uploadFile("mp4", filePath))){
                }
            }

            if (!lyricImage.isEmpty()){
               QString filePath =  downloadFile("image", savePath, lyricImage);
               if (!(uploadFile("", filePath))){
               }
            }
        }

        if(execSql(rowValue)){
            rowList.removeOne(rowListTmp.at(i));
            emit updateView(rowListTmp.at(i));
        }
    }

    //门店更新状态接口
    emit updateStore();
}

QString YunThread::downloadFile(const QString &type, const QString &dir, const QString &url)
{
    QStringList list = url.split(UPANYUN);
    QString path = dir + list.last();
    QString tempPath = path;
    tempPath = tempPath.remove(tempPath.split("/").last());
    QDir dirf(tempPath);
    if(!dirf.exists()){
        dirf.mkpath(tempPath);
    }

    QString dirfile = list.last();
    emit downloadUpload(QString("Downloading %1").arg(dirfile.split("/").last()));
    CurlUpload *curlDownlaod = new CurlUpload();
    if(curlDownlaod->download_yun(type, url, path)){
        writeLogging(QString(" download success : %1").arg(path));
    } else {
        writeLogging(QString(" download failed : %1").arg(path));
    }
    delete curlDownlaod;


    emit downloadUpload("DownLoading end");
    return path;
}

bool YunThread::uploadFile(const QString &type, const QString &filePath)
{
    bool status = false;
    CurlUpload *curlUpload = new CurlUpload();
    QFile file(filePath);
    if(file.exists()){
        QStringList lists = filePath.split("/");
        QString fileName = lists.last();
        QString dir = lists.at(lists.size() - 2);
        QString ok;
        emit downloadUpload(QString("Uploading %1").arg(fileName));
        if (type.compare("mp4") == 0){
            ok = curlUpload->uploadYunVideo(filePath, dir, retSize);
        } else {
            if(dir.compare("lyrics") == 0){
                ok = curlUpload->uploadMedialyric(filePath);
            } else if(dir.compare("fm") == 0){
                ok = curlUpload->uploadFmImage(filePath);
            } else if(dir.compare("avator") == 0){
                ok = curlUpload->uploadActorImage(filePath, retSize);
            }
        }

        if(ok.compare("0") == 0){
            writeLogging(QString(" upload type error : %1").arg(fileName));
            status = false;
        } else if (ok.compare("1") == 0){
            writeLogging(QString("There already exists a file called %1 in %2 and not Overwrite")
                         .arg(fileName).arg(dir));
            status = true;
        }else if (ok.isEmpty()){
            writeLogging(QString("upload return empty."));
            status = false;
        } else if (ok.compare("-2") == 0){
            writeLogging(QString("upload type error."));
            status = false;
        }else {
            status = true;
            writeLogging(QString("upload return : %1").arg(ok));
        }
    }

    if (status){
//        file.remove();
    }

    emit downloadUpload("Uploading end");
    return status;
}

bool YunThread::execSql(const QStringList &info)
{
    bool ret = false;
    if (_sql->executeSql(info.last())){
        rowList.removeOne(info);
        ret = true;
    } else {
        writeLogging(QString(" sql error  : %1").arg(info.last()));
    }

    return ret;
}

void YunThread::writeLogging(const QString &str)
{
    qDebug() << str;
}

