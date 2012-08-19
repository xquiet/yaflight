#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    procFGFS = new QProcess();
    procFGFS->setProcessChannelMode(QProcess::MergedChannels);
    procFGFS->start("/usr/bin/fgfs",QStringList() << "--show-aircraft", QProcess::ReadOnly);
    //if(!pls->waitForStarted())
    //    return false;
    ui->txaLog->append("Retrieving aircrafts...");
    connect(procFGFS,SIGNAL(readyRead()),this,SLOT(readAircrafts()));
    connect(procFGFS,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(procReadAircraftsFinished(int, QProcess::ExitStatus)));

    ui->lblAircraftPreview->setBackgroundRole(QPalette::Base);
    //ui->lblAircraftPreview->setScaledContents(true);

    QImage image(ui->lblAircraftPreview->width(),ui->lblAircraftPreview->height(),QImage::Format_RGB32);
    image.fill(Qt::black);
    ui->lblAircraftPreview->setPixmap(QPixmap::fromImage(image));
}

MainWindow::~MainWindow()
{
    delete ui;
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
    float ratio = 0.;

    float ratioW = 0.;
    float ratioH = 0.;

    int reducedWidth = 0, reducedHeight = 0;
    float newX = 0.;

    qDebug("%s", arg1.toStdString().c_str());

    QImage srcImage("/usr/share/games/flightgear/Aircraft/"+arg1+"/thumbnail.jpg");
    QImage dstImage;
    QPainter *painter;

    if(srcImage.isNull())
    {
        qDebug("srcImage is NULL");
        return;
    }

    //qDebug("W: %d\nH: %d", srcImage.width(),srcImage.height());
    //qDebug("LBLW: %d\nLBLH: %d", ui->lblAircraftPreview->width(),ui->lblAircraftPreview->height());

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

    qDebug("RW: %d\nRH: %d", reducedWidth, reducedHeight);

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
