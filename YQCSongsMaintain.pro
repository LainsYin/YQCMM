#-------------------------------------------------
#
# Project created by QtCreator 2015-05-18T17:20:50
#
#-------------------------------------------------


QT       += core gui
QT       += sql
QT       += xml
QT       += network
QT += axcontainer

LIBS     += -L$$PWD/curl/lib -llibcurl
LIBS     += -L$$PWD/zlib128-dll/lib -lzdll
INCLUDEPATH += $$PWD/zlib128-dll/minizip
DEPENDPATH += $$PWD/zlib128-dll/minizip

LIBS    += -L$$PWD/WpdPack/Lib -lPacket -lwpcap -lws2_32
INCLUDEPATH +=$$PWD/WpdPack/Include
DEPENDPATH +=$$PWD/WpdPack/Include

LIBS     += -L$$PWD/player/libvlc_lib -llibvlc -llibvlccore
INCLUDEPATH +=$$PWD/player/libvlc_include
DEPENDPATH  +=$$PWD/player/libvlc_include
INCLUDEPATH +=$$PWD/player/include
DEPENDPATH  +=$$PWD/player/include

LIBS        += -L$$PWD/VideoConvertion -lConvertion
INCLUDEPATH += $$PWD/VideoConvertion
DEPENDPATH  += $$PWD/VideoConvertion

#LIBS     += -L$$PWD/lvc/lib -lvlc-qt -lvlc-qt-widgets
#INCLUDEPATH +=$$PWD/lvc/include
#DEPENDPATH  +=$$PWD/lvc/include
#QT       += network
#LIBS     += -LE:\YIQICHANG\CODE\SongsManage\SongsMaintain -llibcurl
#LIBS     += -lvlc-qt -lvlc-qt-widgets

INCLUDEPATH +=$$PWD/include
DEPENDPATH  +=$$PWD/include

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


CONFIG += c++11
#CONFIG += warn_off
OBJECTS_DIR += obj
UI_DIR += forms
RCC_DIR += rcc
MOC_DIR += moc
DESTDIR += bin

TARGET = YQC_SM
TEMPLATE = app

SOURCES += src/main.cpp\
    src/mysqlquery.cpp \
    src/yqcdelegate.cpp \
    src/dropshadowwidget.cpp \
    src/pushbutton.cpp \
    src/liblistview.cpp \
    src/mainwidget.cpp \
    src/titlewidget.cpp \
    src/util.cpp \
    src/libinfowidget.cpp \
    src/songsquerywideget.cpp \
    src/singerquerywidget.cpp \
    src/songsonlinewidget.cpp \
    src/singeronlinewidget.cpp \
    src/songinfowidget.cpp \
    src/singerinfowdiget.cpp \
    src/songslistwidget.cpp \
    src/fmwidget.cpp \
    src/fminfowidget.cpp \
    src/adwidget.cpp \
    src/blackwidget.cpp \
    src/listinfowidget.cpp \
    src/tablemodel.cpp \
    src/pagingtableview.cpp \
    src/libinfodialog.cpp \
    src/typeindexvalue.cpp \
    src/songinfodialog.cpp \
    src/singerinfodialog.cpp \
    src/fmaddmodifydialog.cpp \
    src/singeraddmodifydialog.cpp \
    src/releasedialog.cpp \            
    src/songslistaddmodifydialog.cpp \
    src/MD5.cpp \
    src/stafflogin.cpp\    
    zlib128-dll/minizip/ioapi.c \
    zlib128-dll/minizip/mztools.c \
    zlib128-dll/minizip/unzip.c \
    zlib128-dll/minizip/zip.c \ 
    src/pinyinall.cpp \
    src/thread.cpp \
    src/curlupload.cpp \
    player/src/Audio.cpp \
    player/src/Common.cpp \
    player/src/ControlAudio.cpp \
    player/src/ControlVideo.cpp \
    player/src/Enums.cpp \
    player/src/Error.cpp \
    player/src/Instance.cpp \
    player/src/Media.cpp \
    player/src/MediaList.cpp \
    player/src/MediaListPlayer.cpp \
    player/src/MediaPlayer.cpp \
    player/src/MetaManager.cpp \
    player/src/Video.cpp \
    player/src/VideoDelegate.cpp \
    player/src/WidgetSeek.cpp \
    player/src/WidgetVideo.cpp \
    player/src/WidgetVolumeSlider.cpp \
    player/MP4Player.cpp \
    VideoConvertion/ConvertionMP4.cpp \
    src/publicsongdialog.cpp \
    src/yunclient.cpp \
    src/yundm.cpp \
    src/yunthread.cpp
    

