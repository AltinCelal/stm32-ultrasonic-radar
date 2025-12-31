#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // 1️⃣ UI MUTLAKA İLK
    ui->setupUi(this);

    // 2️⃣ RADAR WIDGET OLUŞTUR
    radar = new RadarWidget(this);

    // ⚠️ Qt Designer'da layout adını kontrol et
    // objectName: verticalLayout olmalı
    ui->verticalLayout->addWidget(radar);

    // 3️⃣ SERIAL PORT
    serial = new QSerialPort(this);
    serial->setPortName("COM5");
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (!serial->open(QIODevice::ReadOnly)) {
        ui->labelAngle->setText("PORT AÇILAMADI");
        ui->labelDistance->setText("-");
        return;
    }

    ui->labelAngle->setText("PORT AÇILDI");
    ui->labelDistance->setText("-");

    // 4️⃣ READYREAD BAĞLA
    connect(serial, &QSerialPort::readyRead,
            this, &MainWindow::readSerialData);
}

MainWindow::~MainWindow()
{
    if (serial && serial->isOpen())
        serial->close();

    delete ui;
}

void MainWindow::readSerialData()
{
    buffer += serial->readAll();

    while (buffer.contains('\n')) {

        QByteArray line = buffer.left(buffer.indexOf('\n'));
        buffer.remove(0, buffer.indexOf('\n') + 1);

        line = line.trimmed();
        qDebug() << "LINE:" << line;

        QList<QByteArray> parts = line.split(',');
        if (parts.size() != 2)
            continue;

        bool ok1, ok2;
        int angle = parts[0].toInt(&ok1);
        int distance = parts[1].toInt(&ok2);

        if (!ok1 || !ok2)
            continue;

        // 5️⃣ LABEL GÜNCELLE
        ui->labelAngle->setText("Angle: " + QString::number(angle));
        ui->labelDistance->setText("Distance: " + QString::number(distance));

        // 6️⃣ RADAR GÜNCELLE
        radar->updateData(angle, distance);
    }
}
