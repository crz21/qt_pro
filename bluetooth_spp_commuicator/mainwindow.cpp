#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDebug>
#include <QFile>      // 文件操作
#include <QTextStream> // 文本流写入
#include <QDir>       // 目录操作
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Qt6上位机");

    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &MainWindow::deviceDiscovered);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &MainWindow::scanFinished);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::errorOccurred,
            this, &MainWindow::scanError);

    sppSocket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(sppSocket, &QBluetoothSocket::connected, this, &MainWindow::socketConnected);
    connect(sppSocket, &QBluetoothSocket::disconnected, this, &MainWindow::socketDisconnected);
    connect(sppSocket, &QBluetoothSocket::errorOccurred, this, &MainWindow::socketError);
    connect(sppSocket, &QBluetoothSocket::readyRead, this, &MainWindow::readSocketData);
    // 初始化界面状态
    ui->disconnectButton->setEnabled(false);
    ui->writeButton->setEnabled(false);

    logMessage("应用程序启动完成");
}

MainWindow::~MainWindow()
{
    if (sppSocket->isOpen()) {
        sppSocket->close();
    }
    delete sppSocket;
    delete discoveryAgent;
    delete ui;
}

//扫描按钮点击事件
void MainWindow::on_scanButton_clicked()
{
    ui->deviceList->clear();
    logMessage("开始扫描蓝牙设备...");

    // 停止任何正在进行的扫描
    if (discoveryAgent->isActive()) {
        discoveryAgent->stop();
    }

    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::ClassicMethod);

    ui->scanButton->setEnabled(false);
}

// 发现设备时的处理
void MainWindow::deviceDiscovered(const QBluetoothDeviceInfo &info)
{
    // QString deviceName = info.name();
    // if (deviceName.isEmpty()) return;

    // // QString prefix = deviceName.left(3);
    // // if((prefix == "BCI") || (prefix == "BLE") || (prefix == "Zep") || (prefix == "CRS"))
    // // {
    // if (info.coreConfigurations() & QBluetoothDeviceInfo::BaseRateAndEnhancedDataRateCoreConfiguration) {
    //     QString deviceInfo = QString("%1 (%2)").arg(info.name()).arg(info.address().toString());
    //     int rssi = info.rssi();

    //     if (rssi != 0) {
    //         deviceInfo += QString(" RSSI: %1 dBm").arg(rssi);
    //     }
    //     QListWidgetItem *item = new QListWidgetItem(deviceInfo);
    //     item->setData(Qt::UserRole, QVariant::fromValue(info));
    //     ui->deviceList->addItem(item);

    //     logMessage(QString("发现SPP设备: %1").arg(deviceInfo));
    // }

    // QString deviceName = info.name();
    // if (deviceName.isEmpty()) return;

    // // 1. 获取设备的蓝牙核心配置
    // QBluetoothDeviceInfo::CoreConfigurations configs = info.coreConfigurations();

    // // 2. 精准匹配：只要设备支持经典蓝牙速率（BaseRate），就说明它可能是 SPP 设备
    // bool isClassicBluetooth = (configs & QBluetoothDeviceInfo::BaseRateCoreConfiguration) ||
    //                           (configs & QBluetoothDeviceInfo::BaseRateAndLowEnergyCoreConfiguration);

    // if (isClassicBluetooth) {
    //     QString deviceInfo = QString("%1 (%2)").arg(info.name()).arg(info.address().toString());
    //     int rssi = info.rssi();

    //     if (rssi != 0) {
    //         deviceInfo += QString(" RSSI: %1 dBm").arg(rssi);
    //     }

    //     // 避免列表重复添加
    //     if (ui->deviceList->findItems(deviceInfo, Qt::MatchExactly).isEmpty()) {
    //         QListWidgetItem *item = new QListWidgetItem(deviceInfo);
    //         item->setData(Qt::UserRole, QVariant::fromValue(info));
    //         ui->deviceList->addItem(item);

    //         logMessage(QString("发现SPP设备: %1").arg(deviceInfo));
    //     }
    // }

    QString deviceName = info.name();
    if (deviceName.isEmpty()) return; // 仅过滤掉无名设备

    QString deviceInfo = QString("%1 (%2)").arg(deviceName).arg(info.address().toString());

    if (ui->deviceList->findItems(deviceInfo, Qt::MatchExactly).isEmpty()) {
        QListWidgetItem *item = new QListWidgetItem(deviceInfo);
        item->setData(Qt::UserRole, QVariant::fromValue(info));
        ui->deviceList->addItem(item);
        logMessage(QString("发现蓝牙设备: %1").arg(deviceInfo));
    }

}

