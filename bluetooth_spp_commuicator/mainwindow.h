#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothAddress>
#include <QListWidgetItem>
#include <QBluetoothSocket>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothServiceInfo> // 配套的服务信息类，建议一并添加

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void connectToDevice(const QString &macAddress); // 连接指定MAC地址的设备
    void sendData(const QByteArray &data);          // 发送数据

private slots:
    // 扫描相关槽函数
    void on_scanButton_clicked();
    void deviceDiscovered(const QBluetoothDeviceInfo &info);
    // void deviceConnected();
    void scanFinished();
    void scanError(QBluetoothDeviceDiscoveryAgent::Error error);

    // 连接相关槽函数
    void on_deviceList_itemDoubleClicked(QListWidgetItem *item);
    void on_disconnectButton_clicked();
    void socketConnected();
    void socketDisconnected();
    void socketError(QBluetoothSocket::SocketError error); // 连接错误

    // // 服务相关槽函数
    // void serviceDiscovered(const QBluetoothUuid &uuid);
    // void serviceScanDone();
    // void serviceStateChanged(QLowEnergyService::ServiceState newState);

    // // 特征值相关槽函数
    // void updateCharacteristicValue(const QLowEnergyCharacteristic &c, const QByteArray &value);
    // void characteristicWriteFinished(const QLowEnergyCharacteristic &c, const QByteArray &value);

    // 读写操作槽函数
    void on_writeButton_clicked();
    void on_clearLogButton_clicked();
    void readSocketData();
    // void on_serviceList_itemClicked(QListWidgetItem *item);
    // void on_charList_itemClicked(QListWidgetItem *item);


    // // 扫描SPP设备（经典蓝牙）
    // void on_scanSPPButton_clicked();
    // // 连接SPP设备
    // void on_sppDeviceList_itemDoubleClicked(QListWidgetItem *item);
    // // 断开SPP连接
    // void on_disconnectSPPButton_clicked();
    // // SPP数据发送
    // void on_sendSPPDataButton_clicked();
    // // SPP数据接收
    // void readSPPData();
    // // SPP连接状态变化
    // void sppSocketStateChanged(QBluetoothSocket::SocketState state);
    // // SPP错误处理
    // void sppSocketError(QBluetoothSocket::SocketError error);
private:
    Ui::MainWindow *ui;

    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QBluetoothSocket *sppSocket;  // SPP蓝牙套接字
    // bool isSocketConnected = false;                 // 连接状态标记

    // 日志输出函数
    void logMessage(const QString &message);
    void writeDataToTxt(const QString &data, const QString &fileName);
};
#endif // MAINWINDOW_H
