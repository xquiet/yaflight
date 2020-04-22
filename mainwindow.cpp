#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    //QMainWindow(parent,Qt::FramelessWindowHint),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
#ifdef Q_OS_MACX
    this->setAttribute(Qt::WA_MacBrushedMetal);
    ui->pbtTerraSyncStartStop->setEnabled(false);
#endif
#ifdef Q_OS_WIN
    ui->pbtInstallModel->hide();
#endif

    QApplication::setApplicationName("YaFlight");
    QApplication::setApplicationVersion(STRVERSION);

    ui->lblLoading->setVisible(false);
    ui->pbarAptCache->setVisible(false);

    just_started = true;
    proc_fgfs_is_running = false;
    proc_ts_is_running = false;

    posX = this->x();
    posY = this->y();

    fgenv = new FGEnvironment();

    /**
      * steps to start loading/creating initial configuration
      1) does YFHome exists?
      2) if YFHome exists -> does appconf.ini exists?
      3) if YFHome doesn't exists must start automatic configuration
      4) if automatic configuration fails -> must start manual configuration
      */


    if (!QFile::exists(fgenv->getYFHome()))
    {
        // new setup
        QDir tmppath(QDir::homePath());
        if(!tmppath.mkpath(fgenv->getYFHome()))
        {
            QMessageBox msgbox(QMessageBox::Critical, tr("Errore critico"), tr("Cannot create YF Home Directory"),QMessageBox::Ok, this);
            msgbox.exec();
            QApplication::exit(1);
        }
    }
    if(!QFile::exists(fgenv->getYFHome()+"/appconf.ini"))
    {
        // directory already exists but not the ini file
        // launch automatic setup procedure
        if(!fgenv->start()) // autodetect = true
        {
            QMessageBox msgbox(QMessageBox::Warning,tr("Warning"),tr("AutoDetection failed: manual configuration will be started"),QMessageBox::Ok,this);
            msgbox.exec();
            DialogAppSettings appSettingsDlg(this);
            if(appSettingsDlg.exec() == QMessageBox::Cancel)
            {
                QMessageBox msgbox(QMessageBox::Warning,tr("Error"),tr("Manual configuration aborted"),QMessageBox::Ok,this);
                msgbox.exec();
                QApplication::exit(1);
            }
        }
        appsett = new appsettings(fgenv->getYFHome()+"/appconf.ini");
        appsett->setFGDataPath(fgenv->getRootPath());
        appsett->set_fgfs_bin_path(fgenv->getFgfsBinPath());
        appsett->storeData();
        delete appsett;
    }

    // reload appsett (not a proper solution)
    appsett = new appsettings(fgenv->getYFHome()+"/appconf.ini");
    if(appsett->isEmpty())
    {
        QMessageBox msgbox(QMessageBox::Warning,tr("Warning"),tr("appconf.ini exists but it's empty: running manual configuration in a moment"),QMessageBox::Ok,this);
        msgbox.exec();
        DialogAppSettings appSettingsDlg(this);
        if(appSettingsDlg.exec() == QMessageBox::Cancel)
        {
            QMessageBox msgbox(QMessageBox::Warning,tr("Error"),tr("Manual configuration aborted"),QMessageBox::Ok,this);
            msgbox.exec();
            QApplication::exit(1);
        }
        // reload app conf and fgenvironment
        delete appsett;
        appsett = new appsettings(fgenv->getYFHome()+"/appconf.ini");
    }

    // we're ready to start, let's setup FGEnvironment instance

    fgenv->setRootPath(appsett->getFGDataPath());
    fgenv->setFgFsBinary(appsett->get_fgfs_bin_path());
    fgenv->start(false);

    log = new Logger(fgenv->getYFHome()+"/yf.log");

    connect(ui->expanderOpts,SIGNAL(expanded()),this,SLOT(expOptsExpanded()));
    connect(ui->expanderOpts,SIGNAL(unexpanded()),this,SLOT(expOptsUnexpanded()));

    // cuteexpander
    ui->expanderOpts->setText("More...");
    ui->expanderOpts->unexpand();

    ui->tbxOpts->setGeometry(0,2,ui->tbxOpts->width(),ui->tbxOpts->height());
    ui->tabOpts->setGeometry(0,26,ui->tabOpts->width(),ui->tabOpts->height());

    // reload and populate hashOfAircrafts for
    // a later use
    refreshListOfAircrafts();

    // windows geometries
    QStringList windowGeometries;
    windowGeometries << "1280x1024" << "1024x768" << "800x600" << "640x480" <<
                        "1024x576" << "1152x648" << "1280x720" << "1366x780" <<
                        "1600x900" << "1920x1080" << "2560x1440" << "3840x2160";
    //windowGeometries.sort();
    ui->cboWindowGeometries->addItems(windowGeometries);

    // control mode
    QStringList controlModes;
    controlModes << "keyboard"
                 << "joystick"
                 << "mouse";
    ui->cboControlMode->addItems(controlModes);

    QStringList fdms;
    fdms << ""
         << "jsb"
         << "larcsim"
         << "yasim"
         << "magic"
         << "baloon"
         << "ada"
         << "external"
         << "null";
    ui->cboFDM->addItems(fdms);

    // aircraft failures
    QStringList failures;
    failures << "none" << "pitot"
             << "static" << "vacuum"
             << "electrical";

    ui->cboFailures->addItems(failures);

    // day time
    QStringList daytime;
    daytime << "none" << "dawn"
            << "dusk" << "noon"
            << "midnight";

    ui->cboDayTime->addItems(daytime);

    // seasons
    QStringList seasons;
    seasons << "none" << "summer"
            << "winter";

    ui->cboSeason->addItems(seasons);

    populate_combo_aircrafts();

    populate_scenarios();

    populate_yaflight_theme_list();

    ui->txaLog->append(tr("OS: ") + fgenv->getOS());
    ui->txaLog->append(tr("FG version: ") + fgenv->getFGVersion());
    ui->txaLog->append(tr("Default FGROOT: ") + fgenv->getRootPath());
    ui->txaLog->append(tr("Default FGSCEN: ") + fgenv->getDefaultScenery());
    ui->txaLog->append(tr("Aircraft dir: ") + fgenv->getAircraftsDir());

    setup_default_paths();

    ui->lblTerraSyncStatus->setToolTip(tr("N/A"));

    // initialize aptdat and search for cache file
    // if it doesn't exists,
    // the APT_dat thread is launched to create it
    aptdat.initialize(fgenv->getAPTSource(), fgenv->getYFHome());
    if(!aptdat.aptcache_exists())
    {
        // connect signals emitted from the thread to local handlers
        connect(&aptdat, SIGNAL(sgnDecompressionCompleted()),
                this, SLOT(completedAptCache()));
        connect(&aptdat, SIGNAL(sgnCacheProgress(int, int)),
                this, SLOT(updateCacheProgress(int, int)));

        ui->btnRecreateAiportsIndex->setEnabled(false);
        ui->btnRefreshAirportList->setEnabled(false);
        ui->pbarAptCache->show();
        if(!aptdat.create_cache(collect_all_airports_dir()))
        {
            QMessageBox msgbox(QMessageBox::Critical,tr("Error"),tr("Can't create airport index cache\nCheck you permissions"),QMessageBox::Ok);
            msgbox.exec();
            return;
        }
    }
    else
    {
        setup_airport_list();
    }

    setup_about_box();

    check_updates();

    // disable some controls if no more available in later versions
    if(fgenv->getFGVersion().split(".")[0].toInt()>=3)
    {
        ui->ckbSkyBlending->setVisible(false);
        ui->ckbGameMode->setVisible(false);
    }
    
    // bad solution!
    // start timer to retrieve current aircraft coords
    tmrProcCoords = new QTimer();
    connect(tmrProcCoords, SIGNAL(timeout()),this,SLOT(hndl_tmr_update_map_with_plane()));
    tmrProcCoords->start(500);

    // nice tip from
    // http://www.developer.nokia.com/Community/Discussion/showthread.php?225405-How-to-detect-mouse-press-move-release-in-Qwebview-without-affect-linkcliced
    ui->webView->installEventFilter(this);

    hpdiag = NULL;
    mpmdiag = NULL;
    currentRunway = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->webView)
    {
        if(event->type() == QEvent::MouseButtonRelease)
        {
            change_selected_coords();
        }
        return ui->webView->eventFilter(obj,event);
    }
    else
        return MainWindow::eventFilter(obj,event);
}

void MainWindow::populate_combo_aircrafts()
{
    QStringList listOfAircrafts = hashOfAircrafts.keys();
    listOfAircrafts.sort();
    ui->cboAircrafts->clear();
    ui->cboAircrafts->addItems(listOfAircrafts);
}

void MainWindow::populate_scenarios()
{
    QStringList scenarios = fgenv->getScenarios();
    QStringListModel *lstviewmodel = new QStringListModel(scenarios);
    ui->lstvScenarios->setModel(lstviewmodel);
    ui->lstvScenarios->setSelectionMode(QListView::MultiSelection);
}

void MainWindow::check_updates()
{
    QNetworkAccessManager *mgr;
    QNetworkRequest req;
    mgr = new QNetworkAccessManager();
    QString myurl = appsett->getUpdatesHost()+appsett->getUpdatesScript();
    req.setUrl(QUrl(myurl));
    //req.setRawHeader("User-Agent",userAgent.toStdString().data());
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(verify_updates(QNetworkReply*)));
    mgr->get(req);
}

