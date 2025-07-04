#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>




#include "trajectorymanager.h"
#include "objectlistprovider.h"
#include "SaveHelperLib/savehelperlib.h"



int main(int argc, char *argv[])
{
    qputenv("QT_QUICK_CONTROLS_STYLE", QByteArray("Fusion"));
    QQuickStyle::setStyle("Fusion");
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
    ObjectListProvider *provider = new ObjectListProvider();
    provider->loadFromJson("predefined_objects.json");
    SaveHelper *saveHelper = new SaveHelper();
    TrajectoryManager trajectoryManager;

    engine.rootContext()->setContextProperty("saveProvider", saveHelper);
    engine.rootContext()->setContextProperty("objectListProvider", provider);
    engine.rootContext()->setContextProperty("trajectoryManager", &trajectoryManager);

    engine.loadFromModule("Sigma_ROS", "Main");

    return app.exec();
}
