#include "lookalikemain_p.h"
#include "gallerypeoplelistpage.h"
#include "facedatabaseprovider.h"
#include <MApplication>
#include <MApplicationWindow>
#include <MSceneManager>
#include <MApplicationPage>
#include <QAbstractItemModel>
#include <xqfacedatabase.h>

LookAlikeMainPrivate::LookAlikeMainPrivate(QObject *parent) :
    QObject(parent),
    m_peopleListPage(0)
{
    m_faceDatabaseProvider = new FaceDatabaseProvider(this);
}

void LookAlikeMainPrivate::showPage()
{
    if (m_peopleListPage && MApplication::activeApplicationWindow() && MApplication::activeApplicationWindow()->sceneManager()) {
        MApplication::activeApplicationWindow()->sceneManager()->appearSceneWindowNow(m_peopleListPage);
    }
}

void LookAlikeMainPrivate::createPeopleListPage()
{
    if (!m_peopleListPage) {
        m_peopleListPage = new GalleryPeopleListPage(m_faceDatabaseProvider);
        m_faceDatabaseProvider->update();
    }
}
