/********************************************************************************
** Form generated from reading UI file 'eventwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EVENTWIDGET_H
#define UI_EVENTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_eventwidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_12;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label;
    QComboBox *comboBox_event_type;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_2;
    QComboBox *comboBox_event_code;
    QHBoxLayout *horizontalLayout_13;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_event_open_typ;
    QComboBox *comboBox_event_open_typ;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_event_user_id;
    QSpinBox *spinBox_key_id;
    QHBoxLayout *horizontalLayout_16;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_3;
    QSpinBox *spinBox_BAT1_percent;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_4;
    QSpinBox *spinBox_BAT2_percent;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_event_id;
    QSpinBox *spinBox_event_id;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_event_action;
    QComboBox *comboBox_event_action;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_event_action_param;
    QSpinBox *spinBox_event_action_param;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *checkBox_event_offline;
    QCheckBox *checkBox_timestamp;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *pushButton_event_send;

    void setupUi(QWidget *eventwidget)
    {
        if (eventwidget->objectName().isEmpty())
            eventwidget->setObjectName(QString::fromUtf8("eventwidget"));
        eventwidget->resize(288, 272);
        verticalLayout = new QVBoxLayout(eventwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label = new QLabel(eventwidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_7->addWidget(label);

        comboBox_event_type = new QComboBox(eventwidget);
        comboBox_event_type->setObjectName(QString::fromUtf8("comboBox_event_type"));

        horizontalLayout_7->addWidget(comboBox_event_type);


        horizontalLayout_12->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_2 = new QLabel(eventwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_8->addWidget(label_2);

        comboBox_event_code = new QComboBox(eventwidget);
        comboBox_event_code->setObjectName(QString::fromUtf8("comboBox_event_code"));

        horizontalLayout_8->addWidget(comboBox_event_code);


        horizontalLayout_12->addLayout(horizontalLayout_8);


        verticalLayout->addLayout(horizontalLayout_12);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_event_open_typ = new QLabel(eventwidget);
        label_event_open_typ->setObjectName(QString::fromUtf8("label_event_open_typ"));

        horizontalLayout_5->addWidget(label_event_open_typ);

        comboBox_event_open_typ = new QComboBox(eventwidget);
        comboBox_event_open_typ->setObjectName(QString::fromUtf8("comboBox_event_open_typ"));

        horizontalLayout_5->addWidget(comboBox_event_open_typ);


        horizontalLayout_13->addLayout(horizontalLayout_5);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_event_user_id = new QLabel(eventwidget);
        label_event_user_id->setObjectName(QString::fromUtf8("label_event_user_id"));

        horizontalLayout_9->addWidget(label_event_user_id);

        spinBox_key_id = new QSpinBox(eventwidget);
        spinBox_key_id->setObjectName(QString::fromUtf8("spinBox_key_id"));

        horizontalLayout_9->addWidget(spinBox_key_id);


        horizontalLayout_13->addLayout(horizontalLayout_9);


        verticalLayout->addLayout(horizontalLayout_13);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        label_3 = new QLabel(eventwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_14->addWidget(label_3);

        spinBox_BAT1_percent = new QSpinBox(eventwidget);
        spinBox_BAT1_percent->setObjectName(QString::fromUtf8("spinBox_BAT1_percent"));
        spinBox_BAT1_percent->setMaximum(100);

        horizontalLayout_14->addWidget(spinBox_BAT1_percent);


        horizontalLayout_16->addLayout(horizontalLayout_14);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        label_4 = new QLabel(eventwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_15->addWidget(label_4);

        spinBox_BAT2_percent = new QSpinBox(eventwidget);
        spinBox_BAT2_percent->setObjectName(QString::fromUtf8("spinBox_BAT2_percent"));
        spinBox_BAT2_percent->setMaximum(100);

        horizontalLayout_15->addWidget(spinBox_BAT2_percent);


        horizontalLayout_16->addLayout(horizontalLayout_15);


        verticalLayout->addLayout(horizontalLayout_16);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_event_id = new QLabel(eventwidget);
        label_event_id->setObjectName(QString::fromUtf8("label_event_id"));

        horizontalLayout_6->addWidget(label_event_id);

        spinBox_event_id = new QSpinBox(eventwidget);
        spinBox_event_id->setObjectName(QString::fromUtf8("spinBox_event_id"));
        spinBox_event_id->setMaximum(65535);

        horizontalLayout_6->addWidget(spinBox_event_id);


        verticalLayout->addLayout(horizontalLayout_6);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_event_action = new QLabel(eventwidget);
        label_event_action->setObjectName(QString::fromUtf8("label_event_action"));

        horizontalLayout_10->addWidget(label_event_action);

        comboBox_event_action = new QComboBox(eventwidget);
        comboBox_event_action->setObjectName(QString::fromUtf8("comboBox_event_action"));

        horizontalLayout_10->addWidget(comboBox_event_action);


        verticalLayout_3->addLayout(horizontalLayout_10);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_event_action_param = new QLabel(eventwidget);
        label_event_action_param->setObjectName(QString::fromUtf8("label_event_action_param"));

        horizontalLayout_11->addWidget(label_event_action_param);

        spinBox_event_action_param = new QSpinBox(eventwidget);
        spinBox_event_action_param->setObjectName(QString::fromUtf8("spinBox_event_action_param"));
        spinBox_event_action_param->setMaximum(256);

        horizontalLayout_11->addWidget(spinBox_event_action_param);


        verticalLayout_3->addLayout(horizontalLayout_11);


        verticalLayout->addLayout(verticalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        checkBox_event_offline = new QCheckBox(eventwidget);
        checkBox_event_offline->setObjectName(QString::fromUtf8("checkBox_event_offline"));

        horizontalLayout_2->addWidget(checkBox_event_offline);


        verticalLayout->addLayout(horizontalLayout_2);

        checkBox_timestamp = new QCheckBox(eventwidget);
        checkBox_timestamp->setObjectName(QString::fromUtf8("checkBox_timestamp"));
        checkBox_timestamp->setChecked(true);

        verticalLayout->addWidget(checkBox_timestamp);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_7);

        pushButton_event_send = new QPushButton(eventwidget);
        pushButton_event_send->setObjectName(QString::fromUtf8("pushButton_event_send"));
        pushButton_event_send->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(pushButton_event_send);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(eventwidget);

        QMetaObject::connectSlotsByName(eventwidget);
    } // setupUi

    void retranslateUi(QWidget *eventwidget)
    {
        eventwidget->setWindowTitle(QCoreApplication::translate("eventwidget", "\344\272\213\344\273\266\347\256\241\347\220\206", nullptr));
        label->setText(QCoreApplication::translate("eventwidget", "\344\272\213\344\273\266\347\261\273\345\236\213\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("eventwidget", "\344\272\213\344\273\266\347\240\201\357\274\232", nullptr));
        label_event_open_typ->setText(QCoreApplication::translate("eventwidget", "\345\274\200\351\227\250\347\261\273\345\236\213\357\274\232", nullptr));
        label_event_user_id->setText(QCoreApplication::translate("eventwidget", "\351\222\245\345\214\231ID\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("eventwidget", "\345\244\247\347\224\265\346\261\240\347\224\265\351\207\217\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("eventwidget", "\345\260\217\347\224\265\346\261\240\347\224\265\351\207\217\357\274\232", nullptr));
        label_event_id->setText(QCoreApplication::translate("eventwidget", "\344\272\213\344\273\266ID\357\274\232", nullptr));
        spinBox_event_id->setSuffix(QString());
        spinBox_event_id->setPrefix(QString());
        label_event_action->setText(QCoreApplication::translate("eventwidget", "\344\272\213\344\273\266\350\241\214\344\270\272\347\261\273\345\236\213\357\274\232", nullptr));
        label_event_action_param->setText(QCoreApplication::translate("eventwidget", "\344\272\213\344\273\266\350\241\214\344\270\272\345\217\202\346\225\260\357\274\232", nullptr));
        checkBox_event_offline->setText(QCoreApplication::translate("eventwidget", "\347\246\273\347\272\277\344\272\213\344\273\266", nullptr));
        checkBox_timestamp->setText(QCoreApplication::translate("eventwidget", "\345\220\257\347\224\250\346\227\266\351\227\264\346\210\263", nullptr));
        pushButton_event_send->setText(QCoreApplication::translate("eventwidget", "\350\256\276\347\275\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class eventwidget: public Ui_eventwidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EVENTWIDGET_H
