QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FFmpeg
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    video_previewer.cpp \
    video_recorder.cpp

HEADERS += \
    mainwindow.h \
    video_previewer.h \
    video_recorder.h

# FFmpeg配置
FFMPEG_DIR = /home/tp_21/src/qt/FFmpeg_sample/ffmpeg
X264_DIR = /home/tp_21/src/qt/FFmpeg_sample/x264
INCLUDEPATH += $$FFMPEG_DIR/include \
               $$X264_DIR/include
LIBS += -L$$FFMPEG_DIR/lib \
        -lavformat \
        -lavcodec \
        -lavutil \
        -lswscale \
        -lavdevice \
        -L$$X264_DIR/lib \
        -lswresample \
        -lx264 \
        -lavfilter \
        -lpostproc



# 交叉编译配置
# unix {
#     target.path = /opt/Qt/Qt6.2.4/5.15.2/gcc_64/bin
#     INSTALLS += target
# }
