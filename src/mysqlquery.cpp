#include "mysqlquery.h"
#include "enuminfo.h"
#include "defistruct.h"
#include "releasetech.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSettings>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlField>
#include <QDateTime>

#include <QDir>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCoreApplication>

#include <QDebug>
#include <QTime>
#include <QMessageBox>
#define BLACK_YES 1
#define BLACK_NO  0

MysqlQuery::MysqlQuery()
{
    initConfig = new QSettings("SongsMaintain.conf", QSettings::IniFormat);
    initConfig->setIniCodec("UTF-8");

    _query = NULL;
    readConfigFile();   
}

MysqlQuery::~MysqlQuery()
{

}

void MysqlQuery::readConfigFile()
{
    hostName = initConfig->value("SQL/hostname").toString();
    port = initConfig->value("SQL/port").toString();
    userName = initConfig->value("SQL/username").toString();
    password = initConfig->value("SQL/password").toString();
    dataBase = initConfig->value("SQL/database").toString();
}

bool MysqlQuery::openMysql(const QString databaseName)
{    
    if (QSqlDatabase::contains(dataBase)) //"yqcdb"
        db = QSqlDatabase::database(dataBase);
    else
        db = QSqlDatabase::addDatabase("QMYSQL");

    db.setHostName(hostName);
    db.setPort(port.toInt());
    db.setDatabaseName(dataBase);
    db.setUserName(userName);
    db.setPassword(password);

    if (!db.open()){        
        QString error =  QString("数据库登录失败！\n%1").arg(db.lastError().text());
        QMessageBox::information(NULL, "错误提示", error, QMessageBox::Ok);
//        exit(0);
        return false;
    }else{
        qDebug() << "connection successful." << "  databaseName : " << databaseName;
    }

    _query = new QSqlQuery(db);
    return true;
}

void MysqlQuery::closeMysql(const QString databaseName)
{
    qDebug() << "databaseName : " << databaseName;
    if (db.isOpen()){

        QString connect = QSqlDatabase::database().connectionName();
        db.close();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase(connect);
    }
}

bool MysqlQuery::queryStaff(QSqlQuery &query)
{
    if(query.exec("select * from yqcdb.staff"))
        return true;
    else
        return false;
}

bool MysqlQuery::querySql(const QString &queryStr, QSqlQuery &query)
{
    QSqlQuery _query; //(db);
   if(_query.exec(queryStr)){
        query = _query;
        return true;
    }
    else
        return false;
}

bool MysqlQuery::queryActor(const ActorPagingQuery &argu, const PagingQueryLimitArgu &limitArg, QSqlQuery &query)
{
//    QString condition1, orderStr;
//    if (limitArg.tagPage == -1){
//        condition1 = "";
//        orderStr = "desc";
//    }
//    else if(limitArg.tagPage == paging_upPage){ //上一页
//        condition1 = QString(" where aa.serial_id > %1").arg(limitArg.limitStartVal);
//        orderStr = "asc";
//    }else if(limitArg.tagPage == paging_nextPage){ //下一页
//        condition1 = QString(" where aa.serial_id < %1").arg(limitArg.limitStartVal);
//        orderStr = "desc";
//    }else
//        return false;

//    QStringList andWhere;
//    if(argu._sex != 0 && argu._sex != -1)
//        andWhere.append(QString(" aa.sex = %1").arg(argu._sex));
//    if(argu._nation != 0 && argu._nation != -1)
//        andWhere.append(QString(" aa.nation = %1").arg(argu._nation));
//    if(argu._enabled != 0 && argu._enabled != -1)
//        andWhere.append(QString(" aa.enabled = %1").arg(argu._enabled));
//    if(!argu._stars.isEmpty())
//        andWhere.append(QString(" aa.stars = %1").arg(argu._stars));
//    if(!argu.singerName.isEmpty())
//        andWhere.append(QString(" aa.name like '%%1%' or pinyin like '%%1%' ")
//                        .arg(argu.singerName));

//    andWhere.append(" aa.enabled = 1 ");
//    andWhere.append(" aa.black != 1 ");

//    QString where = NULL;
//    for(int i=0; i<andWhere.size(); i++){
//        if (i == 0 && limitArg.tagPage == -1)
//            where.append(" where ");
//        else
//            where.append(" and ");
//        where.append(andWhere.at(i));
//    }

//    if (limitArg.tagPage == -1 && andWhere.size() == 0)
//        condition1 = "";

//    QString queryStr = QString(
//                        " select "
//                        " aa.serial_id as _serial_id, aa.name as _name, "
//                        " aa.sex as _sex, aa.nation as _nation, "
//                        " aa.pinyin as _pinyin, aa.words as _words,  "
//                        " aa.enabled as _enabled, aa.sid as _sid, aa.song_count as _song_count "
//                        " from actor as aa "
/////                        " left join actor_sex as aass on aa.sex = aass.id "
/////                        " left join actor_nation as aann on aa.nation = aann.id "
//                        " %1 "
//                        " %2 "
//                        " order by aa.serial_id %3 "
//                        " limit %4,%5;")
//                        .arg(condition1)
//                        .arg(where)
//                        .arg(orderStr)
//                        .arg(limitArg.limitOffset)
//                        .arg(limitArg.rowNum);

//    qDebug() << "query actor : " << queryStr;
//    QTime time;
//    time.start();
//    bool ok = querySql(queryStr, query);
//    qDebug()<< time.elapsed()/1000.0<<"s";
//    return ok;



    QStringList andWhere;
    andWhere.append(" aa.enabled = 1 ");
    andWhere.append(" aa.black = 0 ");
    if(argu._sex != 0 && argu._sex != -1)
        andWhere.append(QString(" aa.sex = %1").arg(argu._sex));
    if(argu._nation != 0 && argu._nation != -1)
        andWhere.append(QString(" aa.nation = %1").arg(argu._nation));
    if(argu._enabled != 0 && argu._enabled != -1)
        andWhere.append(QString(" aa.enabled = %1").arg(argu._enabled));
    if(!argu._stars.isEmpty())
        andWhere.append(QString(" aa.stars = %1").arg(argu._stars));
    if(!argu.singerName.isEmpty())
        andWhere.append(QString(" aa.name like '%%1%' or pinyin like '%%1%' ")
                        .arg(argu.singerName));

    QString where = NULL;
    for(int i=0; i<andWhere.size(); i++){
        if (i == 0)
            where.append(" where ");
        else
            where.append(" and ");
        where.append(andWhere.at(i));
    }

    QString queryStr = QString(
                        " select "
                        " aa.serial_id as _serial_id, aa.name as _name, "
                        " aa.sex as _sex, aa.nation as _nation, "
                        " aa.pinyin as _pinyin, aa.words as _words,  "
                        " aa.enabled as _enabled, aa.sid as _sid, aa.song_count as _song_count "
                        " from actor as aa "
///                        " left join actor_sex as aass on aa.sex = aass.id "
///                        " left join actor_nation as aann on aa.nation = aann.id "
                        " %1 "
                        " order by aa.sid desc "
                        " limit %2,%3;")
                        .arg(where) //order by aa.count desc, aa.head desc
                        .arg((limitArg.currentpage)*limitArg.rowNum)
                        .arg(limitArg.rowNum);

    qDebug() << "query actor : " << queryStr;
    QTime time;
    time.start();
    bool ok = querySql(queryStr, query);
    qDebug()<< time.elapsed()/1000.0<<"s";
    return ok;
}

bool MysqlQuery::queryMedia(const MediaPagingQuery &argu, const PagingQueryLimitArgu &limitArg, QSqlQuery &query)
{
    ///serial_id
//    QString condition1, orderStr;
//    if (limitArg.tagPage == -1){
//        condition1 = "";
//        orderStr = "desc";
//    }
//    else if(limitArg.tagPage == paging_upPage){ //上一页
//        condition1 = QString(">%1").arg(limitArg.limitStartVal);
//        orderStr = "asc";
//    }else if(limitArg.tagPage == paging_nextPage){ //下一页
//        condition1 = QString("<%1").arg(limitArg.limitStartVal);
//        orderStr = "desc";
//    }else
//        return false;


//    QString all("全部");
//    QString andWhere;
//    if(!(argu.language.isEmpty() || argu.language.compare(all) == 0))
//        andWhere.append(QString(" and mmll.detail = '%1'").arg(argu.language));
//    if(!(argu.type.isEmpty() || argu.type.compare(all) == 0))
//        andWhere.append(QString(" and tt.detail = '%1'").arg(argu.type));
//    if(!(argu.resolution.isEmpty() || argu.resolution.compare(all) == 0))
//        andWhere.append(QString(" and mm.resolution = '%1'").arg(argu.resolution));
//    if(!(argu.quality.isEmpty() || argu.quality.compare(all) == 0))
//        andWhere.append(QString(" and mm.quality = '%1'").arg(argu.quality));
//    if(!(argu.source.isEmpty() || argu.source.compare(all) == 0))
//        andWhere.append(QString(" and mm.source = '%1'").arg(argu.source));
//    if(!(argu.version.isEmpty() || argu.version.compare(all) == 0))
//        andWhere.append(QString(" and mm.version = '%1'").arg(argu.version));
//    if(!(argu.rthym.isEmpty() || argu.rthym.compare(all) == 0))
//        andWhere.append(QString(" and mm.rthym = '%1'").arg(argu.rthym));
//    if(!(argu.pitch.isEmpty() || argu.pitch.compare(all) == 0))
//        andWhere.append(QString(" and mm.pitch = '%1'").arg(argu.pitch));
//    if(!(argu.enabled.isEmpty() || argu.enabled.compare(all) == 0))
//        andWhere.append(QString(" and mm.enabled = '%1'").arg(argu.enabled));
//    if(!(argu.match.isEmpty() || argu.match.compare(all) == 0)){
//        andWhere.append(QString(" and mm.match != %1").arg(argu.match));
//    }

//    if(!argu.singer.isEmpty())
//        andWhere.append(QString(" and mm.singer like '%%1%'").arg(argu.singer));
//    if(!argu.name.isEmpty())
//        andWhere.append(QString(" and mm.name like '%%1%' or mm.pinyin like '%%1%' ").arg(argu.name));

//    andWhere.append(" and mm.enabled = 1 and mm.black != 1 ");

//    QString queryStr = QString(
//                        " select mm.serial_id as _serial_id, mm.name as _name, mm.singer as _singer, "
//                        " mmll.detail as _language, tt.detail as _type, mm.original_track as _original_track, "
//                        " mm.sound_track as _sound_track, mm.enabled as _enabled, mm.path as _path, "
//                        " mm.lyric as _lyric, mm.mid as _mid, mm.match as _match, "
//                        " mm.artist_sid_1 as _artist_sid_1, mm.artist_sid_2 as _artist_sid_2 "
//                        " from media as mm "
//                        " left join media_language as mmll on mm.language = mmll.id "
//                        " left join media_type as tt on mm.type = tt.id "
//                        " where mm.serial_id %1 "
//                        " %2 "
//                        " order by mm.serial_id %3 "
//                        " limit %4,%5;")
//                        .arg(condition1)
//                        .arg(andWhere)
//                        .arg(orderStr)
//                        .arg(limitArg.limitOffset)
//                        .arg(limitArg.rowNum);

//    QTime time;
//    time.start();
//    qDebug() << "queryStr : " << queryStr;
//    bool ok = querySql(queryStr, query);
//    qDebug()<< time.elapsed()/1000.0<<"s";
//    return ok;


    QString all("全部");
    QStringList andWhere;
    andWhere.append(" mm.enabled = 1 and mm.black != 1 ");
    if(!(argu.language.isEmpty() || argu.language.compare(all) == 0))
        andWhere.append(QString(" mmll.detail = '%1'").arg(argu.language));
    if(!(argu.type.isEmpty() || argu.type.compare(all) == 0))
        andWhere.append(QString(" tt.detail = '%1'").arg(argu.type));
    if(!(argu.resolution.isEmpty() || argu.resolution.compare(all) == 0))
        andWhere.append(QString(" mm.resolution = '%1'").arg(argu.resolution));
    if(!(argu.quality.isEmpty() || argu.quality.compare(all) == 0))
        andWhere.append(QString(" mm.quality = '%1'").arg(argu.quality));
    if(!(argu.source.isEmpty() || argu.source.compare(all) == 0))
        andWhere.append(QString(" mm.source = '%1'").arg(argu.source));
    if(!(argu.version.isEmpty() || argu.version.compare(all) == 0))
        andWhere.append(QString(" mm.version = '%1'").arg(argu.version));
    if(!(argu.rthym.isEmpty() || argu.rthym.compare(all) == 0))
        andWhere.append(QString(" mm.rthym = '%1'").arg(argu.rthym));
    if(!(argu.pitch.isEmpty() || argu.pitch.compare(all) == 0))
        andWhere.append(QString(" mm.pitch = '%1'").arg(argu.pitch));
    if(!(argu.enabled.isEmpty() || argu.enabled.compare(all) == 0))
        andWhere.append(QString(" mm.enabled = '%1'").arg(argu.enabled));
    if(!(argu.match.isEmpty() || argu.match.compare(all) == 0)){
        andWhere.append(QString(" mm.match != %1").arg(argu.match));
    }
    if(!argu.singer.isEmpty())
        andWhere.append(QString(" mm.singer like '%%1%'").arg(argu.singer));
    if(!argu.name.isEmpty())
        andWhere.append(QString(" mm.name like '%%1%' or mm.pinyin like '%%1%' ").arg(argu.name));

    QString whereStr;
    for(int i=0; i<andWhere.count(); i++)
    {
        if(i == 0)
            whereStr.append(andWhere.at(i));
        else{
            whereStr.append(" and ");
            whereStr.append(andWhere.at(i));
        }
    }

    QString queryStr = QString(
                        " select mm.serial_id as _serial_id, mm.name as _name, mm.singer as _singer, "
                        " mmll.detail as _language, tt.detail as _type, mm.original_track as _original_track, "
                        " mm.sound_track as _sound_track, mm.enabled as _enabled, mm.path as _path, "
                        " mm.lyric as _lyric, mm.mid as _mid, mm.match as _match, "
                        " mm.artist_sid_1 as _artist_sid_1, mm.artist_sid_2 as _artist_sid_2 "
                        " from media as mm "
                        " left join media_language as mmll on mm.language = mmll.id "
                        " left join media_type as tt on mm.type = tt.id "
                        " where  %1 "
                        " order by mm.mid desc "
                        " limit %2,%3;")
                        .arg(whereStr)  ///order by mm.words desc, mm.count desc, mm.head desc
                        .arg((limitArg.currentpage)*limitArg.rowNum)
                        .arg(limitArg.rowNum);

    QTime time;
    time.start();
    qDebug() << "queryStr : " << queryStr << " current page :  " << limitArg.currentpage;
    bool ok = querySql(queryStr, query);
    qDebug()<< time.elapsed()/1000.0<<"s";
    return ok;
}

