#ifndef SHOULDERCODE_H
#define SHOULDERCODE_H

#include <QString>

class ShoulderCode
{
public:
    ShoulderCode();
    static QString decode(QString shoulderCode);
};

#endif // SHOULDERCODE_H
