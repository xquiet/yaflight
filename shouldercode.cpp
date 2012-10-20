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
