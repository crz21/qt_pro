#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QBluetoothAddress>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 扫描相关槽函数
    void on_scanButton_clicked();
    void deviceDiscovered(const QBluetoothDeviceInfo &info);
    void scanFinished();
    void scanError(QBluetoothDeviceDiscoveryAgent::Error error);

    // 连接相关槽函数
    void on_deviceList_itemDoubleClicked(QListWidgetItem *item);
    void on_disconnectButton_clicked();
    void deviceConnected();
    void deviceDisconnected();
    void connectionError(QLowEnergyController::Error error);

    // 服务相关槽函数
    void serviceDiscovered(const QBluetoothUuid &uuid);
    void serviceScanDone();
    void serviceStateChanged(QLowEnergyService::ServiceState newState);

    // 特征值相关槽函数
    void updateCharacteristicValue(const QLowEnergyCharacteristic &c, const QByteArray &value);
    void characteristicWriteFinished(const QLowEnergyCharacteristic &c, const QByteArray &value);

    // 读写操作槽函数
    void on_writeButton_clicked();
    void on_clearLogButton_clicked();

    void on_serviceList_itemClicked(QListWidgetItem *item);
    void on_charList_itemClicked(QListWidgetItem *item);
private:
    Ui::MainWindow *ui;
    bool read_status = false;

    // 保存订阅相关的关键对象（关闭时需要）
    QLowEnergyCharacteristic m_notifyChar; // 当前启用通知的特征值
    QLowEnergyService* m_notifyService = nullptr; // 所属服务
    QLowEnergyDescriptor m_notifyDesc; // 客户端特征配置描述符
    // BLE相关对象
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QLowEnergyController *controller;
    QMap<QBluetoothUuid, QLowEnergyService*> services;

    // 当前选中的特征值
    QLowEnergyCharacteristic currentChar;

    // 日志输出函数
    void logMessage(const QString &message);
    void writeDataToTxt(const QString &data, const QString &fileName);
};
#endif // MAINWINDOW_H
