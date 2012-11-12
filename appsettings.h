#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QString>

#include "configuration.h"

#define APTVIEW     "APTVIEW"
#define FGFSBIN     "FGFSBIN"
#define FGDATADIR   "FGDATADIR"

class appsettings
{
public:
    appsettings(QString);

    bool isEmpty();
    bool storeData();
    QString getUpdatesHost();
    QString getUpdatesScript();

    // app - GET
    QString getAirportListFiltered();
    QString get_fgfs_bin_path();
    QString getFGDataPath();

    // app - SET
    void setAirportListFiltered(QString val);
    void set_fgfs_bin_path(QString val);
    void setFGDataPath(QString val);

private:
    Configuration *conf;
    bool conf_empty;
    static QString updatesHost;
    static QString updatesScript;
};

#endif // APPSETTINGS_H