void MainWindow::verify_updates(QNetworkReply *reply)
{

    QString sDataReturned = QString(reply->readAll());
    //qDebug("%s",sDataReturned.toStdString().data());
    QStringList items = sDataReturned.split("\n");
    foreach(QString item, items)
    {
        QStringList couple = item.split(":");
        if(couple[0].trimmed().compare("last-yaflight-vers")==0)
        {
            QStringList veritems = couple[1].split(".");
            int major = veritems[0].toInt();
            int minor = veritems[1].toInt();
	    int patch = veritems[2].toInt();
            if(major > MAX_VERSION)
            {
                ui->lblLatestVersionAvailable->setText(tr("Version")+ " " + veritems.join(".") + " " + tr("is available"));
            }
            else
            {
                if(minor > MIN_VERSION)
                    ui->lblLatestVersionAvailable->setText(tr("Version")+ " " + veritems.join(".") + " " + tr("is available"));
                else
		{
		    if(patch > PATCH_VERSION)
		      ui->lblLatestVersionAvailable->setText(tr("Version")+ " " + veritems.join(".") + " " + tr("is available"));
		    else
		      ui->lblLatestVersionAvailable->setText(tr("Your yaflight is up to date"));
		}
            }
        }
    }
    reply->close();
}

void MainWindow::setup_default_paths()
{
    ui->lblDefaultScenery->setText(fgenv->getDefaultScenery());
    ui->lblYFScenery->setText(fgenv->getYFScenery());
}

void MainWindow::setup_about_box()
{
    ui->lblAppName->setText(QApplication::applicationName());
    ui->lblAppVersion->setText(tr("Version: ") + QApplication::applicationVersion());
    ui->lblAppCopyright->setText("&copy; 2012-2020 by Matteo Pasotti");
    ui->lblAppCopyright->setTextFormat(Qt::RichText);
}

void MainWindow::refreshListOfAircrafts()
{
    hashOfAircrafts = getListOfAircrafts();
}

QHash<QString, QString> MainWindow::getListOfAircrafts()
{
    int i = 0, j = 0;
    QDir *subdir;
    QHash<QString,QString> result;
    QDir aircraftsDir(fgenv->getAircraftsDir());
    if(!aircraftsDir.exists())
    {
        qDebug("Directory %s doesn't exists",aircraftsDir.absolutePath().toStdString().data());
        return result;
    }
    QStringList listOfAircrafts = aircraftsDir.entryList(QDir::Dirs,QDir::Name);
    result.begin();
    for(i=0;i<listOfAircrafts.count();i++)
    {
        subdir = new QDir(aircraftsDir.absolutePath()+"/"+listOfAircrafts.value(i));
        QStringList xml = subdir->entryList(QStringList()<<"*-set.xml",QDir::Files,QDir::Name);
        for(j=0;j<xml.count();j++)
        {
            result.insert(xml.value(j).replace("-set.xml","",Qt::CaseSensitive),listOfAircrafts.value(i));
        }
        delete subdir;
    }
    result.end();
    return result;
}

void MainWindow::on_pbtLaunch_clicked()
{
    if(!proc_fgfs_is_running)
    {
        QStringList params = collectLaunchSettings();
        QStringList env = QProcess::systemEnvironment();

        bool use_terra_sync = false;

        if(just_started)
        {
            use_terra_sync = ui->ckbTerraSync->isChecked();
        }
        else
        {
            if(curr_settings->getTerraSync().compare(SET_TRUE)==0)
                use_terra_sync = true;
        }
        if(use_terra_sync)
        {
            if(!proc_ts_is_running)
            {
#ifdef Q_OS_MACX
                params << "--enable-terrasync";
                params << "--prop:/sim/terrasync/refresh-display=true";
                ui->txaTSLog->append("Running on Mac OS X");
                log->Log(Logger::ET_INFO, "BuiltIn support for scenery download enabled");
                proc_ts_is_running = true;
#else
                startTerraSync();
                ui->pbtTerraSyncStartStop->setText(tr("Stop"));
#endif
            }
        }

        procFGFS = new QProcess();
        procFGFS->setProcessChannelMode(QProcess::MergedChannels);
        procFGFS->setReadChannel(QProcess::StandardOutput);
        procFGFS->setEnvironment(env);

        QString fgfsBinary;

        fgfsBinary = fgenv->getFgfsBinPath();

#ifndef Q_OS_MACX
        if(proc_ts_is_running)
        {
            params << "--atlas=socket,out,1,localhost,5500,udp";
        }
#endif

        log->Log(Logger::ET_INFO, fgfsBinary + " " + params.join(" "));

        procFGFS->start(fgfsBinary, params, QProcess::ReadOnly);

        ui->txaLog->append(tr("Launching..."));
        //ui->txaLog->append(fgenv->getFgfsBinPath()+" "+params.join(" "));
        qDebug("%s",(fgenv->getFgfsBinPath()+" "+params.join(" ")).toStdString().data());
        ui->txaLog->append(Logger::ET_INFO + ": "+ tr("Simulation started"));
        connect(procFGFS,SIGNAL(readyRead()),this,SLOT(readAircrafts()));
        connect(procFGFS,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(procReadAircraftsFinished(int, QProcess::ExitStatus)));

        tmrProcFGFS = new QTimer();
        connect(tmrProcFGFS, SIGNAL(timeout()),this,SLOT(hndl_tmr_procfgfs()));
        tmrProcFGFS->start(350);

        procFGFS->closeWriteChannel();
        ui->txaLog->append(Logger::ET_INFO + ": "+ tr("TerraSync started"));
        if((procFGFS->state() == QProcess::Running)||
                (procFGFS->state() == QProcess::Starting))
            proc_fgfs_is_running = true;
    }
    else
    {
        procFGFS->kill();
        procFGFS->close();
        proc_fgfs_is_running = false;
    }
}

void MainWindow::readAircrafts()
{
    QByteArray bytes = procFGFS->readAll();
    QString strLines = QString(bytes);
    ui->txaLog->append(strLines);
}

void MainWindow::read_ts_output()
{
    QByteArray bytes = procTerraSync->readAll();
    QString strLines = QString(bytes);
    //qDebug("%s", strLines.toStdString().data());
    ui->txaTSLog->append(strLines);
}

void MainWindow::procReadAircraftsFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    ui->txaLog->append(tr("Simulation complete"));
    QString message = QString(tr("process exited with code: %1, status: %2\n")
        .arg(exitCode)
        .arg(exitStatus == QProcess::NormalExit ? "QProcess::NormalExit" : "QProcess::CrashExit"));
    //qDebug("%s",message.toStdString().data());
    ui->txaLog->append(message);
}

void MainWindow::proc_ts_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    ui->txaLog->append(Logger::ET_INFO + ": "+ tr("TerraSync stopped"));
    QString message = QString(tr("process exited with code: %1, status: %2\n")
        .arg(exitCode)
        .arg(exitStatus == QProcess::NormalExit ? "QProcess::NormalExit" : "QProcess::CrashExit"));
    //qDebug("%s",message.toStdString().data());
    ui->txaLog->append(message);
}

void MainWindow::on_cboAircrafts_currentIndexChanged(const QString &arg1)
{
    if(arg1.trimmed().compare("")==0)
        return;
    drawThumbnail(hashOfAircrafts.value(arg1.trimmed()));
    if(!just_started)
        lastAircraft = arg1;
}

void MainWindow::drawThumbnail(QString dir)
{
    QString thumbFilePath = fgenv->getAircraftsDir()+"/"+dir+"/thumbnail.jpg";
    if(!QFile::exists(thumbFilePath))
    {
        ac = new Aircraft(ui->cboAircrafts->currentText(), fgenv->getAircraftsDir(),
                    hashOfAircrafts.value(ui->cboAircrafts->currentText()));

        thumbFilePath = fgenv->getRootPath()+"/"+ac->getSplashTexture();
    }
    ImagePreview iprvw(
                thumbFilePath,
                ui->lblAircraftPreview->width(),
                ui->lblAircraftPreview->height()
                );
    ui->lblAircraftPreview->setPixmap(iprvw.getPixmap());
}

void MainWindow::on_btnAircraftInfo_clicked()
{
    QStringList details = ac->getAircraftDetails(
                ui->cboAircrafts->currentText(),
                fgenv->getAircraftsDir(),
                hashOfAircrafts.value(ui->cboAircrafts->currentText())
                );

    AircraftDetailsDialog dlg(details, fgenv, this);
    dlg.setModal(true);
    dlg.exec();
}

void MainWindow::on_btnExit_clicked()
{
    this->close();
}

/*void MainWindow::resizeEvent(QResizeEvent *event)
{
    ui->expanderMap->setGeometry(ui->expanderMap->x(),
                                 ui->expanderLog->y()+ui->expanderLog->height()+2,
                                 ui->expanderMap->width(),
                                 ui->expanderMap->height());
}*/

void MainWindow::expOptsExpanded()
{
    this->setGeometry(posX,posY,width(),ui->expanderOpts->height()+ui->expanderOpts->height()+2);
}

