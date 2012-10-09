#include "airportidx.h"

AirportIdx::AirportIdx(QString cachePath, QString rwCachePath)
{
    airportsIndexCachePath = cachePath.trimmed();
    runwaysIndexCachePath = rwCachePath.trimmed();
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
        QFile runwayscache(runwaysIndexCachePath);
        if(runwayscache.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            QTextStream out(&cache);
            QTextStream outrwcache(&runwayscache);
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
            outrwcache.flush();
            cache.close();
            runwayscache.close();
        }
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
