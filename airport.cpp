#include "airport.h"

Airport::Airport(QString airportsDir, QString icao, QString aptdatgzipped, QString destpath)
{
    // build airport directory path
    const char *ch_icao;
    ch_icao = icao.toStdString().c_str();
    QString I(ch_icao[0]);
    QString C(ch_icao[1]);
    QString A(ch_icao[2]);

    airportPath = airportsDir + "/" + I + "/" + C + "/" + A;

    aptdat = new APT_dat(aptdatgzipped,destpath);
    aptdat->parse_apt_dat();
}

QString Airport::getAirportDirPath()
{
    return airportPath;
}