void MainWindow::expOptsUnexpanded()
{
    this->setGeometry(posX,posY,width(),ui->expanderOpts->height()+ui->expanderOpts->height()+2);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        posX = this->x();
        posY = this->y();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
 {
     if (event->buttons() & Qt::LeftButton) {
         move(event->globalPos() - dragPosition);
         posX = this->x();
         posY = this->y();
         event->accept();
     }
 }

QStringList MainWindow::collectLaunchSettings()
{
    QString fgScenery = fgenv->getDefaultScenery() + ":" + fgenv->getYFScenery();
    QString fgVersion = fgenv->getFGVersion();
    QStringListModel *lstviewmodel = (QStringListModel *) ui->lstviewSceneries->model();
    Yalib *ya = new Yalib();
    // don't try to autodetect
    ya->initialize(false);
        
    if(lstviewmodel==NULL)
    {
        lstviewmodel = new QStringListModel(QStringList());
        ui->lstviewSceneries->setModel(lstviewmodel);
    }
    if(lstviewmodel->rowCount()>0)
    {
        for(int i=0;i<lstviewmodel->rowCount();i++)
        {
            QString currItem = lstviewmodel->stringList().value(i).trimmed();
            if(fgenv->getDefaultScenery().trimmed().compare(currItem)!=0)
            {
                fgScenery += ":" + currItem;
            }
        }
    }
    QStringList params;

    // static arguments
    //params << "--verbose" << "--fg-root="+fgenv->getRootPath() << "--fg-scenery=/usr/share/games/flightgear/Scenery" << "--aircraft="+ui->cboAircrafts->currentText();

    params << "--verbose"
           << "--fg-root="+fgenv->getRootPath()
           << "--fg-scenery="+fgScenery
           << "--aircraft="+ui->cboAircrafts->currentText();

    if(ui->lnedtCallSign->text().trimmed().compare("")!=0)
    {
        params << "--callsign=\""+ ui->lnedtCallSign->text() +"\"";

        // only if a callsign is defined
        if(ui->lnedtMPServerName->text().trimmed().compare("")!=0)
        {
            params << "--multiplay=out,10," + ui->lnedtMPServerName->text().trimmed() + "," + lastPortOut;
        }
    }

    // ------------- Quick options -------------
    // Sound
    if(ui->ckbSound->isChecked())
    {
        params << "--enable-sound";
    }
    else
    {
        params << "--disable-sound";
    }
    if(fgVersion.split(".")[0].toInt()<3)
    {
        // OBSOLETE since FlightGear 3.0.0
        // Game Mode
        if(ui->ckbGameMode->isChecked())
        {
            params << "--enable-game-mode";
        }
        else
        {
            params << "--disable-game-mode";
        }
    }
    // Clouds
    if(ui->ckbClouds->isChecked())
    {
        params << "--enable-clouds";
    }
    else
    {
        params << "--disable-clouds";
    }
    // Fog
    if(ui->ckbFog->isChecked())
    {
        params << "--fog-fastest";
    }
    else
    {
        params << "--fog-disable";
    }
    // Full Screen
    if(ui->ckbFullScreen->isChecked())
    {
        params << "--enable-fullscreen";
        // devo usare questo trucco altrimenti X11ErrorHandling
        params.insert(0,"-e");
        params.insert(0,"xterm");
    }
    else
    {
        params << "--geometry="+lastResolutionSelected;
    }
    // Units
    if(ui->rdbUnitMeters->isChecked())
    {
        params << "--units-meters";
    }
    else
    {
        params << "--units-feets";
    }

    // control mode
    params << "--control=" + lastControlModeSelected;

    // ------------- Rendering -------------
    // Horizon effect
    if(ui->ckbHorizonEffect->isChecked())
    {
        params << "--enable-horizon-effect";
    }
    else
    {
        params << "--disable-horizon-effect";
    }
    
    if(fgVersion.split(".")[0].toInt()<3)
    {
        // OBSOLETE since FlightGear 3.0.0
        // Sky Blend
        if(ui->ckbSkyBlending->isChecked())
        {
            params << "--enable-skyblend";
        }
        else
        {
            params << "--disable-skyblend";
        }
    }
    // Textures
    if(ui->ckbTextures->isChecked())
    {
        params << "--enable-textures";
    }
    else
    {
        params << "--disable-textures";
    }
    // Distance attenuation
    if(ui->ckbDistanceAttenuation->isChecked())
    {
        params << "--enable-distance-attenuation";
    }
    else
    {
        params << "--disable-distance-attenuation";
    }
    // ------------- Features -------------
    // Failure
    if(ui->cboFailures->currentText().compare("none")!=0)
    {
        params << "--failure=" + lastFailureSelected;
    }
    // Lock fuel
    if(ui->ckbLockFuel->isChecked())
    {
        params << "--enable-fuel-freeze";
    }
    else
    {
        params << "--disable-fuel-freeze";
    }
    // Lock time
    if(ui->ckbLockTime->isChecked())
    {
        params << "--enable-clock-freeze";
    }
    else
    {
        params << "--disable-clock-freeze";
    }
    // Random Objects
    if(ui->ckbRandomObjects->isChecked())
    {
        params << "--enable-random-objects";
    }
    else
    {
        params << "--disable-random-objects";
    }
    // AI Models
    if(ui->ckbAIModels->isChecked())
    {
        params << "--enable-ai-models";
    }
    else
    {
        params << "--disable-ai-models";
    }
    // Random Trees
    if(!ui->ckbRandomTrees->isChecked())
    {
        params << "--prop:/sim/rendering/random-vegetation=false";
    }
    // AI Scenarios
    if(ui->lstvScenarios->selectionModel()->selectedIndexes().count()>0)
    {
        QStringListModel *scenariosModel = (QStringListModel *) ui->lstvScenarios->model();
        foreach(QModelIndex modindex, ui->lstvScenarios->selectionModel()->selectedIndexes())
        {
            params << "--ai-scenario=" + scenariosModel->stringList().value(modindex.row());
        }
    }
    // ------------- Aircraft -------------
    // Auto coordination
    if(ui->ckbAutoCoordination->isChecked())
    {
        params << "--enable-auto-coordination";
    }
    else
    {
        params << "--disable-auto-coordination";
    }
    // Panel
    if(ui->ckbPanel->isChecked())
    {
        params << "--enable-panel";
    }
    else
    {
        params << "--disable-panel";
    }
    if(ui->cboFDM->currentText().trimmed().compare("")!=0)
    {
        params << "--fdm=" + ui->cboFDM->currentText();
    }
    if((ui->ckbInAir->isChecked())&&(ui->lnedtAltitude->text().trimmed().compare("")!=0))
    {
        // using toInt and then QString::number to avoid passing not valid strings
        params << "--in-air" << "--altitude=" + QString::number(ui->lnedtAltitude->text().trimmed().toInt());
    }
    else
    {
        params << "--on-ground";
    }

    // ------------- HUD -------------
    // Hud 2D
    if(ui->rdbHud2D->isChecked())
    {
        params << "--enable-hud";
    }
    else
    {
        params << "--disable-hud";
    }
    // Hud 3D
    if(ui->rdbHud3D->isChecked())
    {
        params << "--enable-hud-3d";
    }
    else
    {
        params << "--disable-hud-3d";
    }
    // HUD AntiAlias
    if(ui->ckbHudAntialias->isChecked())
    {
        params << "--enable-anti-alias-hud";
    }
    else
    {
        params << "--disable-anti-alias-hud";
    }
    // ------------- Lights -------------
    // Specular Highlight
    if(ui->ckbSpecularReflections->isChecked())
    {
        params << "--enable-specular-highlight";
    }
    else
    {
        params << "--disable-specular-highlight";
    }
    // Enhanced lighting
    if(ui->ckbEnhancedLighting->isChecked())
    {
        params << "--enable-enhanced-lighting";
    }
    else
    {
        params << "--disable-enhanced-lighting";
    }
    // ------------- Environment ----------------
    if(ui->ckbWind->isChecked())
    {
        params << "--random-wind";
    }
    if(ui->hzsTurbulence->value())
    {
        params << "--turbulence=" + QString::number(lastTurbulence);
    }
    if(ui->ckbRealWeather->isChecked())
    {
        params << "--enable-real-weather-fetch";
    }
    else
    {
        params << "--disable-real-weather-fetch";
    }
    if(ui->edtCeilingQuote->text().trimmed().compare("")!=0)
    {
        if(ui->edtCeilingThickness->text().trimmed().compare("")!=0)
        {
            params << "--ceiling=" + ui->edtCeilingQuote->text().trimmed() + ":" + ui->edtCeilingThickness->text().trimmed();
        }
        else
        {
            params << "--ceiling=" + ui->edtCeilingQuote->text();
        }
    }
    if(ui->edtVisibility->text().trimmed().compare("")!=0)
    {
        if(ui->rdbUnitMeters->isChecked())
        {
            params << "--visibility=" + ui->edtVisibility->text().trimmed();
        }
        else
        {
            params << "--visibility-miles=" + ui->edtVisibility->text().trimmed();
        }
    }

    // ------------- Time -------------
    // DayTime
    if(ui->cboDayTime->currentText().compare("none")!=0)
    {
        params << "--timeofday="+ui->cboDayTime->currentText().trimmed();
    }
    // Season
    if(ui->cboSeason->currentText().compare("none")!=0)
    {
        params << "--season="+ui->cboSeason->currentText().trimmed();
    }
    // Airport
    // 2 --> longitude
    // 3 --> latitude
    QModelIndexList selected = ui->tbvAirports->selectionModel()->selectedIndexes();
    QStandardItemModel *model = (QStandardItemModel *) ui->tbvAirports->model();

    if((model!=NULL)&&(model->rowCount()>0))
    {
        if(selected.count()>0)
        {
            //QString longitude = model->item(selected.at(0).row(),2)->text().trimmed();
            //QString latitude = model->item(selected.at(0).row(),3)->text().trimmed();
            if((lastLongitude.compare("")!=0)&&(lastLatitude.compare("")!=0))
            {
                params << "--lon="+lastLongitude;
                params << "--lat="+lastLatitude;
            }
        }
    }

    // heading
    if(lastHeading.compare("")!=0)
    {
        params << "--heading="+lastHeading;
    }

    return params;
}

void MainWindow::toggleLoadingBarVisible()
{
    ui->lblLoading->setVisible(!ui->lblLoading->isVisible());
    QApplication::processEvents();
}

void MainWindow::loadSettings(QString confFile, bool appStart)
{
    if(just_started)
        just_started = false;
    toggleLoadingBarVisible();
    curr_settings = new Settings(confFile);
    appsettings *app_settings = new appsettings(fgenv->getYFHome()+"/appconf.ini");
    QString fgVersion = fgenv->getFGVersion();
    Yalib *ya = new Yalib();
    ya->initialize(true);
    
    if(!curr_settings->isEmpty())
    {
        (app_settings->getAirportListFiltered().compare(SET_TRUE)==0) ? ui->ckbFilterInstalled->setChecked(true) : ui->ckbFilterInstalled->setChecked(false);

        on_ckbFilterInstalled_toggled(ui->ckbFilterInstalled->isChecked());

        (curr_settings->getSound().compare(SET_TRUE)==0) ? ui->ckbSound->setChecked(true) : ui->ckbSound->setChecked(false);
        (curr_settings->getClouds().compare(SET_TRUE)==0) ? ui->ckbClouds->setChecked(true) : ui->ckbClouds->setChecked(false);
        (curr_settings->getFullScreen().compare(SET_TRUE)==0) ? ui->ckbFullScreen->setChecked(true) : ui->ckbFullScreen->setChecked(false);
        (curr_settings->getFog().compare(SET_TRUE)==0) ? ui->ckbFog->setChecked(true) : ui->ckbFog->setChecked(false);
        (curr_settings->getMeasureUnit().compare(SET_TRUE)==0) ? ui->rdbUnitMeters->setChecked(true) : ui->rdbUnitFeets->setChecked(true);
        (curr_settings->getFuelLock().compare(SET_TRUE)==0) ? ui->ckbLockFuel->setChecked(true) : ui->ckbLockFuel->setChecked(false);
        (curr_settings->getTimeLock().compare(SET_TRUE)==0) ? ui->ckbLockTime->setChecked(true) : ui->ckbLockTime->setChecked(false);
        (curr_settings->getRandomObjects().compare(SET_TRUE)==0) ? ui->ckbRandomObjects->setChecked(true) : ui->ckbRandomObjects->setChecked(false);
        (curr_settings->getAIModels().compare(SET_TRUE)==0) ? ui->ckbAIModels->setChecked(true) : ui->ckbAIModels->setChecked(false);
        (curr_settings->getRandomTrees().compare(SET_TRUE)==0) ? ui->ckbRandomTrees->setChecked(true) : ui->ckbRandomTrees->setChecked(false);
        (curr_settings->getAutoCoordination().compare(SET_TRUE)==0) ? ui->ckbAutoCoordination->setChecked(true) : ui->ckbAutoCoordination->setChecked(false);
        (curr_settings->getInAir().compare(SET_TRUE)==0) ? ui->ckbInAir->setChecked(true) : ui->ckbInAir->setChecked(false);
        (curr_settings->getPanel().compare(SET_TRUE)==0) ? ui->ckbPanel->setChecked(true) : ui->ckbPanel->setChecked(false);
        (curr_settings->getHorizonEffect().compare(SET_TRUE)==0) ? ui->ckbHorizonEffect->setChecked(true) : ui->ckbHorizonEffect->setChecked(false);
        if(fgVersion.split(".")[0].toInt()<3)
        {
            // OBSOLETE since FlightGear 3.0.0
            (curr_settings->getGameMode().compare(SET_TRUE)==0) ? ui->ckbGameMode->setChecked(true) : ui->ckbGameMode->setChecked(false);
            (curr_settings->getSkyBlending().compare(SET_TRUE)==0) ? ui->ckbSkyBlending->setChecked(true) : ui->ckbSkyBlending->setChecked(false);
        }
        (curr_settings->getTextures().compare(SET_TRUE)==0) ? ui->ckbTextures->setChecked(true) : ui->ckbTextures->setChecked(false);
        (curr_settings->getDistanceAttenuation().compare(SET_TRUE)==0) ? ui->ckbDistanceAttenuation->setChecked(true) : ui->ckbDistanceAttenuation->setChecked(false);
        (curr_settings->getWind().compare(SET_TRUE)==0) ? ui->ckbWind->setChecked(true) : ui->ckbWind->setChecked(false);
        (curr_settings->getRealWeather().compare(SET_TRUE)==0) ? ui->ckbRealWeather->setChecked(true) : ui->ckbRealWeather->setChecked(false);
        (curr_settings->getHUDAntiAliased().compare(SET_TRUE)==0) ? ui->ckbHudAntialias->setChecked(true) : ui->ckbHudAntialias->setChecked(false);
        // this two (hud2d and hud3d) may collide sometimes, check!!!
        (curr_settings->getHUD2D().compare(SET_TRUE)==0) ? ui->rdbHud2D->setChecked(true) : ui->rdbHud3D->setChecked(true);
        (curr_settings->getHUD3D().compare(SET_TRUE)==0) ? ui->rdbHud3D->setChecked(true) : ui->rdbHud3D->setChecked(true);
        (curr_settings->getEnhancedLighting().compare(SET_TRUE)==0) ? ui->ckbEnhancedLighting->setChecked(true) : ui->ckbEnhancedLighting->setChecked(false);
        (curr_settings->getSpecularReflections().compare(SET_TRUE)==0) ? ui->ckbSpecularReflections->setChecked(true) : ui->ckbSpecularReflections->setChecked(false);
        (curr_settings->getTerraSync().compare(SET_TRUE)==0) ? ui->ckbTerraSync->setChecked(true) : ui->ckbTerraSync->setChecked(false);

        if(curr_settings->getCallSign().trimmed().compare("")!=0)
        {
            ui->lnedtCallSign->setText(curr_settings->getCallSign().trimmed());
        }

        if(curr_settings->getMPServer().trimmed().compare("")!=0)
        {
            ui->lnedtMPServerName->setText(curr_settings->getMPServer().trimmed());
        }

        int fdmCurrIdx = ui->cboFDM->findText(curr_settings->getFDM().trimmed());
        if(fdmCurrIdx>=0)
        {
            ui->cboFDM->setCurrentIndex(fdmCurrIdx);
        }

        if(curr_settings->getScenarios().trimmed().compare("")!=0)
        {
            QStringList selection = curr_settings->getScenarios().split(":");
            QStringListModel *model = (QStringListModel *) ui->lstvScenarios->model();
            QItemSelectionModel *selectionModel = ui->lstvScenarios->selectionModel();
            foreach(QString sel, selection)
            {
                for(int row=0;row<model->rowCount();row++)
                {
                    QModelIndex modindex = model->index(row);
                    if(modindex.data().toString().trimmed().compare(sel)==0)
                    {
                        selectionModel->setCurrentIndex(modindex,QItemSelectionModel::Select);
                    }
                }
            }
        }

        if(curr_settings->getAltitude().trimmed().compare("")!=0)
        {
            ui->lnedtAltitude->setText(curr_settings->getAltitude().trimmed());
        }

        if(curr_settings->getCeilingQuote().trimmed().compare("")!=0)
        {
            ui->edtCeilingQuote->setText(curr_settings->getCeilingQuote().trimmed());
            if(curr_settings->getCeilingThickness().trimmed().compare("")!=0)
            {
                ui->edtCeilingThickness->setText(curr_settings->getCeilingThickness().trimmed());
            }
        }

        if(curr_settings->getVisibility().trimmed().compare("")!=0)
        {
            ui->edtVisibility->setText(curr_settings->getVisibility().trimmed());
        }

        int cmCurrIdx = ui->cboControlMode->findText(curr_settings->getControlMode().trimmed());
        if(cmCurrIdx>=0)
        {
            ui->cboControlMode->setCurrentIndex(cmCurrIdx);
        }

        // aircraft
        int aircraft_idx = ui->cboAircrafts->findText(curr_settings->getAircraft());
        if(aircraft_idx>=0)
        {
            ui->cboAircrafts->setCurrentIndex(aircraft_idx);
            on_cboAircrafts_currentIndexChanged(ui->cboAircrafts->itemText(aircraft_idx));
        }
        else
        {
            ui->txaLog->append(Logger::ET_WARN + ": "+ tr("Aircraft from configuration not available"));
        }

        if(curr_settings->getTurbulence().toFloat()>-1)
        {
            float wind = curr_settings->getTurbulence().toFloat();
            ui->hzsTurbulence->setValue((int) floor(wind*10));
        }


        int itemIndex = ui->cboWindowGeometries->findText(curr_settings->getResolution());
        if(itemIndex >= 0)
        {
            ui->cboWindowGeometries->setCurrentIndex(itemIndex);
        }
        itemIndex = ui->cboFailures->findText(curr_settings->getFailure());
        if(itemIndex >= 0)
        {
            ui->cboFailures->setCurrentIndex(itemIndex);
        }
        itemIndex = ui->cboDayTime->findText(curr_settings->getDayTime());
        if(itemIndex >= 0)
        {
            ui->cboDayTime->setCurrentIndex(itemIndex);
        }
        itemIndex = ui->cboSeason->findText(curr_settings->getSeason());
        if(itemIndex >= 0)
        {
            ui->cboSeason->setCurrentIndex(itemIndex);
        }

        // sceneries
        if(curr_settings->getSceneries().trimmed().compare("")!=0)
        {
            QStringListModel *lstviewmodel = (QStringListModel *) ui->lstviewSceneries->model();
            if(lstviewmodel==NULL)
                lstviewmodel = new QStringListModel(QStringList());
            lstviewmodel->removeRows(0,lstviewmodel->rowCount());
            QStringList sceneryList = curr_settings->getSceneries().trimmed().split("|");
            lstviewmodel->setStringList(sceneryList);
            ui->lstviewSceneries->setSelectionMode(QAbstractItemView::SingleSelection);
            ui->lstviewSceneries->setModel(lstviewmodel);
        }

        // heading
        lastHeading = curr_settings->getHeading();

        // mp ports
        lastPortIn = curr_settings->getMPIn();
        lastPortOut = curr_settings->getMPOut();
        check_mp_ports_values();


        if(appStart == false){
            // airports
            QString icao = curr_settings->getAirportICAO().trimmed();
            QStandardItemModel *model = (QStandardItemModel *) ui->tbvAirports->model();
            ui->tbvAirports->clearSelection();
            if(icao.compare("")!=0)
            {
                for(int i=0;i<model->rowCount();i++)
                {
                    if(model->item(i,1)->text().trimmed().compare(icao)==0)
                    {
                        QItemSelectionModel *selectionModel = ui->tbvAirports->selectionModel();
                        selectionModel->select(model->index(i,1),QItemSelectionModel::ClearAndSelect);
                        // runways
                        on_tbvAirports_clicked(model->index(i,1));
                        if(curr_settings->getRunway().trimmed().compare("")!=0){
                            ui->cboRunway->setCurrentIndex(ui->cboRunway->findText(curr_settings->getRunway().trimmed()));
                        }
                        place_aircraft_on_map_reading_settings();
                        center_mpmap_at_coords();
                        break;
                    }
                }
            }
        }

        ui->txaLog->append(Logger::ET_INFO + ": "+ tr("Configuration loaded correctly"));
        log->Log(Logger::ET_INFO, tr("Configuration loaded correctly"));
        /*QMessageBox msgBox("Success","Configuration loaded!",QMessageBox::Information,QMessageBox::Ok,NULL,NULL,this);
        msgBox.exec();*/
    }
    toggleLoadingBarVisible();
}

void MainWindow::check_mp_ports_values()
{
    if(lastPortIn.trimmed().compare("")==0)
        lastPortIn = "5000";
    if(lastPortOut.trimmed().compare("")==0)
        lastPortOut = "5000";
}

bool MainWindow::saveSettings(QString confFile)
{
    //if((!curr_settings)||(just_started))
    curr_settings = new Settings(confFile);

    appsettings *app_settings = new appsettings(fgenv->getYFHome()+"/appconf.ini");
    
    Yalib *ya = new Yalib();
    ya->initialize(true);
    
    ui->ckbFilterInstalled->isChecked() ? app_settings->setAirportListFiltered(SET_TRUE) : app_settings->setAirportListFiltered(SET_FALSE);

    QStringListModel *lstviewmodel = (QStringListModel *) ui->lstviewSceneries->model();
    if((lstviewmodel==NULL)||(lstviewmodel->rowCount()<=0))
    {
        lstviewmodel = new QStringListModel(QStringList());
        ui->lstviewSceneries->setModel(lstviewmodel);
        curr_settings->setSceneries("");
    }
    if(lstviewmodel->rowCount()>0)
    {
        QString sceneries = "";
        QString currScenery;
        for(int i=0;i<lstviewmodel->rowCount();i++)
        {
            currScenery = lstviewmodel->stringList().value(i).trimmed();
            if(currScenery.compare(fgenv->getDefaultScenery().trimmed())!=0)
                sceneries += currScenery + "|";
        }
        sceneries.remove(sceneries.length()-1,1);
        curr_settings->setSceneries(sceneries);
    }

    if(ui->lnedtCallSign->text().trimmed().compare("")!=0)
    {
        curr_settings->setCallSign(ui->lnedtCallSign->text().trimmed());
    }

    if(ui->lnedtMPServerName->text().trimmed().compare("")!=0)
    {
        curr_settings->setMPServer(ui->lnedtMPServerName->text().trimmed());
    }

    ui->ckbSound->isChecked() ? curr_settings->setSound(SET_TRUE) : curr_settings->setSound(SET_FALSE);
    ui->ckbClouds->isChecked() ? curr_settings->setClouds(SET_TRUE) : curr_settings->setClouds(SET_FALSE);
    ui->ckbFullScreen->isChecked() ? curr_settings->setFullScreen(SET_TRUE) : curr_settings->setFullScreen(SET_FALSE);
    ui->ckbFog->isChecked() ? curr_settings->setFog(SET_TRUE) : curr_settings->setFog(SET_FALSE);
    ui->rdbUnitMeters->isChecked() ? curr_settings->setMeasureUnit(SET_TRUE) : curr_settings->setMeasureUnit(SET_FALSE);
    ui->ckbLockFuel->isChecked() ? curr_settings->setFuelLock(SET_TRUE) : curr_settings->setFuelLock(SET_FALSE);
    ui->ckbLockTime->isChecked() ? curr_settings->setTimeLock(SET_TRUE) : curr_settings->setTimeLock(SET_FALSE);
    ui->ckbRandomObjects->isChecked() ? curr_settings->setRandomObjects(SET_TRUE) : curr_settings->setRandomObjects(SET_FALSE);
    ui->ckbAIModels->isChecked() ? curr_settings->setAIModels(SET_TRUE) : curr_settings->setAIModels(SET_FALSE);
    ui->ckbRandomTrees->isChecked() ? curr_settings->setRandomTrees(SET_TRUE) : curr_settings->setRandomTrees(SET_FALSE);
    ui->ckbAutoCoordination->isChecked() ? curr_settings->setAutoCoordination(SET_TRUE) : curr_settings->setAutoCoordination(SET_FALSE);
    ui->ckbInAir->isChecked() ? curr_settings->setInAir(SET_TRUE) : curr_settings->setInAir(SET_FALSE);
    ui->ckbPanel->isChecked() ? curr_settings->setPanel(SET_TRUE) : curr_settings->setPanel(SET_FALSE);
    ui->ckbHorizonEffect->isChecked() ? curr_settings->setHorizonEffect(SET_TRUE) : curr_settings->setHorizonEffect(SET_FALSE);
    if(ya->getFGVersion().split(".")[0].toInt()<3)
    {
        // OBSOLETE since FlightGear 3.0.0
        ui->ckbGameMode->isChecked() ? curr_settings->setGameMode(SET_TRUE) : curr_settings->setGameMode(SET_FALSE);
        ui->ckbSkyBlending->isChecked() ? curr_settings->setSkyBlending(SET_TRUE) : curr_settings->setSkyBlending(SET_FALSE);
    }
    ui->ckbTextures->isChecked() ? curr_settings->setTextures(SET_TRUE) : curr_settings->setTextures(SET_FALSE);
    ui->ckbDistanceAttenuation->isChecked() ? curr_settings->setDistanceAttenuation(SET_TRUE) : curr_settings->setDistanceAttenuation(SET_FALSE);
    ui->ckbWind->isChecked() ? curr_settings->setWind(SET_TRUE) : curr_settings->setWind(SET_FALSE);
    ui->ckbRealWeather->isChecked() ? curr_settings->setRealWeather(SET_TRUE) : curr_settings->setRealWeather(SET_FALSE);
    ui->ckbHudAntialias->isChecked() ? curr_settings->setHUDAntiAliased(SET_TRUE) : curr_settings->setHUDAntiAliased(SET_FALSE);
    ui->rdbHud2D->isChecked() ? curr_settings->setHUD2D(SET_TRUE) : curr_settings->setHUD2D(SET_FALSE);
    ui->rdbHud3D->isChecked() ? curr_settings->setHUD3D(SET_TRUE) : curr_settings->setHUD3D(SET_FALSE);
    ui->ckbEnhancedLighting->isChecked() ? curr_settings->setEnhancedLighting(SET_TRUE) : curr_settings->setEnhancedLighting(SET_FALSE);
    ui->ckbSpecularReflections->isChecked() ? curr_settings->setSpecularReflections(SET_TRUE) : curr_settings->setSpecularReflections(SET_FALSE);
    curr_settings->setTurbulence(QString::number(lastTurbulence));
    ui->ckbTerraSync->isChecked() ? curr_settings->setTerraSync(SET_TRUE) : curr_settings->setTerraSync(SET_FALSE);

    if(ui->lstvScenarios->selectionModel()->selectedIndexes().count()>0)
    {
        QStringListModel *scenariosModel = (QStringListModel *) ui->lstvScenarios->model();
        QString scenarios = "";
        foreach(QModelIndex modindex, ui->lstvScenarios->selectionModel()->selectedIndexes())
        {
            scenarios = scenarios + scenariosModel->stringList().value(modindex.row())+":";
        }
        if(scenarios.trimmed().compare("")!=0)
        {
            scenarios = scenarios.mid(0,scenarios.length()-1);
            curr_settings->setScenarios(scenarios);
        }
    }

    if((ui->lnedtAltitude->isEnabled())&&(ui->lnedtAltitude->text().trimmed().compare("")!=0))
    {
        if(ui->lnedtAltitude->text().trimmed().toInt() > 0)
        {
            curr_settings->setAltitude(ui->lnedtAltitude->text().trimmed());
        }
    }
    else
    {
        curr_settings->setAltitude("");
    }

    curr_settings->setAircraft(lastAircraft);

    curr_settings->setLongitude(lastLongitude);
    curr_settings->setLatitude(lastLatitude);
    curr_settings->setHeading(lastHeading);
    if(ui->tbvAirports->selectionModel()->selectedIndexes().count()>0){
        curr_settings->setAirportID(((QStandardItemModel*)ui->tbvAirports->model())->item(lastAirportIndex.row(),1)->text().trimmed());
        if(currentRunway)
            curr_settings->setRunway(currentRunway->getNumber());
    }

    curr_settings->setResolution(lastResolutionSelected);
    curr_settings->setFailure(lastFailureSelected);
    curr_settings->setDayTime(lastDayTimeSelected);
    curr_settings->setSeason(lastSeasonSelected);
    curr_settings->setControlMode(lastControlModeSelected);

    if(ui->cboFDM->currentText().trimmed().compare("")!=0)
    {
        curr_settings->setFDM(ui->cboFDM->currentText());
    }

    if(ui->edtCeilingQuote->text().trimmed().compare("")!=0)
    {
        curr_settings->setCeilingQuote(ui->edtCeilingQuote->text().trimmed());
        if(ui->edtCeilingThickness->text().trimmed().compare("")!=0)
        {
            curr_settings->setCeilingThickness(ui->edtCeilingThickness->text().trimmed());
        }
    }

    if(ui->edtVisibility->text().trimmed().compare("")!=0)
    {
        curr_settings->setVisibility(ui->edtVisibility->text().trimmed());
    }

    if(lastPortIn.trimmed().compare("")!=0)
    {
        curr_settings->setMPIn(lastPortIn);
    }

    if(lastPortOut.trimmed().compare("")!=0)
    {
        curr_settings->setMPOut(lastPortOut);
    }

    if(curr_settings->storeData() && app_settings->storeData())
    {
        ui->txaLog->append(Logger::ET_INFO + ": "+ tr("Configuration stored correctly"));
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Success"));
        msgBox.setText(tr("Configuration stored!"));
        //msgBox.setInformativeText("Success");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.addButton(QMessageBox::Ok);
        msgBox.setModal(true);
        msgBox.exec();
        return true;
    }else{
        ui->txaLog->append(Logger::ET_WARN + ": "+ tr("Configuration NOT stored"));
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Failure"));
        msgBox.setText(tr("Configuration NOT stored!"));
        //msgBox.setInformativeText("Failure");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.addButton(QMessageBox::Close);
        msgBox.setModal(true);
        msgBox.exec();
    }
    return false;
}

void MainWindow::on_pbtSaveConf_clicked()
{
    QString configurationFilePath = QFileDialog::getSaveFileName(this,
                                                                 tr("Choose the file where you want to store your current configuration"),
                                                                 fgenv->getYFHome(),
                                                                 tr("INI (*.ini);;"));
    if(configurationFilePath.trimmed().compare("")==0)
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"), tr("No file selected\nTrying to use default"), QMessageBox::Ok,this);
        msgBox.exec();
        configurationFilePath = fgenv->getYFHome() + "/conf.ini";
    }
    if(QFile::exists(configurationFilePath))
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"), tr("The file already exists\nClick Ok to overwrite it"), QMessageBox::Ok|QMessageBox::Cancel,this);
        if(msgBox.exec()==QMessageBox::Cancel)
        {
            return;
        }
    }
    if(!configurationFilePath.endsWith(".ini"))
    {
        configurationFilePath.append(".ini");
    }
    saveSettings(configurationFilePath);
}

