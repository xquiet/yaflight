#include "aircraft.h"

Aircraft::Aircraft(QStringList data)
{
    foreach(QString item, data)
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
