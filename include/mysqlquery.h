#ifndef MYSQLQUERY_H
#define MYSQLQUERY_H

#include <QSqlDatabase>
#include <QMap>
#include <QString>
class Media;
class Actor;
class QJsonObject;
class SongList;
class MediaList;
class QSettings;
class QSqlTableModel;
class MediaPagingQuery;
class LimitArgumnet;
class ActorPagingQuery;
class PagingQueryLimitArgu;

/*
 * 操作数据库接口
*/
class MysqlQuery
{
public:
    MysqlQuery();
    ~MysqlQuery();

public:
    void readConfigFile();
    bool openMysql(const QString databaseName="yiqiding_ktv");
    void closeMysql(const QString databaseName="yiqiding_ktv");


    bool queryStaff(QSqlQuery &query);
    /*
     * 查询数据
     * queryStr  查询sql语句
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool querySql(const QString &queryStr, QSqlQuery &query);

    /*
     * 查询歌星信息
     * argu  筛选信息
     * limitArg 分页信息
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool queryActor(const ActorPagingQuery &argu, const PagingQueryLimitArgu &limitArg, QSqlQuery &query);
    /*
     * 查询歌曲信息
     * argu  筛选信息
     * limitArg 分页信息
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool queryMedia(const MediaPagingQuery &argu, const PagingQueryLimitArgu &limitArg, QSqlQuery &query);

    /*
     * 查询歌曲信息
     * argu  筛选信息
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool queryMedia_All(const MediaPagingQuery &argu, QSqlQuery &query);
    /*
     * 查询歌星信息
     * argu  筛选信息
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool queryActor_All(const ActorPagingQuery &argu, QSqlQuery &query);

    /*
     * 根据srial_id查询歌星信息
     * serial 歌星serial_id
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool queryActorOfSerialId(const qint64 serial, QSqlQuery &query);
    /*
     * 根据srial_id查询歌曲信息
     * serial 歌曲serial_id
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool queryMediaOfSerialId(const qint64 serial, QSqlQuery &query);

    /*
     * 根据sid查询歌星信息
     * sid 歌星sid
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool queryActorOfSid(const qint64 sid, QSqlQuery &query);
    /*
     * 根据mid查询歌曲信息
     * mid 歌曲mid
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool queryMediaOfMid(const qint64 mid, QSqlQuery &query);

    /*
     * 分页查询歌单信息
     * argu  筛选信息
     * limitArg 分页信息
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool queryMediaList(const QString argu, const PagingQueryLimitArgu &limitArg, QSqlQuery &query);
    /*
     * 查询广告信息
     * argu  筛选信息
     * limitArg 分页信息
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool queryMediaList_ad(const QString argu, const PagingQueryLimitArgu &limitArg, QSqlQuery &query);
    /*
     * 查询歌单信息
     * argu  筛选信息
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool queryMediaList(const QString argu, QSqlQuery &query);

    /*
     * 查询歌单歌曲列表数据
     * condition  歌曲筛选条件
     * limitNum 创建歌曲数
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool queryMediaList_createList(QList<QStringList> &condition, QSqlQuery &query, const int &limitNum);
    /*
     * 查询歌单数据
     * condition  歌曲筛选条件
     * limitNum 创建歌曲数
     * count  该歌单创建的歌曲数目
     * return 成功true 否则 false
    */
    bool queryCount_createList(QList<QStringList> &condition,  int limitNum, int &count);

