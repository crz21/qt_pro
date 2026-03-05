#include <QDebug>
#include <QMessageBox>
#include "mainwindow.h"
#include "video_recorder.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/error.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

VideoRecorder::VideoRecorder(VideoPreviewer* previewer, QObject* parent)
    : QObject(parent),
      m_previewer(previewer),
      m_isRecording(false),
      m_outputCtx(nullptr),
      m_encoderCtx(nullptr),
      m_videoStream(nullptr),
      m_startTime(0),
      m_frameCount(0),
      m_swsCtx(nullptr),
      m_encodeWidth(0),
      m_encodeHeight(0),
      m_reusableNv12Frame(nullptr),
      m_reusablePacket(nullptr)
{
    connect(previewer, &VideoPreviewer::frameReady, this, &VideoRecorder::onFrameReceived);
    m_reusablePacket = av_packet_alloc();  // 提前分配复用包
}

VideoRecorder::~VideoRecorder()
{
    stopRecording();
    if (m_swsCtx) sws_freeContext(m_swsCtx);
    if (m_reusableNv12Frame) av_frame_free(&m_reusableNv12Frame);
    if (m_reusablePacket) av_packet_free(&m_reusablePacket);
}

int64_t VideoRecorder::get_system_time_us()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}

void VideoRecorder::show_message()
{
    QMessageBox::information(nullptr,"提示","保存完成！");
}

bool VideoRecorder::startRecording(const QString& filename)
{
    QMutexLocker locker(&m_mutex);
    if (m_isRecording) return false;

    int ret = 0;
    // 固定分辨率（录制中不修改）
    m_encodeWidth = 640;
    m_encodeHeight = 480;
    // qDebug("编码器分辨率：%dx%d，输出文件：%s", m_encodeWidth, m_encodeHeight, filename.toUtf8().constData());
    // 创建输出格式上下文
    ret = avformat_alloc_output_context2(&m_outputCtx, nullptr, nullptr, filename.toUtf8().constData());
    if (ret < 0) {
        char errbuf[AV_ERROR_MAX_STRING_SIZE];
        av_strerror(ret, errbuf, sizeof(errbuf));
        qWarning("无法创建输出上下文: %s", errbuf);
        return false;
    }

    // 查找 H.264 编码器
    const AVCodec* encoder = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!encoder) {
        qWarning("找不到 H.264 编码器");
        avformat_free_context(m_outputCtx);
        m_outputCtx = nullptr;
        return false;
    }

    // 创建视频流
    m_videoStream = avformat_new_stream(m_outputCtx, encoder);
    if (!m_videoStream) {
        qWarning("无法创建视频流");
        avformat_free_context(m_outputCtx);
        m_outputCtx = nullptr;
        return false;
    }

    // 初始化解码器上下文
    m_encoderCtx = avcodec_alloc_context3(encoder);
    if (!m_encoderCtx) {
        qWarning("无法分配编码器上下文");
        avformat_free_context(m_outputCtx);
        m_outputCtx = nullptr;
        return false;
    }

    // 编码器参数
    m_encoderCtx->codec_id = encoder->id;
    m_encoderCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    m_encoderCtx->width = m_encodeWidth;
    m_encoderCtx->height = m_encodeHeight;
    m_encoderCtx->pix_fmt = AV_PIX_FMT_NV12;
    m_encoderCtx->time_base = {1, 30};
    m_encoderCtx->framerate = {30, 1};
    m_encoderCtx->bit_rate = 4000000;
    m_encoderCtx->gop_size = 60;
    m_encoderCtx->max_b_frames = 1;
    m_encoderCtx->qmin = 10;
    m_encoderCtx->qmax = 51;

    // 全局头设置
    if (m_outputCtx->oformat->flags & AVFMT_GLOBALHEADER) {
        m_encoderCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    // 打开编码器
    AVDictionary* enc_opts = nullptr;
    av_dict_set(&enc_opts, "pix_fmt", "nv12", 0);
    av_dict_set(&enc_opts, "preset", "ultrafast", 0);  // 嵌入式优先，牺牲画质换速度
    av_dict_set(&enc_opts, "tune", "zerolatency", 0);  // 低延迟，适合实时录制
    ret = avcodec_open2(m_encoderCtx, encoder, &enc_opts);
    av_dict_free(&enc_opts);
    if (ret < 0) {
        char errbuf[AV_ERROR_MAX_STRING_SIZE];
        av_strerror(ret, errbuf, sizeof(errbuf));
        qWarning("无法打开编码器: %s", errbuf);
        avcodec_free_context(&m_encoderCtx);
        avformat_free_context(m_outputCtx);
        return false;
    }

    // 复制编码器参数到流
    ret = avcodec_parameters_from_context(m_videoStream->codecpar, m_encoderCtx);
    if (ret < 0) {
        char errbuf[AV_ERROR_MAX_STRING_SIZE];
        av_strerror(ret, errbuf, sizeof(errbuf));
        qWarning("无法复制编码器参数: %s", errbuf);
        avcodec_close(m_encoderCtx);
        avcodec_free_context(&m_encoderCtx);
        avformat_free_context(m_outputCtx);
        return false;
    }

    // 打开输出文件
    if (!(m_outputCtx->oformat->flags & AVFMT_NOFILE)) {
        ret = avio_open(&m_outputCtx->pb, filename.toUtf8().constData(), AVIO_FLAG_WRITE);
        if (ret < 0) {
            char errbuf[AV_ERROR_MAX_STRING_SIZE];
            av_strerror(ret, errbuf, sizeof(errbuf));
            qWarning("无法打开输出文件: %s", errbuf);
            avcodec_close(m_encoderCtx);
            avcodec_free_context(&m_encoderCtx);
            avformat_free_context(m_outputCtx);
            return false;
        }
    }

    // 写入文件头
    ret = avformat_write_header(m_outputCtx, nullptr);
    if (ret < 0) {
        char errbuf[AV_ERROR_MAX_STRING_SIZE];
        av_strerror(ret, errbuf, sizeof(errbuf));
        qWarning("无法写入文件头: %s", errbuf);
        avio_closep(&m_outputCtx->pb);
        avcodec_close(m_encoderCtx);
        avcodec_free_context(&m_encoderCtx);
        avformat_free_context(m_outputCtx);
        return false;
    }

    // 初始化复用帧和格式转换上下文
    m_reusableNv12Frame = av_frame_alloc();
    m_reusableNv12Frame->format = AV_PIX_FMT_NV12;
    m_reusableNv12Frame->width = m_encodeWidth;
    m_reusableNv12Frame->height = m_encodeHeight;
    ret = av_frame_get_buffer(m_reusableNv12Frame, 0);
    if (ret < 0) {
        char errbuf[AV_ERROR_MAX_STRING_SIZE];
        av_strerror(ret, errbuf, sizeof(errbuf));
        qWarning("无法分配复用帧: %s", errbuf);
        av_frame_free(&m_reusableNv12Frame);
        avio_closep(&m_outputCtx->pb);
        avcodec_close(m_encoderCtx);
        avcodec_free_context(&m_encoderCtx);
        avformat_free_context(m_outputCtx);
        return false;
    }

    m_swsCtx = sws_getContext(m_encodeWidth, m_encodeHeight, AV_PIX_FMT_RGB24, m_encodeWidth, m_encodeHeight,
                              AV_PIX_FMT_NV12, SWS_BILINEAR, nullptr, nullptr, nullptr);
    if (!m_swsCtx) {
        qWarning("无法初始化格式转换上下文");
        av_frame_free(&m_reusableNv12Frame);
        avio_closep(&m_outputCtx->pb);
        avcodec_close(m_encoderCtx);
        avcodec_free_context(&m_encoderCtx);
        avformat_free_context(m_outputCtx);
        return false;
    }

    m_isRecording = true;
    m_frameCount = 0;
    m_startTime = get_system_time_us();
    // qDebug("录制开始：%dx%d@30fps，输出文件：%s", m_encodeWidth, m_encodeHeight, filename.toUtf8().constData());
    // QMessageBox::information(this, "提示", "录像已开始，保存路径：\n");
    return true;
}

