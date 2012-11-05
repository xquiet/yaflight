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

#include "runwaydetailsdialog.h"
#include "ui_runwaydetailsdialog.h"

RunwayDetailsDialog::RunwayDetailsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RunwayDetailsDialog)
{
    ui->setupUi(this);

    this->setFixedSize(330,400);

    // view
    ui->tbvRunwayDetails->setGeometry(5,5,this->width()-10, this->height()-10-ui->buttonBox->height());
    ui->tbvRunwayDetails->setShowGrid(false);
    ui->tbvRunwayDetails->horizontalHeader()->setHidden(true);
    ui->tbvRunwayDetails->verticalHeader()->setHidden(true);

    ui->buttonBox->setGeometry(5,ui->tbvRunwayDetails->height()+ui->tbvRunwayDetails->y()+2,
                                       ui->tbvRunwayDetails->width(),
                                       ui->buttonBox->height());

    connect(ui->buttonBox->buttons().at(0), SIGNAL(clicked()), this, SLOT(close()));
}

RunwayDetailsDialog::~RunwayDetailsDialog()
{
    delete ui;
}

void RunwayDetailsDialog::setCurrentRunway(Runway *r)
{
    currentRunway = r;

    // model
    QStandardItemModel *model = (QStandardItemModel *) ui->tbvRunwayDetails->model();
    if(!model)
    {
        model = new QStandardItemModel();
        ui->tbvRunwayDetails->setModel(model);
    }
    model->setRowCount(6);
    model->setColumnCount(2);
    model->setItem(0,0, new QStandardItem(tr("Number")));
    model->setItem(0,1, new QStandardItem(currentRunway->getNumber()));
    model->setItem(1,0, new QStandardItem(tr("Longitude")));
    model->setItem(1,1, new QStandardItem(currentRunway->getLongitude()));
    model->setItem(2,0, new QStandardItem(tr("Latitude")));
    model->setItem(2,1, new QStandardItem(currentRunway->getLatitude()));
    model->setItem(3,0, new QStandardItem(tr("Heading")));
    model->setItem(3,1, new QStandardItem(currentRunway->getHeading()));
    model->setItem(4,0, new QStandardItem(tr("Shoulder")));
    model->setItem(4,1, new QStandardItem(ShoulderCode::decode(currentRunway->getShoulderCode())));
    model->setItem(5,0, new QStandardItem(tr("Surface")));
    model->setItem(5,1, new QStandardItem(SurfaceCode::decode(currentRunway->getSurfaceCode())));
}