// 扫描完成
void MainWindow::scanFinished()
{
    logMessage(QString("扫描完成，共发现 %1 个BLE设备").arg(ui->deviceList->count()));
    ui->scanButton->setEnabled(true);
}

// 扫描错误处理
void MainWindow::scanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    QString errorStr;
    switch (error) {
    case QBluetoothDeviceDiscoveryAgent::PoweredOffError:
        errorStr = "蓝牙未开启";
        break;
    case QBluetoothDeviceDiscoveryAgent::InputOutputError:
        errorStr = "输入输出错误";
        break;
    default:
        errorStr = "未知错误";
    }

    logMessage(QString("扫描错误: %1").arg(errorStr));
    ui->scanButton->setEnabled(true);
}

// 双击设备进行连接（修复 conn/errConn 未初始化警告 + ConnectionRefusedError 枚举）
void MainWindow::on_deviceList_itemDoubleClicked(QListWidgetItem *item)
{
    // if (sppSocket->isOpen()) {
    //     sppSocket->close();
    // }

    // // 获取设备信息
    // QBluetoothDeviceInfo info = item->data(Qt::UserRole).value<QBluetoothDeviceInfo>();
    // logMessage(QString("开始扫描设备 %1 的所有服务...").arg(info.name()));

    // // 1. 创建服务发现代理（指定要扫描的设备地址）
    // QBluetoothServiceDiscoveryAgent *serviceAgent = new QBluetoothServiceDiscoveryAgent(info.address(), this);
    // // 服务计数器（统计扫描到的服务数）
    // int serviceCount = 0;

    // QTimer::singleShot(10000, serviceAgent, &QBluetoothServiceDiscoveryAgent::stop);
    // // 2. 每发现一个服务的处理逻辑（加mutable允许修改捕获的serviceCount）
    // connect(serviceAgent, &QBluetoothServiceDiscoveryAgent::serviceDiscovered, this, [=](const QBluetoothServiceInfo &service) mutable {
    //     serviceCount++;
    //     // 打印服务详情（调试用）
    //     QString serviceName = service.serviceName().isEmpty() ? "未知服务" : service.serviceName();
    //     QString serviceUuid = service.serviceUuid().toString();
    //     logMessage(QString("发现服务：%1 | UUID：%2").arg(serviceName).arg(serviceUuid));

    //     // 修复点1：Qt6中SerialPort的正确枚举值 + 修复protocol()为socketProtocol()
    //     bool isSPPService =
    //         // Qt6正确的SerialPort UUID枚举值（ServiceClassUuid::SerialPort）
    //         (service.serviceUuid() == QBluetoothUuid(QBluetoothUuid::ServiceClassUuid::SerialPort))
    //         // Qt6中获取RFCOMM协议（替换原protocol()为socketProtocol()）
    //         || (service.socketProtocol() == QBluetoothServiceInfo::RfcommProtocol);

    //     if (isSPPService) {
    //         logMessage(QString("匹配到SPP服务，开始连接 %1...").arg(info.name()));
    //         // 连接匹配到的SPP服务
    //         sppSocket->connectToService(info.address(), service.serviceUuid(), QIODevice::ReadWrite);
    //         serviceAgent->stop(); // 找到服务后停止扫描
    //         // serviceAgent->deleteLater(); // 释放资源
    //     }
    // });

    // // 3. 服务扫描完成（未找到SPP服务）
    // connect(serviceAgent, &QBluetoothServiceDiscoveryAgent::finished, this, [=]() {
    //     if (!sppSocket->isOpen()) {
    //         // logMessage(QString("服务扫描完成！设备 %1 未找到SPP服务，共扫描到 %2 个服务").arg(info.name()).arg(serviceCount));
    //         // logMessage("尝试直接连接RFCOMM通道1（兜底方案）...");

    //         // // Qt6异步连接RFCOMM通道1（无waitForConnected）
    //         // QBluetoothAddress addr = info.address();
    //         // sppSocket->connectToService(addr, 1);

    //         // // 修复点1：解决变量未初始化问题（动态分配Connection，避免自身捕获）
    //         // // 监听连接成功
    //         // QMetaObject::Connection* connPtr = new QMetaObject::Connection;
    //         // *connPtr = connect(sppSocket, &QBluetoothSocket::connected, this, [=]() {
    //         //     logMessage("RFCOMM通道1连接成功！");
    //         //     // 延迟解绑并释放指针
    //         //     QTimer::singleShot(0, this, [connPtr]() {
    //         //         disconnect(*connPtr);
    //         //         delete connPtr;
    //         //     });
    //         // }, Qt::UniqueConnection);

    //         // // 监听连接失败（修复枚举值错误：移除ConnectionRefusedError，改用Qt6兼容逻辑）
    //         // QMetaObject::Connection* errConnPtr = new QMetaObject::Connection;
    //         // *errConnPtr = connect(sppSocket, &QBluetoothSocket::errorOccurred, this, [=](QBluetoothSocket::SocketError error) {
    //         //     // 修复点2：Qt6中无ConnectionRefusedError，替换为OperationError + 错误字符串兜底
    //         //     QString errorMsg;
    //         //     if (error == QBluetoothSocket::SocketError::OperationError) { // Qt6替代ConnectionRefusedError的枚举
    //         //         errorMsg = "连接被拒绝/操作失败";
    //         //     } else if (error == QBluetoothSocket::SocketError::ServiceNotFoundError) {
    //         //         errorMsg = "服务未找到";
    //         //     } else {
    //         //         errorMsg = sppSocket->errorString(); // 兜底使用系统错误字符串
    //         //     }
    //         //     logMessage("RFCOMM通道1连接失败：" + errorMsg);

    //         //     // 延迟解绑并释放指针
    //         //     QTimer::singleShot(0, this, [errConnPtr]() {
    //         //         disconnect(*errConnPtr);
    //         //         delete errConnPtr;
    //         //     });
    //         // }, Qt::UniqueConnection);
    //         logMessage("未找到SPP服务，尝试标准串口UUID直接连接");
    //         sppSocket->connectToService(info.address(), QBluetoothUuid(QBluetoothUuid::ServiceClassUuid::SerialPort), QIODevice::ReadWrite);
    //     }
    //     serviceAgent->deleteLater();
    // });

    // // 扫描错误捕获
    // connect(serviceAgent, &QBluetoothServiceDiscoveryAgent::errorOccurred, this, [=](QBluetoothServiceDiscoveryAgent::Error error) {
    //     logMessage(QString("服务扫描错误：%1").arg(serviceAgent->errorString()));
    //     serviceAgent->deleteLater();
    // });

    // logMessage("启动FullDiscovery服务扫描");
    // serviceAgent->start(QBluetoothServiceDiscoveryAgent::FullDiscovery);

    if (sppSocket->state() != QBluetoothSocket::SocketState::UnconnectedState) {
        sppSocket->disconnectFromService();
    }

    // 1. 获取选中的设备信息
    QBluetoothDeviceInfo info = item->data(Qt::UserRole).value<QBluetoothDeviceInfo>();
    logMessage(QString("正在尝试直接连接设备: %1 (%2)...").arg(info.name()).arg(info.address().toString()));

    // 2. 使用经典蓝牙 SPP 标准串口 UUID 强行连接
    // 自定义的 SPP UUID 值为 "00007033-0000-1000-8000-00805F9B34FB"
    QBluetoothUuid sppUuid(QBluetoothUuid::ServiceClassUuid::BarcoSppService);

    // 3. 异步发起连接
    sppSocket->connectToService(info.address(), sppUuid, QIODevice::ReadWrite);
}

