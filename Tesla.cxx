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
    QString refreshToken;
    QDateTime tokenExpiration;

    QNetworkAccessManager qnam;

    TeslaPrivate(Tesla* parent);

    QByteArray tokenRequestData() const;
    bool isAuthenticated() const;

private:
    Tesla* q_ptr = nullptr;
};

#include "Tesla.moc"


TeslaPrivate::TeslaPrivate(Tesla* parent)
    : QObject(parent)
    , q_ptr(parent)
{
}

QByteArray TeslaPrivate::tokenRequestData() const
{
    QVariantMap tokenRequest;

    if (!token.isEmpty() && QDateTime::currentDateTime() >= tokenExpiration) {
        tokenRequest["grant_type"] = "refresh_token";
        tokenRequest["refresh_token"] = refreshToken;
    }
    else if (!username.isEmpty() && !password.isEmpty()) {
        tokenRequest["grant_type"] = "password";
        tokenRequest["email"] = username;
        tokenRequest["password"] = password;
    }
    else
        return QByteArray();

    tokenRequest["client_id"] = clientId;
    tokenRequest["client_secret"] = clientSecret;

    return QJsonDocument::fromVariant(tokenRequest).toJson();
}

bool TeslaPrivate::isAuthenticated() const
{
    return !token.isEmpty() && QDateTime::currentDateTime() < tokenExpiration;
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

    if (!d->isAuthenticated()) {
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

void Tesla::authenticate()
{
    Q_D(Tesla);

    auto data = d->tokenRequestData();
    if (data.isEmpty())
        return;

    auto request = QNetworkRequest { d->tokenUrl };
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/json");

    auto reply = d->qnam.post(request, data);
    bool isConnected = false;
    Q_UNUSED(isConnected);

    isConnected = QObject::connect(
        reply,
        &QNetworkReply::finished,
        [=]() {
            auto replyData = QJsonDocument::fromJson(reply->readAll());
            auto tokenData = replyData.toVariant().toMap();
            d->token = tokenData["access_token"].toString();
            d->refreshToken = tokenData["refresh_token"].toString();
            d->tokenExpiration = QDateTime::fromTime_t(
                tokenData["created_at"].toUInt()
                + tokenData["expires_in"].toUInt());

            reply->deleteLater();
            emit authenticated();
        });
    Q_ASSERT(isConnected);
}