void VideoRecorder::stopRecording()
{
    QMutexLocker locker(&m_mutex);
    if (!m_isRecording) return;

    m_isRecording = false;
    int64_t duration = (get_system_time_us() - m_startTime) / 1000000;
    // qDebug("录制停止：时长 %lld 秒，编码帧数 %d", duration, m_frameCount);

    // 刷新编码器
    int ret = 0;
    if (m_encoderCtx && m_reusablePacket) {
        while (true) {
            ret = avcodec_send_frame(m_encoderCtx, nullptr);
            if (ret < 0) break;
            while (true) {
                ret = avcodec_receive_packet(m_encoderCtx, m_reusablePacket);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) break;
                if (ret < 0) {
                    char errbuf[AV_ERROR_MAX_STRING_SIZE];
                    av_strerror(ret, errbuf, sizeof(errbuf));
                    qWarning("刷新编码器失败: %s", errbuf);
                    break;
                }
                av_packet_rescale_ts(m_reusablePacket, m_encoderCtx->time_base, m_videoStream->time_base);
                m_reusablePacket->stream_index = m_videoStream->index;
                av_interleaved_write_frame(m_outputCtx, m_reusablePacket);
                av_packet_unref(m_reusablePacket);
            }
        }
    }

    // 写入文件尾
    if (m_outputCtx) {
        ret = av_write_trailer(m_outputCtx);
        if (ret < 0) {
            char errbuf[AV_ERROR_MAX_STRING_SIZE];
            av_strerror(ret, errbuf, sizeof(errbuf));
            qWarning("无法写入文件尾: %s", errbuf);
        }
    }

    // 释放资源（严格顺序）
    if (m_outputCtx && !(m_outputCtx->oformat->flags & AVFMT_NOFILE)) {
        avio_closep(&m_outputCtx->pb);
    }
    if (m_encoderCtx) {
        avcodec_close(m_encoderCtx);
        avcodec_free_context(&m_encoderCtx);
    }
    if (m_outputCtx) {
        avformat_free_context(m_outputCtx);
    }
    if (m_reusableNv12Frame) {
        av_frame_free(&m_reusableNv12Frame);
        m_reusableNv12Frame = nullptr;
    }

    // 重置变量
    m_outputCtx = nullptr;
    m_encoderCtx = nullptr;
    m_videoStream = nullptr;
    m_frameCount = 0;
    // QMessageBox::information(nullptr, "提示", "录像已停止，文件已保存！");
    // show_message();
}

