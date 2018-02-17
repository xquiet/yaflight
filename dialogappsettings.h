/**
    Copyright (C) 2012-2016  Matteo Pasotti <matteo.pasotti@gmail.com>

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

#ifndef DIALOGAPPSETTINGS_H
#define DIALOGAPPSETTINGS_H

#include <QDialog>
#include <QObject>
#include <QMessageBox>
#include <QFileDialog>

#include "appsettings.h"
#include "fgenvironment.h"

namespace Ui {
class DialogAppSettings;
}

class DialogAppSettings : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogAppSettings(QWidget *parent = 0);
    ~DialogAppSettings();
    
private:
    Ui::DialogAppSettings *ui;
    FGEnvironment *fgenv;
    void setup_default_paths();

private slots:
    void closeEvent(QCloseEvent *);
    void on_pbtSearchFGData_clicked();
    void on_pbtSearchFGFSBin_clicked();
    void on_pbtSetupDefaultFGSettings_clicked();
};

#endif // DIALOGAPPSETTINGS_H
