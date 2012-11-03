/*
 * This file is part of LookAlike
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

#include "facedatabaseprovider.h"
#include "facetrackerprovider.h"
#include "facetrackerproxy.h"
#include "facerecognitionresetter.h"
#include "galleryfullscreenpage.h"
#include "gallerygridpage.h"
#include "gallerymodel.h"
#include "gallerypeoplelistpage.h"
#include "lookalikemain_p.h"
#include "math.h"
#include "trackercontentprovider.h"
#include <MAction>
#include <MApplication>
#include <MApplicationPage>
#include <MApplicationWindow>
#include <MDialog>
#include <MImageWidget>
#include <MLabel>
#include <MMessageBox>
#include <MProgressIndicator>
#include <MSceneManager>
#include <MToolBar>
#include <MWidgetAction>
#include <QAbstractItemModel>
#include <QGraphicsLinearLayout>
#include <QSparqlConnection>
#include <QTimer>
#include <QuillMetadata>
#include <QuillMetadataRegion>
#include <xqfacedatabase.h>

LookAlikeMainPrivate::LookAlikeMainPrivate(LookAlikeMain *q) :
    QObject(q),
    m_currentAction(0),
    q_ptr(q)
{
    // Less correct the problem with the DB first of all, if no other
    // application has dealt with it first.
    m_faceRecognitionResetter = new FaceRecognitionResetter();
    if (!m_faceRecognitionResetter->isDBCorrected()) {
        m_faceRecognitionResetter->dealWithDB(1);
    }

    m_galleryModel = new GalleryModel(this);
    m_galleryModel->setFaceRecognitionEnabled(true);
    QSparqlConnection *connection = m_galleryModel->sparqlConnection();
    m_trackerProvider = new TrackerContentProvider(connection, this);
    m_galleryModel->addContentProvider(m_trackerProvider);
    m_faceDatabaseProvider = new FaceDatabaseProvider(connection, this);
    m_faceTrackerProvider = new FaceTrackerProvider(connection, this);
    m_confirmedContactsListPage = new GalleryPeopleListPage(m_faceTrackerProvider->model());
    m_proposedContactsListPage = new GalleryPeopleListPage(m_faceDatabaseProvider);
    m_gridPage = new GalleryGridPage(*m_galleryModel);
    m_fullScreenPage = new GalleryFullScreenPage(*m_galleryModel);

    m_confirmedContactsListPage->setStyleName("GalleryPage");
    m_proposedContactsListPage->setStyleName("GalleryPage");
    m_gridPage->setStyleName("GalleryPage");
    m_fullScreenPage->setStyleName("GalleryPage");

    MAction* allTabAction = new MAction("icon-m-toolbar-all-content-white", "", q);
    allTabAction->setLocation(MAction::ToolBarLocation);
    allTabAction->setCheckable(true);

    MAction* confirmedContactsTabAction = new MAction("icon-m-toolbar-known-people-white", "", q);
    confirmedContactsTabAction->setLocation(MAction::ToolBarLocation);
    confirmedContactsTabAction->setCheckable(true);

    MAction* proposedContactsAction = new MAction("icon-m-toolbar-unknown-people-white", "", q);
    proposedContactsAction->setLocation(MAction::ToolBarLocation);
    proposedContactsAction->setCheckable(true);

    MAction* landscapeTabAction = new MAction("icon-m-toolbar-no-people-white", "", q);
    landscapeTabAction->setLocation(MAction::ToolBarLocation);
    landscapeTabAction->setCheckable(true);

    QList<QAction*> actions;
    actions.append(allTabAction);
    actions.append(confirmedContactsTabAction);
    actions.append(proposedContactsAction);
    actions.append(landscapeTabAction);

    MToolBar* toolbar = new MToolBar();
    toolbar->setStyleName("MToolbarTabStyleInverted");
    toolbar->setOpacity(0.9);
    toolbar->setViewType(MToolBar::tabType);
    toolbar->addActions(actions);

    m_toolbarAction = new MWidgetAction(q);
    m_toolbarAction->setLocation(MAction::ToolBarLocation);
    m_toolbarAction->setWidget(toolbar);

    m_confirmFaceAction = new MAction("Confirm faces", q);
    m_confirmFaceAction->setLocation(MAction::ApplicationMenuLocation);
    m_deleteFaceAction = new MAction("Delete faces", q);
    m_deleteFaceAction->setLocation(MAction::ApplicationMenuLocation);
    m_aboutAction = new MAction("About", q);
    m_aboutAction->setLocation(MAction::ApplicationMenuLocation);

    const QPixmap *pixmap = MTheme::pixmap("icon-m-toolbar-view-menu-dimmed-white");
    MImageWidget *menuImage = new MImageWidget();
    menuImage->setImage(pixmap->toImage());
    menuImage->setZoomFactor(1.f);
    menuImage->setMinimumWidth(88.f);
    MTheme::releasePixmap(pixmap);
    MWidgetAction* fakeAction= new MWidgetAction(q);
    fakeAction->setLocation(MAction::ToolBarLocation);
    fakeAction->setWidget(menuImage);

    m_confirmedContactsListPage->addAction(m_toolbarAction);
    m_confirmedContactsListPage->addAction(fakeAction);

    m_proposedContactsListPage->addAction(m_toolbarAction);
    m_proposedContactsListPage->addAction(fakeAction);

    connect(m_confirmedContactsListPage, SIGNAL(personSelected(QString,QString)),
            this, SLOT(onConfirmedContactSelected(QString,QString)));
    connect(m_proposedContactsListPage, SIGNAL(personSelected(QString,QString)),
            this, SLOT(onProposedContactPersonSelected(QString,QString)));
    connect(m_gridPage, SIGNAL(multiSelectionDone(QList<QUrl>)),
            this, SLOT(onMultiSelectionDone(QList<QUrl>)));
    connect(m_gridPage, SIGNAL(itemSelected(QUrl)),
            this, SLOT(onItemSelected(QUrl)));
    connect(m_fullScreenPage, SIGNAL(itemDeleted(QUrl)),
            m_faceDatabaseProvider, SLOT(update()));
    connect(m_trackerProvider, SIGNAL(dataChanged()),
            this, SLOT(onDataChanged()));
    connect(m_confirmFaceAction, SIGNAL(triggered()),
            this, SLOT(onConfirmFaceActionTriggered()));
    connect(m_deleteFaceAction, SIGNAL(triggered()),
            this, SLOT(onDeleteFaceActionTriggered()));
    connect(m_aboutAction, SIGNAL(triggered()),
            this, SLOT(onAboutActionTriggered()));
    connect(allTabAction, SIGNAL(toggled(bool)),
            this, SLOT(onAllTabActionToggled(bool)));
    connect(confirmedContactsTabAction, SIGNAL(toggled(bool)),
            this, SLOT(onConfirmedContactTabActionToggled(bool)));
    connect(proposedContactsAction, SIGNAL(toggled(bool)),
            this, SLOT(onProposedContactTabActionToggled(bool)));
    connect(landscapeTabAction, SIGNAL(toggled(bool)),
            this, SLOT(onLandscapeTabActionToggled(bool)));
    connect(m_gridPage, SIGNAL(appeared()),
            this, SLOT(onGridPageAppeared()));
    connect(m_fullScreenPage, SIGNAL(appeared()),
            this, SLOT(onFullscreenPageAppeared()));
    connect(m_fullScreenPage, SIGNAL(loadingActiveItemFailed(QString)),
            m_fullScreenPage, SLOT(disappear()));
    connect(m_galleryModel, SIGNAL(galleryItemsRemoved(int,int)),
            this, SLOT(onGalleryItemsRemoved()));

    allTabAction->toggle();
}

LookAlikeMainPrivate::~LookAlikeMainPrivate()
{
    m_galleryModel->removeContentProvider(m_trackerProvider);
    delete m_proposedContactsListPage;
    delete m_confirmedContactsListPage;
    delete m_gridPage;
    delete m_aboutAction;
    delete m_faceRecognitionResetter;
}

QString LookAlikeMainPrivate::urnFromUrl(QUrl url)
{
    QAbstractItemModel *model = m_trackerProvider->model();
    QString urlString = url.toString();
    for (int i = 0; i < model->rowCount(); i++) {
        if (model->index(i, GalleryContentProvider::UrlColumn).data().toString() == urlString) {
            return model->index(i, GalleryContentProvider::UrnColumn).data().toString();
        }
    }
    return QString();
}

XQFaceRegion LookAlikeMainPrivate::findRegion(QString &faceId, QString &sourceId)
{
    QList<XQFaceRegion> regions = m_faceDatabaseProvider->getRegions(faceId);

    foreach (XQFaceRegion r, regions) {
        if (r.sourceId() == sourceId) {
            return r;
        }
    }

    //We shouldn't reach this point
    Q_ASSERT(false);
    return regions.at(0);
}

QRect LookAlikeMainPrivate::scaleRect(const QRect &rect, QSize &fromSize, QSize &toSize)
{
    if (fromSize == toSize) {
        return rect;
    }

    qreal xFactor = fabs((qreal) toSize.width() / (qreal) fromSize.width());
    qreal yFactor = fabs((qreal) toSize.height() / (qreal) fromSize.height());
    int x = xFactor * rect.topLeft().x();
    int y = yFactor * rect.topLeft().y();
    int width = xFactor * rect.width();
    int height = yFactor * rect.height();

    return QRect(x, y, width, height);
}

void LookAlikeMainPrivate::updateTrackerFilter()
{
    m_galleryModel->removeContentProvider(m_trackerProvider);
    m_galleryModel->addContentProvider(m_trackerProvider);
}

void LookAlikeMainPrivate::updateGrid()
{
    m_gridPage->showTopBar(false);
    m_gridPage->removeAction(m_confirmFaceAction);
    m_gridPage->removeAction(m_deleteFaceAction);
    //Remove and insert again to ensure it is always inserted only once
    m_gridPage->removeAction(m_toolbarAction);
    m_gridPage->addAction(m_toolbarAction);
    m_gridPage->resetToDefaultState();
}

void LookAlikeMainPrivate::updateGrid(const QString &displayName, MAction *addAction)
{
    m_gridPage->setTopBarText(displayName);
    m_gridPage->showTopBar(true);
    m_gridPage->removeAction(m_toolbarAction);
    //Remove and insert again to ensure it is always the first action
    m_gridPage->removeAction(m_confirmFaceAction);
    m_gridPage->removeAction(m_deleteFaceAction);
    if (addAction != 0) {
        if (m_gridPage->actions().isEmpty()) {
            m_gridPage->addAction(addAction);
        } else {
            m_gridPage->insertAction(m_gridPage->actions().first(), addAction);
        }
    }
    m_gridPage->resetToDefaultState();
}

void LookAlikeMainPrivate::updateUrnImages(QList<XQFaceRegion> regions)
{
    QSet<QString> urnImages;
    foreach(XQFaceRegion region, regions) {
        urnImages << region.sourceId();
    }
    m_trackerProvider->setUrns(urnImages);
}

void LookAlikeMainPrivate::showPage(MApplicationPage *page, bool history)
{
    MApplication::activeApplicationWindow()->sceneManager()->appearSceneWindowNow(page);
    if (!history) {
        MApplication::activeWindow()->sceneManager()->setPageHistory(QList<MSceneWindow*>());
    }
}

void LookAlikeMainPrivate::deleteFace(QUrl picture, QString& contact)
{
    //Remove face from Face Recognizer database
    QString urnImage = urnFromUrl(picture);
    m_faceDatabaseProvider->hideRegion(contact, urnImage);

    //Update XMP metadata image, removing the deleted face
    QString fileName = picture.toLocalFile();
    QuillMetadata metadata(fileName, QuillMetadata::XmpFormat, QuillMetadata::Tag_Regions);
    QVariant regionListVariant= metadata.entry(QuillMetadata::Tag_Regions);
    QuillMetadataRegionList regionList = regionListVariant.value<QuillMetadataRegionList>();
    //Let's remove the areas for this contact
    int i = 0;
    while (i < regionList.size()) {
        QuillMetadataRegion metadataRegion = regionList.at(i);
        QString regionContact = metadataRegion.extension("nco:PersonContact").toString();
        if (regionContact == contact) {
            regionList.removeAt(i);
        } else {
            i++;
        }
    }
    //Save remaining regions
    saveMetadataRegionList(fileName, regionList);
}

void LookAlikeMainPrivate::confirmFace(QUrl picture, QString& contact)
{
    //Confirm in Face Recognizer database
    QString urnImage = urnFromUrl(picture);
    XQFaceRegion region = findRegion(contact, urnImage);
    region.setFaceId(contact);

    //Update XMP metadata image
    QString fileName = picture.toLocalFile();
    QImage image(fileName);
    //Scale face to image dimensions
    QSize originalSize = image.size();
    QSize thumbnailSize = region.sourceImageSize();
    QRect rect = region.faceRect();
    QRect scaledRect = scaleRect(rect, thumbnailSize, originalSize);
    QuillMetadataRegion metadataRegion;
    metadataRegion.setArea(scaledRect);
    metadataRegion.setType(QuillMetadataRegion::RegionType_Face);
    QString contactName = m_faceDatabaseProvider->getContactName(contact);
    metadataRegion.setName(contactName);
    metadataRegion.setExtension("nco:PersonContact", contact);
    QuillMetadataRegionList regionList;
    regionList.append(metadataRegion);
    saveMetadataRegionList(fileName, regionList);
}

void LookAlikeMainPrivate::onProposedContactPersonSelected(const QString &personId, const QString &displayName)
{
    m_personSelected = personId;
    if (m_personSelected == UNKNOWN_CONTACT) {
        updateGrid(displayName);
    } else {
        updateGrid(displayName, m_confirmFaceAction);
    }
    updateUrnImages(m_faceDatabaseProvider->getRegions(m_personSelected));
    m_trackerProvider->setContentType(TrackerContentProvider::ListImages);
    updateTrackerFilter();
    showPage(m_gridPage, true);
}

void LookAlikeMainPrivate::onConfirmedContactSelected(const QString &personId, const QString &displayName)
{
    m_personSelected = personId;
    updateGrid(displayName, m_deleteFaceAction);
    if (personId == EVERYBODY_CONTACT) {
        m_trackerProvider->setContact(QString());
    } else {
        m_trackerProvider->setContact(personId);
    }
    m_trackerProvider->setContentType(TrackerContentProvider::WithContact);
    updateTrackerFilter();
    showPage(m_gridPage, true);
}

void LookAlikeMainPrivate::onConfirmFaceActionTriggered()
{
    m_currentAction = m_confirmFaceAction;
    m_gridPage->startMultiSelection("Confirm");
}

void LookAlikeMainPrivate::onDeleteFaceActionTriggered()
{
    m_currentAction = m_deleteFaceAction;
    m_gridPage->startMultiSelection("Delete");
}

void LookAlikeMainPrivate::onAboutActionTriggered()
{
    static MApplicationPage *aboutPage = 0;
    if (aboutPage == 0) {
        const QString title = "LookAlike";
        const QString version = "0.0.1";
        const QString copyright = "Copyright (c) 2012 Igalia S.L.";
        const QString contact = "<a href=\"mailto:agomez@igalia.com\">agomez@igalia.com</a> | "
                                "<a href=\"http://www.igalia.com\">www.igalia.com</a>";
        const QString license = "This program is free software: you can redistribute it and/or modify "
                                "it under the terms of the GNU Lesser General Public License "
				" as published by the Free Software Foundation; version 2.1 of "
				" the License, or (at your option) any later version.<br /><br />"
                                "This package is distributed in the hope that it will be useful, "
                                "but WITHOUT ANY WARRANTY; without even the implied warranty of "
                                "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
                                "GNU General Public License for more details.<br /><br />"
                                "You should have received a copy of the GNU General Public License "
                                "along with this program. If not, see "
                                "<a href=\"http://www.gnu.org/licenses\">http://www.gnu.org/licenses</a><br /><br />";

        aboutPage = new MApplicationPage();
        aboutPage->setStyleName("GalleryPage");
        aboutPage->setTitle(title);
        QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical, aboutPage->centralWidget());
        MImageWidget *aboutIcon = new MImageWidget();
        aboutIcon->setImage(QImage("/usr/share/icons/hicolor/64x64/apps/icon-l-lookalike.png"));
        MLabel *aboutTitle = new MLabel(title + " " + version);
        aboutTitle->setAlignment(Qt::AlignCenter);
        aboutTitle->setColor("white");
        MLabel *aboutCopyright = new MLabel(copyright);
        aboutCopyright->setAlignment(Qt::AlignCenter);
        aboutCopyright->setColor("white");
        MLabel *aboutContact = new MLabel(contact);
        aboutContact->setAlignment(Qt::AlignCenter);
        aboutContact->setColor("white");
        MLabel *aboutLicense = new MLabel(license);
        aboutLicense->setAlignment(Qt::AlignJustify);
        aboutLicense->setWordWrap(true);
        aboutLicense->setFont(QFont("Nokia Pure Text Light"));
        aboutLicense->setColor("white");
        layout->addItem(aboutIcon);
        layout->addItem(aboutTitle);
        layout->addItem(aboutCopyright);
        layout->addItem(aboutContact);
        layout->addItem(aboutLicense);
    }
    showPage(aboutPage, true);
}

void LookAlikeMainPrivate::onMultiSelectionDone(QList<QUrl> urlList)
{
    m_facesToProcess = urlList;
    m_progress = new MProgressIndicator(0, MProgressIndicator::barType);
    m_progress->setStyleName(("CommonProgressBarInverted"));
    m_progress->setRange(0, urlList.size());
    m_progressDialog = new MDialog();
    m_progressDialog->setCentralWidget(m_progress);
    m_progressDialog->appear(MSceneWindow::DestroyWhenDone);

    connect(m_progressDialog, SIGNAL(appeared()),
            this, SLOT(processFaces()),
            Qt::QueuedConnection);
    connect(m_progress, SIGNAL(valueChanged(int)),
            this, SLOT(processFaces()),
            Qt::QueuedConnection);
    connect(m_progressDialog, SIGNAL(rejected()),
            this, SLOT(onProgressDialogRejected()));
}

void LookAlikeMainPrivate::processFaces()
{
    if (m_facesToProcess.isEmpty()) {
        m_progressDialog->dismiss();
    } else {
        QUrl toProcess = m_facesToProcess.first();
        if (m_currentAction == m_confirmFaceAction) {
            confirmFace(toProcess, m_personSelected);
        } else {
            deleteFace(toProcess, m_personSelected);
        }
        m_facesToProcess.removeFirst();
        m_progress->setValue(m_progress->value() + 1);
    }
}

void LookAlikeMainPrivate::saveMetadataRegionList(QString &fileName, QuillMetadataRegionList &regionList)
{
    QuillMetadata metadata(fileName, QuillMetadata::XmpFormat);
    QVariant variant;
    variant.setValue(regionList);
    metadata.setEntry(QuillMetadata::Tag_Regions, variant);
    metadata.write(fileName, QuillMetadata::XmpFormat);
}

void LookAlikeMainPrivate::onItemSelected(const QUrl &url)
{
    m_fullScreenPage->resetToDefaultState();
    m_fullScreenPage->moveToItem(url);
    m_fullScreenPage->appear(MApplication::activeWindow());
    m_fullScreenPage->setNavigationBarVisible(false);
}

void LookAlikeMainPrivate::onDataChanged()
{
    m_faceDatabaseProvider->update();
    if (m_trackerProvider->contentType() == TrackerContentProvider::FilterImagesNoFace) {
        updateUrnImages(m_faceDatabaseProvider->getRegions());
    } else {
        updateUrnImages(m_faceDatabaseProvider->getRegions(m_personSelected));
    }
    updateTrackerFilter();
}

void LookAlikeMainPrivate::onAllTabActionToggled(bool toggled)
{
    if (toggled) {
        m_personSelected = UNKNOWN_CONTACT;
        updateGrid();
        m_trackerProvider->setContentType(TrackerContentProvider::AllImages);
        updateTrackerFilter();
        showPage(m_gridPage);
    }
}

void LookAlikeMainPrivate::onProposedContactTabActionToggled(bool toggled)
{
    if (toggled) {
        m_personSelected = UNKNOWN_CONTACT;
        showPage(m_proposedContactsListPage);
    }
}

void LookAlikeMainPrivate::onConfirmedContactTabActionToggled(bool toggled)
{
    if (toggled) {
        m_personSelected = UNKNOWN_CONTACT;
        showPage(m_confirmedContactsListPage);
    }
}

void LookAlikeMainPrivate::onLandscapeTabActionToggled(bool toggled)
{
    if (toggled) {
        m_personSelected = UNKNOWN_CONTACT;
        updateGrid();
        updateUrnImages(m_faceDatabaseProvider->getRegions());
        m_trackerProvider->setContentType(TrackerContentProvider::FilterImagesNoFace);
        updateTrackerFilter();
        showPage(m_gridPage);
    }
}

void LookAlikeMainPrivate::onProgressDialogRejected()
{
    m_facesToProcess.clear();
}

void LookAlikeMainPrivate::onGridPageAppeared()
{
    m_gridPage->addAction(m_aboutAction);
}

void LookAlikeMainPrivate::onFullscreenPageAppeared()
{
    m_fullScreenPage->addAction(m_aboutAction);
}

void LookAlikeMainPrivate::onGalleryItemsRemoved()
{
    if (m_gridPage->itemCount() == 0 &&
        MApplication::activeApplicationWindow()->currentPage() == m_fullScreenPage) {
        m_fullScreenPage->disappear();
    }
}
