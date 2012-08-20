#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QProcess>
#include <QByteArray>
#include <QStringList>
#include <QPainter>
#include <QPixmap>
#include <QDir>
#include <QHash>
#include <QHashIterator>
#include <math.h>

#include "fgenvironment.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pbtExit_clicked();

    void on_pbtAbout_clicked();

    void on_pbtLaunch_clicked();

    void readAircrafts();

    void procReadAircraftsFinished(int, QProcess::ExitStatus);

    void on_cboAircrafts_currentIndexChanged(const QString &arg1);

    void on_btnAircraftInfo_clicked();

private:
    Ui::MainWindow *ui;
    QProcess *procFGFS;
    QStringList aircrafts;
    //QStringList getListOfAircrafts();
    //QStringList listOfAircrafts;
    // hash key   --> aircraft name (unique)
    // hash value --> aircraft dir  (multiple)
    QHash<QString,QString> hashOfAircrafts;
    QHash<QString, QString> getListOfAircrafts();
    void refreshListOfAircrafts();
    void drawThumbnail(QString dir);
};

#endif // MAINWINDOW_H