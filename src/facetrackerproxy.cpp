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

#include "facetrackerproxy.h"
#include "facetrackerprovider.h"
#include <QSparqlConnection>
#include <QSparqlQuery>
#include <QSparqlResult>

FaceTrackerProxy::FaceTrackerProxy(FaceTrackerProvider *trackerProvider, QAbstractItemModel *model) :
    QAbstractProxyModel(trackerProvider),
    m_sparqlConnection(trackerProvider->connection()),
    m_everybodyModel(0, 4)

{
    QList<QStandardItem*> items;
    items.append(new QStandardItem("Everybody"));
    items.append(new QStandardItem("0"));
    items.append(new QStandardItem("/usr/share/themes/base/meegotouch/lookalike/icons/icon-m-lookalike-everybody-main-view.png"));
    items.append(new QStandardItem(EVERYBODY_CONTACT));
    m_everybodyModel.appendRow(items);

    if (model) {
        setSourceModel(model);
        Q_ASSERT(connect(model, SIGNAL(rowsAboutToBeInserted(const QModelIndex&, int, int)),
                this, SLOT(onRowsAboutToBeInserted(const QModelIndex&, int, int))));
        Q_ASSERT(connect(model, SIGNAL(rowsInserted(const QModelIndex&, int, int)),
                         this, SLOT(onRowsInserted())));
        Q_ASSERT(connect(model, SIGNAL(rowsAboutToBeRemoved(const QModelIndex&, int, int)),
                this, SLOT(onRowsAboutToBeRemoved(const QModelIndex&, int, int))));
        Q_ASSERT(connect(model, SIGNAL(rowsRemoved(const QModelIndex&, int, int)),
                         this, SLOT(onRowsRemoved())));
        Q_ASSERT(connect(model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
                         this, SLOT(onDataChanged(const QModelIndex&, const QModelIndex&))));
        updateEverybodyCount();
   }
}

QModelIndex FaceTrackerProxy::mapFromSource(const QModelIndex &sourceIndex) const
{
    if (!sourceIndex.isValid()) {
        return QModelIndex();
    }

    if (sourceIndex.model() == sourceModel()) {
        return createIndex(sourceIndex.row() + 1, sourceIndex.column());
    } else {
        return createIndex(0, sourceIndex.column());
    }
}

QModelIndex FaceTrackerProxy::mapToSource(const QModelIndex &proxyIndex) const
{
    if (!proxyIndex.isValid()) {
        return QModelIndex();
    }

    if (proxyIndex.row() == 0) {
        return m_everybodyModel.index(0, proxyIndex.column());
    } else {
        return sourceModel()->index(proxyIndex.row() - 1, proxyIndex.column());
    }
}

QModelIndex FaceTrackerProxy::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);

    return QModelIndex();
}

int FaceTrackerProxy::rowCount(const QModelIndex &parent) const
{
    return sourceModel()->rowCount(parent) + 1;
}

int FaceTrackerProxy::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 4;
}

QModelIndex FaceTrackerProxy::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if (row == 0) {
        return mapFromSource(m_everybodyModel.index(0, column));
    } else {
        return mapFromSource(sourceModel()->index(row - 1, column));
    }
}

QVariant FaceTrackerProxy::data(const QModelIndex &proxyIndex, int role) const
{
    if (!proxyIndex.isValid()) {
        return QVariant();
    }

    QModelIndex sourceIndex = mapToSource(proxyIndex);
    return sourceIndex.data(role);
}

void FaceTrackerProxy::onRowsAboutToBeInserted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    Q_UNUSED(first);
    Q_UNUSED(last);

    beginInsertRows(parent, first + 1, last + 1);
}

void FaceTrackerProxy::onRowsInserted()
{
    updateEverybodyCount();
    endInsertRows();
}

void FaceTrackerProxy::onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    Q_UNUSED(first);
    Q_UNUSED(last);

    beginRemoveRows(parent, first + 1, last + 1);
}

void FaceTrackerProxy::onRowsRemoved()
{
    updateEverybodyCount();
    endRemoveRows();
}

void FaceTrackerProxy::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    updateEverybodyCount();

    QModelIndex proxyTopLeft = mapFromSource(topLeft);
    QModelIndex proxyBottomRight = mapFromSource(bottomRight);
    emit dataChanged(proxyTopLeft, proxyBottomRight);
}

void FaceTrackerProxy::updateEverybodyCount()
{
    static QSparqlQuery query("SELECT\n"
                             "  COUNT(DISTINCT ?visual)\n"
                             "WHERE {\n"
                             "  ?visual rdf:type nfo:Visual ;\n"
                             "          nfo:hasRegionOfInterest ?region .\n"
                             "  ?region nfo:regionOfInterestType nfo:roi-content-face .\n"
                             "}");
    QSparqlResult *result = m_sparqlConnection->syncExec(query);
    if (!result->hasError() && result->first()) {
        m_everybodyModel.setItem(0, 1, new QStandardItem(result->value(0).toString()));
        QModelIndex indexChanged = createIndex(0, 1);
        emit dataChanged(indexChanged, indexChanged);
    }

    delete result;
}
