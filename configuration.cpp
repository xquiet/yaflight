/**
    Copyright (C) 2012  Matteo Pasotti <matteo.pasotti@gmail.com>

    This file is part of yaflight - Cross platform YaFlight

    yaflight is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    yaflight is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#include "configuration.h"

Configuration::Configuration(QString conf)
{
    confFilePath = conf.trimmed();
}

bool Configuration::exists()
{
    return QFile::exists(confFilePath);
}

bool Configuration::create(QString path, bool ifApp)
{
    QString dirPath = QFileInfo(path).absoluteDir().absolutePath();
    QDir dir(dirPath);
    if (!dir.exists())
    {
        dir.mkpath(dir.absolutePath());
    }
    QFile file(path);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream in(&file);
        if(!ifApp)
        {
            in << "["+QString(MAINSETTINGS)+"]"
               << "\n" << "["+QString(ADVSETTINGS)+"]" << "\n"
               << "["+QString(SCENERIES)+"]";
        }else{
            in << "["+QString(APPSETTINGS)+"]" << "\n";
        }
        file.close();
        return true;
    }
    return false;
}

bool Configuration::store()
{
    QSettings settings(confFilePath, QSettings::IniFormat);
    QStringList sections;
    sections << MAINSETTINGS << ADVSETTINGS << APPSETTINGS << SCENERIES;
    foreach(QString section, sections)
    {
        settings.beginGroup(section);
        QHashIterator<QString,QString> i(confSettings[section]);
        while(i.hasNext())
        {
            i.next();
            if(i.value().trimmed().compare("")!=0)
            {
                qDebug("%s --> %s", i.key().toStdString().data(), i.value().toStdString().data());
                settings.setValue(i.key(),i.value());
            }
            else
            {
                qDebug("Dropping %s",i.key().toStdString().data());
                settings.remove(i.key());
            }
        }
        settings.endGroup();
    }
    return true;
}

void Configuration::parseFile()
{
    QStringList sections;
    sections << MAINSETTINGS << ADVSETTINGS << APPSETTINGS << SCENERIES;
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
    confSettings[key][param] = value;
}
