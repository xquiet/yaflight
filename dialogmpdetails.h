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

#ifndef DIALOGMPDETAILS_H
#define DIALOGMPDETAILS_H

#include <QDialog>
#include <QObject>

namespace Ui {
class DialogMPDetails;
}

class DialogMPDetails : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogMPDetails(QWidget *parent = 0);
    ~DialogMPDetails();

    QString getIn();
    QString getOut();

    void setIn(int pIn);
    void setOut(int pOut);
    
private slots:
    void closeEvent(QCloseEvent *);
    void showEvent(QShowEvent *);

private:
    Ui::DialogMPDetails *ui;
    int portIn,
        portOut;

};

#endif // DIALOGMPDETAILS_H
