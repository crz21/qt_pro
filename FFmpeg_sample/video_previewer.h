#ifndef VIDEO_PREVIEWER_H
#define VIDEO_PREVIEWER_H

#include <QImage>
#include <QLabel>
#include <QMutex>
#include <QObject>
#include <QPainter>
#include <QStyle>
#include <QThread>
#include <QWidget>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

class VideoPreviewer : public QThread
{
    Q_OBJECT
   public:
    explicit VideoPreviewer(QObject* parent = nullptr);
    ~VideoPreviewer();

    bool startPreview(const QString& device = "/dev/video11");
    void stopPreview();
    bool isPreviewing() const { return m_isPreviewing; }

    int getPreviewWidth() const { return m_codecCtx ? m_codecCtx->width : 0; }
    int getPreviewHeight() const { return m_codecCtx ? m_codecCtx->height : 0; }
    int getPreviewFps() const { return m_codecCtx ? m_codecCtx->framerate.num : 30; }
   signals:
    void frameReady(const QImage& frame);

   protected:
    void run() override;

   private:
    bool m_isPreviewing;
    QMutex m_mutex;
    AVFormatContext* m_formatCtx;
    AVCodecContext* m_codecCtx;
    int m_videoStreamIndex;
    struct SwsContext* m_swsCtx;
};

class PreviewWidget : public QWidget
{
    Q_OBJECT
   public:
    explicit PreviewWidget(QWidget* parent = nullptr) : QWidget(parent) {}

   public slots:
    void onFrameReady(const QImage& frame)
    {
        m_frame = frame;
        update();
    }

   protected:
    void paintEvent(QPaintEvent* event) override
    {
        Q_UNUSED(event);
        if (!m_frame.isNull()) {
            QPainter painter(this);
            painter.drawImage(rect(), m_frame.scaled(size(), Qt::KeepAspectRatio));
        }
    }

   private:
    QImage m_frame;
};

#endif  // VIDEOPREVIEWER_H
