#include "configuration.h"

configuration::configuration(QString conf)
{
    confFilePath = conf.trimmed();
}

bool configuration::exists()
{
    return QFile::exists(confFilePath);
}

bool configuration::create(QString path)
{
    QFile file(path);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream in(&file);
        in << "[main]" << "\n" << "[advanced]" << "\n" << "[app]";
        file.close();
        return true;
    }
    return false;
}

void configuration::parseFile()
{
    QStringList sections;
    sections << "main" << "advanced" << "app" << "sceneries";
    confSettings.begin();
    foreach (QString section, sections)
        confSettings.insert(section,parseSection(section));
    confSettings.end();
    //---------------------------------------------
    /*settings.beginGroup("advanced");
    childKeys = new QStringList();
    childKeys = settings.childKeys();
    confSettings.begin();
    QHash<QString, QString> advHash;
    advHash.begin();
    foreach (const QString &childKey, childKeys)
         advHash.insert(childKey, settings.value(childKey).toString());
    confSettings.insert("advanced",advHash);
    settings.endGroup();
    //---------------------------------------------
    settings.beginGroup("app");
    childKeys = new QStringList();
    childKeys = settings.childKeys();
    confSettings.begin();
    QHash<QString, QString> appHash;
    appHash.begin();
    foreach (const QString &childKey, childKeys)
         appHash.insert(childKey, settings.value(childKey).toString());
    confSettings.insert("app",appHash);
    settings.endGroup();*/
}

QHash<QString, QString> configuration::parseSection(QString section)
{
    QSettings settings(confFilePath, QSettings::IniFormat);
    settings.beginGroup(section);
    const QStringList childKeys = settings.childKeys();
    QHash<QString, QString> dummyHash;
    dummyHash.begin();
    foreach (const QString &childKey, childKeys)
         dummyHash.insert(childKey, settings.value(childKey).toString());
    settings.endGroup();
    return dummyHash;
}

QString configuration::get(QString key, QString param)
{
    return confSettings.value(key).value(param);
}
