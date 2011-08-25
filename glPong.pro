#-------------------------------------------------
#
# Project created by QtCreator 2011-07-16T09:42:32
#
#-------------------------------------------------

QT       += core gui opengl phonon network

TARGET = glPong
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    glcorewidget.cpp \
    paddle.cpp \
    pongBall.cpp \
    network.cpp

HEADERS  += widget.h \
    glcorewidget.h \
    paddle.h \
    pongBall.h \
    network.h

FORMS    += widget.ui

RESOURCES += \
    glPongSounds.qrc
