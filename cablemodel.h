#ifndef CABLEMODEL_H
#define CABLEMODEL_H

#include <QObject>
#include <QJsonObject>
#include <QString>


class CableModel : public QObject
{
    Q_OBJECT
public:
    explicit CableModel(const QJsonObject &jsonData, QObject *parent = nullptr);
    ~CableModel();

    QString getName() const;
    QString getManufacturer() const;
    QString getType() const;
    QString getDataSource() const;
    QJsonObject getAdditionalInfo() const;

    double getAttenuationPer100m(double frequencyMHz) const;
    double getMaxFrequency() const;

private:
    void setupSpline(const QJsonObject &attenuationData);

    QString m_name;
    QString m_manufacturer;
    QString m_type;
    QString m_dataSource;
    QJsonObject m_additionalInfo;

    void* m_spline;
    bool m_splineValid = false;
    double m_maxFrequency = 0.0;
};

#endif // CABLEMODEL_H