void MainWindow::on_pbtLoadConf_clicked()
{
    QString configurationFilePath = QFileDialog::getOpenFileName(this,
                                                                 tr("Select the configuration you want to load"),
                                                                 fgenv->getYFHome(),
                                                                 tr("INI (*.ini);;"));
    if(configurationFilePath.trimmed().compare("")==0)
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"), tr("No file selected\nTrying to use default"), QMessageBox::Ok,this);
        msgBox.exec();
        configurationFilePath = fgenv->getYFHome() + "/conf.ini";
    }
    if(!QFile::exists(configurationFilePath))
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"), tr("File do not exists"), QMessageBox::Ok,this);
        msgBox.exec();
        return;
    }
    loadSettings(configurationFilePath);
}

QHash<QString, QStringList> MainWindow::collect_all_airports()
{
    QHash<QString, QStringList> resultAirports = fgenv->getDefaultAirportList();
    QStringListModel *lstviewmodel = (QStringListModel *) ui->lstviewSceneries->model();
    if((lstviewmodel==NULL)||(lstviewmodel->rowCount()<=0))
    {
        lstviewmodel = new QStringListModel(QStringList());
        ui->lstviewSceneries->setModel(lstviewmodel);
    }
    QHash<QString, QStringList> tmpAirports = fgenv->parseAirportsIndex(fgenv->getYFScenery()+"/Airports/index.txt");
    foreach(QString key, tmpAirports.keys())
    {
        if(!resultAirports.contains(key))
        {
            resultAirports.insert(key,tmpAirports[key]);
        }
    }
    for(int i=0;i<lstviewmodel->rowCount();i++)
    {
        tmpAirports = fgenv->parseAirportsIndex(lstviewmodel->stringList().value(i).trimmed()+"/Airports/index.txt");
        foreach(QString key, tmpAirports.keys())
        {
            if(!resultAirports.contains(key))
            {
                resultAirports.insert(key,tmpAirports[key]);
            }
        }
    }

    return resultAirports;
}

