/**
    Copyright (C) 2012  Matteo Pasotti <matteo.pasotti@gmail.com>

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

#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QString>

#include "configuration.h"

#define APTVIEW     "APTVIEW"
#define FGFSBIN     "FGFSBIN"
#define FGDATADIR   "FGDATADIR"

class appsettings
{
public:
    appsettings(QString);

    bool isEmpty();
    bool storeData();
    QString getUpdatesHost();
    QString getUpdatesScript();

    // app - GET
    QString getAirportListFiltered();
    QString get_fgfs_bin_path();
    QString getFGDataPath();

    // app - SET
    void setAirportListFiltered(QString val);
    void set_fgfs_bin_path(QString val);
    void setFGDataPath(QString val);

private:
    Configuration *conf;
    bool conf_empty;
    static QString updatesHost;
    static QString updatesScript;
};

#endif // APPSETTINGS_H
