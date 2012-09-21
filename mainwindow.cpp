#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent,Qt::FramelessWindowHint),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    // triggering the event to cause aircraft preview refresh
    on_cboAircrafts_currentIndexChanged(ui->cboAircrafts->itemText(0));

    FGEnvironment *fgenv = new FGEnvironment();
    ui->txaLog->append("OS: " + fgenv->detectOS());
    ui->txaLog->append("FG version: " + fgenv->detectFGVersion());

    configuration conf(fgenv->getYFHome()+"/conf.ini");
    if(!conf.exists())
    {
        if(!conf.create(fgenv->getYFHome()+"/conf.ini"))
            qFatal("Can't create configuration file");
    }else{
        conf.parseFile();
        //qDebug("%s", conf.get("main", "AIRCRAFT").toStdString().data());
    }
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
    FGEnvironment *fgenv = new FGEnvironment();
    QDir aircraftsDir(fgenv->getAircraftDir());
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
    msgbox.about(this,"About","yaflight4w - yaflight for windows\n(C) 2011-2012 by Matteo Pasotti");
}

void MainWindow::on_pbtLaunch_clicked()
{
    FGEnvironment *fgenv = new FGEnvironment();
    QStringList params = collectLaunchSettings(fgenv);
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
}

void MainWindow::drawThumbnail(QString dir)
{
    FGEnvironment *fgenv = new FGEnvironment();
    QString thumbFilePath = fgenv->getAircraftDir()+"/"+dir+"/thumbnail.jpg";
    if(!QFile::exists(thumbFilePath))
    {
        QStringList details = fgenv->getAircraftDetails(ui->cboAircrafts->currentText(),
                                                        hashOfAircrafts.value(ui->cboAircrafts->currentText())
                                                        );
        Aircraft ac(details);
        thumbFilePath = fgenv->getRootPath()+"/"+ac.getSplashTexture();
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
    FGEnvironment *fgenv = new FGEnvironment();
    QString message;
    QStringList details = fgenv->getAircraftDetails(
                ui->cboAircrafts->currentText(),
                hashOfAircrafts.value(ui->cboAircrafts->currentText())
                );

    dlgAircraftDetails dlg(details, this);
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

QStringList MainWindow::collectLaunchSettings(FGEnvironment *fgenv)
{
    QStringList params;

    // static arguments
    params << "--verbose" << "--fg-root="+fgenv->getRootPath() << "--fg-scenery=/usr/share/games/flightgear/Scenery" << "--aircraft="+ui->cboAircrafts->currentText();

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
    if(ui->ckbSpecularHighlight->isChecked())
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
    return params;
}
