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
