#ifndef MMAPBRIDGE_H
#define MMAPBRIDGE_H

#include <QString>

class mmapbridge
{
public:
    mmapbridge();
    static QString getBaseUrl();

private:
    static QString baseUrl;
};

#endif // MMAPBRIDGE_H
