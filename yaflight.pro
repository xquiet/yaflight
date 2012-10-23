#-------------------------------------------------
#
# Project created by QtCreator 2012-07-04T16:27:06
#
#-------------------------------------------------

QT       += core gui webkit

DEFINES += VERSION="0.99"
TARGET = yaflight
TEMPLATE = app

isEmpty(DATADIR) {
    DATADIR = /usr/share/yaflight
}

TRANSDIR = $$DATADIR/languages/

DEFINES += DATADIR=\\\"$$DATADIR\\\" TRANSDIR=\\\"$$TRANSDIR\\\"

unix:LIBS += -lz

win32:LIBS += "C:\\Users\\matteo\\workspace\\zlib-1.2.7\\contrib\\vstudio\\vc9\\x86\\ZlibDllReleaseWithoutAsm\\zlibwapi.dll"

win32:INCLUDEPATH += "C:\\Users\\matteo\\workspace\\zlib-1.2.7"

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
    airportidx.cpp \
    apt_dat.cpp \
    surfacecode.cpp \
    shouldercode.cpp

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
    airportidx.h \
    apt_dat.h \
    surfacecode.h \
    shouldercode.h

FORMS    += mainwindow.ui \
    dlgaircraftdetails.ui \
    cuteexpander.ui

RESOURCES += \
    yaflightresources.qrc

OTHER_FILES +=

TRANSLATIONS += languages/it_IT.ts
