#ifndef VEHICLE_HXX
#define VEHICLE_HXX

#include <QtCore/QObject>

class VehiclePrivate;


class Vehicle : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Vehicle)
    Q_DISABLE_COPY(Vehicle)

public:
    Vehicle(const QString& vin, QObject* parent = nullptr);

    QString vin() const;
    QString apiVersion() const;
    QString name() const;
    bool isCalendarEnabled() const;
    quint64 id() const;
    QString ids() const;
    bool isInService() const;
    QStringList options() const;
    QString status() const;
    QStringList tokens() const;
    quint32 vehicleId() const;

public slots:
    void setApiVersion(const QString& version);
    void setName(const QString& name);
    void setCalendarEnabled(bool enabled = true);
    void setId(quint64 id);
    void setIds(const QString& ids);
    void setInService(bool service = true);
    void setOptions(const QStringList& options);
    void setStatus(const QString& status);
    void setTokens(const QStringList& tokens);
    void setVehicleId(quint32 id);

signals:
    void apiVersionChanged(const QString& version);
    void nameChanged(const QString& name);
    void calendarEnabledChanged(bool enabled);
    void idChanged(quint64 id);
    void idsChanged(const QString& ids);
    void inServiceChanged(bool service);
    void optionsChanged(const QStringList& options);
    void statusChanged(const QString& status);
    void tokensChanged(const QStringList& tokens);
    void vehicleIdChanged(quint32 id);

private:
    VehiclePrivate* d_ptr = nullptr;
};

#endif
