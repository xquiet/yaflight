#ifndef AIRPORT_H
#define AIRPORT_H

#include <QStringList>

#include "apt_dat.h"

class Airport
{
public:
    Airport(QString airportsDir, QString icao, QString aptdatgzipped, QString destpath); // icao from Airports/index.txt
    // get
    QString getICAO();
    QString getLongitude();
    QString getLatitude();
    QStringList getRunwayList();
    QString getAirportDirPath();
private:
    QString airportPath;

    APT_dat *aptdat;
};

#endif // AIRPORT_H
