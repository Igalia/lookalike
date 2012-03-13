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
