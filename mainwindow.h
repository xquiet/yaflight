#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResource>
#include <QMessageBox>
#include <QProcess>
#include <QByteArray>
#include <QStringList>
#include <QDir>
#include <QHash>
#include <QHashIterator>
#include <QResizeEvent>

#include "fgenvironment.h"
#include "dlgaircraftdetails.h"
#include "imagepreview.h"

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
    void on_btnExit_clicked();

    void on_btnAbout_clicked();

    void on_pbtLaunch_clicked();

    void readAircrafts();

    void procReadAircraftsFinished(int, QProcess::ExitStatus);

    void on_cboAircrafts_currentIndexChanged(const QString &arg1);

    void on_btnAircraftInfo_clicked();

    void exLogExpanded();

    void exMapExpanded();

    void exLogUnexpanded();

    void exMapUnexpanded();


protected:
    //void resizeEvent(QResizeEvent *event);

private:
    Ui::MainWindow *ui;
    QProcess *procFGFS;
    int _mainWindowMaxHeight, _mainWindowMinHeight;
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
