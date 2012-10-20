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

#ifndef SETTINGS_H
#define SETTINGS_H

#include "configuration.h"

#define SET_TRUE "-1"
#define SET_FALSE "0"

class Settings
{
public:
    Settings(QString);
    bool isEmpty();
    bool storeData();

    // app
    QString getAirportListFiltered();
    // main
    QString getAircraft();
    QString getClouds();
    QString getFog();
    QString getGameMode();
    QString getFullScreen();
    QString getSound();
    QString getResolution();
    QString getCallSign();
    // adv
    QString getFailure();
    QString getFuelLock();
    QString getTimeLock();
    QString getRandomObjects();
    QString getAIModels();
    QString getAutoCoordination();
    QString getPanel();
    QString getHorizonEffect();
    QString getSkyBlending();
    QString getTextures();
    QString getDistanceAttenuation();
    QString getDayTime();
    QString getSeason();
    QString getTurbulence();
    QString getWind();
    QString getMeasureUnit();
    QString getAltitude();
    QString getHeading();
    QString getLatitude();
    QString getLongitude();
    QString getCeilingQuote();
    QString getCeilingThickness();
    QString getHUDAntiAliased();
    QString getHUD2D();
    QString getHUD3D();
    QString getEnhancedLighting();
    QString getSpecularReflections();
    QString getAirportICAO();
    QString getRunway();
    QString getTerraSync();
    QString getFDM();
    // sceneries
    QString getSceneries();


    // app
    void setAirportListFiltered(QString val);
    // main
    void setAircraft(QString val);
    void setClouds(QString val);
    void setFog(QString val);
    void setGameMode(QString val);
    void setFullScreen(QString val);
    void setSound(QString val);
    void setResolution(QString val);
    void setCallSign(QString val);
    // adv
    void setFailure(QString val);
    void setFuelLock(QString val);
    void setTimeLock(QString val);
    void setRandomObjects(QString val);
    void setAIModels(QString val);
    void setAutoCoordination(QString val);
    void setPanel(QString val);
    void setHorizonEffect(QString val);
    void setSkyBlending(QString val);
    void setTextures(QString val);
    void setDistanceAttenuation(QString val);
    void setDayTime(QString val);
    void setSeason(QString val);
    void setTurbulence(QString val);
    void setWind(QString val);
    void setMeasureUnit(QString val);
    void setAltitude(QString val);
    void setHeading(QString val);
    void setLatitude(QString val);
    void setLongitude(QString val);
    void setCeilingQuote(QString val);
    void setCeilingThickness(QString val);
    void setHUDAntiAliased(QString val);
    void setHUD2D(QString val);
    void setHUD3D(QString val);
    void setEnhancedLighting(QString val);
    void setSpecularReflections(QString val);
    void setAirportID(QString val);
    void setRunway(QString val);
    void setTerraSync(QString val);
    void setFDM(QString val);
    // sceneries
    void setSceneries(QString val);

private:
    Configuration *conf;
    bool conf_empty;
};

#endif // SETTINGS_H
