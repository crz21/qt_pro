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
#include <QtWidgets/QHBoxLayout>
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
    QWidget *widget;
    QVBoxLayout *verticalLayout_9;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_8;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_7;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QVBoxLayout *verticalLayout_2;
    QComboBox *serailCb;
    QComboBox *baundrateCb;
    QComboBox *databitCb;
    QComboBox *stopbitCb;
    QComboBox *checkbitCb;
    QVBoxLayout *verticalLayout_4;
    QPushButton *btnSerialCheck;
    QPushButton *openBt;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_3;
    QGridLayout *gridLayout_2;
    QPushButton *clearBt;
    QCheckBox *chkRevHex;
    QCheckBox *chkRevTime;
    QCheckBox *chkRevLine;
    QGroupBox *groupBox_4;
    QHBoxLayout *horizontalLayout_2;
    QGridLayout *gridLayout_3;
    QCheckBox *chkSendHex;
    QCheckBox *chkSendLine;
    QCheckBox *chkTimSend;
    QSpinBox *txtSendMs;
    QLabel *label_9;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_4;
    QPlainTextEdit *recvEdit;
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
        MainWindow->resize(780, 557);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(10, 10, 761, 521));
        verticalLayout_9 = new QVBoxLayout(widget);
        verticalLayout_9->setObjectName("verticalLayout_9");
        verticalLayout_9->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName("verticalLayout_8");
        groupBox = new QGroupBox(widget);
        groupBox->setObjectName("groupBox");
        verticalLayout_7 = new QVBoxLayout(groupBox);
        verticalLayout_7->setObjectName("verticalLayout_7");
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName("verticalLayout_5");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");

        verticalLayout_3->addWidget(label_2);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName("label_3");

        verticalLayout_3->addWidget(label_3);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName("label_4");

        verticalLayout_3->addWidget(label_4);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName("label_5");

        verticalLayout_3->addWidget(label_5);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName("label_6");

        verticalLayout_3->addWidget(label_6);


        horizontalLayout->addLayout(verticalLayout_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        serailCb = new QComboBox(groupBox);
        serailCb->setObjectName("serailCb");

        verticalLayout_2->addWidget(serailCb);

        baundrateCb = new QComboBox(groupBox);
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
        baundrateCb->addItem(QString());
        baundrateCb->setObjectName("baundrateCb");
        baundrateCb->setMaxVisibleItems(11);
        baundrateCb->setMaxCount(2147483647);

        verticalLayout_2->addWidget(baundrateCb);

        databitCb = new QComboBox(groupBox);
        databitCb->addItem(QString());
        databitCb->addItem(QString());
        databitCb->addItem(QString());
        databitCb->addItem(QString());
        databitCb->setObjectName("databitCb");

        verticalLayout_2->addWidget(databitCb);

        stopbitCb = new QComboBox(groupBox);
        stopbitCb->addItem(QString());
        stopbitCb->addItem(QString());
        stopbitCb->addItem(QString());
        stopbitCb->setObjectName("stopbitCb");

        verticalLayout_2->addWidget(stopbitCb);

        checkbitCb = new QComboBox(groupBox);
        checkbitCb->addItem(QString());
        checkbitCb->addItem(QString());
        checkbitCb->addItem(QString());
        checkbitCb->setObjectName("checkbitCb");

        verticalLayout_2->addWidget(checkbitCb);


        horizontalLayout->addLayout(verticalLayout_2);


        verticalLayout_5->addLayout(horizontalLayout);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName("verticalLayout_4");
        btnSerialCheck = new QPushButton(groupBox);
        btnSerialCheck->setObjectName("btnSerialCheck");

        verticalLayout_4->addWidget(btnSerialCheck);

        openBt = new QPushButton(groupBox);
        openBt->setObjectName("openBt");

        verticalLayout_4->addWidget(openBt);


        verticalLayout_5->addLayout(verticalLayout_4);


        verticalLayout_7->addLayout(verticalLayout_5);


        verticalLayout_8->addWidget(groupBox);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName("verticalLayout_6");
        groupBox_2 = new QGroupBox(widget);
        groupBox_2->setObjectName("groupBox_2");
        horizontalLayout_3 = new QHBoxLayout(groupBox_2);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName("gridLayout_2");
        clearBt = new QPushButton(groupBox_2);
        clearBt->setObjectName("clearBt");

        gridLayout_2->addWidget(clearBt, 0, 0, 1, 1);

        chkRevHex = new QCheckBox(groupBox_2);
        chkRevHex->setObjectName("chkRevHex");

        gridLayout_2->addWidget(chkRevHex, 0, 1, 1, 1);

        chkRevTime = new QCheckBox(groupBox_2);
        chkRevTime->setObjectName("chkRevTime");

        gridLayout_2->addWidget(chkRevTime, 1, 0, 1, 1);

        chkRevLine = new QCheckBox(groupBox_2);
        chkRevLine->setObjectName("chkRevLine");

        gridLayout_2->addWidget(chkRevLine, 1, 1, 1, 1);


        horizontalLayout_3->addLayout(gridLayout_2);


        verticalLayout_6->addWidget(groupBox_2);

        groupBox_4 = new QGroupBox(widget);
        groupBox_4->setObjectName("groupBox_4");
        horizontalLayout_2 = new QHBoxLayout(groupBox_4);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName("gridLayout_3");
        chkSendHex = new QCheckBox(groupBox_4);
        chkSendHex->setObjectName("chkSendHex");

        gridLayout_3->addWidget(chkSendHex, 0, 0, 1, 1);

        chkSendLine = new QCheckBox(groupBox_4);
        chkSendLine->setObjectName("chkSendLine");

        gridLayout_3->addWidget(chkSendLine, 0, 1, 1, 2);

        chkTimSend = new QCheckBox(groupBox_4);
        chkTimSend->setObjectName("chkTimSend");

        gridLayout_3->addWidget(chkTimSend, 1, 0, 1, 1);

        txtSendMs = new QSpinBox(groupBox_4);
        txtSendMs->setObjectName("txtSendMs");

        gridLayout_3->addWidget(txtSendMs, 1, 1, 1, 1);

        label_9 = new QLabel(groupBox_4);
        label_9->setObjectName("label_9");

        gridLayout_3->addWidget(label_9, 1, 2, 1, 1);


        horizontalLayout_2->addLayout(gridLayout_3);


        verticalLayout_6->addWidget(groupBox_4);


        verticalLayout_8->addLayout(verticalLayout_6);


        horizontalLayout_5->addLayout(verticalLayout_8);

        groupBox_3 = new QGroupBox(widget);
        groupBox_3->setObjectName("groupBox_3");
        horizontalLayout_4 = new QHBoxLayout(groupBox_3);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        recvEdit = new QPlainTextEdit(groupBox_3);
        recvEdit->setObjectName("recvEdit");
        recvEdit->setReadOnly(true);

        horizontalLayout_4->addWidget(recvEdit);


        horizontalLayout_5->addWidget(groupBox_3);


        verticalLayout_9->addLayout(horizontalLayout_5);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName("gridLayout_4");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        sendBt = new QPushButton(widget);
        sendBt->setObjectName("sendBt");

        verticalLayout->addWidget(sendBt);

        btnClearSend = new QPushButton(widget);
        btnClearSend->setObjectName("btnClearSend");

        verticalLayout->addWidget(btnClearSend);


        gridLayout_4->addLayout(verticalLayout, 0, 0, 1, 1);

        sendEdit = new QTextEdit(widget);
        sendEdit->setObjectName("sendEdit");

        gridLayout_4->addWidget(sendEdit, 0, 1, 1, 1);


        verticalLayout_9->addLayout(gridLayout_4);

        MainWindow->setCentralWidget(centralwidget);
        groupBox->raise();
        groupBox_4->raise();
        groupBox_2->raise();
        groupBox_3->raise();
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
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\345\217\202\346\225\260\350\256\276\347\275\256", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\347\253\257\345\217\243", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\346\263\242\347\211\271\347\216\207", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\346\225\260\346\215\256\344\275\215", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\345\201\234\346\255\242\344\275\215", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\346\240\241\351\252\214\344\275\215", nullptr));
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
        baundrateCb->setItemText(10, QCoreApplication::translate("MainWindow", "2000000", nullptr));

        databitCb->setItemText(0, QCoreApplication::translate("MainWindow", "5", nullptr));
        databitCb->setItemText(1, QCoreApplication::translate("MainWindow", "6", nullptr));
        databitCb->setItemText(2, QCoreApplication::translate("MainWindow", "7", nullptr));
        databitCb->setItemText(3, QCoreApplication::translate("MainWindow", "8", nullptr));

        stopbitCb->setItemText(0, QCoreApplication::translate("MainWindow", "1", nullptr));
        stopbitCb->setItemText(1, QCoreApplication::translate("MainWindow", "1.5", nullptr));
        stopbitCb->setItemText(2, QCoreApplication::translate("MainWindow", "2", nullptr));

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
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "\346\211\223\345\215\260\344\277\241\346\201\257", nullptr));
        sendBt->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        btnClearSend->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
