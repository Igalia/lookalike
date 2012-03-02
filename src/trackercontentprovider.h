#ifndef TRACKERCONTENTPROVIDER_H
#define TRACKERCONTENTPROVIDER_H

#include "gallerycontentprovider.h"

class QSparqlConnection;
class TrackerContentProviderPrivate;

class TrackerContentProvider : public GalleryContentProvider
{
    Q_OBJECT
public:
    enum ContentType {
        AllImages,
        ListImages,
        WithContact,
    };

    TrackerContentProvider(QSparqlConnection *connection, QObject *parent = 0);
    QAbstractItemModel *model() const;
    virtual bool isLoadingContent() const;
    virtual void queryContent(int limit=-1);
    void setUrns(QSet<QString>& urnList);
    void setContentType(ContentType t);
    void setContact(const QString& contact);

signals:
    void dataChanged();

private slots:
    void onInitialQueryFinished();

private:
    ContentType m_contentType;
    TrackerContentProviderPrivate *d_ptr;
    Q_DECLARE_PRIVATE(TrackerContentProvider);
};

#endif // TRACKERCONTENTPROVIDER_H
