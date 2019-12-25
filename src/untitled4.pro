#-------------------------------------------------
#
# Project created by QtCreator 2019-11-22T02:45:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled4
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS


# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

INCLUDEPATH += -L/usr/local/include/opencv -I/usr/local/include

LIBS += -L/usr/local/lib \
        -lopencv_ml \
        -lopencv_dnn \
        -lopencv_videostab \
        -lopencv_photo \
        -lopencv_stitching \
        -lopencv_calib3d \
        -lopencv_features2d \
        -lopencv_highgui \
        -lopencv_shape \
        -lopencv_flann \
        -lopencv_objdetect \
        -lopencv_superres \
        -lopencv_videoio \
        -lopencv_imgcodecs \
        -lopencv_video \
        -lopencv_imgproc \
        -lopencv_core



SOURCES += \
        main.cpp \
        mainwindow.cpp \
    videothread.cpp \
    clickedlabel.cpp

HEADERS += \
        mainwindow.h \
    videothread.h \
    clickedlabel.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    video/1.avi
