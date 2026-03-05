#include "mainwindow.h"

#include <QApplication>
#include <QtPlugin>

// 为了在Windows上使用蓝牙功能，需要显式加载插件
Q_IMPORT_PLUGIN(QWindowsBluetoothPlugin)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // 确保应用程序支持中文显示
    QFont font("SimHei", 9);
    a.setFont(font);
    
    MainWindow w;
    w.show();
    return a.exec();
}
