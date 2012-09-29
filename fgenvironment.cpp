#include "fgenvironment.h"

FGEnvironment::FGEnvironment()
{
    operating_system = detectOS();
    fgRootPath = detectRootPath();
    // detectFGVersion needs the rootpath
    fgVersion = detectFGVersion();
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
#endif
#ifdef Q_OS_LINUX
    return "/usr/bin/fgfs";
#endif
    return "";
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
#endif
#ifdef Q_OS_LINUX
    yfhome = "/.yaflight";
#endif
    yfhome = QDir::homePath() + yfhome;
    if (!QFile::exists(yfhome))
    {
        QDir tmppath(QDir::homePath());
        tmppath.mkpath(yfhome);
    }
    return yfhome;
}

QString FGEnvironment::detectOS()
{
    #ifdef Q_OS_UNIX
        #ifdef Q_OS_MAC
            return "Mac";
        #endif
        #ifdef Q_OS_LINUX
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
            return "Unix";
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
    qWarning("Failed to open %s for write: %s",
             version.fileName().toStdString().data(),
             version.errorString().toStdString().data());
    return output;
}

QString FGEnvironment::detectFGBinPath()
{
    return "";
}

QString FGEnvironment::detectRootPath()
{
    QStringList possiblePaths;
#ifdef Q_OS_WIN
    possiblePaths << _win_program_files + "/FlightGear/data";
    //possiblePaths << "C:/Program Files/FlightGear/data";
#endif
#ifdef Q_OS_MAC
#endif
#ifdef Q_OS_LINUX
    possiblePaths << "/usr/share/flightgear/data"
                  << "/usr/share/FlightGear"
                  << "/usr/share/games/FlightGear"
                  << "/usr/share/games/flightgear";
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
