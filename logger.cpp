#include "logger.h"

const QString Logger::ET_INFO = "INFO";
const QString Logger::ET_WARN = "WARN";
const QString Logger::ET_ERR =  "ERR";

Logger::Logger(QString path)
{
    logFilePath = path.trimmed();
}

bool Logger::Log(QString event_type, QString msg)
{
    QFile logfile(logFilePath);
    if(logfile.open(QIODevice::Append|QIODevice::Text))
    {

        QTextStream out(&logfile);
        out << "[" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
            << "] " << event_type << "  " << msg.trimmed() << endl;
        logfile.close();
        return true;
    }
    else
    {
        qDebug("Can't open file %s", logFilePath.toStdString().data());
        return false;
    }
}
