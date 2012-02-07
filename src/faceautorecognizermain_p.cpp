#include "faceautorecognizermain_p.h"
#include "gallerypeoplelistpage.h"
#include <MApplication>
#include <MApplicationWindow>
#include <MSceneManager>
#include <MApplicationPage>
#include <QAbstractItemModel>

EmptyModel::EmptyModel(QObject *parent) :
    QAbstractItemModel(parent)
{
}

EmptyModel::~EmptyModel()
{
}

QVariant EmptyModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(index);
    Q_UNUSED(role);
    return QVariant();
}

QModelIndex EmptyModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(parent);

    return QModelIndex();
}

QModelIndex EmptyModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

int EmptyModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 0;
}

int EmptyModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 5;
}

FaceAutorecognizerMainPrivate::FaceAutorecognizerMainPrivate(QObject *parent) :
    QObject(parent),
    m_peopleListPage(0)
{
}

void FaceAutorecognizerMainPrivate::showPage(/*MApplicationPage *page*/)
{
    if (m_peopleListPage && MApplication::activeApplicationWindow() && MApplication::activeApplicationWindow()->sceneManager()) {
        MApplication::activeApplicationWindow()->sceneManager()->appearSceneWindowNow(m_peopleListPage);
    }
}

void FaceAutorecognizerMainPrivate::createPeopleListPage()
{
    if (!m_peopleListPage) {
        m_peopleListPage = new GalleryPeopleListPage(new EmptyModel());
        m_peopleListPage->setStyleName("GalleryPage");
    }
}
