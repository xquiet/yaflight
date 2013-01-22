#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator yaflightTranslator;
    #ifdef Q_OS_UNIX
        #ifdef Q_OS_MACX
        yaflightTranslator.load(QLocale::system().name(),
                                a.applicationDirPath() + "/../Resources/languages");
        #else
        yaflightTranslator.load(QLocale::system().name(),
                                TRANSDIR);
        #endif
    #elif defined Q_OS_WIN32
    yaflightTranslator.load(QLocale::system().name(),
                            a.applicationDirPath() + "/languages");
    #endif
    a.installTranslator(&yaflightTranslator);
    MainWindow w;
    w.show();
    
    return a.exec();
}
