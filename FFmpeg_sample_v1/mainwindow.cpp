#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    // 创建预览和录像组件
    m_previewer = new VideoPreviewer(this);
    m_previewWidget = new PreviewWidget(this);
    m_recorder = new VideoRecorder(m_previewer, this);
    isRecording = false;
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

    /** botton */
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(20);

    startPreviewBtn = new QPushButton("开始预览", this);
    startPreviewBtn->setMinimumSize(180, 80);
    startPreviewBtn->setStyleSheet("font-size: 24px; background-color: #904cafff; color: white; border-radius: 10px;");
    buttonLayout->addWidget(startPreviewBtn);

    stopPreviewBtn = new QPushButton("停止预览", this);
    stopPreviewBtn->setMinimumSize(180, 80);
    stopPreviewBtn->setStyleSheet("font-size: 24px; background-color: #904cafff; color: white; border-radius: 10px;");
    buttonLayout->addWidget(stopPreviewBtn);

    startRecordBtn = new QPushButton("开始录像", this);
    startRecordBtn->setMinimumSize(180, 80);
    startRecordBtn->setStyleSheet("font-size: 24px; background-color: #af814cff; color: white; border-radius: 10px;");
    buttonLayout->addWidget(startRecordBtn);

    stopRecordBtn = new QPushButton("停止录像", this);
    stopRecordBtn->setMinimumSize(180, 80);
    stopRecordBtn->setStyleSheet("font-size: 24px; background-color: #afa54cff; color: white; border-radius: 10px;");
    buttonLayout->addWidget(stopRecordBtn);

    mainLayout->addLayout(buttonLayout);
    setCentralWidget(centralWidget);

    m_timeLabel = new QLabel(this);
    m_timeLabel->setAlignment(Qt::AlignCenter);
    QFont font = m_timeLabel->font();
    font.setPointSize(12);
    m_timeLabel->setFont(font);
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);

    connect(m_timer, &QTimer::timeout, this, &MainWindow::update_time);
    connect(startPreviewBtn, &QPushButton::clicked, this, &MainWindow::open_camera);
    connect(stopPreviewBtn, &QPushButton::clicked, this, &MainWindow::close_camera);
    connect(startRecordBtn, &QPushButton::clicked, this, &MainWindow::start_recording);
    connect(stopRecordBtn, &QPushButton::clicked, this, &MainWindow::stop_recording);
    connect(stopRecordBtn, &QPushButton::clicked, this, &MainWindow::stop_recording);
}

MainWindow::~MainWindow()
{
    m_previewer->close();
    m_recorder->stop();
}

void MainWindow::open_camera()
{
    startRecordBtn->setMinimumSize(180, 80);
    stopRecordBtn->setMinimumSize(180, 80);
    m_previewer->open();
    connect(m_previewer, &VideoPreviewer::frameReady, m_previewWidget, &PreviewWidget::onFrameReady);
}

void MainWindow::close_camera()
{
    startRecordBtn->setMinimumSize(180, 80);
    stopRecordBtn->setMinimumSize(180, 80);
    m_previewer->close();
}

void MainWindow::start_recording()
{
    startRecordBtn->setMinimumSize(80, 40);
    stopRecordBtn->setMinimumSize(180, 80);
    set_time();
    m_timer->start();
    QString filename = QString("/root/recording_%1.mp4").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    m_recorder->record(filename);
    // QMessageBox::information(this, "提示", "录像开始！");
}

void MainWindow::stop_recording()
{
    m_timer->stop();
    set_time();
    startRecordBtn->setMinimumSize(180, 80);
    stopRecordBtn->setMinimumSize(80, 40);
    m_recorder->stop();
    // QMessageBox::information(this, "提示", "录像已停止，文件已保存！");
}

void MainWindow::set_time()
{
    m_totalElapsed = 0;
    QString timeStr = "00:00:00";
    m_timeLabel->setText(timeStr);
}

void MainWindow::update_time()
{
    m_totalElapsed++;

    int seconds = m_totalElapsed % 60;
    int hours = m_totalElapsed / 3600;
    int minutes = (m_totalElapsed % 3600) / 60;

    // 格式化字符串（补零确保两位或三位）
    QString timeStr = QString("%1:%2:%3")
                          .arg(hours, 2, 10, QChar('0'))
                          .arg(minutes, 2, 10, QChar('0'))
                          .arg(seconds, 2, 10, QChar('0'));
    m_timeLabel->setText(timeStr);
}
