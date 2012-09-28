#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <QStringList>
#include <QFile>
#include <QTextStream>

class Aircraft
{
public:
    Aircraft(QString aircraftName, QString aircraftsDir, QString aircraftDir);
    QStringList getAircraftDetails(QString, QString aircraftsDir, QString aircraftDir);
    // set
    void setAero(QString);
    void setDescription(QString);
    void setAuthor(QString);
    void setStatus(QString);
    void setVersion(QString);
    void setFlightModel(QString);
    void setFuelFraction(QString);
    void setSplashTexture(QString);
    // get
    QString getAero();
    QString getDescription();
    QString getAuthor();
    QString getStatus();
    QString getVersion();
    QString getFlightModel();
    QString getFuelFraction();
    QString getSplashTexture();

private:
    QString aero,
            description,
            author,
            status,
            version,
            flightmodel,
            fuelfraction,
            splashtexture;
};

#endif // AIRCRAFT_H
