/**
    Copyright (C) 2012-2013  Matteo Pasotti <matteo.pasotti@gmail.com>

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
#include <QStringListModel>
#include <QStandardItemModel>
#include <QWebFrame>
#include <QFileDialog>
#include <QTimer>
#include <QTranslator>
#include <QtNetwork/QHttp>

#include "logger.h"
#include "fgenvironment.h"
#include "aircraftdetailsdialog.h"
#include "dialogappsettings.h"
#include "imagepreview.h"
#include "aircraft.h"
#include "airport.h"
#include "surfacecode.h"
#include "shouldercode.h"
#include "runwaydetailsdialog.h"
#include "mpmdialog.h"
#include "dialogmpdetails.h"
#include "settings.h"
#include "appsettings.h"
#include "helpdialog.h"

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
    void closeEvent(QCloseEvent *event);

    void on_btnExit_clicked();

    void on_pbtLaunch_clicked();

    void readAircrafts();

    void read_ts_output();

    void procReadAircraftsFinished(int, QProcess::ExitStatus);

    void proc_ts_finished(int, QProcess::ExitStatus);

    void on_cboAircrafts_currentIndexChanged(const QString &arg1);

    void on_btnAircraftInfo_clicked();

    void expOptsUnexpanded();

    void expOptsExpanded();

    void on_pbtSaveConf_clicked();

    void on_pbtLoadConf_clicked();

    void on_btnRefreshAirportList_clicked();

    void on_ckbFilterInstalled_toggled(bool checked);

    void on_tbvAirports_clicked(const QModelIndex &index);

    void on_hzsTurbulence_valueChanged(int value);

    void on_cboWindowGeometries_currentIndexChanged(const QString &arg1);

    void on_cboFailures_currentIndexChanged(const QString &arg1);

    void on_cboDayTime_currentIndexChanged(const QString &arg1);

    void on_cboSeason_currentIndexChanged(const QString &arg1);

    void on_dialHeading_valueChanged(int value);

    void on_btnGoToMap_clicked();

    void on_btnAboutQt_clicked();

    void on_cboRunway_currentIndexChanged(const QString &arg1);

    void on_btnRunwayInfo_clicked();

    void on_btnAdd_clicked();

    void on_btnDel_clicked();

    void hndl_tmr_procfgfs();

    void hndl_tmr_procts();

    void on_ckbInAir_toggled(bool checked);

    void on_cboControlMode_currentIndexChanged(const QString &arg1);

    void on_btnRecreateAiportsIndex_clicked();

    void on_pbtSearchAirport_clicked();

    void verify_updates();

    void on_pbtSpecifyMPPorts_clicked();

    void on_spboxHeading_valueChanged(QString value);

    void on_pbtTerraSyncStartStop_clicked();

    void on_pbtOpenMPMap_clicked();

    void on_pbtMPHostnameContextualHelp_clicked();

    void on_pbtWeatherContextualHelp_clicked();

    void on_pbtFindNext_clicked();

protected:
    //void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void setup_about_box();

private:
    // ----- app -----
    Ui::MainWindow *ui;
    QPoint dragPosition; // used to move the frameless window
    int _mainWindowMaxHeight, _mainWindowMinHeight;
    int posX, posY;
    void toggleLoadingBarVisible();
    void setup_default_paths();
    Logger *log;
    QHttp *http;
    void check_updates();
    //void setup_mpmap_viewer();
    MPMDialog *mpmdiag;
    HelpDialog *hpdiag;
    void showHelp(QString url);
    QList<QStandardItem *> searchFoundAirports;
    int searchFoundAirportsIndex;
    //void follow_pilot_on_mpmap();

    // ----- processes -----
    QProcess *procFGFS;
    QProcess *procTerraSync;
    bool proc_fgfs_is_running;
    bool proc_ts_is_running;
    QTimer *tmrProcFGFS;
    QTimer *tmrProcTS;
    void startTerraSync();
    void stopTerraSync();

    // ----- aircrafts -----
    QStringList aircrafts;
    // hash key   --> aircraft name (unique)
    // hash value --> aircraft dir  (multiple)
    QHash<QString,QString> hashOfAircrafts;
    QHash<QString, QString> getListOfAircrafts();
    void refreshListOfAircrafts();
    void drawThumbnail(QString dir);
    Aircraft *ac;

    // ----- airports -----
    QHash<QString, QStringList> collect_all_airports();
    QStringList collect_all_airports_dir();
    void setup_airport_list(bool forceAptIdxRebuild=false);
    QList<Runway *> ap_runways;
    Runway *currentRunway;
    QModelIndex lastAirportIndex;

    // ----- settings -----
    Settings *curr_settings;
    appsettings *appsett;
    QStringList collectLaunchSettings();
    void loadSettings(QString confFile, bool appStart=false);
    bool saveSettings(QString confFile);
    void add_scenery_path(QString sceneryPath);
    QString lastResolutionSelected;
    QString lastFailureSelected;
    QString lastSeasonSelected;
    QString lastDayTimeSelected;
    QString lastAirport;
    QString lastLongitude;
    QString lastLatitude;
    QString lastHeading;
    QString lastControlModeSelected;
    QString lastAircraft;
    QString lastPortIn;
    QString lastPortOut;
    float lastTurbulence;

    FGEnvironment *fgenv;
    void check_mp_ports_values();

    // ----- flags -----
    bool just_started;

    // ----- web service -----
    void update_latlonhead(QString lat, QString lon, QString heading);
    double convert_dialhead_to_azimuth(double head);
    void place_aircraft_on_map_reading_table();
    void place_aircraft_on_map_reading_settings();
    void center_mpmap_at_coords();
};

#endif // MAINWINDOW_H
