/* Copyright 2019 Tronlong Elec. Tech. Co. Ltd. All Rights Reserved. */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QCamera>
#include <QVideoWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QCameraInfo>
#include <QMessageBox>
#include <QStyle>
#include <QApplication>
// #include <QMediaDevices>
// #include <QCameraDevice>
// #include <QMediaCaptureSession>  // 媒体捕获会话（Qt 6 必需）
#include <QMediaRecorder>       // 录像核心类
#include <QDateTime>            // 用于生成带时间戳的录像文件名
#include <QDir>                 // 处理文件路径
#include <QUrl>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openCamera();
    void closeCamera();
    void startRecording();
    void stopRecording();
    void updateButtonState();

private:
    QCamera *camera;             // 摄像头对象
    // QCamera *camera_rc;
    QVideoWidget *viewfinder; // 摄像头预览窗口
    QPushButton *openButton;     // 打开摄像头按钮
    QPushButton *closeButton;    // 关闭摄像头按钮

    // 新增录像相关成员变量
    // QMediaCaptureSession *captureSession = nullptr;
    QMediaRecorder *mediaRecorder = nullptr;
    QPushButton *startRecordBtn = nullptr;
    QPushButton *stopRecordBtn = nullptr;
    bool isRecording;  // 标记是否正在录像
};

#endif // MAINWINDOW_H
