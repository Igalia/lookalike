#ifndef FACEAUTORECOGNIZERMAIN_P_H
#define FACEAUTORECOGNIZERMAIN_P_H

#include "faceautorecognizermain.h"
#include <QObject>
#include <QAbstractItemModel>

class FaceAutorecognizerMain;
class GalleryPeopleListPage;
class MApplicationPage;

class EmptyModel: public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit EmptyModel(QObject *parent = 0);
    ~EmptyModel();
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
};

class FaceAutorecognizerMainPrivate : public QObject
{
    Q_OBJECT
public:
    explicit FaceAutorecognizerMainPrivate(QObject *parent = 0);
    void showPage(/*MApplicationPage *page*/);
    void createPeopleListPage();

    GalleryPeopleListPage *m_peopleListPage;
signals:

public slots:

private:
    FaceAutorecognizerMain *q_ptr;
    Q_DECLARE_PUBLIC(FaceAutorecognizerMain);
};

#endif // FACEAUTORECOGNIZERMAIN_P_H