    /*
     * 查询热歌榜
     * argu  筛选信息
     * limitArg 分页信息
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool queryMediaListHot(const QString argu, const PagingQueryLimitArgu &limitArg, QSqlQuery &query);
    /*
     * 查询网络榜
     * argu  筛选信息
     * limitArg 分页信息
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool queryMediaListNet(const QString argu, const PagingQueryLimitArgu &limitArg, QSqlQuery &query);

    /*
     * 查询歌曲黑名单
     * limitArg 分页信息
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool queryMediaBlack(const PagingQueryLimitArgu &limitArg, QSqlQuery &query); ///查询黑名单
    /*
     * 查询歌星黑名单
     * limitArg 分页信息
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool queryActorBlack(const PagingQueryLimitArgu &limitArg, QSqlQuery &query); ///
    /*
     * 歌曲黑名单搜索
     * argu  筛选信息
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool queryMediaBlackSearch(const QString argu, QSqlQuery &query);
    /*
     * 歌星黑名单搜索
     * argu  筛选信息
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool queryActorBlackSearch(const QString argu, QSqlQuery &query);

    /*
     * 根据歌星或歌曲信息搜索歌曲
     * argu  筛选信息
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool querySongOrSingerSearch(const QString argu, QSqlQuery &query);
    /*
     * 歌星搜索
     * name  歌星名
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool querySingerSearch(const QString &name, QSqlQuery &query);//歌星名或拼音
    /*
     * 歌曲搜索
     * name  歌曲名
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool querySongSearch(const QString &name, QSqlQuery &query);
    /*
     * 根据serial_id搜索歌曲信息
     * serial_id  歌曲serial_id
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool querySongSearch_serial_id(qint64 serial_id, QSqlQuery &query);
    /*
     * 根据serial_id搜索歌星信息
     * serial_id  歌星serial_id
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool querySingerSearch_serial_id(qint64 serial_id, QSqlQuery &query);

    /*
     * 查询fm歌单信息
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool queryFMSongList(QSqlQuery &query);
    /*
     * 查询歌单歌曲信息
     * lid  歌单lid】
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool queryFMSongListDetail(const qint64 &lid, QSqlQuery &query);

    /*
     * 查询歌曲各种状态的数量
    */
    bool queryCountListMedia(const QString type, qint64 &count);
    bool queryCountListHot(const QString type, qint64 &count);
    bool queryCOuntListNet(const QString type, qint64 &count);
    bool queryCountMedia_M(const MediaPagingQuery &argu, qint64 &count);
    bool queryCountTotal_M(qint64 &count);
    bool queryCountEnabled_M(qint64 &count);
    bool queryCountDisabled_M(qint64 &count);
    bool queryCountIsMatch_M(qint64 &count);
    bool queryCountBlack_M(qint64 &count);

    /*
     * 查询歌曲各种属性数据
    */
    bool queryCountLanguage_M(QSqlQuery &query);
    bool queryCountType_M(QSqlQuery &query);
    bool queryCountVersion_M(QSqlQuery &query);
    bool queryCountEffect_M(QSqlQuery &query);
    bool queryCountResolution_M(QSqlQuery &query);
    bool queryCountQuality_M(QSqlQuery &query);
    bool queryCountSource_M(QSqlQuery &query);
    bool queryCountRhythm_M(QSqlQuery &query);
    bool queryCountPitch_M(QSqlQuery &query);

    /*
     * 查询歌星各种状态数量
    */
    bool queryCountActor_A(const ActorPagingQuery &argu, qint64 &count);
    bool queryCountSinger_A(qint64 &count);
    bool queryCountEnabled_A(qint64 &count);
    bool queryCountDisable_A(qint64 &count);
    bool queryCountBlack_A(qint64 &count);
    bool queryCountSex_A(QSqlQuery &query);
    bool queryCountNation_A(QSqlQuery &query);

    /*
     * 根据歌星查询歌曲信息
     * singer  歌星名
     * query  查询结果的QSqlQuery对象
     * return 成功true 否则 false
    */
    bool queryMediaOfSinger(const QString &singer, QSqlQuery &query);

