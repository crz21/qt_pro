#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothLocalDevice>
#include <QBluetoothSocket>
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
    void on_scanButton_clicked();
    void on_connectButton_clicked();
    void on_sendButton_clicked();
    void on_disconnectButton_clicked();
    void on_clearButton_clicked();
    
    // 蓝牙相关槽函数
    void deviceDiscovered(const QBluetoothDeviceInfo &info);
    void discoveryFinished();
    void socketConnected();
    void socketDisconnected();
    void dataReceived();
    void errorOccurred(QBluetoothSocket::SocketError error);
    void stateChanged(QBluetoothSocket::SocketState state);

private:
    Ui::MainWindow *ui;
    
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QBluetoothLocalDevice *localDevice;
    QBluetoothSocket *socket;
    QBluetoothAddress currentAddress;
    
    void setupUI();
    void logMessage(const QString &message, bool isReceived = true);
};
#endif // MAINWINDOW_H
