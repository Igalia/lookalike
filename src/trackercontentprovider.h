#ifndef TRACKERCONTENTPROVIDER_H
#define TRACKERCONTENTPROVIDER_H

#include "gallerycontentprovider.h"

class TrackerContentProviderPrivate;
class QSparqlConnection;

class TrackerContentProvider : public GalleryContentProvider
{
    Q_OBJECT
public:
    TrackerContentProvider(QSparqlConnection *connection, QObject *parent = 0);
    QAbstractItemModel *model() const;
    virtual bool isLoadingContent() const;
    virtual void queryContent(int limit=-1);
    void setUrns(QSet<QString>& urnList);

private slots:
    void onInitialQueryFinished();

private:
    TrackerContentProviderPrivate *d_ptr;
    Q_DECLARE_PRIVATE(TrackerContentProvider);
};

#endif // TRACKERCONTENTPROVIDER_H
