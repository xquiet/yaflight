#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    refreshListOfAircrafts();

    /*
    QHashIterator<QString, QString> item(hashOfAircrafts);
    while (item.hasNext()) {
        item.next();
        listOfAircrafts << item.key();
    }
    */

    QStringList listOfAircrafts = hashOfAircrafts.keys();
    listOfAircrafts.sort();
    ui->cboAircrafts->addItems(listOfAircrafts);

    on_cboAircrafts_currentIndexChanged(ui->cboAircrafts->itemText(0));

    /*
    procFGFS = new QProcess();
    procFGFS->setProcessChannelMode(QProcess::MergedChannels);
    procFGFS->start("/usr/bin/fgfs",QStringList() << "--show-aircraft", QProcess::ReadOnly);
    //if(!pls->waitForStarted())
    //    return false;
    ui->txaLog->append("Retrieving aircrafts...");
    connect(procFGFS,SIGNAL(readyRead()),this,SLOT(readAircrafts()));
    connect(procFGFS,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(procReadAircraftsFinished(int, QProcess::ExitStatus)));
    */
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

void MainWindow::on_pbtExit_clicked()
{
    this->close();
}

void MainWindow::on_pbtAbout_clicked()
{
    QMessageBox msgbox;
    msgbox.about(this,"About","yaflight4w - yaflight for windows\n(C) 2011-2012 by Matteo Pasotti");
}

void MainWindow::on_pbtLaunch_clicked()
{

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
    ui->cboAircrafts->addItems(aircrafts);
    qDebug("%s (%d)", QString(exitStatus).toStdString().c_str(), exitCode);
}

void MainWindow::on_cboAircrafts_currentIndexChanged(const QString &arg1)
{
    drawThumbnail(hashOfAircrafts.value(arg1.trimmed()));
}

void MainWindow::drawThumbnail(QString dir)
{
    float ratio = 0.;

    float ratioW = 0.;
    float ratioH = 0.;

    int reducedWidth = 0, reducedHeight = 0;
    float newX = 0.;

    QImage srcImage("/usr/share/games/flightgear/Aircraft/"+dir+"/thumbnail.jpg");
    QImage dstImage;
    QPainter *painter;

    if(srcImage.isNull())
    {
        qDebug("srcImage is NULL");
        return;
    }

    ratioW = (float)ui->lblAircraftPreview->width() / (float)srcImage.width();
    ratioH = (float)ui->lblAircraftPreview->height() / (float)srcImage.height();

    if( ratioW < ratioH )
    {
        ratio = ratioW;
    }
    else
    {
        ratio = ratioH;
    }

    reducedWidth = floor(ratio * srcImage.width());
    reducedHeight = floor(ratio * srcImage.height());

    // spostamento laterale - centering the image
    newX = (ui->lblAircraftPreview->width() / 2) - ((reducedWidth) / 2);

    dstImage = srcImage.scaled(
                reducedWidth,
                reducedHeight,
                Qt::KeepAspectRatio,
                Qt::FastTransformation
                );

    QPixmap pixmap(ui->lblAircraftPreview->size());

    painter = new QPainter();

    painter->begin(&pixmap);
    painter->fillRect(pixmap.rect(),Qt::black);
    painter->drawImage(newX,0,dstImage);
    painter->end();

    ui->lblAircraftPreview->setPixmap(pixmap);
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
