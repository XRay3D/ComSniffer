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

private Q_SLOTS:
    void on_pbStartStop_clicked(bool checked);
    void on_pbClear_clicked();
    void on_pbCopy_clicked();

private:
    Ui::MainWindow* ui;
    QSerialPort port1;
    QSerialPort port2;
    int timerId {};

    static const inline auto msg { QStringLiteral("%1\t%2:\t%3") };

    // QObject interface
protected:
    void timerEvent(QTimerEvent* event) override;
};

Q_DECLARE_METATYPE(QSerialPortInfo)

#endif // MAINWINDOW_H
