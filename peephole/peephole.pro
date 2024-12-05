#-------------------------------------------------
#
# Project created by QtCreator 2021-02-03T19:44:50
#
#-------------------------------------------------

QT       += core gui opengl
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = peephole
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG  += debug_and_release
INCLUDEPATH += $$PWD\3rdlibs\ffmpeg\include
INCLUDEPATH += $$PWD\3rdlibs\opencv\include
INCLUDEPATH += $$PWD\3rdlibs\opencv\include\opencv2
INCLUDEPATH += $$PWD\3rdlibs\opencv\include\opencv2\core
INCLUDEPATH += $$PWD\3rdlibs\opencv\include\opencv2\highgui
INCLUDEPATH += $$PWD\3rdlibs\libLive555\include\BasicUsageEnvironment\
INCLUDEPATH += $$PWD\3rdlibs\libLive555\include\groupsock\
INCLUDEPATH += $$PWD\3rdlibs\libLive555\include\liveMedia\
INCLUDEPATH += $$PWD\3rdlibs\libLive555\include\UsageEnvironment\
INCLUDEPATH += $$PWD\3rdlibs\openssl\include
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    RecFile.cpp \
    ThreadProc.cpp \
    dataproc.cpp \
    GLDisply.cpp \
    DataShow.cpp \
    common.cpp \
    CvDisply.cpp \
    DataBuf.cpp \
    DataBufManager.cpp \
    DataShow.cpp \
    GLDisply.cpp \
    H264Dec.cpp \
    H265Dec.cpp \
    ImageUtils.cpp \
    MediaPly.cpp \
    RtspClient.cpp \
    StreamPly.cpp \
    H265DecHw.cpp \
    SerialProtocol3V.cpp \
    SerialProtocolOrb.cpp \
    SerialProtocolK1S.cpp \
    SerialProtocolOrbFac.cpp \
    Secure.cpp \
    SerialProtocolOrbComm.cpp \
    OrbUtils.cpp

HEADERS += \
        mainwindow.h \
    DataTyp.h \
    FileHelper.h \
    RecFile.h \
    ThreadProc.h \
    dataproc.h \
    GLDisply.h \
    cdatadisply.h \
    DataShow.h \
    cdatadisply.h \
    common.h \
    CvDisply.h \
    DataBuf.h \
    DataBufManager.h \
    DataShow.h \
    H264Dec.h \
    H265Dec.h \
    ImageUtils.h \
    MediaDec.h \
    MediaPly.h \
    RtspClient.h \
    StreamDataTyp.h \
    StreamPly.h \
    clogcat.h \
    H265DecHw.h \
    PeepholeSerialK1SDataTyp.h \
    PeepholeSerial3vDataTyp.h \
    SerialOrbCmdPkt.h \
    OrbDataTyp.h \
    SerialProtocolBase.h \
    SerialProtocolOrb.h \
    SerialProtocolK1S.h \
    SerialProtocol3V.h \
    SerialProtocolOrbFac.h \
    Secure.h \
    OrbProtocolDataTyp.h \
    OrbCmdPktAes.h \
    OrbCmdPktBase.h \
    SerialProtocolOrbComm.h \
    OrbUtils.h \
    OrbCmdPktComm.h \
    FileHelper.h \
    FileHelper.h

FORMS += \
        mainwindow.ui

LIBS+=-lopengl32 -lAdvAPI32 -lUser32 #-lComDlg32 -lKernel32 -lGdi32 -lWinSpool -lshell32
LIBS+=-lOle32 -lOleAut32 -lUuid -lodbc32 -lodbccp32

CONFIG(debug, debug|release) {
LIBS += $$PWD\3rdlibs\opencv\Win32-Debug\opencv_core340d.lib
LIBS += $$PWD\3rdlibs\opencv\Win32-Debug\opencv_highgui340d.lib
LIBS += $$PWD\3rdlibs\opencv\Win32-Debug\opencv_imgcodecs340d.lib
LIBS += $$PWD\3rdlibs\opencv\Win32-Debug\opencv_imgproc340d.lib
LIBS += $$PWD\3rdlibs\opencv\Win32-Debug\opencv_videoio340d.lib
LIBS += $$PWD\3rdlibs\opencv\Win32-Debug\opencv_video340d.lib
LIBS += $$PWD\3rdlibs\opencv\Win32-Debug\opencv_videostab340d.lib
}else {
#LIBS += $$PWD\3rdlibs\sys\x86\AdvAPI32.Lib
LIBS += $$PWD\3rdlibs\ffmpeg\lib\x86\Release\libavcodec.lib
LIBS += $$PWD\3rdlibs\ffmpeg\lib\x86\Release\libavdevice.lib
LIBS += $$PWD\3rdlibs\ffmpeg\lib\x86\Release\libavfilter.lib
LIBS += $$PWD\3rdlibs\ffmpeg\lib\x86\Release\libavformat.lib
LIBS += $$PWD\3rdlibs\ffmpeg\lib\x86\Release\libavutil.lib
LIBS += $$PWD\3rdlibs\ffmpeg\lib\x86\Release\libpostproc.lib
LIBS += $$PWD\3rdlibs\ffmpeg\lib\x86\Release\libswresample.lib
LIBS += $$PWD\3rdlibs\ffmpeg\lib\x86\Release\libswscale.lib
LIBS += $$PWD\3rdlibs\opencv\Win32-Release\opencv_core340.lib
LIBS += $$PWD\3rdlibs\opencv\Win32-Release\opencv_highgui340.lib
LIBS += $$PWD\3rdlibs\opencv\Win32-Release\opencv_imgcodecs340.lib
LIBS += $$PWD\3rdlibs\opencv\Win32-Release\opencv_imgproc340.lib
LIBS += $$PWD\3rdlibs\opencv\Win32-Release\opencv_videoio340.lib
LIBS += $$PWD\3rdlibs\opencv\Win32-Release\opencv_video340.lib
LIBS += $$PWD\3rdlibs\opencv\Win32-Release\opencv_videostab340.lib
LIBS += $$PWD\3rdlibs\libLive555\lib\libLive555.lib
LIBS += $$PWD\3rdlibs\openssl\lib\libeay32.lib
LIBS += $$PWD\3rdlibs\openssl\lib\ssleay32.lib
}
