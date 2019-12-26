#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

#include <Tesla>
#include <Vehicle>


int main(int argc, char** argv)
{
    QCoreApplication app { argc, argv };
    auto account = new Tesla { USERNAME, PASSWORD };

    QObject::connect(
        account,
        &Tesla::vehiclesChanged,
        [=](const QVector<Vehicle*> vehicles) {
            for (auto vehicle : vehicles) {
                qDebug() << vehicle->vin() << vehicle->name();
            }

            account->deleteLater();
            qApp->quit();
        });

    account->requestVehicles();


    return app.exec();
}