// 连接错误（修复 ConnectionRefusedError 枚举值）
void MainWindow::socketError(QBluetoothSocket::SocketError error)
{
    QString errorStr;
    switch (error) {
    case QBluetoothSocket::SocketError::OperationError: // 对应原ConnectionRefusedError场景
        errorStr = "连接被拒绝/操作失败";
        break;
    case QBluetoothSocket::SocketError::RemoteHostClosedError:
        errorStr = "远程设备关闭连接";
        break;
    case QBluetoothSocket::SocketError::HostNotFoundError:
        errorStr = "未找到设备";
        break;
    case QBluetoothSocket::SocketError::NoSocketError:
        errorStr = "无错误";
        break;
    case QBluetoothSocket::SocketError::UnknownSocketError:
        errorStr = "未知错误";
        break;
    case QBluetoothSocket::SocketError::ServiceNotFoundError:
        errorStr = "服务未找到";
        break;
    case QBluetoothSocket::SocketError::NetworkError:
        errorStr = "网络错误";
        break;
    case QBluetoothSocket::SocketError::UnsupportedProtocolError:
        errorStr = "不支持的协议";
        break;
    case QBluetoothSocket::SocketError::MissingPermissionsError:
        errorStr = "权限不足";
        break;
    default:
        errorStr = "未知错误: " + QString::number(static_cast<int>(error));
    }

    logMessage(QString("连接错误: %1").arg(errorStr));
    ui->disconnectButton->setEnabled(false);
    ui->scanButton->setEnabled(true);
}

