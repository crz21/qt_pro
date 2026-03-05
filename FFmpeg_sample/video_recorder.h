#ifndef VIDEO_RECORDER_H
#define VIDEO_RECORDER_H

#include <QObject>
#include <QString>
#include <QMutex>
#include <QImage>
#include <QMessageBox>
#include <QWidget>
#include "video_previewer.h"

#include <time.h> 

// FFmpeg 前置声明
struct AVFormatContext;
struct AVCodecContext;
struct AVStream;
struct SwsContext;
struct AVFrame;
struct AVPacket;

class VideoRecorder : public QObject
{
    Q_OBJECT
public:
    explicit VideoRecorder(VideoPreviewer *previewer, QObject *parent = nullptr);
    ~VideoRecorder();
    
    bool startRecording(const QString &filename);
    void stopRecording();
    bool isRecording() const { return m_isRecording; }
    void show_message();

private slots:
    void onFrameReceived(const QImage &frame);

private:
    int64_t get_system_time_us();

private:
    VideoPreviewer *m_previewer;
    bool m_isRecording;
    QMutex m_mutex;
    AVFormatContext *m_outputCtx;
    AVCodecContext *m_encoderCtx;
    AVStream *m_videoStream;
    int64_t m_startTime;
    int m_frameCount;
    SwsContext *m_swsCtx;
    int m_encodeWidth;
    int m_encodeHeight;
    AVFrame *m_reusableNv12Frame;  // 复用 NV12 帧
    AVPacket *m_reusablePacket;    // 复用编码包
};

#endif // VIDEORECORDER_H