QStringList MainWindow::collect_all_airports_dir()
{
    QStringList allAirportDirectories;
    allAirportDirectories << fgenv->getDefaultScenery() + "/Airports";
    allAirportDirectories << fgenv->getYFScenery() + "/Airports";
    QStringListModel *lstviewmodel = (QStringListModel *) ui->lstviewSceneries->model();
    if(!lstviewmodel)
        lstviewmodel = new QStringListModel();
    for(int i=0;i<lstviewmodel->rowCount();i++)
    {
        allAirportDirectories << lstviewmodel->stringList().value(i).trimmed()+"/Airports";
    }
    return allAirportDirectories;
}

void MainWindow::completedAptCache()
{
    QString cacheBuilt = (aptdat.cacheBuilt())? "READY" : "NOT READY";
    // now that cache is built, setup the airport list
    setup_airport_list();
    ui->pbarAptCache->hide();
    ui->btnRecreateAiportsIndex->setEnabled(true);
    ui->btnRefreshAirportList->setEnabled(true);
    ui->lblLoading->setText("Airports cache status");
}

void MainWindow::updateCacheProgress(int currLine, int lineCount)
{
    ui->pbarAptCache->setMinimum(0);
    ui->pbarAptCache->setMaximum(lineCount);
    ui->pbarAptCache->setValue(currLine);
}

