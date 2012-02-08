#ifndef LOOKALIKEMAIN_P_H
#define LOOKALIKEMAIN_P_H

#include "lookalikemain.h"
#include <QObject>
#include <QAbstractItemModel>

class GalleryPeopleListPage;
class MApplicationPage;
class XQFaceDatabase;
class FaceDatabaseProvider;

class LookAlikeMainPrivate : public QObject
{
    Q_OBJECT
public:
    explicit LookAlikeMainPrivate(QObject *parent = 0);
    void showPage(/*MApplicationPage *page*/);
    void createPeopleListPage();

    GalleryPeopleListPage *m_peopleListPage;
    FaceDatabaseProvider *m_faceDatabaseProvider;

private:
    LookAlikeMain *q_ptr;
    Q_DECLARE_PUBLIC(LookAlikeMain);
};

#endif // LOOKALIKEMAIN_P_H