bool MysqlQuery::queryMedia_All(const MediaPagingQuery &argu, QSqlQuery &query)
{
    QString all("全部");
    QString andWhere;
    if(!(argu.language.isEmpty() || argu.language.compare(all) == 0))
        andWhere.append(QString(" and mmll.detail = '%1'").arg(argu.language));
    if(!(argu.type.isEmpty() || argu.type.compare(all) == 0))
        andWhere.append(QString(" and tt.detail = '%1'").arg(argu.type));
    if(!(argu.resolution.isEmpty() || argu.resolution.compare(all) == 0))
        andWhere.append(QString(" and mm.resolution = '%1'").arg(argu.resolution));
    if(!(argu.quality.isEmpty() || argu.quality.compare(all) == 0))
        andWhere.append(QString(" and mm.quality = '%1'").arg(argu.quality));
    if(!(argu.source.isEmpty() || argu.source.compare(all) == 0))
        andWhere.append(QString(" and mm.source = '%1'").arg(argu.source));
    if(!(argu.version.isEmpty() || argu.version.compare(all) == 0))
        andWhere.append(QString(" and mm.version = '%1'").arg(argu.version));
    if(!(argu.rthym.isEmpty() || argu.rthym.compare(all) == 0))
        andWhere.append(QString(" and mm.rthym = '%1'").arg(argu.rthym));
    if(!(argu.pitch.isEmpty() || argu.pitch.compare(all) == 0))
        andWhere.append(QString(" and mm.pitch = '%1'").arg(argu.pitch));
    if(!(argu.enabled.isEmpty() || argu.enabled.compare(all) == 0))
        andWhere.append(QString(" and mm.enabled = '%1'").arg(argu.enabled));
    if(!(argu.match.isEmpty() || argu.match.compare(all) == 0)){
        andWhere.append(QString(" and mm.match != %1").arg(argu.match));
    }

    if(!argu.singer.isEmpty())
        andWhere.append(QString(" and mm.singer like '%1'").arg(argu.singer));
    if(!argu.name.isEmpty())
        andWhere.append(QString(" and mm.name like '%1' or mm.pinyin like '%1' ").arg(argu.name));

    andWhere.append(" and mm.enabled = 1 and mm.black != 1 ");

    if(!andWhere.isEmpty())
    {
        andWhere.replace(0, 4, "");
        andWhere.insert(0, "where ");
    }


    QString queryStr = QString(
                        " select *  from media as mm "
                        " left join media_language as mmll on mm.language = mmll.id "
                        " left join media_type as tt on mm.type = tt.id "
                        " %1 "
                        " order by mm.mid desc;")
                        .arg(andWhere);


    QTime time;
    time.start();
    qDebug() << "queryStr : " << queryStr;
    bool ok = querySql(queryStr, query);
    qDebug()<< time.elapsed()/1000.0<<"s";
    return ok;
}

bool MysqlQuery::queryActor_All(const ActorPagingQuery &argu, QSqlQuery &query)
{
    QStringList andWhere;
    if(argu._sex != 0 && argu._sex != -1)
        andWhere.append(QString(" aa.sex = %1").arg(argu._sex));
    if(argu._nation != 0 && argu._nation != -1)
        andWhere.append(QString(" aa.nation = %1").arg(argu._nation));
    if(argu._enabled != 0 && argu._enabled != -1)
        andWhere.append(QString(" aa.enabled = %1").arg(argu._enabled));
    if(!argu._stars.isEmpty())
        andWhere.append(QString(" aa.stars = %1").arg(argu._stars));
    if(!argu.singerName.isEmpty())
        andWhere.append(QString(" aa.name like '%%1%' or pinyin like '%%1%' ")
                        .arg(argu.singerName));


    andWhere.append(" aa.enabled = 1 ");
    andWhere.append(" and aa.black != 1 ");

    QString where = NULL;
    for(int i=0; i<andWhere.size(); i++){
        if (i == 0 )
            where.append(" where ");
        else
            where.append(" and ");
        where.append(andWhere.at(i));
    }

    QString queryStr = QString(
                        " select * from actor as aa "
                        " %1 "
                        " order by aa.sid desc ;")
                        .arg(where);

    qDebug() << "query actor : " << queryStr;
    QTime time;
    time.start();
    bool ok = querySql(queryStr, query);
    qDebug()<< time.elapsed()/1000.0<<"s";
    return ok;
}

bool MysqlQuery::queryActorOfSerialId(const qint64 serial, QSqlQuery &query)
{
    QString str = QString("select * from actor where serial_id = %1 ;").arg(serial);

    return querySql(str, query);
}

bool MysqlQuery::queryMediaOfSerialId(const qint64 serial, QSqlQuery &query)
{
    QString str = QString(" select "
                          " mm.mid, mm.serial_id, mm.name, mm.language, mm.type, "
                          " singer, artist_sid_1, artist_sid_2, pinyin, header, "
                          " head, words, path, lyric, original_track, "
                          " sound_track, start_volume_1, start_volume_2, prelude, effect, "
                          " version, create_time, stars, hot, count, "
                          " enabled, black, mm.match, update_time, resolution, "
                          " quality, source, rhythm, pitch, info "
                          " from media as mm "
                          " where serial_id = %1;").arg(serial);

    return querySql(str, query);
}

bool MysqlQuery::queryActorOfSid(const qint64 sid, QSqlQuery &query)
{
    QString str = QString("select * from actor where sid = %1 ;").arg(sid);

    return querySql(str, query);
}



bool MysqlQuery::queryMediaOfMid(const qint64 mid, QSqlQuery &query)
{
    QString str = QString(" select "
                          " mm.mid, mm.serial_id, mm.name, mm.language, mm.type, "
                          " singer, artist_sid_1, artist_sid_2, pinyin, header, "
                          " head, words, path, lyric, original_track, "
                          " sound_track, start_volume_1, start_volume_2, prelude, effect, "
                          " version, create_time, stars, hot, count, "
                          " enabled, black, mm.match, update_time, resolution, "
                          " quality, source, rhythm, pitch, info "
                          " from media as mm "
                          " where mid = %1;").arg(mid);

    return querySql(str, query);
}

bool MysqlQuery::queryMediaList(const QString argu, const PagingQueryLimitArgu &limitArg, QSqlQuery &query)
{
    QString queryStr = getQueryStr(argu, limitArg);
    return querySql(queryStr, query);
}

bool MysqlQuery::queryMediaList_ad(const QString argu, const PagingQueryLimitArgu &limitArg, QSqlQuery &query)
{
    QString queryStr;
    QString tableName;
    if(argu.compare("ad") == 0 || argu.compare("pro") == 0)
        tableName = QString("media_list");
    else
        tableName = QString("publicsong_list");

    QString typeStr;
    typeStr = QString(" ll.type='%1' ").arg(argu);

    queryStr =  QString(" select "
                       " mm.serial_id as _serial_id, mm.name as _name, mm.singer as _singer, mmll.detail as _language, "
                       " tt.detail as _type, ll.index as _index, mm.mid as _mid, "
                       " mm.enabled as _enabled, mm.path as _path, mm.match as _match, "
                       " mm.original_track as _original_track "
                       " from media as mm "
                       " left join media_language as mmll on mm.language = mmll.id "
                       " left join media_type as tt on mm.type = tt.id "
                       " left join %4 as ll on ll.mid=mm.mid "
                       " where %1 "
                       " and "
                       " mm.enabled = 1 "
                       " order by ll.index "
                       " limit %2, %3;"
                       ).arg(typeStr)
                        .arg((limitArg.currentpage)*limitArg.rowNum)
                        .arg(limitArg.rowNum)
                        .arg(tableName);

    return querySql(queryStr, query);
}

bool MysqlQuery::queryMediaList(const QString argu, QSqlQuery &query)
{
    QString queryStr = getQueryStr(argu);
    return querySql(queryStr, query);
}

bool MysqlQuery::queryMediaList_createList(QList<QStringList> &condition, QSqlQuery &query, const int &limitNum)
{
    QStringList temp, whereStr;
    temp = condition.at(0);
    if(!temp.isEmpty()){
        whereStr.append(QString(" create_time > %1 and create_time < %2 ").arg(temp.at(0)).arg(temp.at(1)));
    }

    if(!temp.isEmpty())
        temp.clear();
    temp = condition.at(1);
    if(!temp.isEmpty()){
        QString tempStr;
        for(int i=0; i<temp.size(); i++){
            if(i != 0)
                tempStr.append(" or ");

            tempStr.append(QString(" language = %1 ").arg(temp.at(i)));
        }

        whereStr.append(tempStr);
    }

    if(!temp.isEmpty())
        temp.clear();
    temp = condition.at(2);
    if(!temp.isEmpty()){
        QString tempStr;
        for(int i=0; i<temp.size(); i++){
            if(i != 0)
                tempStr.append(" or ");
            tempStr.append(QString(" type = %1 ").arg(temp.at(i)));
        }
        whereStr.append(tempStr);
    }

    QString filterStr;
    for(int i=0; i<whereStr.size(); i++){
        if(i != 0)
            filterStr.append(" and ");
        filterStr.append(whereStr.at(i));
    }

    QString limitStr;
    if(limitNum == 0){
        limitStr = "";
    }else{
        limitStr = QString("limit %1 ").arg(limitNum);
    }

    QString queryStr =  QString(" select mm.mid as _mid, mm.serial_id as _serial_id from media as mm "
                       " where %1 "
//                       " and "
//                       " mm.enabled = 1 "
                       " order by mm.mid desc %2;"
                       ).arg(filterStr).arg(limitStr);

    qDebug() << " queryStr : " << queryStr;

    return querySql(queryStr, query);
}

bool MysqlQuery::queryCount_createList(QList<QStringList> &condition, int limitNum, int &count)
{
    QStringList temp, whereStr;
    temp = condition.at(0);
    if(!temp.isEmpty()){
        whereStr.append(QString(" create_time > %1 and create_time < %2 ").arg(temp.at(0)).arg(temp.at(1)));
    }

    if(!temp.isEmpty())
        temp.clear();
    temp = condition.at(1);
    if(!temp.isEmpty()){
        QString tempStr;
        for(int i=0; i<temp.size(); i++){
            if(i != 0)
                tempStr.append(" or ");

            tempStr.append(QString(" language = %1 ").arg(temp.at(i)));
        }

        whereStr.append(tempStr);
    }

    if(!temp.isEmpty())
        temp.clear();
    temp = condition.at(2);
    if(!temp.isEmpty()){
        QString tempStr;
        for(int i=0; i<temp.size(); i++){
            if(i != 0)
                tempStr.append(" or ");
            tempStr.append(QString(" type = %1 ").arg(temp.at(i)));
        }
        whereStr.append(tempStr);
    }

    QString filterStr;
    for(int i=0; i<whereStr.size(); i++){
        if(i != 0)
            filterStr.append(" and ");
        filterStr.append(whereStr.at(i));
    }

    QString queryStr =  QString(" select "
                       " count(mid) as _count "
                       " from media as mm "
                       " where "
                       " %1 "
//                       " and "
//                       " mm.enabled = 1 "
                       " order by mid desc;"
                       ).arg(filterStr);

    qDebug() << " queryStr : " << queryStr;

    QSqlQuery query(db);
    if(query.exec(queryStr)){
        if(query.first()){
            count = query.value("_count").toLongLong();
            if(count > limitNum)
                count = limitNum;
            return true;
        }
    }
    return false;
}

bool MysqlQuery::queryMediaListHot(const QString argu, const PagingQueryLimitArgu &limitArg, QSqlQuery &query)
{
    QString queryStr = getQueryStr(argu, limitArg, "hot");
    return querySql(queryStr, query);
}

bool MysqlQuery::queryMediaListNet(const QString argu, const PagingQueryLimitArgu &limitArg, QSqlQuery &query)
{
    QString queryStr = getQueryStr(argu, limitArg, "net");
    return querySql(queryStr, query);
}


bool MysqlQuery::queryMediaBlack(const PagingQueryLimitArgu &limitArg, QSqlQuery &query)
{
//    QString condition1, orderStr;
//    if (limitArg.tagPage == -1){
//        condition1 = "";
//        orderStr = "desc";
//    }else if(limitArg.tagPage == paging_upPage){ //上一页
//        condition1 = QString("mm.serial_id>%1 and ").arg(limitArg.limitStartVal);
//        orderStr = "asc";
//    }else if(limitArg.tagPage == paging_nextPage){ //下一页
//        condition1 = QString("mm.serial_id<%1 and ").arg(limitArg.limitStartVal);
//        orderStr = "desc";
//    }else
//        return false;

//    QString queryStr = QString(
//                     " select "
//                     " mm.serial_id as _serial_id, mm.mid as _mid, mm.name as _name, mm.singer as _singer, "
//                     " mmll.detail as _language, tt.detail as _type, "
//                     " mm.original_track  as _original_track, mm.sound_track as _sound_track, "
//                     " mm.path as _path, mm.black as _black, mm.match as _match "
//                     " from media as mm "
//                     " left join media_language as mmll on mm.language = mmll.id "
//                     " left join media_type as tt on mm.type = tt.id "
//                     " where  %1 "
//                     " mm.enabled = 1 "
//                     " and mm.black = 1 "
//                     " order by mm.serial_id %2 "
//                     " limit %3,%4;"
//                     ).arg(condition1)
//                      .arg(orderStr)
//                      .arg(limitArg.limitOffset)
//                      .arg(limitArg.rowNum);


//    return querySql(queryStr, query);



    ////
//    QString condition1, orderStr;
//    if (limitArg.tagPage == -1){
//        condition1 = "";
//        orderStr = "desc";
//    }else if(limitArg.tagPage == paging_upPage){ //上一页
//        condition1 = QString("mm.serial_id>%1 and ").arg(limitArg.limitStartVal);
//        orderStr = "asc";
//    }else if(limitArg.tagPage == paging_nextPage){ //下一页
//        condition1 = QString("mm.serial_id<%1 and ").arg(limitArg.limitStartVal);
//        orderStr = "desc";
//    }else
//        return false;

    QString queryStr = QString(
                     " select "
                     " mm.serial_id as _serial_id, mm.mid as _mid, mm.name as _name, mm.singer as _singer, "
                     " mmll.detail as _language, tt.detail as _type, "
                     " mm.original_track  as _original_track, mm.sound_track as _sound_track, "
                     " mm.path as _path, mm.black as _black, mm.match as _match "
                     " from media as mm "
                     " left join media_language as mmll on mm.language = mmll.id "
                     " left join media_type as tt on mm.type = tt.id "
                     " where "
                     " mm.enabled = 1 "
                     " and mm.black = 1 "
                     " order by mm.mid desc "
                     " limit %1,%2;"
                     )
                      .arg((limitArg.currentpage)*limitArg.rowNum)
                      .arg(limitArg.rowNum);


    return querySql(queryStr, query);
}

bool MysqlQuery::queryActorBlack(const PagingQueryLimitArgu &limitArg, QSqlQuery &query)
{
//    QString condition1, orderStr;
//    if (limitArg.tagPage == -1){
//        condition1 = "";
//        orderStr = "desc";
//    }else if(limitArg.tagPage == paging_upPage){ //上一页
//        condition1 = QString("aa.serial_id>%1 and ").arg(limitArg.limitStartVal);
//        orderStr = "asc";
//    }else if(limitArg.tagPage == paging_nextPage){ //下一页
//        condition1 = QString("mm.serial_id<%1 and ").arg(limitArg.limitStartVal);
//        orderStr = "desc";
//    }else
//        return false;

//    QString queryStr = QString(" select "
//                     " aa.serial_id as _serial_id, aa.sid as _sid, aa.name as _name, nn.detail as _nation, ss.detail as _sex, "
//                     " aa.song_count as _song_count, aa.black as _black, aa.sid as _sid "
//                     " from actor as aa "
//                     " left join actor_nation as nn on aa.nation = nn.id "
//                     " left join actor_sex as ss on aa.sex = ss.id "
//                     " where %1 "
//                     " aa.enabled = 1 "
//                     " and aa.black !=0 "
//                     " order by aa.serial_id %2 "
//                     " limit %3,%4; "
//                     ).arg(condition1)
//                      .arg(orderStr)
//                      .arg(limitArg.limitOffset)
//                      .arg(limitArg.rowNum);

//    qDebug() << " meida black sql : " << queryStr;

//    return querySql(queryStr, query);


    ////
    QString condition1, orderStr;
    if (limitArg.tagPage == -1){
        condition1 = "";
        orderStr = "desc";
    }else if(limitArg.tagPage == paging_upPage){ //上一页
        condition1 = QString("aa.serial_id>%1 and ").arg(limitArg.limitStartVal);
        orderStr = "asc";
    }else if(limitArg.tagPage == paging_nextPage){ //下一页
        condition1 = QString("mm.serial_id<%1 and ").arg(limitArg.limitStartVal);
        orderStr = "desc";
    }else
        return false;

    QString queryStr = QString(" select "
                     " aa.serial_id as _serial_id, aa.sid as _sid, aa.name as _name, nn.detail as _nation, ss.detail as _sex, "
                     " aa.song_count as _song_count, aa.black as _black, aa.sid as _sid "
                     " from actor as aa "
                     " left join actor_nation as nn on aa.nation = nn.id "
                     " left join actor_sex as ss on aa.sex = ss.id "
                     " where "
                     " aa.enabled = 1 "
                     " and aa.black = 1 "
                     " order by aa.sid desc "
                     " limit %1,%2; "
                     ).arg((limitArg.currentpage)*limitArg.rowNum)
                      .arg(limitArg.rowNum);


    qDebug() << " meida black sql : " << queryStr;

    return querySql(queryStr, query);
}

