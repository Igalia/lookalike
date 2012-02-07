#include "faceautorecognizermain.h"
#include "faceautorecognizermain_p.h"
#include <MApplication>
#include <MApplicationWindow>

FaceAutorecognizerMain::FaceAutorecognizerMain(QObject *parent) :
    QObject(parent),
    d_ptr(new FaceAutorecognizerMainPrivate(this))
{
    Q_D(FaceAutorecognizerMain);
    d->createPeopleListPage();
    d->showPage(/*d->m_peopleListPage*/);
}