    ///公播
    bool queryPublicSongIPGroupBy(QStringList &types);
    bool queryPublicSongListGroupBy(QStringList &types);
    bool deletePublicSongList(const QString &type);
//    bool queryPublicSongList();

public:
    /*
     * 获取相关属性
    */
    bool getSex(QMap<int, QString> &sex);
    bool getNation(QMap<int, QString> &nation);
    bool getMediaLanguage(QMap<int, QString> &mediaLanguage);
    bool getMediaType(QMap<int, QString> &mediaType);
    bool getMediaType(QMap<QString, QString> &mediaType);
    bool getMediaVersion(QMap<int, QString> &mediaVer);
    bool getMediaEffect(QMap<int, QString> &mediaEff); //音效
    bool getMediaResolution(QMap<int, QString> &mediaRes); //分辨率
    bool getMediaQuality(QMap<int, QString> &mediaQua);   //音质
    bool getMediaSource(QMap<int, QString> &mediaSou); //来源
    bool getMediaRthym(QMap<int, QString> &mediaRth); //节奏
    bool getMediaPitch(QMap<int, QString> &mediaPit); //音高
    bool getMidsOfSid(const qint64 &sid, QVector<qint64> &mids);


    bool getMediaListNet(QMap<int, QString> &listNet); //网络排行榜
    bool getMediaListHot(QMap<int, QString> &listHot); //热歌排行榜
    bool getMaxIndex(const QString &type, qint64 &maxIndex);
    bool getMaxMediaListIndex(qint64 &maxIndex);
    bool getMaxIndex_publicsong(qint64 &maxIndex);
    bool getMaxMediaListIndex_hotnet(const QString &hotnet, qint64 &maxIndex);
    bool getMediaList(const QString &type, QVector<qint64> &indexList);
    bool getMediaList(const QString &type, QList<MediaList> &medias);
    bool getMediaListOfMid(const qint64 &mid, QList<MediaList> &medias);
    bool getMediaListOfMid_ad(const qint64 &mid, QList<MediaList> &medias);
    bool getMediaListType(QStringList &typeList);
    bool getMediaListHotNetType(QMap<int, QString> &type, const bool isHot = true);

    bool getFmLids(const qint64 &lid, QVector<qint64> &lids);
    bool getFmMaxLid(qint64 &maxLid);
    bool getFmMaxIndex(int &maxIndex);
    bool getFmOfTitle(const QString &title, SongList &value);

    /*
     * 根据sid查找serial_id
     * return 成功true 否则 false
    */
    bool getActorSerial_id(const qint64 &sid, qint64 &serial_id);
    /*
     * 根据serial_id获取歌曲信息
     * media  歌曲信息结构体
     * return 成功true 否则 false
    */
    bool getMediaOfSerial_id(const qint64 &ser_id, Media &media);
    /*
     * 根据serial_id获取歌星信息
     * actor 歌星信息结构体
     * return 成功true 否则 false
    */
    bool getActorOfSerial_id(const qint64 &ser_id, Actor &actor);

    /*
     * 获取歌星最大serial_id
    */
    bool getActorMaxSerial_id(qint64 &serial_id);
    /*
     * 获取歌曲最大serial_id
    */
    bool getMediaMaxSerial_id(qint64 &serial_id);
    /*
     * 获取歌曲最大mid
    */
    bool getMediaMaxMid(qint64 &mid);
    /*
     * 获取歌星最大sid
    */
    bool getActorMaxSid(qint64 &sid);


public:   
    /*
     * 根据sid删除歌星信息
     * serial_id 歌星对应serial_id（在云更新使用到参数）
     * actor  歌星名 （在云更新使用到参数）
     * return 成功true 否则 false
    */
    bool deleteActor(const qint64 &sid, const qint64 &serial_id, const QString &actor);
    /*
     * 根据mid删除歌曲信息
     * serial_id 歌曲对应serial_id（在云更新使用到参数）
     * lyric  歌曲歌词路径 （在云更新使用到参数）
     * path   歌曲视频路径 （在云更新使用到参数）
     * return 成功true 否则 false
    */
    bool deleteMedia(const qint64 &mid, const qint64 &serial_id, const QString &lyric, QString &path);

//    bool deleteMuchActor(const QStringList &sidM);
//    bool deleteMuchMedia(const QStringList &midM);

