/**
    Copyright (C) 2012-2013  Matteo Pasotti <matteo.pasotti@gmail.com>

    This file is part of yaflight - Cross platform YaFlight

    yaflight is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    yaflight is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#ifndef MPMAPBRIDGE_H
#define MPMAPBRIDGE_H

#include <QString>

#include "lonlat.h"

class mpmapbridge
{
public:
    mpmapbridge();
    static QString getBaseUrl();
    QString getLLUrl();
    QString getLLUrlWithoutMenu();
    QString getFollowPilotUrl();
    QString getUrlWithoutMenu();
    QString getUrl();
    QString getFollowPilotUrlWithoutMenu();
    void setLonLat(QString lon, QString lat);
    void setPilotToFollow(QString p);

private:
    static QString baseUrl;
    LonLat *ll;
    QString pilot;
};

#endif // MPMAPBRIDGE_H
