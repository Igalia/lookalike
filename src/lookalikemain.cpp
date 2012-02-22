#include "lookalikemain.h"
#include "lookalikemain_p.h"
#include <MApplication>
#include <MApplicationWindow>

LookAlikeMain::LookAlikeMain(QObject *parent) :
    QObject(parent),
    d_ptr(new LookAlikeMainPrivate(this))
{
}
