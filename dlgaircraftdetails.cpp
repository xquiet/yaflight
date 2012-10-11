#include "dlgaircraftdetails.h"
#include "ui_dlgaircraftdetails.h"

dlgAircraftDetails::dlgAircraftDetails(QStringList details, FGEnvironment *fgenv, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgAircraftDetails)
{
    ui->setupUi(this);
    ui->tbvDetails->setRowCount(details.size());
    ui->tbvDetails->setColumnCount(2);
    ui->tbvDetails->horizontalHeader()->hide();
    ui->tbvDetails->verticalHeader()->hide();
    ui->tbvDetails->setColumnWidth(0,150);
    ui->tbvDetails->setColumnWidth(1,400);
    int row=0;
    foreach(QString item, details)
    {
        QStringList couple = item.split(":");
        if(couple.value(0).trimmed().compare("splash-texture")!=0)
        {
            ui->tbvDetails->setItem(row,0,new QTableWidgetItem(couple.value(0).toUpper()));
            ui->tbvDetails->setItem(row,1,new QTableWidgetItem(couple.value(1)));
            row++;
        }
        else
        {
            if(couple.value(1).trimmed().endsWith(".rgb",Qt::CaseInsensitive))
                return;
            ImagePreview iprvw(fgenv->getRootPath()+"/"+couple.value(1).trimmed(),ui->lblAircraft->width(),ui->lblAircraft->height());
            ui->lblAircraft->setScaledContents(true);
            ui->lblAircraft->setPixmap(iprvw.getPixmap());
        }
    }
}

dlgAircraftDetails::~dlgAircraftDetails()
{
    delete ui;
}
