#include <QAbstractItemModel>
#include <QDebug>
#include "trackercontentprovider.h"
#include "trackercontentprovider_p.h"

TrackerContentProvider::TrackerContentProvider(QSparqlConnection *connection, QObject *parent):
    GalleryContentProvider(parent),
    d_ptr(new TrackerContentProviderPrivate(this, connection))
{
}

void TrackerContentProvider::queryContent(int limit)
{
    Q_D(TrackerContentProvider);

    d->m_limit = limit;
    switch (m_contentType) {
    case ListImages:
        if (d->m_urnSet.isEmpty()) {
            d->deleteLiveQuery();
            onInitialQueryFinished();
            return;
        }
        d->m_queryRunning = true;
        d->buildQueryListImages();
        break;
    case AllImages:
        d->m_queryRunning = true;
        d->buildQueryAllImages();
        break;
    case WithContact:
        if (d->m_urnSet.isEmpty()) {
            d->deleteLiveQuery();
            onInitialQueryFinished();
            return;
        }
        d->m_queryRunning = true;
        d->buildQueryWithContact();
        break;
    default:
        return;
    }

    connect(d->m_liveQuery, SIGNAL(initialQueryFinished()),
            this, SLOT(onInitialQueryFinished()),
            Qt::UniqueConnection);
    d->m_liveQuery->start();
}

void TrackerContentProvider::onInitialQueryFinished()
{
    Q_D(TrackerContentProvider);

    d->m_queryRunning = false;
    QAbstractItemModel *m = model();
    if (m != 0) {
        connect(m, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                this, SIGNAL(dataChanged()),Qt::UniqueConnection);
    }
    emit initialQueryFinished();
}

bool TrackerContentProvider::isLoadingContent() const
{
    Q_D(const TrackerContentProvider);

    return d->m_queryRunning;
}

QAbstractItemModel* TrackerContentProvider::model() const
{
    Q_D(const TrackerContentProvider);

    return d->m_liveQuery? d->m_liveQuery->model(): 0;
}

void TrackerContentProvider::setUrns(QSet<QString> &urnList)
{
    Q_D(TrackerContentProvider);

    d->m_urnSet = urnList;
}

void TrackerContentProvider::setContact(const QString &contact)
{
    Q_D(TrackerContentProvider);

    d->m_contact = contact;
}

void TrackerContentProvider::setContentType(ContentType t)
{
    m_contentType = t;
}
