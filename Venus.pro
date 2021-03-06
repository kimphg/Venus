#-------------------------------------------------
#
# Project created by QtCreator 2015-06-20T15:53:11
# Author Phung Kim Phuong pawnc7@gmail.com
#-------------------------------------------------
#Features:
#Control and process data from HR2D radar
#view ARPA data
#-------------------------------------------------
# Version 1.1
#-------------------------------------------------
#file type defined as .r2d, radar data starts from 22nd byte of each frame
#run for both 1024 and 2048 data type
#radar resolution can be changed by the user
@CONFIG += debug_and_release@
QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = Venus_1
TEMPLATE = app
SOURCES += main.cpp\
    mainwindow.cpp \
    vnmap.cpp \
    Config.cpp \
    gpsdialog.cpp \
    onexitdialog.cpp \
    qcustombutton.cpp \
    qcustomframe.cpp \
    qcustomcombobox.cpp \
    qcustomgroupbox.cpp \
    qcustomtabwidget.cpp \
    Utility.cpp \
    Tracker.cpp \
    ctarget.cpp \
    CDefine.cpp \
    CLocal.cpp \
    CPoints.cpp \
    CTrack.cpp
HEADERS  += mainwindow.h \
    vnmap.h \
    Config.h \
    gpsdialog.h \
    onexitdialog.h \
    qcustombutton.h \
    qcustomframe.h \
    qcustomcombobox.h \
    qcustomgroupbox.h \
    qcustomtabwidget.h \
    Utility.h \
    Tracker.h \
    ctarget.h \
    CDefine.h \
    CLocal.h \
    CPoints.h \
    CTrack.h

FORMS    += mainwindow.ui \
    gpsdialog.ui \
    onexitdialog.ui
#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/shapelib/ -lshapelib
#else:unix: LIBS += -L$$PWD\shapelib\ -lshapelib
#INCLUDEPATH += $$PWD/shapelib
#DEPENDPATH += $$PWD/shapelib
INCLUDEPATH += $$PWD/../WpdPack/Include
DEPENDPATH += $$PWD/../WpdPack/Include
RESOURCES += \
    icons.qrc
#win32:LIBS += -L$$PWD/../armadilloWin32/lib_winx86/ -lblas_win32_MT
#win32:LIBS += -L$$PWD/../armadilloWin32/lib_winx86/ -llapack_win32_MT
unix: LIBS += -larmadillo
win32:LIBS += -L$$PWD/../WpdPack/Lib/ -lPacket
win32:LIBS += -L$$PWD/../WpdPack/Lib/ -lwpcap
INCLUDEPATH += $$PWD/../opencv/build/include
#debug:
win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_core2411d
win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_highgui2411d
win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_imgproc2411d
win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_calib3d2411d
win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_contrib2411d
win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_core2411d
win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_features2d2411d
win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_flann2411d
win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_gpu2411d
win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_legacy2411d
win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_ml2411d
win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_nonfree2411d
win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_objdetect2411d
win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_ocl2411d
win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_photo2411d
win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_stitching2411d
win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_superres2411d
win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_video2411d
win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_videostab2411d
#release:
win32:CONFIG(release, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_core2411
win32:CONFIG(release, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_highgui2411
win32:CONFIG(release, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_imgproc2411
win32:CONFIG(release, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_calib3d2411
win32:CONFIG(release, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_contrib2411
win32:CONFIG(release, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_core2411
win32:CONFIG(release, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_features2d2411
win32:CONFIG(release, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_flann2411
win32:CONFIG(release, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_gpu2411
win32:CONFIG(release, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_legacy2411
win32:CONFIG(release, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_ml2411
win32:CONFIG(release, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_nonfree2411
win32:CONFIG(release, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_objdetect2411
win32:CONFIG(release, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_ocl2411
win32:CONFIG(release, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_photo2411
win32:CONFIG(release, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_stitching2411
win32:CONFIG(release, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_superres2411
win32:CONFIG(release, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_video2411
win32:CONFIG(release, debug|release):LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_videostab2411
#LIBS +=
#win32:CONFIG(release, debug|release):INCLUDEPATH += $$PWD/armadillo/lib_winx86
#win32:CONFIG(release, debug|release):DEPENDPATH += $$PWD/armadillo/lib_winx86

#unix:QMAKE_CXXFLAGS += -O2
#unix:QMAKE_CXXFLAGS += -l -DARMA_DONT_USE_WRAPPER -lblas -llapack
DISTFILES += \
    appIcon.rc \
    notes.txt
win32:RC_FILE += appIcon.rc

