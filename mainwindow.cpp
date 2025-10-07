#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qtcoaxcablelosscalcmanager.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_manager = new QtCoaxCableLossCalcManager(this);
    m_manager->setIndividualLengthAllowed(true);
    ui->cableManager_groupBox->setLayout(new QVBoxLayout());
    ui->cableManager_groupBox->layout()->addWidget(m_manager);

    setupFrequencyControls();
    setWindowTitle("Coaxial Cable Loss Calculator");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupFrequencyControls()
{
    ui->startFreq_doubleSpinBox->setDecimals(3);
    ui->startFreq_doubleSpinBox->setSingleStep(0.001);
    ui->startFreq_doubleSpinBox->setRange(0.001, 10000.0);

    ui->endFreq_doubleSpinBox->setDecimals(3);
    ui->endFreq_doubleSpinBox->setSingleStep(0.001);
    ui->endFreq_doubleSpinBox->setRange(0.001, 10000.0);

    // Configure the new current frequency spinbox
    ui->currentFreq_doubleSpinBox->setDecimals(3);
    ui->currentFreq_doubleSpinBox->setSingleStep(0.001);

    ui->startFreq_doubleSpinBox->setValue(1.0);
    ui->endFreq_doubleSpinBox->setValue(3000.0);

    connect(ui->startFreq_doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onStartFreqChanged);
    connect(ui->endFreq_doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onEndFreqChanged);

    // Connect both the slider and the new spinbox
    connect(ui->freq_horizontalSlider, &QSlider::valueChanged, this, &MainWindow::onSliderValueChanged);
    connect(ui->currentFreq_doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onCurrentFreqSpinBoxChanged);

    connect(ui->length_doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), m_manager, &QtCoaxCableLossCalcManager::setGlobalLength);

    onStartFreqChanged(ui->startFreq_doubleSpinBox->value());
    onEndFreqChanged(ui->endFreq_doubleSpinBox->value());
    onSliderValueChanged(ui->startFreq_doubleSpinBox->value() * 1000); // Set initial value
    m_manager->setGlobalLength(ui->length_doubleSpinBox->value());
    m_manager->setPlotRange(ui->startFreq_doubleSpinBox->value(), ui->endFreq_doubleSpinBox->value());
}

void MainWindow::onSliderValueChanged(int value)
{
    double frequencyMHz = static_cast<double>(value) / 1000.0;
    m_manager->setFrequency(frequencyMHz);

    // Update the spinbox, blocking signals to prevent a loop
    ui->currentFreq_doubleSpinBox->blockSignals(true);
    ui->currentFreq_doubleSpinBox->setValue(frequencyMHz);
    ui->currentFreq_doubleSpinBox->blockSignals(false);
}

void MainWindow::onCurrentFreqSpinBoxChanged(double value)
{
    // Update the manager with the new frequency
    m_manager->setFrequency(value);

    // Update the slider, blocking signals to prevent a loop
    int sliderValue = static_cast<int>(value * 1000);
    ui->freq_horizontalSlider->blockSignals(true);
    ui->freq_horizontalSlider->setValue(sliderValue);
    ui->freq_horizontalSlider->blockSignals(false);
}

void MainWindow::onStartFreqChanged(double value)
{
    ui->endFreq_doubleSpinBox->setMinimum(value);
    ui->currentFreq_doubleSpinBox->setMinimum(value);
    ui->freq_horizontalSlider->setMinimum(static_cast<int>(value * 1000));
    m_manager->setPlotRange(value, ui->endFreq_doubleSpinBox->value());
}

void MainWindow::onEndFreqChanged(double value)
{
    ui->startFreq_doubleSpinBox->setMaximum(value);
    ui->currentFreq_doubleSpinBox->setMaximum(value);
    ui->freq_horizontalSlider->setMaximum(static_cast<int>(value * 1000));
    m_manager->setPlotRange(ui->startFreq_doubleSpinBox->value(), value);
}
