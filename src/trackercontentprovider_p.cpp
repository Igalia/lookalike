#include "QAbstractItemModel"
#include "QSparqlConnection"
#include "QStringList"
#include "trackercontentprovider_p.h"

TrackerContentProviderPrivate::TrackerContentProviderPrivate(TrackerContentProvider *q, QSparqlConnection *connection) :
    QObject(q),
    m_liveQuery(0),
    m_queryRunning(false),
    m_sparqlConnection(connection),
    q_ptr(q)
{
}


TrackerContentProviderPrivate::~TrackerContentProviderPrivate()
{
    deleteLiveQuery();
}

void TrackerContentProviderPrivate::buildQuery(int limit)
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

    if (limit > 0) {
        mainSentence += QString("LIMIT %1").arg(limit);
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


void TrackerContentProviderPrivate::deleteLiveQuery()
{
    if (m_liveQuery != 0) {
        m_liveQuery->deleteLater();
        m_liveQuery = 0;
    }
}
