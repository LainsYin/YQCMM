
#include "ConvertionMP4.h"

extern "C"{
#include "Convertion.h"
}


#include <windows.h>
#include <QDir>
#include <QFile>
#include <QApplication>
#include <QProcessEnvironment>

#include <QDebug>
#include <QDateTime>
#include <QMessageBox>
#include <QTextCodec>

ConvertionMP4::ConvertionMP4(QObject *parent) :
    QObject(parent)
{
    appPath = QApplication::applicationDirPath();
    currDiskFreeSize = 0;

    QFileInfoList driversInfo = QDir::drives();
    qDebug() << " absoluteDier : " << driversInfo.at(0).absoluteDir();
    qDebug() << " path : " << driversInfo.at(0).absoluteDir().absolutePath();

    QDir dir(appPath);

    QString driver(dir.rootPath());
    LPCWSTR lpcwstrDriver=(LPCWSTR)driver.utf16();
    ULARGE_INTEGER liFreeBytesAvailable, liTotalBytes, liTotalFreeBytes;
    if( !GetDiskFreeSpaceEx( lpcwstrDriver, &liFreeBytesAvailable, &liTotalBytes, &liTotalFreeBytes) )
    {
        qDebug() << "ERROR: Call to GetDiskFreeSpaceEx() failed.";

    }
    currDiskFreeSize = (quint64) liTotalFreeBytes.QuadPart/1024/1024;
}

ConvertionMP4::~ConvertionMP4()
{
}

QString ConvertionMP4::convertionFile(const QString &srcPath, const QString &serial_id)
{
    QString retStr = NULL;
//    QString tempDestPath = srcPath;
//    if(tempDestPath.indexOf(".mpg") != -1)
//    {
//        tempDestPath = tempDestPath.replace(".mpg", ".mp4");
//    }
//    else if(tempDestPath.indexOf(".mp3") != -1)
//    {
//        tempDestPath = tempDestPath.replace(".mp3", ".mp4");
//    }
//    ///修改文件名为serial_id后六位
//    QString path = tempDestPath;
//    int before = path.lastIndexOf("/");
//    int after = path.lastIndexOf(".");
//    QString mediaName = serial_id.right(6);
//    path = path.replace(before+1, after-before-1, mediaName);

//    path = path.replace(" ", "_");
//    if(srcPath.compare(path) != 0)
//    {
//        QFile reName(srcPath);
//        reName.rename(path);
//    }

    if(srcPath.indexOf(".mp4") != -1)
    {
        ///修改文件名为serial_id后六位
        QString path = srcPath;
        int before = path.lastIndexOf("/");
        int after = path.lastIndexOf(".");
        QString mediaName = serial_id.right(6);
        path = path.replace(before+1, after-before-1, mediaName);

        path = path.replace(" ", "_");
        if(srcPath.compare(path) != 0)
        {
            QFile reName(srcPath);
            reName.rename(path);
        }
        return path;
    }

    QFile srcFile(srcPath);
    if(srcFile.size()/1024/1024 >= currDiskFreeSize)
    {
        QMessageBox box(QMessageBox::Warning, "提示", "磁盘空间不足。。。");
        box.setStandardButtons(QMessageBox::Ok);
        box.setButtonText(QMessageBox::Ok, "确定");
        box.exec();
        return retStr;
    }

    QString tempDestPath = srcPath;
    if(tempDestPath.indexOf(".mpg") != -1)
    {
        tempDestPath = tempDestPath.replace(".mpg", ".mp4");
    }
    else if(tempDestPath.indexOf(".mp3") != -1)
    {
        tempDestPath = tempDestPath.replace(".mp3", ".mp4");
    }


    ///修改文件名为serial_id后六位
    QString path = srcPath;
    int before = path.lastIndexOf("/");
    int after = path.lastIndexOf(".");
    QString mediaName = serial_id.right(6);
    tempDestPath = tempDestPath.replace(before+1, after-before-1, mediaName);

    path = path.replace(" ", "_");
    if(srcPath.compare(path) != 0)
    {
        QFile reName(srcPath);
        reName.rename(path);
    }

    QString dest = tempDestPath;

    QTextCodec *gbk = QTextCodec::codecForName("GBK");
    QByteArray _src = gbk->fromUnicode(path);
    QByteArray _dest = gbk->fromUnicode(dest);
    int ret = Convertion(_src.data(), _dest.data());

    if(ret != 0){
//        QString filepath = QApplication::applicationDirPath();
        QString filepath = appPath;
        filepath.append("/convertion.log");
        QFile logfile(filepath);
        if(logfile.open(QIODevice::Append | QIODevice::WriteOnly))
        {

            QString logpath = path;

            logpath.append(QString(" =  %1\r\n").arg(ret));
            logfile.write(logpath.toLatin1());

            logfile.close();
        }
    }
    else
    {
        retStr = dest;
    }

    if(srcPath.compare(path) != 0)
    {
        QFile srcFile(path);
        srcFile.rename(srcPath);
    }

    return retStr;
}