bool MysqlQuery::queryMediaBlackSearch(const QString argu, QSqlQuery &query)
{
    QString str = QString("select "
                     " mm.mid as _mid, mm.name as _name, mm.singer as _singer, mmll.detail as _language, tt.detail as _type, "
                     " mm.original_track as _original_track, mm.sound_track as _sound_track, mm.pinyin as _pinyin, "
                     " mm.header as _header, mm.words as _words "
                     " from media as mm "
                     " left join media_language as mmll on mm.language = mmll.id "
                     " left join media_type as tt on mm.type = tt.id "
                     " where "
                     " and mm.black=1 "
                     " and mm.singer = '%1' "
                     " order by mm.mid;")
                    .arg(argu);

    return querySql(str, query);
}

bool MysqlQuery::queryActorBlackSearch(const QString argu, QSqlQuery &query)
{
    QString str = QString(" select "
                         " aa.sid as _sid, aa.name as _name, nn.detail as _nation, aa.pinyin as _pinyin, "
                         " aa.header as _header, aa.words as _words, aa.song_count as _song_count "
                         " from actor as aa "
                         " left join actor_nation as nn on aa.nation = nn.id "
                         " where "
                         " aa.black=1 "
                         " and aa.name = '%1'  "
                         " order by aa.sid;"
                         ).arg(argu);

    return querySql(str, query);
}

bool MysqlQuery::querySongOrSingerSearch(const QString argu, QSqlQuery &query)
{

    QString sqlStr = QString(" select mm.serial_id as _serial_id, mm.mid as _mid, mm.name as _name, mmll.detail as _language, mmtt.detail as _type, "
                             " mm.singer as _singer, mm.pinyin as _pinyin, mm.path as _path, "
                             " mm.enabled as _enabled, mm.black as _black, mm.match as _match, "
                             " mm.original_track as _original_track "
                             " from media as mm "
                             " left join media_language as mmll on mm.language = mmll.id "
                             " left join media_type as mmtt on mm.type = mmtt.id "
                             " where  mm.name like '%%1%' or mm.singer like '%%2%' "
                             " and mm.enabled = 1 "
                             " order by mm.mid;"
                             )
                            .arg(argu)
                            .arg(argu);

    return querySql(sqlStr, query);
}

bool MysqlQuery::querySingerSearch(const QString &name, QSqlQuery &query)
{
    QString queryStr = QString(
                        " select "
                        " aa.serial_id as _serial_id, aa.name as _name, "
                        " aass.detail as _sex, aann.detail as _nation, "
                        " aa.pinyin as _pinyin, aa.words as _words,  "
                        " aa.enabled as _enabled, aa.sid as _sid "
                        " from actor as aa "
                        " left join actor_sex as aass on aa.sex = aass.id "
                        " left join actor_nation as aann on aa.nation = aann.id "
                        " where aa.name like '%%1%' or aa.pinyin like '%%1%' "
                        " order by aa.serial_id desc;")
                        .arg(name);

    qDebug() << " add singer sql :  " << queryStr;

    return querySql(queryStr, query);
}

bool MysqlQuery::querySongSearch(const QString &name, QSqlQuery &query)
{
    QString queryStr = QString(
                " select mm.serial_id as _serial_id, mm.name as _name, mm.singer as _singer, "
                " mmll.detail as _language, tt.detail as _type, mm.original_track as _original_track, "
                " mm.sound_track as _sound_track, mm.enabled as _enabled, mm.path as _path, mm.lyric as _lyric, mm.mid as _mid "
                " from media as mm "
                " left join media_language as mmll on mm.language = mmll.id "
                " left join media_type as tt on mm.type = tt.id "
                " where mm.name like '%%1%' or mm.pinyin like '%%1%' "
                " order by mm.serial_id desc ;")
            .arg(name);

    return querySql(queryStr, query);
}

bool MysqlQuery::querySongSearch_serial_id(qint64 serial_id, QSqlQuery &query)
{
    QString queryStr = QString(
                " select mm.serial_id as _serial_id, mm.name as _name, mm.singer as _singer, "
                " mmll.detail as _language, tt.detail as _type, mm.original_track as _original_track, "
                " mm.sound_track as _sound_track, mm.enabled as _enabled, mm.path as _path, mm.lyric as _lyric, mm.mid as _mid "
                " from media as mm "
                " left join media_language as mmll on mm.language = mmll.id "
                " left join media_type as tt on mm.type = tt.id "
                " where mm.serial_id = %1 ;")
            .arg(serial_id);

    return querySql(queryStr, query);
}

bool MysqlQuery::querySingerSearch_serial_id(qint64 serial_id, QSqlQuery &query)
{
    QString queryStr = QString(
                        " select "
                        " aa.serial_id as _serial_id, aa.name as _name, "
                        " aass.detail as _sex, aann.detail as _nation, "
                        " aa.pinyin as _pinyin, aa.words as _words,  "
                        " aa.enabled as _enabled, aa.sid as _sid "
                        " from actor as aa "
                        " left join actor_sex as aass on aa.sex = aass.id "
                        " left join actor_nation as aann on aa.nation = aann.id "
                        " where aa.serial_id = %1 "
                        " order by aa.serial_id desc;")
                        .arg(serial_id);

    qDebug() << " search sql :  " << queryStr;

    return querySql(queryStr, query);
}

bool MysqlQuery::queryFMSongList(QSqlQuery &query)
{
    QString sqlStr = QString(" select lid as _lid, serial_id as _serial_id, title as _title, "
                             " image as _image, type as _type, count as _count, special as _special "
                             " from songlist order by _lid desc;");
    return querySql(sqlStr, query);
}

bool MysqlQuery::queryFMSongListDetail(const qint64 &lid, QSqlQuery &query)
{
    QString sqlStr = QString(" select  "
                             " mm.serial_id as _serial_id, mm.name as _name, mm.singer as _singer, mmll.detail as _language, "
                             " tt.detail as _type,  ss.lid as _lid, ss.index as _index, ss.mid as _mid, mm.path as _path, mm.match as _match, "
                             " mm.original_track as _original_track "
                             " from media as mm "
                             " left join media_language as mmll on mm.language = mmll.id "
                             " left join media_type as tt on mm.type = tt.id "
                             " left join songlist_detail as ss on ss.mid=mm.mid "
                             " where ss.lid=%1 "
                             " and mm.enabled = 1 "
                             " order by ss.index desc;"
                             ).arg(lid);

    return querySql(sqlStr, query);
}

bool MysqlQuery::queryCountListMedia(const QString type, qint64 &count)
{
//    QString queryStr = QString(
//                        " select count(mid) as _count "
//                        " from media_list "
//                        " where type = '%1' ;").arg(type);

    QString queryStr = QString(
                        " select count(mm.mid) as _count "
                        " from media as mm "
                        " left join media_list as ml on ml.mid = mm.mid "
                        " where ml.type = '%1' and mm.enabled=1; "
                        ).arg(type);

    qDebug() << "str  : " << queryStr;
    QSqlQuery query(db);
    if(!query.exec(queryStr))
        return false;

    if(query.next()){
        count = query.value("_count").toLongLong();
        return true;
    }
    return false;
}

bool MysqlQuery::queryCountListHot(const QString type, qint64 &count)
{
    QString queryStr = QString(
                        " select count(mid) as _count "
                        " from media_listhot_detail "
                        " where id = '%1' ;").arg(type);

    qDebug() << "str  : " << queryStr;
    QSqlQuery query(db);
    if(!query.exec(queryStr))
        return false;

    if(query.next()){
        count = query.value("_count").toLongLong();
        return true;
    }
    return false;
}

bool MysqlQuery::queryCOuntListNet(const QString type, qint64 &count)
{
    QString queryStr = QString(
                        " select count(mid) as _count "
                        " from media_listnet_detail "
                        " where id = '%1' ;").arg(type);

    qDebug() << "str  : " << queryStr;
    QSqlQuery query(db);
    if(!query.exec(queryStr))
        return false;

    if(query.next()){
        count = query.value("_count").toLongLong();
        return true;
    }
    return false;
}

bool MysqlQuery::queryCountMedia_M(const MediaPagingQuery &argu, qint64 &count)
{
    QStringList andWhere;
    if(argu._language != 0 && argu._language != -1)
        andWhere.append(QString(" mm.language = %1").arg(argu._language));
    if(argu._type != 0 && argu._type != -1)
        andWhere.append(QString(" mm.type = %1").arg(argu._type));
    if(argu._resolution != 0 && argu._resolution != -1)
        andWhere.append(QString(" mm.resolution = %1").arg(argu._resolution));
    if(argu._quality != 0 && argu._quality != -1)
        andWhere.append(QString(" mm.quality = %1").arg(argu._quality));
    if(argu._source != 0 && argu._source != -1)
        andWhere.append(QString(" mm.source = %1").arg(argu._source));
    if(argu._version != 0 && argu._source != -1)
        andWhere.append(QString(" mm.version = %1").arg(argu._version));
    if(argu._rthym != 0 && argu._source != -1)
        andWhere.append(QString(" mm.rthym = %1").arg(argu._rthym));
    if(argu._pitch != 0 && argu._source != -1)
        andWhere.append(QString(" mm.pitch = %1").arg(argu._pitch));
    if(argu._enabled != 0 && argu._enabled != -1)
        andWhere.append(QString(" mm.enabled = %1").arg(argu._enabled));
    if(argu._match != 0 && argu._match != -1){
        andWhere.append(QString(" mm.match != %1").arg(argu._match));
    }

    if(!argu.singer.isEmpty())
        andWhere.append(QString(" mm.singer like '%%1%' ").arg(argu.singer));
    if(!argu.name.isEmpty())
        andWhere.append(QString(" mm.name like '%%1%' ").arg(argu.name));

    andWhere.append(" mm.enabled = 1 ");
    andWhere.append(" mm.black != 1 ");

    QString where = NULL;
    for(int i=0; i<andWhere.size(); i++){
        if (i == 0)
            where.append(" where ");
        else
            where.append(" and ");
        where.append(andWhere.at(i));
    }

    QString queryStr = QString(
                        " select count(serial_id) as _count "
                        " from media as mm "
                        " %1 ")
                        .arg(where);



    QSqlQuery query(db);
    if(!query.exec(queryStr))
        return false;

    if(query.next()){
        count = query.value("_count").toLongLong();
        return true;
    }
    return false;
}

bool MysqlQuery::queryCountTotal_M(qint64 &count)
{
    QSqlQuery query(db);
    if(!query.exec("select count(mid) as _count from media;"))
       return false;

    if(query.next())
       count = query.value("_count").toLongLong();
    return true;
}

bool MysqlQuery::queryCountEnabled_M(qint64 &count)
{
    QSqlQuery query(db);
    if(!query.exec("select count(mid) as _count from media where enabled = 1;"))
       return false;

    if(query.next())
       count = query.value("_count").toLongLong();
    return true;
}

bool MysqlQuery::queryCountDisabled_M(qint64 &count)
{
    QSqlQuery query(db);
    if(!query.exec("select count(mid) as _count from media where enabled = 0;"))
       return false;

    if(query.next())
       count = query.value("_count").toLongLong();
    return true;
}

bool MysqlQuery::queryCountIsMatch_M(qint64 &count)
{
    QSqlQuery query(db);
    if(!query.exec("select count(mm.mid) as _count from media as mm where mm.match = 1;"))
       return false;

    if(query.next())
       count = query.value("_count").toLongLong();
    return true;
}

bool MysqlQuery::queryCountBlack_M(qint64 &count)
{
    QSqlQuery query(db);
    if(!query.exec("select count(mm.mid) as _count from media as mm where mm.enabled=1 and mm.black = 1;"))
       return false;

    if(query.next())
       count = query.value("_count").toLongLong();
    return true;
}

bool MysqlQuery::queryCountLanguage_M(QSqlQuery &query)
{
    QString str(" select count(mm.mid) as _count, ll.detail as _language "
                " from media as mm "
                " left join media_language as ll "
                " on mm.language=ll.id group by mm.language;");

    return querySql(str, query);
}

bool MysqlQuery::queryCountType_M(QSqlQuery &query)
{
    QString str(" select count(mm.mid) as _count, tt.detail as _type "
                " from media as mm "
                " left join media_type as tt "
                " on mm.type=tt.id group by mm.type; ");

    return querySql(str, query);
}

bool MysqlQuery::queryCountVersion_M(QSqlQuery &query)
{
    QString str(" select count(mm.mid) as _count, vv.name as _version "
                " from media as mm "
                " left join media_version as vv "
                " on mm.version=vv.id group by mm.version; ");

    return querySql(str, query);
}

bool MysqlQuery::queryCountEffect_M(QSqlQuery &query)
{
    QString str(" select count(mm.mid) as _count, ee.detail as _effect "
                " from media as mm "
                " left join media_effect as ee "
                " on mm.effect=ee.id group by mm.effect; ");

    return querySql(str, query);
}

bool MysqlQuery::queryCountResolution_M(QSqlQuery &query)
{
    QString str(" select count(mm.mid) as _count, rr.name as _resolution "
                " from media as mm "
                " left join media_resolution as rr "
                " on mm.resolution=rr.id group by mm.resolution; ");

    return querySql(str, query);
}

bool MysqlQuery::queryCountQuality_M(QSqlQuery &query)
{
    QString str(" select count(mm.mid) as _count, qq.name as _quality "
                " from media as mm "
                " left join media_quality as qq "
                " on mm.quality=qq.id group by mm.quality; ");

    return querySql(str, query);
}

bool MysqlQuery::queryCountSource_M(QSqlQuery &query)
{
    QString str(" select count(mm.mid) as _count, ss.name as _source "
                " from media as mm "
                " left join media_source as ss "
                " on mm.source=ss.id group by mm.source; ");

    return querySql(str, query);
}

bool MysqlQuery::queryCountRhythm_M(QSqlQuery &query)
{
    QString str(" select count(mm.mid) as _count, rr.name as _rhythm "
                " from media as mm "
                " left join media_rhythm as rr "
                " on mm.rhythm=rr.id group by mm.rhythm; ");

    return querySql(str, query);
}

bool MysqlQuery::queryCountPitch_M(QSqlQuery &query)
{
    QString str(" select count(mm.mid) as _count, pp.name as _pitch "
                " from media as mm "
                " left join media_pitch as pp "
                " on mm.pitch=pp.id group by mm.pitch; ");

    return querySql(str, query);
}

bool MysqlQuery::queryCountActor_A(const ActorPagingQuery &argu, qint64 &count)
{
    QStringList andWhere;
    if(argu._sex != 0 && argu._sex != -1)
        andWhere.append(QString(" aa.sex = %1").arg(argu._sex));
    if(argu._nation != 0 && argu._nation != -1)
        andWhere.append(QString(" aa.nation = %1").arg(argu._nation));
    if(argu._enabled != 0 && argu._enabled != -1)
        andWhere.append(QString(" aa.enabled = %1").arg(argu._enabled));
    if(!argu._stars.isEmpty())
        andWhere.append(QString(" aa.stars = %1").arg(argu._stars));
    if(!argu.singerName.isEmpty())
        andWhere.append(QString(" aa.name like '%%1%' or pinyin like '%%1%' ")
                        .arg(argu.singerName));

    andWhere.append(" aa.enabled = 1 ");
    andWhere.append(" aa.black != 1 ");
    QString where = NULL;
    for(int i=0; i<andWhere.size(); i++){
        if (i == 0)
            where.append(" where ");
        else
            where.append(" and ");
        where.append(andWhere.at(i));
    }

    QString queryStr = QString(
                        " select count(serial_id) as _count "
                        " from actor as aa "
                        " %1 ")
                        .arg(where);



    QSqlQuery query(db);
    if(!query.exec(queryStr))
        return false;

    if(query.next()){
        count = query.value("_count").toLongLong();
        return true;
    }
    return false;
}

bool MysqlQuery::queryCountSinger_A(qint64 &count)
{
    QSqlQuery query(db);
    if(!query.exec("select count(sid) as _count from actor;"))
       return false;

    if(query.next())
       count = query.value("_count").toLongLong();
    return true;
}

