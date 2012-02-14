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
    QSet<QString>& getImages();
    QSet<QString> getImages(const QString &faceId);

private:
    QStringList resolveContact(QString &contactUrn);

private:
    XQFaceDatabase *m_faceDatabase;
    QStandardItemModel *m_model;
    QSparqlConnection *m_sparqlConnection;
    QSet<QString> m_unknownImages;
    QHash<QString, QSet<QString> > m_suspectedImages;
};

#endif // FACEDATABASEPROVIDER_H
