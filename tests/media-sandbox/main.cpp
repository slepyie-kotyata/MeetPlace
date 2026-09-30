#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "media/LocalCameraManager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    LocalCameraManager cameraManager;
    qmlRegisterSingletonInstance("MeetPlace.Core", 1, 0, "LocalCameraManager", &cameraManager);

    QQmlApplicationEngine engine;
    engine.loadFromModule("MediaSandbox", "Main");
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}