bool MysqlQuery::queryCountEnabled_A(qint64 &count)
{
    QSqlQuery query(db);
    if(!query.exec("select count(sid) as _count from actor where enabled=1;"))
       return false;

    if(query.next())
       count = query.value("_count").toLongLong();
    return true;
}

bool MysqlQuery::queryCountDisable_A(qint64 &count)
{
    QSqlQuery query(db);
    if(!query.exec("select count(sid) as _count from actor where enabled=0;"))
       return false;

    if(query.next())
       count = query.value("_count").toLongLong();
    return true;
}

bool MysqlQuery::queryCountBlack_A(qint64 &count)
{
    QSqlQuery query(db);
    if(!query.exec("select count(aa.sid) as _count from actor as aa where aa.enabled=1 and aa.black = 1;"))
       return false;

    if(query.next())
       count = query.value("_count").toLongLong();
    return true;
}

bool MysqlQuery::queryCountSex_A(QSqlQuery &query)
{
    QString str(" select count(aa.sid) as _count, ss.detail as _sex "
                " from actor as aa "
                " left join actor_sex as ss "
                " on aa.sex=ss.id group by aa.sex;");
    return querySql(str, query);
}

bool MysqlQuery::queryCountNation_A(QSqlQuery &query)
{
    QString str(" select count(aa.sid) as _count, nn.detail as _nation "
                " from actor as aa "
                " left join actor_nation as nn "
                " on aa.nation=nn.id group by aa.nation;");
    return querySql(str, query);
}

bool MysqlQuery::queryMediaOfSinger(const QString &singer, QSqlQuery &query)
{
    QString str = QString(
                " select mm.serial_id as _serial_id, mm.name as _name, mm.singer as _singer, "
                " mmll.detail as _language, tt.detail as _type, mm.original_track as _original_track, "
                " mm.sound_track as _sound_track, mm.enabled as _enabled, mm.path as _path, "
                " mm.lyric as _lyric, mm.mid as _mid, mm.match as _match, "
                " mm.artist_sid_1 as _artist_sid_1, mm.artist_sid_2 as _artist_sid_2 "
                " from media as mm "
                " left join media_language as mmll on mm.language = mmll.id "
                " left join media_type as tt on mm.type = tt.id "
                " where mm.singer = '%1' ;").arg(singer);

    qDebug() << "str : " << str;
    return querySql(str, query);
}

bool MysqlQuery::queryPublicSongIPGroupBy(QStringList &types)
{
    QString sqlStr = QString(
                            " select * from publicsong_ip group by type  order by id;"
                            );
    QSqlQuery query;
    if(!query.exec(sqlStr))
        return false;

    if(!types.isEmpty())
        types.clear();

    while(query.next())
    {
        QString type = query.value("type").toString();
        if(!type.isEmpty())
            types.append(type);
    }

    return true;
}

bool MysqlQuery::queryPublicSongListGroupBy(QStringList &types)
{
    QString sqlStr = QString(
                            " select * from publicsong_list group by type;"
                            );
    QSqlQuery query;
    if(!query.exec(sqlStr))
        return false;

    if(!types.isEmpty())
        types.clear();

    while(query.next())
    {
        types.append(query.value("type").toString());
    }

    return true;
}

bool MysqlQuery::deletePublicSongList(const QString &type)
{
    QString str = QString(" delete from publicsong_list where type='%1';")
                .arg(type);

    qDebug() << " delete publicsong sql str : " << str;
    QSqlQuery query(db);
    if (!query.exec(str))
        return false;
    else
        return true;
}

bool MysqlQuery::getSex(QMap<int, QString> &sex)
{
    QSqlQuery query(db);
    if(!query.exec("select * from actor_sex;")){
        return false;
    }

    if(!sex.isEmpty())
        sex.clear();

    while(query.next()){
        int index = query.value("id").toInt();
        QString str = query.value("detail").toString();         
        sex.insert(index, str);
    }
    return true;
}

bool MysqlQuery::getNation(QMap<int, QString> &nation)
{
    QSqlQuery query(db);
    if(!query.exec("select * from actor_nation;")){
        return false;
    }

    if(!nation.isEmpty())
        nation.clear();

    while(query.next()){
        int index = query.value("id").toInt();
        QString str = query.value("detail").toString();
        nation.insert(index, str);
    }
    return true;
}

bool MysqlQuery::getMediaLanguage(QMap<int, QString> &mediaLanguage)
{
    QSqlQuery query(db);
    if(!query.exec("select * from media_language;")){
        return false;
    }

    if(!mediaLanguage.isEmpty())
        mediaLanguage.clear();

    while(query.next()){
        int index = query.value("id").toInt();
         QString str = query.value("detail").toString();         
         mediaLanguage.insert(index, str);
    }
    return true;
}

bool MysqlQuery::getMediaType(QMap<int, QString> &mediaType)
{
    QSqlQuery query(db);
    if(!query.exec("select * from media_type;")){
        return false;
    }

    if(!mediaType.isEmpty())
        mediaType.clear();

    while(query.next()){
        int index = query.value("id").toInt();
         QString str = query.value("detail").toString();
         mediaType.insert(index, str);
    }
    return true;
}

bool MysqlQuery::getMediaType(QMap<QString, QString> &mediaType)
{
    QSqlQuery query(db);
    if(!query.exec("select * from media_type;")){
        return false;
    }

    if(!mediaType.isEmpty())
        mediaType.clear();

    while(query.next()){
        QString name = query.value("name").toString();
         QString str = query.value("detail").toString();
         mediaType.insert(name, str);
    }
    return true;
}

bool MysqlQuery::getMediaVersion(QMap<int, QString> &mediaVer)
{
    QSqlQuery query(db);
    if(!query.exec("select * from media_version;"))
        return false;

    if(!mediaVer.isEmpty())
        mediaVer.clear();

    while(query.next()){
        int index = query.value("id").toInt();
         QString str = query.value("name").toString();
         mediaVer.insert(index, str);
    }
    return true;
}

bool MysqlQuery::getMediaEffect(QMap<int, QString> &mediaEff)
{
    QSqlQuery query(db);
    if(!query.exec("select * from media_effect;"))
        return false;

    if(!mediaEff.isEmpty())
        mediaEff.clear();

    while(query.next()){
        int index = query.value("id").toInt();
         QString str = query.value("detail").toString();
         mediaEff.insert(index, str);
    }
    return true;
}

bool MysqlQuery::getMediaResolution(QMap<int, QString> &mediaRes)
{
    QSqlQuery query(db);
    if(!query.exec("select * from media_resolution;"))
        return false;

    if(!mediaRes.isEmpty())
        mediaRes.clear();

    while(query.next()){
        int index = query.value("id").toInt();
         QString str = query.value("name").toString();         
         mediaRes.insert(index, str);
    }
    return true;
}

bool MysqlQuery::getMediaQuality(QMap<int, QString> &mediaQua)
{
    QSqlQuery query(db);
    if(!query.exec("select * from media_quality;"))
        return false;

    if(!mediaQua.isEmpty())
        mediaQua.clear();

    while(query.next()){
        int index = query.value("id").toInt();
         QString str = query.value("name").toString();         
         mediaQua.insert(index, str);
    }
    return true;
}

bool MysqlQuery::getMediaSource(QMap<int, QString> &mediaSou)
{
    QSqlQuery query(db);
    if(!query.exec("select * from media_source;"))
        return false;

    if(!mediaSou.isEmpty())
        mediaSou.clear();

    while(query.next()){
        int index = query.value("id").toInt();
         QString str = query.value("name").toString();
         mediaSou.insert(index, str);
    }
    return true;
}

bool MysqlQuery::getMediaRthym(QMap<int, QString> &mediaRth)
{
    QSqlQuery query(db);
    if(!query.exec("select * from media_rhythm;"))
        return false;

    if(!mediaRth.isEmpty())
        mediaRth.clear();

    while(query.next()){
        int  index = query.value("id").toInt();
         QString str = query.value("name").toString();
         mediaRth.insert(index, str);
    }
    return true;
}

bool MysqlQuery::getMediaPitch(QMap<int, QString> &mediaPit)
{
    QSqlQuery query(db);
    if(!query.exec("select * from media_pitch;"))
        return false;

    if(!mediaPit.isEmpty())
        mediaPit.clear();

    while(query.next()){
        int index = query.value("id").toInt();
         QString str = query.value("name").toString();
         mediaPit.insert(index, str);
    }
    return true;
}

bool MysqlQuery::getMidsOfSid(const qint64 &sid, QVector<qint64> &mids)
{
    QSqlQuery query(db);
    QString str = QString(" select mid from media "
                          " where artist_sid_1 = %1 or artist_sid_2 = %2 ;")
                        .arg(sid).arg(sid);
    if(!query.exec(str))
        return false;

    while(query.next()){
         mids.append(query.value("mid").toLongLong());
    }
    return true;
}

bool MysqlQuery::getMediaListNet(QMap<int, QString> &listNet)
{
    QSqlQuery query(db);
    if(!query.exec(QString(" SELECT nn.id, nn.detail "
                           " FROM media_listnet as nn "))){
        return false;
    }

    while(query.next()){
        int index = query.value("id").toInt();
         QString str = query.value("detail").toString();
         listNet.insert(index, str);
    }

    return true;
}

bool MysqlQuery::getMediaListHot(QMap<int, QString> &listHot)
{
    QSqlQuery query(db);
    if(!query.exec(QString(" SELECT hh.id, hh.detail "
                           " FROM media_listhot as hh "))){
        return false;
    }

    while(query.next()){
        int index = query.value("id").toInt();
         QString str = query.value("detail").toString();
         listHot.insert(index, str);
    }

    return true;
}




bool MysqlQuery::getMaxIndex(const QString &type, qint64 &maxIndex)
{
    QSqlQuery query(db);    
    if(!query.exec(QString(" SELECT mmll.type, mmll.index "
                           " FROM media_list as mmll "
                           " where mmll.type='%1' order by mmll.index;").arg(type))){
        return false;
    }

    query.last();
    maxIndex = query.value("index").toLongLong();
    return true;
}

bool MysqlQuery::getMaxMediaListIndex(qint64 &maxIndex)
{
    QSqlQuery query(db);
    if(!query.exec(QString(" SELECT mmll.type, mmll.index "
                           " FROM media_list as mmll "
                           " order by mmll.index;"))){
        return false;
    }

    query.last();
    maxIndex = query.value("index").toLongLong();
    return true;
}

bool MysqlQuery::getMaxIndex_publicsong(qint64 &maxIndex)
{
    QSqlQuery query(db);
    if(!query.exec(QString(" SELECT mmll.type, mmll.index "
                           " FROM publicsong_list as mmll "
                           " order by mmll.index;"))){
        return false;
    }

    query.last();
    maxIndex = query.value("index").toLongLong();
    return true;
}

//bool MysqlQuery::getMaxIndex_hotnet(const QString &id,  const QString &hotnet, qint64 &maxIndex)
//{
//    QString hnStr;
//    if(hotnet.compare("hot") == 0)
//        hnStr = QString("media_listhot_detail");
//    else
//        hnStr = QString("media_listnet_detail");

//    QSqlQuery query(db);
//    QString str = QString(" SELECT mmll.type, mmll.index "
//                          " FROM %1 as mmll "
//                          " where mmll.id='%2' order by mmll.index;")
//                  .arg(hnStr).arg(id);
//    if(!query.exec(str)){
//        return false;
//    }

//    query.last();
//    maxIndex = query.value("index").toLongLong();
//    return true;
//}

bool MysqlQuery::getMaxMediaListIndex_hotnet(const QString &hotnet, qint64 &maxIndex)
{
    QString hnStr;
    if(hotnet.compare("hot") == 0)
        hnStr = QString("media_listhot_detail");
    else
        hnStr = QString("media_listnet_detail");

    QSqlQuery query(db);
    QString str = QString(" select * "
                          " from %1 as mmll "
                          " order by mmll.index;")
                  .arg(hnStr);
    if(query.exec(str))
    {
        if(query.last())
        {
            maxIndex = query.value("index").toLongLong();
            return true;
        }
    }

    return false;
}

bool MysqlQuery::getMediaList(const QString &type, QVector<qint64> &indexList)
{
    QSqlQuery query(db);
    if(!query.exec(QString(" SELECT mmll.type as _type, mmll.index as _index, mmll.mid as _mid"
                           " FROM media_list as mmll "
                           " where mmll.type='%1' order by mmll.index;").arg(type))){
        return false;
    }

    while(query.next()){
         qint64 list = query.value("_mid").toLongLong();
         indexList.append(list);
    }
    return true;
}

bool MysqlQuery::getMediaList(const QString &type, QList<MediaList> &medias)
{
    QSqlQuery query(db);
    if(!query.exec(QString(" SELECT mmll.type as _type, mmll.index as _index, mmll.mid as _mid "
                           " FROM media_list as mmll "
                           " where mmll.type='%1' order by mmll.index;").arg(type))){
        return false;
    }

    while(query.next()){
         MediaList list;
         list.mid = query.value("_mid").toString();
         list.index = query.value("_index").toString();
         list.type = query.value("_type").toString();
         medias.append(list);
    }
    return true;
}

bool MysqlQuery::getMediaListOfMid(const qint64 &mid, QList<MediaList> &medias)
{
    QSqlQuery query(db);
    if(!query.exec(QString(" SELECT mmll.type as _type, mmll.index as _index, mmll.mid as _mid "
                           " FROM media_list as mmll "
                           " where mmll.mid=%1;").arg(mid))){
        return false;
    }

    while(query.next()){
         MediaList list;
         list.mid = query.value("_mid").toString();
         list.index = query.value("_index").toString();
         list.type = query.value("_type").toString();
         medias.append(list);
    }
    return true;
}

bool MysqlQuery::getMediaListOfMid_ad(const qint64 &mid, QList<MediaList> &medias)
{
    QSqlQuery query(db);
    if(!query.exec(QString(" SELECT mmll.type as _type, mmll.index as _index, mmll.mid as _mid "
                           " FROM publicsong_list as mmll "
                           " where mmll.mid=%1;").arg(mid))){
        return false;
    }

    while(query.next()){
         MediaList list;
         list.mid = query.value("_mid").toString();
         list.index = query.value("_index").toString();
         list.type = query.value("_type").toString();
         medias.append(list);
    }
    return true;
}

bool MysqlQuery::getMediaListType(QStringList &typeList)
{
    QSqlQuery query(db);
    if(!query.exec(QString(" SELECT mmll.type, mmll.index, count(mmll.mid) as total "
                           " FROM media_list as mmll "
                           " group by mmll.type;"))){
        return false;
    }

    while(query.next()){
         QString typeStr = query.value("type").toString();
         typeList.append(typeStr);
    }
    return true;
}

bool MysqlQuery::getMediaListHotNetType(QMap<int, QString> &type, const bool isHot)
{
    QSqlQuery query(db);
    QString hn;
    if(isHot)
        hn = QString("media_listhot");
    else
        hn = QString("media_listnet");

    QString str = QString(" SELECT hn.id, hn.detail "
                          " FROM %1 as hn "
                          " order by hn.id asc;").arg(hn);
    if(!query.exec(str)){
        return false;
    }

    while(query.next()){
         int index = query.value("id").toInt();
         QString str = query.value("detail").toString();
         type.insert(index, str);
    }
    return true;
}

bool MysqlQuery::getFmLids(const qint64 &lid, QVector<qint64> &lids)
{
    QSqlQuery query(db);
    QString str = QString(" SELECT ssll.lid "
                          " FROM songlist_detail as ssll "
                          " WHERE ssll.lid = %1 order by ssll.index desc;").arg(lid);
    if(!query.exec(str)){
        return false;
    }

    while(query.next()){
         lids.append(query.value("lid").toLongLong());
    }
    return true;
}

bool MysqlQuery::getFmMaxLid(qint64 &maxLid)
{
    QSqlQuery query(db);
    QString str = QString(" SELECT ssll.lid "
                          " FROM songlist_detail as ssll "
                          " Group by ssll.lid Order by ssll.lid desc;");
    if(!query.exec(str)){
        return false;
    }

    query.seek(0);
    maxLid = query.value("lid").toLongLong();

    return true;
}

