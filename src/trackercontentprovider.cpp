#include "trackercontentprovider.h"
#include "trackercontentprovider_p.h"

TrackerContentProvider::TrackerContentProvider(QObject *parent):
    GalleryContentProvider(parent),
    d_ptr(new TrackerContentProviderPrivate(this))
{
}

void TrackerContentProvider::queryContent(int limit)
{
    Q_D(TrackerContentProvider);

    if (d->m_urnSet.isEmpty()) {
        emit initialQueryFinished();
        return;
    }
    d->m_queryRunning = true;
    d->buildQuery(limit);
    connect(d->m_liveQuery, SIGNAL(initialQueryFinished()),
            this, SLOT(onInitialQueryFinished()));
    d->m_liveQuery->start();
}

void TrackerContentProvider::onInitialQueryFinished()
{
    Q_D(TrackerContentProvider);

    d->m_queryRunning = false;
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
