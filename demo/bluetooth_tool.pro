QT       += core gui bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BluetoothTool
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# 确保Windows平台下能正确链接蓝牙库
win32 {
    LIBS += -lBthprops
}

# 支持C++11及以上标准
CONFIG += c++11

# 应用程序图标（可选）
# RC_ICONS = app_icon.ico
