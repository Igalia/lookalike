#ifndef FACEDATABASEPROVIDER_H
#define FACEDATABASEPROVIDER_H

#include <QStandardItemModel>
#include <QSet>
#include <QHash>

class XQFaceDatabase;
class QStandardItemModel;
class QSparqlConnection;

class FaceDatabaseProvider : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit FaceDatabaseProvider(QObject *parent = 0);
    ~FaceDatabaseProvider();
    void update();

private:
    QStringList resolveContact(QString &contactUrn);

private:
    XQFaceDatabase *m_faceDatabase;
    QStandardItemModel *m_model;
    QSparqlConnection *m_sparqlConnection;
};

#endif // FACEDATABASEPROVIDER_H
