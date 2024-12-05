/********************************************************************************
** Form generated from reading UI file 'factory1widget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FACTORY1WIDGET_H
#define UI_FACTORY1WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_factory1widget
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_sdcard;
    QLineEdit *lineEdit_sdcard;
    QPushButton *pushButton_sdcard_get;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_snesor_sta;
    QLineEdit *lineEdit_sensor_sta;
    QPushButton *pushButton_sensor_sta_get;
    QGroupBox *groupBox_ircut;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_ircut_state;
    QComboBox *comboBox_ircut_state;
    QPushButton *pushButton_ircut_state_get;
    QPushButton *pushButton_ircut_state_set;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label;
    QLineEdit *lineEdit_orb_sn;
    QPushButton *pushButton_orb_sn_set;
    QPushButton *pushButton_orb_sn_get;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_light_sensitive_val;
    QLineEdit *lineEdit_light_sensitive_val;
    QPushButton *pushButton_light_sensitive_get;
    QPushButton *pushButton_light_sensitive_open;
    QGroupBox *groupBox_pir;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_pir_time;
    QLineEdit *lineEdit_pir_time;
    QPushButton *pushButton_pir_time_get;
    QPushButton *pushButton_pir_time_set;
    QHBoxLayout *horizontalLayout;
    QLabel *label_pir_state;
    QLineEdit *lineEdit_pir_state;
    QPushButton *pushButton_pir_state_get;
    QPushButton *pushButton_pir_state_set;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_pir_dist;
    QLineEdit *lineEdit_pir_dist;
    QPushButton *pushButton_pir_thresh_get;
    QPushButton *pushButton_pir_thresh_set;
    QHBoxLayout *horizontalLayout_10;
    QCheckBox *checkBox_led;
    QPushButton *pushButton_peephole_reset;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_version_wifi;
    QLineEdit *lineEdit_version_wifi;
    QPushButton *pushButton_version_wifi_get;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_version_system;
    QLineEdit *lineEdit_version_system;
    QPushButton *pushButton_version_system_get;

    void setupUi(QWidget *factory1widget)
    {
        if (factory1widget->objectName().isEmpty())
            factory1widget->setObjectName(QString::fromUtf8("factory1widget"));
        factory1widget->resize(374, 471);
        verticalLayout_2 = new QVBoxLayout(factory1widget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_sdcard = new QLabel(factory1widget);
        label_sdcard->setObjectName(QString::fromUtf8("label_sdcard"));

        horizontalLayout_8->addWidget(label_sdcard);

        lineEdit_sdcard = new QLineEdit(factory1widget);
        lineEdit_sdcard->setObjectName(QString::fromUtf8("lineEdit_sdcard"));
        lineEdit_sdcard->setReadOnly(true);

        horizontalLayout_8->addWidget(lineEdit_sdcard);

        pushButton_sdcard_get = new QPushButton(factory1widget);
        pushButton_sdcard_get->setObjectName(QString::fromUtf8("pushButton_sdcard_get"));
        pushButton_sdcard_get->setMinimumSize(QSize(0, 30));

        horizontalLayout_8->addWidget(pushButton_sdcard_get);


        verticalLayout_2->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_snesor_sta = new QLabel(factory1widget);
        label_snesor_sta->setObjectName(QString::fromUtf8("label_snesor_sta"));

        horizontalLayout_9->addWidget(label_snesor_sta);

        lineEdit_sensor_sta = new QLineEdit(factory1widget);
        lineEdit_sensor_sta->setObjectName(QString::fromUtf8("lineEdit_sensor_sta"));
        lineEdit_sensor_sta->setReadOnly(true);

        horizontalLayout_9->addWidget(lineEdit_sensor_sta);

        pushButton_sensor_sta_get = new QPushButton(factory1widget);
        pushButton_sensor_sta_get->setObjectName(QString::fromUtf8("pushButton_sensor_sta_get"));
        pushButton_sensor_sta_get->setMinimumSize(QSize(0, 30));

        horizontalLayout_9->addWidget(pushButton_sensor_sta_get);


        verticalLayout_2->addLayout(horizontalLayout_9);

        groupBox_ircut = new QGroupBox(factory1widget);
        groupBox_ircut->setObjectName(QString::fromUtf8("groupBox_ircut"));
        verticalLayout_3 = new QVBoxLayout(groupBox_ircut);
        verticalLayout_3->setSpacing(1);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(3, 3, 3, 3);
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_ircut_state = new QLabel(groupBox_ircut);
        label_ircut_state->setObjectName(QString::fromUtf8("label_ircut_state"));

        horizontalLayout_5->addWidget(label_ircut_state);

        comboBox_ircut_state = new QComboBox(groupBox_ircut);
        comboBox_ircut_state->setObjectName(QString::fromUtf8("comboBox_ircut_state"));

        horizontalLayout_5->addWidget(comboBox_ircut_state);

        pushButton_ircut_state_get = new QPushButton(groupBox_ircut);
        pushButton_ircut_state_get->setObjectName(QString::fromUtf8("pushButton_ircut_state_get"));
        pushButton_ircut_state_get->setMinimumSize(QSize(0, 30));

        horizontalLayout_5->addWidget(pushButton_ircut_state_get);

        pushButton_ircut_state_set = new QPushButton(groupBox_ircut);
        pushButton_ircut_state_set->setObjectName(QString::fromUtf8("pushButton_ircut_state_set"));
        pushButton_ircut_state_set->setMinimumSize(QSize(0, 30));

        horizontalLayout_5->addWidget(pushButton_ircut_state_set);


        verticalLayout_3->addLayout(horizontalLayout_5);


        verticalLayout_2->addWidget(groupBox_ircut);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label = new QLabel(factory1widget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_6->addWidget(label);

        lineEdit_orb_sn = new QLineEdit(factory1widget);
        lineEdit_orb_sn->setObjectName(QString::fromUtf8("lineEdit_orb_sn"));

        horizontalLayout_6->addWidget(lineEdit_orb_sn);

        pushButton_orb_sn_set = new QPushButton(factory1widget);
        pushButton_orb_sn_set->setObjectName(QString::fromUtf8("pushButton_orb_sn_set"));
        pushButton_orb_sn_set->setMinimumSize(QSize(0, 30));

        horizontalLayout_6->addWidget(pushButton_orb_sn_set);

        pushButton_orb_sn_get = new QPushButton(factory1widget);
        pushButton_orb_sn_get->setObjectName(QString::fromUtf8("pushButton_orb_sn_get"));
        pushButton_orb_sn_get->setMinimumSize(QSize(0, 30));

        horizontalLayout_6->addWidget(pushButton_orb_sn_get);


        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_light_sensitive_val = new QLabel(factory1widget);
        label_light_sensitive_val->setObjectName(QString::fromUtf8("label_light_sensitive_val"));

        horizontalLayout_4->addWidget(label_light_sensitive_val);

        lineEdit_light_sensitive_val = new QLineEdit(factory1widget);
        lineEdit_light_sensitive_val->setObjectName(QString::fromUtf8("lineEdit_light_sensitive_val"));

        horizontalLayout_4->addWidget(lineEdit_light_sensitive_val);

        pushButton_light_sensitive_get = new QPushButton(factory1widget);
        pushButton_light_sensitive_get->setObjectName(QString::fromUtf8("pushButton_light_sensitive_get"));
        pushButton_light_sensitive_get->setMinimumSize(QSize(0, 30));

        horizontalLayout_4->addWidget(pushButton_light_sensitive_get);

        pushButton_light_sensitive_open = new QPushButton(factory1widget);
        pushButton_light_sensitive_open->setObjectName(QString::fromUtf8("pushButton_light_sensitive_open"));
        pushButton_light_sensitive_open->setMinimumSize(QSize(0, 30));

        horizontalLayout_4->addWidget(pushButton_light_sensitive_open);


        verticalLayout_2->addLayout(horizontalLayout_4);

        groupBox_pir = new QGroupBox(factory1widget);
        groupBox_pir->setObjectName(QString::fromUtf8("groupBox_pir"));
        verticalLayout = new QVBoxLayout(groupBox_pir);
        verticalLayout->setSpacing(1);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(3, 3, 3, 3);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_pir_time = new QLabel(groupBox_pir);
        label_pir_time->setObjectName(QString::fromUtf8("label_pir_time"));

        horizontalLayout_2->addWidget(label_pir_time);

        lineEdit_pir_time = new QLineEdit(groupBox_pir);
        lineEdit_pir_time->setObjectName(QString::fromUtf8("lineEdit_pir_time"));

        horizontalLayout_2->addWidget(lineEdit_pir_time);

        pushButton_pir_time_get = new QPushButton(groupBox_pir);
        pushButton_pir_time_get->setObjectName(QString::fromUtf8("pushButton_pir_time_get"));
        pushButton_pir_time_get->setMinimumSize(QSize(0, 30));

        horizontalLayout_2->addWidget(pushButton_pir_time_get);

        pushButton_pir_time_set = new QPushButton(groupBox_pir);
        pushButton_pir_time_set->setObjectName(QString::fromUtf8("pushButton_pir_time_set"));
        pushButton_pir_time_set->setMinimumSize(QSize(0, 30));

        horizontalLayout_2->addWidget(pushButton_pir_time_set);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_pir_state = new QLabel(groupBox_pir);
        label_pir_state->setObjectName(QString::fromUtf8("label_pir_state"));

        horizontalLayout->addWidget(label_pir_state);

        lineEdit_pir_state = new QLineEdit(groupBox_pir);
        lineEdit_pir_state->setObjectName(QString::fromUtf8("lineEdit_pir_state"));

        horizontalLayout->addWidget(lineEdit_pir_state);

        pushButton_pir_state_get = new QPushButton(groupBox_pir);
        pushButton_pir_state_get->setObjectName(QString::fromUtf8("pushButton_pir_state_get"));
        pushButton_pir_state_get->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(pushButton_pir_state_get);

        pushButton_pir_state_set = new QPushButton(groupBox_pir);
        pushButton_pir_state_set->setObjectName(QString::fromUtf8("pushButton_pir_state_set"));
        pushButton_pir_state_set->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(pushButton_pir_state_set);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_pir_dist = new QLabel(groupBox_pir);
        label_pir_dist->setObjectName(QString::fromUtf8("label_pir_dist"));

        horizontalLayout_3->addWidget(label_pir_dist);

        lineEdit_pir_dist = new QLineEdit(groupBox_pir);
        lineEdit_pir_dist->setObjectName(QString::fromUtf8("lineEdit_pir_dist"));

        horizontalLayout_3->addWidget(lineEdit_pir_dist);

        pushButton_pir_thresh_get = new QPushButton(groupBox_pir);
        pushButton_pir_thresh_get->setObjectName(QString::fromUtf8("pushButton_pir_thresh_get"));
        pushButton_pir_thresh_get->setMinimumSize(QSize(0, 30));

        horizontalLayout_3->addWidget(pushButton_pir_thresh_get);

        pushButton_pir_thresh_set = new QPushButton(groupBox_pir);
        pushButton_pir_thresh_set->setObjectName(QString::fromUtf8("pushButton_pir_thresh_set"));
        pushButton_pir_thresh_set->setMinimumSize(QSize(0, 30));

        horizontalLayout_3->addWidget(pushButton_pir_thresh_set);


        verticalLayout->addLayout(horizontalLayout_3);


        verticalLayout_2->addWidget(groupBox_pir);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        checkBox_led = new QCheckBox(factory1widget);
        checkBox_led->setObjectName(QString::fromUtf8("checkBox_led"));

        horizontalLayout_10->addWidget(checkBox_led);

        pushButton_peephole_reset = new QPushButton(factory1widget);
        pushButton_peephole_reset->setObjectName(QString::fromUtf8("pushButton_peephole_reset"));
        pushButton_peephole_reset->setMinimumSize(QSize(0, 30));
        pushButton_peephole_reset->setCheckable(false);
        pushButton_peephole_reset->setAutoRepeat(false);

        horizontalLayout_10->addWidget(pushButton_peephole_reset);


        verticalLayout_2->addLayout(horizontalLayout_10);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_version_wifi = new QLabel(factory1widget);
        label_version_wifi->setObjectName(QString::fromUtf8("label_version_wifi"));

        horizontalLayout_7->addWidget(label_version_wifi);

        lineEdit_version_wifi = new QLineEdit(factory1widget);
        lineEdit_version_wifi->setObjectName(QString::fromUtf8("lineEdit_version_wifi"));
        lineEdit_version_wifi->setReadOnly(true);

        horizontalLayout_7->addWidget(lineEdit_version_wifi);

        pushButton_version_wifi_get = new QPushButton(factory1widget);
        pushButton_version_wifi_get->setObjectName(QString::fromUtf8("pushButton_version_wifi_get"));
        pushButton_version_wifi_get->setMinimumSize(QSize(0, 30));

        horizontalLayout_7->addWidget(pushButton_version_wifi_get);


        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_version_system = new QLabel(factory1widget);
        label_version_system->setObjectName(QString::fromUtf8("label_version_system"));

        horizontalLayout_11->addWidget(label_version_system);

        lineEdit_version_system = new QLineEdit(factory1widget);
        lineEdit_version_system->setObjectName(QString::fromUtf8("lineEdit_version_system"));
        lineEdit_version_system->setReadOnly(true);

        horizontalLayout_11->addWidget(lineEdit_version_system);

        pushButton_version_system_get = new QPushButton(factory1widget);
        pushButton_version_system_get->setObjectName(QString::fromUtf8("pushButton_version_system_get"));
        pushButton_version_system_get->setMinimumSize(QSize(0, 30));

        horizontalLayout_11->addWidget(pushButton_version_system_get);


        verticalLayout_2->addLayout(horizontalLayout_11);


        retranslateUi(factory1widget);

        QMetaObject::connectSlotsByName(factory1widget);
    } // setupUi

    void retranslateUi(QWidget *factory1widget)
    {
        factory1widget->setWindowTitle(QCoreApplication::translate("factory1widget", "\347\224\237\344\272\2471", nullptr));
        label_sdcard->setText(QCoreApplication::translate("factory1widget", "sdcard", nullptr));
        pushButton_sdcard_get->setText(QCoreApplication::translate("factory1widget", "get", nullptr));
        label_snesor_sta->setText(QCoreApplication::translate("factory1widget", "sensor\347\212\266\346\200\201", nullptr));
        pushButton_sensor_sta_get->setText(QCoreApplication::translate("factory1widget", "get", nullptr));
        groupBox_ircut->setTitle(QCoreApplication::translate("factory1widget", "IRCUT", nullptr));
        label_ircut_state->setText(QCoreApplication::translate("factory1widget", "\347\212\266\346\200\201", nullptr));
        pushButton_ircut_state_get->setText(QCoreApplication::translate("factory1widget", "get", nullptr));
        pushButton_ircut_state_set->setText(QCoreApplication::translate("factory1widget", "\345\210\207\346\215\242", nullptr));
        label->setText(QCoreApplication::translate("factory1widget", "SN", nullptr));
        pushButton_orb_sn_set->setText(QCoreApplication::translate("factory1widget", "set", nullptr));
        pushButton_orb_sn_get->setText(QCoreApplication::translate("factory1widget", "get", nullptr));
        label_light_sensitive_val->setText(QCoreApplication::translate("factory1widget", "\345\205\211\346\225\217\345\200\274", nullptr));
        pushButton_light_sensitive_get->setText(QCoreApplication::translate("factory1widget", "get", nullptr));
        pushButton_light_sensitive_open->setText(QCoreApplication::translate("factory1widget", "open", nullptr));
        groupBox_pir->setTitle(QCoreApplication::translate("factory1widget", "PIR", nullptr));
        label_pir_time->setText(QCoreApplication::translate("factory1widget", "\346\227\266\351\227\264", nullptr));
        pushButton_pir_time_get->setText(QCoreApplication::translate("factory1widget", "get", nullptr));
        pushButton_pir_time_set->setText(QCoreApplication::translate("factory1widget", "set", nullptr));
        label_pir_state->setText(QCoreApplication::translate("factory1widget", "\347\212\266\346\200\201", nullptr));
        pushButton_pir_state_get->setText(QCoreApplication::translate("factory1widget", "get", nullptr));
        pushButton_pir_state_set->setText(QCoreApplication::translate("factory1widget", "set", nullptr));
        label_pir_dist->setText(QCoreApplication::translate("factory1widget", "\350\267\235\347\246\273", nullptr));
        pushButton_pir_thresh_get->setText(QCoreApplication::translate("factory1widget", "get", nullptr));
        pushButton_pir_thresh_set->setText(QCoreApplication::translate("factory1widget", "set", nullptr));
        checkBox_led->setText(QCoreApplication::translate("factory1widget", "LED", nullptr));
        pushButton_peephole_reset->setText(QCoreApplication::translate("factory1widget", "reset", nullptr));
#if QT_CONFIG(shortcut)
        pushButton_peephole_reset->setShortcut(QCoreApplication::translate("factory1widget", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        label_version_wifi->setText(QCoreApplication::translate("factory1widget", "WIFI\347\211\210\346\234\254", nullptr));
        pushButton_version_wifi_get->setText(QCoreApplication::translate("factory1widget", "get", nullptr));
        label_version_system->setText(QCoreApplication::translate("factory1widget", "\347\263\273\347\273\237\347\211\210\346\234\254", nullptr));
        pushButton_version_system_get->setText(QCoreApplication::translate("factory1widget", "get", nullptr));
    } // retranslateUi

};

namespace Ui {
    class factory1widget: public Ui_factory1widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FACTORY1WIDGET_H
