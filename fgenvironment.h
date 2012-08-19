#ifndef FGENVIRONMENT_H
#define FGENVIRONMENT_H

#include <QString>

class FGEnvironment
{
private:
    QString fgRootPath;
public:
    FGEnvironment();
    void setRootPath(QString path);
    QString getRootPath();
    QString getAircraftDir();
};

#endif // FGENVIRONMENT_H
