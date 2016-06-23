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
    C_radar_data.cpp \
    c_arpa_data.cpp \
    onexitdialog.cpp \
    qcustombutton.cpp \
    qcustomframe.cpp \
    qcustomcombobox.cpp \
    qcustomgroupbox.cpp \
    qcustomtabwidget.cpp \
    Utility.cpp \
    Tracker.cpp \
    ctarget.cpp
HEADERS  += mainwindow.h \
    vnmap.h \
    Config.h \
    gpsdialog.h \
    C_radar_data.h \
    c_arpa_data.h \
    onexitdialog.h \
    qcustombutton.h \
    qcustomframe.h \
    qcustomcombobox.h \
    qcustomgroupbox.h \
    qcustomtabwidget.h \
    Utility.h \
    Tracker.h \
    ctarget.h

FORMS    += mainwindow.ui \
    gpsdialog.ui \
    onexitdialog.ui
#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/shapelib/ -lshapelib
#else:unix: LIBS += -L$$PWD\shapelib\ -lshapelib
#INCLUDEPATH += $$PWD/shapelib
#DEPENDPATH += $$PWD/shapelib
#INCLUDEPATH += $$PWD/../WpdPack/Include
#DEPENDPATH += $$PWD/../WpdPack/Include
RESOURCES += \
    icons.qrc
win32:LIBS += -L$$PWD/../armadilloWin32/lib_winx86/ -lblas_win32_MT
win32:LIBS += -L$$PWD/../armadilloWin32/lib_winx86/ -llapack_win32_MT
unix: LIBS += -larmadillo
#win32:LIBS += -L$$PWD/../WpdPack/Lib/ -lPacket
#win32:LIBS += -L$$PWD/../WpdPack/Lib/ -lwpcap
INCLUDEPATH += $$PWD/../opencv/build/include
win32:LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_core2412d
win32:LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_highgui2412d
win32:LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_imgproc2412d
win32:LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_calib3d2412d
win32:LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_contrib2412d
win32:LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_core2412d
win32:LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_features2d2412d
win32:LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_flann2412d
win32:LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_gpu2412d
win32:LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_legacy2412d
win32:LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_ml2412d
win32:LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_nonfree2412d
win32:LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_objdetect2412d
win32:LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_ocl2412d
win32:LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_photo2412d
win32:LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_stitching2412d
win32:LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_superres2412d
win32:LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_video2412d
win32:LIBS += -L$$PWD/../opencv/build/x86/vc11/lib/ -lopencv_videostab2412d

#LIBS +=
#win32:CONFIG(release, debug|release):INCLUDEPATH += $$PWD/armadillo/lib_winx86
#win32:CONFIG(release, debug|release):DEPENDPATH += $$PWD/armadillo/lib_winx86

unix:QMAKE_CXXFLAGS += -O2
unix:QMAKE_CXXFLAGS += -l -DARMA_DONT_USE_WRAPPER -lblas -llapack
DISTFILES += \
    appIcon.rc \
    notes.txt
win32:RC_FILE += appIcon.rc

