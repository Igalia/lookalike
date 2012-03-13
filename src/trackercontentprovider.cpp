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

#include <QAbstractItemModel>
#include <QDebug>
#include "trackercontentprovider.h"
#include "trackercontentprovider_p.h"

TrackerContentProvider::TrackerContentProvider(QSparqlConnection *connection, QObject *parent):
    GalleryContentProvider(parent),
    d_ptr(new TrackerContentProviderPrivate(this, connection))
{
}

void TrackerContentProvider::queryContent(int limit)
{
    Q_D(TrackerContentProvider);

    d->m_limit = limit;
    switch (m_contentType) {
    case ListImages:
        if (d->m_urnSet.isEmpty()) {
            d->deleteLiveQuery();
            onInitialQueryFinished();
            return;
        }
        d->m_queryRunning = true;
        d->buildQueryListImages();
        break;
    case AllImages:
        d->m_queryRunning = true;
        d->buildQueryAllImages();
        break;
    case WithContact:
        if (d->m_urnSet.isEmpty()) {
            d->deleteLiveQuery();
            onInitialQueryFinished();
            return;
        }
        d->m_queryRunning = true;
        d->buildQueryWithContact();
        break;
    default:
        return;
    }

    connect(d->m_liveQuery, SIGNAL(initialQueryFinished()),
            this, SLOT(onInitialQueryFinished()),
            Qt::UniqueConnection);
    d->m_liveQuery->start();
}

void TrackerContentProvider::onInitialQueryFinished()
{
    Q_D(TrackerContentProvider);

    d->m_queryRunning = false;
    QAbstractItemModel *m = model();
    if (m != 0) {
        connect(m, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                this, SIGNAL(dataChanged()),Qt::UniqueConnection);
    }
    emit initialQueryFinished();
}

bool TrackerContentProvider::isLoadingContent() const
{
    Q_D(const TrackerContentProvider);

    return d->m_queryRunning;
}

QAbstractItemModel* TrackerContentProvider::model() const
{
    Q_D(const TrackerContentProvider);

    return d->m_liveQuery? d->m_liveQuery->model(): 0;
}

void TrackerContentProvider::setUrns(QSet<QString> &urnList)
{
    Q_D(TrackerContentProvider);

    d->m_urnSet = urnList;
}

void TrackerContentProvider::setContact(const QString &contact)
{
    Q_D(TrackerContentProvider);

    d->m_contact = contact;
}

void TrackerContentProvider::setContentType(ContentType t)
{
    m_contentType = t;
}
