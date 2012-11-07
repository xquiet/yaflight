#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

class Logger
{
public:
    Logger(QString path);
    bool Log(QString event_type, QString msg);
    static const QString ET_WARN;
    static const QString ET_INFO;
    static const QString ET_ERR;

private:
    QString logFilePath;
};

#endif // LOGGER_H
