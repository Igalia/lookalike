#include "faceautorecognizermain.h"
#include <MApplication>
#include <MApplicationWindow>
#include <MComponentCache>
#include <MExport>

M_EXPORT int main(int argc, char *argv[])
{
    MApplication *application = MComponentCache::mApplication(argc, argv, "faceautorecognizer");
    MApplicationWindow *window = MComponentCache::mApplicationWindow();
    window->setWindowTitle("Face autorecognizer");
    FaceAutorecognizerMain *main = new FaceAutorecognizerMain();
    window->show();

    int result = application->exec();

    delete main;
    delete window;
    delete application;

    return result;
}
