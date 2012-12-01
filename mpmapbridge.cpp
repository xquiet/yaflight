#include "mpmapbridge.h"

QString mpmapbridge::baseUrl = "http://mpmap02.flightgear.org/";

mpmapbridge::mpmapbridge()
{
}

QString mpmapbridge::getBaseUrl()
{
    return baseUrl;
}

void mpmapbridge::setPilotToFollow(QString p)
{
    pilot = p.trimmed();
}

void mpmapbridge::setLonLat(QString lon, QString lat)
{
    ll = new LonLat();
    ll->set(lon, lat);
}

QString mpmapbridge::getUrl()
{
    //return getBaseUrl() + "?ll=" + ll->getLatLon() + "&nomenu&nomapcontrol";
    return getBaseUrl() + "?ll=" + ll->getLatLon() + "&nomapcontrol";
}

QString mpmapbridge::followPilot()
{
    return getBaseUrl() + "?nomapcontrol&follow=" + pilot;
}
