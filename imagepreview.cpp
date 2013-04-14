#include "imagepreview.h"

ImagePreview::ImagePreview(QString srcImagePath, int lblWidth, int lblHeight)
{
    float ratio = 0.;

    float ratioW = 0.;
    float ratioH = 0.;

    int reducedWidth = 0, reducedHeight = 0;
    float newX = 0.;

    QImage srcImage;

    if(srcImagePath.endsWith(".rgb"))
    {
        QFile raw(srcImagePath);
        QByteArray data = raw.readAll();
        srcImage = QImage::fromData(data);
    }
    else
    {
        srcImage = QImage(srcImagePath);
    }

    QImage dstImage;

    if(srcImage.isNull())
    {
        qDebug("srcImage is NULL");
        return;
    }

    ratioW = (float)lblWidth / (float)srcImage.width();
    ratioH = (float)lblHeight / (float)srcImage.height();

    if( ratioW < ratioH )
    {
        ratio = ratioW;
    }
    else
    {
        ratio = ratioH;
    }

    reducedWidth = floor(ratio * srcImage.width());
    reducedHeight = floor(ratio * srcImage.height());

    // spostamento laterale - centering the image
    newX = (lblWidth / 2) - ((reducedWidth) / 2);

    dstImage = srcImage.scaled(
                reducedWidth,
                reducedHeight,
                Qt::KeepAspectRatio,
                Qt::FastTransformation
                );

    pixmap = new QPixmap(lblWidth,lblHeight);

    QPainter *painter = new QPainter();

    painter->begin(pixmap);
    painter->fillRect(pixmap->rect(),Qt::black);
    painter->drawImage(newX,0,dstImage);
    painter->end();
    delete painter;
}

QPixmap ImagePreview::getPixmap()
{
    return *pixmap;
}