bool MysqlQuery::getFmMaxIndex(int &maxIndex)
{
    QSqlQuery query(db);
    QString str = QString(" SELECT ssdd.lid, ssdd.index, ssdd.mid FROM songlist_detail"
                          " as ssdd order by ssdd.index desc ;");

    if(query.exec(str)){
        query.seek(0);
        maxIndex = query.value("index").toInt();
        return true;
    }


    return false;
}

bool MysqlQuery::getFmOfTitle(const QString &title, SongList &value)
{
    QString str = QString(" select lid, serial_id, title from songlist where title = '%1' order by serial_id desc ;").arg(title);

    QSqlQuery query;
    if(!query.exec(str))
        return false;

    if(query.next())
    {
        value.lid = query.value("lid").toString();
        value.serial_id = query.value("serial_id").toString();
        value.title = query.value("title").toString();
        return true;
    }

    return false;
}

bool MysqlQuery::getActorSerial_id(const qint64 &sid, qint64 &serial_id)
{
    QString str = QString(" select sid, serial_id from actor where sid = %1 ;").arg(sid);

    QSqlQuery query;
    if(!query.exec(str))
        return false;

    if(query.next())
    {
        serial_id = query.value("serial_id").toLongLong();
        return true;
    }

    return false;
}

bool MysqlQuery::getMediaOfSerial_id(const qint64 &ser_id, Media &media)
{
    QString sqlstr = QString("select * from  media where serial_id = %1;").arg(ser_id);

    QSqlQuery query;
    if(!query.exec(sqlstr))
        return false;

    if(query.next())
    {
        media.name = query.value("name").toString();

        return true;
    }

    return false;
}

bool MysqlQuery::getActorOfSerial_id(const qint64 &ser_id, Actor &actor)
{
    QString sqlstr = QString("select * from  actor where serial_id = %1;").arg(ser_id);

    QSqlQuery query;
    if(!query.exec(sqlstr))
        return false;

    if(query.next())
    {
        actor.name = query.value("name").toString();

        return true;
    }

    return false;
}

bool MysqlQuery::getActorMaxSerial_id(qint64 &serial_id)
{
    QSqlQuery query;
    if(!query.exec("select serial_id from actor order by serial_id desc;")){
        return false;
    }

    while(query.next()){

        query.first();
        serial_id = query.value("serial_id").toLongLong();
        return true;
    }
   return false;
}

bool MysqlQuery::getMediaMaxSerial_id(qint64 &serial_id)
{
    QSqlQuery query;
    if(!query.exec("select serial_id from media order by serial_id desc;")){
        return false;
    }

    while(query.next()){

        query.first();
        serial_id = query.value("serial_id").toLongLong();
        return true;
    }

    return false;
}

bool MysqlQuery::getMediaMaxMid(qint64 &mid)
{
    QSqlQuery query;
    if(!query.exec("select mid from media order by mid desc;")){
        return false;
    }

    while(query.next()){

        query.first();
        mid = query.value("mid").toLongLong();
        return true;
    }

    return false;
}

bool MysqlQuery::getActorMaxSid(qint64 &sid)
{
    QSqlQuery query;
    if(!query.exec("select sid from actor order by sid desc;")){
        return false;
    }

    while(query.next()){

        query.first();
        sid = query.value("sid").toLongLong();
        return true;
    }
   return false;
}

bool MysqlQuery::deleteActor(const qint64 &sid,  const qint64 &serial_id, const QString &name)
{
    QString sqlstr = QString("delete from  actor where sid = %1;").arg(sid);

    QSqlQuery query;
    if(!query.exec(sqlstr))
        return false;

#ifndef YQC_TECH

#else
    QJsonObject  json;
    json.insert("sql", sqlstr);
    json.insert("serial_id", QJsonValue(serial_id));
    json.insert("name", name);
    json.insert("op", "delete");
    json.insert("type", QJsonValue(101));
    JsonFile(json);
#endif
    return true;
}

bool MysqlQuery::deleteMedia(const qint64 &mid, const qint64 &serial_id, const QString &lyric, QString &path)
{
    QString sqlstr = QString("delete from  media where mid = %1;").arg(mid);

    QSqlQuery query;
    if(!query.exec(sqlstr))
        return false;

#ifndef YQC_TECH

#else
    QJsonObject  json;
    json.insert("sql", sqlstr);
    json.insert("serial_id", QJsonValue(serial_id));
    json.insert("lyric", lyric);
    json.insert("path", path);
    json.insert("op", "delete");
    json.insert("type", QJsonValue(100));
    JsonFile(json);
#endif
    return true;
}

bool MysqlQuery::updateActor(const qint64 &sid, const Actor &actor, const Actor &oldActor)
{
    QString str = QString(" update actor as aa "
                          " set aa.serial_id = %1, aa.name = '%2', "
                          " aa.nation = %3, aa.sex = %4, "
                          " aa.pinyin = '%5', aa.header = '%6', "
                          " aa.head = '%7', aa.words = %8, "
                          " aa.stars = %9, aa.enabled = %10, "
                          " aa.song_count = %11 "
                          ).arg(actor.serial_id).arg(actor.name)
                           .arg(actor.nation).arg(actor.sex)
                           .arg(actor.pinyin).arg(actor.header)
                           .arg(actor.head).arg(actor.words)
                           .arg(actor.stars)
                           .arg(actor.enabled)
                           .arg(actor.song_count.toInt());

    if(!actor.info.isEmpty())
        str.append(QString(" , aa.info = '%1' ").arg(actor.info));

    str.append(QString(" where serial_id = %1 ;").arg(actor.serial_id));
    qDebug() << " actor save : " << str;
    qDebug() << " sid : " << sid;
    QSqlQuery query;
    if(!query.exec(str))
        return false;

#ifndef YQC_TECH

#else
    QJsonObject  json;
    json.insert("sql", str);
    json.insert("serial_id", QJsonValue(actor.serial_id.toLongLong()));
    json.insert("old_name", oldActor.name);
    json.insert("new_name", actor.name);
    json.insert("op", "update");
    json.insert("type", QJsonValue(101));
    JsonFile(json);
#endif

    return true;
}

bool MysqlQuery::updateMedia(const qint64 &mid, const Media &media, const Media &oldMedia)
{
    QString sqlstr = QString(" update media as mm set "
                          " mm.serial_id = %1, mm.name = '%2', "
                          " mm.language = %3, mm.type = %4, "
                          " mm.singer = '%5', mm.pinyin = '%6', "
                          " mm.header = '%7', mm.head = '%8', "
                          " mm.words = %9, mm.path = '%10', "
                          " mm.lyric = %11, mm.original_track = %12, "
                          " mm.sound_track = %13, mm.prelude = %14, "
                          " mm.effect = %15, mm.version = %16, "
                          " mm.create_time = %17, mm.enabled = %18, "
                          " mm.match = %19, mm.resolution = %20, "
                          " mm.quality = %21, mm.source = %22, "
                          " mm.rhythm = %23, mm.pitch = %24, "
                          " mm.update_time = '%25' "      ///   mm.stars = %20 "
                          ).arg(media.serial_id).arg(media.name)
                           .arg(media.language).arg(media.type)
                           .arg(media.singer).arg(media.pinyin)
                           .arg(media.header).arg(media.head)
                           .arg(media.words).arg(media.path.isEmpty()?"":media.path)
                           .arg(media.lyric.isEmpty()?"NULL":QString("'%1'").arg(media.lyric)).arg(media.original_track)
                           .arg(media.sound_track).arg(media.prelude.isEmpty()?"NULL":media.prelude)
                           .arg(media.effect.isEmpty()?"NULL":media.effect).arg(media.version.isEmpty()?"NULL":media.version)
                           .arg(media.create_time.isEmpty()?"NULL":QString("'%1'").arg(media.create_time)).arg(media.enabled)
                           .arg(media.match.isEmpty()?"0":media.match).arg(media.resolution.isEmpty()?"NULL":media.resolution)
                           .arg(media.quality.isEmpty()?"NULL":media.quality).arg(media.source.isEmpty()?"NULL":media.source)
                           .arg(media.rhythm.isEmpty()?"NULL":media.rhythm).arg(media.pitch.isEmpty()?"NULL":media.pitch)
                           .arg(media.update_time); //QDateTime::currentDateTime().toString("yyyy-MM-dd")

    if(!media.artist_sid_2.isEmpty())
        sqlstr.append(QString(", mm.artist_sid_2 = %1").arg(media.artist_sid_2));

    if(!media.info.isEmpty())
        sqlstr.append(QString(", mm.info = '%1' ").arg(media.info));

    sqlstr.append(QString(" where mid = %1;").arg(mid));

    qDebug() << " str : " << sqlstr;
    QSqlQuery query;
    if(!query.exec(sqlstr)){
        return false;
    }

    qint64 actor_serialid_1 = -1,  actor_serialid_2 = -1;
    qint64 sid1 = media.artist_sid_1.toLongLong();
    qint64 sid2 = media.artist_sid_2.toLongLong();
    getActorSerial_id(sid1, actor_serialid_1);
    getActorSerial_id(sid2, actor_serialid_2);

#ifndef YQC_TECH

#else
    QJsonObject mediajson;
    mediajson.insert("serial_id", QJsonValue(media.serial_id.toLongLong()));
    mediajson.insert("name", media.name);
    mediajson.insert("language", QJsonValue(media.language.toInt()));
    mediajson.insert("type", QJsonValue(media.type.toInt()));

    mediajson.insert("singer", media.singer);
    mediajson.insert("pinyin", media.pinyin);
    mediajson.insert("header", media.header);
    mediajson.insert("head", media.head);
    mediajson.insert("words", QJsonValue(media.words.toInt()));

    mediajson.insert("path", media.path);
    mediajson.insert("lyric", media.lyric);
    mediajson.insert("original_track", QJsonValue(media.original_track.toInt()));
    mediajson.insert("sound_track", QJsonValue(media.sound_track.toInt()));
    mediajson.insert("start_volume_1", QJsonValue(-1)); //media.start_volume_1.toInt()

    mediajson.insert("start_volume_2", QJsonValue(-1)); //media.start_volume_2.toInt()
    mediajson.insert("prelude", QJsonValue(media.prelude.toInt()));
    mediajson.insert("effect", QJsonValue(media.effect.toInt()));
    mediajson.insert("version", QJsonValue(media.version.toInt()));
    mediajson.insert("create_time", media.create_time);

    mediajson.insert("stars", QJsonValue(media.stars.toFloat()));
    mediajson.insert("hot", QJsonValue(media.hot.toInt()));
    mediajson.insert("count", QJsonValue(media.count.toInt()));
    mediajson.insert("enabled", QJsonValue(media.enabled.toInt()));
    mediajson.insert("black", QJsonValue(media.black.toInt()));

    mediajson.insert("match", QJsonValue(-1)); //media.match.toInt()
    mediajson.insert("update_time", media.update_time); //media.update_time
    mediajson.insert("resolution", QJsonValue(media.resolution.toInt()));
    mediajson.insert("quality", QJsonValue(media.quality.toInt()));
    mediajson.insert("source", QJsonValue(media.source.toInt()));

    mediajson.insert("rhythm", QJsonValue(media.rhythm.toInt()));
    mediajson.insert("pitch", QJsonValue(media.pitch.toInt()));
    mediajson.insert("info", media.info);


    QJsonObject  json;
    json.insert("media", QJsonValue(mediajson));
    json.insert("serial_id",QJsonValue(media.serial_id.toLongLong()));
    json.insert("new_path", media.path);
    json.insert("old_path", oldMedia.path);
    json.insert("new_lyric", media.lyric);
    json.insert("old_lyric", oldMedia.lyric);
    json.insert("actor_serialid_2", QJsonValue(actor_serialid_2));
    json.insert("actor_serialid_1", QJsonValue(actor_serialid_1));
    json.insert("op", "update");
    json.insert("type", QJsonValue(100));
    JsonFile(json);
#endif

    return true;
}

bool MysqlQuery::deleteMediaList_hotnet(const MediaList &value, QString listType)
{

    QString table_detail;
//    QString table;
    QSqlQuery query(db);
    if(listType.compare("hot") == 0){
        table_detail = "media_listhot_detail";
//        table = "media_listhot";

    }else if(listType.compare("net") == 0){

        table_detail = "media_listnet_detail";
//        table = "media_listnet";
    }else return false;

    QString detailStr;
    QString str;
    if(value.mid.isEmpty()){

        detailStr = QString(" delete from %1  "
                      " where id=%2;").arg(table_detail).arg(value.type);
//        str = QString(" delete from %1 as mmll"
//                      " where mmll.id=%2 ").arg(table).arg(value.type);

    }else{
        detailStr = QString(" delete from %1  "
                      " where id=%2 and mid=%3;")
                .arg(table_detail)
                .arg(value.type)
                .arg(value.mid);
    }
    if(query.exec(detailStr))
        return true;

    return false;
}

bool MysqlQuery::deletePublicSong(const MediaList &value, QString type)
{
    QSqlQuery query(db);
    QString detailStr;
    QString str;
    if(value.mid.isEmpty()){

        detailStr = QString(" delete from publicsong_list  "
                      " where type='%1';").arg(value.type);

    }else{
        detailStr = QString(" delete from publicsong_list  "
                      " where type='%1' and publicsong_list.index=%2;")
                .arg(value.type)
                .arg(value.index);
    }
    if(query.exec(detailStr))
        return true;

    return false;
}

bool MysqlQuery::deleteMedialist(const MediaList &value)
{
    QString str;
    if(value.mid.isEmpty()){
        str = QString(" delete from media_list  "
                      " where type='%1';")
                .arg(value.type);
    }else{
        str = QString(" delete from media_list  "
                      " where type='%1' and media_list.index=%2;")
                .arg(value.type)
                .arg(value.index);
    }

    qDebug() << " delete sql str : " << str;
    QSqlQuery query(db);
    if (!query.exec(str))
        return false;

#ifndef YQC_TECH

#else
    QJsonObject  json;
//    json.insert("sql", str);
    json.insert("serial_id", QJsonValue(value.serial_id.toLongLong()));
    json.insert("index", QJsonValue(value.index.toInt()));
    json.insert("class", QJsonValue(value.type));
    json.insert("op", "delete");
    json.insert("type", QJsonValue(2));
    JsonFile(json);
#endif

    qDebug() << " index : " << value.index;

    return true;
}

bool MysqlQuery::deleteMediaListOne(const MediaList &value)
{
    QString str;

    str = QString(" delete from media_list  "
                  " where type='%1' and index=%2;")
            .arg(value.type)
            .arg(value.index);


    qDebug() << " delete sql str : " << str;
    QSqlQuery query(db);
    if (!query.exec(str))
        return false;

#ifndef YQC_TECH

#else
    QJsonObject  json;
//    json.insert("sql", str);
    json.insert("serial_id", QJsonValue(value.serial_id.toLongLong()));
    json.insert("index", QJsonValue(value.index.toInt()));
    json.insert("class", QJsonValue(value.type));
    json.insert("op", "delete");
    json.insert("type", QJsonValue(2));
    JsonFile(json);
#endif

    return true;
}

bool MysqlQuery::deleteMediaListOne_ad(const MediaList &value)
{
    QString str;

    str = QString(" delete from publicsong_list  "
                  " where type='%1' and index=%2;")
            .arg(value.type)
            .arg(value.index);


    qDebug() << " delete sql str : " << str;
    QSqlQuery query(db);
    if (!query.exec(str))
        return false;

    return true;
}

bool MysqlQuery::deleteFMSonglistDetail(const qint64 &lid, const MediaList &value, const qint64 &serial_id_lid)
{
    QString str = QString("delete from songlist_detail where lid=%1 and mid=%2;")
                         .arg(lid).arg(value.mid);

    QSqlQuery query(db);
    if (!query.exec(str))
        return false;

#ifndef YQC_TECH

#else
    QJsonObject  json;
    json.insert("sql", str);
    json.insert("index", QJsonValue(value.index.toInt()));
    json.insert("serial_id_lid", QJsonValue(serial_id_lid));
    json.insert("op", "delete");
    json.insert("type", QJsonValue(1));
    JsonFile(json);
#endif

    return true;
}

