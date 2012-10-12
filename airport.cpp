#include "airport.h"

Airport::Airport(QString airportsDir, QString icao)
{
    // build airport directory path
    QStringList prova = icao.trimmed().split("");
    // ottengo un vettore di 6 elementi
    // i cui estremi non mi interessano
    prova.pop_front();
    prova.pop_back();
    /*const char *ch_icao;
    ch_icao = icao.toStdString().data();*/
    QString I(prova[0]);
    QString C(prova[1]);
    QString A(prova[2]);
    //ch_icao = "";
    airportPath = airportsDir + "/" + I + "/" + C + "/" + A;
}

QString Airport::getAirportDirPath()
{
    return airportPath;
}
