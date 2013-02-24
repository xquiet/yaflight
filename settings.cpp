#include "settings.h"

Settings::Settings(QString confFilePath)
{
    conf = new Configuration(confFilePath);
    if(!conf->exists())
    {
        if(!conf->create(confFilePath,false))
            qFatal("Can't create configuration file");
        conf_empty = true;
    }else{
        conf->parseFile();
        conf_empty = false;
        //qDebug("%s", conf->get(MAINSETTINGS, "AIRCRAFT").toStdString().data());
    }
}

bool Settings::isEmpty()
{
    return conf_empty;
}

bool Settings::storeData()
{
    return conf->store();
}

/*
  GET
  */

//----------------------------------------
// MAIN
QString Settings::getAircraft()
{
    return conf->get(MAINSETTINGS, AIRCRAFT);
}

QString Settings::getSound()
{
    return conf->get(MAINSETTINGS, SOUND);
}


QString Settings::getClouds()
{
    return conf->get(MAINSETTINGS, CLOUDS);
}

QString Settings::getGameMode()
{
    return conf->get(MAINSETTINGS, GAMEMODE);
}

QString Settings::getFullScreen()
{
    return conf->get(MAINSETTINGS, FULLSCREEN);
}

QString Settings::getFog()
{
    return conf->get(MAINSETTINGS, FOG);
}

QString Settings::getResolution()
{
    return conf->get(MAINSETTINGS, RESOLUTION);
}

QString Settings::getCallSign()
{
    return conf->get(MAINSETTINGS, CALLSIGN);
}

QString Settings::getMPServer()
{
    return conf->get(MAINSETTINGS, MPSERVER);
}

QString Settings::getMPIn()
{
    return conf->get(MAINSETTINGS, MPIN);
}

QString Settings::getMPOut()
{
    return conf->get(MAINSETTINGS, MPIN);
}

QString Settings::getControlMode()
{
    return conf->get(MAINSETTINGS, CM);
}

//----------------------------------------
// Advanced
QString Settings::getFailure()
{
    return conf->get(ADVSETTINGS, FAILURE);
}

QString Settings::getFuelLock()
{
    return conf->get(ADVSETTINGS, FUELLOCK);
}

QString Settings::getTimeLock()
{
    return conf->get(ADVSETTINGS, TIMELOCK);
}

QString Settings::getRandomObjects()
{
    return conf->get(ADVSETTINGS, RNDOBJECTS);
}

QString Settings::getAIModels()
{
    return conf->get(ADVSETTINGS, AIMODELS);
}

QString Settings::getRandomTrees()
{
    return conf->get(ADVSETTINGS, RNDTREES);
}

QString Settings::getAutoCoordination()
{
    return conf->get(ADVSETTINGS, AUTOCOORD);
}

QString Settings::getInAir()
{
    return conf->get(ADVSETTINGS, INAIR);
}

QString Settings::getPanel()
{
    return conf->get(ADVSETTINGS, PANEL);
}

QString Settings::getHorizonEffect()
{
    return conf->get(ADVSETTINGS, HORIZEFFECT);
}

QString Settings::getSkyBlending()
{
    return conf->get(ADVSETTINGS, SKYBLENDING);
}

QString Settings::getTextures()
{
    return conf->get(ADVSETTINGS, TEXTURES);
}

QString Settings::getDistanceAttenuation()
{
    return conf->get(ADVSETTINGS, DISTATTENUATION);
}

QString Settings::getDayTime()
{
    return conf->get(ADVSETTINGS, DAYTIME);
}

QString Settings::getSeason()
{
    return conf->get(ADVSETTINGS, SEASON);
}

QString Settings::getTurbulence()
{
    return conf->get(ADVSETTINGS, TURBULENCE);
}

QString Settings::getWind()
{
    return conf->get(ADVSETTINGS, WIND);
}

QString Settings::getRealWeather()
{
    return conf->get(ADVSETTINGS, REALWEATHER);
}

QString Settings::getMeasureUnit()
{
    return conf->get(ADVSETTINGS, MEASUREUNIT);
}

QString Settings::getAltitude()
{
    return conf->get(ADVSETTINGS, ALTITUDE);
}

QString Settings::getHeading()
{
    return conf->get(ADVSETTINGS, HEADING);
}

QString Settings::getLatitude()
{
    return conf->get(ADVSETTINGS, LATITUDE);
}

QString Settings::getLongitude()
{
    return conf->get(ADVSETTINGS, LONGITUDE);
}

QString Settings::getCeilingQuote()
{
    return conf->get(ADVSETTINGS, CEILINGQUOTE);
}

QString Settings::getCeilingThickness()
{
    return conf->get(ADVSETTINGS, CEILINGTHICKNESS);
}

QString Settings::getVisibility()
{
    return conf->get(ADVSETTINGS, VISIBILITY);
}

QString Settings::getHUDAntiAliased()
{
    return conf->get(ADVSETTINGS, HUDANTIALIASED);
}

QString Settings::getHUD2D()
{
    return conf->get(ADVSETTINGS, HUD2D);
}

QString Settings::getHUD3D()
{
    return conf->get(ADVSETTINGS, HUD3D);
}

