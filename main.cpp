#include <QtCore/QDebug>
#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

QList<QSerialPortInfo> filterPorts(QStringList mask) {
    qDebug() << "searching for ports with mask" << mask;
    QList<QSerialPortInfo> result;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        foreach (const QString& maskItem, mask) {
            //qDebug() << "looking for:" << maskItem;
            if (info.portName().contains(QRegExp(maskItem))) {
                qDebug() << "found " << maskItem;
                result.append(info);
                break;
            }
        }
    }
    return result;
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/arduinoremote1/main.qml"));
    viewer.showExpanded();

    // Example use QSerialPortInfo
    QStringList mask;
    mask << "ttyUSB" << "ACM";

    QList<QSerialPortInfo> filteredPorts(filterPorts(mask));

    foreach (const QSerialPortInfo &info, filteredPorts) {
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
        qDebug() << "ProcducerId : " << info.hasProductIdentifier() << "  " << info.productIdentifier();
        qDebug() << "BaudRates   : " << info.standardBaudRates();
        qDebug() << "sysLocation : " << info.systemLocation();
        qDebug() << "isValid     : " << info.isValid();
        qDebug() << "isNull      : " << info.isNull();
        qDebug() << "isBusy      : " << info.isBusy();

        // Example use QSerialPort
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite)) {
            serial.close();
            qDebug() << "opened and closed successful";
        }
        else {
            qDebug() << "opened failed";
        }
    }
    return app.exec();
}
