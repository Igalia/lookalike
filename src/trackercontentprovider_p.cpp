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

#include "QAbstractItemModel"
#include "QSparqlConnection"
#include "QStringList"
#include "trackercontentprovider_p.h"

TrackerContentProviderPrivate::TrackerContentProviderPrivate(TrackerContentProvider *q, QSparqlConnection *connection) :
    QObject(q),
    m_liveQuery(0),
    m_queryRunning(false),
    m_limit(-1),
    m_sparqlConnection(connection),
    q_ptr(q)
{
}


TrackerContentProviderPrivate::~TrackerContentProviderPrivate()
{
    deleteLiveQuery();
}

void TrackerContentProviderPrivate::buildQueryListImages()
{
    QString sparqlStubSentence("SELECT\n"
                         "  nie:url(?urn) AS url\n"
                         "  nfo:fileName(?urn) AS filename\n"
                         "  ?created AS created\n"
                         "  nie:mimeType(?urn) AS mimetype\n"
                         "  \"false\"^^xsd:boolean AS favorite\n"
                         "  nfo:duration AS duration\n"
                         "  ?urn AS urn\n"
                         "  \"false\"^^xsd:boolean AS virtual\n"
                         "  COUNT(nfo:hasRegionOfInterest(?urn)) AS totalRegions\n"
                         "  tracker:id(?urn) AS trackerid\n"
                         "WHERE {\n"
                         "  ?urn rdf:type nfo:Visual ;\n"
                         "       tracker:available \"true\"^^xsd:boolean .\n"
                         "  FILTER(?urn  IN (%1) ) .\n"
                         "  OPTIONAL { ?urn nie:contentCreated ?created } .\n"
                         "  %2"
                         "}\n"
                         "GROUP BY ?urn\n");

    QString urnList;
    QSetIterator<QString> iter(m_urnSet);
    while (iter.hasNext()) {
        urnList += "<" + iter.next() + ">";
        if (iter.hasNext()) {
            urnList += " , ";
        }
    }

    QString mainSentence = QString(sparqlStubSentence).arg(urnList).arg("");
    QString updateSentence = QString(sparqlStubSentence).arg(urnList).arg("%FILTER\n");

    if (m_limit > 0) {
        mainSentence += QString("LIMIT %1").arg(m_limit);
    }

    deleteLiveQuery();
    m_liveQuery = new TrackerLiveQuery(mainSentence, 10, *m_sparqlConnection);
    TrackerPartialUpdater updater(updateSentence);
    updater.watchClass("nmm:Photo",
                       QStringList(),
                       "tracker:id(?urn) in %LIST",
                       TrackerPartialUpdater::Subject,
                       9);

    m_liveQuery->addUpdater(updater);
    m_liveQuery->setIdentityColumns(QList<int>() << 9);
    m_liveQuery->setUpdatesEnabled(true);
}

void TrackerContentProviderPrivate::buildQueryFilterImagesNoFace()
{
    QString sparqlStubSentence("SELECT\n"
                         "  nie:url(?urn) AS url\n"
                         "  nfo:fileName(?urn) AS filename\n"
                         "  ?created AS created\n"
                         "  nie:mimeType(?urn) AS mimetype\n"
                         "  \"false\"^^xsd:boolean AS favorite\n"
                         "  nfo:duration AS duration\n"
                         "  ?urn AS urn\n"
                         "  \"false\"^^xsd:boolean AS virtual\n"
                         "  tracker:id(?urn) AS trackerid\n"
                         "WHERE {\n"
                         "  ?urn rdf:type nmm:Photo ;\n"
                         "       tracker:available \"true\"^^xsd:boolean .\n"
                         "  OPTIONAL { ?urn nie:contentCreated ?created } .\n"
                         "  FILTER( NOT EXISTS { ?urn nfo:hasRegionOfInterest ?roi .\n"
                         "                       ?roi nfo:regionOfInterestType nfo:roi-content-face .\n"
                         "                     } ).\n"
                         "  FILTER( ?urn NOT IN (%1) ) .\n"
                         "  %2"
                         "}\n");

    QString urnList;
    QSetIterator<QString> iter(m_urnSet);
    while (iter.hasNext()) {
        urnList += "<" + iter.next() + ">";
        if (iter.hasNext()) {
            urnList += " , ";
        }
    }

    QString mainSentence = QString(sparqlStubSentence).arg(urnList).arg("");
    QString updateSentence = QString(sparqlStubSentence).arg(urnList).arg("%FILTER\n");

    if (m_limit > 0) {
        mainSentence += QString("LIMIT %1").arg(m_limit);
    }

    deleteLiveQuery();
    m_liveQuery = new TrackerLiveQuery(mainSentence, 9, *m_sparqlConnection);
    TrackerPartialUpdater updater(updateSentence);
    updater.watchClass("nmm:Photo",
                       QStringList(),
                       "tracker:id(?urn) in %LIST",
                       TrackerPartialUpdater::Subject,
                       8);

    m_liveQuery->addUpdater(updater);
    m_liveQuery->setIdentityColumns(QList<int>() << 8);
    m_liveQuery->setUpdatesEnabled(true);
}

