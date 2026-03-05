#include "mainwindow.h"

#include <QApplication>
#include <QBluetoothLocalDevice>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 检查蓝牙是否可用
    QBluetoothLocalDevice localDevice;
    if (!localDevice.isValid()) {
        qWarning() << "本设备不支持蓝牙功能";
        return 1;
    }

    // 如果蓝牙未开启，则尝试开启
    if (localDevice.hostMode() == QBluetoothLocalDevice::HostPoweredOff) {
        localDevice.powerOn();
    }

    MainWindow w;
    w.show();

    return a.exec();
}
