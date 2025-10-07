#include "cablewidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QTableWidget>
#include <QHeaderView>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QJsonObject>
#include <QDebug>

CableWidget::CableWidget(CableModel *model, QWidget *parent)
    : QFrame(parent), m_model(model)
{
    Q_ASSERT(m_model != nullptr);
    setupUi();
    populateTable();

    m_nameLineEdit->setText(m_model->getName());
    setLength(1.0);
}

CableWidget::~CableWidget()
{
    qDebug()<<"CableWidget for"<<m_model->getName()<<"deleted.";
}

void CableWidget::setupUi()
{
    this->setStyleSheet("QFrame { border: 1px solid #adadad; background-color: #f0f0f0; border-radius: 4px; }");
    this->setFrameShape(QFrame::StyledPanel);
    this->setFrameShadow(QFrame::Raised);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed); // Allow horizontal stretch, but fixed vertical

    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(5);
    m_mainLayout->setContentsMargins(9, 9, 9, 9);

    QHBoxLayout *topLayout = new QHBoxLayout();
    m_deleteCheckBox = new QCheckBox(this);
    m_nameLineEdit = new QLineEdit(this);
    m_nameLineEdit->setReadOnly(true);
    m_nameLineEdit->setStyleSheet("font-weight: bold;");

    topLayout->addWidget(m_deleteCheckBox);
    topLayout->addWidget(m_nameLineEdit);
    m_mainLayout->addLayout(topLayout);

    m_tableWidget = new QTableWidget(this);
    m_tableWidget->setColumnCount(2);
    m_tableWidget->verticalHeader()->setVisible(false);
    m_tableWidget->horizontalHeader()->setVisible(false);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // FIX: Disable scrollbars
    m_tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_mainLayout->addWidget(m_tableWidget);
}

void CableWidget::populateTable()
{
    int row = 0;
    m_tableWidget->setRowCount(m_model->getAdditionalInfo().count() + 3); // +3 for Length, Loss, and Source

    // -- Calculated Loss Row --
    m_tableWidget->setItem(row, 0, new QTableWidgetItem("Calculated Loss:"));
    m_lossValueItem = new QTableWidgetItem("0.00 dB");
    m_tableWidget->setItem(row++, 1, m_lossValueItem);

    // -- Length Row --
    m_tableWidget->setItem(row, 0, new QTableWidgetItem("Length:"));
    m_lengthSpinBox = new QDoubleSpinBox(m_tableWidget);
    m_lengthSpinBox->setFrame(false);
    m_lengthSpinBox->setRange(0.01, 10000.0);
    m_lengthSpinBox->setDecimals(2);
    m_lengthSpinBox->setSingleStep(0.1);
    m_lengthSpinBox->setSuffix(" m");
    connect(m_lengthSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CableWidget::setLength);
    m_tableWidget->setCellWidget(row++, 1, m_lengthSpinBox);

    // -- Dynamic Additional Info Rows --
    QJsonObject additionalInfo = m_model->getAdditionalInfo();
    for (auto it = additionalInfo.constBegin(); it != additionalInfo.constEnd(); ++it)
        {
            QString labelText = it.key();
            labelText.replace('_', ' ');
            labelText = labelText.replace(0, 1, labelText[0].toUpper()) + ":";
            m_tableWidget->setItem(row, 0, new QTableWidgetItem(labelText));
            m_tableWidget->setItem(row++, 1, new QTableWidgetItem(it.value().toVariant().toString()));
        }

    // -- Data Source Row --
    m_tableWidget->setItem(row, 0, new QTableWidgetItem("Data Source:"));
    QLabel *dataSourceLabel = new QLabel(QString("<a href=\"%1\">Link</a>").arg(m_model->getDataSource()), this);
    dataSourceLabel->setOpenExternalLinks(true);
    m_tableWidget->setCellWidget(row++, 1, dataSourceLabel);

    m_tableWidget->resizeRowsToContents();

    // FIX: Calculate and set the table's height to prevent scrolling
    int totalHeight = 0;
    for (int i = 0; i < m_tableWidget->rowCount(); ++i)
        {
            totalHeight += m_tableWidget->rowHeight(i);
        }
    totalHeight += 2 * m_tableWidget->frameWidth(); // Account for border
    m_tableWidget->setMinimumHeight(totalHeight);
    m_tableWidget->setMaximumHeight(totalHeight);
}

void CableWidget::setLengthEditable(bool editable)
{
    m_lengthSpinBox->setEnabled(editable);
}

CableModel *CableWidget::getModel() const { return m_model; }
double CableWidget::getCurrentAttenuation() const { return m_currentAttenuation; }
bool CableWidget::isMarkedForDeletion() const { return m_deleteCheckBox->isChecked(); }

void CableWidget::setFrequency(double frequencyMHz)
{
    m_currentFrequency = frequencyMHz;
    updateCalculatedValues();
}

void CableWidget::setLength(double lengthM)
{
    m_currentLength = lengthM;
    if (m_lengthSpinBox->value() != lengthM)
        {
            m_lengthSpinBox->blockSignals(true);
            m_lengthSpinBox->setValue(lengthM);
            m_lengthSpinBox->blockSignals(false);
        }
    updateCalculatedValues();
}

void CableWidget::updateCalculatedValues()
{
    double lossPer100m = m_model->getAttenuationPer100m(m_currentFrequency);
    m_currentAttenuation = lossPer100m * (m_currentLength / 100.0);

    if (m_lossValueItem)
        {
            m_lossValueItem->setText(QString::number(m_currentAttenuation, 'f', 2) + " dB");

            double maxDataFreq = m_model->getMaxFrequency();
            if (maxDataFreq > 0 && m_currentFrequency > maxDataFreq)
                {
                    m_lossValueItem->setBackground(QColor("darkorange"));
                    m_lossValueItem->setForeground(Qt::white);
                    m_lossValueItem->setToolTip(QString("Warning: This value is extrapolated beyond the cable's max data frequency of %1 MHz.").arg(maxDataFreq));
                }
            else
                {
                    m_lossValueItem->setBackground(Qt::white);
                    m_lossValueItem->setForeground(Qt::black);
                    m_lossValueItem->setToolTip("");
                }
        }

    emit attenuationChanged(m_currentAttenuation);
}
