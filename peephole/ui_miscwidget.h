/********************************************************************************
** Form generated from reading UI file 'miscwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MISCWIDGET_H
#define UI_MISCWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_miscwidget
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_pic_dir;
    QHBoxLayout *horizontalLayout_11;
    QRadioButton *radioButton_pic_landscape;
    QRadioButton *radioButton_pic_portrait;
    QHBoxLayout *horizontalLayout_12;
    QCheckBox *checkBox_pir;
    QCheckBox *checkBox_ota;
    QCheckBox *checkBox_scan_conf;
    QGroupBox *groupBox_wifi_action_typ;
    QHBoxLayout *horizontalLayout_10;
    QRadioButton *radioButton_wifi_action_standard;
    QRadioButton *radioButton_wifi_action_live;
    QRadioButton *radioButton_wifi_action_sleep;
    QGroupBox *groupBox_wifi_live_face;
    QHBoxLayout *horizontalLayout_9;
    QRadioButton *radioButton_wifi_action_face_live;
    QRadioButton *radioButton_wifi_action_face_sleep;
    QGroupBox *groupBox_lool_up_remind;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_remind_open_typ;
    QComboBox *comboBox_remind_open_typ;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_remind_user_id;
    QLineEdit *lineEdit_remind_user_id;
    QHBoxLayout *horizontalLayout_14;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_look_up_remind;
    QHBoxLayout *horizontalLayout;
    QLabel *label_version;
    QLineEdit *lineEdit_version;
    QPushButton *pushButton_version_get;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_net_time;
    QLineEdit *lineEdit_net_time;
    QPushButton *pushButton_net_time_get;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_fov;
    QLineEdit *lineEdit_fov_set;
    QPushButton *pushButton_fov_set;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_dev_state;
    QLineEdit *lineEdit_dev_state;
    QPushButton *pushButton_dev_state_get;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_person_detect;
    QLineEdit *lineEdit_person_detect;
    QPushButton *pushButton_person_detect_state_get;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_mask_detect;
    QLineEdit *lineEdit_mask_detect_state;
    QPushButton *pushButton_mask_detect_state_get;
    QHBoxLayout *horizontalLayout_13;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_peephole_sleep;
    QPushButton *pushButton_peephole_reset_2;

    void setupUi(QWidget *miscwidget)
    {
        if (miscwidget->objectName().isEmpty())
            miscwidget->setObjectName(QString::fromUtf8("miscwidget"));
        miscwidget->resize(352, 555);
        verticalLayout_2 = new QVBoxLayout(miscwidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox_pic_dir = new QGroupBox(miscwidget);
        groupBox_pic_dir->setObjectName(QString::fromUtf8("groupBox_pic_dir"));
        groupBox_pic_dir->setEnabled(true);
        horizontalLayout_11 = new QHBoxLayout(groupBox_pic_dir);
        horizontalLayout_11->setSpacing(1);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        horizontalLayout_11->setContentsMargins(3, 3, 3, 3);
        radioButton_pic_landscape = new QRadioButton(groupBox_pic_dir);
        radioButton_pic_landscape->setObjectName(QString::fromUtf8("radioButton_pic_landscape"));

        horizontalLayout_11->addWidget(radioButton_pic_landscape);

        radioButton_pic_portrait = new QRadioButton(groupBox_pic_dir);
        radioButton_pic_portrait->setObjectName(QString::fromUtf8("radioButton_pic_portrait"));

        horizontalLayout_11->addWidget(radioButton_pic_portrait);


        verticalLayout_2->addWidget(groupBox_pic_dir);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        checkBox_pir = new QCheckBox(miscwidget);
        checkBox_pir->setObjectName(QString::fromUtf8("checkBox_pir"));

        horizontalLayout_12->addWidget(checkBox_pir);

        checkBox_ota = new QCheckBox(miscwidget);
        checkBox_ota->setObjectName(QString::fromUtf8("checkBox_ota"));

        horizontalLayout_12->addWidget(checkBox_ota);

        checkBox_scan_conf = new QCheckBox(miscwidget);
        checkBox_scan_conf->setObjectName(QString::fromUtf8("checkBox_scan_conf"));

        horizontalLayout_12->addWidget(checkBox_scan_conf);


        verticalLayout_2->addLayout(horizontalLayout_12);

        groupBox_wifi_action_typ = new QGroupBox(miscwidget);
        groupBox_wifi_action_typ->setObjectName(QString::fromUtf8("groupBox_wifi_action_typ"));
        groupBox_wifi_action_typ->setEnabled(true);
        horizontalLayout_10 = new QHBoxLayout(groupBox_wifi_action_typ);
        horizontalLayout_10->setSpacing(1);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(3, 3, 3, 3);
        radioButton_wifi_action_standard = new QRadioButton(groupBox_wifi_action_typ);
        radioButton_wifi_action_standard->setObjectName(QString::fromUtf8("radioButton_wifi_action_standard"));

        horizontalLayout_10->addWidget(radioButton_wifi_action_standard);

        radioButton_wifi_action_live = new QRadioButton(groupBox_wifi_action_typ);
        radioButton_wifi_action_live->setObjectName(QString::fromUtf8("radioButton_wifi_action_live"));

        horizontalLayout_10->addWidget(radioButton_wifi_action_live);

        radioButton_wifi_action_sleep = new QRadioButton(groupBox_wifi_action_typ);
        radioButton_wifi_action_sleep->setObjectName(QString::fromUtf8("radioButton_wifi_action_sleep"));

        horizontalLayout_10->addWidget(radioButton_wifi_action_sleep);


        verticalLayout_2->addWidget(groupBox_wifi_action_typ);

        groupBox_wifi_live_face = new QGroupBox(miscwidget);
        groupBox_wifi_live_face->setObjectName(QString::fromUtf8("groupBox_wifi_live_face"));
        groupBox_wifi_live_face->setEnabled(true);
        horizontalLayout_9 = new QHBoxLayout(groupBox_wifi_live_face);
        horizontalLayout_9->setSpacing(1);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(3, 3, 3, 3);
        radioButton_wifi_action_face_live = new QRadioButton(groupBox_wifi_live_face);
        radioButton_wifi_action_face_live->setObjectName(QString::fromUtf8("radioButton_wifi_action_face_live"));

        horizontalLayout_9->addWidget(radioButton_wifi_action_face_live);

        radioButton_wifi_action_face_sleep = new QRadioButton(groupBox_wifi_live_face);
        radioButton_wifi_action_face_sleep->setObjectName(QString::fromUtf8("radioButton_wifi_action_face_sleep"));

        horizontalLayout_9->addWidget(radioButton_wifi_action_face_sleep);


        verticalLayout_2->addWidget(groupBox_wifi_live_face);

        groupBox_lool_up_remind = new QGroupBox(miscwidget);
        groupBox_lool_up_remind->setObjectName(QString::fromUtf8("groupBox_lool_up_remind"));
        groupBox_lool_up_remind->setEnabled(true);
        groupBox_lool_up_remind->setCheckable(true);
        groupBox_lool_up_remind->setChecked(false);
        verticalLayout = new QVBoxLayout(groupBox_lool_up_remind);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_remind_open_typ = new QLabel(groupBox_lool_up_remind);
        label_remind_open_typ->setObjectName(QString::fromUtf8("label_remind_open_typ"));

        horizontalLayout_7->addWidget(label_remind_open_typ);

        comboBox_remind_open_typ = new QComboBox(groupBox_lool_up_remind);
        comboBox_remind_open_typ->setObjectName(QString::fromUtf8("comboBox_remind_open_typ"));

        horizontalLayout_7->addWidget(comboBox_remind_open_typ);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_remind_user_id = new QLabel(groupBox_lool_up_remind);
        label_remind_user_id->setObjectName(QString::fromUtf8("label_remind_user_id"));

        horizontalLayout_8->addWidget(label_remind_user_id);

        lineEdit_remind_user_id = new QLineEdit(groupBox_lool_up_remind);
        lineEdit_remind_user_id->setObjectName(QString::fromUtf8("lineEdit_remind_user_id"));

        horizontalLayout_8->addWidget(lineEdit_remind_user_id);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_2);

        pushButton_look_up_remind = new QPushButton(groupBox_lool_up_remind);
        pushButton_look_up_remind->setObjectName(QString::fromUtf8("pushButton_look_up_remind"));
        pushButton_look_up_remind->setMinimumSize(QSize(0, 30));

        horizontalLayout_14->addWidget(pushButton_look_up_remind);


        verticalLayout->addLayout(horizontalLayout_14);


        verticalLayout_2->addWidget(groupBox_lool_up_remind);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_version = new QLabel(miscwidget);
        label_version->setObjectName(QString::fromUtf8("label_version"));

        horizontalLayout->addWidget(label_version);

        lineEdit_version = new QLineEdit(miscwidget);
        lineEdit_version->setObjectName(QString::fromUtf8("lineEdit_version"));
        lineEdit_version->setReadOnly(true);

        horizontalLayout->addWidget(lineEdit_version);

        pushButton_version_get = new QPushButton(miscwidget);
        pushButton_version_get->setObjectName(QString::fromUtf8("pushButton_version_get"));
        pushButton_version_get->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(pushButton_version_get);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_net_time = new QLabel(miscwidget);
        label_net_time->setObjectName(QString::fromUtf8("label_net_time"));

        horizontalLayout_2->addWidget(label_net_time);

        lineEdit_net_time = new QLineEdit(miscwidget);
        lineEdit_net_time->setObjectName(QString::fromUtf8("lineEdit_net_time"));
        lineEdit_net_time->setReadOnly(true);

        horizontalLayout_2->addWidget(lineEdit_net_time);

        pushButton_net_time_get = new QPushButton(miscwidget);
        pushButton_net_time_get->setObjectName(QString::fromUtf8("pushButton_net_time_get"));
        pushButton_net_time_get->setMinimumSize(QSize(0, 30));

        horizontalLayout_2->addWidget(pushButton_net_time_get);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_fov = new QLabel(miscwidget);
        label_fov->setObjectName(QString::fromUtf8("label_fov"));

        horizontalLayout_6->addWidget(label_fov);

        lineEdit_fov_set = new QLineEdit(miscwidget);
        lineEdit_fov_set->setObjectName(QString::fromUtf8("lineEdit_fov_set"));

        horizontalLayout_6->addWidget(lineEdit_fov_set);

        pushButton_fov_set = new QPushButton(miscwidget);
        pushButton_fov_set->setObjectName(QString::fromUtf8("pushButton_fov_set"));
        pushButton_fov_set->setMinimumSize(QSize(0, 30));

        horizontalLayout_6->addWidget(pushButton_fov_set);


        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_dev_state = new QLabel(miscwidget);
        label_dev_state->setObjectName(QString::fromUtf8("label_dev_state"));

        horizontalLayout_3->addWidget(label_dev_state);

        lineEdit_dev_state = new QLineEdit(miscwidget);
        lineEdit_dev_state->setObjectName(QString::fromUtf8("lineEdit_dev_state"));
        lineEdit_dev_state->setReadOnly(true);

        horizontalLayout_3->addWidget(lineEdit_dev_state);

        pushButton_dev_state_get = new QPushButton(miscwidget);
        pushButton_dev_state_get->setObjectName(QString::fromUtf8("pushButton_dev_state_get"));
        pushButton_dev_state_get->setMinimumSize(QSize(0, 30));

        horizontalLayout_3->addWidget(pushButton_dev_state_get);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_person_detect = new QLabel(miscwidget);
        label_person_detect->setObjectName(QString::fromUtf8("label_person_detect"));

        horizontalLayout_4->addWidget(label_person_detect);

        lineEdit_person_detect = new QLineEdit(miscwidget);
        lineEdit_person_detect->setObjectName(QString::fromUtf8("lineEdit_person_detect"));
        lineEdit_person_detect->setReadOnly(true);

        horizontalLayout_4->addWidget(lineEdit_person_detect);

        pushButton_person_detect_state_get = new QPushButton(miscwidget);
        pushButton_person_detect_state_get->setObjectName(QString::fromUtf8("pushButton_person_detect_state_get"));
        pushButton_person_detect_state_get->setMinimumSize(QSize(0, 30));

        horizontalLayout_4->addWidget(pushButton_person_detect_state_get);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_mask_detect = new QLabel(miscwidget);
        label_mask_detect->setObjectName(QString::fromUtf8("label_mask_detect"));

        horizontalLayout_5->addWidget(label_mask_detect);

        lineEdit_mask_detect_state = new QLineEdit(miscwidget);
        lineEdit_mask_detect_state->setObjectName(QString::fromUtf8("lineEdit_mask_detect_state"));
        lineEdit_mask_detect_state->setReadOnly(true);

        horizontalLayout_5->addWidget(lineEdit_mask_detect_state);

        pushButton_mask_detect_state_get = new QPushButton(miscwidget);
        pushButton_mask_detect_state_get->setObjectName(QString::fromUtf8("pushButton_mask_detect_state_get"));
        pushButton_mask_detect_state_get->setMinimumSize(QSize(0, 30));

        horizontalLayout_5->addWidget(pushButton_mask_detect_state_get);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer);

        pushButton_peephole_sleep = new QPushButton(miscwidget);
        pushButton_peephole_sleep->setObjectName(QString::fromUtf8("pushButton_peephole_sleep"));
        pushButton_peephole_sleep->setMinimumSize(QSize(0, 30));
        pushButton_peephole_sleep->setCheckable(false);
        pushButton_peephole_sleep->setAutoRepeat(false);

        horizontalLayout_13->addWidget(pushButton_peephole_sleep);

        pushButton_peephole_reset_2 = new QPushButton(miscwidget);
        pushButton_peephole_reset_2->setObjectName(QString::fromUtf8("pushButton_peephole_reset_2"));
        pushButton_peephole_reset_2->setMinimumSize(QSize(0, 30));
        pushButton_peephole_reset_2->setCheckable(false);
        pushButton_peephole_reset_2->setAutoRepeat(false);

        horizontalLayout_13->addWidget(pushButton_peephole_reset_2);


        verticalLayout_2->addLayout(horizontalLayout_13);


        retranslateUi(miscwidget);

        QMetaObject::connectSlotsByName(miscwidget);
    } // setupUi

    void retranslateUi(QWidget *miscwidget)
    {
        miscwidget->setWindowTitle(QCoreApplication::translate("miscwidget", "\345\205\266\344\273\226\351\205\215\347\275\256", nullptr));
        groupBox_pic_dir->setTitle(QCoreApplication::translate("miscwidget", "\346\230\276\347\244\272\346\226\271\345\220\221", nullptr));
        radioButton_pic_landscape->setText(QCoreApplication::translate("miscwidget", "\346\250\252\345\261\217", nullptr));
        radioButton_pic_portrait->setText(QCoreApplication::translate("miscwidget", "\347\253\226\345\261\217", nullptr));
        checkBox_pir->setText(QCoreApplication::translate("miscwidget", "PIR", nullptr));
        checkBox_ota->setText(QCoreApplication::translate("miscwidget", "OTA", nullptr));
        checkBox_scan_conf->setText(QCoreApplication::translate("miscwidget", "\346\211\253\347\240\201\351\205\215\347\275\221", nullptr));
        groupBox_wifi_action_typ->setTitle(QCoreApplication::translate("miscwidget", "wifi\344\277\235\346\264\273(\345\215\225\347\214\253\347\234\274)", nullptr));
        radioButton_wifi_action_standard->setText(QCoreApplication::translate("miscwidget", "\351\273\230\350\256\244", nullptr));
        radioButton_wifi_action_live->setText(QCoreApplication::translate("miscwidget", "\344\277\235\346\264\273", nullptr));
        radioButton_wifi_action_sleep->setText(QCoreApplication::translate("miscwidget", "\344\274\221\347\234\240", nullptr));
        groupBox_wifi_live_face->setTitle(QCoreApplication::translate("miscwidget", "wifi\344\277\235\346\264\273(\344\272\272\350\204\270\347\214\253\347\234\274)", nullptr));
        radioButton_wifi_action_face_live->setText(QCoreApplication::translate("miscwidget", "\344\277\235\346\264\273", nullptr));
        radioButton_wifi_action_face_sleep->setText(QCoreApplication::translate("miscwidget", "\344\274\221\347\234\240", nullptr));
        groupBox_lool_up_remind->setTitle(QCoreApplication::translate("miscwidget", "\346\237\245\350\257\242\347\225\231\350\250\200", nullptr));
        label_remind_open_typ->setText(QCoreApplication::translate("miscwidget", "\345\274\200\351\227\250\347\261\273\345\236\213", nullptr));
        label_remind_user_id->setText(QCoreApplication::translate("miscwidget", "UserID", nullptr));
        pushButton_look_up_remind->setText(QCoreApplication::translate("miscwidget", "\346\237\245\350\257\242", nullptr));
        label_version->setText(QCoreApplication::translate("miscwidget", "\347\211\210\346\234\254", nullptr));
        pushButton_version_get->setText(QCoreApplication::translate("miscwidget", "get", nullptr));
        label_net_time->setText(QCoreApplication::translate("miscwidget", "\346\227\266\351\227\264", nullptr));
        pushButton_net_time_get->setText(QCoreApplication::translate("miscwidget", "get", nullptr));
        label_fov->setText(QCoreApplication::translate("miscwidget", "FOV", nullptr));
        pushButton_fov_set->setText(QCoreApplication::translate("miscwidget", "set", nullptr));
        label_dev_state->setText(QCoreApplication::translate("miscwidget", "\350\256\276\345\244\207\347\212\266\346\200\201", nullptr));
        pushButton_dev_state_get->setText(QCoreApplication::translate("miscwidget", "get", nullptr));
        label_person_detect->setText(QCoreApplication::translate("miscwidget", "\344\272\272\345\275\242", nullptr));
        pushButton_person_detect_state_get->setText(QCoreApplication::translate("miscwidget", "get", nullptr));
        label_mask_detect->setText(QCoreApplication::translate("miscwidget", "\351\201\256\346\214\241", nullptr));
        pushButton_mask_detect_state_get->setText(QCoreApplication::translate("miscwidget", "get", nullptr));
        pushButton_peephole_sleep->setText(QCoreApplication::translate("miscwidget", "\345\277\253\351\200\237\344\274\221\347\234\240", nullptr));
#if QT_CONFIG(shortcut)
        pushButton_peephole_sleep->setShortcut(QCoreApplication::translate("miscwidget", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        pushButton_peephole_reset_2->setText(QCoreApplication::translate("miscwidget", "\346\201\242\345\244\215\345\207\272\345\216\202", nullptr));
#if QT_CONFIG(shortcut)
        pushButton_peephole_reset_2->setShortcut(QCoreApplication::translate("miscwidget", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
    } // retranslateUi

};

namespace Ui {
    class miscwidget: public Ui_miscwidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MISCWIDGET_H
