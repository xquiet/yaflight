#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <QStringList>

class Aircraft
{
public:
    Aircraft(QStringList data);
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
