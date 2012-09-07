#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //this->setFixedSize(500,400);
    _mainWindowMinHeight = 300;
    _mainWindowMaxHeight = 700;
    setMinimumHeight(_mainWindowMinHeight);
    setMaximumHeight(_mainWindowMaxHeight);

    ui->expanderLog->setText("Log");
    ui->expanderMap->setText("Map");

    ui->expanderLog->setMinimumHeight(25); // cutexpander button height
    ui->expanderMap->setMinimumHeight(25); // cutexpander button height
    ui->expanderLog->setMaximumHeight(151);
    ui->expanderMap->setMaximumHeight(200);


    ui->txaLog->setGeometry(0,26,ui->expanderLog->width()-2,ui->expanderLog->height()-26);
    ui->webView->setGeometry(0,26,ui->expanderMap->width()-2,ui->expanderMap->height());

    // connecting expanders signals to local handlers!
    connect(ui->expanderLog,SIGNAL(expanded()),this,SLOT(exLogExpanded()));
    connect(ui->expanderLog,SIGNAL(unexpanded()),this,SLOT(exLogUnexpanded()));
    connect(ui->expanderMap,SIGNAL(expanded()),this,SLOT(exMapExpanded()));
    connect(ui->expanderMap,SIGNAL(unexpanded()),this,SLOT(exMapUnexpanded()));

    ui->expanderLog->unexpand();
    ui->expanderMap->unexpand();


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

    QStringList listOfAircrafts = hashOfAircrafts.keys();
    listOfAircrafts.sort();
    ui->cboAircrafts->addItems(listOfAircrafts);

    // triggering the event to cause aircraft preview refresh
    on_cboAircrafts_currentIndexChanged(ui->cboAircrafts->itemText(0));
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
    procFGFS = new QProcess();
    procFGFS->setProcessChannelMode(QProcess::MergedChannels);
    procFGFS->start(fgenv->getFgfsBinPath(),QStringList() << "--show-aircraft", QProcess::ReadOnly);
    //if(!pls->waitForStarted())
    //    return false;
    ui->txaLog->append("Retrieving aircrafts...");
    connect(procFGFS,SIGNAL(readyRead()),this,SLOT(readAircrafts()));
    connect(procFGFS,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(procReadAircraftsFinished(int, QProcess::ExitStatus)));
}

void MainWindow::readAircrafts()
{
    QByteArray bytes = procFGFS->readAll();
    QStringList strLines = QString(bytes).split("\n");
    QStringList cols;
    foreach (QString line, strLines){
        line = line.trimmed();
        if(line.compare("Available aircraft:")!=0){
            cols = line.split(" ",QString::SkipEmptyParts,Qt::CaseInsensitive);
            if (cols.length()>0)
                aircrafts.append(cols[0].trimmed());
        }
    }
}

void MainWindow::procReadAircraftsFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    ui->txaLog->append("Retrieving aircrafts: DONE");
    //ui->cboAircrafts->addItems(aircrafts);
    qDebug("%s (%d)", QString(exitStatus).toStdString().c_str(), exitCode);
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
        QStringList details = fgenv->getAircraftDetails(ui->cboAircrafts->currentText(), fgenv->getAircraftDir());
        foreach(QString item, details)
        {
            QStringList couple = item.split(":");
            if(couple.value(0).trimmed().compare("splash-texture")==0)
            {
                thumbFilePath = fgenv->getRootPath()+"/"+couple.value(1).trimmed();
                break;
            }
        }
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

void MainWindow::exLogExpanded()
{
    ui->expanderMap->setGeometry(ui->expanderMap->x(),
                                 ui->expanderLog->y()+ui->expanderLog->height()+2,
                                 ui->expanderMap->width(),
                                 ui->expanderMap->height());
    this->setGeometry(this->x(),
                      this->y(),
                      this->width(),
                      ui->expanderLog->y()+ui->expanderMap->height()+ui->expanderLog->height()+2);
}

void MainWindow::exMapExpanded()
{
    this->setGeometry(this->x(),
                      this->y(),
                      this->width(),
                      ui->expanderMap->y()+ui->expanderMap->height()+2);
    //qDebug("%d, %d",ui->expanderLog->height(), ui->expanderMap->height());
}

void MainWindow::exLogUnexpanded()
{
    ui->expanderMap->setGeometry(ui->expanderMap->x(),
                                 ui->expanderLog->y()+ui->expanderLog->height()+2,
                                 ui->expanderMap->width(),
                                 ui->expanderMap->height());
    this->setGeometry(this->x(),
                      this->y(),
                      this->width(),
                      ui->expanderLog->y()+ui->expanderMap->height()+ui->expanderLog->height()+2);
}

void MainWindow::exMapUnexpanded()
{
    this->setGeometry(this->x(),
                      this->y(),
                      this->width(),
                      ui->expanderMap->y()+ui->expanderMap->height()+2);
}
