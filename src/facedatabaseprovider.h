#ifndef FACEDATABASEPROVIDER_H
#define FACEDATABASEPROVIDER_H

#include <QList>
#include <QStandardItemModel>
#include <xqfacedatabase.h>

class QSparqlConnection;
class QStandardItemModel;
class XQFaceDatabase;

static QString UNKNOWN_CONTACT="extfr:unknown";

class FaceDatabaseProvider : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit FaceDatabaseProvider(QSparqlConnection *connection, QObject *parent = 0);
    ~FaceDatabaseProvider();
    QList<XQFaceRegion> getRegions(const QString &faceId);
    QString getContactName(const QString &faceId);

public slots:
    void update();

private:
    QStringList resolveContact(QString &contactUrn);
    void addRegion(QString &faceId, XQFaceRegion &region);
    void moveRegions(QString &fromFaceId, QString &toFaceId);

private:
    XQFaceDatabase *m_faceDatabase;
    QStandardItemModel *m_model;
    QSparqlConnection *m_sparqlConnection;
    QHash<QString, QList<XQFaceRegion> > m_suspectedRegions;
};

#endif // FACEDATABASEPROVIDER_H
