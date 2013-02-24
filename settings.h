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

#ifndef SETTINGS_H
#define SETTINGS_H

#include "configuration.h"

#define SET_TRUE "-1"
#define SET_FALSE "0"

#define AIRCRAFT    "AIRCRAFT"
#define SOUND       "SOUND"
#define CLOUDS      "CLOUDS"
#define GAMEMODE    "GAMEMODE"
#define FULLSCREEN  "FULLSCREEN"
#define FOG         "FOG"
#define RESOLUTION  "RESOLUTION"
#define CALLSIGN    "CALLSIGN"
#define MPSERVER    "MPSERV"
#define MPIN        "MPI"
#define MPOUT       "MPO"
#define CM          "CM"
// ------- advsettings --------------
#define FAILURE     "FAILURE"
#define FUELLOCK    "FUELLOCK"
#define TIMELOCK    "TIMELOCK"
#define RNDOBJECTS  "RNDOBJECTS"
#define RNDTREES    "RNDTREES"
#define AIMODELS    "AIMODELS"
#define AUTOCOORD   "AUTOCOORD"
#define INAIR       "INAIR"
#define PANEL       "PANEL"
#define HORIZEFFECT "HORIZEFFECT"
#define SKYBLENDING "SKYBLENDING"
#define TEXTURES    "TEXTURES"
#define DISTATTENUATION "DISTATTENUATION"
#define DAYTIME     "DAYTIME"
#define SEASON      "SEASON"
#define TURBULENCE  "TURBULENCE"
#define WIND        "WIND"
#define REALWEATHER "REALWEATHER"
#define MEASUREUNIT "MEASUREUNIT"
#define ALTITUDE    "ALTITUDE"
#define HEADING     "HEADING"
#define LATITUDE    "LATITUDE"
#define LONGITUDE   "LONGITUDE"
#define CEILINGQUOTE "CEILINGQUOTE"
#define CEILINGTHICKNESS "CEILINGTHICKNESS"
#define VISIBILITY  "VISIBILITY"
#define HUDANTIALIASED "HUDANTIALIASED"
#define HUD2D       "HUD2D"
#define HUD3D       "HUD3D"
#define ENHANCEDRWLIGHTING "ENHANCEDRWLIGHTING"
#define SPECULARREFLECTIONS "SPECULARREFLECTIONS"
#define AIRPORTID   "AIRPORTID"
#define RUNWAY      "RUNWAY"
#define TERRASYNC   "TERRASYNC"
#define FDM         "FDM"

#define CUSTOMSCENERIES "CUSTOMSCENERIES"

class Settings
{
public:
    Settings(QString);
    bool isEmpty();
    bool storeData();

    // main
    QString getAircraft();
    QString getClouds();
    QString getFog();
    QString getGameMode();
    QString getFullScreen();
    QString getSound();
    QString getResolution();
    QString getCallSign();
    QString getMPServer();
    QString getMPIn();
    QString getMPOut();
    QString getControlMode();
    // adv
    QString getFailure();
    QString getFuelLock();
    QString getTimeLock();
    QString getRandomObjects();
    QString getAIModels();
    QString getRandomTrees();
    QString getAutoCoordination();
    QString getInAir();
    QString getAltitude();
    QString getPanel();
    QString getHorizonEffect();
    QString getSkyBlending();
    QString getTextures();
    QString getDistanceAttenuation();
    QString getDayTime();
    QString getSeason();
    QString getTurbulence();
    QString getWind();
    QString getRealWeather();
    QString getMeasureUnit();
    QString getHeading();
    QString getLatitude();
    QString getLongitude();
    QString getCeilingQuote();
    QString getCeilingThickness();
    QString getVisibility();
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


    // main
    void setAircraft(QString val);
    void setClouds(QString val);
    void setFog(QString val);
    void setGameMode(QString val);
    void setFullScreen(QString val);
    void setSound(QString val);
    void setResolution(QString val);
    void setCallSign(QString val);
    void setMPServer(QString val);
    void setMPIn(QString val);
    void setMPOut(QString val);
    void setControlMode(QString val);
    // adv
    void setFailure(QString val);
    void setFuelLock(QString val);
    void setTimeLock(QString val);
    void setRandomObjects(QString val);
    void setAIModels(QString val);
    void setRandomTrees(QString val);
    void setAutoCoordination(QString val);
    void setInAir(QString val);
    void setAltitude(QString val);
    void setPanel(QString val);
    void setHorizonEffect(QString val);
    void setSkyBlending(QString val);
    void setTextures(QString val);
    void setDistanceAttenuation(QString val);
    void setDayTime(QString val);
    void setSeason(QString val);
    void setTurbulence(QString val);
    void setWind(QString val);
    void setRealWeather(QString val);
    void setMeasureUnit(QString val);
    void setHeading(QString val);
    void setLatitude(QString val);
    void setLongitude(QString val);
    void setCeilingQuote(QString val);
    void setCeilingThickness(QString val);
    void setVisibility(QString val);
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
