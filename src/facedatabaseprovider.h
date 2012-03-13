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

#ifndef FACEDATABASEPROVIDER_H
#define FACEDATABASEPROVIDER_H

#include <QList>
#include <QStandardItemModel>
#include <xqfacedatabase.h>

class QSparqlConnection;
class QStandardItemModel;
class XQFaceDatabase;

static QString UNKNOWN_CONTACT="extfr:unknown";

class FaceDatabaseProvider : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit FaceDatabaseProvider(QSparqlConnection *connection, QObject *parent = 0);
    ~FaceDatabaseProvider();
    QList<XQFaceRegion> getRegions(const QString &faceId);
    QString getContactName(const QString &faceId);

public slots:
    void update();

private:
    QStringList resolveContact(QString &contactUrn);
    void addRegion(QString &faceId, XQFaceRegion &region);
    void moveRegions(QString &fromFaceId, QString &toFaceId);

private:
    XQFaceDatabase *m_faceDatabase;
    QStandardItemModel *m_model;
    QSparqlConnection *m_sparqlConnection;
    QHash<QString, QList<XQFaceRegion> > m_suspectedRegions;
};

#endif // FACEDATABASEPROVIDER_H
