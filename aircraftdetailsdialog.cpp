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

#include "aircraftdetailsdialog.h"
#include "ui_aircraftdetailsdialog.h"

AircraftDetailsDialog::AircraftDetailsDialog(QStringList details, FGEnvironment *fgenv, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AircraftDetailsDialog)
{
    ui->setupUi(this);

    this->setFixedSize(590,400);

    // view
    ui->tbvDetails->setGeometry(5,5,
                                this->width()-10,
                                this->height() - ui->lblAircraft->height() - ui->buttonBox->height() - 40);
    ui->tbvDetails->setShowGrid(false);
    ui->tbvDetails->horizontalHeader()->setHidden(true);
    ui->tbvDetails->verticalHeader()->setHidden(true);
    ui->tbvDetails->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // model
    QStandardItemModel *model = (QStandardItemModel *) ui->tbvDetails->model();
    if(!model)
    {
        model = new QStandardItemModel();
        ui->tbvDetails->setModel(model);
    }
    model->setRowCount(details.count()-1);
    model->setColumnCount(2);

    int row=0;
    foreach(QString item, details)
    {
        QStringList couple = item.split(":");
        if(couple.value(0).trimmed().compare("splash-texture")!=0)
        {
            model->setItem(row,0, new QStandardItem(couple.value(0).toUpper()));
            model->setItem(row,1, new QStandardItem(couple.value(1)));
            row++;
        }
        else
        {
            if(couple.value(1).trimmed().endsWith(".rgb",Qt::CaseInsensitive))
                return;
            ImagePreview iprvw(fgenv->getRootPath()+"/"+couple.value(1).trimmed(),
                               ui->lblAircraft->width(),
                               ui->lblAircraft->height());
            ui->lblAircraft->setScaledContents(true);
            ui->lblAircraft->setPixmap(iprvw.getPixmap());
        }
    }

    ui->tbvDetails->setColumnWidth(0,250);
    ui->tbvDetails->setColumnWidth(1,300);
}

AircraftDetailsDialog::~AircraftDetailsDialog()
{
    delete ui;
}
