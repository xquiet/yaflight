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
//#include <QStringListModel>
#include <QStandardItemModel>
#include <QWebFrame>

#include "fgenvironment.h"
#include "dlgaircraftdetails.h"
#include "imagepreview.h"
#include "aircraft.h"
#include "airport.h"
#include "airportidx.h"
#include "settings.h"

#include "apt_dat.h"

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

    void expOptsUnexpanded();

    void expOptsExpanded();

    void on_pbtSaveConf_clicked();

    void on_pbtLoadConf_clicked();

    void on_btnRefreshAirportList_clicked();

    void on_ckbFilterInstalled_toggled(bool checked);

    void on_tbvAirports_clicked(const QModelIndex &index);

    void on_ckbSound_toggled(bool checked);

    void on_ckbClouds_toggled(bool checked);

    void on_ckbGameMode_toggled(bool checked);

    void on_ckbFullScreen_toggled(bool checked);

    void on_ckbFog_toggled(bool checked);

    void on_rdbUnitMeters_toggled(bool checked);

    void on_ckbLockFuel_toggled(bool checked);

    void on_ckbLockTime_toggled(bool checked);

    void on_ckbRandomObjects_toggled(bool checked);

    void on_ckbAIModels_toggled(bool checked);

    void on_ckbAutoCoordination_toggled(bool checked);

    void on_ckbPanel_toggled(bool checked);

    void on_ckbHorizonEffect_toggled(bool checked);

    void on_ckbSkyBlending_toggled(bool checked);

    void on_ckbTextures_toggled(bool checked);

    void on_ckbDistanceAttenuation_toggled(bool checked);

    void on_ckbWind_toggled(bool checked);

    void on_ckbHudAntialias_toggled(bool checked);

    void on_rdbHud2D_toggled(bool checked);

    void on_rdbHud3D_toggled(bool checked);

    void on_ckbEnhancedLighting_toggled(bool checked);

    void on_ckbSpecularReflections_toggled(bool checked);

    void on_hzsTurbulence_valueChanged(int value);

    void on_cboWindowGeometries_currentIndexChanged(const QString &arg1);

    void on_cboFailures_currentIndexChanged(const QString &arg1);

    void on_cboDayTime_currentIndexChanged(const QString &arg1);

    void on_cboSeason_currentIndexChanged(const QString &arg1);

    void on_tbvAirports_doubleClicked(const QModelIndex &index);

    void on_dialHeading_valueChanged(int value);

    void on_btnGoToMap_clicked();

    void on_btnAboutQt_clicked();

    void on_cboRunway_currentIndexChanged(const QString &arg1);

    void on_btnRunwayInfo_clicked();

protected:
    //void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::MainWindow *ui;
    QProcess *procFGFS;
    int _mainWindowMaxHeight, _mainWindowMinHeight;
    QStringList aircrafts;
    // hash key   --> aircraft name (unique)
    // hash value --> aircraft dir  (multiple)
    QHash<QString,QString> hashOfAircrafts;
    QHash<QString, QString> getListOfAircrafts();
    void refreshListOfAircrafts();
    void drawThumbnail(QString dir);
    QStringList collectLaunchSettings();
    QHash<QString, QStringList> collect_all_airports();
    void setup_airport_list();
    Settings *curr_settings;

    Aircraft *ac;

    QList<Runway *> ap_runways;
    Runway *currentRunway;

    FGEnvironment *fgenv;

    //void loadSettings(FGEnvironment *fgenv);
    void loadSettings(bool appStart=false);
    bool saveSettings();

    QPoint dragPosition; // used to move the frameless window

    bool just_started;

    void update_latlonhead(QString lat, QString lon, QString heading);
    void adjust_heading_value(int head);
    int convert_dialhead_to_azimuth(int value);
};

#endif // MAINWINDOW_H