void MainWindow::setup_airport_list()
{   
    QStandardItemModel *model = new QStandardItemModel(1,4);
    ui->tbvAirports->setModel(model);

    model->setHorizontalHeaderLabels(QStringList() << "I"
                                                   << "ICAO"
                                                   << "Description"
                                                   << "Directory");

    ui->tbvAirports->setColumnWidth(0,20);
    ui->tbvAirports->setColumnWidth(1,50);
    ui->tbvAirports->setColumnWidth(2,ui->tbvAirports->width()-80);
    ui->tbvAirports->setColumnHidden(3,true);
    ui->tbvAirports->setShowGrid(false);
    ui->tbvAirports->verticalHeader()->hide();

    ui->tbvAirports->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tbvAirports->setSelectionMode(QAbstractItemView::SingleSelection);

    // IMPORTANT: this should be ported inside a separated thread to allow GUI to appear real-time while it works

    //APT_dat aptdat(fgenv->getAPTSource(),fgenv->getYFHome());

    int row = 0;
    QHash<QString, QStringList> cache = aptdat.getAirports();
    model->setRowCount(cache.count());
    foreach(QString key, cache.keys())
    {
        if(cache[key][1].trimmed().compare("")==0)
        {
            // ap not installed
            model->setData(model->index(row,0),"");
            model->setData(model->index(row,0), QBrush(Qt::gray),Qt::BackgroundRole);
        }
        else
        {
            model->setData(model->index(row,0),"X");
            model->setData(model->index(row,0), QBrush(Qt::green),Qt::BackgroundRole);
        }

        model->setData(model->index(row,1),key);
        model->setData(model->index(row,2),cache[key][0]);
        model->setData(model->index(row,3),cache[key][1]);
        row++;
    }
}

void MainWindow::on_btnRefreshAirportList_clicked()
{
    setup_airport_list();
}

void MainWindow::on_ckbFilterInstalled_toggled(bool checked)
{
    QStandardItemModel *model = (QStandardItemModel *) ui->tbvAirports->model();
    QList<QStandardItem *> filterItems;
    filterItems = model->findItems("",Qt::MatchExactly,0);
    if(checked)
    {
        foreach(QStandardItem *item, filterItems)
        {
            ui->tbvAirports->hideRow(item->row());
        }
    }
    else
    {
        foreach(QStandardItem *item, filterItems)
        {
            ui->tbvAirports->showRow(item->row());
        }
    }
}

void MainWindow::on_tbvAirports_clicked(const QModelIndex &index)
{
    QStandardItemModel *model = (QStandardItemModel *) ui->tbvAirports->model();
    QString icao = model->item(index.row(),1)->text().trimmed();
    //APT_dat apdat(fgenv->getAPTSource(),fgenv->getYFHome());

    ap_runways = aptdat.getRunwaysByAirport(icao);

    ui->cboRunway->clear();

    bool isFirst = true;

    foreach(Runway *rw, ap_runways)
    {
        if(rw->getNumber().compare("xxx")!=0)
        {
            if(isFirst){
                currentRunway = rw;
                isFirst = false;
            }
            ui->cboRunway->addItem(rw->getNumber());
        }
        else if(rw->getNumber().compare("xxx")==0)
        {
            // taxiway

        }
        else
        {

        }
    }

    on_cboRunway_currentIndexChanged(ui->cboRunway->currentText());

    lastAirportIndex = index;
    lastLongitude = currentRunway->getLongitude();
    lastLatitude = currentRunway->getLatitude();

}

