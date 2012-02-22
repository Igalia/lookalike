#ifndef LOOKALIKEMAIN_P_H
#define LOOKALIKEMAIN_P_H

#include "lookalikemain.h"
#include <QAbstractItemModel>
#include <QObject>
#include <QUrl>

class FaceDatabaseProvider;
class GalleryFullScreenPage;
class GalleryGridPage;
class GalleryModel;
class GalleryPeopleListPage;
class MAction;
class MApplicationPage;
class MWidgetAction;
class QSparqlConnection;
class TrackerContentProvider;
class XQFaceDatabase;
class XQFaceRegion;

class LookAlikeMainPrivate : public QObject
{
    Q_OBJECT
public:
    explicit LookAlikeMainPrivate(LookAlikeMain *q);
    ~LookAlikeMainPrivate();
    void showPage();
    QString urnFromUrl(QUrl url);
    XQFaceRegion findRegion(QString &faceId, QString &sourceId);
    QRect scaleRect(const QRect &rect, QSize &fromSize, QSize &toSize);
    void updateTrackerFilter();
    void updateGrid();
    void updateGrid(const QString& displayName);

    TrackerContentProvider *m_trackerProvider;
    GalleryModel *m_galleryModel;
    GalleryPeopleListPage *m_peopleListPage;
    GalleryGridPage *m_gridPage;
    GalleryFullScreenPage *m_fullScreenPage;
    FaceDatabaseProvider *m_faceDatabaseProvider;
    QString m_personSelected;
    MAction* m_confirmFaceAction;
    MWidgetAction* m_toolbarAction;

public slots:
    void onPersonSelected(const QString &personId, const QString &displayName);
    void onConfirmFaceActionTriggered();
    void onMultiSelectionDone(QList<QUrl> urlList);
    void onItemSelected(const QUrl& url);
    void onDataChanged();
    void onUnknownTabActionToggled(bool toggled);
    void onPeopleTabActionToggled(bool toggled);

private:
    LookAlikeMain *q_ptr;
    Q_DECLARE_PUBLIC(LookAlikeMain);
};

#endif // LOOKALIKEMAIN_P_H
