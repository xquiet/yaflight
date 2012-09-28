#ifndef AIRPORT_H
#define AIRPORT_H

#include <QStringList>

#include "runway.h"

class Airport
{
public:
    Airport(QString airportDir, QString icao); // icao from Airports/index.txt

    // get
    QString getICAO();
    QString getLongitude();
    QString getLatitude();
    QStringList getRunwayList();
    Runway getRunway();
};

#endif // AIRPORT_H