void TrackerContentProviderPrivate::buildQueryAllImages()
{
    QString sparqlStubSentence("SELECT\n"
                         "  nie:url(?urn) AS url\n"
                         "  nfo:fileName(?urn) AS filename\n"
                         "  ?created AS created\n"
                         "  nie:mimeType(?urn) AS mimetype\n"
                         "  \"false\"^^xsd:boolean AS favorite\n"
                         "  nfo:duration AS duration\n"
                         "  ?urn AS urn\n"
                         "  \"false\"^^xsd:boolean AS virtual\n"
                         "  tracker:id(?urn) AS trackerid\n"
                         "WHERE {\n"
                         "  ?urn rdf:type nmm:Photo ;\n"
                         "       tracker:available \"true\"^^xsd:boolean .\n"
                         "  OPTIONAL { ?urn nie:contentCreated ?created } .\n"
                         "  %1"
                         "}\n");
    QString mainSentence = QString(sparqlStubSentence).arg("");
    QString updateSentence = QString(sparqlStubSentence).arg("%FILTER\n");

    if (m_limit > 0) {
        mainSentence += QString("LIMIT %1").arg(m_limit);
    }

    deleteLiveQuery();
    m_liveQuery = new TrackerLiveQuery(mainSentence, 9, *m_sparqlConnection);
    TrackerPartialUpdater updater(updateSentence);
    updater.watchClass("nmm:Photo",
                       QStringList(),
                       "tracker:id(?urn) in %LIST",
                       TrackerPartialUpdater::Subject,
                       8);
    m_liveQuery->addUpdater(updater);
    m_liveQuery->setIdentityColumns(QList<int>() << 8);
    m_liveQuery->setUpdatesEnabled(true);
}

void TrackerContentProviderPrivate::buildQueryWithContact()
{
    QString sparqlStubSentence("SELECT\n"
                         "  nie:url(?urn) AS url\n"
                         "  nfo:fileName(?urn) AS filename\n"
                         "  ?created AS created\n"
                         "  nie:mimeType(?urn) AS mimetype\n"
                         "  \"false\"^^xsd:boolean AS favorite\n"
                         "  nfo:duration AS duration\n"
                         "  ?urn AS urn\n"
                         "  \"false\"^^xsd:boolean AS virtual\n"
                         "  tracker:id(?urn) AS trackerid\n"
                         "WHERE {\n"
                         "  ?urn rdf:type nmm:Photo ;\n"
                         "       tracker:available \"true\"^^xsd:boolean .\n"
                         "  OPTIONAL { ?urn nie:contentCreated ?created } .\n"
                         "  FILTER( EXISTS { ?urn nfo:hasRegionOfInterest ?roi .\n"
                         "                   ?roi nfo:regionOfInterestType nfo:roi-content-face .\n"
                         "                   %1 } ).\n"
                         "  %2"
                         "}\n");

    QString mainSentence;
    QString updateSentence;
    if (m_contact.isEmpty()) {
        mainSentence = QString(sparqlStubSentence).arg("").arg("");
        updateSentence = QString(sparqlStubSentence).arg("").arg("%FILTER");
    } else {
        mainSentence = QString(sparqlStubSentence).arg("?roi nfo:roiRefersTo <" + m_contact + "> .").arg("");
        updateSentence = QString(sparqlStubSentence).arg("?roi nfo:roiRefersTo <" + m_contact + "> .").arg("%FILTER\n");
    }

    if (m_limit > 0) {
        mainSentence += QString("LIMIT %1").arg(m_limit);
    }

    deleteLiveQuery();
    m_liveQuery = new TrackerLiveQuery(mainSentence, 9, *m_sparqlConnection);
    TrackerPartialUpdater updater(updateSentence);
    updater.watchClass("nmm:Photo",
                       QStringList(),
                       "tracker:id(?urn) in %LIST",
                       TrackerPartialUpdater::Subject,
                       8);
    m_liveQuery->addUpdater(updater);
    m_liveQuery->setIdentityColumns(QList<int>() << 8);
    m_liveQuery->setUpdatesEnabled(true);
}

void TrackerContentProviderPrivate::deleteLiveQuery()
{
    if (m_liveQuery != 0) {
        m_liveQuery->deleteLater();
        m_liveQuery = 0;
    }
}
