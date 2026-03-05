#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    // 创建预览和录像组件
    m_previewer = new VideoPreviewer(this);
    m_previewWidget = new PreviewWidget(this);
    m_recorder = new VideoRecorder(m_previewer, this);

    /** title */
    setWindowTitle("RK3576 视频预览与录像");
    setMinimumSize(800, 600);

    /** view  */
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 10, 10, 200);
    m_previewWidget->setMinimumSize(20, 40);
    m_previewWidget->setStyleSheet("border: 2px solid #664cafff;");
    mainLayout->addWidget(m_previewWidget);
    connect(m_previewer, &VideoPreviewer::frameReady, m_previewWidget, &PreviewWidget::onFrameReady);

    /** botton */
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(20);
    QPushButton* startPreviewBtn = new QPushButton("开始预览", this);
    startPreviewBtn->setMinimumSize(180, 80);
    startPreviewBtn->setStyleSheet("font-size: 24px; background-color: #904cafff; color: white; border-radius: 10px;");
    buttonLayout->addWidget(startPreviewBtn);
    QPushButton* stopPreviewBtn = new QPushButton("停止预览", this);
    stopPreviewBtn->setMinimumSize(180, 80);
    stopPreviewBtn->setStyleSheet("font-size: 24px; background-color: #904cafff; color: white; border-radius: 10px;");
    buttonLayout->addWidget(stopPreviewBtn);
    QPushButton* startRecordBtn = new QPushButton("开始录像", this);
    startRecordBtn->setMinimumSize(180, 80);
    startRecordBtn->setStyleSheet("font-size: 24px; background-color: #af814cff; color: white; border-radius: 10px;");
    buttonLayout->addWidget(startRecordBtn);
    QPushButton* stopRecordBtn = new QPushButton("停止录像", this);
    stopRecordBtn->setMinimumSize(180, 80);
    stopRecordBtn->setStyleSheet("font-size: 24px; background-color: #afa54cff; color: white; border-radius: 10px;");
    buttonLayout->addWidget(stopRecordBtn);

    mainLayout->addLayout(buttonLayout);
    setCentralWidget(centralWidget);

    connect(startPreviewBtn, &QPushButton::clicked, [this]() { m_previewer->startPreview(); });
    connect(stopPreviewBtn, &QPushButton::clicked, [this]() { m_previewer->stopPreview(); });
    connect(startRecordBtn, &QPushButton::clicked, [this]() {
        QString filename =
            QString("/root/recording_%1.mp4").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
        m_recorder->startRecording(filename);
    });
    connect(stopRecordBtn, &QPushButton::clicked, [this]() { m_recorder->stopRecording(); });
}

MainWindow::~MainWindow() {}
