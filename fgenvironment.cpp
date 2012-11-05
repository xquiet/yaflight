/**
    Copyright (C) 2012  Matteo Pasotti <matteo.pasotti@gmail.com>

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

#include "fgenvironment.h"

FGEnvironment::FGEnvironment()
{

}

bool FGEnvironment::start()
{
    operating_system = detectOS();
    fgRootPath = detectRootPath();
    // detectFGVersion needs the rootpath
    fgVersion = detectFGVersion();

    if(fgRootPath.trimmed().compare("")==0)
        return false;

    QDir dir(getYFScenery());
    if(!dir.exists())
    {
        //if(dir.mkpath(getYFScenery()))
        if(!dir.mkdir(getYFScenery()))
        {
            qFatal("Cannot create dir: %s",getYFScenery().toStdString().data());
            return false;
        }
    }
    return true;
}

void FGEnvironment::setRootPath(QString path)
{
    fgRootPath = path.trimmed();
}

QString FGEnvironment::getOS()
{
    return operating_system;
}

QString FGEnvironment::getFGVersion()
{
    return fgVersion;
}

QString FGEnvironment::getRootPath()
{
    return fgRootPath;
}

QString FGEnvironment::getAircraftsDir()
{
    return getRootPath()+"/Aircraft";
}

QString FGEnvironment::getDefaultAirportsDir()
{
    return getDefaultScenery() + "/Airports";
}

QString FGEnvironment::getAPTSource()
{
    return getRootPath() + "/Airports/apt.dat.gz";
}

QHash<QString, QStringList> FGEnvironment::parseAirportsIndex(QString path)
{
    QFile airports(path);
    QHash<QString, QStringList> result;
    QStringList pair;
    if(airports.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream in(&airports);
        result.begin();
        while(!in.atEnd())
        {
            /*
              pair[0] ICAO
              pair[1] longitude
              pair[2] latitude

              resultHash -> | string | <lon, lat, empty space for directory> |
              */
            pair = in.readLine().split("|");
            result.insert(pair[0],QStringList() << pair[1] << pair[2] << "");
        }
        result.end();
        airports.close();
    }
    return result;
}

QHash<QString, QStringList> FGEnvironment::getDefaultAirportList()
{
    return parseAirportsIndex(getDefaultAirportsDir()+"/"+"index.txt");
}

QString FGEnvironment::getFgfsBinPath()
{
    #ifdef Q_OS_WIN
        return "\""+getRootPath()+"/../bin/Win32/fgfs.exe\"";
    #elif defined Q_OS_UNIX
        #ifdef Q_OS_LINUX
            return "/usr/bin/fgfs";
        #else
            //bsd
            return "/usr/local/bin/fgfs";
        #endif
    #endif
    return "";
}

QString FGEnvironment::getTerraSyncBinPath()
{
    QString ts_path = "";
    #ifdef Q_OS_WIN
        ts_path = "\""+getRootPath()+"/../bin/Win32/terrasync.exe\"";
    #elif defined Q_OS_UNIX
        #ifdef Q_OS_LINUX
            ts_path = "/usr/bin/terrasync";
        #else
            //bsd
            ts_path = "/usr/local/bin/terrasync";
        #endif
    #endif
    return ts_path;
}

QString FGEnvironment::getTerraSyncPidFilePath()
{
    return getYFHome() + "/ts_pid";
}

QString FGEnvironment::getDefaultScenery()
{
    // default scenery
    return getRootPath()+"/Scenery";
}

QString FGEnvironment::getYFHome()
{
    QString yfhome = "";
    #ifdef Q_OS_WIN
        yfhome = "/yaflight";
    #else
        #if defined Q_OS_LINUX || defined Q_OS_UNIX
            yfhome = "/.yaflight";
        #endif
    #endif
    yfhome = QDir::homePath() + yfhome;
    if (!QFile::exists(yfhome))
    {
        QDir tmppath(QDir::homePath());
        tmppath.mkpath(yfhome);
    }
    return yfhome;
}

QString FGEnvironment::getYFScenery()
{
    return getYFHome() + "/Scenery";
}

QString FGEnvironment::detectOS()
{
    #ifdef Q_OS_UNIX
        #ifdef Q_OS_MAC
            return "Mac";
        #elif defined Q_OS_LINUX
            QString os = "GNU/Linux";

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
            return os + " - " + os_details.join(" ");
        #else
            //bsd
            QString os = "Unix";
            sysinfo = new QProcess();
            sysinfo->setProcessChannelMode(QProcess::MergedChannels);
            sysinfo->start("/usr/bin/env",QStringList() << "uname" << "-s", QProcess::ReadOnly);
            sysinfo->waitForFinished();
            QByteArray bytes = sysinfo->readAll();
            os = os + " " + QString(bytes).split("\n")[0];
            return os;
        #endif
    #endif
    #ifdef Q_OS_WIN
        QProcessEnvironment pe(QProcessEnvironment::systemEnvironment());
        _win_program_files = pe.value("ProgramFiles").replace("\\","/");

        return "Windows";
    #endif
    return "";
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
    else
    {
        qWarning("Failed to open %s for read: %s",
                 version.fileName().toStdString().data(),
                 version.errorString().toStdString().data());
    }
    return output;
}

QString FGEnvironment::detectRootPath()
{
    QStringList possiblePaths;
    #ifdef Q_OS_WIN
        possiblePaths << _win_program_files + "/FlightGear/data";
        //possiblePaths << "C:/Program Files/FlightGear/data";
    #endif
    #ifdef Q_OS_UNIX
        #ifdef Q_OS_MAC
        #elif defined Q_OS_LINUX
            possiblePaths << "/usr/share/flightgear/data"
                          << "/usr/share/FlightGear"
                          << "/usr/share/games/FlightGear"
                          << "/usr/share/games/flightgear";
        #else
            //bsd
            possiblePaths << "/usr/local/share/flightgear";
        #endif
    #endif
    for(int i=0;i<possiblePaths.count();i++)
    {
        if(QFile::exists(possiblePaths[i]))
            return possiblePaths[i];
    }
    return QString();
}


// --------------------------
// DEBUG methods

QString FGEnvironment::__read_winprogramfiles()
{
    return _win_program_files;
}
