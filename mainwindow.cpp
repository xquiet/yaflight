#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    //QMainWindow(parent,Qt::FramelessWindowHint),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    just_started = true;

    fgenv = new FGEnvironment();

    //curr_settings = new Settings(fgenv->getYFHome()+"/conf.ini");

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

    /*
    QHashIterator<QString, QString> item(hashOfAircrafts);
    while (item.hasNext()) {
        item.next();
        listOfAircrafts << item.key();
    }
    */

    // windows geometries
    QStringList windowGeometries;
    windowGeometries << "1280x1024" << "1024x768" << "800x600" << "640x480" <<
                        "1024x576" << "1152x648" << "1280x720" << "1366x780" <<
                        "1600x900" << "1920x1080" << "2560x1440" << "3840x2160";
    //windowGeometries.sort();
    ui->cboWindowGeometries->addItems(windowGeometries);

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


    QStringList listOfAircrafts = hashOfAircrafts.keys();
    listOfAircrafts.sort();
    ui->cboAircrafts->addItems(listOfAircrafts);

    ui->txaLog->append("OS: " + fgenv->getOS());
    ui->txaLog->append("FG version: " + fgenv->getFGVersion());
    ui->txaLog->append("FGROOT: " + fgenv->getRootPath());
    ui->txaLog->append("FGSCEN: " + fgenv->getDefaultScenery());
    ui->txaLog->append("Aircraft dir: " + fgenv->getAircraftsDir());

    ui->lblDefaultScenery->setText(fgenv->getDefaultScenery());

//    QMessageBox msgbox;
//    msgbox.setText(fgenv->__read_winprogramfiles());
//    msgbox.exec();

    loadSettings(true);

    setup_airport_list();

    // double call to loadSettings
    // trick to make airport from conf.ini
    // to be properly selected
    loadSettings();

    just_started = false;
}

MainWindow::~MainWindow()
{
    delete ui;
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
        qDebug("Directory %s doesn't exists",aircraftsDir.absolutePath().toStdString().c_str());
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
            //result << QString(listOfAircrafts.value(i) + ":" + xml.value(j).replace("-set.xml","",Qt::CaseSensitive));
            qDebug("%s",xml.value(j).toStdString().c_str());
        }
    }
    result.end();
    return result;
}

void MainWindow::on_btnAbout_clicked()
{
    QMessageBox msgbox;
    msgbox.about(this,"About","yaflight4w - yaflight for windows\nVersion:" + QString::number(VERSION) + "\n(C) 2011-2012 by Matteo Pasotti");
}

void MainWindow::on_pbtLaunch_clicked()
{
    QStringList params = collectLaunchSettings();
    procFGFS = new QProcess();
    procFGFS->setProcessChannelMode(QProcess::MergedChannels);
    procFGFS->setReadChannel(QProcess::StandardOutput);
    QStringList env = QProcess::systemEnvironment();
    procFGFS->setEnvironment(env);
    procFGFS->start(fgenv->getFgfsBinPath(), params, QProcess::ReadOnly);
    //if(!pls->waitForStarted())
    //    return false;
    ui->txaLog->append("Launching...");
    ui->txaLog->append(fgenv->getFgfsBinPath()+" "+params.join(" "));
    connect(procFGFS,SIGNAL(readyRead()),this,SLOT(readAircrafts()));
    connect(procFGFS,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(procReadAircraftsFinished(int, QProcess::ExitStatus)));

    procFGFS->closeWriteChannel();
}

void MainWindow::readAircrafts()
{
    QByteArray bytes = procFGFS->readAll();
    QString strLines = QString(bytes);
    ui->txaLog->append(strLines);
}

void MainWindow::procReadAircraftsFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    ui->txaLog->append("Simulation complete");
    QString message = QString("process exited with code: %1, status: %2\n")
        .arg(exitCode)
        .arg(exitStatus == QProcess::NormalExit ? "QProcess::NormalExit" : "QProcess::CrashExit");
    qDebug("%s",message.toStdString().data());
    ui->txaLog->append(message);
}

