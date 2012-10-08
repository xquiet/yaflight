#ifndef APT_DAT_H
#define APT_DAT_H

#include <QFile>
#include <QTextStream>
#include <QHash>
#include <QStringList>

#include <zlib.h>

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
    APT_dat(QString zpath, QString dpath);
    void parse_apt_dat();
private:
    QString aptdatFilePath;
    QString decompressedFilePath;
    QHash<QString, Runway *> runwayList;
    QHash<QString, QString> airportNameList;
    void read();
    QString parseAirportLine(QStringList items);
    void parseRunwayLine(QString icao, QStringList items);
    QByteArray gUncompress(const QByteArray &data);

    bool isDecompressed;
};

#endif // APT_DAT_H