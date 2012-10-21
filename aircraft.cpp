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

#include "aircraft.h"

Aircraft::Aircraft(QString aircraftName, QString aircraftsDir, QString aircraftDir)
{
    foreach(QString item, getAircraftDetails(aircraftName, aircraftsDir, aircraftDir))
    {
        QStringList couple = item.split(":");
        if(couple.value(0).trimmed().compare("splash-texture")==0)
        {
            setSplashTexture(couple.value(1).trimmed());
        }
        else if(couple.value(0).trimmed().compare("aero")==0)
        {
            setAero(couple.value(1).trimmed());
        }
        else if(couple.value(0).trimmed().compare("description")==0)
        {
            setDescription(couple.value(1).trimmed());
        }
        else if(couple.value(0).trimmed().compare("author")==0)
        {
            setAuthor(couple.value(1).trimmed());
        }
        else if(couple.value(0).trimmed().compare("status")==0)
        {
            setStatus(couple.value(1).trimmed());
        }
        else if(couple.value(0).trimmed().compare("aircraft-version")==0)
        {
            setVersion(couple.value(1).trimmed());
        }
        else if(couple.value(0).trimmed().compare("flight-model")==0)
        {
            setFlightModel(couple.value(1).trimmed());
        }
        else if(couple.value(0).trimmed().compare("fuel-fraction")==0)
        {
            setFuelFraction(couple.value(1).trimmed());
        }
    }
}

QStringList Aircraft::getAircraftDetails(QString aircraft, QString aircraftsDir, QString aircraftDir)
{
    // retrieve data from xml files

    /**
      File header example
        <description>Boeing 777-200ER</description>
        <author>Justin Smithies, Syd Adams, Thorsten Brehm</author>
        <status>Development</status>
        <aircraft-version>20111215</aircraft-version>
        <flight-model>yasim</flight-model>
        <aero>777-200ER</aero>
        <fuel-fraction>0.7</fuel-fraction>
        <startup>
            <splash-texture>Aircraft/777-200/splash.png</splash-texture>
        </startup>
      */

    QFile aircraftFile (aircraftsDir+"/"+aircraftDir+"/"+aircraft+"-set.xml");
    //qDebug("%s",(aircraftsDir+"/"+aircraftDir+"/"+aircraft+"-set.xml").toStdString().data());
    if (!aircraftFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return QStringList()<<"";

    QTextStream in(&aircraftFile);
    QString line;
    QStringList result;
    while(!in.atEnd()) // remember: cannot use atEnd on /proc/*
    {
        line = in.readLine();
        if(line.trimmed().contains("<description>"))
        {
            result << "description:" + line.replace("<description>","").replace("</description>","").trimmed();
        }
        else if(line.trimmed().contains("<author>"))
        {
            result << "author:" + line.replace("<author>","").replace("</author>","").trimmed();
        }
        else if(line.trimmed().contains("<status>"))
        {
            result << "status:" + line.replace("<status>","").replace("</status>","").trimmed();
        }
        else if(line.trimmed().contains("<aircraft-version>"))
        {
            result << "aircraft-version:" + line.replace("<aircraft-version>","").replace("</aircraft-version>","").trimmed();
        }
        else if(line.trimmed().contains("<flight-model>"))
        {
            result << "flight-model:" + line.replace("<flight-model>","").replace("</flight-model>","").trimmed();
        }
        else if(line.trimmed().contains("<fuel-fraction>"))
        {
            result << "fuel-fraction:" + line.replace("<fuel-fraction>","").replace("</fuel-fraction>","").trimmed();
        }
        else if(line.trimmed().contains("<splash-texture>"))
        {
            result << "splash-texture:" + line.replace("<splash-texture>","").replace("</splash-texture>","").trimmed();
        }
        else if(line.trimmed().contains("<aero>"))
        {
            result << "aero:" + line.replace("<aero>","").replace("</aero>","").trimmed();
        }
    }
    aircraftFile.close();
    return result;
}

//------------------------------------------
// set
void Aircraft::setAero(QString string)
{
    aero = string.trimmed();
}

void Aircraft::setAuthor(QString string)
{
    author = string.trimmed();
}

void Aircraft::setDescription(QString string)
{
    description = string.trimmed();
}

void Aircraft::setFlightModel(QString string)
{
    flightmodel = string.trimmed();
}

void Aircraft::setFuelFraction(QString string)
{
    fuelfraction = string.trimmed();
}

void Aircraft::setSplashTexture(QString string)
{
    splashtexture = string.trimmed();
}

void Aircraft::setStatus(QString string)
{
    status = string.trimmed();
}

void Aircraft::setVersion(QString string)
{
    version = string.trimmed();
}
//------------------------------------------
// get
QString Aircraft::getAero()
{
    return aero;
}

QString Aircraft::getAuthor()
{
    return author;
}

QString Aircraft::getDescription()
{
    return description;
}

QString Aircraft::getFlightModel()
{
    return flightmodel;
}

QString Aircraft::getFuelFraction()
{
    return fuelfraction;
}

QString Aircraft::getSplashTexture()
{
    return splashtexture;
}

QString Aircraft::getStatus()
{
    return status;
}

QString Aircraft::getVersion()
{
    return version;
}
