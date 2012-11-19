#include "mmapbridge.h"

QString mmapbridge::baseUrl = "http://mpmap02.flightgear.org/";

mmapbridge::mmapbridge()
{
}

QString mmapbridge::getBaseUrl()
{
    return baseUrl;
}
