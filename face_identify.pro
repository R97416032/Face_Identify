#-------------------------------------------------
#
# Project created by QtCreator 2021-09-09T16:50:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = face_identify
TEMPLATE = app
INCLUDEPATH += /usr/local/include/ \
            /usr/local/include/opencv/ \
            /usr/local/include/opencv2/ \
            /usr/local/include/opencv2/core
LIBS += /usr/local/lib/libopencv_*

INCLUDEPATH += /usr/local/include \              #（这部分可能路径不太一样，可以打开文档查找一下）
/usr/local/include/opencv2

LIBS += /usr/local/lib/libopencv_core.so.3.4
LIBS += /usr/local/lib/libopencv_highgui.so.3.4
LIBS += /usr/local/lib/libopencv_core.so.3.4
LIBS += /usr/local/lib/libopencv_highgui.so.3.4
LIBS += /usr/local/lib/libopencv_calib3d.so.3.4
LIBS += /usr/local/lib/libopencv_features2d.so.3.4
LIBS += /usr/local/lib/libopencv_flann.so.3.4
LIBS += /usr/local/lib/libopencv_imgcodecs.so.3.4
LIBS += /usr/local/lib/libopencv_imgcodecs.so
LIBS += /usr/local/lib/libopencv_imgproc.so.3.4
LIBS += /usr/local/lib/libopencv_ml.so.3.4
LIBS += /usr/local/lib/libopencv_objdetect.so.3.4
LIBS += /usr/local/lib/libopencv_video.so.3.4
LIBS += /usr/local/lib/libopencv_videoio.so.3.4
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    info.cpp

HEADERS += \
        mainwindow.h \
    info.h

FORMS += \
        mainwindow.ui \
    info.ui
