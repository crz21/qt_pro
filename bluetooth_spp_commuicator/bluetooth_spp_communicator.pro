QT       += core gui bluetooth
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BLE_Upper_Computer
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# 确保蓝牙模块链接
# LIBS += -lQt6Bluetooth

# 目标路径
DESTDIR = ./bin
MOC_DIR = ./build/moc
OBJECTS_DIR = ./build/obj
UI_DIR = ./build/ui
RCC_DIR = ./build/rcc

# 平台特定设置
# unix {
#     target.path = /usr/bin
#     INSTALLS += target
# }

# win32 {
#     # Windows 特定设置
# }
