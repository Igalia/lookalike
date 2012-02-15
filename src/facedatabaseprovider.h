#ifndef FACEDATABASEPROVIDER_H
#define FACEDATABASEPROVIDER_H

#include <QList>
#include <QStandardItemModel>
#include <xqfacedatabase.h>

class QSparqlConnection;
class QStandardItemModel;
class XQFaceDatabase;

class FaceDatabaseProvider : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit FaceDatabaseProvider(QObject *parent = 0);
    ~FaceDatabaseProvider();
    void update();
    QList<XQFaceRegion>& getRegions();
    QList<XQFaceRegion> getRegions(const QString &faceId);
    QString getContactName(const QString &faceId);

private:
    QStringList resolveContact(QString &contactUrn);

private:
    XQFaceDatabase *m_faceDatabase;
    QStandardItemModel *m_model;
    QSparqlConnection *m_sparqlConnection;
    QList<XQFaceRegion> m_unknownRegions;
    QHash<QString, QList<XQFaceRegion> > m_suspectedRegions;
};

#endif // FACEDATABASEPROVIDER_H
