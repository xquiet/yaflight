#include "settings.h"

Settings::Settings(QString confFilePath)
{
    conf = new Configuration(confFilePath);
    if(!conf->exists())
    {
        if(!conf->create(confFilePath))
            qFatal("Can't create configuration file");
        conf_empty = true;
    }else{
        conf->parseFile();
        conf_empty = false;
        //qDebug("%s", conf->get("main", "AIRCRAFT").toStdString().data());
    }
}

bool Settings::isEmpty()
{
    return conf_empty;
}

/*
  GET
  */

//----------------------------------------
// MAIN
QString Settings::getAircraft()
{
    return conf->get("main", "AIRCRAFT");
}

QString Settings::getSound()
{
    return conf->get("main", "SOUND");
}


QString Settings::getClouds()
{
    return conf->get("main", "CLOUDS");
}

QString Settings::getGameMode()
{
    return conf->get("main", "GAMEMODE");
}

QString Settings::getFullScreen()
{
    return conf->get("main", "FULLSCREEN");
}

QString Settings::getFog()
{
    return conf->get("main", "FOG");
}

QString Settings::getResolution()
{
    return conf->get("main", "RESOLUTION");
}

//----------------------------------------
// Advanced
QString Settings::getFailure()
{
    return conf->get("advanced", "FAILURE");
}

QString Settings::getFuelLock()
{
    return conf->get("advanced", "FUELLOCK");
}

QString Settings::getTimeLock()
{
    return conf->get("advanced", "TIMELOCK");
}

QString Settings::getRandomObjects()
{
    return conf->get("advanced", "RNDOBJECTS");
}

QString Settings::getAIModels()
{
    return conf->get("advanced", "AIMODELS");
}

QString Settings::getAutoCoordination()
{
    return conf->get("advanced", "AUTOCOORD");
}

QString Settings::getPanel()
{
    return conf->get("advanced", "PANEL");
}

QString Settings::getHorizonEffect()
{
    return conf->get("advanced", "HORIZEFFECT");
}

QString Settings::getSkyBlending()
{
    return conf->get("advanced", "SKYBLENDING");
}

QString Settings::getTextures()
{
    return conf->get("advanced", "TEXTURES");
}

QString Settings::getDistanceAttenuation()
{
    return conf->get("advanced", "DISTATTENUATION");
}

QString Settings::getDayTime()
{
    return conf->get("advanced", "DAYTIME");
}

QString Settings::getSeason()
{
    return conf->get("advanced", "SEASON");
}

QString Settings::getTurbulence()
{
    return conf->get("advanced", "TURBULENCE");
}

QString Settings::getWind()
{
    return conf->get("advanced", "WIND");
}

QString Settings::getMeasureUnit()
{
    return conf->get("advanced", "MEASUREUNIT");
}

QString Settings::getAltitude()
{
    return conf->get("advanced", "ALTITUDE");
}

QString Settings::getHeading()
{
    return conf->get("advanced", "HEADING");
}

QString Settings::getLatitude()
{
    return conf->get("advanced", "LATITUDE");
}

QString Settings::getLongitude()
{
    return conf->get("advanced", "LONGITUDE");
}

QString Settings::getCeilingQuote()
{
    return conf->get("advanced", "CEILINGQUOTE");
}

QString Settings::getCeilingThickness()
{
    return conf->get("advanced", "CEILINGTHICKNESS");
}

QString Settings::getHUDAntiAliased()
{
    return conf->get("advanced", "HUDANTIALIASED");
}

QString Settings::getHUD2D()
{
    return conf->get("advanced", "HUD2D");
}

QString Settings::getHUD3D()
{
    return conf->get("advanced", "HUD3D");
}

QString Settings::getEnhancedLighting()
{
    return conf->get("advanced", "ENHANCEDRWLIGHTING");
}

QString Settings::getSpecularReflections()
{
    return conf->get("advanced", "SPECULARREFLECTIONS");
}

QString Settings::getAirportID()
{
    return conf->get("advanced", "AIRPORTID");
}

QString Settings::getRunway()
{
    return conf->get("advanced", "RUNWAY");
}

QString Settings::getTerraSync()
{
    return conf->get("advanced", "TERRASYNC");
}


/*
  SET
  */


//----------------------------------------
// MAIN
void Settings::setAircraft(QString val)
{
    conf->set(QString("main"), QString("AIRCRAFT"), val);
}

void Settings::setSound(QString val)
{
    conf->set(QString("main"), QString("SOUND"), val);
}


