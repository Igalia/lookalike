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

#include "facetrackerprovider.h"
#include "facetrackerproxy.h"
#include <QAbstractItemModel>
#include <QSparqlConnection>
#include <QStringList>
#include <TrackerLiveQuery>

FaceTrackerProvider::FaceTrackerProvider(QSparqlConnection *connection, QObject *parent) :
    QObject(parent),
    m_sparqlConnection(connection)
{
    QString sparqlSentence("SELECT\n"
                               "    tracker:coalesce(fn:concat(nco:nameGiven(?contact), \" \",\n"
                               "                               nco:nameFamily(?contact)),\n"
                               "                     nco:nameGiven(?contact),\n"
                               "                     nco:nickname(?contact),\n"
                               "                     nco:nameFamily(?contact),\n"
                               "                     nco:fullname(?contact),\n"
                               "                     nco:imNickname(?imaddress))\n"
                               "    COUNT(DISTINCT ?visual)\n"
                               "    tracker:coalesce(nie:url(?photo),\n"
                               "                     nco:imAvatar(?imaddress))\n"
                               "    ?contact\n"
                               "    tracker:id(?contact)\n"
                               "WHERE {\n"
                               "    ?contact rdf:type nco:PersonContact .\n"
                               "    ?visual rdf:type nfo:Visual ;\n"
                               "            nfo:hasRegionOfInterest ?region .\n"
                               "    ?region nfo:regionOfInterestType nfo:roi-content-face ;\n"
                               "            nfo:roiRefersTo ?contact .\n"
                               "    OPTIONAL {\n"
                               "        ?contact nco:hasAffiliation ?affiliation .\n"
                               "        ?affiliation nco:hasIMAddress ?imaddress\n"
                               "    } .\n"
                               "    OPTIONAL {\n"
                               "        ?contact nco:photo ?photo\n"
                               "    } .\n"
                               "}\n"
                               "GROUP BY ?contact\n"
                               "ORDER BY ?name");
    m_liveQuery = new TrackerLiveQuery(sparqlSentence, 5, *m_sparqlConnection);
    m_liveQuery->setIdentityColumns(QList<int>() << 4);

    TrackerFullUpdater updater;
    updater.watchClass("nmm:Photo",
                       QStringList() << "rdf:type" << "nfo:hasRegionOfInterest");
    updater.watchClass("nfo:RegionOfInterest",
                       QStringList() << "rdf:type" << "nfo:roiRefersTo");
    updater.watchClass("nco:PersonContact",
                       QStringList() << "rdf:type" << "nco:photo");
    m_liveQuery->addUpdater(updater);
    m_liveQuery->setUpdatesEnabled(true);
    m_liveQuery->start();

    m_proxy = new FaceTrackerProxy(m_liveQuery->model(), this);
}

FaceTrackerProvider::~FaceTrackerProvider()
{
    delete m_liveQuery;
    delete m_proxy;
}

QAbstractItemModel* FaceTrackerProvider::model()
{
    return m_proxy;
}
