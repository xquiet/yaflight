#include "appsettings.h"

QString appsettings::updatesHost = "yaflight.altervista.org";
QString appsettings::updatesScript = "/index.php/downloads/last_available_version";

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

QString appsettings::getUpdatesHost()
{
    return updatesHost;
}

QString appsettings::getUpdatesScript()
{
    return updatesScript;
}

QStringList appsettings::getAvailableThemes()
{
    Yalib *ya = new Yalib();
    QString homeDir = ya->getYFHome();
    QDir dir(homeDir+"/themes");
    if(dir.exists())
    {
        QStringList results;
        QStringList fileList = dir.entryList(QDir::Files);
        foreach(QString file, fileList)
        {
            if(file.endsWith(".qss"))
            {
                results << file.replace(".qss","");
            }
        }
        return results;
    }
    qDebug("Directory %s not found", dir.absolutePath().toStdString().data());
    return QStringList();
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

QString appsettings::getCurrentTheme()
{
    return conf->get(QString(APPSETTINGS), YFCURRTHEME);
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

void appsettings::setCurrentTheme(QString val)
{
    conf->set(QString(APPSETTINGS), QString(YFCURRTHEME), val);
}
