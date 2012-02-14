#ifndef TRACKERCONTENTPROVIDER_P_H
#define TRACKERCONTENTPROVIDER_P_H

#include <QObject>
#include <QSet>
#include <TrackerLiveQuery>

class SparqlConnection;

class TrackerContentProviderPrivate : public QObject
{
    Q_OBJECT
public:
    explicit TrackerContentProviderPrivate(QSparqlConnection *connection, QObject *parent = 0);
    void buildQuery(int limit = -1);

    TrackerLiveQuery *m_liveQuery;
    bool m_queryRunning;
    QSet<QString> m_urnSet;
private:
    QSparqlConnection *m_sparqlConnection;
};

#endif // TRACKERCONTENTPROVIDER_P_H
