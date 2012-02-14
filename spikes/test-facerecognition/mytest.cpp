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