QString ConvertionMP4::convertionFilePS(const QString &srcPath, const QString &mid)
{
    QString retStr = NULL;
    if(srcPath.indexOf(".mp4") != -1)
    {
        ///修改文件名为serial_id后六位
        QString path = srcPath;
        int before = path.lastIndexOf("/");
        int after = path.lastIndexOf(".");

        QString destName = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh_mm_ss");
        QString mediaName = destName; //("yqc");
        mediaName.append(mid);
        path = path.replace(before+1, after-before-1, mediaName);

        path = path.replace(" ", "_");
        if(srcPath.compare(path) != 0)
        {
            QFile reName(srcPath);
            reName.rename(path);
        }
        return path;
    }

    QFile srcFile(srcPath);
    if(srcFile.size()/1024/1024 >= currDiskFreeSize)
    {
        QMessageBox box(QMessageBox::Warning, "提示", "磁盘空间不足。。。");
        box.setStandardButtons(QMessageBox::Ok);
        box.setButtonText(QMessageBox::Ok, "确定");
        box.exec();
        return retStr;
    }

    QString tempDestPath = srcPath;
    if(tempDestPath.indexOf(".mpg") != -1)
    {
        tempDestPath = tempDestPath.replace(".mpg", ".mp4");
    }
    else if(tempDestPath.indexOf(".mp3") != -1)
    {
        tempDestPath = tempDestPath.replace(".mp3", ".mp4");
    }


    ///修改文件名为serial_id后六位
    QString path = srcPath;
    int before = path.lastIndexOf("/");
    int after = path.lastIndexOf(".");
//    QString mediaName = serial_id.right(6);

    QString destName = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh_mm_ss");
    QString mediaName = destName; //("yqc");
    mediaName.append(mid);
    tempDestPath = tempDestPath.replace(before+1, after-before-1, mediaName);

    path = path.replace(" ", "_");
    if(srcPath.compare(path) != 0)
    {
        QFile reName(srcPath);
        reName.rename(path);
    }

    QString dest = tempDestPath;

    QTextCodec *gbk = QTextCodec::codecForName("GBK");
    QByteArray _src = gbk->fromUnicode(path);
    QByteArray _dest = gbk->fromUnicode(dest);
    int ret = Convertion(_src.data(), _dest.data());

    if(ret != 0){
//        QString filepath = QApplication::applicationDirPath();
        QString filepath = appPath;
        filepath.append("/convertion.log");
        QFile logfile(filepath);
        if(logfile.open(QIODevice::Append | QIODevice::WriteOnly))
        {

            QString logpath = path;

            logpath.append(QString(" =  %1\r\n").arg(ret));
            logfile.write(logpath.toLatin1());

            logfile.close();
        }
    }
    else
    {
        retStr = dest;
    }

    if(srcPath.compare(path) != 0)
    {
        QFile srcFile(path);
        srcFile.rename(srcPath);
    }

    return retStr;
}