bool MysqlQuery::deleteFmSonglist(const qint64 &lid, const QString &title, const qint64 &serial_id)
{
    QString strDetail = QString("delete from songlist_detail where lid=%1;").arg(lid);
    QString str = QString("delete from songlist where lid=%1;").arg(lid);

    QSqlQuery query(db);
    if (query.exec(strDetail))
        if (!query.exec(str))
            return false;

#ifndef YQC_TECH

#else
    QJsonObject  json;
    json.insert("sql", str);
    json.insert("serial_id", QJsonValue(serial_id));
    json.insert("title", title);
    json.insert("op", "delete");
    json.insert("type", QJsonValue(102));
    JsonFile(json);
#endif

    return true;
}

//bool MysqlQuery::deleteMedialist(const MediaList &value)
//{
//    QString str = QString("delete from media_list where type='%1' and mid=%2;")
//            .arg(value.type).arg(value.mid);

//    QSqlQuery query(db);
//    if (!query.exec(str))
//        return false;

//    return true;
//}

qint64 MysqlQuery::isActorSerialId(qint64 serialid)
{
    QSqlQuery query;
    if(!query.exec("select serial_id from actor order by serial_id")){
        return -2;
    }

    while(query.next()){
         qint64 tempid = query.value("serial_id").toLongLong();
         if(tempid == serialid){
             query.last();
             qint64 id = query.value("serial_id").toLongLong();
             return id;
         }
    }
   return -1;
}

qint64 MysqlQuery::isMediaSerialId(qint64 serialid)
{
    QSqlQuery query;
    if(!query.exec("select serial_id from media order by serial_id desc;")){
        return -2;
    }

    while(query.next()){
         qint64 tempid = query.value("serial_id").toLongLong();
         if(tempid == serialid){
             query.first();
             qint64 id = query.value("serial_id").toLongLong();
             return id;
         }
    }
    return -1;
}

int MysqlQuery::isSongOfSid(const int &sid)
{
    QSqlQuery query;
    QString str = QString(" SELECT count(serial_id) as _count " //serial_id, name, language, type, singer, artist_sid_1, artist_sid_2 "
                          " FROM media "
                          " where artist_sid_1 = %1 or artist_sid_2 = %1 ;")
                         .arg(sid);

    if(query.exec(str))
    {
        if(query.next())
            return query.value("_count").toInt();
    }

    return -1;
}

int MysqlQuery::mediaOnlineRepeat(const QString &name, const QString &singers, const int &language)
{
    QStringList singerlist = singers.split("_");
    QString sqlstr, sqlstr1;

    if(singerlist.size() >= 1)
    {
        sqlstr = QString(" select count(serial_id) as _count from media "
                              " where "
                              " name='%1' "
                              " and singer='%2'"
                              " and language=%3 ").arg(name).arg(singers).arg(language);
    }

    if(singerlist.size() == 2)
    {
        if(!(singerlist.at(0).isEmpty() || singerlist.isEmpty()))
        {
            QString singer = singerlist.at(1);
            singer.append("_");
            singer.append(singerlist.at(0));

            sqlstr1 = QString(" select count(serial_id) as _count from media "
                              " where "
                              " name='%1' "
                              " and singer='%2'"
                              " and language=%3 ").arg(name).arg(singer).arg(language);
        }
    }


    QSqlQuery query;
    if(!sqlstr.isEmpty())
    {
        if(query.exec(sqlstr))
        {
            if(query.next())
                return query.value("_count").toInt();
        }
    }

    if(!sqlstr1.isEmpty())
    {
        if(query.exec(sqlstr1))
        {
            if(query.next())
                return query.value("_count").toInt();
        }
    }

    return 0;
}

int MysqlQuery::actorOnlineRepeat(const QString &name, const int &sex, const int &nation)
{
    QString sqlstr = QString(" select count(serial_id) as _count from actor "
                              " where "
                              " name='%1' "
                              " and sex=%2 "
                              " and nation=%3 ").arg(name).arg(sex).arg(nation);

    QSqlQuery query;
    if(query.exec(sqlstr))
    {
        if(query.next())
            return query.value("_count").toInt();
    }

    return 0;
}

bool MysqlQuery::updateMediaBlack(const int &mid, int black)
{
    QSqlQuery query(db);
    if(!query.exec(QString("update media set black = %1 where mid=%2;")
                   .arg(black)
                   .arg(mid))){
        return false;
    }
    return true;
}

bool MysqlQuery::updateActorBlack(const int &sid, int black)
{
    QSqlQuery query(db);
    if(!query.exec(QString("update actor set black = %1 where sid=%2;")
                   .arg(black)
                   .arg(sid))){
        return false;
    }
    return true;
}

bool MysqlQuery::updateMediaBlackMush(const QVector<qint64> &mids, int black)
{
    QSqlQuery query(db);
    bool _ok = true;
    for (int i=0; i<mids.size(); i++){
        if(!query.exec(QString("update media set black = %1 where mid=%2;")
                       .arg(black)
                       .arg(mids.at(i)))){
            _ok = false;
        }
    }

    return _ok;
}

bool MysqlQuery::updateActorBlackMush(const QVector<qint64> &sids, int black)
{
    QSqlQuery query(db);
    bool _ok = true;
    for (int i=0; i<sids.size(); i++){
        if(!query.exec(QString("update actor set black = %1 where sid=%2;")
                       .arg(black)
                       .arg(sids.at(i)))){
            _ok = false;
        }
    }

    return _ok;
}

bool MysqlQuery::updateMediaBlackMush_Medias(const QVector<qint64> &mids, const QVector<Media> &medias, int black)
{
    QSqlQuery query;
    bool _ok = true;
    for (int i=0; i<mids.size(); i++){
        if(query.exec(QString("update media set black = %1 where mid=%2;")
                       .arg(black)
                       .arg(mids.at(i)))){

#ifndef YQC_TECH

#else
            if(i<medias.size())
            {
                Media media = medias.at(i);
                qint64 actor_serialid_1 = -1,  actor_serialid_2 = -1;
                qint64 sid1 = media.artist_sid_1.toLongLong();
                qint64 sid2 = media.artist_sid_2.toLongLong();
                getActorSerial_id(sid1, actor_serialid_1);
                getActorSerial_id(sid2, actor_serialid_2);

                QJsonObject mediajson;
                mediajson.insert("black", QJsonValue(black));
                mediajson.insert("update_time", media.update_time); //media.update_time

                QJsonObject  json;
                json.insert("media", QJsonValue(mediajson));
                json.insert("serial_id",QJsonValue(media.serial_id.toLongLong()));
                json.insert("new_path", media.path);
                json.insert("old_path", media.path);
                json.insert("new_lyric", media.lyric);
                json.insert("old_lyric", media.lyric);
                json.insert("actor_serialid_2", QJsonValue(actor_serialid_2));
                json.insert("actor_serialid_1", QJsonValue(actor_serialid_1));
                json.insert("op", "update");
                json.insert("type", QJsonValue(100));
                JsonFile(json);
            }
#endif
        }
    }


    return _ok;
}

bool MysqlQuery::updateActorBlackMush_Actors(const QVector<qint64> &sids, const QVector<Actor> &actors, int black)
{
    QSqlQuery query(db);
    bool _ok = true;
    for (int i=0; i<actors.size(); i++){

        QString str = QString("update actor set black = %1 where sid=%2;")
                .arg(black)
                .arg(sids.at(i));

#ifndef YQC_TECH

#else
        if(query.exec(str)){
            QJsonObject  json;
            json.insert("sql", str);
            json.insert("serial_id", QJsonValue(actors.at(i).serial_id.toLongLong()));
            json.insert("old_name", actors.at(i).name);
            json.insert("new_name", actors.at(i).name);
            json.insert("op", "update");
            json.insert("type", QJsonValue(101));
            JsonFile(json);
        }
#endif

    }

    return _ok;
}

bool MysqlQuery::updateMatchMusic(const int &mid, int isMatch)
{
    QSqlQuery query(db);
    QString str = QString("update media as mm set mm.match = %1 where mid=%2;")
                         .arg(isMatch)
                         .arg(mid);
    if(!query.exec(str)){
        return false;
    }
    return true;
}

bool MysqlQuery::updateMoveFmLid(const qint64 &upLid, const qint64 &downLid, const qint64 &maxLid)
{
    QSqlQuery query(db);
    QString str = QString(" update songlist_detail set lid = %1 where lid=%2").arg(maxLid + 100).arg(downLid);
    QString str1 = QString(" update songlist_detail set lid = %1 where lid=%2").arg(downLid).arg(upLid);
    QString str2 = QString(" update songlist_detail set lid = %1 where lid=%2").arg(upLid).arg(maxLid + 100);
    if(query.exec(str))
        if(query.exec(str1))
            if(query.exec(str2)){
                return true;
            }  
    return false;
}

bool MysqlQuery::updateFmLid(const SongList &value, const SongList &oldValue)
{
    QSqlQuery query(db);
    QString curStr = QString(" update songlist as ssll "
                             " set ssll.title='%1', ssll.image='%2', ssll.type='%3', "
                             " ssll.count=%4, ssll.special=%5 "
                             " where ssll.lid=%6 and ssll.serial_id=%7;")
                            .arg(value.title).arg("").arg(value.type)
                            .arg(value.count).arg(value.special)
                            .arg(value.lid).arg(value.serial_id);

//    qDebug() << "str : " << curStr;
    if(!query.exec(curStr)){
            return false;
    }

#ifndef YQC_TECH

#else
    QJsonObject  json;
    json.insert("sql", curStr);
    json.insert("serial_id", QJsonValue(oldValue.serial_id.toLongLong()));
    json.insert("new_title", value.title);
    json.insert("old_title", oldValue.title);
    json.insert("op", "update");
    json.insert("type",QJsonValue(102));
    JsonFile(json);
#endif

    return true;
}

bool MysqlQuery::updateFmDetail(const MediaList &value, const MediaList &conditiaon, const qint64 &serial_id_lid)
{

    QString str = QString(" update songlist_detail as ssdd "
                          " set ssdd.mid = %1 "
                          " where ssdd.index = %2 and ssdd.mid = %3 ;"
                          ).arg(value.mid)
                           .arg(conditiaon.index)
                           .arg(conditiaon.mid);

    QSqlQuery query(db);
    if(!query.exec(str)){
            return false;
    }

#ifndef YQC_TECH

#else
    QJsonObject  json;
    json.insert("sql", str);
    json.insert("serial_id_mid", QJsonValue(value.serial_id.toLongLong()));
    json.insert("index", QJsonValue(conditiaon.index.toInt()));
    json.insert("serial_id_lid", QJsonValue(serial_id_lid));
    json.insert("op", "update");
    json.insert("type", QJsonValue(1));
    JsonFile(json);
#endif

    return true;
}

bool MysqlQuery::updateMediaList(const MediaList &value, const MediaList &conditiaon,  const QString &type)
{


    QString str = QString(" update media_list as mmll "
                          " set mmll.mid = %1 "
                          " where mmll.index = %2 and mmll.mid = %3 ;"
                          ).arg(value.mid)
                           .arg(conditiaon.index)
                           .arg(conditiaon.mid);

    qDebug() << " move sql : " << str;
    QSqlQuery query(db);
    if(!query.exec(str)){
            return false;
    }


#ifndef YQC_TECH

#else
    QJsonObject  json;
    QString tempstr;
    json.insert("sql", str);
    tempstr = value.serial_id;
    json.insert("serial_id", QJsonValue(tempstr.toLongLong()));
    tempstr = conditiaon.index;
    json.insert("index", QJsonValue(tempstr.toInt()));
    json.insert("class", QJsonValue(conditiaon.type));
    json.insert("op", "update");
    json.insert("type", QJsonValue(2));
    JsonFile(json);
#endif

    return true;
}

bool MysqlQuery::updateMediaList_ad(const MediaList &value, const MediaList &conditiaon, const QString &type)
{
    QString typeStr;
    if(type.compare("ad") == 0 || type.compare("pro") == 0)
        typeStr = QString("media_list");
    else
        typeStr = QString("publicsong_list");

    QString str = QString(" update %4 as mmll "
                          " set mmll.mid = %1 "
                          " where mmll.index = %2 and mmll.mid = %3 ;"
                          ).arg(value.mid)
                           .arg(conditiaon.index)
                           .arg(conditiaon.mid)
                           .arg(typeStr);

    qDebug() << " move sql : " << str;
    QSqlQuery query(db);
    if(!query.exec(str)){
            return false;
    }

#ifndef YQC_TECH

#else
    if(type.compare("ad") == 0 || type.compare("pro") == 0)
    {
        QJsonObject  json;
        QString tempstr;
        json.insert("sql", str);
        tempstr = value.serial_id;
        json.insert("serial_id", QJsonValue(tempstr.toLongLong()));
        tempstr = conditiaon.index;
        json.insert("index", QJsonValue(tempstr.toInt()));
        json.insert("class", QJsonValue(conditiaon.type));
        json.insert("op", "update");
        json.insert("type", QJsonValue(2));
        JsonFile(json);
    }
#endif

    return true;

}

bool MysqlQuery::insertMedia(const Media &media)
{
    QString create_time = media.create_time.isEmpty()?"NULL":QString("'%1'").arg(media.create_time);
    QString info = media.info.isEmpty()?"NULL":QString("'%1'").arg(media.info);
    QString lyric = media.lyric.isEmpty()?"NULL":QString("'%1'").arg(media.lyric);
    QString match = media.match.isEmpty()?"0":media.match;
    QString enabled = media.enabled.isEmpty()?"1":media.enabled;
    QString path = media.path.isEmpty()?"":QString("'%1'").arg(media.path);
//    if(media.create_time.isEmpty())
//        create_time = "NULL";
//    else
//        create_time = QString('%1').arg(media.create_time);
    QString sqlstr = QString(" insert into media"
                          "(serial_id, name, language, type, "
                          " singer, artist_sid_1, artist_sid_2, pinyin, header, "
                          " head, words, path, lyric, original_track, "
                          " sound_track, start_volume_1, start_volume_2, prelude, effect, "
                          " version, create_time, stars, hot, count, "
                          " enabled, black, media.match, update_time, resolution, "
                          " quality, source, rhythm, pitch, info) "
                          " values("
                          " %1, '%2', %3, %4, "
                          " '%5', %6, %7, '%8', '%9', "
                          " '%10', %11, '%12', %13, %14, "
                          " %15, %16, %17, %18, %19, "
                          " %20, %21, %22, %23, %24, "
                          " %25, %26, %27, %28, %29, "
                          " %30, %31, %32, %33, %34 "
                          ");"
                          ).arg(media.serial_id).arg(media.name).arg(media.language).arg(media.type)
                           .arg(media.singer).arg(media.artist_sid_1).arg(media.artist_sid_2.isEmpty()?"NULL":media.artist_sid_2 ).arg(media.pinyin).arg(media.header)
                           .arg(media.head).arg(media.words).arg(media.path).arg(lyric).arg(media.original_track)
                           .arg(media.sound_track).arg("NULL").arg("NULL")
                           .arg(media.prelude.isEmpty()?"NULL":media.prelude).arg(media.effect.isEmpty()?"NULL":media.effect)
                           .arg(media.version.isEmpty()?"NULL":media.version).arg(create_time)
                           .arg("0.0").arg(1).arg(0)
                           .arg(enabled).arg(0).arg(match).arg("NULL")
                           .arg(media.resolution.isEmpty()?"NULL":media.resolution)
                           .arg(media.quality.isEmpty()?"NULL":media.quality).arg(media.source.isEmpty()?"NULL":media.source)
                           .arg(media.rhythm.isEmpty()?"NULL":media.rhythm).arg(media.pitch.isEmpty()?"NULL":media.pitch)
                           .arg(info);


    QSqlQuery query;
    bool ok = query.exec(sqlstr); //query.exec(sqlstr)
    qDebug() << "sqlstr : " << sqlstr << "\n  : " << ok;
    if(!ok){
        return false;
    }


    qint64 actor_serialid_1 = -1,  actor_serialid_2 = -1;
    qint64 sid1 = media.artist_sid_1.toLongLong();
    qint64 sid2 = media.artist_sid_2.toLongLong();
    getActorSerial_id(sid1, actor_serialid_1);
    getActorSerial_id(sid2, actor_serialid_2);

#ifndef YQC_TECH

#else
    QJsonObject mediajson;
    mediajson.insert("serial_id", QJsonValue(media.serial_id.toLongLong()));
    mediajson.insert("name", media.name);
    mediajson.insert("language", QJsonValue(media.language.toInt()));
    mediajson.insert("type", QJsonValue(media.type.toInt()));

    mediajson.insert("singer", media.singer);
    mediajson.insert("pinyin", media.pinyin);
    mediajson.insert("header", media.header);
    mediajson.insert("head", media.head);
    mediajson.insert("words", QJsonValue(media.words.toInt()));

    mediajson.insert("path", media.path);
    mediajson.insert("lyric", media.lyric);
    mediajson.insert("original_track", QJsonValue(media.original_track.toInt()));
    mediajson.insert("sound_track", QJsonValue(media.sound_track.toInt()));
    mediajson.insert("start_volume_1", QJsonValue(-1)); //media.start_volume_1.toInt()

    mediajson.insert("start_volume_2", QJsonValue(-1)); //media.start_volume_2.toInt()
    mediajson.insert("prelude", QJsonValue(media.prelude.toInt()));
    mediajson.insert("effect", QJsonValue(media.effect.toInt()));
    mediajson.insert("version", QJsonValue(media.version.toInt()));
    mediajson.insert("create_time", media.create_time);

    mediajson.insert("stars", QJsonValue(0.0));
    mediajson.insert("hot", QJsonValue(1));
    mediajson.insert("count", QJsonValue(0));
    mediajson.insert("enabled", QJsonValue(media.enabled.toInt()));
    mediajson.insert("black", QJsonValue(0));

    mediajson.insert("match", QJsonValue(-1)); //media.match.toInt()
    mediajson.insert("update_time", ""); //media.update_time
    mediajson.insert("resolution", QJsonValue(media.resolution.toInt()));
    mediajson.insert("quality", QJsonValue(media.quality.toInt()));
    mediajson.insert("source", QJsonValue(media.source.toInt()));

    mediajson.insert("rhythm", QJsonValue(media.rhythm.toInt()));
    mediajson.insert("pitch", QJsonValue(media.pitch.toInt()));
    mediajson.insert("info", media.info);


    QJsonObject  json;
    json.insert("media", QJsonValue(mediajson));
    json.insert("serial_id", QJsonValue(media.serial_id.toLongLong()));
    json.insert("lyric", media.lyric);
    json.insert("path", media.path);
    json.insert("actor_serialid_2", QJsonValue(actor_serialid_2));
    json.insert("actor_serialid_1", QJsonValue(actor_serialid_1));
    json.insert("op", "insert");
    json.insert("type", QJsonValue(100));
    JsonFile(json);

#endif

    return true;
}

