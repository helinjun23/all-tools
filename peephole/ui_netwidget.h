/********************************************************************************
** Form generated from reading UI file 'netwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NETWIDGET_H
#define UI_NETWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_netwidget
{
public:
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox_wifi_sn;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_wifi_sn;
    QLineEdit *lineEdit_wifi_device_name;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_wifi_sn_get;
    QGroupBox *groupBox_wifi_ssid_pwd;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout;
    QLabel *label_wifi_ssid;
    QLineEdit *lineEdit_wifi_ssid;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_wifi_pwd;
    QLineEdit *lineEdit_wifi_pwd;
    QHBoxLayout *horizontalLayout_3;
    QCheckBox *checkBox_wifi_reserved;
    QLineEdit *lineEdit_wifi_reserved;
    QCheckBox *checkBox_wifi_fac_mode;
    QHBoxLayout *horizontalLayout_14;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton_wifi_ssid_pwd_set;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_wifi_mac;
    QLineEdit *lineEdit_wifi_mac;
    QPushButton *pushButton_wifi_mac_get;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_device_sn;
    QLineEdit *lineEdit_device_sn;
    QPushButton *pushButton_device_sn_get;
    QGroupBox *groupBox_wifi_ip_info;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_wifi_ip;
    QLineEdit *lineEdit_wifi_ip;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_wifi_ip_mask;
    QLineEdit *lineEdit_wifi_ip_mask;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_wifi_ip_gate;
    QLineEdit *lineEdit_wifi_gate;
    QHBoxLayout *horizontalLayout_15;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *pushButton_wifi_ip_info_get;
    QGroupBox *groupBox_wifi_sta;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_wifi_state;
    QLineEdit *lineEdit_wifi_state;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_wifi_snr;
    QLineEdit *lineEdit_wifi_snr;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_wifi_db;
    QLineEdit *lineEdit_wifi_db;
    QHBoxLayout *horizontalLayout_13;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_wifi_state_get;

    void setupUi(QWidget *netwidget)
    {
        if (netwidget->objectName().isEmpty())
            netwidget->setObjectName(QString::fromUtf8("netwidget"));
        netwidget->resize(296, 612);
        verticalLayout_4 = new QVBoxLayout(netwidget);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        groupBox_wifi_sn = new QGroupBox(netwidget);
        groupBox_wifi_sn->setObjectName(QString::fromUtf8("groupBox_wifi_sn"));
        verticalLayout = new QVBoxLayout(groupBox_wifi_sn);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_wifi_sn = new QLabel(groupBox_wifi_sn);
        label_wifi_sn->setObjectName(QString::fromUtf8("label_wifi_sn"));

        horizontalLayout_4->addWidget(label_wifi_sn);

        lineEdit_wifi_device_name = new QLineEdit(groupBox_wifi_sn);
        lineEdit_wifi_device_name->setObjectName(QString::fromUtf8("lineEdit_wifi_device_name"));
        lineEdit_wifi_device_name->setReadOnly(true);

        horizontalLayout_4->addWidget(lineEdit_wifi_device_name);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        pushButton_wifi_sn_get = new QPushButton(groupBox_wifi_sn);
        pushButton_wifi_sn_get->setObjectName(QString::fromUtf8("pushButton_wifi_sn_get"));
        pushButton_wifi_sn_get->setMinimumSize(QSize(0, 30));

        horizontalLayout_4->addWidget(pushButton_wifi_sn_get);


        verticalLayout->addLayout(horizontalLayout_4);


        verticalLayout_4->addWidget(groupBox_wifi_sn);

        groupBox_wifi_ssid_pwd = new QGroupBox(netwidget);
        groupBox_wifi_ssid_pwd->setObjectName(QString::fromUtf8("groupBox_wifi_ssid_pwd"));
        verticalLayout_5 = new QVBoxLayout(groupBox_wifi_ssid_pwd);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_wifi_ssid = new QLabel(groupBox_wifi_ssid_pwd);
        label_wifi_ssid->setObjectName(QString::fromUtf8("label_wifi_ssid"));

        horizontalLayout->addWidget(label_wifi_ssid);

        lineEdit_wifi_ssid = new QLineEdit(groupBox_wifi_ssid_pwd);
        lineEdit_wifi_ssid->setObjectName(QString::fromUtf8("lineEdit_wifi_ssid"));

        horizontalLayout->addWidget(lineEdit_wifi_ssid);


        verticalLayout_5->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_wifi_pwd = new QLabel(groupBox_wifi_ssid_pwd);
        label_wifi_pwd->setObjectName(QString::fromUtf8("label_wifi_pwd"));

        horizontalLayout_2->addWidget(label_wifi_pwd);

        lineEdit_wifi_pwd = new QLineEdit(groupBox_wifi_ssid_pwd);
        lineEdit_wifi_pwd->setObjectName(QString::fromUtf8("lineEdit_wifi_pwd"));

        horizontalLayout_2->addWidget(lineEdit_wifi_pwd);


        verticalLayout_5->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        checkBox_wifi_reserved = new QCheckBox(groupBox_wifi_ssid_pwd);
        checkBox_wifi_reserved->setObjectName(QString::fromUtf8("checkBox_wifi_reserved"));

        horizontalLayout_3->addWidget(checkBox_wifi_reserved);

        lineEdit_wifi_reserved = new QLineEdit(groupBox_wifi_ssid_pwd);
        lineEdit_wifi_reserved->setObjectName(QString::fromUtf8("lineEdit_wifi_reserved"));

        horizontalLayout_3->addWidget(lineEdit_wifi_reserved);


        verticalLayout_5->addLayout(horizontalLayout_3);

        checkBox_wifi_fac_mode = new QCheckBox(groupBox_wifi_ssid_pwd);
        checkBox_wifi_fac_mode->setObjectName(QString::fromUtf8("checkBox_wifi_fac_mode"));

        verticalLayout_5->addWidget(checkBox_wifi_fac_mode);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_3);

        pushButton_wifi_ssid_pwd_set = new QPushButton(groupBox_wifi_ssid_pwd);
        pushButton_wifi_ssid_pwd_set->setObjectName(QString::fromUtf8("pushButton_wifi_ssid_pwd_set"));
        pushButton_wifi_ssid_pwd_set->setMinimumSize(QSize(0, 30));

        horizontalLayout_14->addWidget(pushButton_wifi_ssid_pwd_set);


        verticalLayout_5->addLayout(horizontalLayout_14);


        verticalLayout_4->addWidget(groupBox_wifi_ssid_pwd);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_wifi_mac = new QLabel(netwidget);
        label_wifi_mac->setObjectName(QString::fromUtf8("label_wifi_mac"));

        horizontalLayout_5->addWidget(label_wifi_mac);

        lineEdit_wifi_mac = new QLineEdit(netwidget);
        lineEdit_wifi_mac->setObjectName(QString::fromUtf8("lineEdit_wifi_mac"));
        lineEdit_wifi_mac->setReadOnly(true);

        horizontalLayout_5->addWidget(lineEdit_wifi_mac);

        pushButton_wifi_mac_get = new QPushButton(netwidget);
        pushButton_wifi_mac_get->setObjectName(QString::fromUtf8("pushButton_wifi_mac_get"));
        pushButton_wifi_mac_get->setMinimumSize(QSize(0, 30));

        horizontalLayout_5->addWidget(pushButton_wifi_mac_get);


        verticalLayout_4->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_device_sn = new QLabel(netwidget);
        label_device_sn->setObjectName(QString::fromUtf8("label_device_sn"));

        horizontalLayout_6->addWidget(label_device_sn);

        lineEdit_device_sn = new QLineEdit(netwidget);
        lineEdit_device_sn->setObjectName(QString::fromUtf8("lineEdit_device_sn"));
        lineEdit_device_sn->setReadOnly(true);

        horizontalLayout_6->addWidget(lineEdit_device_sn);

        pushButton_device_sn_get = new QPushButton(netwidget);
        pushButton_device_sn_get->setObjectName(QString::fromUtf8("pushButton_device_sn_get"));
        pushButton_device_sn_get->setMinimumSize(QSize(0, 30));

        horizontalLayout_6->addWidget(pushButton_device_sn_get);


        verticalLayout_4->addLayout(horizontalLayout_6);

        groupBox_wifi_ip_info = new QGroupBox(netwidget);
        groupBox_wifi_ip_info->setObjectName(QString::fromUtf8("groupBox_wifi_ip_info"));
        verticalLayout_2 = new QVBoxLayout(groupBox_wifi_ip_info);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_wifi_ip = new QLabel(groupBox_wifi_ip_info);
        label_wifi_ip->setObjectName(QString::fromUtf8("label_wifi_ip"));

        horizontalLayout_7->addWidget(label_wifi_ip);

        lineEdit_wifi_ip = new QLineEdit(groupBox_wifi_ip_info);
        lineEdit_wifi_ip->setObjectName(QString::fromUtf8("lineEdit_wifi_ip"));
        lineEdit_wifi_ip->setReadOnly(true);

        horizontalLayout_7->addWidget(lineEdit_wifi_ip);


        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_wifi_ip_mask = new QLabel(groupBox_wifi_ip_info);
        label_wifi_ip_mask->setObjectName(QString::fromUtf8("label_wifi_ip_mask"));

        horizontalLayout_8->addWidget(label_wifi_ip_mask);

        lineEdit_wifi_ip_mask = new QLineEdit(groupBox_wifi_ip_info);
        lineEdit_wifi_ip_mask->setObjectName(QString::fromUtf8("lineEdit_wifi_ip_mask"));
        lineEdit_wifi_ip_mask->setReadOnly(true);

        horizontalLayout_8->addWidget(lineEdit_wifi_ip_mask);


        verticalLayout_2->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_wifi_ip_gate = new QLabel(groupBox_wifi_ip_info);
        label_wifi_ip_gate->setObjectName(QString::fromUtf8("label_wifi_ip_gate"));

        horizontalLayout_9->addWidget(label_wifi_ip_gate);

        lineEdit_wifi_gate = new QLineEdit(groupBox_wifi_ip_info);
        lineEdit_wifi_gate->setObjectName(QString::fromUtf8("lineEdit_wifi_gate"));
        lineEdit_wifi_gate->setReadOnly(true);

        horizontalLayout_9->addWidget(lineEdit_wifi_gate);


        verticalLayout_2->addLayout(horizontalLayout_9);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_4);

        pushButton_wifi_ip_info_get = new QPushButton(groupBox_wifi_ip_info);
        pushButton_wifi_ip_info_get->setObjectName(QString::fromUtf8("pushButton_wifi_ip_info_get"));
        pushButton_wifi_ip_info_get->setMinimumSize(QSize(0, 30));

        horizontalLayout_15->addWidget(pushButton_wifi_ip_info_get);


        verticalLayout_2->addLayout(horizontalLayout_15);


        verticalLayout_4->addWidget(groupBox_wifi_ip_info);

        groupBox_wifi_sta = new QGroupBox(netwidget);
        groupBox_wifi_sta->setObjectName(QString::fromUtf8("groupBox_wifi_sta"));
        verticalLayout_3 = new QVBoxLayout(groupBox_wifi_sta);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_wifi_state = new QLabel(groupBox_wifi_sta);
        label_wifi_state->setObjectName(QString::fromUtf8("label_wifi_state"));

        horizontalLayout_10->addWidget(label_wifi_state);

        lineEdit_wifi_state = new QLineEdit(groupBox_wifi_sta);
        lineEdit_wifi_state->setObjectName(QString::fromUtf8("lineEdit_wifi_state"));
        lineEdit_wifi_state->setReadOnly(true);

        horizontalLayout_10->addWidget(lineEdit_wifi_state);


        verticalLayout_3->addLayout(horizontalLayout_10);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label_wifi_snr = new QLabel(groupBox_wifi_sta);
        label_wifi_snr->setObjectName(QString::fromUtf8("label_wifi_snr"));

        horizontalLayout_12->addWidget(label_wifi_snr);

        lineEdit_wifi_snr = new QLineEdit(groupBox_wifi_sta);
        lineEdit_wifi_snr->setObjectName(QString::fromUtf8("lineEdit_wifi_snr"));
        lineEdit_wifi_snr->setReadOnly(true);

        horizontalLayout_12->addWidget(lineEdit_wifi_snr);


        verticalLayout_3->addLayout(horizontalLayout_12);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_wifi_db = new QLabel(groupBox_wifi_sta);
        label_wifi_db->setObjectName(QString::fromUtf8("label_wifi_db"));

        horizontalLayout_11->addWidget(label_wifi_db);

        lineEdit_wifi_db = new QLineEdit(groupBox_wifi_sta);
        lineEdit_wifi_db->setObjectName(QString::fromUtf8("lineEdit_wifi_db"));
        lineEdit_wifi_db->setReadOnly(true);

        horizontalLayout_11->addWidget(lineEdit_wifi_db);


        verticalLayout_3->addLayout(horizontalLayout_11);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_2);

        pushButton_wifi_state_get = new QPushButton(groupBox_wifi_sta);
        pushButton_wifi_state_get->setObjectName(QString::fromUtf8("pushButton_wifi_state_get"));
        pushButton_wifi_state_get->setMinimumSize(QSize(0, 30));

        horizontalLayout_13->addWidget(pushButton_wifi_state_get);


        verticalLayout_3->addLayout(horizontalLayout_13);


        verticalLayout_4->addWidget(groupBox_wifi_sta);


        retranslateUi(netwidget);

        QMetaObject::connectSlotsByName(netwidget);
    } // setupUi

    void retranslateUi(QWidget *netwidget)
    {
        netwidget->setWindowTitle(QCoreApplication::translate("netwidget", "\347\275\221\347\273\234\347\256\241\347\220\206", nullptr));
        groupBox_wifi_sn->setTitle(QString());
        label_wifi_sn->setText(QCoreApplication::translate("netwidget", "\344\272\221\345\272\217\345\210\227\345\217\267", nullptr));
        pushButton_wifi_sn_get->setText(QCoreApplication::translate("netwidget", "\350\216\267\345\217\226", nullptr));
        groupBox_wifi_ssid_pwd->setTitle(QCoreApplication::translate("netwidget", "ssid", nullptr));
        label_wifi_ssid->setText(QCoreApplication::translate("netwidget", "ssid", nullptr));
        label_wifi_pwd->setText(QCoreApplication::translate("netwidget", "\345\257\206\347\240\201", nullptr));
        checkBox_wifi_reserved->setText(QCoreApplication::translate("netwidget", "\344\277\235\347\225\231\347\240\201(\346\266\202\351\270\246)", nullptr));
        checkBox_wifi_fac_mode->setText(QCoreApplication::translate("netwidget", "\345\267\245\345\216\202\346\250\241\345\274\217(3V)", nullptr));
        pushButton_wifi_ssid_pwd_set->setText(QCoreApplication::translate("netwidget", "\350\256\276\347\275\256", nullptr));
        label_wifi_mac->setText(QCoreApplication::translate("netwidget", "MAC\345\234\260\345\235\200", nullptr));
        pushButton_wifi_mac_get->setText(QCoreApplication::translate("netwidget", "\350\216\267\345\217\226", nullptr));
        label_device_sn->setText(QCoreApplication::translate("netwidget", "SN", nullptr));
        pushButton_device_sn_get->setText(QCoreApplication::translate("netwidget", "\350\216\267\345\217\226", nullptr));
        groupBox_wifi_ip_info->setTitle(QCoreApplication::translate("netwidget", "IP", nullptr));
        label_wifi_ip->setText(QCoreApplication::translate("netwidget", "IP", nullptr));
        label_wifi_ip_mask->setText(QCoreApplication::translate("netwidget", "\346\216\251\347\240\201", nullptr));
        label_wifi_ip_gate->setText(QCoreApplication::translate("netwidget", "\347\275\221\345\205\263", nullptr));
        pushButton_wifi_ip_info_get->setText(QCoreApplication::translate("netwidget", "\350\216\267\345\217\226", nullptr));
        groupBox_wifi_sta->setTitle(QCoreApplication::translate("netwidget", "Wifi\347\212\266\346\200\201", nullptr));
        label_wifi_state->setText(QCoreApplication::translate("netwidget", "\347\212\266\346\200\201", nullptr));
        label_wifi_snr->setText(QCoreApplication::translate("netwidget", "\344\277\241\345\231\252\346\257\224", nullptr));
        label_wifi_db->setText(QCoreApplication::translate("netwidget", "\345\274\272\345\272\246", nullptr));
        pushButton_wifi_state_get->setText(QCoreApplication::translate("netwidget", "\350\216\267\345\217\226", nullptr));
    } // retranslateUi

};

namespace Ui {
    class netwidget: public Ui_netwidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NETWIDGET_H