    /*
     * 删除歌单歌曲
     * lid 歌单id
     * value 歌单信息
     * serial_id_lid 歌单serial_id
     * return 成功true 否则 false
    */
    bool deleteFMSonglistDetail(const qint64 &lid, const MediaList &value, const qint64 &serial_id_lid);
    /*
     * 删除歌单
     * lid 歌单id
     * title 歌单标题
     * serial_id 歌单serial_id
     * return 成功true 否则 false
    */
    bool deleteFmSonglist(const qint64 &lid, const  QString &title, const qint64 &serial_id );


    /*
     * 根据serial_id 查询歌星是否存在
    */
    qint64 isActorSerialId(qint64 serialid); // -1不存在 存在返回最大值
    /*
     * 根据serial_id 查询歌曲是否存在
    */
    qint64 isMediaSerialId(qint64 serialid);

    /*
     * 判断是否是新歌星
    */
    int isSongOfSid(const int &sid_1);

    /*
     * 歌曲上线判断是否会重复
     * name 歌曲名
     * singers 歌星名
     * language 语言类型
    */
    int mediaOnlineRepeat(const QString &name, const QString &singers, const int &language);
    /*
     * 歌星上线判断是否会重复
     * name 歌星名
     * sex 性别
     * nation 国籍
    */
    int actorOnlineRepeat(const QString &name, const int &sex, const int &nation);

public:
    /*
     * 更新歌星信息
     * sid 要更新的歌星sid
     * actor 歌星信息
     * oldActor 更新前歌星信息
     * return 成功true 否则 false
    */
    bool updateActor(const qint64 &sid, const Actor &actor, const Actor &oldActor);
    /*
     * 更新歌曲信息
     * mid 要更新的歌曲mid
     * media 歌曲信息
     * oldMedia 更新前歌曲信息
     * return 成功true 否则 false
    */
    bool updateMedia(const qint64 &mid, const Media &media, const Media &oldMedia);
    /*
     * 更新歌曲黑名单
     * mid 要更新的歌曲mid
     * black 黑名单状态
     * return 成功true 否则 false
    */
    bool updateMediaBlack(const int &mid, int black);  // 0取消黑名单  1 黑名单
    /*
     * 更新歌星黑名单
     * sid 要更新的歌星sid
     * black 黑名单状态
     * return 成功true 否则 false
    */
    bool updateActorBlack(const int &sid, int black);
    /*
     * 更新歌曲黑名单
     * mids 要更新的一组歌曲mid
     * black 黑名单状态
     * return 成功true 否则 false
    */
    bool updateMediaBlackMush(const QVector<qint64> &mids, int black);
    /*
     * 更新歌星黑名单
     * sids 要更新的一组歌星sid
     * black 黑名单状态
     * return 成功true 否则 false
    */
    bool updateActorBlackMush(const QVector<qint64> &sids, int black);
    /*
     * 更新歌曲黑名单
     * mids 要更新的一组歌曲mid
     * black 黑名单状态
     * medias 更新的所有歌曲信息 （用在云更新）
     * return 成功true 否则 false
    */
    bool updateMediaBlackMush_Medias(const QVector<qint64> &mids, const QVector<Media> &medias, int black);
    /*
     * 更新歌星黑名单
     * sids 要更新的一组歌星sid
     * black 黑名单状态
     * actors 更新的所有歌星信息 （用在云更新）
     * return 成功true 否则 false
    */
    bool updateActorBlackMush_Actors(const QVector<qint64> &sids, const QVector<Actor> &actors, int black);

    /*
     * 更新歌曲k歌比赛状态
     * mid 要更新的歌曲mid
     * isMatch K歌状态
     * return 成功true 否则 false
    */
    bool updateMatchMusic(const int &mid, int isMatch);

    /*
     * 上下移动歌单
     * mid 要更新的歌曲mid
     * isMatch K歌状态
     * return 成功true 否则 false
    */
    bool updateMoveFmLid(const qint64 &upLid, const qint64 &downLid, const qint64 &maxLid);
    /*
     * 更新FM歌单
     * value 歌单结构体
     * oldValue 旧的歌单数据
     * return 成功true 否则 false
    */
    bool updateFmLid(const SongList &value, const SongList &oldValue);
    /*
     * 更新FM歌单歌曲
     * value 歌单歌曲结构
     * conditiaon 歌单条件查询
     * serial_id_lid 歌单serial_id
     * return 成功true 否则 false
    */
    bool updateFmDetail(const MediaList &value, const MediaList &conditiaon, const qint64 &serial_id_lid);

