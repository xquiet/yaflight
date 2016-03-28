/**
    Copyright (C) 2012-2016  Matteo Pasotti <matteo.pasotti@gmail.com>

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

#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <QStringList>
#include <QFile>
#include <QTextStream>

class Aircraft
{
public:
    Aircraft(QString aircraftName, QString aircraftsDir, QString aircraftDir);
    QStringList getAircraftDetails(QString, QString aircraftsDir, QString aircraftDir);
    // set
    void setAero(QString);
    void setDescription(QString);
    void setAuthor(QString);
    void setStatus(QString);
    void setVersion(QString);
    void setFlightModel(QString);
    void setFuelFraction(QString);
    void setSplashTexture(QString);
    // get
    QString getAero();
    QString getDescription();
    QString getAuthor();
    QString getStatus();
    QString getVersion();
    QString getFlightModel();
    QString getFuelFraction();
    QString getSplashTexture();

private:
    QString aero,
            description,
            author,
            status,
            version,
            flightmodel,
            fuelfraction,
            splashtexture;
};

#endif // AIRCRAFT_H
