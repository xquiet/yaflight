#-------------------------------------------------
#
# Project created by QtCreator 2012-07-04T16:27:06
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -fPIC -std=gnu++0x

QT       += core gui network 
lessThan(QT_MAJOR_VERSION, 5): QT += webkit 
greaterThan(QT_MAJOR_VERSION, 4): QT += webkitwidgets

MAX_VERSION = 0
MIN_VERSION = 99
PATCH_VERSION = 30

DEFINES += MAX_VERSION=$$MAX_VERSION
DEFINES += MIN_VERSION=$$MIN_VERSION
DEFINES += PATCH_VERSION=$$PATCH_VERSION
DEFINES += STRVERSION=\\\"$${MAX_VERSION}.$${MIN_VERSION}.$${PATCH_VERSION}\\\"

TARGET = yaflight
TEMPLATE = app

VERSION = $${MAX_VERSION}.$${MIN_VERSION}.$${PATCH_VERSION}

mac: ICON = icons/yaflight.icns

win32: RC_FILE = yaflight.rc

unix: isEmpty($$DATADIR) {
    DATADIR = /usr/share/yaflight
}

TRANSDIR = $$DATADIR/languages/

DEFINES += DATADIR=\\\"$$DATADIR\\\" TRANSDIR=\\\"$$TRANSDIR\\\"

unix:LIBS += -lz

# for development environment
win32:LIBS += "$$PWD/../zlib/lib/zlib.lib" -lz


win32:INCLUDEPATH += "$$PWD/../zlib/include/"

else:mac: LIBS += -F$$PWD/../yalib/ -framework yalib
else:unix: LIBS += -L$$PWD/../yalib/ -lyalib

unix:!macx {
    target.path = /usr/bin
}

# for the binary to link
INCLUDEPATH += $$PWD/../yalib-build/src
DEPENDPATH += $$PWD/../yalib-build/src

# for the header file
INCLUDEPATH += $$PWD/../yalib/src
DEPENDPATH += $$PWD/../yalib/src

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

INSTALLS += target

# libyalib for both release and debug as I'm building libyalib as RelWithDebInfo
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../yalib-build/src/ -llibyalib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../yalib-build/src/ -llibyalib
else:unix: LIBS += -L$$PWD/../yalib-build/src/ -llibyalib