bool MysqlQuery::insertActor(const Actor &actor)
{
    QString stars = actor.stars.isEmpty()?"0.0":actor.stars;
    QString enabled = actor.enabled.isEmpty()?"1":actor.enabled;
    QString str = QString(" INSERT INTO actor"
                          "(serial_id, name, nation, sex, pinyin, "
                          " header, head, words, song_count, stars, "
                          " count, actor.order, enabled, black, info)"
                          " VALUES("
                          " %1, '%2', %3, %4, '%5',"
                          " '%6', '%7', %8, %9, %10, "
                          " %11, %12, %13, %14, '%15' );"
                          ).arg(actor.serial_id).arg(actor.name).arg(actor.nation).arg(actor.sex).arg(actor.pinyin)
                           .arg(actor.header).arg(actor.head).arg(actor.words).arg(0).arg(stars)
                           .arg(0).arg("NULL").arg(enabled).arg(0).arg(actor.info);


    qDebug() << " str : " << str;

    QSqlQuery query;
    if(!query.exec(str)){
        return false;
    }

#ifndef YQC_TECH

#else
    QJsonObject  json;
    json.insert("sql", str);
    json.insert("serial_id", QJsonValue(actor.serial_id.toLongLong()));
    json.insert("name", actor.name);
    json.insert("op", "insert");
    json.insert("type", QJsonValue(101));
    JsonFile(json);
#endif

    return true;
}

bool MysqlQuery::insertActor_PS(const Actor &actor)
{
    QString stars = actor.stars.isEmpty()?"0.0":actor.stars;
    QString enabled = actor.enabled.isEmpty()?"1":actor.enabled;
    QString str = QString(" INSERT INTO actor"
                          "(serial_id, name, nation, sex, pinyin, "
                          " header, head, words, song_count, stars, "
                          " count, actor.order, enabled, black, info, sid)"
                          " VALUES("
                          " %1, '%2', %3, %4, '%5',"
                          " '%6', '%7', %8, %9, %10, "
                          " %11, %12, %13, %14, '%15', %16);"
                          ).arg(0).arg(actor.name).arg(actor.nation).arg(actor.sex).arg(actor.pinyin)
                           .arg(actor.header).arg(actor.head).arg(actor.words).arg(0).arg(stars)
                           .arg(0).arg("NULL").arg(enabled).arg(0).arg(actor.info).arg(actor.sid);


    qDebug() << " str : " << str;

    QSqlQuery query;
    if(!query.exec(str)){
        return false;
    }

    return true;
}

bool MysqlQuery::insertMedia_PS(const Media &media)
{
    QString create_time = media.create_time.isEmpty()?"NULL":QString("'%1'").arg(media.create_time);
    QString info = media.info.isEmpty()?"NULL":QString("'%1'").arg(media.info);
    QString lyric = media.lyric.isEmpty()?"NULL":QString("'%1'").arg(media.lyric);
    QString match = media.match.isEmpty()?"0":media.match;
    QString enabled = media.enabled.isEmpty()?"1":media.enabled;
    QString path = media.path.isEmpty()?"":QString("'%1'").arg(media.path);
//    if(media.create_time.isEmpty())
//        create_time = "NULL";
//    else
//        create_time = QString('%1').arg(media.create_time);
    QString sqlstr = QString(" insert into media"
                          "(serial_id, name, language, type, "
                          " singer, artist_sid_1, artist_sid_2, pinyin, header, "
                          " head, words, path, lyric, original_track, "
                          " sound_track, start_volume_1, start_volume_2, prelude, effect, "
                          " version, create_time, stars, hot, count, "
                          " enabled, black, media.match, update_time, resolution, "
                          " quality, source, rhythm, pitch, info, mid) "
                          " values("
                          " %1, '%2', %3, %4, "
                          " '%5', %6, %7, '%8', '%9', "
                          " '%10', %11, '%12', %13, %14, "
                          " %15, %16, %17, %18, %19, "
                          " %20, %21, %22, %23, %24, "
                          " %25, %26, %27, %28, %29, "
                          " %30, %31, %32, %33, %34, %35 "
                          ");"
                          ).arg("NULL").arg(media.name).arg(media.language).arg(media.type)
                           .arg(media.singer).arg(media.artist_sid_1).arg(media.artist_sid_2.isEmpty()?"NULL":media.artist_sid_2 ).arg(media.pinyin).arg(media.header)
                           .arg(media.head).arg(media.words).arg(media.path).arg(lyric).arg(media.original_track)
                           .arg(media.sound_track).arg("NULL").arg("NULL")
                           .arg(media.prelude.isEmpty()?"NULL":media.prelude).arg(media.effect.isEmpty()?"NULL":media.effect)
                           .arg(media.version.isEmpty()?"NULL":media.version).arg(create_time)
                           .arg("0.0").arg(1).arg(0)
                           .arg(enabled).arg(0).arg(match).arg("NULL")
                           .arg(media.resolution.isEmpty()?"NULL":media.resolution)
                           .arg(media.quality.isEmpty()?"NULL":media.quality).arg(media.source.isEmpty()?"NULL":media.source)
                           .arg(media.rhythm.isEmpty()?"NULL":media.rhythm).arg(media.pitch.isEmpty()?"NULL":media.pitch)
                           .arg(info).arg(media.mid);


    QSqlQuery query;
    bool ok = query.exec(sqlstr); //query.exec(sqlstr)
    qDebug() << "sqlstr : " << sqlstr << "\n  : " << ok;
    if(!ok){
        return false;
    }
    return true;
}

bool MysqlQuery::insertFmDetail(const int &lid, const QStringList &value)
{
    if(value.size() <= 0)
        return false;

    QSqlQuery query(db);
    QString deleteStr = QString(" delete from songlist_detail where lid = %1 ;").arg(lid);

    QString str = QString(" insert into songlist_detail values");
    for(int i=0; i<value.size(); i++){

        if(i != 0)
            str.append(" , ");
        str.append(QString("(%1)").arg(value.value(i)));
    }
    str.append(";");

    if(query.exec(deleteStr)){
        if(query.exec(str))
        {
            return true;
        }
    }

    return false;
}

bool MysqlQuery::insertFmDetail1(const qint64 &lid, const MediaList &value, const qint64 &serial_id_lid)
{
    int maxIndex = -1;
    bool ok = getFmMaxIndex(maxIndex);
    if(ok && maxIndex != -1)
    {
        maxIndex++;
        QString str = QString(" insert into songlist_detail "
                              " values(%1, %2, %3) ;" )
                .arg(lid).arg(maxIndex).arg(value.mid);

        QSqlQuery query(db);
        if(!query.exec(str))
            return false;

#ifndef YQC_TECH

#else
        QJsonObject  json;
        json.insert("sql", str);
        json.insert("serial_id_mid", QJsonValue(value.serial_id.toLongLong()));
        json.insert("index", QJsonValue(maxIndex));
        json.insert("serial_id_lid", QJsonValue(serial_id_lid));
        json.insert("op", "insert");
        json.insert("type", QJsonValue(1));
        JsonFile(json);
#endif

    }

    return ok;
}

bool MysqlQuery::insertFmLid(const SongList &value)
{
    QString str = QString(" insert into songlist(serial_id, title, image, type, count, special) "
                          " values(%1, '%2', '%3', '%4', %5, %6);")
                         .arg(value.serial_id)
                         .arg(value.title)
                         .arg("")
                         .arg(value.type)
                         .arg(value.count)
                         .arg(value.special.toInt());

    QSqlQuery query(db);
    if(!query.exec(str))
        return false;

#ifndef YQC_TECH

#else
    QJsonObject  json;
    json.insert("sql", str);
    json.insert("serial_id",QJsonValue(value.serial_id.toLongLong()));
    json.insert("title", value.title);
    json.insert("op", "insert");
    json.insert("type", QJsonValue(102));
    JsonFile(json);
#endif

    return true;
}

bool MysqlQuery::insertMediaList(const QString &type, const QStringList &value,
                                 const QList<MediaList> &medias,
                                 const QList<MediaList> &old_medias)
{    
    QSqlQuery query(db);
    QString deleteStr = QString(" delete from media_list where type = '%1' ;").arg(type);
    QString str = QString(" insert into media_list values ");
    if(value.size() <= 0){
        query.exec(deleteStr);
        return false;
    }

    for(int i=0; i<value.size(); i++){

        if(i != 0)
            str.append(" , ");
        str.append(QString("(%1)").arg(value.value(i)));
    }
    str.append(";");

//    qDebug() << "str : " << str;
    if(query.exec(deleteStr)){
        if(!query.exec(str))
            return false;
    }

#ifndef YQC_TECH

#else
    for(int i=0; i<old_medias.size(); i++)
    {
        QJsonObject  json;
        QString tempStr;
        QString _cla = old_medias.at(i).type;
//        _cla = _cla.replace("'", "");
        tempStr = medias.at(i).index;
        json.insert("index", QJsonValue(tempStr.toInt()));
        json.insert("class", QJsonValue(_cla));
        json.insert("op", "delete");
        json.insert("type", QJsonValue(2));
        JsonFile(json);
    }

    for(int i=0; i<medias.size(); i++)
    {
        QJsonObject  json;
        QString tempStr;
        QString _cla = medias.at(i).type;
        _cla = _cla.replace("'", "");
//        json.insert("sql", str);
        tempStr = medias.at(i).serial_id;
        json.insert("serial_id", QJsonValue(tempStr.toLongLong()));
        tempStr = medias.at(i).index;
        json.insert("index", QJsonValue(tempStr.toInt()));
        json.insert("class", QJsonValue(_cla));
        json.insert("op", "insert");
        json.insert("type", QJsonValue(2));
        JsonFile(json);
    }
#endif
    return false;
}

bool MysqlQuery::insertMediaList_hotnet(const QString &type, const int &id, const QStringList &value)
{    
    QString table_detail;
    if(type.compare("hot") == 0){
        table_detail = "media_listhot_detail";
    }else if(type.compare("net") == 0){
        table_detail = "media_listnet_detail";
    }else return false;


    QSqlQuery query(db);
    QString deleteStr = QString(" delete from %1 where id = %2 ;").arg(table_detail).arg(id);
    QString str = QString(" insert into %1 values").arg(table_detail);
    if(value.size() <= 0){
        query.exec(deleteStr);
        return false;
    }

    for(int i=0; i<value.size(); i++){

        if(i != 0)
            str.append(" , ");
        str.append(QString("(%1)").arg(value.value(i)));
    }
    str.append(";");

    if(query.exec(deleteStr)){
        if(query.exec(str))
            return true;
        else
            db.rollback();
    }

    return false;
}

bool MysqlQuery::addMediaList(const QString &, const QStringList &value, QList<MediaList> medias)
{
    QSqlQuery query(db);

    QString str = QString(" insert into media_list values");
    for(int i=0; i<value.size(); i++){

        if(i != 0)
            str.append(" , ");
        str.append(QString("(%1)").arg(value.value(i)));
    }
    str.append(";");

//    qDebug() << "str : " << str;

    if(!query.exec(str))
        return false;

#ifndef YQC_TECH

#else
    for(int i=0; i<medias.size(); i++)
    {
        QJsonObject  json;
        QString tempstr;
        json.insert("sql", str);
        tempstr = medias.at(i).serial_id;
        json.insert("serial_id", QJsonValue(tempstr.toLongLong()));
        tempstr = medias.at(i).index;
        json.insert("index", QJsonValue(tempstr.toInt()));
        json.insert("class", QJsonValue(medias.at(i).type));
        json.insert("op", "insert");
        json.insert("type", QJsonValue(2));
        JsonFile(json);
    }
#endif
    return true;
}

bool MysqlQuery::addMediaList_publicsong(const QString &, const QStringList &value)
{

    QSqlQuery query(db);
    QString str = QString(" insert into publicsong_list values");
    for(int i=0; i<value.size(); i++){

        if(i != 0)
            str.append(" , ");
        str.append(QString("(%1)").arg(value.value(i)));
    }
    str.append(";");

    if(query.exec(str))
        return true;
    else
        db.rollback();

    return false;
}

bool MysqlQuery::addMediaList_hotnet(const QString &hotnet, const QString &, const QStringList &value)
{
    QString table_detail;
    if(hotnet.compare("hot") == 0){
        table_detail = "media_listhot_detail";
    }else if(hotnet.compare("net") == 0){
        table_detail = "media_listnet_detail";
    }else return false;


    QSqlQuery query(db);
    QString str = QString(" insert into %1 values").arg(table_detail);
    for(int i=0; i<value.size(); i++){

        if(i != 0)
            str.append(" , ");
        str.append(QString("(%1)").arg(value.value(i)));
    }
    str.append(";");

    if(query.exec(str))
        return true;
    else
        db.rollback();

    return false;
}

bool MysqlQuery::replaceMediaList(int srcMid, int srcIndex, int destMid, QString type,  QString serial_id, QString hotnet)
{
    QString table_detail;
    QString table_type;
    if(hotnet.compare("hot") == 0){
        table_detail = "media_listhot_detail";
        table_type = QString(" mmll.id = %1").arg(type);
    }else if(hotnet.compare("net") == 0){
        table_detail = "media_listnet_detail";
        table_type = QString(" mmll.id = %1 ").arg(type);
    }else{
        table_detail = "media_list";
        table_type = QString(" mmll.type = '%1' ").arg(type);
    }

    QString str = QString(" update %1  as mmll"
                          " set mmll.mid = %2 "
                          " where mmll.index = %3 "
                          " and %4 "
                          " and mmll.mid = %5 ;"
                          ).arg(table_detail)
                           .arg(destMid)
                           .arg(srcIndex)
                           .arg(table_type)
                           .arg(srcMid);

    qDebug() << "replace sql : " << str;
    QSqlQuery query(db);
    if(!query.exec(str))
        return false;

#ifndef YQC_TECH

#else
    QJsonObject  json;    
    json.insert("sql", str);    
    json.insert("serial_id", QJsonValue(serial_id.toLongLong()));
    json.insert("index", QJsonValue(srcIndex));
    json.insert("class", QJsonValue(type));
    json.insert("op", "update");
    json.insert("type", QJsonValue(2));
    JsonFile(json);
#endif
    return true;
}

