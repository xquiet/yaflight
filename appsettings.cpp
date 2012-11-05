#include "appsettings.h"

appsettings::appsettings(QString confFilePath)
{
    conf = new Configuration(confFilePath);
    if(!conf->exists())
    {
        if(!conf->create(confFilePath, true))
            qFatal("Can't create configuration file");
        conf_empty = true;
    }else{
        conf->parseFile();
        conf_empty = false;
    }
}

bool appsettings::isEmpty()
{
    return conf_empty;
}

bool appsettings::storeData()
{
    return conf->store();
}

//----------------------------------------
// App - GET

QString appsettings::getAirportListFiltered()
{
    return conf->get(QString(APPSETTINGS), APTVIEW);
}

QString appsettings::get_fgfs_bin_path()
{
    return conf->get(QString(APPSETTINGS), FGFSBIN);
}

QString appsettings::getFGDataPath()
{
    return conf->get(QString(APPSETTINGS), FGDATADIR);
}



//----------------------------------------
// App - SET

void appsettings::setAirportListFiltered(QString val)
{
    conf->set(QString(APPSETTINGS), QString(APTVIEW), val);
}

void appsettings::set_fgfs_bin_path(QString val)
{
    conf->set(QString(APPSETTINGS), QString(FGFSBIN), val);
}

void appsettings::setFGDataPath(QString val)
{
    conf->set(QString(APPSETTINGS), QString(FGDATADIR), val);
}
