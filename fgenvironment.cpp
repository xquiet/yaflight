#include "fgenvironment.h"

FGEnvironment::FGEnvironment()
{
    ya = new Yalib();
}

bool FGEnvironment::start(bool autoDetect)
{
    return ya->initialize(autoDetect);
}

void FGEnvironment::setRootPath(QString path)
{
    ya->setRootPath(path);
}

void FGEnvironment::setFgFsBinary(QString path)
{
    ya->setFgfsBinPath(path);
}

QString FGEnvironment::getOS()
{
    return ya->getOS();
}

QString FGEnvironment::getFGVersion()
{
    return ya->getFGVersion();
}

QString FGEnvironment::getRootPath()
{
    return ya->getRootPath();
}

QString FGEnvironment::getAircraftsDir()
{
    return ya->getAircraftsDir();
}

QString FGEnvironment::getDefaultAirportsDir()
{
    return ya->getDefaultAirportsDir();
}

QString FGEnvironment::getAPTSource()
{
    return ya->getAPTSource();
}

QHash<QString, QStringList> FGEnvironment::parseAirportsIndex(QString path)
{
    return ya->parseAirportsIndex(path);
}

QHash<QString, QStringList> FGEnvironment::getDefaultAirportList()
{
    return ya->getDefaultAirportList();
}

QString FGEnvironment::detectFGBinPath(bool autodetect)
{
    return ya->detectFGBinPath(autodetect);
}

QString FGEnvironment::getFgfsBinPath()
{
    return ya->getFgfsBinPath();
}

QString FGEnvironment::getTerraSyncBinPath()
{
    return ya->getTerraSyncBinPath();
}

QString FGEnvironment::getTerraSyncPidFilePath()
{
    return ya->getTerraSyncPidFilePath();
}

QString FGEnvironment::getDefaultScenery()
{
    return ya->getDefaultScenery();
}

QString FGEnvironment::getYFHome()
{
    return ya->getYFHome();
}

QString FGEnvironment::getYFScenery()
{
    return ya->getYFScenery();
}

QString FGEnvironment::detectOS()
{
    return ya->detectOS();
}

QString FGEnvironment::detectFGVersion()
{
    return ya->detectFGVersion();
}

QString FGEnvironment::detectRootPath()
{
    return ya->detectRootPath();
}


// --------------------------
// DEBUG methods

QString FGEnvironment::__read_winprogramfiles()
{
    return _win_program_files;
}
