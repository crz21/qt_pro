#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QHBoxLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QDateTime>
//#include <QVideoWidget>
#include <QLabel>
#include <QStyle>
#include "video_previewer.h"
#include "video_recorder.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
   public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

   private:
    VideoPreviewer* m_previewer;
    PreviewWidget* m_previewWidget;
    VideoRecorder* m_recorder;
};

#endif
