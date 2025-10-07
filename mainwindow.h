#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QtCoaxCableLossCalcManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSliderValueChanged(int value);
    void onStartFreqChanged(double value);
    void onEndFreqChanged(double value);
    // Slot for the new spinbox
    void onCurrentFreqSpinBoxChanged(double value);

private:
    void setupFrequencyControls();

    Ui::MainWindow *ui;
    QtCoaxCableLossCalcManager *m_manager;
};
#endif // MAINWINDOW_H
