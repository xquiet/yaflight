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

#ifndef CUTEEXPANDER_H
#define CUTEEXPANDER_H

#include <QWidget>
#include <QObject>
#include <QObjectList>

namespace Ui{
class CuteExpander;
}

class CuteExpander : public QWidget
{
    Q_OBJECT
public:
    explicit CuteExpander(QWidget *parent = 0);

    void setText(QString text);
    void expand();
    void unexpand();
    
signals:
    void expanded();
    void unexpanded();

public slots:

private slots:
    void on_btnToggle_clicked();

private:
    Ui::CuteExpander *ui;
};

#endif // CUTEEXPANDER_H
