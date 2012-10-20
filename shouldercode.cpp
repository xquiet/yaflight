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

#include "shouldercode.h"

ShoulderCode::ShoulderCode()
{
}

QString ShoulderCode::decode(QString shoulderCode)
{
    int code = shoulderCode.toInt();
    QString result = "";
    switch(code)
    {
        case 0:
            result = "No shoulder";
            break;
        case 1:
            result = "Asphalt";
            break;
        case 2:
            result = "Concrete";
            break;
    }
    return result;
}
