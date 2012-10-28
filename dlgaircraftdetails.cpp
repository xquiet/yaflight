/**
    Copyright (C) 2012  Matteo Pasotti <matteo.pasotti@gmail.com>

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

#include "dlgaircraftdetails.h"
#include "ui_dlgaircraftdetails.h"

dlgAircraftDetails::dlgAircraftDetails(QStringList details, FGEnvironment *fgenv, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgAircraftDetails)
{
    ui->setupUi(this);

    this->setFixedSize(590,400);

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
