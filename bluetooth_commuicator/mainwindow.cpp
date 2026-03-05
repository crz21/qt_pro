#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDebug>
#include <QFile>      // 文件操作
#include <QTextStream> // 文本流写入
#include <QDir>       // 目录操作

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Qt6 BLE上位机");

    // 初始化扫描代理
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    // 连接扫描相关信号和槽
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &MainWindow::deviceDiscovered);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &MainWindow::scanFinished);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::errorOccurred,
            this, &MainWindow::scanError);

    // 初始化界面状态
    ui->disconnectButton->setEnabled(false);
    ui->writeButton->setEnabled(false);

    logMessage("应用程序启动完成");
}

MainWindow::~MainWindow()
{
    // 断开连接并清理资源
    if (controller) {
        controller->disconnectFromDevice();
        delete controller;
    }

    qDeleteAll(services);
    delete discoveryAgent;
    delete ui;
}

//扫描按钮点击事件
void MainWindow::on_scanButton_clicked()
{
    ui->deviceList->clear();
    logMessage("开始扫描BLE设备...");

    // 停止任何正在进行的扫描
    if (discoveryAgent->isActive()) {
        discoveryAgent->stop();
    }

    // 开始扫描，只扫描BLE设备
    discoveryAgent->setLowEnergyDiscoveryTimeout(5000); // 5秒超时
    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);

    ui->scanButton->setEnabled(false);
}

