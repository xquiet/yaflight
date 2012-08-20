#ifndef FGENVIRONMENT_H
#define FGENVIRONMENT_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>

class FGEnvironment
{
private:
    QString fgRootPath;
public:
    FGEnvironment();
    void setRootPath(QString path);
    QString getRootPath();
    QString getAircraftDir();
    QStringList getAircraftDetails(QString aircraft, QString aircraftDir);
};

#endif // FGENVIRONMENT_H
