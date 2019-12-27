#include "Vehicle.hxx"
#include "Tesla.hxx"

//! \todo What is backseat_token and backseat_token_updated_at?
class VehiclePrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(Vehicle)
    Q_DISABLE_COPY(VehiclePrivate)

public:
    QString vin;
    QString apiVersion;
    QString name;
    bool calendarEnabled = false;
    quint64 id = 0;
    QString ids;
    bool inService = false;
    QStringList options;
    QString status;
    QStringList tokens;
    int vehicleId = 0;

    Tesla* api = nullptr;

    VehiclePrivate(Vehicle* parent);

private:
    Vehicle* q_ptr = nullptr;
};

#include "Vehicle.moc"


VehiclePrivate::VehiclePrivate(Vehicle* parent)
    : QObject(parent)
    , q_ptr(parent)
{

}


Vehicle::Vehicle(const QString& vin, Tesla* parent)
    : QObject(parent)
    , d_ptr(new VehiclePrivate { this })
{
    Q_D(Vehicle);

    d->vin = vin;
    d->api = parent;
}

QString Vehicle::vin() const
{
    Q_D(const Vehicle);
    return d->vin;
}

QString Vehicle::apiVersion() const
{
    Q_D(const Vehicle);
    return d->apiVersion;
}

QString Vehicle::name() const
{
    Q_D(const Vehicle);
    return d->name;
}

bool Vehicle::isCalendarEnabled() const
{
    Q_D(const Vehicle);
    return d->calendarEnabled;
}

quint64 Vehicle::id() const
{
    Q_D(const Vehicle);
    return d->id;
}

QString Vehicle::ids() const
{
    Q_D(const Vehicle);
    return d->ids;
}

bool Vehicle::isInService() const
{
    Q_D(const Vehicle);
    return d->inService;
}

QStringList Vehicle::options() const
{
    Q_D(const Vehicle);
    return d->options;
}

QString Vehicle::status() const
{
    Q_D(const Vehicle);
    return d->status;
}

QStringList Vehicle::tokens() const
{
    Q_D(const Vehicle);
    return d->tokens;
}

quint32 Vehicle::vehicleId() const
{
    Q_D(const Vehicle);
    return d->vehicleId;
}

void Vehicle::setApiVersion(const QString& version)
{
    Q_D(Vehicle);

    d->apiVersion = version;
    emit apiVersionChanged(d->apiVersion);
}

void Vehicle::setName(const QString& name)
{
    Q_D(Vehicle);

    d->name = name;
    emit nameChanged(d->name);
}

void Vehicle::setCalendarEnabled(bool enabled)
{
    Q_D(Vehicle);

    d->calendarEnabled = enabled;
    emit calendarEnabledChanged(d->calendarEnabled);
}

void Vehicle::setId(quint64 id)
{
    Q_D(Vehicle);

    d->id = id;
    emit idChanged(d->id);
}

void Vehicle::setIds(const QString& ids)
{
    Q_D(Vehicle);

    d->ids = ids;
    emit idsChanged(d->ids);
}

void Vehicle::setInService(bool service)
{
    Q_D(Vehicle);

    d->inService = service;
    emit inServiceChanged(d->inService);
}

void Vehicle::setOptions(const QStringList& options)
{
    Q_D(Vehicle);

    d->options = options;
    emit optionsChanged(d->options);
}

void Vehicle::setStatus(const QString& status)
{
    Q_D(Vehicle);

    d->status = status;
    emit statusChanged(d->status);
}

void Vehicle::setTokens(const QStringList& tokens)
{
    Q_D(Vehicle);

    d->tokens = tokens;
    emit tokensChanged(d->tokens);
}

void Vehicle::setVehicleId(quint32 id)
{
    Q_D(Vehicle);

    d->vehicleId = id;
    emit vehicleIdChanged(d->vehicleId);
}

