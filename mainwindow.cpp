#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QClipboard>
#include <QSettings>
#include <ranges>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto ports { QSerialPortInfo::availablePorts().toVector() };

    std::ranges::sort(ports, std::less(), [](const QSerialPortInfo& pi) { return pi.portName().midRef(3).toInt(); });

    for (auto& pi : ports) {
        ui->cbxPort1->addItem(pi.portName());
        ui->cbxPort2->addItem(pi.portName());
    }

    for (auto& baud : ports.front().standardBaudRates())
        ui->cbxBaud->addItem(QString::number(baud), baud);

    QSettings settings;
    restoreGeometry(settings.value(QStringLiteral("Geometry")).toByteArray());
    restoreState(settings.value(QStringLiteral("State")).toByteArray());
    ui->cbxPort1->setCurrentText(settings.value(QStringLiteral("Port1")).toString());
    ui->cbxPort2->setCurrentText(settings.value(QStringLiteral("Port2")).toString());
    ui->cbxBaud->setCurrentText(settings.value(QStringLiteral("Baud")).toString());
}

MainWindow::~MainWindow()
{
    QSettings settings;
    settings.setValue(QStringLiteral("Geometry"), saveGeometry());
    settings.setValue(QStringLiteral("State"), saveState());
    settings.setValue(QStringLiteral("Port1"), ui->cbxPort1->currentText());
    settings.setValue(QStringLiteral("Port2"), ui->cbxPort2->currentText());
    settings.setValue(QStringLiteral("Baud"), ui->cbxBaud->currentText());
    delete ui;
}

void MainWindow::on_pbStartStop_clicked(bool checked)
{
    if (timerId) {
        killTimer(timerId);
        timerId = 0;
    }

    port1.setBaudRate(ui->cbxBaud->currentData().toInt());
    port2.setBaudRate(ui->cbxBaud->currentData().toInt());
    port1.setPortName(ui->cbxPort1->currentText());
    port2.setPortName(ui->cbxPort2->currentText());
    if (checked && port1.open(QIODevice::ReadOnly) && port2.open(QIODevice::ReadOnly)) {
        timerId = startTimer(10);
        ui->cbxBaud->setEnabled(false);
        ui->cbxPort1->setEnabled(false);
        ui->cbxPort2->setEnabled(false);
        ui->pbStartStop->setText(QStringLiteral("close"));
    } else {
        ui->textEdit->setTextColor(Qt::darkRed);
        ui->textEdit->append(QStringLiteral("%1\t%2:\t%3").arg(QStringLiteral("P1"), //
            QTime::currentTime().toString(QStringLiteral("hh:mm:ss:zzz")), port1.errorString()));
        ui->textEdit->append(QStringLiteral("%1\t%2:\t%3").arg(QStringLiteral("P2"), //
            QTime::currentTime().toString(QStringLiteral("hh:mm:ss:zzz")), port2.errorString()));
        port1.close();
        port2.close();
        ui->cbxBaud->setEnabled(true);
        ui->cbxPort1->setEnabled(true);
        ui->cbxPort2->setEnabled(true);
        ui->pbStartStop->setText(QStringLiteral("open"));
        ui->pbStartStop->setChecked(false);
    }
}

void MainWindow::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == timerId) {
        if (port1.canReadLine()) {
            ui->textEdit->setTextColor(Qt::darkBlue);
            ui->textEdit->append(QStringLiteral("%1\t%2:\t%3").arg(QStringLiteral("P1"), //
                QTime::currentTime().toString(QStringLiteral("hh:mm:ss:zzz")), QString::fromLatin1(port1.readAll())));
        }
        if (port2.canReadLine()) {
            ui->textEdit->setTextColor(Qt::darkGreen);
            ui->textEdit->append(QStringLiteral("%1\t%2:\t%3").arg(QStringLiteral("P2"), //
                QTime::currentTime().toString(QStringLiteral("hh:mm:ss:zzz")), QString::fromLatin1(port2.readAll())));
        }
    }
}

void MainWindow::on_pbClear_clicked()
{
    ui->textEdit->clear();
}

void MainWindow::on_pbCopy_clicked()
{
    QClipboard* clipboard = QGuiApplication::clipboard();
    clipboard->setText(ui->textEdit->toPlainText());
}
