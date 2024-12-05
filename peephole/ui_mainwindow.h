/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_9;
    QOpenGLWidget *openGLWidget;
    QMenuBar *menuBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QDockWidget *dockWidget_2;
    QWidget *dockWidgetContents_2;
    QVBoxLayout *verticalLayout_6;
    QListView *listView_note;
    QDockWidget *dockWidget_3;
    QWidget *dockWidgetContents_3;
    QVBoxLayout *verticalLayout_7;
    QGroupBox *groupBox_peephole_dev_com;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_peephole_com_port;
    QComboBox *comboBox_peephole_com_port;
    QHBoxLayout *horizontalLayout;
    QLabel *label_peephole_baud_rate;
    QComboBox *comboBox_peephole_baud_rate;
    QPushButton *pushButton_peephole_coms;
    QPushButton *pushButton_peephole_open;
    QGroupBox *groupBox_protocol;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *radioButton_protocol_3v;
    QRadioButton *radioButton_protocol_k1s;
    QRadioButton *radioButton_protocol_orbbec;
    QCheckBox *checkBox_msg_encrypt;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QTextEdit *textEdit_send_cmd;
    QPushButton *pushButton_send_cmd;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(871, 666);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_9 = new QVBoxLayout(centralWidget);
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setContentsMargins(11, 11, 11, 11);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        openGLWidget = new QOpenGLWidget(centralWidget);
        openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));

        verticalLayout_9->addWidget(openGLWidget);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 871, 23));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->setMinimumSize(QSize(350, 40));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(Qt::RightDockWidgetArea, dockWidget);
        dockWidget_2 = new QDockWidget(MainWindow);
        dockWidget_2->setObjectName(QString::fromUtf8("dockWidget_2"));
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        verticalLayout_6 = new QVBoxLayout(dockWidgetContents_2);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        listView_note = new QListView(dockWidgetContents_2);
        listView_note->setObjectName(QString::fromUtf8("listView_note"));

        verticalLayout_6->addWidget(listView_note);

        dockWidget_2->setWidget(dockWidgetContents_2);
        MainWindow->addDockWidget(Qt::BottomDockWidgetArea, dockWidget_2);
        dockWidget_3 = new QDockWidget(MainWindow);
        dockWidget_3->setObjectName(QString::fromUtf8("dockWidget_3"));
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QString::fromUtf8("dockWidgetContents_3"));
        verticalLayout_7 = new QVBoxLayout(dockWidgetContents_3);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        groupBox_peephole_dev_com = new QGroupBox(dockWidgetContents_3);
        groupBox_peephole_dev_com->setObjectName(QString::fromUtf8("groupBox_peephole_dev_com"));
        verticalLayout_8 = new QVBoxLayout(groupBox_peephole_dev_com);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_peephole_com_port = new QLabel(groupBox_peephole_dev_com);
        label_peephole_com_port->setObjectName(QString::fromUtf8("label_peephole_com_port"));

        horizontalLayout_2->addWidget(label_peephole_com_port);

        comboBox_peephole_com_port = new QComboBox(groupBox_peephole_dev_com);
        comboBox_peephole_com_port->setObjectName(QString::fromUtf8("comboBox_peephole_com_port"));

        horizontalLayout_2->addWidget(comboBox_peephole_com_port);


        verticalLayout_8->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_peephole_baud_rate = new QLabel(groupBox_peephole_dev_com);
        label_peephole_baud_rate->setObjectName(QString::fromUtf8("label_peephole_baud_rate"));

        horizontalLayout->addWidget(label_peephole_baud_rate);

        comboBox_peephole_baud_rate = new QComboBox(groupBox_peephole_dev_com);
        comboBox_peephole_baud_rate->setObjectName(QString::fromUtf8("comboBox_peephole_baud_rate"));

        horizontalLayout->addWidget(comboBox_peephole_baud_rate);


        verticalLayout_8->addLayout(horizontalLayout);

        pushButton_peephole_coms = new QPushButton(groupBox_peephole_dev_com);
        pushButton_peephole_coms->setObjectName(QString::fromUtf8("pushButton_peephole_coms"));

        verticalLayout_8->addWidget(pushButton_peephole_coms);

        pushButton_peephole_open = new QPushButton(groupBox_peephole_dev_com);
        pushButton_peephole_open->setObjectName(QString::fromUtf8("pushButton_peephole_open"));
        pushButton_peephole_open->setCheckable(false);
        pushButton_peephole_open->setAutoRepeat(false);

        verticalLayout_8->addWidget(pushButton_peephole_open);

        groupBox_protocol = new QGroupBox(groupBox_peephole_dev_com);
        groupBox_protocol->setObjectName(QString::fromUtf8("groupBox_protocol"));
        horizontalLayout_3 = new QHBoxLayout(groupBox_protocol);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 9);
        radioButton_protocol_3v = new QRadioButton(groupBox_protocol);
        radioButton_protocol_3v->setObjectName(QString::fromUtf8("radioButton_protocol_3v"));

        horizontalLayout_3->addWidget(radioButton_protocol_3v);

        radioButton_protocol_k1s = new QRadioButton(groupBox_protocol);
        radioButton_protocol_k1s->setObjectName(QString::fromUtf8("radioButton_protocol_k1s"));

        horizontalLayout_3->addWidget(radioButton_protocol_k1s);

        radioButton_protocol_orbbec = new QRadioButton(groupBox_protocol);
        radioButton_protocol_orbbec->setObjectName(QString::fromUtf8("radioButton_protocol_orbbec"));

        horizontalLayout_3->addWidget(radioButton_protocol_orbbec);


        verticalLayout_8->addWidget(groupBox_protocol);

        checkBox_msg_encrypt = new QCheckBox(groupBox_peephole_dev_com);
        checkBox_msg_encrypt->setObjectName(QString::fromUtf8("checkBox_msg_encrypt"));

        verticalLayout_8->addWidget(checkBox_msg_encrypt);

        groupBox = new QGroupBox(groupBox_peephole_dev_com);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(3);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(3, 3, 3, 3);
        textEdit_send_cmd = new QTextEdit(groupBox);
        textEdit_send_cmd->setObjectName(QString::fromUtf8("textEdit_send_cmd"));

        verticalLayout_2->addWidget(textEdit_send_cmd);

        pushButton_send_cmd = new QPushButton(groupBox);
        pushButton_send_cmd->setObjectName(QString::fromUtf8("pushButton_send_cmd"));

        verticalLayout_2->addWidget(pushButton_send_cmd);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);


        verticalLayout_8->addWidget(groupBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer);


        verticalLayout_7->addWidget(groupBox_peephole_dev_com);

        dockWidget_3->setWidget(dockWidgetContents_3);
        MainWindow->addDockWidget(Qt::LeftDockWidgetArea, dockWidget_3);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\347\214\253\347\234\274 v1.1.1", nullptr));
        dockWidget->setWindowTitle(QCoreApplication::translate("MainWindow", "\345\217\202\346\225\260\350\256\276\347\275\256", nullptr));
        dockWidget_2->setWindowTitle(QCoreApplication::translate("MainWindow", "\346\227\245\345\277\227", nullptr));
        dockWidget_3->setWindowTitle(QCoreApplication::translate("MainWindow", "\350\256\276\345\244\207\347\256\241\347\220\206", nullptr));
        groupBox_peephole_dev_com->setTitle(QCoreApplication::translate("MainWindow", "\350\256\276\345\244\207\344\270\262\345\217\243", nullptr));
        label_peephole_com_port->setText(QCoreApplication::translate("MainWindow", "\344\270\262\345\217\243", nullptr));
        label_peephole_baud_rate->setText(QCoreApplication::translate("MainWindow", "\346\263\242\347\211\271\347\216\207", nullptr));
        pushButton_peephole_coms->setText(QCoreApplication::translate("MainWindow", "\346\237\245\350\257\242\344\270\262\345\217\243", nullptr));
        pushButton_peephole_open->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200", nullptr));
#if QT_CONFIG(shortcut)
        pushButton_peephole_open->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        groupBox_protocol->setTitle(QCoreApplication::translate("MainWindow", "\345\215\217\350\256\256\347\261\273\345\236\213", nullptr));
        radioButton_protocol_3v->setText(QCoreApplication::translate("MainWindow", "C1D", nullptr));
        radioButton_protocol_k1s->setText(QCoreApplication::translate("MainWindow", "K1S", nullptr));
        radioButton_protocol_orbbec->setText(QCoreApplication::translate("MainWindow", "\345\245\245\346\257\224", nullptr));
        checkBox_msg_encrypt->setText(QCoreApplication::translate("MainWindow", "\345\212\240\345\257\206", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\344\270\262\345\217\243\346\214\207\344\273\244", nullptr));
        pushButton_send_cmd->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
