#ifndef AIRPORTIDX_H
#define AIRPORTIDX_H

#include <QHash>
#include <QStringList>
#include <QFile>
#include <QTextStream>

//#include "apt_dat.h"

class AirportIdx
{
public:
    AirportIdx(QString cachePath);
    bool exists();
    bool load();
    int count();
    bool create(QHash<QString, QStringList> airportHash,
                QString aptdatgzipped,
                QString decompresseddirpath);
    QHash<QString, QStringList> get();
private:
    QString airportsIndexCachePath;
    QHash<QString, QStringList> airportIdxCache;

    //APT_dat *aptdat;
};

#endif // AIRPORTIDX_H
