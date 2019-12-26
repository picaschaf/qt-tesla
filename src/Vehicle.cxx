#include "Vehicle.hxx"


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


Vehicle::Vehicle(const QString& vin, QObject* parent)
    : QObject(parent)
    , d_ptr(new VehiclePrivate { this })
{
    Q_D(Vehicle);

    d->vin = vin;
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
