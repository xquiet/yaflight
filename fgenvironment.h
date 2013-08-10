/**
    Copyright (C) 2012-2013  Matteo Pasotti <matteo.pasotti@gmail.com>

    This file is part of yaflight - Cross platform YaFlight

    yaflight is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    yaflight is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#ifndef FGENVIRONMENT_H
#define FGENVIRONMENT_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QtGlobal>
#include <QProcess>
#include <QEvent>
#include <QDir>
#ifdef Q_OS_MACX
#include "yalib.h"
#elif defined Q_OS_WIN
#include "yalib.h"
#else
#include "yalib/yalib.h"
#endif

class FGEnvironment
{
private:
    Yalib *ya;

    // windows_exception
    QString _win_program_files;

public:
    FGEnvironment();
    bool start(bool autoDetect = true);
    void setRootPath(QString path);
    void setFgFsBinary(QString path);
    QString getRootPath();
    QString getDefaultScenery();
    QString getAircraftsDir();
    QString getDefaultAirportsDir();
    QString getAirportsCacheFilePath();
    QString getRunwaysCacheFilePath();
    QString getAPTSource();
    QHash<QString, QStringList> parseAirportsIndex(QString path);
    QHash<QString, QStringList> getDefaultAirportList();
    QString getFgfsBinPath();
    QString getTerraSyncBinPath();
    QString getTerraSyncPidFilePath();
    QString getYFHome();
    QString getYFScenery();
    QString getOS();
    QString getFGVersion();
    QString getYaInstallBinary();
    QStringList getScenarios();

    // debug
    QString __read_winprogramfiles();

};

#endif // FGENVIRONMENT_H

