#include "lookalikemain.h"
#include <MApplication>
#include <MApplicationWindow>
#include <MComponentCache>
#include <MExport>

M_EXPORT int main(int argc, char *argv[])
{
    MApplication *application = MComponentCache::mApplication(argc, argv, "lookalike");
    MApplicationWindow *window = MComponentCache::mApplicationWindow();
    window->setStyleName(QLatin1String("CommonApplicationWindowInverted"));
    window->setWindowTitle("Look alike");
    LookAlikeMain *main = new LookAlikeMain();
    window->show();

    int result = application->exec();

    delete main;
    delete window;
    delete application;

    return result;
}
