#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QStringList>
#include <QSettings>
#include <QHash>
#include <QFile>
#include <QDir>
#include <QTextStream>


#define MAINSETTINGS "MAINSETTINGS"
#define ADVSETTINGS "ADVSETTINGS"
#define APPSETTINGS "APPSETTINGS"
#define SCENERIES "SCENERIES"

class Configuration
{
public:
    Configuration(QString conf);

    QStringList get();
    QHash<QString, QString> get(QString key);
    QString get(QString key, QString param);
    void set(QStringList);
    void set(QString key, QString param, QString value);

    QHash<QString, QHash<QString, QString> > confSettings;

    bool create(QString path);
    bool exists();
    void parseFile();
    bool store();
private:
    QHash<QString, QString> parseSection(QString section);
    QString confFilePath;
};

#endif // CONFIGURATION_H
