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

#include "cuteexpander.h"
#include "ui_cuteexpander.h"

CuteExpander::CuteExpander(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CuteExpander)
{
    ui->setupUi(this);
    /*QObjectList figli = this->children();
    int lastY = 26;
    foreach(QObject *figlio, figli)
    {
        QWidget *wdgFiglio = (QWidget*) figlio;
        wdgFiglio->setParent(this);
        wdgFiglio->setGeometry(1,lastY,width(),height());
        lastY += wdgFiglio->height()+2;
    }*/

    ui->btnToggle->setIcon(QIcon(":/icons/icons/arrowUp.png"));
    ui->btnToggle->setIconSize(QSize(16,16));
}

void CuteExpander::on_btnToggle_clicked()
{
    if(this->height() == ui->btnToggle->height()+1)
    {
        expand();
    }
    else
    {
        unexpand();
    }
}

void CuteExpander::setText(QString text)
{
    ui->btnToggle->setText(text);
}

void CuteExpander::expand()
{
    this->setGeometry(this->x(),this->y(),this->width(),this->maximumHeight());
    ui->btnToggle->setIcon(QIcon(":/icons/icons/arrowUp.png"));
    emit expanded();
}

void CuteExpander::unexpand()
{
    this->setGeometry(this->x(),this->y(),this->width(),ui->btnToggle->height()+1);
    ui->btnToggle->setIcon(QIcon(":/icons/icons/arrowDown.png"));
    emit unexpanded();
}
