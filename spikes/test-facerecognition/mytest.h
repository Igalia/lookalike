#ifndef MYTEST_H
#define MYTEST_H

#include <QImage>
#include <xqfacethumbnailer.h>
#include <xqfacedatabase.h>
#include <xqfacerecognizer.h>

class XQFaceDatabase;
class XQFaceRecognizer;
class XQFaceThumbnailer;

class MyTest : public XQFaceThumbnailer
{
    Q_OBJECT
public:
    MyTest();
    void createThumbnail(const QString &sourceId, XQFaceThumbnailer::ThumbnailSizeHint sizeHint, const QSize &preferredSize);
    void createFaceThumbnail(const QString &sourceId, const QSize &sourceSize, const QRect &thumbnailRect);
    void thumbnailNotUsed(const QStringList &thumbnailPaths);

public Q_SLOTS:
    void onFacesDetected(const QString &sourceId, const QImage image, const QSize& imageSize, QList<QRect> faceRects);
    void onFacesRecognized(const QString &sourceId, const QImage &image, QList<XQFaceRegion> faceRegions);

public:
    XQFaceDatabase *db;
    XQFaceRecognizer *recognizer;
};

#endif // MYTEST_H
