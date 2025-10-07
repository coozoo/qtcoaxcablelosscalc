#ifndef QTCOAXCABLELOSSCALCMANAGER_H
#define QTCOAXCABLELOSSCALCMANAGER_H

#include <QWidget>
#include <QList>
#include <QMap>
#include "qcustomplot.h"

class CableModel;
class CableWidget;
class QLineEdit;
class QComboBox;
class QPushButton;
class QVBoxLayout;
class QScrollArea;
class QLabel;
class QGridLayout;
class QResizeEvent;

class QtCoaxCableLossCalcManager : public QWidget
{
    Q_OBJECT

public:
    explicit QtCoaxCableLossCalcManager(QWidget *parent = nullptr);
    ~QtCoaxCableLossCalcManager();

public slots:
    void setFrequency(double frequencyMHz);
    void setGlobalLength(double lengthM);
    void setPlotRange(double lower, double upper);
    // This is the new public property setter
    void setIndividualLengthAllowed(bool allowed);

signals:
    void totalAttenuationChanged(double totalDb);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onAddCableClicked();
    void onDeleteMarkedClicked();
    void onSearchTextChanged(const QString &text);
    void updateAttenuations();
    void replotGraphs();

private:
    void setupUi();
    void loadCablesFromJson();
    void setupPlot();
    void addCableToPlot(CableWidget *cableWidget);
    void removeCableFromPlot(CableWidget *cableWidget);
    void updateGrid();
    void updateTracers();

    QMap<QString, CableModel *> m_cableModels;
    QList<CableWidget *> m_activeCableWidgets;
    double m_globalLength = 1.0;
    bool m_individualLengthAllowed = false; // The new property

    // UI Elements
    QVBoxLayout *m_mainLayout;
    QLineEdit *m_searchEdit;
    QComboBox *m_cableComboBox;
    QPushButton *m_addButton;
    QPushButton *m_deleteButton;
    QScrollArea *m_scrollArea;
    QWidget *m_scrollAreaWidget;
    QGridLayout *m_cableWidgetsLayout;
    QCustomPlot *m_plot;
    QLabel *m_totalAttenuationLabel;

    QCPItemStraightLine *m_frequencyLine;
    QMap<QCPGraph *, QCPItemTracer *> m_extrapolationTracers;
};

#endif // QTCOAXCABLELOSSCALCMANAGER_H
