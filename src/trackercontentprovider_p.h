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

#ifndef TRACKERCONTENTPROVIDER_P_H
#define TRACKERCONTENTPROVIDER_P_H

#include <QObject>
#include <QSet>
#include <TrackerLiveQuery>
#include "trackercontentprovider.h"

class SparqlConnection;

class TrackerContentProviderPrivate : public QObject
{
    Q_OBJECT
public:
    explicit TrackerContentProviderPrivate(TrackerContentProvider *q, QSparqlConnection *connection);
    ~TrackerContentProviderPrivate();
    void buildQueryListImages();
    void buildQueryFilterImagesNoFace();
    void buildQueryAllImages();
    void buildQueryWithContact();
    void deleteLiveQuery();

    TrackerLiveQuery *m_liveQuery;
    bool m_queryRunning;
    QSet<QString> m_urnSet;
    QString m_contact;
    int m_limit;

private:
    QSparqlConnection *m_sparqlConnection;
    TrackerContentProvider *q_ptr;
    Q_DECLARE_PUBLIC(TrackerContentProvider);
};

#endif // TRACKERCONTENTPROVIDER_P_H
