/**
    Copyright (C) 2012  Matteo Pasotti <matteo.pasotti@gmail.com>

    This file is part of cyaflight - Cross platform YaFlight

    cyaflight is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    cyaflight is distributed in the hope that it will be useful,
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

class FGEnvironment
{
private:
    QString fgRootPath;
    QString fgVersion;
    QProcess *sysinfo;
    QString operating_system;
    QStringList os_details;

    // windows_exception
    QString _win_program_files;

    QString detectOS();
    QString detectFGVersion();
    QString detectFGBinPath();
    QString detectRootPath();

public:
    FGEnvironment();
    void setRootPath(QString path);
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
    QString getYFHome();
    QString getOS();
    QString getFGVersion();

    // debug
    QString __read_winprogramfiles();



};

#endif // FGENVIRONMENT_H
