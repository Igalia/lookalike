#include "facedatabaseprovider.h"
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
#include <MSceneManager>
#include <QAbstractItemModel>
#include <QSparqlConnection>
#include <QuillMetadata>
#include <QuillMetadataRegion>
#include <QuillMetadataRegionList>
#include <xqfacedatabase.h>

LookAlikeMainPrivate::LookAlikeMainPrivate(LookAlikeMain *q) :
    QObject(q),
    q_ptr(q)
{
    m_galleryModel = new GalleryModel(this);
    m_galleryModel->setFaceRecognitionEnabled(true);
    QSparqlConnection *connection = m_galleryModel->sparqlConnection();
    m_trackerProvider = new TrackerContentProvider(connection, this);
    m_galleryModel->addContentProvider(m_trackerProvider);
    m_faceDatabaseProvider = new FaceDatabaseProvider(connection, this);
    m_peopleListPage = new GalleryPeopleListPage(m_faceDatabaseProvider);
    m_gridPage = new GalleryGridPage(*m_galleryModel);
    m_fullScreenPage = new GalleryFullScreenPage(*m_galleryModel);
    m_confirmFaceAction = new MAction("Confirm faces", this);
    m_confirmFaceAction->setLocation(MAction::ApplicationMenuLocation);
    m_confirmFaceAction->setObjectName("ConfirmFacesAction");
    m_gridPage->addAction(m_confirmFaceAction);
    connect(m_peopleListPage, SIGNAL(personSelected(QString,QString)),
            this, SLOT(onPersonSelected(QString,QString)));
    connect(m_confirmFaceAction, SIGNAL(triggered()),
            this, SLOT(onConfirmFaceActionTriggered()));
    connect(m_gridPage, SIGNAL(multiSelectionDone(QList<QUrl>)),
            this, SLOT(onMultiSelectionDone(QList<QUrl>)));
    connect(m_gridPage, SIGNAL(itemSelected(QUrl)),
            this, SLOT(onItemSelected(QUrl)));
    connect(m_fullScreenPage, SIGNAL(itemDeleted(QUrl)),
            m_faceDatabaseProvider, SLOT(update()));
}

LookAlikeMainPrivate::~LookAlikeMainPrivate()
{
    m_galleryModel->removeContentProvider(m_trackerProvider);
    delete m_peopleListPage;
    delete m_gridPage;
}

void LookAlikeMainPrivate::showPage()
{
    if (m_peopleListPage && MApplication::activeApplicationWindow() && MApplication::activeApplicationWindow()->sceneManager()) {
        MApplication::activeApplicationWindow()->sceneManager()->appearSceneWindowNow(m_peopleListPage);
    }
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

void LookAlikeMainPrivate::updateTrackerFilter(const QString &personId)
{
    m_galleryModel->removeContentProvider(m_trackerProvider);
    QList<XQFaceRegion> regions = m_faceDatabaseProvider->getRegions(personId);
    QSet<QString> urnImages;
    foreach(XQFaceRegion region, regions) {
        urnImages << region.sourceId();
    }
    m_trackerProvider->setUrns(urnImages);
    m_galleryModel->addContentProvider(m_trackerProvider);
}

void LookAlikeMainPrivate::onPersonSelected(const QString &personId, const QString &displayName)
{
    m_personSelected = personId;
    m_gridPage->setTopBarText(displayName);
    m_gridPage->showTopBar(true);
    m_gridPage->resetToDefaultState();
    updateTrackerFilter(personId);
    m_gridPage->appear(MApplication::activeWindow());
}

void LookAlikeMainPrivate::onConfirmFaceActionTriggered()
{
    m_gridPage->startMultiSelection("Confirm");
}

void LookAlikeMainPrivate::onMultiSelectionDone(QList<QUrl> urlList)
{
    for (int i = 0; i < urlList.size(); i++) {
        QString contactName = m_faceDatabaseProvider->getContactName(m_personSelected);
        QString fileName = urlList.at(i).toLocalFile();
        QString urnImage = urnFromUrl(urlList.at(i));
        XQFaceRegion region = findRegion(m_personSelected, urnImage);
        region.setFaceId(m_personSelected);
        QImage image(fileName);
        QSize originalSize = image.size();
        QSize thumbnailSize = region.sourceImageSize();
        QRect rect = region.faceRect();
        QRect scaledRect = scaleRect(rect, thumbnailSize, originalSize);
        QuillMetadataRegion metadataRegion;
        metadataRegion.setArea(scaledRect);
        metadataRegion.setType(QuillMetadataRegion::RegionType_Face);
        metadataRegion.setName(contactName);
        metadataRegion.setExtension("nco:PersonContact", m_personSelected);
        QuillMetadataRegionList regionList;
        regionList.append(metadataRegion);
        QuillMetadata metadata(fileName, QuillMetadata::XmpFormat);
        QVariant variant;
        variant.setValue(regionList);
        metadata.setEntry(QuillMetadata::Tag_Regions, variant);
        metadata.write(fileName, QuillMetadata::XmpFormat);
    }

    m_faceDatabaseProvider->update();
    updateTrackerFilter(m_personSelected);
}

void LookAlikeMainPrivate::onItemSelected(const QUrl &url)
{
    m_fullScreenPage->resetToDefaultState();
    m_fullScreenPage->moveToItem(url);
    m_fullScreenPage->appear(MApplication::activeWindow());
    m_fullScreenPage->setNavigationBarVisible(false);
}
