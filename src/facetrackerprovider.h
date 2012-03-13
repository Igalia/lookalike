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

#ifndef FACETRACKERPROVIDER_H
#define FACETRACKERPROVIDER_H

#include <QObject>

class QAbstractItemModel;
class QSparqlConnection;
class TrackerLiveQuery;

class FaceTrackerProvider : public QObject
{
    Q_OBJECT
public:
    explicit FaceTrackerProvider(QSparqlConnection *connection, QObject *parent = 0);
    ~FaceTrackerProvider();
    QAbstractItemModel* model();
private:
    QSparqlConnection *m_sparqlConnection;
    TrackerLiveQuery *m_liveQuery;
};

#endif // FACETRACKERPROVIDER_H
