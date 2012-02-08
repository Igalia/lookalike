#include "nullthumbnailer.h"

NullThumbnailer::NullThumbnailer(QObject *parent):
    XQFaceThumbnailer(parent)
{
}

void NullThumbnailer::createThumbnail(const QString &sourceId, XQFaceThumbnailer::ThumbnailSizeHint sizeHint, const QSize &preferredSize)
{
    Q_UNUSED(sourceId);
    Q_UNUSED(sizeHint);
    Q_UNUSED(preferredSize);
}

void NullThumbnailer::createFaceThumbnail(const QString &sourceId, const QSize &sourceSize, const QRect &thumbnailRect)
{
    Q_UNUSED(sourceId);
    Q_UNUSED(sourceSize);
    Q_UNUSED(thumbnailRect);
}

void NullThumbnailer::thumbnailNotUsed(const QStringList &thumbnailPaths)
{
    Q_UNUSED(thumbnailPaths)
}