void MainWindow::on_cboWindowGeometries_currentIndexChanged(const QString &arg1)
{
    lastResolutionSelected = arg1;
}

void MainWindow::on_cboFailures_currentIndexChanged(const QString &arg1)
{
    lastFailureSelected = arg1;
}

void MainWindow::on_cboDayTime_currentIndexChanged(const QString &arg1)
{
    lastDayTimeSelected = arg1;
}

void MainWindow::on_cboSeason_currentIndexChanged(const QString &arg1)
{
    lastSeasonSelected = arg1;
}

//void MainWindow::place_aircraft_on_map_reading_table()
//{
//    QStandardItemModel *model = (QStandardItemModel *) ui->tbvAirports->model();
//    QModelIndexList modelidxlst = ui->tbvAirports->selectionModel()->selectedIndexes();

//    if(modelidxlst.count()<=0)
//        return;

//    lastLongitude = model->item(modelidxlst.at(0).row(),2)->text().trimmed();
//    lastLatitude = model->item(modelidxlst.at(0).row(),3)->text().trimmed();
//    //lastHeading = QString::number(ui->dialHeading->value());
//    lastHeading = currentRunway->getHeading();

//    update_latlonhead(lastLatitude, lastLongitude, lastHeading);
//}

void MainWindow::place_aircraft_on_map_reading_settings()
{
    lastLongitude = QString::number(currentRunway->getLongitude().toDouble());
    lastLatitude = QString::number(currentRunway->getLatitude().toDouble());
    lastHeading = currentRunway->getHeading();

//    adjust_heading_value(lastHeading.toDouble());
    ui->spboxHeading->setValue(lastHeading.toDouble());

    update_latlonhead(lastLatitude, lastLongitude, lastHeading);
}


void MainWindow::center_mpmap_at_coords()
{
    mpmapbridge *bridge = new mpmapbridge();
    bridge->setLonLat(lastLongitude, lastLatitude);
    bridge->setPilotToFollow(ui->lnedtCallSign->text().trimmed());

    log->Log(Logger::ET_INFO, bridge->getUrl());

    //TOMIGRATE
    ui->webView->setUrl(bridge->getLLUrlWithoutMenu());
}

void MainWindow::update_latlonhead(QString lat, QString lon, QString heading)
{
    ui->dialHeading->setValue((int)convert_dialhead_to_azimuth(lastHeading.toDouble()));
    ui->webView->page()->mainFrame()->evaluateJavaScript("aggiornaCentroMappa(" + lon + "," + lat + ",true);");
    ui->webView->page()->mainFrame()->evaluateJavaScript("aggiornaLonLat(" + lon + "," + lat + ", true," + heading + ");");
}

double MainWindow::convert_dialhead_to_azimuth(double head)
{
    if((head >= 0) && (head < 180))
    {
        return head + 180;
    }
    else if ((head >= 180) && (head < 360))
    {
        return head - 180;
    }
    return -1;
}

void MainWindow::on_dialHeading_valueChanged(int value)
{
    update_latlonhead(lastLatitude, lastLongitude, QString::number(convert_dialhead_to_azimuth(value)));
    QStringList splitted = QString::number(ui->spboxHeading->value()).split('.');
    QString decimal = "";
    if(splitted.count() > 1)
        decimal = splitted[1];
    else
        decimal = "0";
    double newValue = convert_dialhead_to_azimuth(value) + QString("0," + decimal).toDouble();
    ui->spboxHeading->setValue(newValue);
}

void MainWindow::on_btnGoToMap_clicked()
{
    place_aircraft_on_map_reading_settings();
    center_mpmap_at_coords();
    ui->tabOpts->setCurrentWidget(ui->tabMap);
}

void MainWindow::on_btnAboutQt_clicked()
{
    QMessageBox msgBox;
    msgBox.aboutQt(this, "About Qt");
}

void MainWindow::on_cboRunway_currentIndexChanged(const QString &arg1)
{
    if(just_started)
        return;
    foreach(Runway *rw, ap_runways)
    {
        if(rw->getNumber().compare(arg1)==0)
        {
            currentRunway = rw;
            lastHeading = rw->getHeading();
            break;
        }
    }
}

void MainWindow::on_btnRunwayInfo_clicked()
{
    if(currentRunway != nullptr)
    {
        RunwayDetailsDialog dlgRunwayDetails(this);
        dlgRunwayDetails.setCurrentRunway(currentRunway);
        dlgRunwayDetails.exec();
    }
}

void MainWindow::on_btnAdd_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select scenery directory"),
                                                    fgenv->getYFHome(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    add_scenery_path(dir);
}

void MainWindow::on_btnDel_clicked()
{
    QStringListModel *lstviewmodel = (QStringListModel *) ui->lstviewSceneries->model();
    if(!ui->lstviewSceneries->currentIndex().isValid())
        return;
    QMessageBox msgbox;
    msgbox.setText(tr("Dropping: ")+
                   lstviewmodel->stringList().value(ui->lstviewSceneries->currentIndex().row()).trimmed()+
                   tr("\nAre you sure?"));
    msgbox.addButton(QMessageBox::Ok);
    msgbox.addButton(QMessageBox::Cancel);
    if(msgbox.exec() == QMessageBox::Ok)
        lstviewmodel->removeRow(ui->lstviewSceneries->currentIndex().row());
}

void MainWindow::add_scenery_path(QString sceneryPath)
{
    bool alreadyPresent=false;

    QStringListModel *lstviewmodel = (QStringListModel *) ui->lstviewSceneries->model();
    if((lstviewmodel==NULL)||(lstviewmodel->rowCount()<=0))
    {
        lstviewmodel = new QStringListModel(QStringList());
        ui->lstviewSceneries->setModel(lstviewmodel);
        if((sceneryPath.trimmed().compare(fgenv->getDefaultScenery())!=0)&&
                (sceneryPath.trimmed().compare(fgenv->getYFScenery())!=0))
        {
            alreadyPresent = false;
        }
        else
        {
            alreadyPresent = true;
        }
    }
    else{
        for(int i=0;i<lstviewmodel->rowCount();i++)
        {
            if(lstviewmodel->stringList().value(i).trimmed().compare(sceneryPath.trimmed())!=0)
            {
                if((lstviewmodel->stringList().value(i).trimmed().compare(fgenv->getDefaultScenery().trimmed())!=0)&&
                        (lstviewmodel->stringList().value(i).trimmed().compare(fgenv->getYFScenery())!=0))
                {
                    alreadyPresent = false;
                }
                else
                {
                    alreadyPresent = true;
                    break;
                }
            }
            else
            {
                alreadyPresent = true;
                break;
            }
        }
    }

    if(alreadyPresent == false)
    {
        QStringList newList = lstviewmodel->stringList();
        newList.append(sceneryPath.trimmed());
        lstviewmodel->setStringList(newList);
        qDebug("%s", sceneryPath.toStdString().data());
    }

}

//void MainWindow::follow_pilot_on_mpmap()
//{
//    mpmapbridge *bridge = new mpmapbridge();
//    bridge->setPilotToFollow(ui->lnedtCallSign->text().trimmed());
//    ui->webViewMMap->stop();
//    ui->webViewMMap->load(QUrl(bridge->followPilot()));
//}

void MainWindow::hndl_tmr_update_map_with_plane()
{
    on_dialHeading_valueChanged(360);
    tmrProcCoords->stop();
}

void MainWindow::hndl_tmr_procfgfs()
{
    if(procFGFS->state() == QProcess::Running)
    {
        QIcon icon(":/icons/icons/btn_yf_cross.png");
        ui->pbtLaunch->setIcon(icon);
        ui->pbtLaunch->setText("Stop");
    }
    else
    {
        QIcon icon(":/icons/icons/fgrun.png");
        ui->pbtLaunch->setIcon(icon);
        ui->pbtLaunch->setText(tr("Launch!"));
        proc_fgfs_is_running = false;
        tmrProcFGFS->stop();
    }
}

void MainWindow::hndl_tmr_procts()
{
    if(procTerraSync->state() == QProcess::Running)
    {
        QPixmap pixmap(":/icons/icons/applications-internet.png");
        ui->lblTerraSyncStatus->setPixmap(pixmap);
        ui->lblTerraSyncStatus->setToolTip(tr("TerraSync running"));
        ui->lblTerraSyncStatus->setScaledContents(true);
    }
    else
    {
        QPixmap pixmap(":/icons/icons/btn_yf_cross.png");
        ui->lblTerraSyncStatus->setPixmap(pixmap);
        ui->lblTerraSyncStatus->setToolTip(tr("TerraSync not running"));
        ui->lblTerraSyncStatus->setScaledContents(true);
        tmrProcTS->stop();
    }
}

