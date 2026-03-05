#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QMessageBox>
#include <QBluetoothUuid>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupUI();
    
    // 初始化蓝牙相关对象
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    localDevice = new QBluetoothLocalDevice(this);
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
    
    // 连接信号与槽
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &MainWindow::deviceDiscovered);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &MainWindow::discoveryFinished);
    
    connect(socket, &QBluetoothSocket::connected, this, &MainWindow::socketConnected);
    connect(socket, &QBluetoothSocket::disconnected, this, &MainWindow::socketDisconnected);
    connect(socket, &QBluetoothSocket::readyRead, this, &MainWindow::dataReceived);
    connect(socket, QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::errorOccurred),
            this, &MainWindow::errorOccurred);
    connect(socket, &QBluetoothSocket::stateChanged, this, &MainWindow::stateChanged);
    
    // 检查蓝牙是否可用
    if (localDevice->isValid()) {
        // 使蓝牙可见
        localDevice->setHostMode(QBluetoothLocalDevice::HostDiscoverable);
        logMessage("本地蓝牙设备已初始化");
    } else {
        logMessage("未检测到可用的蓝牙设备", false);
        ui->scanButton->setEnabled(false);
    }
}

MainWindow::~MainWindow()
{
    if (socket->isOpen()) {
        socket->close();
    }
    delete ui;
}

void MainWindow::setupUI()
{
    setWindowTitle("蓝牙收发工具");
    ui->sendButton->setEnabled(false);
    ui->disconnectButton->setEnabled(false);
    
    // 设置列表控件
    ui->deviceListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    
    // 设置日志区域只读
    ui->logTextEdit->setReadOnly(true);
}

void MainWindow::logMessage(const QString &message, bool isReceived)
{
    QString timeStamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString logEntry;
    
    if (isReceived) {
        logEntry = QString("[%1] 接收: %2").arg(timeStamp).arg(message);
    } else {
        logEntry = QString("[%1] 发送: %2").arg(timeStamp).arg(message);
    }
    
    ui->logTextEdit->append(logEntry);
    // 滚动到底部
    ui->logTextEdit->moveCursor(QTextCursor::End);
}

void MainWindow::on_scanButton_clicked()
{
    ui->deviceListWidget->clear();
    logMessage("开始扫描蓝牙设备...", false);
    
    discoveryAgent->start();
    ui->scanButton->setEnabled(false);
}

void MainWindow::deviceDiscovered(const QBluetoothDeviceInfo &info)
{
    QString deviceName = info.name();
    QString deviceAddress = info.address().toString();
    
    if (deviceName.isEmpty()) {
        deviceName = "未知设备";
    }
    
    QString itemText = QString("%1 (%2)").arg(deviceName).arg(deviceAddress);
    QListWidgetItem *item = new QListWidgetItem(itemText, ui->deviceListWidget);
    item->setData(Qt::UserRole, QVariant::fromValue(info.address()));
}

void MainWindow::discoveryFinished()
{
    logMessage(QString("扫描完成，共发现 %1 个设备").arg(ui->deviceListWidget->count()), false);
    ui->scanButton->setEnabled(true);
}

void MainWindow::on_connectButton_clicked()
{
    QListWidgetItem *item = ui->deviceListWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, "警告", "请先选择一个蓝牙设备");
        return;
    }
    
    currentAddress = item->data(Qt::UserRole).value<QBluetoothAddress>();
    
    if (socket->state() == QBluetoothSocket::ConnectedState) {
        socket->disconnectFromService();
    }
    
    // 连接到RFCOMM服务
    static const QBluetoothUuid uuid(QBluetoothUuid::SerialPort);
    socket->connectToService(currentAddress, uuid);
    
    logMessage("正在连接到设备...", false);
}

void MainWindow::on_sendButton_clicked()
{
    QString message = ui->sendTextEdit->toPlainText();
    if (message.isEmpty()) {
        return;
    }
    
    if (socket->state() == QBluetoothSocket::ConnectedState) {
        socket->write(message.toUtf8());
        logMessage(message, false);
        ui->sendTextEdit->clear();
    } else {
        QMessageBox::warning(this, "警告", "未连接到设备");
    }
}

void MainWindow::on_disconnectButton_clicked()
{
    if (socket->state() == QBluetoothSocket::ConnectedState) {
        socket->disconnectFromService();
    }
}

void MainWindow::on_clearButton_clicked()
{
    ui->logTextEdit->clear();
}

void MainWindow::socketConnected()
{
    logMessage("已成功连接到设备", false);
    ui->connectButton->setEnabled(false);
    ui->disconnectButton->setEnabled(true);
    ui->sendButton->setEnabled(true);
    ui->scanButton->setEnabled(false);
}

void MainWindow::socketDisconnected()
{
    logMessage("已与设备断开连接", false);
    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);
    ui->sendButton->setEnabled(false);
    ui->scanButton->setEnabled(true);
}

void MainWindow::dataReceived()
{
    QByteArray data = socket->readAll();
    logMessage(QString::fromUtf8(data));
}

void MainWindow::errorOccurred(QBluetoothSocket::SocketError error)
{
    QString errorString;
    switch (error) {
    case QBluetoothSocket::NoSocketError:
        errorString = "无错误";
        break;
    case QBluetoothSocket::UnknownSocketError:
        errorString = "未知错误";
        break;
    case QBluetoothSocket::HostNotFoundError:
        errorString = "未找到主机";
        break;
    case QBluetoothSocket::ConnectionRefusedError:
        errorString = "连接被拒绝";
        break;
    case QBluetoothSocket::RemoteHostClosedError:
        errorString = "远程主机已关闭";
        break;
    default:
        errorString = socket->errorString();
    }
    
    logMessage(QString("错误: %1").arg(errorString), false);
}

void MainWindow::stateChanged(QBluetoothSocket::SocketState state)
{
    QString stateString;
    switch (state) {
    case QBluetoothSocket::UnconnectedState:
        stateString = "未连接";
        break;
    case QBluetoothSocket::ConnectingState:
        stateString = "连接中";
        break;
    case QBluetoothSocket::ConnectedState:
        stateString = "已连接";
        break;
    case QBluetoothSocket::ClosingState:
        stateString = "关闭中";
        break;
    }
    
    ui->statusLabel->setText(QString("状态: %1").arg(stateString));
}