HEADERS  += \
    include/enuminfo.h \
    include/mysqlquery.h \
    include/yqcdelegate.h \
    include/defistruct.h \
    include/dropshadowwidget.h \
    include/pushbutton.h \
    include/liblistview.h \
    include/mainwidget.h \
    include/titlewidget.h \
    include/util.h \
    include/libinfowidget.h \
    include/songsquerywideget.h \
    include/singerquerywidget.h \
    include/songsonlinewidget.h \
    include/singeronlinewidget.h \
    include/songinfowidget.h \
    include/singerinfowdiget.h \
    include/songslistwidget.h \
    include/fmwidget.h \
    include/fminfowidget.h \
    include/adwidget.h \
    include/blackwidget.h \
    include/listinfowidget.h \
    include/tablemodel.h \
    include/pagingtableview.h \
    include/libinfodialog.h \
    include/typeindexvalue.h \
    include/songinfodialog.h \
    include/singerinfodialog.h \
    include/fmaddmodifydialog.h \
    include/singeraddmodifydialog.h \
    include/releasedialog.h \
    include/songslistaddmodifydialog.h \
    include/MD5.h \
    include/stafflogin.h\
    curl/curl.h \
    curl/curlbuild.h \
    curl/curlrules.h \
    curl/curlver.h \
    curl/easy.h \
    curl/mprintf.h \
    curl/multi.h \
    curl/stdcheaders.h \
    curl/typecheck-gcc.h \       
    zlib128-dll/minizip/crypt.h \
    zlib128-dll/minizip/ioapi.h \
    zlib128-dll/minizip/iowin32.h \
    zlib128-dll/minizip/mztools.h \
    zlib128-dll/minizip/unzip.h \
    zlib128-dll/minizip/zip.h \ 
    include/pinyinall.h \
    include/thread.h \
    include/curlupload.h \
    player/include/Audio.h \
    player/include/Common.h \
    player/include/Config.h \
    player/include/ControlAudio.h \
    player/include/ControlVideo.h \
    player/include/Enums.h \
    player/include/Error.h \
    player/include/Instance.h \
    player/include/Media.h \
    player/include/MediaList.h \
    player/include/MediaListPlayer.h \
    player/include/MediaPlayer.h \
    player/include/MetaManager.h \
    player/include/SharedExportCore.h \
    player/include/SharedExportWidgets.h \
    player/include/Video.h \
    player/include/VideoDelegate.h \
    player/include/WidgetSeek.h \
    player/include/WidgetVideo.h \
    player/include/WidgetVolumeSlider.h \
    player/MP4Player.h \
    VideoConvertion/ConvertionMP4.h \
    include/releasetech.h \
    include/publicsongdialog.h \
    include/yunclient.h \
    include/yundm.h \
    include/yunthread.h

FORMS    += \
    ui/addfmmedialist.ui \
    ui/stafflogin.ui \
    player/MP4Player.ui

RESOURCES += \    
    Resources\YQCML.qrc

win64:RC_FILE += YQCSMico.rc
win64:OTHER_FILES += YQCSMico.rc

DISTFILES += \
    search.qss \
    zlib128-dll/include/zlib.def \
    zlib128-dll/minizip/make_vms.com \
    zlib128-dll/minizip/miniunzip.1 \
    zlib128-dll/minizip/minizip.1 \
    zlib128-dll/minizip/minizip.pc.in \
    zlib128-dll/minizip/MiniZip64_Changes.txt \
    zlib128-dll/minizip/MiniZip64_info.txt \
    zlib128-dll/minizip/configure.ac \
    zlib128-dll/minizip/Makefile \
    zlib128-dll/minizip/Makefile.am


#win32: LIBS += -L$$PWD/quazip-0.7.1/lib/ -lquazip

#INCLUDEPATH += $$PWD/quazip-0.7.1/include
#DEPENDPATH += $$PWD/quazip-0.7.1/include







