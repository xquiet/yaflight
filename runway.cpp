#include "runway.h"

Runway::Runway(QString number, QString lat, QString lon, QString heading, QString surface, QString shoulder)
{
    rwNumber = number;
    rwLatitude = lat;
    rwLongitude = lon;
    rwHeading = heading;
    rwSurfaceCode = surface;
    rwShoulderCode = shoulder;
}

QString Runway::toString()
{
    return QString(rwNumber + "|" +
                   rwLatitude + "|" +
                   rwLongitude + "|" +
                   rwHeading + "|" +
                   rwSurfaceCode + "|" +
                   rwShoulderCode);
}

QString Runway::getNumber()
{
    return rwNumber;
}

QString Runway::getLatitude()
{
    return rwLatitude;
}

QString Runway::getLongitude()
{
    return rwLongitude;
}

QString Runway::getHeading()
{
    return rwHeading;
}

QString Runway::getSurfaceCode()
{
    return rwSurfaceCode;
}

QString Runway::getShoulderCode()
{
    return rwShoulderCode;
}
