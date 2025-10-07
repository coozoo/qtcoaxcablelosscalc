#ifndef CABLEMODEL_H
#define CABLEMODEL_H

#include <QObject>
#include <QJsonObject>
#include <QString>
#include <vector>
#include "spline.h"

class CableModel : public QObject
{
    Q_OBJECT
public:
    explicit CableModel(const QJsonObject &jsonData, QObject *parent = nullptr);

    QString getName() const;
    QString getManufacturer() const;
    QString getType() const;
    QString getDataSource() const;
    QJsonObject getAdditionalInfo() const;

    double getAttenuationPer100m(double frequencyMHz) const;
    // New method to check for extrapolation
    double getMaxFrequency() const;

private:
    void setupSpline(const QJsonObject &attenuationData);

    QString m_name;
    QString m_manufacturer;
    QString m_type;
    QString m_dataSource;
    QJsonObject m_additionalInfo;

    tk::spline m_spline;
    bool m_splineValid = false;
    double m_maxFrequency = 0.0; // Store the max known frequency
};

#endif // CABLEMODEL_H
