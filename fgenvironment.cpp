#include "fgenvironment.h"

FGEnvironment::FGEnvironment()
{
    fgRootPath = "/usr/share/games/flightgear";
}

void FGEnvironment::setRootPath(QString path)
{
    fgRootPath = path.trimmed();
}

QString FGEnvironment::getRootPath()
{
    return fgRootPath;
}

QString FGEnvironment::getAircraftDir()
{
    return fgRootPath+"/Aircraft";
}
