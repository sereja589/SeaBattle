#-------------------------------------------------
#
# Project created by QtCreator 2016-05-03T18:22:48
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SeaBattle
TEMPLATE = app


SOURCES += main.cpp\
        game.cpp \
    menu.cpp \
    mygraphicsview.cpp \
    mygraphicsscene.cpp \
    getip.cpp \
    cell.cpp \
    field.cpp \
    ship.cpp

HEADERS  += game.h \
    menu.h \
    mygraphicsview.h \
    mygraphicsscene.h \
    getip.h \
    cell.h \
    field.h \
    ship.h
