#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtCore/QDebug>

#include "Tesla.hxx"
#include "Vehicle.hxx"
#include "Credentials.hxx"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    auto account = new Tesla { USERNAME, PASSWORD };

    QObject::connect(
        account,
        &Tesla::vehiclesChanged,
        [=](const QVector<Vehicle*> vehicles) {
            for (auto vehicle : vehicles) {
                qDebug() << vehicle->vin() << vehicle->name();
            }
        });

    account->requestVehicles();


    /*QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    */
    return app.exec();
}
