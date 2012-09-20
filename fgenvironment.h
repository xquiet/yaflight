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
    QProcess *sysinfo;
    QString operating_system;
    QStringList os_details;

public:
    FGEnvironment();
    void setRootPath(QString path);
    QString getRootPath();
    QString getAircraftDir();
    QStringList getAircraftDetails(QString aircraft, QString aircraftDir);
    QString getFgfsBinPath();
    QString getYFHome();

    QString detectOS();
    QString detectFGVersion();
    QString detectFGBinPath();
    QString detectRootPath();

};

#endif // FGENVIRONMENT_H
