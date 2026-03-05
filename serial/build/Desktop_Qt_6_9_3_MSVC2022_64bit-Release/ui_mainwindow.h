/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPlainTextEdit *recvEdit;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QComboBox *serailCb;
    QLabel *label_3;
    QComboBox *baundrateCb;
    QLabel *label_4;
    QComboBox *databitCb;
    QLabel *label_5;
    QComboBox *stopbitCb;
    QLabel *label_6;
    QComboBox *checkbitCb;
    QPushButton *btnSerialCheck;
    QPushButton *openBt;
    QGroupBox *groupBox_2;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_2;
    QPushButton *clearBt;
    QCheckBox *chkRevHex;
    QCheckBox *chkRevTime;
    QCheckBox *chkRevLine;
    QGroupBox *groupBox_4;
    QWidget *layoutWidget2;
    QGridLayout *gridLayout_3;
    QCheckBox *chkSendHex;
    QCheckBox *chkSendLine;
    QCheckBox *chkTimSend;
    QSpinBox *txtSendMs;
    QLabel *label_9;
    QWidget *layoutWidget3;
    QGridLayout *gridLayout_4;
    QVBoxLayout *verticalLayout;
    QPushButton *sendBt;
    QPushButton *btnClearSend;
    QTextEdit *sendEdit;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(744, 576);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        recvEdit = new QPlainTextEdit(centralwidget);
        recvEdit->setObjectName("recvEdit");
        recvEdit->setGeometry(QRect(200, 20, 511, 431));
        recvEdit->setReadOnly(true);
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(20, 40, 161, 199));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        serailCb = new QComboBox(layoutWidget);
        serailCb->setObjectName("serailCb");

        gridLayout->addWidget(serailCb, 0, 1, 1, 1);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        baundrateCb = new QComboBox(layoutWidget);
        baundrateCb->addItem(QString());
        baundrateCb->addItem(QString());
        baundrateCb->addItem(QString());
        baundrateCb->addItem(QString());
        baundrateCb->addItem(QString());
        baundrateCb->addItem(QString());
        baundrateCb->addItem(QString());
        baundrateCb->addItem(QString());
        baundrateCb->addItem(QString());
        baundrateCb->addItem(QString());
        baundrateCb->setObjectName("baundrateCb");
        baundrateCb->setMaxCount(2147483647);

        gridLayout->addWidget(baundrateCb, 1, 1, 1, 1);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName("label_4");

        gridLayout->addWidget(label_4, 2, 0, 1, 1);

        databitCb = new QComboBox(layoutWidget);
        databitCb->addItem(QString());
        databitCb->addItem(QString());
        databitCb->addItem(QString());
        databitCb->addItem(QString());
        databitCb->setObjectName("databitCb");

        gridLayout->addWidget(databitCb, 2, 1, 1, 1);

        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName("label_5");

        gridLayout->addWidget(label_5, 3, 0, 1, 1);

        stopbitCb = new QComboBox(layoutWidget);
        stopbitCb->addItem(QString());
        stopbitCb->addItem(QString());
        stopbitCb->addItem(QString());
        stopbitCb->setObjectName("stopbitCb");

        gridLayout->addWidget(stopbitCb, 3, 1, 1, 1);

        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName("label_6");

        gridLayout->addWidget(label_6, 4, 0, 1, 1);

        checkbitCb = new QComboBox(layoutWidget);
        checkbitCb->addItem(QString());
        checkbitCb->addItem(QString());
        checkbitCb->addItem(QString());
        checkbitCb->setObjectName("checkbitCb");

        gridLayout->addWidget(checkbitCb, 4, 1, 1, 1);

        btnSerialCheck = new QPushButton(layoutWidget);
        btnSerialCheck->setObjectName("btnSerialCheck");

        gridLayout->addWidget(btnSerialCheck, 5, 0, 1, 2);

        openBt = new QPushButton(layoutWidget);
        openBt->setObjectName("openBt");

        gridLayout->addWidget(openBt, 6, 0, 1, 2);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(10, 258, 181, 91));
        layoutWidget1 = new QWidget(groupBox_2);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(10, 30, 160, 51));
        gridLayout_2 = new QGridLayout(layoutWidget1);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        clearBt = new QPushButton(layoutWidget1);
        clearBt->setObjectName("clearBt");

        gridLayout_2->addWidget(clearBt, 0, 0, 1, 1);

        chkRevHex = new QCheckBox(layoutWidget1);
        chkRevHex->setObjectName("chkRevHex");

        gridLayout_2->addWidget(chkRevHex, 0, 1, 1, 1);

        chkRevTime = new QCheckBox(layoutWidget1);
        chkRevTime->setObjectName("chkRevTime");

        gridLayout_2->addWidget(chkRevTime, 1, 0, 1, 1);

        chkRevLine = new QCheckBox(layoutWidget1);
        chkRevLine->setObjectName("chkRevLine");

        gridLayout_2->addWidget(chkRevLine, 1, 1, 1, 1);

        groupBox_4 = new QGroupBox(centralwidget);
        groupBox_4->setObjectName("groupBox_4");
        groupBox_4->setGeometry(QRect(10, 360, 181, 91));
        layoutWidget2 = new QWidget(groupBox_4);
        layoutWidget2->setObjectName("layoutWidget2");
        layoutWidget2->setGeometry(QRect(10, 30, 161, 52));
        gridLayout_3 = new QGridLayout(layoutWidget2);
        gridLayout_3->setObjectName("gridLayout_3");
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        chkSendHex = new QCheckBox(layoutWidget2);
        chkSendHex->setObjectName("chkSendHex");

        gridLayout_3->addWidget(chkSendHex, 0, 0, 1, 1);

        chkSendLine = new QCheckBox(layoutWidget2);
        chkSendLine->setObjectName("chkSendLine");

        gridLayout_3->addWidget(chkSendLine, 0, 1, 1, 2);

        chkTimSend = new QCheckBox(layoutWidget2);
        chkTimSend->setObjectName("chkTimSend");

        gridLayout_3->addWidget(chkTimSend, 1, 0, 1, 1);

        txtSendMs = new QSpinBox(layoutWidget2);
        txtSendMs->setObjectName("txtSendMs");

        gridLayout_3->addWidget(txtSendMs, 1, 1, 1, 1);

        label_9 = new QLabel(layoutWidget2);
        label_9->setObjectName("label_9");

        gridLayout_3->addWidget(label_9, 1, 2, 1, 1);

        layoutWidget3 = new QWidget(centralwidget);
        layoutWidget3->setObjectName("layoutWidget3");
        layoutWidget3->setGeometry(QRect(110, 460, 601, 91));
        gridLayout_4 = new QGridLayout(layoutWidget3);
        gridLayout_4->setObjectName("gridLayout_4");
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        sendBt = new QPushButton(layoutWidget3);
        sendBt->setObjectName("sendBt");

        verticalLayout->addWidget(sendBt);

        btnClearSend = new QPushButton(layoutWidget3);
        btnClearSend->setObjectName("btnClearSend");

        verticalLayout->addWidget(btnClearSend);


        gridLayout_4->addLayout(verticalLayout, 0, 0, 1, 1);

        sendEdit = new QTextEdit(layoutWidget3);
        sendEdit->setObjectName("sendEdit");

        gridLayout_4->addWidget(sendEdit, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        groupBox_4->raise();
        groupBox_2->raise();
        layoutWidget->raise();
        layoutWidget->raise();
        recvEdit->raise();
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        baundrateCb->setCurrentIndex(0);
        databitCb->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\344\270\262\345\217\243\345\267\245\345\205\267", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\347\253\257\345\217\243", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\346\263\242\347\211\271\347\216\207", nullptr));
        baundrateCb->setItemText(0, QCoreApplication::translate("MainWindow", "1200", nullptr));
        baundrateCb->setItemText(1, QCoreApplication::translate("MainWindow", "2400", nullptr));
        baundrateCb->setItemText(2, QCoreApplication::translate("MainWindow", "4800", nullptr));
        baundrateCb->setItemText(3, QCoreApplication::translate("MainWindow", "9600", nullptr));
        baundrateCb->setItemText(4, QCoreApplication::translate("MainWindow", "19200", nullptr));
        baundrateCb->setItemText(5, QCoreApplication::translate("MainWindow", "38400", nullptr));
        baundrateCb->setItemText(6, QCoreApplication::translate("MainWindow", "57600", nullptr));
        baundrateCb->setItemText(7, QCoreApplication::translate("MainWindow", "115200", nullptr));
        baundrateCb->setItemText(8, QCoreApplication::translate("MainWindow", "921600", nullptr));
        baundrateCb->setItemText(9, QCoreApplication::translate("MainWindow", "1152000", nullptr));

        label_4->setText(QCoreApplication::translate("MainWindow", "\346\225\260\346\215\256\344\275\215", nullptr));
        databitCb->setItemText(0, QCoreApplication::translate("MainWindow", "5", nullptr));
        databitCb->setItemText(1, QCoreApplication::translate("MainWindow", "6", nullptr));
        databitCb->setItemText(2, QCoreApplication::translate("MainWindow", "7", nullptr));
        databitCb->setItemText(3, QCoreApplication::translate("MainWindow", "8", nullptr));

        label_5->setText(QCoreApplication::translate("MainWindow", "\345\201\234\346\255\242\344\275\215", nullptr));
        stopbitCb->setItemText(0, QCoreApplication::translate("MainWindow", "1", nullptr));
        stopbitCb->setItemText(1, QCoreApplication::translate("MainWindow", "1.5", nullptr));
        stopbitCb->setItemText(2, QCoreApplication::translate("MainWindow", "2", nullptr));

        label_6->setText(QCoreApplication::translate("MainWindow", "\346\240\241\351\252\214\344\275\215", nullptr));
        checkbitCb->setItemText(0, QCoreApplication::translate("MainWindow", "none", nullptr));
        checkbitCb->setItemText(1, QCoreApplication::translate("MainWindow", "\345\257\204\346\240\241\351\252\214", nullptr));
        checkbitCb->setItemText(2, QCoreApplication::translate("MainWindow", "\345\201\266\346\240\241\351\252\214", nullptr));

        btnSerialCheck->setText(QCoreApplication::translate("MainWindow", "\346\243\200\346\265\213\344\270\262\345\217\243", nullptr));
        openBt->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\344\270\262\345\217\243", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "\346\216\245\346\224\266\350\256\276\347\275\256", nullptr));
        clearBt->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\346\216\245\346\224\266", nullptr));
        chkRevHex->setText(QCoreApplication::translate("MainWindow", "Hex\346\216\245\346\224\266", nullptr));
        chkRevTime->setText(QCoreApplication::translate("MainWindow", "\346\227\266\351\227\264\346\210\263", nullptr));
        chkRevLine->setText(QCoreApplication::translate("MainWindow", "\350\207\252\345\212\250\346\215\242\350\241\214", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201\350\256\276\347\275\256", nullptr));
        chkSendHex->setText(QCoreApplication::translate("MainWindow", "Hex\345\217\221\351\200\201", nullptr));
        chkSendLine->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201\346\226\260\350\241\214", nullptr));
        chkTimSend->setText(QCoreApplication::translate("MainWindow", "\350\207\252\345\212\250\345\217\221\351\200\201", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "ms", nullptr));
        sendBt->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        btnClearSend->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