QString Settings::getEnhancedLighting()
{
    return conf->get(ADVSETTINGS, ENHANCEDRWLIGHTING);
}

QString Settings::getSpecularReflections()
{
    return conf->get(ADVSETTINGS, SPECULARREFLECTIONS);
}

QString Settings::getAirportICAO()
{
    return conf->get(ADVSETTINGS, AIRPORTID);
}

QString Settings::getRunway()
{
    return conf->get(ADVSETTINGS, RUNWAY);
}

QString Settings::getTerraSync()
{
    return conf->get(ADVSETTINGS, TERRASYNC);
}

QString Settings::getFDM()
{
    return conf->get(ADVSETTINGS, FDM);
}

//----------------------------------------
// Sceneries

QString Settings::getSceneries()
{
    return conf->get(SCENERIES, CUSTOMSCENERIES);
}

/*
  SET
  */

//----------------------------------------
// MAIN
void Settings::setAircraft(QString val)
{
    conf->set(QString(MAINSETTINGS), QString(AIRCRAFT), val);
}

void Settings::setSound(QString val)
{
    conf->set(QString(MAINSETTINGS), QString(SOUND), val);
}


void Settings::setClouds(QString val)
{
    conf->set(QString(MAINSETTINGS), QString(CLOUDS), val);
}

void Settings::setGameMode(QString val)
{
    conf->set(QString(MAINSETTINGS), QString(GAMEMODE), val);
}

void Settings::setFullScreen(QString val)
{
    conf->set(QString(MAINSETTINGS), QString(FULLSCREEN),val);
}

void Settings::setFog(QString val)
{
    conf->set(QString(MAINSETTINGS), QString(FOG),val);
}

void Settings::setResolution(QString val)
{
    conf->set(QString(MAINSETTINGS), QString(RESOLUTION),val);
}

void Settings::setCallSign(QString val)
{
    conf->set(QString(MAINSETTINGS), QString(CALLSIGN),val);
}

void Settings::setMPServer(QString val)
{
    conf->set(QString(MAINSETTINGS), QString(MPSERVER),val);
}

void Settings::setMPIn(QString val)
{
    conf->set(QString(MAINSETTINGS), QString(MPIN),val);
}

void Settings::setMPOut(QString val)
{
    conf->set(QString(MAINSETTINGS), QString(MPOUT),val);
}

void Settings::setControlMode(QString val)
{
    conf->set(QString(MAINSETTINGS), QString(CM),val);
}

//----------------------------------------
// Advanced
void Settings::setFailure(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(FAILURE),val);
}

void Settings::setFuelLock(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(FUELLOCK),val);
}

void Settings::setTimeLock(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(TIMELOCK), val);
}

void Settings::setRandomObjects(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(RNDOBJECTS), val);
}

void Settings::setAIModels(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(AIMODELS), val);
}

void Settings::setRandomTrees(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(RNDTREES), val);
}

void Settings::setAutoCoordination(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(AUTOCOORD), val);
}

void Settings::setInAir(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(INAIR), val);
}

void Settings::setPanel(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(PANEL), val);
}

void Settings::setHorizonEffect(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(HORIZEFFECT), val);
}

void Settings::setSkyBlending(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(SKYBLENDING), val);
}

void Settings::setTextures(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(TEXTURES), val);
}

void Settings::setDistanceAttenuation(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(DISTATTENUATION), val);
}

void Settings::setDayTime(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(DAYTIME), val);
}

void Settings::setSeason(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(SEASON), val);
}

void Settings::setTurbulence(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(TURBULENCE), val);
}

void Settings::setWind(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(WIND), val);
}

void Settings::setRealWeather(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(REALWEATHER), val);
}

void Settings::setMeasureUnit(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(MEASUREUNIT), val);
}

void Settings::setAltitude(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(ALTITUDE), val);
}

void Settings::setHeading(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(HEADING), val);
}

void Settings::setLatitude(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(LATITUDE), val);
}

void Settings::setLongitude(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(LONGITUDE), val);
}

void Settings::setCeilingQuote(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(CEILINGQUOTE), val);
}

void Settings::setCeilingThickness(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(CEILINGTHICKNESS), val);
}

void Settings::setVisibility(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(VISIBILITY), val);
}

void Settings::setHUDAntiAliased(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(HUDANTIALIASED), val);
}

void Settings::setHUD2D(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(HUD2D), val);
}

void Settings::setHUD3D(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(HUD3D), val);
}

void Settings::setEnhancedLighting(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(ENHANCEDRWLIGHTING), val);
}

void Settings::setSpecularReflections(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(SPECULARREFLECTIONS), val);
}

void Settings::setAirportID(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(AIRPORTID), val);
}

void Settings::setRunway(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(RUNWAY), val);
}

void Settings::setTerraSync(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(TERRASYNC), val);
}

void Settings::setFDM(QString val)
{
    conf->set(QString(ADVSETTINGS), QString(FDM), val);
}

//----------------------------------------
// Sceneries

void Settings::setSceneries(QString val)
{
    conf->set(QString(SCENERIES), QString(CUSTOMSCENERIES), val);
}
