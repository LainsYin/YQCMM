#ifndef CURLUPLOAD_H
#define CURLUPLOAD_H
#include <iostream>
extern "C"{
#include "curl/curl.h"
}
#include <QString>
#include <QObject>
#include <QStringList>

class QPixmap;
class QByteArray;
class QJsonObject;
/*
 * curl
*/
class CurlUpload
{

public:
    CurlUpload();
    ~CurlUpload();

public:
    /*
     * 下载歌星头像
     * actorName 歌星名字
     * filePath  下载结束文件保存的路径名
     * return 下载完成true 否则false
    */
    bool downloadActorImage( QString actorName, QString filePath);

    /*
     * 下载fm头像
     * titleName 歌单名字
     * filePath  下载结束文件保存的路径名
     * return 下载完成true 否则false
    */
    bool download_fmImage(const QString titleName, QString &savPath);

    /*
     * 云更新下载图片
     * type 图片类型（fm/avatar）
     * name  图片名字
     * savePath 文件保存位置
     * return 下载完成true 否则false
    */
    bool download_jsonIm(const QString &type, const QString &name, QString &savePath);

    /*
     * 云更新下载视频
     * type 视频或歌词（fm/avatar）
     * urlPath  url地址
     * savePath 文件保存位置
     * return 下载完成true 否则false
    */
    bool download_jsonVi(const QString &urlPath, const QString &type, QString &savePath);

    /*
     * 云更新资源资源下载
    */
    bool download_yun(const QString &type, const QString &url, const QString &savePath);

    /*
     * 上传歌星头像
     * fileName 图片名
     * filePath 文件路径
     * return 下载完成true 否则false
    */
    bool uploadActorImage(const char *fileName, const char *filePath);

    /*
     * 上传歌星头像
     * filePath 文件路径
     * ret  上传数据大小（通过ret指针获取上传服务器数据大小）
     * return 返回服务器返回数据
    */
    QString uploadActorImage(const QString &filePath, QString *ret = NULL);

    /*
     * 上传fm头像
     * filePath 文件路径
     * return 返回服务器返回数据
    */
    QString uploadFmImage(const QString &filePath);

    /*
     * 上传视频文件
     * videoPath 文件路径
     * ret  上传数据大小（通过ret指针获取上传服务器数据大小）
     * return 返回服务器返回数据
    */
    QString uploadMediaVideo(const QString videoPath, QString *ret = NULL);
    QString uploadYunVideo(const QString videoPath, const QString dir="08", QString *ret=NULL);
    /*
     * 上传歌词
     * filePath 文件路径
     * return 返回服务器返回数据
    */
    QString uploadMedialyric(const QString lyricPath);

    /*
     * post云更新log数据
     * json log数据
     * return 返回服务器数据
    */
    QString postJson(const QString &json);

    /*
     * 上传云更新压缩包
     * filename 文件路径
     * localpath
     * url
     * ret  上传数据大小（通过ret指针获取上传服务器数据大小）
     * return 上传成功true 否则false
    */
    bool uploadYQDyun(const QString &filename , const QString &localpath,
                      QString &url, QString *ret = NULL);


    QString uploadStoreUpdateStatus(const QString &urlstr, const QString &str);

    QString getRequest(std::string &url, std::string &retdata);

private:
    QString getImgPath(QString filename);
    QString getUrl(QString filename);


    CURLcode download_image( QString url,  QString save_path);
    CURLcode download_mp4( QString url,  QString save_path);
    CURLcode download_image(const char *url, const char *save_path, const char *file_name);


    int uploadFile(const char *url,
                   const char *type,
                   const char *filename,
                   const char *filepath,
                   const char *dir = "08"
                   );
    int uploadFile(QByteArray &url,
                   QByteArray &type,
                   QByteArray &fileName,
                   QByteArray &filePath
                   );

private:
    QStringList  resHostList;
    QString  resHost;
    QString  resUpload;

    QString *retSize;
};

#endif // CURLUPLOAD_H
