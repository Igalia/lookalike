/*
 * This file is part of LookALike
 *
 * Copyright (C) 2012 Igalia S.L.
 *
 * Contact: agomez@igalia.com
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#include "mytest.h"
#include <QDebug>

MyTest::MyTest()
{
    db = new XQFaceDatabase(*this, "/tmp/myface.db");
    recognizer = new XQFaceRecognizer(*db);
}

void MyTest::createFaceThumbnail(const QString &sourceId, const QSize &sourceSize, const QRect &thumbnailRect)
{
    Q_UNUSED(sourceId);
    Q_UNUSED(sourceSize);
    Q_UNUSED(thumbnailRect);
}

void MyTest::createThumbnail(const QString &sourceId, XQFaceThumbnailer::ThumbnailSizeHint sizeHint, const QSize &preferredSize)
{
    Q_UNUSED(sourceId);
    Q_UNUSED(sizeHint);
    Q_UNUSED(preferredSize);
}

void MyTest::thumbnailNotUsed(const QStringList &thumbnailPaths)
{
    Q_UNUSED(thumbnailPaths);
}

void MyTest::onFacesDetected(const QString &sourceId, const QImage image, const QSize &imageSize, QList<QRect> faceRects)
{
    int i = faceRects.size();
    qDebug() << i;
}

void MyTest::onFacesRecognized(const QString &sourceId, const QImage &image, QList<XQFaceRegion> faceRegions)
{
    int i = faceRegions.size();
    qDebug() << i;
    XQFaceRegion region = faceRegions.at(0);
    //region.setFaceId("Jota");
    qDebug() << region.isUnknown();
    qDebug() << region.isConfirmed();
    i = region.faceIds().size();
    qDebug() << i;
    QString  s = region.faceIds().at(0);
    qDebug() << s;
}
