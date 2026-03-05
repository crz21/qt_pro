#include "video_previewer.h"

VideoPreviewer::VideoPreviewer(QObject* parent)
    : QThread(parent),
      m_isPreviewing(false),
      m_formatCtx(nullptr),
      m_codecCtx(nullptr),
      m_videoStreamIndex(-1),
      m_swsCtx(nullptr)
{
    avdevice_register_all();
}

VideoPreviewer::~VideoPreviewer() { stopPreview(); }

bool VideoPreviewer::startPreview(const QString& device)
{
    QMutexLocker locker(&m_mutex);
    if (m_isPreviewing) return false;

    // 打开视频设备
    const AVInputFormat* inputFormat = av_find_input_format("v4l2");
    if (!inputFormat) {
        qWarning("找不到v4l2输入格式");
        return false;
    }

    /** device == /dev/video11 */
    int ret = avformat_open_input(&m_formatCtx, device.toUtf8().constData(), inputFormat, nullptr);
    if (ret != 0) {
        char errbuf[AV_ERROR_MAX_STRING_SIZE];
        av_strerror(ret, errbuf, sizeof(errbuf));
        qWarning("无法打开视频设备: %s", errbuf);
        return false;
    }

    // 查找流信息
    if (avformat_find_stream_info(m_formatCtx, nullptr) < 0) {
        qWarning("无法找到流信息");
        avformat_close_input(&m_formatCtx);
        return false;
    }

    // 查找视频流
    for (unsigned int i = 0; i < m_formatCtx->nb_streams; i++) {
        if (m_formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            m_videoStreamIndex = i;
            break;
        }
    }

    if (m_videoStreamIndex == -1) {
        qWarning("找不到视频流");
        avformat_close_input(&m_formatCtx);
        return false;
    }

    // 获取解码器
    AVCodecParameters* codecPar = m_formatCtx->streams[m_videoStreamIndex]->codecpar;
    const AVCodec* codec = avcodec_find_decoder(codecPar->codec_id);
    if (!codec) {
        qWarning("找不到解码器");
        avformat_close_input(&m_formatCtx);
        return false;
    }

    // 初始化解码器上下文
    m_codecCtx = avcodec_alloc_context3(codec);
    if (!m_codecCtx) {
        qWarning("无法分配解码器上下文");
        avformat_close_input(&m_formatCtx);
        return false;
    }

    if (avcodec_parameters_to_context(m_codecCtx, codecPar) < 0) {
        qWarning("无法将参数复制到解码器上下文");
        avcodec_free_context(&m_codecCtx);
        avformat_close_input(&m_formatCtx);
        return false;
    }

    if (avcodec_open2(m_codecCtx, codec, nullptr) < 0) {
        qWarning("无法打开解码器");
        avcodec_free_context(&m_codecCtx);
        avformat_close_input(&m_formatCtx);
        return false;
    }

    m_isPreviewing = true;
    start();
    return true;
}

void VideoPreviewer::stopPreview()
{
    QMutexLocker locker(&m_mutex);
    if (!m_isPreviewing) return;

    m_isPreviewing = false;
    wait();

    if (m_swsCtx) {
        sws_freeContext(m_swsCtx);
        m_swsCtx = nullptr;
    }

    if (m_codecCtx) {
        avcodec_close(m_codecCtx);
        avcodec_free_context(&m_codecCtx);
        m_codecCtx = nullptr;
    }

    if (m_formatCtx) {
        avformat_close_input(&m_formatCtx);
        m_formatCtx = nullptr;
    }

    m_videoStreamIndex = -1;
}

void VideoPreviewer::run()
{
    AVPacket* packet = av_packet_alloc();
    AVFrame* frame = av_frame_alloc();
    AVFrame* rgbFrame = av_frame_alloc();

    // 1. 根据设备实际输出格式和目标格式（RGB24）配置
    int dstWidth = 640;  // m_codecCtx->width;
    int dstHeight = 480;  // m_codecCtx->height;
    AVPixelFormat dstPixFmt = AV_PIX_FMT_RGB24;

    // 2. 为目标帧分配内存（关键：确保内存有效）
    rgbFrame->format = dstPixFmt;
    rgbFrame->width = dstWidth;
    rgbFrame->height = dstHeight;
    if (av_frame_get_buffer(rgbFrame, 0) < 0) {  // 分配缓冲区
        qWarning("无法为目标帧分配内存");
        return;
    }

    // 3. 初始化格式转换上下文（源格式需与设备输出格式一致，如 NV12）
    // m_swsCtx = sws_getContext(m_codecCtx->width, m_codecCtx->height, m_codecCtx->pix_fmt,  // 源宽高、格式（如 NV12）
    //                           dstWidth, dstHeight, dstPixFmt,                              // 目标宽高、格式（RGB24）
    //                           SWS_BILINEAR, nullptr, nullptr, nullptr);

    m_swsCtx = sws_getContext(dstWidth, dstHeight, m_codecCtx->pix_fmt,  // 源宽高、格式（如 NV12）
                              dstWidth, dstHeight, dstPixFmt,            // 目标宽高、格式（RGB24）
                              SWS_BILINEAR, nullptr, nullptr, nullptr);

    while (m_isPreviewing) {
        if (av_read_frame(m_formatCtx, packet) < 0) {
            msleep(10);
            continue;
        }

        if (packet->stream_index == m_videoStreamIndex) {
            // 解码
            if (avcodec_send_packet(m_codecCtx, packet) >= 0) {
                while (avcodec_receive_frame(m_codecCtx, frame) == 0) {
                    // // 转换为RGB格式
                    // sws_scale(m_swsCtx, (uint8_t const* const*)frame->data, frame->linesize, 0, m_codecCtx->height,
                    //           rgbFrame->data, rgbFrame->linesize);

                    // // 转换为QImage并发送信号
                    // QImage image(rgbFrame->data[0], m_codecCtx->width, m_codecCtx->height, rgbFrame->linesize[0],
                    //              QImage::Format_RGB888);
                    // 转换为RGB格式
                    sws_scale(m_swsCtx, (uint8_t const* const*)frame->data, frame->linesize, 0, dstHeight,
                              rgbFrame->data, rgbFrame->linesize);

                    // 转换为QImage并发送信号
                    QImage image(rgbFrame->data[0], dstWidth, dstHeight, rgbFrame->linesize[0], QImage::Format_RGB888);
                    emit frameReady(image.copy());  // 发送副本避免线程问题
                }
            }
        }

        av_packet_unref(packet);
    }

    // 释放资源
    av_frame_free(&rgbFrame);
    av_frame_free(&frame);
    av_packet_free(&packet);
}
