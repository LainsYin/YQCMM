#ifndef CONVERTIONMP4_H
#define CONVERTIONMP4_H

#include <QObject>

class ConvertionMP4 : public QObject
{
    Q_OBJECT

public:
    explicit ConvertionMP4(QObject *parent = 0);
    ~ConvertionMP4();

    ///
    /// \brief convertionFile
    /// \param srcPath
    /// \param serial_id
    /// \return  返回转换成功路径 失败返回NULL
    ///
    QString convertionFile(const QString &srcPath, const QString &serial_id);

    QString convertionFilePS(const QString &srcPath, const QString &mid);

private:
    QString appPath;
    qint64 currDiskFreeSize;
};

#endif // CONVERTIONMP4_H
