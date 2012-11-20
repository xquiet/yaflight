#include "lonlat.h"

LonLat::LonLat()
{
}

QString LonLat::getLonLat()
{
    return QString(lon + "," + lat);
}

QString LonLat::getLatLon()
{
    return QString(lat + "," + lon);
}

void LonLat::set(QString l, QString la)
{
    lon = l.trimmed();
    lat = la.trimmed();
}
