/**
    Copyright (C) 2012-2020 Matteo Pasotti <matteo.pasotti@gmail.com>

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

#ifndef RUNWAYDETAILSDIALOG_H
#define RUNWAYDETAILSDIALOG_H

#include <QDialog>
#include <QObject>
#include <QStandardItemModel>
#include <QAbstractButton>

#include "runway.h"
#include "shouldercode.h"
#include "surfacecode.h"

namespace Ui {
class RunwayDetailsDialog;
}

class RunwayDetailsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit RunwayDetailsDialog(QWidget *parent = 0);
    ~RunwayDetailsDialog();
    void setCurrentRunway(Runway *r);

private:
    Ui::RunwayDetailsDialog *ui;
    Runway *currentRunway;
};

#endif // RUNWAYDETAILSDIALOG_H
