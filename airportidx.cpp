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

#include "airportidx.h"

AirportIdx::AirportIdx(QString cachePath)
{
    airportsIndexCachePath = cachePath.trimmed();
}

bool AirportIdx::create(QHash<QString, QStringList> airportHash,
                        QString aptdatgzipped,
                        QString decompresseddirpath)
{
    if(airportHash.count()<=0)
    {
        qWarning("Empty hash");
        return false;
    }
    QFile cache(airportsIndexCachePath);
    if(cache.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream out(&cache);
        foreach(QString key, airportHash.keys())
        {
            /*aptdat = new APT_dat(aptdatgzipped,decompresseddirpath);
            if(!aptdat->retrieve_ap_details(key))
            {
                cache.close();
                runwayscache.close();
                return false;
            }
            QString airportDescription = aptdat->get_ap_description(key);
            QList<Runway *> runways = aptdat->get_ap_runways(key);*/
            out << key + "|"
                       + airportHash[key][0]
                       + "|"
                       + airportHash[key][1]
                       + "|"
                       + airportHash[key][2]
                       + "|"
                       //+ airportDescription
                << endl;
            /*foreach(Runway *rw, runways)
                outrwcache << key + "|" + rw->toString() << endl;*/
        }
        out.flush();
        cache.close();
    }
    else
    {
        qWarning("Can't open file %s . Error: %s", cache.fileName().toStdString().data(), cache.errorString().toStdString().data());
        return false;
    }
    return true;
}

bool AirportIdx::exists()
{
    QFile cache(airportsIndexCachePath);
    return cache.exists();
}

bool AirportIdx::load()
{
    QFile file(airportsIndexCachePath);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream in(&file);
        while(!in.atEnd())
        {
            QStringList items = in.readLine().split("|");
            airportIdxCache.insert(items[0].trimmed(), QStringList() << items[1].trimmed() << items[2].trimmed() << items[3].trimmed());
        }
    }
    else
    {
        return false;
    }
    return true;
}

int AirportIdx::count()
{
    return airportIdxCache.count();
}

QHash<QString, QStringList> AirportIdx::get()
{
    return airportIdxCache;
}
