#include "configuration.h"

Configuration::Configuration(QString conf)
{
    confFilePath = conf.trimmed();
}

bool Configuration::exists()
{
    return QFile::exists(confFilePath);
}

bool Configuration::create(QString path)
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

void Configuration::parseFile()
{
    QStringList sections;
    sections << "main" << "advanced" << "app" << "sceneries";
    confSettings.begin();
    foreach (QString section, sections)
        confSettings.insert(section,parseSection(section));
    confSettings.end();
}

QHash<QString, QString> Configuration::parseSection(QString section)
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

QString Configuration::get(QString key, QString param)
{
    return confSettings.value(key).value(param);
}

QHash<QString, QString> Configuration::get(QString key)
{
    return confSettings.value(key);
}

void Configuration::set(QString key, QString param, QString value)
{
    //confSettings.value(key).value(param) = value;
}
