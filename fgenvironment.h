#ifndef FGENVIRONMENT_H
#define FGENVIRONMENT_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QtGlobal>
#include <QProcess>
#include <QEvent>
#include <QDir>

class FGEnvironment
{
private:
    QString fgRootPath;
    QString fgVersion;
    QProcess *sysinfo;
    QString operating_system;
    QStringList os_details;

    // windows_exception
    QString _win_program_files;

    QString detectOS();
    QString detectFGVersion();
    QString detectFGBinPath();
    QString detectRootPath();

public:
    FGEnvironment();
    void setRootPath(QString path);
    QString getRootPath();
    QString getDefaultScenery();
    QString getAircraftsDir();
    QStringList getAirportList();
    QString getFgfsBinPath();
    QString getYFHome();
    QString getOS();
    QString getFGVersion();

    // debug
    QString __read_winprogramfiles();



};

#endif // FGENVIRONMENT_H
