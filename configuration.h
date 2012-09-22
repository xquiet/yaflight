#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QStringList>
#include <QSettings>
#include <QHash>
#include <QFile>
#include <QTextStream>

class configuration
{
public:
    configuration(QString conf);

    QStringList get();
    QString get(QString key, QString param);
    void set(QStringList);
    void set(QString key, QString param, QString value);

    QHash<QString, QHash<QString, QString> > confSettings;

    bool create(QString path);
    bool exists();
    void parseFile();
private:
    QHash<QString, QString> parseSection(QString section);
    QString confFilePath;
};

#endif // CONFIGURATION_H