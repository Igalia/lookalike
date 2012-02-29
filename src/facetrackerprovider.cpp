#include "facetrackerprovider.h"
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
}

FaceTrackerProvider::~FaceTrackerProvider()
{
    delete m_liveQuery;
}

QAbstractItemModel* FaceTrackerProvider::model()
{
    return m_liveQuery->model();
}
