/*
 * This file is part of LookAlike
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

#ifndef TRACKERCONTENTPROVIDER_H
#define TRACKERCONTENTPROVIDER_H

#include "gallerycontentprovider.h"

class QSparqlConnection;
class TrackerContentProviderPrivate;

class TrackerContentProvider : public GalleryContentProvider
{
    Q_OBJECT
public:
    enum ContentType {
        AllImages,
        ListImages,
        FilterImagesNoFace,
        WithContact,
    };

    TrackerContentProvider(QSparqlConnection *connection, QObject *parent = 0);
    QAbstractItemModel *model() const;
    ContentType contentType();
    virtual bool isLoadingContent() const;
    virtual void queryContent(int limit=-1);
    void setUrns(QSet<QString>& urnList);
    void setContentType(ContentType t);
    void setContact(const QString& contact);

signals:
    void dataChanged();

private slots:
    void onInitialQueryFinished();

private:
    ContentType m_contentType;
    TrackerContentProviderPrivate *d_ptr;
    Q_DECLARE_PRIVATE(TrackerContentProvider);
};

#endif // TRACKERCONTENTPROVIDER_H
