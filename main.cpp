#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "Tesla.hxx"
#include "Credentials.hxx"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    auto account = new Tesla { USERNAME, PASSWORD };
    account->authenticate();

/*
    auto request2 = QNetworkRequest(QString("%1/vehicles").arg(apiUrl));
    request2.setRawHeader("Authorization", QString("Bearer %1").arg(token).toLatin1());

    auto response2 = qnam.get(request2);
    while (!response2->isFinished())
    {
        qApp->processEvents();
    }
    auto name = QJsonDocument::fromJson(response2->readAll()).toVariant().toMap()["response"].toList().first().toMap()["display_name"].toString();
    qDebug() << name;
*/
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
