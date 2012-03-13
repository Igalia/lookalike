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

#include "facedatabaseprovider.h"
#include "gallerypeoplelistpage.h"
#include "nullthumbnailer.h"
#include "enforce.h"
#include <QSparqlConnection>
#include <QSparqlQuery>
#include <QSparqlResult>
#include <QStandardItemModel>
#include <QUrl>

FaceDatabaseProvider::FaceDatabaseProvider(QSparqlConnection *connection, QObject *parent) :
    QStandardItemModel(0, GalleryPeopleListPage::CeilingColumn, parent),
    m_sparqlConnection(connection)
{
    QString databaseName = QString(getenv("XDG_DATA_HOME")) + "/gallerycore/data/faces.db";
    NullThumbnailer thumbnailer;
    m_faceDatabase = new XQFaceDatabase(thumbnailer, databaseName);
    update();
    ENFORCE(connect(m_faceDatabase, SIGNAL(faceUpdated(XQFaceRegion,QString)),
                    this, SLOT(update())));
    connect(m_faceDatabase, SIGNAL(groupMerged(QString,QString)),
            this, SLOT(update()));
}

FaceDatabaseProvider::~FaceDatabaseProvider()
{
    delete m_faceDatabase;
}

QStringList FaceDatabaseProvider::resolveContact(QString &contactUrn)
{
    static QSparqlQuery query("SELECT\n"
                              " tracker:coalesce(fn:concat(nco:nameGiven(?:contact), \" \",\n"
                              "                            nco:nameFamily(?:contact)),\n"
                              "                            nco:nameGiven(?:contact),\n"
                              "                            nco:nickname(?:contact),\n"
                              "                            nco:nameFamily(?:contact),\n"
                              "                            nco:fullname(?:contact)\n"
                              "                  )\n"
                              " ?photoUrl \n"
                              "WHERE {\n"
                              " ?:contact rdf:type nco:PersonContact .\n"
                              " OPTIONAL {\n"
                              "     ?:contact nco:photo ?photoUrn .\n"
                              "     ?photoUrn nie:url ?photoUrl .\n"
                              " }\n"
                              "}\n");

    query.bindValue("contact", QUrl(contactUrn));
    QSparqlResult *result = m_sparqlConnection->syncExec(query);
    if (result->hasError() || !result->first()) {
        delete result;
        return QStringList();
    } else {
        QStringList stringResult;
        stringResult << result->value(0).toString() << result->value(1).toString();
        delete result;
        return stringResult;
    }
}

void FaceDatabaseProvider::update()
{
    QAbstractItemModel *faceGroupsModel = m_faceDatabase->faceGroups(XQFaceDatabase::UnnamedGroup);
    clear();
    m_suspectedRegions.clear();

    /* Add to our hash all the (faceid, <regions>) */
    for (int faceGroupRow = 0; faceGroupRow < faceGroupsModel->rowCount(); faceGroupRow++) {
        XQFaceGroup faceGroup = m_faceDatabase->faceGroup(faceGroupsModel->index(faceGroupRow, 0).data().toString());
        QAbstractItemModel *facesModel = faceGroup.faces();
        for (int faceRow = 0; faceRow < facesModel->rowCount(); faceRow++) {
            QString sourceId = facesModel->index(faceRow, 0).data().toString();
            XQFaceRegion faceRegion = faceGroup.faceRegion(sourceId);
            QStringList faceIds = faceRegion.faceIds();
            if (faceIds.isEmpty()) {
                addRegion(UNKNOWN_CONTACT, faceRegion);
                continue;
            }
            foreach(QString faceId, faceIds) {
                addRegion(faceId, faceRegion);
            }
        }
    }

    /* Based on previous data, create the model */
    foreach(QString key, m_suspectedRegions.keys()) {
        if (key == UNKNOWN_CONTACT) {
            /* Let's keep unknown contact for the end */
            continue;
        }
        QStringList resolvedValues = resolveContact(key);
        if (resolvedValues.isEmpty()) {
            /* The contact does not exist; let's move all the regions to unknown contact */
            moveRegions(key, UNKNOWN_CONTACT);
        } else {
            QList<QStandardItem *> row;
            row << new QStandardItem(resolvedValues[0]);
            row << new QStandardItem(QString().setNum(m_suspectedRegions.value(key).count()));
            row << new QStandardItem(resolvedValues[1]);
            row << new QStandardItem(key);
            appendRow(row);
        }
    }

    /* Add the unknown contact, if needed */
    if (m_suspectedRegions.contains(UNKNOWN_CONTACT)) {
        QList<QStandardItem *> row;
        row << new QStandardItem("Unknown");
        row << new QStandardItem(QString().setNum(m_suspectedRegions.value(UNKNOWN_CONTACT).count()));
        row << new QStandardItem("/usr/share/icons/hicolor/64x64/apps/icon-l-lookalike.png");
        row << new QStandardItem(UNKNOWN_CONTACT);
        insertRow(0, row);
    }

    /* Notify model change */
    QModelIndex startIndex = index(0,0);
    QModelIndex endIndex = index(rowCount()-1, GalleryPeopleListPage::CeilingColumn);
    emit dataChanged(startIndex, endIndex);
}

QList<XQFaceRegion> FaceDatabaseProvider::getRegions(const QString &faceId)
{
    return m_suspectedRegions.value(faceId);
}

QString FaceDatabaseProvider::getContactName(const QString &faceId)
{
    int row = findItems(faceId, Qt::MatchExactly, GalleryPeopleListPage::IdColumn).at(0)->row();
    return index(row, 0).data().toString();
}

void FaceDatabaseProvider::addRegion(QString &faceId, XQFaceRegion &region)
{
    QList<XQFaceRegion> regions = m_suspectedRegions.value(faceId);
    regions << region;
    m_suspectedRegions.insert(faceId, regions);
}

void FaceDatabaseProvider::moveRegions(QString &fromFaceId, QString &toFaceId)
{
    QList<XQFaceRegion> fromRegions = m_suspectedRegions.take(fromFaceId);
    QList<XQFaceRegion> toRegions = m_suspectedRegions.value(toFaceId);
    toRegions << fromRegions;
    m_suspectedRegions.insert(toFaceId, toRegions);
}
