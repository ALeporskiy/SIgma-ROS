#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>



#include "trajectorymanager.h"



int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    qmlRegisterType<TrajectoryManager>("SpaceSim", 1, 0, "TrajectoryManager");

    qRegisterMetaType<QGeoCoordinate>("QGeoCoordinate");
    engine.loadFromModule("untitled1", "Main");

    return app.exec();
}
