#-------------------------------------------------
#
# Project created by QtCreator 2012-07-04T16:27:06
#
#-------------------------------------------------

QT       += core gui webkit network

DEFINES += MAX_VERSION="0.99"
DEFINES += MIN_VERSION="22"
TARGET = yaflight
TEMPLATE = app

mac: ICON = icons/yaflight.icns

win32: RC_FILE = yaflight.rc

isEmpty(DATADIR) {
    DATADIR = /usr/share/yaflight
}

TRANSDIR = $$DATADIR/languages/

DEFINES += DATADIR=\\\"$$DATADIR\\\" TRANSDIR=\\\"$$TRANSDIR\\\"

unix:LIBS += -lz

win32:LIBS += "C:\\librerie\\zlib-1.2.7\\contrib\\vstudio\\vc9\\x86\\ZlibDllReleaseWithoutAsm\\zlibwapi.dll"

win32:INCLUDEPATH += "C:\\librerie\\zlib-1.2.7"

SOURCES += main.cpp\
        mainwindow.cpp \
    fgenvironment.cpp \
    imagepreview.cpp \
    cuteexpander.cpp \
    aircraft.cpp \
    configuration.cpp \
    settings.cpp \
    airport.cpp \
    runway.cpp \
    apt_dat.cpp \
    surfacecode.cpp \
    shouldercode.cpp \
    runwaydetailsdialog.cpp \
    appsettings.cpp \
    dialogappsettings.cpp \
    logger.cpp \
    aircraftdetailsdialog.cpp \
    mpmapbridge.cpp \
    dialogmpdetails.cpp \
    lonlat.cpp \
    mpmdialog.cpp \
    helpdialog.cpp

HEADERS  += mainwindow.h \
    fgenvironment.h \
    imagepreview.h \
    cuteexpander.h \
    aircraft.h \
    configuration.h \
    settings.h \
    airport.h \
    runway.h \
    apt_dat.h \
    surfacecode.h \
    shouldercode.h \
    runwaydetailsdialog.h \
    appsettings.h \
    dialogappsettings.h \
    logger.h \
    aircraftdetailsdialog.h \
    mpmapbridge.h \
    dialogmpdetails.h \
    lonlat.h \
    mpmdialog.h \
    helpdialog.h

FORMS    += mainwindow.ui \
    cuteexpander.ui \
    runwaydetailsdialog.ui \
    dialogappsettings.ui \
    aircraftdetailsdialog.ui \
    dialogmpdetails.ui \
    mpmdialog.ui \
    helpdialog.ui

RESOURCES += \
    yaflightresources.qrc

OTHER_FILES +=

TRANSLATIONS += languages/cs_CZ.ts \
    languages/it_IT.ts \
    languages/fr_FR.ts \
    languages/de_DE.ts \
    languages/es_ES.ts \
    languages/ru_RU.ts \
    languages/zh_CN.ts \
    languages/zh_TW.ts


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../yalib/release/ -lyalib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../yalib/debug/ -lyalib
else:mac: LIBS += -F$$PWD/../yalib/ -framework yalib
else:unix: LIBS += -L$$PWD/../yalib/ -lyalib

INCLUDEPATH += $$PWD/../yalib
DEPENDPATH += $$PWD/../yalib
