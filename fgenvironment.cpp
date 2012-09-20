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

QString FGEnvironment::getFgfsBinPath()
{
    return "/usr/bin/fgfs";
}

QString FGEnvironment::getYFHome()
{
    return QDir::homePath()+"/.yaflight";
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
    qDebug("%s",(getAircraftDir()+"/"+aircraftDir+"/"+aircraft+"-set.xml").toStdString().data());
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
        else if(line.trimmed().contains("<aero>"))
        {
            result << "aero:" + line.replace("<aero>","").replace("</aero>","").trimmed();
        }
    }
    aircraftFile.close();
    return result;
}

QString FGEnvironment::detectOS()
{
#ifdef Q_OS_LINUX
    operating_system = "GNU/Linux";

    sysinfo = new QProcess();
    sysinfo->setProcessChannelMode(QProcess::MergedChannels);
    sysinfo->start("/usr/bin/env",QStringList() << "lsb_release" << "-a", QProcess::ReadOnly);
    sysinfo->waitForFinished();
    QByteArray bytes = sysinfo->readAll();
    QStringList strLines = QString(bytes).split("\n");
    QStringList cols;
    foreach (QString line, strLines){
        line = line.trimmed();
        if(line.contains("Distributor ID")){
            cols = line.split(":",QString::SkipEmptyParts,Qt::CaseSensitive);
            if (cols.length()>0)
                os_details.append(cols[1].trimmed());
        }
    }
    return operating_system + " - " + os_details.join(" ");
#endif
#ifdef Q_OS_UNIX
    operating_system = "Unix";
#endif
#ifdef Q_OS_WIN32
    operating_system = "Windows";
#endif
#ifdef Q_OS_MAC
    operating_system = "Mac";
#else
    return "";
#endif
}

QString FGEnvironment::detectFGVersion()
{
    QString output = "";
    QFile version(getRootPath()+"/"+"version");
    if(version.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream in(&version);
        while(!in.atEnd())
        {
            output += in.readLine();
        }
        version.close();
    }

    return output;
}

QString FGEnvironment::detectFGBinPath()
{

}

QString FGEnvironment::detectRootPath()
{
    QStringList possiblePaths;
    possiblePaths << "/usr/share/flightgear/data"
                  << "/usr/share/FlightGear"
                  << "/usr/share/games/FlightGear"
                  << "/usr/share/games/flightgear";
    for(int i=0;i<possiblePaths.count();i++)
    {
        if(QFile::exists(possiblePaths[i]))
            return possiblePaths[i];
    }
    return "";
}