void MainWindow::on_cboAircrafts_currentIndexChanged(const QString &arg1)
{
    drawThumbnail(hashOfAircrafts.value(arg1.trimmed()));
    if(!just_started)
        curr_settings->setAircraft(arg1);
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

    dlgAircraftDetails dlg(details, this);
    dlg.setModal(true);
    dlg.exec();
}

void MainWindow::on_btnExit_clicked()
{
    QMessageBox msgBox(QMessageBox::Warning,"Warning","Are you sure you want stop you simulation?",QMessageBox::Ok|QMessageBox::Cancel,this);
    if(msgBox.exec()==QMessageBox::Ok)
    {
        this->close();
    }
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
    this->setGeometry(x(),y(),width(),ui->expanderOpts->height()+ui->expanderOpts->height()+2);
}

void MainWindow::expOptsUnexpanded()
{
    this->setGeometry(x(),y(),width(),ui->expanderOpts->height()+ui->expanderOpts->height()+2);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
 {
     if (event->buttons() & Qt::LeftButton) {
         move(event->globalPos() - dragPosition);
         event->accept();
     }
 }

QStringList MainWindow::collectLaunchSettings()
{
    QString fgScenery = fgenv->getDefaultScenery();
    if(ui->tblSceneries->rowCount()>0)
    {
        for(int i=0;i<ui->tblSceneries->rowCount();i++)
        {
            QString currItem = ui->tblSceneries->item(i,0)->text().trimmed();
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
    // Game Mode
    if(ui->ckbGameMode->isChecked())
    {
        params << "--enable-game-mode";
    }
    else
    {
        params << "--disable-game-mode";
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
        params << "--geometry="+ui->cboWindowGeometries->currentText();
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
    // Sky Blend
    if(ui->ckbSkyBlending->isChecked())
    {
        params << "--enable-skyblend";
    }
    else
    {
        params << "--disable-skyblend";
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
        params << "--failure="+ui->cboFailures->currentText().trimmed();
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
    QString longitude = model->item(selected.at(0).row(),2)->text().trimmed();
    QString latitude = model->item(selected.at(0).row(),3)->text().trimmed();
    //QString longitude = ui->tbvAirports->item(ui->tbvAirports->selectedItems()[0]->row(),2)->text().trimmed();
    //QString latitude  = ui->tbvAirports->item(ui->tbvAirports->selectedItems()[0]->row(),3)->text().trimmed();
    if((longitude.compare("")!=0)&&(latitude.compare("")!=0))
    {
        params << "--lon="+longitude;
        params << "--lat="+latitude;
    }
    return params;
}

void MainWindow::loadSettings(bool appStart)
{
    curr_settings = new Settings(fgenv->getYFHome()+"/conf.ini");
    if(!curr_settings->isEmpty())
    {
        (curr_settings->getSound().compare(SET_TRUE)==0) ? ui->ckbSound->setChecked(true) : ui->ckbSound->setChecked(false);
        (curr_settings->getClouds().compare(SET_TRUE)==0) ? ui->ckbClouds->setChecked(true) : ui->ckbClouds->setChecked(false);
        (curr_settings->getGameMode().compare(SET_TRUE)==0) ? ui->ckbGameMode->setChecked(true) : ui->ckbGameMode->setChecked(false);
        (curr_settings->getFullScreen().compare(SET_TRUE)==0) ? ui->ckbFullScreen->setChecked(true) : ui->ckbFullScreen->setChecked(false);
        (curr_settings->getFog().compare(SET_TRUE)==0) ? ui->ckbFog->setChecked(true) : ui->ckbFog->setChecked(false);
        (curr_settings->getMeasureUnit().compare(SET_TRUE)==0) ? ui->rdbUnitMeters->setChecked(true) : ui->rdbUnitFeets->setChecked(true);
        (curr_settings->getFuelLock().compare(SET_TRUE)==0) ? ui->ckbLockFuel->setChecked(true) : ui->ckbLockFuel->setChecked(false);
        (curr_settings->getTimeLock().compare(SET_TRUE)==0) ? ui->ckbLockTime->setChecked(true) : ui->ckbLockTime->setChecked(false);
        (curr_settings->getRandomObjects().compare(SET_TRUE)==0) ? ui->ckbRandomObjects->setChecked(true) : ui->ckbRandomObjects->setChecked(false);
        (curr_settings->getAIModels().compare(SET_TRUE)==0) ? ui->ckbAIModels->setChecked(true) : ui->ckbAIModels->setChecked(false);
        (curr_settings->getAutoCoordination().compare(SET_TRUE)==0) ? ui->ckbAutoCoordination->setChecked(true) : ui->ckbAutoCoordination->setChecked(false);
        (curr_settings->getPanel().compare(SET_TRUE)==0) ? ui->ckbPanel->setChecked(true) : ui->ckbPanel->setChecked(false);
        (curr_settings->getHorizonEffect().compare(SET_TRUE)==0) ? ui->ckbHorizonEffect->setChecked(true) : ui->ckbHorizonEffect->setChecked(false);
        (curr_settings->getSkyBlending().compare(SET_TRUE)==0) ? ui->ckbSkyBlending->setChecked(true) : ui->ckbSkyBlending->setChecked(false);
        (curr_settings->getTextures().compare(SET_TRUE)==0) ? ui->ckbTextures->setChecked(true) : ui->ckbTextures->setChecked(false);
        (curr_settings->getDistanceAttenuation().compare(SET_TRUE)==0) ? ui->ckbDistanceAttenuation->setChecked(true) : ui->ckbDistanceAttenuation->setChecked(false);
        (curr_settings->getWind().compare(SET_TRUE)==0) ? ui->ckbWind->setChecked(true) : ui->ckbWind->setChecked(false);
        (curr_settings->getHUDAntiAliased().compare(SET_TRUE)==0) ? ui->ckbHudAntialias->setChecked(true) : ui->ckbHudAntialias->setChecked(false);
        // this two (hud2d and hud3d) may collide sometimes, check!!!
        (curr_settings->getHUD2D().compare(SET_TRUE)==0) ? ui->rdbHud2D->setChecked(true) : ui->rdbHud3D->setChecked(true);
        (curr_settings->getHUD3D().compare(SET_TRUE)==0) ? ui->rdbHud3D->setChecked(true) : ui->rdbHud3D->setChecked(true);
        (curr_settings->getEnhancedLighting().compare(SET_TRUE)==0) ? ui->ckbEnhancedLighting->setChecked(true) : ui->ckbEnhancedLighting->setChecked(false);
        (curr_settings->getSpecularReflections().compare(SET_TRUE)==0) ? ui->ckbSpecularReflections->setChecked(true) : ui->ckbSpecularReflections->setChecked(false);

        // missing handles to Altitude, Heading, Lat, Long, Terrasync, Aircraft

        // aircraft
        int aircraft_idx = ui->cboAircrafts->findText(curr_settings->getAircraft());
        if(aircraft_idx>=0)
        {
            ui->cboAircrafts->setCurrentIndex(aircraft_idx);
            on_cboAircrafts_currentIndexChanged(ui->cboAircrafts->itemText(aircraft_idx));
        }
        else
        {
            ui->txaLog->append("WARN: Aircraft from configuration not available");
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

        if(curr_settings->getSceneries().trimmed().compare("")!=0)
        {
            ui->tblSceneries->setRowCount(0);
            ui->tblSceneries->setColumnCount(0);
            ui->tblSceneries->clear();
            QStringList sceneryList = curr_settings->getSceneries().trimmed().split(":");
            ui->tblSceneries->setRowCount(sceneryList.count());
            ui->tblSceneries->setColumnCount(1);
            ui->tblSceneries->setShowGrid(false);
            ui->tblSceneries->horizontalHeader()->hide();
            ui->tblSceneries->verticalHeader()->hide();
            ui->tblSceneries->setColumnWidth(0,ui->tblSceneries->width()-2);
            ui->tblSceneries->setSelectionMode(QAbstractItemView::SingleSelection);
            int row = 0;
            foreach(QString item, sceneryList)
            {
                ui->tblSceneries->setItem(row,0,new QTableWidgetItem(item));
                row++;
            }

        }


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
                        break;
                    }
                }
            }

            ui->webView->reload();
        }

        ui->txaLog->append("INFO: Configuration loaded correctly");
        /*QMessageBox msgBox("Success","Configuration loaded!",QMessageBox::Information,QMessageBox::Ok,NULL,NULL,this);
        msgBox.exec();*/
    }
}

bool MainWindow::saveSettings()
{
    if(ui->tblSceneries->rowCount()>0)
    {
        QString sceneries = "";
        QString currScenery;
        for(int i=0;i<ui->tblSceneries->rowCount();i++)
        {
            currScenery = ui->tblSceneries->item(i,0)->text().trimmed();
            if(currScenery.compare(fgenv->getDefaultScenery().trimmed())!=0)
                sceneries += currScenery + ":";
        }
        sceneries.remove(sceneries.length()-1,1);
        curr_settings->setSceneries(sceneries);
    }

    if(curr_settings->storeData())
    {
        ui->txaLog->append("INFO: Configuration stored correctly");
        QMessageBox msgBox("Success","Configuration stored!",QMessageBox::Information,QMessageBox::Ok,NULL,NULL,this);
        msgBox.exec();
        return true;
    }else{
        ui->txaLog->append("WARN: Configuration NOT stored");
        QMessageBox msgBox("Failure","Configuration NOT stored!",QMessageBox::Critical,QMessageBox::Close,NULL,NULL,this);
        msgBox.exec();
    }
    return false;
}

void MainWindow::on_pbtSaveConf_clicked()
{
    saveSettings();
}

void MainWindow::on_pbtLoadConf_clicked()
{
    loadSettings();
}

QHash<QString, QStringList> MainWindow::collect_all_airports()
{
    QHash<QString, QStringList> resultAirports = fgenv->getDefaultAirportList();
    for(int i=0;i<ui->tblSceneries->rowCount();i++)
    {
        QHash<QString, QStringList> tmpAirports = fgenv->parseAirportsIndex(ui->tblSceneries->item(i,0)->text().trimmed()+"/Airports/index.txt");
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

void MainWindow::setup_airport_list()
{   
    QStandardItemModel *model = new QStandardItemModel(1,5);
    ui->tbvAirports->setModel(model);

    model->setHorizontalHeaderLabels(QStringList() << "I" << "ICAO" << "Lon" << "Lat" << "Directory");

    ui->tbvAirports->setColumnWidth(0,20);
    ui->tbvAirports->setColumnWidth(1,50);
    ui->tbvAirports->setColumnWidth(4,ui->tbvAirports->width()-80);
    ui->tbvAirports->setColumnHidden(2,false);
    ui->tbvAirports->setColumnHidden(3,false);
    ui->tbvAirports->setColumnHidden(4,true);
    ui->tbvAirports->setShowGrid(false);
    ui->tbvAirports->verticalHeader()->hide();

    ui->tbvAirports->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tbvAirports->setSelectionMode(QAbstractItemView::SingleSelection);


    AirportIdx apindex(fgenv->getAirportsCacheFilePath(), fgenv->getRunwaysCacheFilePath());
    Airport *ap;

    if(!apindex.exists())
    {
        // creating airports index cache
        QHash<QString, QStringList> airportsHash = collect_all_airports();
        QStringList allAirportDirectories;
        allAirportDirectories << fgenv->getDefaultAirportsDir();
        for(int i=0;i<ui->tblSceneries->rowCount();i++)
        {
            allAirportDirectories << ui->tblSceneries->item(i,0)->text().trimmed()+"/Airports";
        }
        int row = 0;
        model->setRowCount(airportsHash.count());
        foreach(QString key, airportsHash.keys())
        {
            foreach(QString airportDir, allAirportDirectories)
            {
                ap = new Airport(airportDir,key);
                QDir dir(ap->getAirportDirPath());
                if(dir.exists())
                {
                    model->setData(model->index(row,0), QString("X"));
                    model->setData(model->index(row,0), QBrush(Qt::green),Qt::BackgroundRole);
                    airportsHash[key][2] = QString(ap->getAirportDirPath());
                    break;
                }
            }
            if(model->item(row,0)==NULL){
                model->setData(model->index(row,0), QBrush(Qt::gray),Qt::BackgroundRole);
                model->setData(model->index(row,0), "");
            }
            model->setData(model->index(row,1), key);
            model->setData(model->index(row,2), airportsHash[key][0]);
            model->setData(model->index(row,3), airportsHash[key][1]);
            model->setData(model->index(row,4), airportsHash[key][2]);
            row++;
        }
        if(!apindex.create(airportsHash,fgenv->getAPTSource(),fgenv->getYFHome())){
            QMessageBox msgbox(QMessageBox::Critical,"Error","Can't create airport index cache\nCheck you permissions",QMessageBox::Ok);
            msgbox.exec();
        }
    }
    else{
        if(apindex.load())
        {
            int row = 0;
            model->setRowCount(apindex.count());
            QHash<QString, QStringList> cache = apindex.get();
            foreach(QString key, cache.keys())
            {
                //ap = new Airport(airportDir,key,fgenv->getAPTSource(),fgenv->getYFHome()+"/tmp_apt_dat");
                if(cache[key][2].trimmed().compare("")==0)
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
                model->setData(model->index(row,4),cache[key][2]);
                row++;
            }
        }
    }
}

void MainWindow::on_btnRefreshAirportList_clicked()
{
    setup_airport_list();
}

void MainWindow::on_ckbFilterInstalled_toggled(bool checked)
{
    QStandardItemModel *model = (QStandardItemModel *) ui->tbvAirports->model();
    if(checked)
    {
        for(int i=0;i<model->rowCount();i++)
        {
            if(model->item(i,0)->text().compare("X")!=0)
                ui->tbvAirports->hideRow(i);
        }
    }
    else
    {
        for(int i=0;i<model->rowCount();i++)
        {
            if(model->item(i,0)->text().compare("X")!=0)
                ui->tbvAirports->showRow(i);
        }
    }
}

void MainWindow::on_tbvAirports_clicked(const QModelIndex &index)
{
    QStandardItemModel *model = (QStandardItemModel *) ui->tbvAirports->model();
    QString icao = model->item(index.row(),1)->text().trimmed();
    APT_dat apdat(fgenv->getAPTSource(),fgenv->getYFHome());
    if(!apdat.retrieve_ap_details(icao))
        return;

    ap_runways = apdat.get_ap_runways(icao);

    ui->cboRunway->clear();

    bool isFirst = true;

    foreach(Runway *rw, ap_runways)
    {
        if(rw->getNumber().compare("xxx")!=0)
        {
            if(isFirst)
                currentRunway = rw;
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

    curr_settings->setLongitude(model->item(index.row(),2)->text());
    curr_settings->setLatitude(model->item(index.row(),3)->text());
    curr_settings->setAirportID(icao);
}

void MainWindow::on_ckbSound_toggled(bool checked)
{
    checked ? curr_settings->setSound(SET_TRUE) : curr_settings->setSound(SET_FALSE);
}

void MainWindow::on_ckbClouds_toggled(bool checked)
{
    checked ? curr_settings->setClouds(SET_TRUE) : curr_settings->setClouds(SET_FALSE);
}

void MainWindow::on_ckbGameMode_toggled(bool checked)
{
    checked ? curr_settings->setGameMode(SET_TRUE) : curr_settings->setGameMode(SET_FALSE);
}

void MainWindow::on_ckbFullScreen_toggled(bool checked)
{
    checked ? curr_settings->setFullScreen(SET_TRUE) : curr_settings->setFullScreen(SET_FALSE);
}

void MainWindow::on_ckbFog_toggled(bool checked)
{
    checked ? curr_settings->setFog(SET_TRUE) : curr_settings->setFog(SET_FALSE);
}

void MainWindow::on_rdbUnitMeters_toggled(bool checked)
{
    checked ? curr_settings->setMeasureUnit(SET_TRUE) : curr_settings->setMeasureUnit(SET_FALSE);
}

void MainWindow::on_ckbLockFuel_toggled(bool checked)
{
    checked ? curr_settings->setFuelLock(SET_TRUE) : curr_settings->setFuelLock(SET_FALSE);
}

void MainWindow::on_ckbLockTime_toggled(bool checked)
{
    checked ? curr_settings->setTimeLock(SET_TRUE) : curr_settings->setTimeLock(SET_FALSE);
}

void MainWindow::on_ckbRandomObjects_toggled(bool checked)
{
    checked ? curr_settings->setRandomObjects(SET_TRUE) : curr_settings->setRandomObjects(SET_FALSE);
}

void MainWindow::on_ckbAIModels_toggled(bool checked)
{
    checked ? curr_settings->setAIModels(SET_TRUE) : curr_settings->setAIModels(SET_FALSE);
}

void MainWindow::on_ckbAutoCoordination_toggled(bool checked)
{
    checked ? curr_settings->setAutoCoordination(SET_TRUE) : curr_settings->setAutoCoordination(SET_FALSE);
}

void MainWindow::on_ckbPanel_toggled(bool checked)
{
    checked ? curr_settings->setPanel(SET_TRUE) : curr_settings->setPanel(SET_FALSE);
}

void MainWindow::on_ckbHorizonEffect_toggled(bool checked)
{
    checked ? curr_settings->setHorizonEffect(SET_TRUE) : curr_settings->setHorizonEffect(SET_FALSE);
}

void MainWindow::on_ckbSkyBlending_toggled(bool checked)
{
    checked ? curr_settings->setSkyBlending(SET_TRUE) : curr_settings->setSkyBlending(SET_FALSE);
}

void MainWindow::on_ckbTextures_toggled(bool checked)
{
    checked ? curr_settings->setTextures(SET_TRUE) : curr_settings->setTextures(SET_FALSE);
}

void MainWindow::on_ckbDistanceAttenuation_toggled(bool checked)
{
    checked ? curr_settings->setDistanceAttenuation(SET_TRUE) : curr_settings->setDistanceAttenuation(SET_FALSE);
}

void MainWindow::on_ckbWind_toggled(bool checked)
{
    checked ? curr_settings->setWind(SET_TRUE) : curr_settings->setWind(SET_FALSE);
}

void MainWindow::on_ckbHudAntialias_toggled(bool checked)
{
    checked ? curr_settings->setHUDAntiAliased(SET_TRUE) : curr_settings->setHUDAntiAliased(SET_FALSE);
}

void MainWindow::on_rdbHud2D_toggled(bool checked)
{
    checked ? curr_settings->setHUD2D(SET_TRUE) : curr_settings->setHUD2D(SET_FALSE);
}

void MainWindow::on_rdbHud3D_toggled(bool checked)
{
    checked ? curr_settings->setHUD3D(SET_TRUE) : curr_settings->setHUD3D(SET_FALSE);
}

void MainWindow::on_ckbEnhancedLighting_toggled(bool checked)
{
    checked ? curr_settings->setEnhancedLighting(SET_TRUE) : curr_settings->setEnhancedLighting(SET_FALSE);
}

void MainWindow::on_ckbSpecularReflections_toggled(bool checked)
{
    checked ? curr_settings->setSpecularReflections(SET_TRUE) : curr_settings->setSpecularReflections(SET_FALSE);
}

void MainWindow::on_hzsTurbulence_valueChanged(int value)
{
    float turbulence = value / 10.0;

    curr_settings->setTurbulence(QString::number(turbulence));
}

void MainWindow::on_cboWindowGeometries_currentIndexChanged(const QString &arg1)
{
    if(!just_started)
        curr_settings->setResolution(arg1);
}

void MainWindow::on_cboFailures_currentIndexChanged(const QString &arg1)
{
    if(!just_started)
        curr_settings->setFailure(arg1);
}

void MainWindow::on_cboDayTime_currentIndexChanged(const QString &arg1)
{
    if(!just_started)
        curr_settings->setDayTime(arg1);
}

void MainWindow::on_cboSeason_currentIndexChanged(const QString &arg1)
{
    if(!just_started)
        curr_settings->setSeason(arg1);
}

void MainWindow::on_tbvAirports_doubleClicked(const QModelIndex &index)
{
    QStandardItemModel *model = (QStandardItemModel *) ui->tbvAirports->model();
    QModelIndexList modelidxlst = ui->tbvAirports->selectionModel()->selectedIndexes();

    if(modelidxlst.count()<=0)
        return;

    QString longitude = model->item(modelidxlst.at(0).row(),2)->text().trimmed();
    QString latitude  = model->item(modelidxlst.at(0).row(),3)->text().trimmed();
    QString head = QString::number(ui->dialHeading->value());

    update_latlonhead(latitude, longitude, head);
    ui->tabOpts->setCurrentWidget(ui->tabBasic);
}

void MainWindow::update_latlonhead(QString lat, QString lon, QString heading)
{
    adjust_heading_value((int)ceil(heading.toFloat()));
    ui->webView->page()->mainFrame()->evaluateJavaScript("aggiornaCentroMappa(" + lon + "," + lat + ",true);");
    ui->webView->page()->mainFrame()->evaluateJavaScript("aggiornaLonLat(" + lon + "," + lat + ", true," + heading + ");");

}

void MainWindow::adjust_heading_value(int head)
{
    if((head >= 0) && (head < 180))
    {
        ui->dialHeading->setValue(head + 180);
    }
    else if((head >= 180) && (head < 360))
    {
        ui->dialHeading->setValue(head - 180);
    }
}

int MainWindow::convert_dialhead_to_azimuth(int value)
{
    if((value >= 0) && (value < 180))
    {
        return value + 180;
    }
    else if ((value >= 180) && (value < 360))
    {
        return value - 180;
    }
    return -1;
}

void MainWindow::on_dialHeading_valueChanged(int value)
{
    QStandardItemModel *model = (QStandardItemModel *) ui->tbvAirports->model();
    QModelIndexList modelidxlst = ui->tbvAirports->selectionModel()->selectedIndexes();

    if(modelidxlst.count()<=0)
        return;
    QString longitude = model->item(modelidxlst.at(0).row(),2)->text().trimmed();
    QString latitude  = model->item(modelidxlst.at(0).row(),3)->text().trimmed();

    update_latlonhead(latitude, longitude, QString::number(convert_dialhead_to_azimuth(value)));
}

/*
  sCoords = wbvMap.Eval("document.getElementById('yaflight').innerText;")
  If Trim(sCoords) = "" Then Return
  aCoords = Split(sCoords, ",", Null, True)
  txtLongitude.Text = Trim(aCoords[0])
  txtLatitude.Text = Trim(aCoords[1])
  */

void MainWindow::on_btnGoToMap_clicked()
{
    QString longitude = currentRunway->getLongitude();
    QString latitude = currentRunway->getLatitude();
    QString head = currentRunway->getHeading();

    ui->dialHeading->setValue(head.toInt());

    longitude = QString::number(longitude.toDouble());
    latitude = QString::number(latitude.toDouble());

    curr_settings->setLongitude(longitude);
    curr_settings->setLatitude(latitude);
    curr_settings->setHeading(head);

    update_latlonhead(latitude, longitude, head);
    ui->tabOpts->setCurrentWidget(ui->tabBasic);
}

void MainWindow::on_btnAboutQt_clicked()
{
    QMessageBox msgBox;
    msgBox.aboutQt(this, "About Qt");
}

void MainWindow::on_cboRunway_currentIndexChanged(const QString &arg1)
{
    foreach(Runway *rw, ap_runways)
    {
        if(rw->getNumber().compare(arg1)==0)
        {
            currentRunway = rw;
            break;
        }
    }
    if(currentRunway!=NULL)
        curr_settings->setRunway(currentRunway->getNumber());
}

void MainWindow::on_btnRunwayInfo_clicked()
{
    QMessageBox msgbox;
    msgbox.setText(currentRunway->getNumber() + " " + currentRunway->getHeading());
    msgbox.exec();
}
