#ifndef AIRPORTIDX_H
#define AIRPORTIDX_H

#include <QHash>
#include <QStringList>
#include <QFile>
#include <QTextStream>

class AirportIdx
{
public:
    AirportIdx(QString cachePath);
    bool exists();
    bool load();
    int count();
    bool create(QHash<QString, QStringList> airportHash);
    QHash<QString, QStringList> get();
private:
    QString indexCachePath;
    QHash<QString, QStringList> airportIdxCache;
};

#endif // AIRPORTIDX_H