void VideoRecorder::onFrameReceived(const QImage& frame)
{
    QMutexLocker locker(&m_mutex);
    if (!m_isRecording || !m_encoderCtx || !m_swsCtx || !m_reusableNv12Frame || !m_reusablePacket || frame.isNull()) {
        // qWarning("跳过无效帧：录制未启动或资源未初始化");
        return;
    }

    // 打印帧信息（定位分辨率问题）
    // qDebug("收到帧：%dx%d，编码器分辨率：%dx%d", frame.width(), frame.height(), m_encodeWidth, m_encodeHeight);

    // 确保帧格式为 RGB888
    QImage img = frame.convertToFormat(QImage::Format_RGB888);
    if (img.isNull()) {
        // qWarning("帧格式转换失败，跳过");
        return;
    }

    // RGB→NV12 转换
    uint8_t* srcData[1] = {(uint8_t*)img.bits()};
    int srcLinesize[1] = {img.bytesPerLine()};
    sws_scale(m_swsCtx, srcData, srcLinesize, 0, m_encodeHeight, m_reusableNv12Frame->data,
              m_reusableNv12Frame->linesize);

    // 设置时间戳
    m_reusableNv12Frame->pts = m_frameCount++;

    // 发送帧到编码器（错误处理）
    int ret = avcodec_send_frame(m_encoderCtx, m_reusableNv12Frame);
    // qDebug("avcodec_send_frame 返回：%d", ret);
    if (ret == AVERROR(EAGAIN)) {
        // 编码器忙，读取已编码包
        while (true) {
            ret = avcodec_receive_packet(m_encoderCtx, m_reusablePacket);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) break;
            if (ret < 0) {
                char errbuf[AV_ERROR_MAX_STRING_SIZE];
                av_strerror(ret, errbuf, sizeof(errbuf));
                qCritical("接收编码包失败：%s，停止录制", errbuf);
                stopRecording();
                return;
            }
            // 写入文件
            av_packet_rescale_ts(m_reusablePacket, m_encoderCtx->time_base, m_videoStream->time_base);
            m_reusablePacket->stream_index = m_videoStream->index;
            ret = av_interleaved_write_frame(m_outputCtx, m_reusablePacket);
            if (ret < 0) {
                char errbuf[AV_ERROR_MAX_STRING_SIZE];
                av_strerror(ret, errbuf, sizeof(errbuf));
                qCritical("写入帧失败：%s，停止录制", errbuf);
                av_packet_unref(m_reusablePacket);
                stopRecording();
                return;
            }
            // qDebug("成功写入 1 帧，累计帧数：%d", m_frameCount);
            av_packet_unref(m_reusablePacket);
        }
        // 重试发送帧
        ret = avcodec_send_frame(m_encoderCtx, m_reusableNv12Frame);
        if (ret < 0) {
            char errbuf[AV_ERROR_MAX_STRING_SIZE];
            av_strerror(ret, errbuf, sizeof(errbuf));
            qCritical("重试发送帧失败：%s，停止录制", errbuf);
            stopRecording();
            return;
        }
    } else if (ret < 0) {
        char errbuf[AV_ERROR_MAX_STRING_SIZE];
        av_strerror(ret, errbuf, sizeof(errbuf));
        qCritical("帧发送失败：%s，停止录制", errbuf);
        stopRecording();
        return;
    }

    // 读取并写入编码包
    while (true) {
        ret = avcodec_receive_packet(m_encoderCtx, m_reusablePacket);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) break;
        if (ret < 0) {
            char errbuf[AV_ERROR_MAX_STRING_SIZE];
            av_strerror(ret, errbuf, sizeof(errbuf));
            qCritical("接收包失败：%s，停止录制", errbuf);
            stopRecording();
            break;
        }
        av_packet_rescale_ts(m_reusablePacket, m_encoderCtx->time_base, m_videoStream->time_base);
        m_reusablePacket->stream_index = m_videoStream->index;
        ret = av_interleaved_write_frame(m_outputCtx, m_reusablePacket);
        if (ret < 0) {
            char errbuf[AV_ERROR_MAX_STRING_SIZE];
            av_strerror(ret, errbuf, sizeof(errbuf));
            qCritical("写入包失败：%s，停止录制", errbuf);
            av_packet_unref(m_reusablePacket);
            stopRecording();
            break;
        }
        // qDebug("成功写入 1 帧，累计帧数：%d", m_frameCount);
        av_packet_unref(m_reusablePacket);
    }
}
