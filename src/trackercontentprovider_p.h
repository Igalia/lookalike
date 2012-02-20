#ifndef TRACKERCONTENTPROVIDER_P_H
#define TRACKERCONTENTPROVIDER_P_H

#include <QObject>
#include <QSet>
#include <TrackerLiveQuery>
#include "trackercontentprovider.h"

class SparqlConnection;

class TrackerContentProviderPrivate : public QObject
{
    Q_OBJECT
public:
    explicit TrackerContentProviderPrivate(TrackerContentProvider *q, QSparqlConnection *connection);
    ~TrackerContentProviderPrivate();
    void buildQuery(int limit = -1);
    void deleteLiveQuery();

    TrackerLiveQuery *m_liveQuery;
    bool m_queryRunning;
    QSet<QString> m_urnSet;

private:
    QSparqlConnection *m_sparqlConnection;
    TrackerContentProvider *q_ptr;
    Q_DECLARE_PUBLIC(TrackerContentProvider);
};

#endif // TRACKERCONTENTPROVIDER_P_H
