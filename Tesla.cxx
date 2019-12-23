#include "Tesla.hxx"

#include <QtCore/QDebug>
#include <QtCore/QJsonDocument>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include "Vehicle.hxx"


class TeslaPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(Tesla)
    Q_DISABLE_COPY(TeslaPrivate)

public:
    QString clientId = "81527cff06843c8634fdc09e8ac0abefb46ac849f38fe1e431c2ef2106796384";
    QString clientSecret = "c7257eb71a564034f9419ee651c7d0e5f7aa6bfbd18bafb5c5c033b093bb2fa3";
    QString baseUrl = "https://owner-api.teslamotors.com";
    QString tokenUrl = QString("%1/oauth/token").arg(baseUrl);
    QString apiUrl = QString("%1/api/1").arg(baseUrl);

    QString username;
    QString password;
    QString token;

    bool isAuthenticated = false;

    QNetworkAccessManager qnam;

    TeslaPrivate(Tesla* parent);

private:
    Tesla* q_ptr = nullptr;
};

#include "Tesla.moc"


TeslaPrivate::TeslaPrivate(Tesla* parent)
    : QObject(parent)
    , q_ptr(parent)
{
}


Tesla::Tesla(const QString& username,
             const QString& password,
             const QString& token,
             QObject* parent)
    : QObject(parent)
    , d_ptr(new TeslaPrivate { this })
{
    if (!token.isEmpty()) {
        if (!username.isEmpty() || !password.isEmpty()) {
            emit error();
            return;
        }

        setToken(token);
    }
    else if (!username.isEmpty() && !password.isEmpty()) {
        setUsername(username);
        setPassword(password);
    }
}

QString Tesla::username() const
{
    Q_D(const Tesla);
    return d->username;
}

QString Tesla::password() const
{
    Q_D(const Tesla);
    return d->password;
}

QString Tesla::token() const
{
    Q_D(const Tesla);
    return d->token;
}

void Tesla::requestVehicles()
{
    Q_D(const Tesla);

    // Prevent looping from any previous authentications.
    QObject::disconnect(this,
                        SIGNAL(authenticated()),
                        this,
                        SLOT(requestVehicles()));

    if (!d->isAuthenticated) {
        bool isConnected = false;
        Q_UNUSED(isConnected);

        // Try again after authentication.
        isConnected = QObject::connect(
            this,
            SIGNAL(authenticated()),
            this,
            SLOT(requestVehicles()));
        Q_ASSERT(isConnected);

        authenticate();
    }

    qDebug() << "Requesting vehicles.";
}

void Tesla::setUsername(const QString& username)
{
    Q_D(Tesla);

    d->username = username;
    emit usernameChanged(d->username);
}

void Tesla::setPassword(const QString& password)
{
    Q_D(Tesla);

    d->password = password;
    emit passwordChanged(d->password);
}

void Tesla::setToken(const QString& token)
{
    Q_D(Tesla);

    d->token = token;
    emit tokenChanged(d->token);
}
#include <QGuiApplication>
//! \todo Implement token authentication
void Tesla::authenticate()
{
    Q_D(Tesla);

    QVariantMap tokenRequest;
    tokenRequest["grant_type"] = "password";
    tokenRequest["client_id"] = d->clientId;
    tokenRequest["client_secret"] = d->clientSecret;
    tokenRequest["email"] = d->username;
    tokenRequest["password"] = d->password;

    auto request = QNetworkRequest { d->tokenUrl };
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/json");

    auto data = QJsonDocument::fromVariant(tokenRequest).toJson();
    auto reply = d->qnam.post(request, data);
    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QString token = QJsonDocument::fromJson(reply->readAll()).toVariant().toMap()["access_token"].toString();
    qDebug() << token;
}
