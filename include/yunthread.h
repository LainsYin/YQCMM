#ifndef YUNTHREAD_H
#define YUNTHREAD_H

#include <QThread>
#include <QWidget>
class MysqlQuery;
class QStringList;

class YunThread : public QThread
{
    Q_OBJECT
public:
    YunThread(QObject *parent = 0);
    ~YunThread();
    void initValue(const QList<QStringList> &value, MysqlQuery *sql, QString *size);

    void run();

private:
    QString downloadFile(const QString &type, const QString &dir, const QString &url);
    bool uploadFile(const QString &type, const QString &filePath);

    bool execSql(const QStringList &info);
    void writeLogging(const QString &str);
    void messageBoxShow(const QString &title, const QString &text);

signals:
    void updateView(QStringList rowList);
    void downloadUpload(QString status);
    void updateStore();

private:
    QList<QStringList> rowList;
    QString *retSize;
    MysqlQuery *_sql;
};

#endif // YUNTHREAD_H
