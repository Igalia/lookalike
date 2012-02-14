#ifndef LOOKALIKEMAIN_P_H
#define LOOKALIKEMAIN_P_H

#include "lookalikemain.h"
#include <QObject>
#include <QAbstractItemModel>

class GalleryPeopleListPage;
class GalleryGridPage;
class GalleryModel;
class MApplicationPage;
class XQFaceDatabase;
class FaceDatabaseProvider;
class TrackerContentProvider;

class LookAlikeMainPrivate : public QObject
{
    Q_OBJECT
public:
    explicit LookAlikeMainPrivate(QObject *parent = 0);
    ~LookAlikeMainPrivate();
    void showPage();

    TrackerContentProvider *m_trackerProvider;
    GalleryModel *m_galleryModel;
    GalleryPeopleListPage *m_peopleListPage;
    GalleryGridPage *m_gridPage;
    FaceDatabaseProvider *m_faceDatabaseProvider;

public slots:
    void onPersonSelected(const QString &personId, const QString &displayName);

private:
    LookAlikeMain *q_ptr;
    Q_DECLARE_PUBLIC(LookAlikeMain);
};

#endif // LOOKALIKEMAIN_P_H