// 发现设备时的处理
void MainWindow::deviceDiscovered(const QBluetoothDeviceInfo &info)
{
    // 只处理BLE设备
    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
        QString deviceName = QString("%1").arg(info.name()); // 获取蓝牙设备名字
        QString prefix = deviceName.left(3); // 获取指定长度部分的名字
        if((prefix == "BCI") || (prefix == "BLE") || (prefix == "Zep") || (prefix == "CRS"))
        {
            QString deviceInfo = QString("%1 (%2)").arg(info.name()).arg(info.address().toString());
            int rssi = info.rssi();

            if (rssi != 0) {
                deviceInfo += QString(" RSSI: %1 dBm").arg(rssi);
            }

            QListWidgetItem *item = new QListWidgetItem(deviceInfo);
            item->setData(Qt::UserRole, QVariant::fromValue(info));
            ui->deviceList->addItem(item);

            logMessage(QString("发现设备: %1").arg(deviceInfo));
        }
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

// 双击设备进行连接
void MainWindow::on_deviceList_itemDoubleClicked(QListWidgetItem *item)
{
    // 如果已有连接，先断开
    if (controller) {
        controller->disconnectFromDevice();
        delete controller;
        controller = nullptr;
    }

    qDeleteAll(services);
    services.clear();
    ui->serviceList->clear();
    ui->charList->clear();

    // 获取设备信息
    QBluetoothDeviceInfo info = item->data(Qt::UserRole).value<QBluetoothDeviceInfo>();
    logMessage(QString("正在连接设备: %1").arg(info.name()));

    // 创建控制器并连接
    controller = QLowEnergyController::createCentral(info, this);
    connect(controller, &QLowEnergyController::connected,
            this, &MainWindow::deviceConnected);
    connect(controller, &QLowEnergyController::disconnected,
            this, &MainWindow::deviceDisconnected);
    connect(controller, &QLowEnergyController::errorOccurred,
            this, &MainWindow::connectionError);
    connect(controller, &QLowEnergyController::serviceDiscovered,
            this, &MainWindow::serviceDiscovered);
    connect(controller, &QLowEnergyController::discoveryFinished,
            this, &MainWindow::serviceScanDone);

    controller->connectToDevice();
}

// 断开连接按钮
void MainWindow::on_disconnectButton_clicked()
{
    if (controller) {
        controller->disconnectFromDevice();
    }
}

// 设备连接成功
void MainWindow::deviceConnected()
{
    logMessage("设备连接成功，开始发现服务...");
    ui->disconnectButton->setEnabled(true);
    ui->scanButton->setEnabled(false);

    // 开始发现服务
    controller->discoverServices();
}

// 设备断开连接
void MainWindow::deviceDisconnected()
{
    logMessage("设备已断开连接");
    ui->disconnectButton->setEnabled(false);
    ui->scanButton->setEnabled(true);
    ui->serviceList->clear();
    ui->charList->clear();
    ui->writeButton->setEnabled(false);

    qDeleteAll(services);
    services.clear();
}

// 连接错误
void MainWindow::connectionError(QLowEnergyController::Error error)
{
    QString errorStr;
    switch (error) {
    case QLowEnergyController::UnknownError:
        errorStr = "未知错误";
        break;
    case QLowEnergyController::ConnectionError:
        errorStr = "连接错误";
        break;
    case QLowEnergyController::AdvertisingError:
        errorStr = "广播错误";
        break;
    default:
        errorStr = "其他错误";
    }

    logMessage(QString("连接错误: %1").arg(errorStr));
    ui->disconnectButton->setEnabled(false);
    ui->scanButton->setEnabled(true);
}

// 发现服务
void MainWindow::serviceDiscovered(const QBluetoothUuid &uuid)
{
    if (!services.contains(uuid)) {
        QLowEnergyService *service = controller->createServiceObject(uuid, this);
        if (service) {
            services[uuid] = service;
            connect(service, &QLowEnergyService::stateChanged,
                    this, &MainWindow::serviceStateChanged);
            connect(service, &QLowEnergyService::characteristicChanged,
                    this, &MainWindow::updateCharacteristicValue);
            connect(service, &QLowEnergyService::characteristicWritten,
                    this, &MainWindow::characteristicWriteFinished);

            services.insert(uuid, service);
            logMessage(QString("服务 %1 已存入容器，当前容器大小：%2").arg(uuid.toString()).arg(services.size()));


            QListWidgetItem *item = new QListWidgetItem(uuid.toString());

            // 明确存入 QBluetoothUuid 对象，避免隐式转为 QUuid
            item->setData(Qt::UserRole, QVariant::fromValue<QBluetoothUuid>(uuid)); // item->setData(Qt::UserRole, uuid);
            ui->serviceList->addItem(item);

            logMessage(QString("发现服务: %1").arg(uuid.toString()));
        }
    }
}

// 服务扫描完成
void MainWindow::serviceScanDone()
{
    logMessage(QString("服务扫描完成，共发现 %1 个服务").arg(services.count()));
}

// 服务状态变化
void MainWindow::serviceStateChanged(QLowEnergyService::ServiceState newState)
{
    QLowEnergyService *service = qobject_cast<QLowEnergyService*>(sender());
    if (!service) return;

    if (newState == QLowEnergyService::RemoteServiceDiscovered) {
        // 服务发现完成，显示特征值
        ui->charList->clear();
        const QList<QLowEnergyCharacteristic> chars = service->characteristics();

        for (const QLowEnergyCharacteristic &ch : chars) {
            QString charInfo = QString("%1 - %2")
            .arg(ch.uuid().toString())
                .arg(ch.name());

            QListWidgetItem *item = new QListWidgetItem(charInfo);
            item->setData(Qt::UserRole, QVariant::fromValue(ch));

            // 根据特征值属性设置不同颜色
            if (ch.properties() & QLowEnergyCharacteristic::Write) {
                item->setForeground(Qt::blue);
            }
            if (ch.properties() & QLowEnergyCharacteristic::Read) {
                item->setForeground(Qt::darkGreen);
            }
            if (ch.properties() & QLowEnergyCharacteristic::Notify) {
                item->setForeground(Qt::darkRed);
            }

            ui->charList->addItem(item);
        }

        // logMessage(QString("服务 %1 发现 %2 个特征值")
        //                .arg(service->uuid().toString())
        //                .arg(chars.count()));
        logMessage(QString("发现 %1 个特征值").arg(chars.count()));
    }
}

// 特征值更新
void MainWindow::updateCharacteristicValue(const QLowEnergyCharacteristic &c, const QByteArray &value)
{
    /** 显示两个字节十六进制数 */
    QString decimalStr = QString("特征值 %1 (十六进制: %2)").arg(c.uuid().toString()).arg(value.toHex().data());
    logMessage(decimalStr);

    // QString decimalStr = QString("特征值 %1 (十六进制: %2)").arg(c.uuid().toString()).arg(value.data());
    // logMessage(decimalStr);

    /** 显示四个字节十进制数 */
    /** 新增：将QByteArray的每个字节转为十进制字符串，并用空格分隔 */
    // QString decimalStr;
    // for (int i = 0; i < value.size(); i+=4) {
    //     // 取出单个字节的十进制值（0~255）
    //     quint32 byteValue = (static_cast<quint32>(static_cast<quint8>(value.at(i))) << 24) |
    //                         (static_cast<quint32>(static_cast<quint8>(value.at(i + 1))) << 16) |
    //                         (static_cast<quint32>(static_cast<quint8>(value.at(i + 2))) << 8) |
    //                         (static_cast<quint32>(static_cast<quint8>(value.at(i + 3))));
    //     decimalStr += QString::number(byteValue);
    //     // 最后一个字节不加空格，避免末尾多余空格
    //     if (i +4 < value.size()) {
    //         decimalStr += ",";
    //     }
    // }
    // /** 修改日志输出：替换原十六进制为十进制（空格分隔） */
    // logMessage(QString("特征值 %1 (十进制: %2)")
    //                .arg(c.uuid().toString())
    //                .arg(decimalStr)); // 改用拼接好的十进制字符串

    /** 显示两个字节十进制数 */
    /** 新增：将QByteArray的每个字节转为十进制字符串，并用空格分隔 */
    // QString decimalStr;
    // for (int i = 0; i < value.size(); i+=2) {
    //     // 取出单个字节的十进制值（0~255）
    //     quint16 byteValue = (static_cast<quint16>(value.at(i)) << 8) |
    //                         (static_cast<quint16>(value.at(i + 1)));
    //     decimalStr += QString::number(byteValue); // 将字符串输出为整数
    //     // 最后一个字节不加空格，避免末尾多余空格
    //     if (i != value.size() - 1) {
    //         decimalStr += ",";
    //     }
    // }
    // /** 修改日志输出：替换原十六进制为十进制（空格分隔） */
    // logMessage(QString("特征值 %1 (十进制: %2)")
    //                .arg(c.uuid().toString())
    //                .arg(decimalStr)); // 改用拼接好的十进制字符串

    /** 显示单字节十进制数 */
    /** 新增：将QByteArray的每个字节转为十进制字符串，并用空格分隔 */
    // QString decimalStr;
    // for (int i = 0; i < value.size(); i++) {
    //     // 取出单个字节的十进制值（0~255）
    //     quint8 byteValue = static_cast<quint8>(value.at(i));
    //     decimalStr += QString::number(byteValue); // 将字符串输出为整数
    //     // 最后一个字节不加空格，避免末尾多余空格
    //     if (i != value.size() - 1) {
    //         decimalStr += ",";
    //     }
    // }
    /** 修改日志输出：替换原十六进制为十进制（空格分隔） */
    // logMessage(QString("特征值 %1 (十进制: %2)")
    //                .arg(c.uuid().toString())
    //                .arg(decimalStr)); // 改用拼接好的十进制字符串

    /** 将数据写入TXT文件 */
    writeDataToTxt(decimalStr, "ble_hex_data.txt");

    /** 如果是当前选中的特征值，更新显示 */
    if (c == currentChar) {
        ui->valueEdit->setText(value);
        ui->hexValueEdit->setText(value.toHex());
    }
}

// 特征值写入完成
void MainWindow::characteristicWriteFinished(const QLowEnergyCharacteristic &c, const QByteArray &value)
{
    logMessage(QString("特征值 %1 写入完成: %2 (十六进制: %3)")
                   .arg(c.uuid().toString())
                   .arg(QString(value))
                   .arg(value.toHex().data()));
}

// 写入按钮点击事件
void MainWindow::on_writeButton_clicked()
{
    if (!currentChar.isValid()) return;

    // 获取要写入的值
    QByteArray value;
    if (ui->hexRadio->isChecked()) {
        // 十六进制模式
        value = QByteArray::fromHex(ui->writeValueEdit->text().toUtf8());
    } else {
        // 字符串模式
        value = ui->writeValueEdit->text().toUtf8();
    }

    // 写入特征值
    if (currentChar.properties() & QLowEnergyCharacteristic::WriteNoResponse) {
        QLowEnergyService *service = services.value(currentChar.uuid());
        if (service) {
            service->writeCharacteristic(currentChar, value, QLowEnergyService::WriteWithoutResponse);
            logMessage(QString("正在写入特征值 %1: %2").arg(currentChar.uuid().toString()).arg(QString(value)));
        }
    } else if (currentChar.properties() & QLowEnergyCharacteristic::Write) {
        QLowEnergyService *service = services.value(currentChar.uuid());
        if (service) {
            service->writeCharacteristic(currentChar, value);
            logMessage(QString("正在写入特征值 %1: %2").arg(currentChar.uuid().toString()).arg(QString(value)));
        }
    } else {
        logMessage("该特征值不支持写入操作");
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

// 服务列表点击事件 - 自动生成的槽函数
void MainWindow::on_serviceList_itemClicked(QListWidgetItem *item)
{
    if (!item) {
        logMessage("错误：选中的列表项为空");
        return;
    }

    // 第一步：读取serviceUuid（确保类型是QBluetoothUuid）
    QVariant var = item->data(Qt::UserRole);
    if (!var.canConvert<QBluetoothUuid>()) {
        logMessage(QString("错误：列表项数据类型错误，当前类型：%1").arg(var.typeName()));
        return;
    }

    QBluetoothUuid serviceUuid = var.value<QBluetoothUuid>();
    logMessage(QString("选中服务UUID：%1").arg(serviceUuid.toString()));

    serviceUuid = item->data(Qt::UserRole).value<QBluetoothUuid>();
    QLowEnergyService *service = services.value(serviceUuid);

    if (service) {
        service->discoverDetails(); // 发现服务详情（特征值等）
        logMessage(QString("正在获取服务 %1 的详情").arg(serviceUuid.toString()));
    } else {
        logMessage("未找到选中的服务对象");
    }
}

// 特征值列表点击事件 - 自动生成的槽函数
void MainWindow::on_charList_itemClicked(QListWidgetItem *item)
{
    if (!item) return;

    currentChar = item->data(Qt::UserRole).value<QLowEnergyCharacteristic>();

    // 检查是否可以写入
    bool canWrite = (currentChar.properties() & QLowEnergyCharacteristic::Write) ||
                    (currentChar.properties() & QLowEnergyCharacteristic::WriteNoResponse);
    ui->writeButton->setEnabled(canWrite);
    if(!read_status) {
        read_status = true;
        // 读取当前值
        if (currentChar.properties() & QLowEnergyCharacteristic::Read) {
            QLowEnergyService *service = services.value(currentChar.uuid());
            if (service) {
                QByteArray value = currentChar.value();
                ui->valueEdit->setText(value);
                ui->hexValueEdit->setText(value.toHex());
                logMessage(QString("读取特征值 %1: %2 (十六进制: %3)")
                               .arg(currentChar.uuid().toString())
                               .arg(QString(value))
                               .arg(value.toHex().data()));
            }
        }

        // 启用通知或指示（核心接收逻辑）
        if (currentChar.properties() & (QLowEnergyCharacteristic::Notify | QLowEnergyCharacteristic::Indicate)) {
            // 查找当前特征值所属的服务
            QLowEnergyService *service = nullptr;
            for (auto s : services.values()) {
                if (s->characteristics().contains(currentChar)) {
                    service = s;
                    break;
                }
            }
            if (!service) {
                logMessage("未找到特征值所属的服务");
                return;
            }

            m_notifyChar = currentChar;
            m_notifyService = service;

            // 用标准UUID字符串获取客户端特征配置描述符
            m_notifyDesc = currentChar.descriptor(
                QBluetoothUuid(QStringLiteral("00002902-0000-1000-8000-00805f9b34fb"))
                );
            if (m_notifyDesc.isValid()) {
                QByteArray enableData;
                if (currentChar.properties() & QLowEnergyCharacteristic::Notify) {
                    enableData = QByteArray::fromHex("0100"); // 启用通知
                    logMessage(QString("启用特征值 %1 的通知").arg(currentChar.uuid().toString()));
                } else if (currentChar.properties() & QLowEnergyCharacteristic::Indicate) {
                    enableData = QByteArray::fromHex("0200"); // 启用指示
                    logMessage(QString("启用特征值 %1 的指示").arg(currentChar.uuid().toString()));
                }
                // 写入描述符以启用通知/指示
                service->writeDescriptor(m_notifyDesc, enableData);
            } else {
                logMessage(QString("特征值 %1 不支持通知（无配置描述符）").arg(currentChar.uuid().toString()));
            }
        }
    }
    else
    {
        read_status = false;
        logMessage("关闭读取特征值");
        // 1. 禁用通知/指示（核心：向2902描述符写入0000）
        if (m_notifyDesc.isValid() && m_notifyService) {
            // 写入0000禁用通知/指示（通用禁用指令）
            QByteArray disableData = QByteArray::fromHex("0000");
            m_notifyService->writeDescriptor(m_notifyDesc, disableData);
            logMessage(QString("禁用特征值 %1 的通知/指示").arg(m_notifyChar.uuid().toString()));

            // 重置订阅相关对象
            m_notifyChar = QLowEnergyCharacteristic();
            m_notifyService = nullptr;
            m_notifyDesc = QLowEnergyDescriptor();
        } else {
            logMessage("无活跃的订阅特征值，无需禁用");
        }

        // // 2. 清空特征值显示（可选，提升用户体验）
        // ui->valueEdit->clear();
        // ui->hexValueEdit->clear();

        // // 3. 禁用写入按钮（可选，根据需求）
        // ui->writeButton->setEnabled(false);
    }
}

