#ifndef AIRPORT_H
#define AIRPORT_H

#include <QStringList>
//#include <QDir>

#include "runway.h"

class Airport
{
public:
    Airport(QString airportsDir, QString icao); // icao from Airports/index.txt
    // get
    QString getICAO();
    QString getLongitude();
    QString getLatitude();
    QStringList getRunwayList();
    Runway getRunway();
    QString getAirportDirPath();
private:
    QString airportPath;
};

#endif // AIRPORT_H
