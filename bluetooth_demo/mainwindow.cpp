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
    setWindowTitle("SPP上位机");

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
    logMessage(QString("扫描完成，共发现 %1 个SPP设备").arg(ui->deviceList->count()));
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

void MainWindow::on_deviceList_itemDoubleClicked(QListWidgetItem *item)
{
    if (sppSocket->state() != QBluetoothSocket::SocketState::UnconnectedState) {
        sppSocket->disconnectFromService();
    }

    // 1. 获取选中的设备信息
    QBluetoothDeviceInfo info = item->data(Qt::UserRole).value<QBluetoothDeviceInfo>();
    logMessage(QString("正在尝试直接连接设备: %1 (%2)...").arg(info.name()).arg(info.address().toString()));

    // 2. 使用经典蓝牙 SPP 标准串口 UUID 强行连接
    // 自定义的 SPP UUID 值为 "00007133-0000-1000-8000-00805F9B34FB"
    QBluetoothUuid sppUuid(QBluetoothUuid::ServiceClassUuid::BarcoSppRecordService);
    // QBluetoothUuid sppUuid(QBluetoothUuid::ServiceClassUuid::BarcoSppService);

    // 3. 异步发起连接
    sppSocket->connectToService(info.address(), sppUuid, QIODevice::ReadWrite);

    // QBluetoothServiceInfo info = item->data(Qt::UserRole).value<QBluetoothServiceInfo>();

    // sppSocket->connectToService(info, QIODevice::ReadWrite);
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
    QString logStr = QString("接收数据（十六进制）: %1").arg(hexStr);
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
        logMessage(QString("发送数据（十六进制）: %1")
                       .arg(value.toHex(' ').toUpper()));
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
