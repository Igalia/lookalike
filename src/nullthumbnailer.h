#ifndef NULLTHUMBNAILER_H
#define NULLTHUMBNAILER_H

#include "xqfacethumbnailer.h"

class NullThumbnailer : public XQFaceThumbnailer
{
    Q_OBJECT
 public:
    explicit NullThumbnailer(QObject *parent = 0);
    void createThumbnail(const QString &sourceId, XQFaceThumbnailer::ThumbnailSizeHint sizeHint, const QSize &preferredSize);
    void createFaceThumbnail(const QString &sourceId, const QSize &sourceSize, const QRect &thumbnailRect);
    void thumbnailNotUsed(const QStringList &thumbnailPaths);
};


#endif // NULLTHUMBNAILER_H
