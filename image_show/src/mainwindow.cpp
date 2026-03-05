#include "mainwindow.h"

#include <assert.h>
#include <limits.h>
#include <stdlib.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    setWindowTitle("摄像头控制器");
    setMinimumSize(800, 600);

    isRecording = false;                              // 录像状态标记
    camera = nullptr;
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 10, 10, 200);
    viewfinder = new QVideoWidget(this);
    viewfinder->setMinimumSize(20, 40);
    viewfinder->setStyleSheet("border: 2px solid #664cafff;");
    mainLayout->addWidget(viewfinder);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(30);

    openButton = new QPushButton("打开摄像头", this);
    openButton->setMinimumSize(180, 80);
    openButton->setStyleSheet("font-size: 24px; background-color: #904cafff; color: white; border-radius: 10px;");
    buttonLayout->addWidget(openButton);

    closeButton = new QPushButton("关闭摄像头", this);
    closeButton->setMinimumSize(180, 80);
    closeButton->setStyleSheet("font-size: 24px; background-color: #f44336; color: white; border-radius: 10px;");
    closeButton->setEnabled(false);// 初始禁用
    buttonLayout->addWidget(closeButton);

    startRecordBtn = new QPushButton("开始录像", this);
    startRecordBtn->setMinimumSize(180, 80);
    startRecordBtn->setStyleSheet("font-size: 22px; background-color: #ff9800; color: white; border-radius: 10px;");
    startRecordBtn->setEnabled(false);  // 初始禁用（摄像头未开）
    buttonLayout->addWidget(startRecordBtn);

    stopRecordBtn = new QPushButton("停止录像", this);
    stopRecordBtn->setMinimumSize(180, 80);
    stopRecordBtn->setStyleSheet("font-size: 22px; background-color: #f44336; color: white; border-radius: 10px;");
    stopRecordBtn->setEnabled(false);  // 初始禁用（未录像）
    buttonLayout->addWidget(stopRecordBtn);

    mainLayout->addLayout(buttonLayout);
    setCentralWidget(centralWidget);

    connect(openButton, &QPushButton::clicked, this, &MainWindow::openCamera);
    connect(closeButton, &QPushButton::clicked, this, &MainWindow::closeCamera);
    connect(startRecordBtn, &QPushButton::clicked, this, &MainWindow::startRecording);
    connect(stopRecordBtn, &QPushButton::clicked, this, &MainWindow::stopRecording);
}

void MainWindow::openCamera() {
    // 如果已有摄像头实例，先关闭
    if (camera) closeCamera();

    // 获取可用摄像头列表
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    if (cameras.isEmpty()) {
        QMessageBox::warning(this, "错误", "未检测到可用摄像头！");
        return;
    }

    // 初始化摄像头并设置预览窗口
    camera = new QCamera(cameras[0], this);
    camera->setViewfinder(viewfinder);
    mediaRecorder = new QMediaRecorder(camera,this);         // 录像器
    // 设置录像格式（Qt 5用QVideoEncoderSettings）
    QVideoEncoderSettings videoSettings;
    QCameraViewfinderSettings viewfinderSettings;
    // viewfinderSettings.setPixelFormat(QVideoFrame::Format_NV12);  // 匹配硬件编码器输入格式
    viewfinderSettings.setMaximumFrameRate(10);
    viewfinderSettings.setResolution(320, 240);                  // 分辨率
    camera->setViewfinderSettings(viewfinderSettings);
    videoSettings.setCodec("video/x-h264");  // MP4编码
    videoSettings.setResolution(320, 240); // 分辨率
    videoSettings.setFrameRate(10);        // 帧率
    mediaRecorder->setVideoSettings(videoSettings);

    // 启动摄像头
    camera->start();

    // 更新按钮状态
    openButton->setEnabled(false);
    closeButton->setEnabled(true);
    startRecordBtn->setEnabled(true);
    stopRecordBtn->setEnabled(true);
}

void MainWindow::closeCamera() {
    if (camera) {
        camera->stop();
        delete camera;
        camera = nullptr;
    }
    if (mediaRecorder) {
        delete mediaRecorder;
        mediaRecorder = nullptr;
    }
    // 更新按钮状态
    openButton->setEnabled(true);
    closeButton->setEnabled(false);
    startRecordBtn->setEnabled(false);
    stopRecordBtn->setEnabled(false);
}

// 开始录像（新增槽函数）
void MainWindow::startRecording() {
    if (!camera || !mediaRecorder || isRecording) return;

    // 生成带时间戳的录像文件名（避免覆盖）
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    QString savePath = "/home/ftp/rec.h264";

    // 设置录像保存路径并开始录制
    // QString savePath = "/home/root/recordings/rec_202405.mp4";  // 你的路径

    mediaRecorder->setOutputLocation(QUrl::fromLocalFile(savePath));
    mediaRecorder->record();
    isRecording = true;
    // updateButtonState();  // 禁用开始录像、启用停止录像
    openButton->setEnabled(true);
    closeButton->setEnabled(true);
    startRecordBtn->setEnabled(false);
    stopRecordBtn->setEnabled(true);

}

// 停止录像（新增槽函数）
void MainWindow::stopRecording() {
    if (!mediaRecorder || !isRecording) return;

    mediaRecorder->stop();
    isRecording = false;
    // updateButtonState();  // 启用开始录像、禁用停止录像
    openButton->setEnabled(true);
    closeButton->setEnabled(true);
    startRecordBtn->setEnabled(true);
    stopRecordBtn->setEnabled(false);
    QMessageBox::information(this, "提示", "录像已停止，文件已保存！");
}

// 统一更新按钮状态（新增，避免重复代码）
void MainWindow::updateButtonState() {
    bool cameraActive = (camera && camera->state() == QCamera::ActiveState);

    openButton->setEnabled(!cameraActive);       // 摄像头未开时启用“打开”
    closeButton->setEnabled(cameraActive);       // 摄像头已开时启用“关闭”
    startRecordBtn->setEnabled(cameraActive && !isRecording); // 已开且未录像时启用“开始”
    stopRecordBtn->setEnabled(cameraActive && isRecording);  // 已开且录像时启用“停止”
}

MainWindow::~MainWindow() {
    if (mediaRecorder->state() == QMediaRecorder::RecordingState) {
        mediaRecorder->stop();
        QMessageBox::information(this, "提示", "选择停止录像！");
    }

    if (camera) {
        camera->stop();
        delete camera;
    }
}
