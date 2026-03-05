#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QDateTime>
#include <QElapsedTimer>  // 用于高精度计时
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QStyle>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include "video_previewer.h"
#include "video_recorder.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
   public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();
    void set_time();

   public slots:
    void open_camera();
    void close_camera();
    void start_recording();
    void stop_recording();
    void update_time();

   private:
    VideoPreviewer* m_previewer;
    PreviewWidget* m_previewWidget;
    VideoRecorder* m_recorder;
    bool isRecording;
    QPushButton* startPreviewBtn = nullptr;
    QPushButton* stopPreviewBtn = nullptr;
    QPushButton* startRecordBtn = nullptr;
    QPushButton* stopRecordBtn = nullptr;
    QTimer* m_timer;  // 定时器对象
    QLabel* m_timeLabel;
    QElapsedTimer m_elapsedTimer;  // 记录流逝的绝对时间
    qint64 m_totalElapsed;         // 累计的毫秒数（用于暂停后继续计时）
};

#endif