void MainWindow::change_selected_coords()
{
    QString coords = ui->webView->page()->mainFrame()->evaluateJavaScript("document.getElementById('yaflight').innerText;").toString();
    if(coords.trimmed().compare("")!=0)
    {
        QStringList aCoords = coords.split(",");
        ui->lblLatitude->setText(tr("Latitude")+": "+aCoords[1].trimmed());
        ui->lblLongitude->setText(tr("Longitude")+": "+aCoords[0].trimmed());
        ui->lblLatitude->setToolTip(ui->lblLatitude->text().trimmed());
        ui->lblLongitude->setToolTip(ui->lblLongitude->text().trimmed());
        lastLongitude = aCoords[0].trimmed();
        lastLatitude = aCoords[1].trimmed();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox(QMessageBox::Warning,tr("Warning"),tr("Are you sure you want stop your simulation?"),QMessageBox::Ok|QMessageBox::Cancel,this);
    if(msgBox.exec()==QMessageBox::Ok)
    {
        if(mpmdiag)
        {
            if(mpmdiag->isVisible()||
                    mpmdiag->isHidden())
                mpmdiag->close();
        }
        if(hpdiag)
        {
            if(hpdiag->isVisible()||
                    hpdiag->isHidden())
                   hpdiag->close();
        }
        stopTerraSync();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::on_hzsTurbulence_valueChanged(int value)
{
    lastTurbulence = value / 10.0;
}

void MainWindow::on_ckbInAir_toggled(bool checked)
{
    if(checked)
    {
        ui->lnedtAltitude->setEnabled(true);
    }
    else
    {
        ui->lnedtAltitude->setEnabled(false);
    }
}


void MainWindow::on_cboControlMode_currentIndexChanged(const QString &arg1)
{
    lastControlModeSelected = arg1;
}

void MainWindow::on_btnRecreateAiportsIndex_clicked()
{
    QMessageBox msgbox(QMessageBox::Warning,tr("Warning"),tr("Are you sure you want to rebuild the entire Airports index?\nIt should take a few minutes on slower machines"),QMessageBox::Ok|QMessageBox::Cancel,this);
    if(msgbox.exec()==QMessageBox::Ok)
    {
        aptdat.create_cache(collect_all_airports_dir());
    }
}

void MainWindow::on_pbtSearchAirport_clicked()
{
    QStandardItemModel *model = (QStandardItemModel *)ui->tbvAirports->model();
    int column_to_search = -1;

    if(ui->rdbSearchById->isChecked())
    {
        column_to_search = 1; // ICAO
    }
    else
    {
        column_to_search = 2; // Long name of the Airport
    }

    searchFoundAirportsIndex = 0;
    searchFoundAirports = model->findItems(ui->lnedtSearchAirport->text().trimmed(),
                                                         Qt::MatchContains,
                                                         column_to_search);
    if(searchFoundAirports.count() > 0)
    {
        ui->lblFoundItems->setText(tr("Found")+": "+QString::number(searchFoundAirports.count()));
        ui->tbvAirports->setCurrentIndex(model->indexFromItem(searchFoundAirports.at(0)));
        ui->tbvAirports->selectRow(searchFoundAirports.at(0)->row());
        searchFoundAirportsIndex = 0;
        if(searchFoundAirports.count()>1)
            ui->pbtFindNext->setEnabled(true);
    }
    else
    {
        ui->lblFoundItems->setText(tr("Found")+": 0");
        ui->pbtFindNext->setEnabled(false);
    }
}

void MainWindow::on_pbtFindNext_clicked()
{
    QStandardItemModel *model = (QStandardItemModel *)ui->tbvAirports->model();
    searchFoundAirportsIndex++;
    ui->tbvAirports->setCurrentIndex(model->indexFromItem(searchFoundAirports.at(searchFoundAirportsIndex)));
    ui->tbvAirports->selectRow(searchFoundAirports.at(searchFoundAirportsIndex)->row());
    if(searchFoundAirportsIndex >= searchFoundAirports.count()-1)
        ui->pbtFindNext->setEnabled(false);
}

void MainWindow::on_pbtSpecifyMPPorts_clicked()
{
    DialogMPDetails *mpdetailsdiag = new DialogMPDetails(this);
    check_mp_ports_values();
    mpdetailsdiag->setIn(lastPortIn.trimmed().toInt());
    mpdetailsdiag->setOut(lastPortOut.trimmed().toInt());
    if(mpdetailsdiag->exec() == 1)
    {
        lastPortIn = mpdetailsdiag->getIn();
        lastPortOut = mpdetailsdiag->getOut();
    }
}

void MainWindow::on_spboxHeading_valueChanged(QString value)
{
    lastHeading = value;
}

void MainWindow::on_pbtTerraSyncStartStop_clicked()
{
#ifndef Q_OS_MACX
    if(proc_ts_is_running)
    {
        stopTerraSync();
        ui->pbtTerraSyncStartStop->setText(tr("Start"));
    }
    else
    {
        startTerraSync();
        ui->pbtTerraSyncStartStop->setText(tr("Stop"));
    }
#else
    ui->pbtTerraSyncStartStop->setEnabled(false);
#endif
}

void MainWindow::startTerraSync()
{
    QStringList ts_params;
    QStringList env = QProcess::systemEnvironment();
    procTerraSync = new QProcess();
    procTerraSync->setProcessChannelMode(QProcess::MergedChannels);
    procTerraSync->setReadChannel(QProcess::StandardOutput);
    procTerraSync->setEnvironment(env);
    ts_params << "-pid" << fgenv->getTerraSyncPidFilePath()
              << "-S" // (makes ts using svn protocol rather than rsync)
              << "-p" << "5500"
              << "-d" << fgenv->getYFScenery();
    procTerraSync->start(fgenv->getTerraSyncBinPath(), ts_params, QProcess::ReadOnly);
    log->Log(Logger::ET_INFO, fgenv->getTerraSyncBinPath() + " " + ts_params.join(" "));
    ui->txaLog->append(Logger::ET_INFO + ": " + tr("Starting TerraSync"));
    connect(procTerraSync,SIGNAL(readyRead()),this,SLOT(read_ts_output()));
    connect(procTerraSync,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(proc_ts_finished()));

    tmrProcTS = new QTimer();
    connect(tmrProcTS, SIGNAL(timeout()),this,SLOT(hndl_tmr_procts()));
    tmrProcTS->start(350);

    procTerraSync->closeWriteChannel();
    if((procTerraSync->state() == QProcess::Starting)||
            (procTerraSync->state() == QProcess::Starting))
        proc_ts_is_running = true;
}

void MainWindow::stopTerraSync()
{
    if((proc_ts_is_running)&&((procTerraSync->state()==QProcess::Running)||
            (procTerraSync->state()==QProcess::Starting)))
        procTerraSync->kill();
    proc_ts_is_running = false;
}

void MainWindow::on_pbtOpenMPMap_clicked()
{
    mpmdiag = new MPMDialog(this);
    mpmdiag->show();
}

void MainWindow::on_pbtMPHostnameContextualHelp_clicked()
{
    showHelp("http://wiki.flightgear.org/Howto:Multiplayer#Servers");
}

void MainWindow::on_pbtWeatherContextualHelp_clicked()
{
    showHelp("http://wiki.flightgear.org/Weather");
}

void MainWindow::showHelp(QString url)
{
    if(!hpdiag)
    {
        hpdiag = new HelpDialog();
    }
    hpdiag->setUrl(url);
    hpdiag->showMaximized();
}


void MainWindow::on_pbtInstallModel_clicked()
{
    QStringList env = QProcess::systemEnvironment();
    procYaInstall = new QProcess();
    procYaInstall->setEnvironment(env);
    procYaInstall->setProcessChannelMode(QProcess::MergedChannels);
    connect(procYaInstall,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(yainstall_proc_finished()));
    log->Log(Logger::ET_INFO,tr("Launching")+" "+fgenv->getYaInstallBinary());
    procYaInstall->start(fgenv->getYaInstallBinary(),QStringList(),QProcess::ReadOnly);
}

void MainWindow::yainstall_proc_finished()
{
    procYaInstall->kill();
    procYaInstall->close();
    delete procYaInstall;
    refreshListOfAircrafts();
    populate_combo_aircrafts();
}

void MainWindow::on_pbtDeselectAllScenarios_clicked()
{
    ui->lstvScenarios->clearSelection();
}

void MainWindow::populate_yaflight_theme_list()
{
    ui->cboTheme->clear();
    ui->cboTheme->addItems(appsett->getAvailableThemes());
    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    int idxDefaultTheme = ui->cboTheme->findText("default");
    if(idxDefaultTheme >= 0)
    {
        ui->cboTheme->setCurrentIndex(idxDefaultTheme);
    }
    else
    {
        ui->cboTheme->setCurrentIndex(0);
    }
    #else
    ui->cboTheme->setCurrentText("default");
    #endif
}

/*void MainWindow::paintEvent(QPaintEvent* event)
{
        Q_UNUSED(event)

        updateMask();

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing); // we need this in order to get correct rounded corners
        painter.setPen(QPen(QBrush(Qt::black), 2.0));
        painter.setBrush(QBrush(QColor(Qt::yellow)));
        painter.drawRoundedRect(rect().adjusted(2,1,-2,-1), 5, 5);
}

QRegion MainWindow::getRegion() const
{
        QPainterPath path;
        path.addRoundedRect(rect().adjusted(1, -1, -1, 0), 6, 6);
        return QRegion(path.toFillPolygon().toPolygon());
}

void MainWindow::updateMask()
{
        QBitmap Bitmap(size());
        Bitmap.clear();
        QPainter Painter(&Bitmap);

        Painter.setClipRegion(getRegion());
        Painter.fillRect(rect(), Qt::color1);
        Painter.end();

        setMask(Bitmap);
}
*/

void MainWindow::on_cboTheme_currentTextChanged(const QString &arg1)
{
    QFile styleFile(fgenv->getYFHome()+"/themes/"+arg1+".qss");
    QString styleContent = "";
    if(!styleFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(0,"Error",styleFile.errorString());
    }
    QTextStream in(&styleFile);
    while(!in.atEnd())
    {
        styleContent += in.readLine();
    }
    styleFile.close();
    this->setStyleSheet(styleContent);
    this->update();
}
