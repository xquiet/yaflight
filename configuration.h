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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QStringList>
#include <QSettings>
#include <QHash>
#include <QFile>
#include <QDir>
#include <QTextStream>


#define MAINSETTINGS "MAINSETTINGS"
#define ADVSETTINGS "ADVSETTINGS"
#define APPSETTINGS "APPSETTINGS"
#define SCENERIES "SCENERIES"

class Configuration
{
public:
    Configuration(QString conf);

    QStringList get();
    QHash<QString, QString> get(QString key);
    QString get(QString key, QString param);
    void set(QStringList);
    void set(QString key, QString param, QString value);

    QHash<QString, QHash<QString, QString> > confSettings;

    bool create(QString path);
    bool exists();
    void parseFile();
    bool store();
private:
    QHash<QString, QString> parseSection(QString section);
    QString confFilePath;
};

#endif // CONFIGURATION_H
