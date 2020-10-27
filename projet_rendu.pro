#-------------------------------------------------
#
# Project created by QtCreator 2019-01-12T12:27:11
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = projet_rendu
TEMPLATE = app


SOURCES += main.cpp\
        demicylindre.cpp \
        dialog.cpp \
        princ.cpp \
        glarea.cpp \
        cylindre.cpp \
        sens_rotation.cpp \
        type_cylindre.cpp \
        util.cpp

HEADERS  += princ.h \
        demicylindre.h \
        dialog.h \
        glarea.h \
        cylindre.h \
        util.h

FORMS    += princ.ui \
    dialog.ui

RESOURCES += \
    projet_rendu.qrc

DISTFILES += \
    ../cours 5/PROGS/Qt/pro09-light/fragment.glsl \
    ../cours 5/PROGS/Qt/pro09-light/vertex.glsl \
    texture-metal.png
