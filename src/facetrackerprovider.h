#ifndef FACETRACKERPROVIDER_H
#define FACETRACKERPROVIDER_H

#include <QObject>

class QAbstractItemModel;
class QSparqlConnection;
class TrackerLiveQuery;

class FaceTrackerProvider : public QObject
{
    Q_OBJECT
public:
    explicit FaceTrackerProvider(QSparqlConnection *connection, QObject *parent = 0);
    ~FaceTrackerProvider();
    QAbstractItemModel* model();
private:
    QSparqlConnection *m_sparqlConnection;
    TrackerLiveQuery *m_liveQuery;
};

#endif // FACETRACKERPROVIDER_H
