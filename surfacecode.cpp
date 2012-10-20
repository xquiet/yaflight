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

#include "surfacecode.h"

SurfaceCode::SurfaceCode()
{
}

QString SurfaceCode::decode(QString surfaceCode)
{
    int code = surfaceCode.toInt();
    QString result = "";
    switch(code)
    {
        case 1:
            result = "Asphalt";
            break;
        case 2:
            result = "Concrete";
            break;
        case 3:
            result = "Turf/Grass";
            break;
        case 4:
            result = "Dirt";
            break;
        case 5:
            result = "Gravel";
            break;
        case 6:
            result = "Asphalt Helipad";
            break;
        case 7:
            result ="Concrete Helipad";
            break;
        case 8:
            result = "Turf Helipad";
            break;
        case 9:
            result = "Dirt Helipad";
            break;
        case 10:
            result = "Asphalt taxiway with yellow hold lines";
            break;
        case 11:
            result = "Concrete taxiway with yellow hold lines";
            break;
        case 12:
            result = "Dry lakebed runway";
            break;
        case 13:
            result = "Water runway";
            break;
    }
    return result;
}
