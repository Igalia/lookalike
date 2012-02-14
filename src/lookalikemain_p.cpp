#include "lookalikemain_p.h"
#include "gallerypeoplelistpage.h"
#include "gallerygridpage.h"
#include "gallerymodel.h"
#include "facedatabaseprovider.h"
#include "trackercontentprovider.h"
#include <MApplication>
#include <MApplicationWindow>
#include <MSceneManager>
#include <MApplicationPage>
#include <QAbstractItemModel>
#include <xqfacedatabase.h>

LookAlikeMainPrivate::LookAlikeMainPrivate(QObject *parent) :
    QObject(parent)
{
    m_galleryModel = new GalleryModel(this);
    m_trackerProvider = new TrackerContentProvider(this);
    m_galleryModel->addContentProvider(m_trackerProvider);
    m_faceDatabaseProvider = new FaceDatabaseProvider(this);
    m_peopleListPage = new GalleryPeopleListPage(m_faceDatabaseProvider);
    m_gridPage = new GalleryGridPage(*m_galleryModel);
    connect(m_peopleListPage, SIGNAL(personSelected(QString,QString)),
            this, SLOT(onPersonSelected(QString,QString)));
    m_faceDatabaseProvider->update();
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

void LookAlikeMainPrivate::onPersonSelected(const QString &personId, const QString &displayName)
{
    Q_UNUSED(personId);
    m_gridPage->setTopBarText(displayName);
    m_gridPage->showTopBar(true);
    m_gridPage->resetToDefaultState();
    m_galleryModel->removeContentProvider(m_trackerProvider);
    QSet<QString> images;
    if (personId.startsWith("urn:")) {
        images = m_faceDatabaseProvider->getImages(personId);
    } else {
        images = m_faceDatabaseProvider->getImages();
    }
    m_trackerProvider->setUrns(images);
    m_galleryModel->addContentProvider(m_trackerProvider);
    m_gridPage->appear(MApplication::activeWindow());
}
