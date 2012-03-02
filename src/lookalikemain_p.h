#ifndef LOOKALIKEMAIN_P_H
#define LOOKALIKEMAIN_P_H

#include "lookalikemain.h"
#include <QAbstractItemModel>
#include <QObject>
#include <QUrl>

class FaceDatabaseProvider;
class FaceTrackerProvider;
class GalleryFullScreenPage;
class GalleryGridPage;
class GalleryModel;
class GalleryPeopleListPage;
class MAction;
class MApplicationPage;
class MWidgetAction;
class MDialog;
class MProgressIndicator;
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
    QString urnFromUrl(QUrl url);
    XQFaceRegion findRegion(QString &faceId, QString &sourceId);
    QRect scaleRect(const QRect &rect, QSize &fromSize, QSize &toSize);
    void updateTrackerFilter();
    void updateGrid();
    void updateGrid(const QString& displayName, bool addConfirmationMenu = true);
    void showPage(MApplicationPage *page, bool history = false);
    void confirmFace(QUrl image, QString& contact);

    TrackerContentProvider *m_trackerProvider;
    GalleryModel *m_galleryModel;
    GalleryPeopleListPage *m_unconfirmedPeopleListPage;
    GalleryPeopleListPage *m_confirmedPeopleListPage;
    GalleryGridPage *m_gridPage;
    GalleryFullScreenPage *m_fullScreenPage;
    FaceDatabaseProvider *m_faceDatabaseProvider;
    FaceTrackerProvider *m_faceTrackerProvider;
    QString m_personSelected;
    MAction* m_confirmFaceAction;
    MWidgetAction* m_toolbarAction;
    QList<QUrl> m_facesToConfirm;
    MDialog* m_progressDialog;
    MProgressIndicator *m_progress;

public slots:
    void confirmFaces();
    void onUnconfirmedPersonSelected(const QString &personId, const QString &displayName);
    void onConfirmedPersonSelected(const QString &personId, const QString &displayName);
    void onConfirmFaceActionTriggered();
    void onMultiSelectionDone(QList<QUrl> urlList);
    void onItemSelected(const QUrl& url);
    void onDataChanged();
    void onAllTabActionToggled(bool toggled);
    void onUnconfirmedPeopleTabActionToggled(bool toggled);
    void onConfirmedPeopleTabActionToggled(bool toggled);
    void onProgressDialogRejected();

private:
    LookAlikeMain *q_ptr;
    Q_DECLARE_PUBLIC(LookAlikeMain);
};

#endif // LOOKALIKEMAIN_P_H
