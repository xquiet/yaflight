#include "airportidx.h"

AirportIdx::AirportIdx(QString cachePath)
{
    indexCachePath = cachePath.trimmed();
}

bool AirportIdx::create(QHash<QString, QStringList> airportHash)
{
    if(airportHash.count()<=0)
    {
        qWarning("Empty hash");
        return false;
    }
    QFile cache(indexCachePath);
    if(cache.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream out(&cache);
        foreach(QString key, airportHash.keys())
        {
            out << key + "|" + airportHash[key][0] + "|" + airportHash[key][1] + "|" + airportHash[key][2] << endl;
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
    QFile cache(indexCachePath);
    return cache.exists();
}

bool AirportIdx::load()
{
    QFile file(indexCachePath);
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