void Vehicle::wake()
{
    Q_D(Vehicle);
    d->api->request(QString { "vehicles/%1/wake_up" }.arg(d->id),
                    QByteArray {},
                    RequestType::Post,
                    [=](const QString& response) {
                        Q_UNUSED(response);
                        //! \todo Update vehicle data from response.
                        //!       Deduplicate helper from Tesla.cxx.
                    });
}

void Vehicle::unlockDoors()
{
    Q_D(Vehicle);
    d->api->request(QString { "vehicles/%1/command/door_unlock" }.arg(d->id),
                    QByteArray {},
                    RequestType::Post,
                    [=](const QString& response) {
                        Q_UNUSED(response);
                        //! \todo Do error handling.
                    });
}

void Vehicle::lockDoors()
{
    Q_D(Vehicle);
    d->api->request(QString { "vehicles/%1/command/door_lock" }.arg(d->id),
                    QByteArray {},
                    RequestType::Post,
                    [=](const QString& response) {
                        Q_UNUSED(response);
                        //! \todo Do error handling.
                    });
}

void Vehicle::honkHorn()
{
    Q_D(Vehicle);
    d->api->request(QString { "vehicles/%1/command/honk_horn" }.arg(d->id),
                    QByteArray {},
                    RequestType::Post,
                    [=](const QString& response) {
                        Q_UNUSED(response);
                        //! \todo Do error handling.
                    });
}

void Vehicle::flashLights()
{
    Q_D(Vehicle);
    d->api->request(QString { "vehicles/%1/command/flash_lights" }.arg(d->id),
                    QByteArray {},
                    RequestType::Post,
                    [=](const QString& response) {
                        Q_UNUSED(response);
                        //! \todo Do error handling.
                    });
}

void Vehicle::startHVAC()
{
    Q_D(Vehicle);
    d->api->request(QString { "vehicles/%1/command/auto_conditioning_start" }
                        .arg(d->id),
                    QByteArray {},
                    RequestType::Post,
                    [=](const QString& response) {
                        Q_UNUSED(response);
                        //! \todo Do error handling.
                    });
}

void Vehicle::stopHVAC()
{
    Q_D(Vehicle);
    d->api->request(QString { "vehicles/%1/command/auto_conditioning_stop" }
                        .arg(d->id),
                    QByteArray {},
                    RequestType::Post,
                    [=](const QString& response){
                        Q_UNUSED(response);
                        //! \todo Do error handling.
                    });
}

void Vehicle::setTemperature(float driver, float passenger)
{
    Q_D(Vehicle);
    d->api->request(QString { "vehicles/%1/command/set_temps?"
                              "driver_temp=%2&passenger_temp=%3" }
                        .arg(d->id)
                        .arg(driver)
                        .arg(passenger),
                    QByteArray {},
                    RequestType::Post,
                    [=](const QString& response) {
                        Q_UNUSED(response);
                        //! \todo Do error handling.
                    });
}

void Vehicle::setChargeLimit(int percent)
{
    Q_D(Vehicle);
    d->api->request(QString { "vehicle/%1/command/set_charge_limit?"
                              "percent=%2" }.arg(d->id).arg(percent),
                    QByteArray {},
                    RequestType::Post,
                    [=](const QString& response) {
                        Q_UNUSED(response);
                        //! \todo Do error handling.
                    });
}

void Vehicle::setMaxChargeLimit()
{
    Q_D(Vehicle);
    d->api->request(
        QString { "vehicle/%1/command/charge_max_range" }.arg(d->id),
        QByteArray {},
        RequestType::Post,
        [=](const QString& response) {
            Q_UNUSED(response);
            //! \todo Do error handling.
        });
}

void Vehicle::setDefaultChargeLimit()
{
    Q_D(Vehicle);
    d->api->request(
        QString { "vehicle/%1/command/charge_standard" }.arg(d->id),
        QByteArray {},
        RequestType::Post,
        [=](const QString& response) {
            Q_UNUSED(response);
            //! \todo Do error handling.
        });
}
