/********************************************************************************
** Form generated from reading UI file 'factory2widget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FACTORY2WIDGET_H
#define UI_FACTORY2WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_factory2widget
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_wifi;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_wifi_state_2;
    QLineEdit *lineEdit_wifi_state_2;
    QPushButton *pushButton_wifi_state_get_2;
    QPushButton *pushButton_wifi_state_set;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_wifi_ap;
    QLineEdit *lineEdit_wifi_ap;
    QPushButton *pushButton_wifi_ap_get;
    QPushButton *pushButton_wifi_ap_set;
    QGroupBox *groupBox_wire;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_wire_ip;
    QLineEdit *lineEdit_wire_ip;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_wire_ip_mask;
    QLineEdit *lineEdit_wire_ip_mask;
    QHBoxLayout *horizontalLayout_16;
    QLabel *label_wire_ip_gate;
    QLineEdit *lineEdit_wire_ip_gate;
    QHBoxLayout *horizontalLayout_17;
    QCheckBox *checkBox_factory_mode;
    QPushButton *pushButton_wire_ip_set;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_19;
    QPushButton *pushButton_media_audio_record;
    QPushButton *pushButton_audio_rec_play;
    QGroupBox *groupBox_stream;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_20;
    QLabel *label_media_stream_typ;
    QComboBox *comboBox_media_stream_typ;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_media_start;
    QHBoxLayout *horizontalLayout_18;
    QLabel *label_media_cur_frm;
    QLineEdit *lineEdit_media_cur_frm;
    QPushButton *pushButton_media_cur_frm_get;
    QHBoxLayout *horizontalLayout_22;
    QLabel *label_media_url;
    QLineEdit *lineEdit_media_url;
    QPushButton *pushButton_media_url_get;
    QHBoxLayout *horizontalLayout_21;
    QPushButton *pushButton_media_play;
    QPushButton *pushButton_media_snap;
    QPushButton *pushButton_media_record;

    void setupUi(QWidget *factory2widget)
    {
        if (factory2widget->objectName().isEmpty())
            factory2widget->setObjectName(QString::fromUtf8("factory2widget"));
        factory2widget->resize(400, 480);
        verticalLayout = new QVBoxLayout(factory2widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_wifi = new QGroupBox(factory2widget);
        groupBox_wifi->setObjectName(QString::fromUtf8("groupBox_wifi"));
        verticalLayout_2 = new QVBoxLayout(groupBox_wifi);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label_wifi_state_2 = new QLabel(groupBox_wifi);
        label_wifi_state_2->setObjectName(QString::fromUtf8("label_wifi_state_2"));

        horizontalLayout_12->addWidget(label_wifi_state_2);

        lineEdit_wifi_state_2 = new QLineEdit(groupBox_wifi);
        lineEdit_wifi_state_2->setObjectName(QString::fromUtf8("lineEdit_wifi_state_2"));

        horizontalLayout_12->addWidget(lineEdit_wifi_state_2);

        pushButton_wifi_state_get_2 = new QPushButton(groupBox_wifi);
        pushButton_wifi_state_get_2->setObjectName(QString::fromUtf8("pushButton_wifi_state_get_2"));

        horizontalLayout_12->addWidget(pushButton_wifi_state_get_2);

        pushButton_wifi_state_set = new QPushButton(groupBox_wifi);
        pushButton_wifi_state_set->setObjectName(QString::fromUtf8("pushButton_wifi_state_set"));

        horizontalLayout_12->addWidget(pushButton_wifi_state_set);


        verticalLayout_2->addLayout(horizontalLayout_12);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        label_wifi_ap = new QLabel(groupBox_wifi);
        label_wifi_ap->setObjectName(QString::fromUtf8("label_wifi_ap"));

        horizontalLayout_13->addWidget(label_wifi_ap);

        lineEdit_wifi_ap = new QLineEdit(groupBox_wifi);
        lineEdit_wifi_ap->setObjectName(QString::fromUtf8("lineEdit_wifi_ap"));

        horizontalLayout_13->addWidget(lineEdit_wifi_ap);

        pushButton_wifi_ap_get = new QPushButton(groupBox_wifi);
        pushButton_wifi_ap_get->setObjectName(QString::fromUtf8("pushButton_wifi_ap_get"));

        horizontalLayout_13->addWidget(pushButton_wifi_ap_get);

        pushButton_wifi_ap_set = new QPushButton(groupBox_wifi);
        pushButton_wifi_ap_set->setObjectName(QString::fromUtf8("pushButton_wifi_ap_set"));

        horizontalLayout_13->addWidget(pushButton_wifi_ap_set);


        verticalLayout_2->addLayout(horizontalLayout_13);


        verticalLayout->addWidget(groupBox_wifi);

        groupBox_wire = new QGroupBox(factory2widget);
        groupBox_wire->setObjectName(QString::fromUtf8("groupBox_wire"));
        groupBox_wire->setCheckable(true);
        groupBox_wire->setChecked(false);
        verticalLayout_4 = new QVBoxLayout(groupBox_wire);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        label_wire_ip = new QLabel(groupBox_wire);
        label_wire_ip->setObjectName(QString::fromUtf8("label_wire_ip"));

        horizontalLayout_14->addWidget(label_wire_ip);

        lineEdit_wire_ip = new QLineEdit(groupBox_wire);
        lineEdit_wire_ip->setObjectName(QString::fromUtf8("lineEdit_wire_ip"));

        horizontalLayout_14->addWidget(lineEdit_wire_ip);


        verticalLayout_4->addLayout(horizontalLayout_14);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        label_wire_ip_mask = new QLabel(groupBox_wire);
        label_wire_ip_mask->setObjectName(QString::fromUtf8("label_wire_ip_mask"));

        horizontalLayout_15->addWidget(label_wire_ip_mask);

        lineEdit_wire_ip_mask = new QLineEdit(groupBox_wire);
        lineEdit_wire_ip_mask->setObjectName(QString::fromUtf8("lineEdit_wire_ip_mask"));

        horizontalLayout_15->addWidget(lineEdit_wire_ip_mask);


        verticalLayout_4->addLayout(horizontalLayout_15);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        label_wire_ip_gate = new QLabel(groupBox_wire);
        label_wire_ip_gate->setObjectName(QString::fromUtf8("label_wire_ip_gate"));

        horizontalLayout_16->addWidget(label_wire_ip_gate);

        lineEdit_wire_ip_gate = new QLineEdit(groupBox_wire);
        lineEdit_wire_ip_gate->setObjectName(QString::fromUtf8("lineEdit_wire_ip_gate"));

        horizontalLayout_16->addWidget(lineEdit_wire_ip_gate);


        verticalLayout_4->addLayout(horizontalLayout_16);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        checkBox_factory_mode = new QCheckBox(groupBox_wire);
        checkBox_factory_mode->setObjectName(QString::fromUtf8("checkBox_factory_mode"));

        horizontalLayout_17->addWidget(checkBox_factory_mode);

        pushButton_wire_ip_set = new QPushButton(groupBox_wire);
        pushButton_wire_ip_set->setObjectName(QString::fromUtf8("pushButton_wire_ip_set"));

        horizontalLayout_17->addWidget(pushButton_wire_ip_set);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_17->addItem(horizontalSpacer);


        verticalLayout_4->addLayout(horizontalLayout_17);


        verticalLayout->addWidget(groupBox_wire);

        groupBox = new QGroupBox(factory2widget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_19 = new QHBoxLayout(groupBox);
        horizontalLayout_19->setObjectName(QString::fromUtf8("horizontalLayout_19"));
        horizontalLayout_19->setContentsMargins(9, 9, 9, 9);
        pushButton_media_audio_record = new QPushButton(groupBox);
        pushButton_media_audio_record->setObjectName(QString::fromUtf8("pushButton_media_audio_record"));

        horizontalLayout_19->addWidget(pushButton_media_audio_record);

        pushButton_audio_rec_play = new QPushButton(groupBox);
        pushButton_audio_rec_play->setObjectName(QString::fromUtf8("pushButton_audio_rec_play"));

        horizontalLayout_19->addWidget(pushButton_audio_rec_play);


        verticalLayout->addWidget(groupBox);

        groupBox_stream = new QGroupBox(factory2widget);
        groupBox_stream->setObjectName(QString::fromUtf8("groupBox_stream"));
        verticalLayout_5 = new QVBoxLayout(groupBox_stream);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setObjectName(QString::fromUtf8("horizontalLayout_20"));
        label_media_stream_typ = new QLabel(groupBox_stream);
        label_media_stream_typ->setObjectName(QString::fromUtf8("label_media_stream_typ"));

        horizontalLayout_20->addWidget(label_media_stream_typ);

        comboBox_media_stream_typ = new QComboBox(groupBox_stream);
        comboBox_media_stream_typ->setObjectName(QString::fromUtf8("comboBox_media_stream_typ"));

        horizontalLayout_20->addWidget(comboBox_media_stream_typ);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_20->addItem(horizontalSpacer_2);

        pushButton_media_start = new QPushButton(groupBox_stream);
        pushButton_media_start->setObjectName(QString::fromUtf8("pushButton_media_start"));

        horizontalLayout_20->addWidget(pushButton_media_start);


        verticalLayout_5->addLayout(horizontalLayout_20);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        label_media_cur_frm = new QLabel(groupBox_stream);
        label_media_cur_frm->setObjectName(QString::fromUtf8("label_media_cur_frm"));

        horizontalLayout_18->addWidget(label_media_cur_frm);

        lineEdit_media_cur_frm = new QLineEdit(groupBox_stream);
        lineEdit_media_cur_frm->setObjectName(QString::fromUtf8("lineEdit_media_cur_frm"));
        lineEdit_media_cur_frm->setReadOnly(true);

        horizontalLayout_18->addWidget(lineEdit_media_cur_frm);

        pushButton_media_cur_frm_get = new QPushButton(groupBox_stream);
        pushButton_media_cur_frm_get->setObjectName(QString::fromUtf8("pushButton_media_cur_frm_get"));

        horizontalLayout_18->addWidget(pushButton_media_cur_frm_get);


        verticalLayout_5->addLayout(horizontalLayout_18);

        horizontalLayout_22 = new QHBoxLayout();
        horizontalLayout_22->setObjectName(QString::fromUtf8("horizontalLayout_22"));
        label_media_url = new QLabel(groupBox_stream);
        label_media_url->setObjectName(QString::fromUtf8("label_media_url"));

        horizontalLayout_22->addWidget(label_media_url);

        lineEdit_media_url = new QLineEdit(groupBox_stream);
        lineEdit_media_url->setObjectName(QString::fromUtf8("lineEdit_media_url"));
        lineEdit_media_url->setReadOnly(true);

        horizontalLayout_22->addWidget(lineEdit_media_url);

        pushButton_media_url_get = new QPushButton(groupBox_stream);
        pushButton_media_url_get->setObjectName(QString::fromUtf8("pushButton_media_url_get"));

        horizontalLayout_22->addWidget(pushButton_media_url_get);


        verticalLayout_5->addLayout(horizontalLayout_22);

        horizontalLayout_21 = new QHBoxLayout();
        horizontalLayout_21->setObjectName(QString::fromUtf8("horizontalLayout_21"));
        pushButton_media_play = new QPushButton(groupBox_stream);
        pushButton_media_play->setObjectName(QString::fromUtf8("pushButton_media_play"));
        pushButton_media_play->setEnabled(true);

        horizontalLayout_21->addWidget(pushButton_media_play);

        pushButton_media_snap = new QPushButton(groupBox_stream);
        pushButton_media_snap->setObjectName(QString::fromUtf8("pushButton_media_snap"));
        pushButton_media_snap->setEnabled(true);

        horizontalLayout_21->addWidget(pushButton_media_snap);

        pushButton_media_record = new QPushButton(groupBox_stream);
        pushButton_media_record->setObjectName(QString::fromUtf8("pushButton_media_record"));
        pushButton_media_record->setEnabled(true);

        horizontalLayout_21->addWidget(pushButton_media_record);


        verticalLayout_5->addLayout(horizontalLayout_21);


        verticalLayout->addWidget(groupBox_stream);


        retranslateUi(factory2widget);

        QMetaObject::connectSlotsByName(factory2widget);
    } // setupUi

    void retranslateUi(QWidget *factory2widget)
    {
        factory2widget->setWindowTitle(QCoreApplication::translate("factory2widget", "\347\224\237\344\272\2472", nullptr));
        groupBox_wifi->setTitle(QCoreApplication::translate("factory2widget", "WIFI", nullptr));
        label_wifi_state_2->setText(QCoreApplication::translate("factory2widget", "\347\212\266\346\200\201", nullptr));
        pushButton_wifi_state_get_2->setText(QCoreApplication::translate("factory2widget", "get", nullptr));
        pushButton_wifi_state_set->setText(QCoreApplication::translate("factory2widget", "set", nullptr));
        label_wifi_ap->setText(QCoreApplication::translate("factory2widget", "AP", nullptr));
        pushButton_wifi_ap_get->setText(QCoreApplication::translate("factory2widget", "get", nullptr));
        pushButton_wifi_ap_set->setText(QCoreApplication::translate("factory2widget", "set", nullptr));
        groupBox_wire->setTitle(QCoreApplication::translate("factory2widget", "\346\234\211\347\272\277\347\275\221\347\273\234", nullptr));
        label_wire_ip->setText(QCoreApplication::translate("factory2widget", "IP(\347\202\271\345\210\206\345\210\266)", nullptr));
        label_wire_ip_mask->setText(QCoreApplication::translate("factory2widget", "\346\216\251\347\240\201(\347\202\271\345\210\206\345\210\266)", nullptr));
        label_wire_ip_gate->setText(QCoreApplication::translate("factory2widget", "\347\275\221\345\205\263(\347\202\271\345\210\206\345\210\266)", nullptr));
        checkBox_factory_mode->setText(QCoreApplication::translate("factory2widget", "\345\267\245\345\216\202\346\250\241\345\274\217", nullptr));
        pushButton_wire_ip_set->setText(QCoreApplication::translate("factory2widget", "set", nullptr));
        pushButton_media_audio_record->setText(QCoreApplication::translate("factory2widget", "AudioRec", nullptr));
        pushButton_audio_rec_play->setText(QCoreApplication::translate("factory2widget", "AudioPlay", nullptr));
        groupBox_stream->setTitle(QCoreApplication::translate("factory2widget", "stream", nullptr));
        label_media_stream_typ->setText(QCoreApplication::translate("factory2widget", "\347\240\201\346\265\201", nullptr));
        pushButton_media_start->setText(QCoreApplication::translate("factory2widget", "start", nullptr));
        label_media_cur_frm->setText(QCoreApplication::translate("factory2widget", "\345\256\236\346\227\266\345\270\247\346\225\260", nullptr));
        pushButton_media_cur_frm_get->setText(QCoreApplication::translate("factory2widget", "get", nullptr));
        label_media_url->setText(QCoreApplication::translate("factory2widget", "url", nullptr));
        pushButton_media_url_get->setText(QCoreApplication::translate("factory2widget", "get", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_media_play->setToolTip(QCoreApplication::translate("factory2widget", "\351\234\200\350\246\201\350\277\236\346\216\245\344\270\216\350\256\276\345\244\207\345\220\214\344\270\200\344\270\252WIFI", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_media_play->setText(QCoreApplication::translate("factory2widget", "play", nullptr));
        pushButton_media_snap->setText(QCoreApplication::translate("factory2widget", "\351\207\207\345\233\276", nullptr));
        pushButton_media_record->setText(QCoreApplication::translate("factory2widget", "Record", nullptr));
    } // retranslateUi

};

namespace Ui {
    class factory2widget: public Ui_factory2widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FACTORY2WIDGET_H
