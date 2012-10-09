#ifndef AIRPORT_H
#define AIRPORT_H

#include <QStringList>

class Airport
{
public:
    Airport(QString airportsDir, QString icao); // icao from Airports/index.txt
    // get
    QString getICAO();
    QString getLongitude();
    QString getLatitude();
    QStringList getRunwayList();
    QString getAirportDirPath();
private:
    QString airportPath;

};

#endif // AIRPORT_H
