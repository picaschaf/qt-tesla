#ifndef TESLA_HXX
#define TESLA_HXX

#include <QtCore/QObject>
#include <QtCore/QVector>

class TeslaPrivate;

class Vehicle;


class Tesla : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Tesla)
    Q_DISABLE_COPY(Tesla)
    Q_PROPERTY(QString username
               READ username
               WRITE setUsername
               NOTIFY usernameChanged)
    Q_PROPERTY(QString password
               READ password
               WRITE setPassword
               NOTIFY passwordChanged)
    Q_PROPERTY(QString token
               READ token
               WRITE setToken
               NOTIFY tokenChanged)

public:
    explicit Tesla(const QString& username = QString(),
                   const QString& password = QString(),
                   const QString& token = QString(),
                   QObject* parent = nullptr);

    QString username() const;
    QString password() const;
    QString token() const;

public slots:
    void setUsername(const QString& username);
    void setPassword(const QString& password);
    void setToken(const QString& token);

    void authenticate();

    void requestVehicles();

signals:
    void usernameChanged(const QString& username);
    void passwordChanged(const QString& password);
    void tokenChanged(const QString& token);

    void authenticated();
    void error() const;

    void vehiclesChanged(const QVector<Vehicle*> vehicles);

private:
    TeslaPrivate* d_ptr = nullptr;
};

#endif