// 断开连接按钮
void MainWindow::on_disconnectButton_clicked()
{
    if (sppSocket->state() != QBluetoothSocket::SocketState::UnconnectedState) {
        sppSocket->disconnectFromService();
    }
}

// 设备连接成功
void MainWindow::socketConnected()
{
    logMessage("设备连接成功！串口通道已建立，可以开始收发数据。");
    ui->disconnectButton->setEnabled(true);
    ui->scanButton->setEnabled(false);
    ui->writeButton->setEnabled(true);
}

// 设备断开连接
void MainWindow::socketDisconnected()
{
    logMessage("设备已断开连接");
    ui->disconnectButton->setEnabled(false);
    ui->scanButton->setEnabled(true);
    ui->writeButton->setEnabled(false);
}

// 读取SPP串口数据
void MainWindow::readSocketData()
{
    if (sppSocket->state() != QBluetoothSocket::SocketState::ConnectedState) return;

    QByteArray data = sppSocket->readAll();
    if (data.isEmpty()) return;

    // 十六进制显示
    QString hexStr = data.toHex(' ').toUpper();
    QString logStr = QString("接收数据（十六进制）: %1 | （字符串）: %2").arg(hexStr).arg(QString(data));
    logMessage(logStr);

    // 写入TXT文件
    writeDataToTxt(logStr, "spp_data.txt");

    // 更新UI显示
    ui->valueEdit->setText(QString(data));
    ui->hexValueEdit->setText(hexStr);
}

// 写入按钮点击事件
void MainWindow::on_writeButton_clicked()
{
    if (sppSocket->state() != QBluetoothSocket::SocketState::ConnectedState) {
        logMessage("数据发送失败：蓝牙未处于连接状态");
        return;
    }

    // 获取要写入的值
    QByteArray value;
    if (ui->hexRadio->isChecked()) {
        // 十六进制模式
        value = QByteArray::fromHex(ui->writeValueEdit->text().toUtf8());
    } else {
        // 字符串模式
        value = ui->writeValueEdit->text().toUtf8();
    }

    qint64 bytesWritten = sppSocket->write(value);
    if (bytesWritten > 0) {
        logMessage(QString("发送数据（十六进制）: %1 | （字符串）: %2")
                       .arg(value.toHex(' ').toUpper())
                       .arg(QString(value)));
    } else {
        logMessage("数据发送失败");
    }
}

// 清空日志按钮
void MainWindow::on_clearLogButton_clicked()
{
    ui->logTextEdit->clear();
}

// 显示日志消息
void MainWindow::logMessage(const QString &message)
{
    QString timeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    ui->logTextEdit->append(QString("[%1] %2").arg(timeStr).arg(message));

    // 滚动到底部
    QTextCursor cursor = ui->logTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->logTextEdit->setTextCursor(cursor);
}

// 写入数据到TXT文件
void MainWindow::writeDataToTxt(const QString &data, const QString &fileName)
{
    // 确保程序运行目录存在（可选，也可指定绝对路径）
    QDir dir;
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    // 拼接文件路径（当前目录下的fileName，也可改为绝对路径如："D:/ble_data.txt"）
    QString filePath = QString("%1").arg(fileName);

    // 以追加模式打开文件（QIODevice::Append），不存在则创建
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        logMessage(QString("文件打开失败：%1").arg(file.errorString()));
        return;
    }

    // 写入数据（带时间戳）
    QTextStream out(&file);
    QString timeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    out << QString("[%1] %2\n").arg(timeStr).arg(data);

    // 刷新并关闭文件
    out.flush();
    file.close();
}

// 外部调用的连接函数（可选保留）
void MainWindow::connectToDevice(const QString &macAddress)
{
    if (sppSocket->isOpen()) {
        sppSocket->close();
    }
    sppSocket->connectToService(QBluetoothAddress(macAddress), QBluetoothUuid(QBluetoothUuid::ServiceClassUuid::SerialPort), QIODevice::ReadWrite);
}

// 外部调用的发送函数（可选保留）
void MainWindow::sendData(const QByteArray &data)
{
    if (sppSocket->isOpen()) {
        sppSocket->write(data);
    }
}
