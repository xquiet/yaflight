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

QString mpmapbridge::getLLUrlWithoutMenu()
{
    //return getBaseUrl() + "?ll=" + ll->getLatLon() + "&nomenu&nomapcontrol";
    return getBaseUrl() + "?ll=" + ll->getLatLon() + "&nomapcontrol&nomenu";
}

QString mpmapbridge::getLLUrl()
{
    //return getBaseUrl() + "?ll=" + ll->getLatLon() + "&nomenu&nomapcontrol";
    return getBaseUrl() + "?ll=" + ll->getLatLon() + "&nomapcontrol";
}

QString mpmapbridge::getFollowPilotUrl()
{
    return getBaseUrl() + "?nomapcontrol&follow=" + pilot;
}

QString mpmapbridge::getFollowPilotUrlWithoutMenu()
{
    return getBaseUrl() + "?nomapcontrol&nomenu&follow=" + pilot;
}

QString mpmapbridge::getUrlWithoutMenu()
{
    return getBaseUrl() + "?nomapcontrol&nomenu";
}

QString mpmapbridge::getUrl()
{
    return getBaseUrl() + "?nomapcontrol";
}
