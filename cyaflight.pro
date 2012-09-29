#-------------------------------------------------
#
# Project created by QtCreator 2012-07-04T16:27:06
#
#-------------------------------------------------

QT       += core gui webkit

DEFINES += VERSION="1.1"
TARGET = cyaflight
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    fgenvironment.cpp \
    dlgaircraftdetails.cpp \
    imagepreview.cpp \
    cuteexpander.cpp \
    aircraft.cpp \
    configuration.cpp \
    settings.cpp \
    airport.cpp \
    runway.cpp \
    airportidx.cpp

HEADERS  += mainwindow.h \
    fgenvironment.h \
    dlgaircraftdetails.h \
    imagepreview.h \
    cuteexpander.h \
    aircraft.h \
    configuration.h \
    settings.h \
    airport.h \
    runway.h \
    airportidx.h

FORMS    += mainwindow.ui \
    dlgaircraftdetails.ui \
    cuteexpander.ui

RESOURCES += \
    yaflightresources.qrc

OTHER_FILES +=
