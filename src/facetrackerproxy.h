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

#ifndef FACETRACKERPROXY_H
#define FACETRACKERPROXY_H

#include <QAbstractProxyModel>
#include <QStandardItemModel>

const QString EVERYBODY_CONTACT = "extfr:everybody";

class FaceTrackerProvider;
class QSparqlConnection;

class FaceTrackerProxy : public QAbstractProxyModel
{
    Q_OBJECT
public:
    explicit FaceTrackerProxy(FaceTrackerProvider *trackerProvider, QAbstractItemModel *model);
    virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;
    virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &proxyIndex, int role) const;

private slots:
    void onRowsAboutToBeInserted(const QModelIndex &parent, int first, int last);
    void onRowsInserted();
    void onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    void onRowsRemoved();
    void onDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);

private:
    void updateEverybodyCount();

    QSparqlConnection *m_sparqlConnection;
    QStandardItemModel m_everybodyModel;
};

#endif // FACETRACKERPROXY_H