void Settings::setClouds(QString val)
{
    conf->set(QString("main"), QString("CLOUDS"), val);
}

void Settings::setGameMode(QString val)
{
    conf->set(QString("main"), QString("GAMEMODE"), val);
}

void Settings::setFullScreen(QString val)
{
    conf->set(QString("main"), QString("FULLSCREEN"),val);
}

void Settings::setFog(QString val)
{
    conf->set(QString("main"), QString("FOG"),val);
}

void Settings::setResolution(QString val)
{
    conf->set(QString("main"), QString("RESOLUTION"),val);
}

//----------------------------------------
// Advanced
void Settings::setFailure(QString val)
{
    conf->set(QString("advanced"), QString("FAILURE"),val);
}

void Settings::setFuelLock(QString val)
{
    conf->set(QString("advanced"), QString("FUELLOCK"),val);
}

void Settings::setTimeLock(QString val)
{
    conf->set(QString("advanced"), QString("TIMELOCK"), val);
}

void Settings::setRandomObjects(QString val)
{
    conf->set(QString("advanced"), QString("RNDOBJECTS"), val);
}

void Settings::setAIModels(QString val)
{
    conf->set(QString("advanced"), QString("AIMODELS"), val);
}

void Settings::setAutoCoordination(QString val)
{
    conf->set(QString("advanced"), QString("AUTOCOORD"), val);
}

void Settings::setPanel(QString val)
{
    conf->set(QString("advanced"), QString("PANEL"), val);
}

void Settings::setHorizonEffect(QString val)
{
    conf->set(QString("advanced"), QString("HORIZEFFECT"), val);
}

void Settings::setSkyBlending(QString val)
{
    conf->set(QString("advanced"), QString("SKYBLENDING"), val);
}

void Settings::setTextures(QString val)
{
    conf->set(QString("advanced"), QString("TEXTURES"), val);
}

void Settings::setDistanceAttenuation(QString val)
{
    conf->set(QString("advanced"), QString("DISTATTENUATION"), val);
}

void Settings::setDayTime(QString val)
{
    conf->set(QString("advanced"), QString("DAYTIME"), val);
}

void Settings::setSeason(QString val)
{
    conf->set(QString("advanced"), QString("SEASON"), val);
}

void Settings::setTurbulence(QString val)
{
    conf->set(QString("advanced"), QString("TURBULENCE"), val);
}

void Settings::setWind(QString val)
{
    conf->set(QString("advanced"), QString("WIND"), val);
}

void Settings::setMeasureUnit(QString val)
{
    conf->set(QString("advanced"), QString("MEASUREUNIT"), val);
}

void Settings::setAltitude(QString val)
{
    conf->set(QString("advanced"), QString("ALTITUDE"), val);
}

void Settings::setHeading(QString val)
{
    conf->set(QString("advanced"), QString("HEADING"), val);
}

void Settings::setLatitude(QString val)
{
    conf->set(QString("advanced"), QString("LATITUDE"), val);
}

void Settings::setLongitude(QString val)
{
    conf->set(QString("advanced"), QString("LONGITUDE"), val);
}

void Settings::setCeilingQuote(QString val)
{
    conf->set(QString("advanced"), QString("CEILINGQUOTE"), val);
}

void Settings::setCeilingThickness(QString val)
{
    conf->set(QString("advanced"), QString("CEILINGTHICKNESS"), val);
}

void Settings::setHUDAntiAliased(QString val)
{
    conf->set(QString("advanced"), QString("HUDANTIALIASED"), val);
}

void Settings::setHUD2D(QString val)
{
    conf->set(QString("advanced"), QString("HUD2D"), val);
}

void Settings::setHUD3D(QString val)
{
    conf->set(QString("advanced"), QString("HUD3D"), val);
}

void Settings::setEnhancedLighting(QString val)
{
    conf->set(QString("advanced"), QString("ENHANCEDRWLIGHTING"), val);
}

void Settings::setSpecularReflections(QString val)
{
    conf->set(QString("advanced"), QString("SPECULARREFLECTIONS"), val);
}

void Settings::setAirportID(QString val)
{
    conf->set(QString("advanced"), QString("AIRPORTID"), val);
}

void Settings::setRunway(QString val)
{
    conf->set(QString("advanced"), QString("RUNWAY"), val);
}

void Settings::setTerraSync(QString val)
{
    conf->set(QString("advanced"), QString("TERRASYNC"), val);
}