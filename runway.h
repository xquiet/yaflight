#ifndef RUNWAY_H
#define RUNWAY_H

#include <QString>

class Runway
{
public:
    Runway(QString number, QString lat, QString lon, QString heading, QString surface, QString shoulder);
    QString toString();

private:
    QString rwNumber;
    QString rwLatitude;
    QString rwLongitude;
    QString rwHeading;
    QString rwSurfaceCode;
    QString rwShoulderCode;
};

#endif // RUNWAY_H