bool MysqlQuery::insertMediaListSongs(const int &index, const QString &detail)
{
//    QStringList list;
//    list << "歌曲语种" << "歌曲分类" << "歌曲分辨率"
//         << "歌曲质量" << "歌曲来源" << "版本类型"
//         << "歌曲节奏" << "高音类型";

    QString dataType, fieldType;
    getNameOrDetail(index, fieldType);
    if(!getSongsType(index, dataType))
        return false;

    QString str = QString(" insert into %1(%2) "
                  " values('%3') ;").arg(dataType).arg(fieldType).arg(detail);

    qDebug() << " str:  " << str;
    QSqlQuery query(db);
    if(query.exec(str)){

//        if(index != 0 && index != 1)
//            return true;

//        int id01, id02;
//        query.clear();
//        if(query.exec(QString(" select id from %1 where %2='%3' ;").arg(dataType).arg(fieldType).arg(detail))){
//            query.first();
//            id01 =  query.value("id").toInt();
//        }
//        if(query.exec(QString(" select id from %1 where %2='其他' ;").arg(dataType).arg(fieldType))){
//          query.first();
//          id02 = query.value("id").toInt();
//        }

//        str = QString(" update %1 set id = %2 where %3 = '%4';").arg(dataType).arg(id01+1).arg(fieldType).arg(detail);
//        if(query.exec(str)){
//            str = QString(" update %1 set id = %2 where %3 = '%4';").arg(dataType).arg(id01).arg(fieldType).arg("其他");
//            if(query.exec(str)){
//                str = QString(" update %1 set id = %2 where %3 = '%4';").arg(dataType).arg(id02).arg(fieldType).arg(detail);
//                query.exec(str);
//                ;
//            }

//            else
//                query.exec(QString(" update %1 set id = %2 where %3 = '%4';").arg(dataType).arg(id01).arg(fieldType).arg(detail));
//        }
        return true;
    }

    return false;
}

bool MysqlQuery::deleteMediaListSongs(const int &index, const QString &detail)
{
    QString dataType, fieldType;
    getNameOrDetail(index, fieldType);
    if(!getSongsType(index, dataType))
        return false;

    QString str = QString(" delete from %1 where %2='%3';").arg(dataType).arg(fieldType).arg(detail);
    qDebug() << " str:  " << str;
    QSqlQuery query(db);
    if(query.exec(str)){
        return true;
    }

    return false;
}

bool MysqlQuery::updateMediaListSongs(const int &index, const QString &src, const QString &dest)
{
    QString dataType, fieldType;
    getNameOrDetail(index, fieldType);
    if(!getSongsType(index, dataType))
        return false;

    QString str = QString(" update %1 set %2 = '%3' where %2='%4';").arg(dataType).arg(fieldType).arg(dest).arg(src);
    qDebug() << " str:  " << str;
    QSqlQuery query(db);
    if(query.exec(str)){
        return true;
    }

    return false;
}

bool MysqlQuery::isMediaListExistSongs(const int &index, const QString &detail)
{
    QString dataType, fieldType;
    getNameOrDetail(index, fieldType);
    if(!getSongsType(index, dataType))
        return false;

    QString str = QString(" select * from %1 where %2='%3';").arg(dataType).arg(fieldType).arg(detail);
    qDebug() << " str:  " << str;
    QSqlQuery query(db);
    if(query.exec(str)){
        if(query.size() >= 1)
        return true;
    }

    return false;
}

bool MysqlQuery::getSongsType(const int &index, QString &type)
{
    if(index == 0)
        type = QString("media_language");
    else if(index == 1)
        type = QString("media_type");
    else if(index == 2)
        type = QString("media_resolution");
    else if(index == 3)
        type = QString("media_quality");
    else if(index == 4)
        type = QString("media_source");
    else if(index == 5)
        type = QString("media_version");
    else if(index == 6)
        type = QString("media_rhythm");
    else if(index == 7)
        type = QString("media_pitch");
    else
      return false;

    return true;
}

void MysqlQuery::getNameOrDetail(const int &index, QString &typeA)
{
    if(index == 0 || index == 1)
        typeA = QString("detail");
    else
        typeA = QString("name");
}

bool MysqlQuery::insertMediaListSinger(const QString &detail)
{
    QString str;
    str = QString(" insert into actor_nation(detail) "
                  " values('%1') ;").arg(detail);

    QSqlQuery query(db);
    if(query.exec(str)){

//        int idA, idB;
//        query.clear();
//        if(query.exec(QString(" select id from actor_nation where detail='%1' ;").arg(detail))){
//            query.first();
//            idA =  query.value("id").toInt();
//        }
//        if(query.exec(QString(" select id from actor_nation where detail='其他' ;"))){
//          query.first();
//          idB = query.value("id").toInt();
//        }

//        str = QString(" update actor_nation set id = %1 where detail = '%2';").arg(idA+1).arg(detail);
//        if(query.exec(str)){
//            str = QString(" update actor_nation set id = %1 where detail = '%2';").arg(idA).arg("其他");
//            if(query.exec(str)){
//                str = QString(" update actor_nation set id = %1 where detail = '%2';").arg(idB).arg(detail);
//                query.exec(str);
//                ;
//            }

//            else
//                query.exec(QString(" update actor_nation set id = %1 where detail = '%2';").arg(idA).arg(detail));
//        }
        return true;
    }
    return false;
}

bool MysqlQuery::deleteMediaListSinger(const QString &detail)
{
    QString str;
    str = QString(" delete from actor_nation where detail='%1';").arg(detail);

    QSqlQuery query(db);
    if(query.exec(str)){
        return true;
    }

    return false;
}

bool MysqlQuery::updateMediaListSinger(const QString &src, const QString &dest)
{
    QString str;
    str = QString(" update actor_nation set detail = '%1' where detail='%2';").arg(dest).arg(src);
    QSqlQuery query(db);
    if(query.exec(str)){
        return true;
    }

    return false;
}

bool MysqlQuery::isMediaListExistSinger(const QString &detail)
{
    QString str;
    str = QString(" select * from actor_nation where detail='%1';").arg(detail);

    QSqlQuery query(db);
    if(query.exec(str)){
        if(query.size() >= 1)
        return true;
    }

    return false;
}

bool MysqlQuery::insertMediaListInfo(const int &index, const QString &detail)
{
    QString dataType;
    if(!getInfoType(index, dataType))
        return false;

   QString str = QString(" insert into %1(detail) "
                         " values('%2');").arg(dataType).arg(detail);
   qDebug() << "insert str : " << str;
   QSqlQuery query(db);
    if(query.exec(str)){
        return true;
    }
    return false;
}

bool MysqlQuery::deleteMediaListInfo(const int &index, const QString &detail)
{
    QString dataType;
    if(!getInfoType(index, dataType))
        return false;

    QString str = QString(" delete from %1 where detail='%2';").arg(dataType).arg(detail);
    qDebug() << " str:  " << str;
    QSqlQuery query(db);
    if(query.exec(str)){
        return true;
    }

    return false;
}

bool MysqlQuery::updateMediaListInfo(const int &index, const QString &src, const QString &dest)
{
    QString dataType;
    if(!getInfoType(index, dataType))
        return false;

    QString str = QString(" update %1 set detail = '%2' where detail='%3';").arg(dataType).arg(dest).arg(src);
    qDebug() << " str:  " << str;
    QSqlQuery query(db);
    if(query.exec(str)){
        return true;
    }

    return false;
}

bool MysqlQuery::isMediaListExistInfo(const int &index, const QString &detail)
{
    QString dataType;
    if(!getInfoType(index, dataType))
        return false;
    QString str = QString(" select * from %1 where detail='%2';").arg(dataType).arg(detail);
    qDebug() << " str:  " << str;
    QSqlQuery query(db);
    if(query.exec(str)){
        if(query.size() >= 1)
        return true;
    }

    return false;
}

bool MysqlQuery::getInfoType(const int &index, QString &type)
{
    if(index == 0)
        type = QString("media_listhot");
    else if(index == 1)
        type = QString("media_listnet");
    else
        return false;

    return true;
}


QString MysqlQuery::getQueryStr(const QString argu, const PagingQueryLimitArgu &limitArg,  QString type)
{
//    QString queryStr;

//    QString condition1, orderStr;
//    condition1 = " and ll.index ";
//    if (limitArg.tagPage == -1){
//        condition1 = "";
//        orderStr = "desc";
//    }
//    else if(limitArg.tagPage == paging_upPage){ //上一页
//        condition1.append(QString(" >%1 ").arg(limitArg.limitStartVal));
//        orderStr = "asc";
//    }else if(limitArg.tagPage == paging_nextPage){ //下一页
//        condition1.append(QString(" <%1 ").arg(limitArg.limitStartVal));
//        orderStr = "desc";
//    }else
//        return "";

//    QString tableName;
//    if(argu.compare("ad") == 0 || argu.compare("pro") == 0)
//        tableName = QString("media_list");
//    else
//        tableName = QString("publicsong_list");

//    QString typeStr;
//    typeStr = QString(" ll.type='%1' ").arg(argu);

//    queryStr =  QString(" select "
//                       " mm.serial_id as _serial_id, mm.name as _name, mm.singer as _singer, mmll.detail as _language, "
//                       " tt.detail as _type, ll.index as _index, mm.mid as _mid, "
//                       " mm.enabled as _enabled, mm.path as _path, mm.match as _match, "
//                       " mm.original_track as _original_track "
//                       " from media as mm "
//                       " left join media_language as mmll on mm.language = mmll.id "
//                       " left join media_type as tt on mm.type = tt.id "
//                       " left join %6 as ll on ll.mid=mm.mid "
//                       " where %1 "
//                       " %2 "
//                       " and "
//                       " mm.enabled = 1 "
//                       " order by ll.index %3 "
//                       " limit %4, %5;"
//                       ).arg(typeStr)
//                        .arg(condition1)
//                        .arg(orderStr)
//                        .arg(limitArg.limitOffset)
//                        .arg(limitArg.rowNum)
//                        .arg(tableName);

//    qDebug() << "hot net : " << queryStr;
//    return queryStr;



    QString queryStr;
    QString tableName;
    if(argu.compare("ad") == 0 || argu.compare("pro") == 0)
        tableName = QString("media_list");
    else
        tableName = QString("publicsong_list");

    QString typeStr;
    typeStr = QString(" ll.type='%1' ").arg(argu);

    queryStr =  QString(" select "
                       " mm.serial_id as _serial_id, mm.name as _name, mm.singer as _singer, mmll.detail as _language, "
                       " tt.detail as _type, ll.index as _index, mm.mid as _mid, "
                       " mm.enabled as _enabled, mm.path as _path, mm.match as _match, "
                       " mm.original_track as _original_track "
                       " from media as mm "
                       " left join media_language as mmll on mm.language = mmll.id "
                       " left join media_type as tt on mm.type = tt.id "
                       " left join media_list as ll on ll.mid=mm.mid "
                       " where %1 "
                       " and "
                       " mm.enabled = 1 "
                       " order by ll.index "
                       " limit %2, %3;"
                       ).arg(typeStr)
                        .arg((limitArg.currentpage)*limitArg.rowNum)
                        .arg(limitArg.rowNum);

    qDebug() << "meida list sqlStr : " << queryStr;
    return queryStr;
}

QString MysqlQuery::getQueryStr(const QString argu)
{
    QString queryStr =  QString(" select "
                       " mm.serial_id as _serial_id, mm.name as _name, mm.singer as _singer, mmll.detail as _language, "
                       " tt.detail as _type, ll.index as _index, mm.mid as _mid, "
                       " mm.enabled as _enabled, mm.path as _path, mm.original_track as _original_track "
                       " from media as mm "
                       " left join media_language as mmll on mm.language = mmll.id "
                       " left join media_type as tt on mm.type = tt.id "
                       " left join media_list as ll on ll.mid=mm.mid "
                       " where ll.type='%1' "
                       " and "
                       " mm.enabled = 1 "
                       " order by ll.index desc ;"
                       ).arg(argu);

    return queryStr;
}



void MysqlQuery::JsonFile(const QJsonObject &json)
{
    QString path = QCoreApplication::applicationDirPath();
    QString jsonPath = path;
    jsonPath.append("/JSON/");

    QDir dir(jsonPath);
    if(!dir.exists())
    {
        QDir createdir;
        if(createdir.mkpath(jsonPath))
        {
        }
    }

    QString  fileName = QDateTime::currentDateTime().toString("yyyy-MM-dd");        
    jsonPath.append(fileName);
    jsonPath.append(".json");


    QJsonDocument document;
    document.setObject(json);
    QByteArray array = document.toJson(QJsonDocument::Compact);
    array.append(",\r\n");
    QFile file(jsonPath);
    if(file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        file.write(array);
        file.close();
    }
}

//QString MysqlQuery::getHotNetQueryStr(const QString argu, const PagingQueryLimitArgu &limitArg, )
//{
//    QString queryStr;

//    QString condition1, orderStr;
//    condition1 = " and ll.index ";
//    if (limitArg.tagPage == -1){
//        condition1 = "";
//        orderStr = "desc";
//    }
//    else if(limitArg.tagPage == paging_upPage){ //上一页
//        condition1.append(QString(" >%1 ").arg(limitArg.limitStartVal));
//        orderStr = "asc";
//    }else if(limitArg.tagPage == paging_nextPage){ //下一页
//        condition1.append(QString(" <%1 ").arg(limitArg.limitStartVal));
//        orderStr = "desc";
//    }else
//        return false;



//    queryStr =  QString(" select "
//                       " mm.serial_id as _serial_id, mm.name as _name, mm.singer as _singer, mmll.detail as _language, "
//                       " tt.detail as _type, ll.index as _index, mm.mid as _mid, "
//                       " mm.enabled as _enabled, mm.path as _path "
//                       " from media as mm "
//                       " left join media_language as mmll on mm.language = mmll.id "
//                       " left join media_type as tt on mm.type = tt.id "
//                       " left join media_list as ll on ll.mid=mm.mid "
//                       " where ll.type='%1' "
//                       " %2 "
////                       " and "
////                       " mm.enabled = 1 "
//                       " order by ll.index %3 "
//                       " limit %4, %5;"
//                       ).arg(argu)
//                        .arg(condition1)
//                        .arg(orderStr)
//                        .arg(limitArg.limitOffset)
//                        .arg(limitArg.rowNum);

//    return queryStr;
//}

//bool MysqlQuery::updateFmMediaListUpDownMove(const qint64 &lid, const qint64 &index, const qint64 &mid)
//{
//    QSqlQuery query(db);
//    QString curStr = QString("update songlist_detail as ssdd set ssdd.mid=%1 where ssdd.lid=%2 and ssdd.index=%3;")
//                            .arg(mid).arg(lid).arg(index);

//    if(query.exec(curStr)){
//            return true;
//    }

//    return false;
//}


//bool MysqlQuery::addMediaList(QString &typeStr, qint64 &maxIndex, QList<qint64> &mids)
//{
//    QString selectStr=NULL;
//    for (int i=0; i<mids.size(); i++){
//        maxIndex++;
//        QString str = QString(" ('%1', %2, %3)")
//                                .arg(typeStr)
//                                .arg(maxIndex)
//                                .arg(mids.at(i));

//        if (i == mids.size() - 1)
//            str.append(";");
//        else
//            str.append(",");
//        selectStr.append(str);
//    }


//    QString str("insert into media_list values ");
//    str.append(selectStr);
//    qDebug() << "select str : " << str;

//    QSqlQuery query(db);
//    if (!query.exec(str))
//        return false;

//    return true;
//}
