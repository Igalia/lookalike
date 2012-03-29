/*
 * This file is part of LookALike
 *
 * Copyright (C) 2012 Igalia S.L.
 *
 * Contact: agomez@igalia.com
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#ifndef LOOKALIKEMAIN_P_H
#define LOOKALIKEMAIN_P_H

#include "lookalikemain.h"
#include <QAbstractItemModel>
#include <QObject>
#include <QUrl>
#include <QuillMetadataRegionList>

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
    void updateGrid(const QString& displayName, MAction *addAction = 0);
    void updateUrnImages(QList<XQFaceRegion> regions);
    void showPage(MApplicationPage *page, bool history = false);
    void confirmFace(QUrl image, QString& contact);
    void deleteFace(QUrl image, QString& contact);

    TrackerContentProvider *m_trackerProvider;
    GalleryModel *m_galleryModel;
    GalleryPeopleListPage *m_proposedContactsListPage;
    GalleryPeopleListPage *m_confirmedContactsListPage;
    GalleryGridPage *m_gridPage;
    GalleryFullScreenPage *m_fullScreenPage;
    FaceDatabaseProvider *m_faceDatabaseProvider;
    FaceTrackerProvider *m_faceTrackerProvider;
    QString m_personSelected;
    MAction* m_confirmFaceAction;
    MAction* m_deleteFaceAction;
    MAction* m_aboutAction;
    MWidgetAction* m_toolbarAction;
    QList<QUrl> m_facesToProcess;
    MDialog* m_progressDialog;
    MProgressIndicator *m_progress;
    MAction* m_currentAction;

public slots:
    void processFaces();
    void saveMetadataRegionList(QString &fileName, QuillMetadataRegionList &regionList);
    void onProposedContactPersonSelected(const QString &personId, const QString &displayName);
    void onConfirmedContactSelected(const QString &personId, const QString &displayName);
    void onConfirmFaceActionTriggered();
    void onDeleteFaceActionTriggered();
    void onAboutActionTriggered();
    void onMultiSelectionDone(QList<QUrl> urlList);
    void onItemSelected(const QUrl& url);
    void onDataChanged();
    void onAllTabActionToggled(bool toggled);
    void onProposedContactTabActionToggled(bool toggled);
    void onConfirmedContactTabActionToggled(bool toggled);
    void onLandscapeTabActionToggled(bool toggled);
    void onProgressDialogRejected();
    void onGridPageAppeared();
    void onFullscreenPageAppeared();

private:
    LookAlikeMain *q_ptr;
    Q_DECLARE_PUBLIC(LookAlikeMain);
};

#endif // LOOKALIKEMAIN_P_H
