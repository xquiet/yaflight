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
}

QPixmap ImagePreview::getPixmap()
{
    return *pixmap;
}
