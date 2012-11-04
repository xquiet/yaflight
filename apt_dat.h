/**
    Copyright (C) 2012  Matteo Pasotti <matteo.pasotti@gmail.com>

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

#ifndef APT_DAT_H
#define APT_DAT_H

#include <QFile>
#include <QTextStream>
#include <QHash>
#include <QStringList>
#include <QDir>

#include <zlib.h>

#include "airport.h"
#include "runway.h"

#define APTDAT_AIRPORT          1
#define APTDAT_SEAPLANE_BASE    16
#define APTDAT_HELIPORT         17

#define APTDAT_RUNWAY           10  // v810
#define APTDAT_RUNWAY_850       100 // v850
#define APTDAT_RUNWAY_WATER     101 // water rw v850
#define APTDAT_RUNWAY_HELIPAD   102 // helipad v850


class APT_dat
{
public:
    APT_dat(QString zpath, QString yfhomedir);
    //void parse_apt_data();
    bool create_cache(QHash<QString, QStringList> allAirports, QStringList all_airports_dir);
    QHash<QString, QStringList> getAirports();

    QList<Runway *> getRunwaysByAirport(QString key);

    bool aptcache_exists();
    bool rwscache_exists();

private:
    QString homeDir;
    QString aptdatFilePath;
    QString decompressedFilePath;
    QString aptcache;
    QString rwscache;
    QHash<QString, QStringList> airports;
    Runway *lastRunway;
    QString lastAirportName;
    QString lastAirportDescription;
    QStringList allAirportsDir;

    void read();
    void parseAirportLine(QStringList items);
    void parseRunwayLine(QStringList items);
    QByteArray gUncompress(const QByteArray &data);

    bool addAirport();
    bool addRunway();

    bool store_runways(QString apname, QString data);

    bool isDecompressed;
};

#endif // APT_DAT_H
