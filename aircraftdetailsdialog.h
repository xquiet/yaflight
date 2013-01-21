/**
    Copyright (C) 2012-2013  Matteo Pasotti <matteo.pasotti@gmail.com>

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

#ifndef DLGAIRCRAFTDETAILS_H
#define DLGAIRCRAFTDETAILS_H

#include <QDialog>
#include <QStringList>
#include <QStandardItemModel>

#include "fgenvironment.h"
#include "imagepreview.h"

namespace Ui {
class AircraftDetailsDialog;
}

class AircraftDetailsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AircraftDetailsDialog(QStringList details,
                                FGEnvironment *fgenv,
                                QWidget *parent = 0);
    ~AircraftDetailsDialog();
    
private:
    Ui::AircraftDetailsDialog *ui;
};

#endif // DLGAIRCRAFTDETAILS_H
