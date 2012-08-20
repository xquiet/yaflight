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

QStringList FGEnvironment::getAircraftDetails(QString aircraft, QString aircraftDir)
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

    QFile aircraftFile (getAircraftDir()+"/"+aircraftDir+"/"+aircraft+"-set.xml");
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
    }
    aircraftFile.close();
    return result;
}
