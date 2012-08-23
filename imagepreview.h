#ifndef IMAGEPREVIEW_H
#define IMAGEPREVIEW_H

#include <QPixmap>
#include <QPainter>
#include <QFile>
//#include <QByteArray>
#include <math.h>

class ImagePreview
{
private:
    QPixmap *pixmap;

public:
    ImagePreview(QString srcImagePath, int lblWidth, int lblHeight);
    QPixmap getPixmap();
};

#endif // IMAGEPREVIEW_H
