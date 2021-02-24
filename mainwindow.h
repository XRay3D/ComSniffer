#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_pbStartStop_clicked(bool checked);

private:
    Ui::MainWindow* ui;

    QSerialPort port1;

    QSerialPort port2;

    int timerId {};

    // QObject interface
protected:
    void timerEvent(QTimerEvent* event) override;
};

Q_DECLARE_METATYPE(QSerialPortInfo)

#endif // MAINWINDOW_H