    /*
     * 更新歌单歌曲
     * value 歌单歌曲结构
     * conditiaon 歌单条件查询
     * type 歌单type
     * return 成功true 否则 false
    */
    bool updateMediaList(const MediaList &value, const MediaList &conditiaon, const QString &type = NULL);
    /*
     * 更新公播歌曲
     * value 歌单歌曲结构
     * conditiaon 歌单条件查询
     * type 公播type
     * return 成功true 否则 false
    */
    bool updateMediaList_ad(const MediaList &value, const MediaList &conditiaon, const QString &type = NULL);

    /*
     * 添加新数据
    */
    bool insertMedia(const Media &media);
    bool insertActor(const Actor &actor);
    bool insertActor_PS(const Actor &actor);
    bool insertMedia_PS(const Media &media);
    bool insertFmDetail(const int &lid, const QStringList &value);
    bool insertFmDetail1(const qint64 &lid, const MediaList &value, const qint64 &serial_id_lid);
    bool insertFmLid(const SongList &value);    

    bool insertMediaListSongs(const int &index, const QString &detail);
    bool deleteMediaListSongs(const int &index, const QString &detail);
    bool updateMediaListSongs(const int &index, const QString &src, const QString &dest);
    bool isMediaListExistSongs(const int &index, const QString &detail);
    bool getSongsType(const int &index, QString &type);
    void getNameOrDetail(const int &index, QString &typeA);

    bool insertMediaListSinger(const QString &detail);
    bool deleteMediaListSinger(const QString &detail);
    bool updateMediaListSinger(const QString &src, const QString &dest);
    bool isMediaListExistSinger(const QString &detail);

    bool insertMediaListInfo(const int &index, const QString &detail);
    bool deleteMediaListInfo(const int &index, const QString &detail);
    bool updateMediaListInfo(const int &index, const QString &src, const QString &dest);
    bool isMediaListExistInfo(const int &index, const QString &detail);
    bool getInfoType(const int &index, QString &type);

    //删除数据
    bool deleteMedialist(const MediaList &value);
    bool deleteMediaListOne(const MediaList &value);
    bool deleteMediaListOne_ad(const MediaList &value);
    bool deleteMediaList_hotnet(const MediaList &value, QString listType = "");
    bool deletePublicSong(const MediaList &value, QString type = NULL);
    bool insertMediaList(const QString &type, const QStringList &value,
                         const QList<MediaList> &medias,
                         const QList<MediaList> &old_medias);
    bool insertMediaList_hotnet(const QString &type, const int &id, const QStringList &value);

    bool addMediaList(const QString &, const QStringList &value, QList<MediaList> medias);
    bool addMediaList_publicsong(const QString &, const QStringList &value);
    bool addMediaList_hotnet(const QString &hotnet, const QString &, const QStringList &value);
    bool replaceMediaList(int srcMid, int srcIndex,  int destMid, QString type, QString serial_id, QString hotnet = NULL);

    bool executeSql(const QString &sql);

private:
    QString getQueryStr(const QString argu, const PagingQueryLimitArgu &limitArg, QString type = NULL);
    QString getQueryStr(const QString argu);

    void JsonFile(const QJsonObject &json);

public:
    QString hostName;
    QString port;
    QString databaseName;
    QString userName;
    QString password;
    QString dataBase;

private:

public:
    QSqlDatabase db;
    QSettings *initConfig;
    QSqlQuery *_query;
};

//    QString getHotNetQueryStr(const QString argu, const PagingQueryLimitArgu &limitArg);

    ///未使用
//    bool updateFmMediaListUpDownMove(const qint64 &lid, const qint64 &index, const qint64 &mid);
//    bool addMediaList(QString &typeStr, qint64 &maxIndex, QList<qint64> &mids);
#endif // MYSQLQUERY_H
