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
