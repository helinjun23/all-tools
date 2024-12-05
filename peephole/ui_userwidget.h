/********************************************************************************
** Form generated from reading UI file 'userwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERWIDGET_H
#define UI_USERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_userwidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_18;
    QLabel *label_user_data_user_typ;
    QComboBox *comboBox_user_data_user_typ;
    QHBoxLayout *horizontalLayout_17;
    QLabel *label_user_data_event_typ;
    QComboBox *comboBox_user_data_event_typ;
    QHBoxLayout *horizontalLayout_19;
    QLabel *label_user_data_auth_typ;
    QComboBox *comboBox_user_data_auth_typ;
    QHBoxLayout *horizontalLayout_20;
    QLabel *label_user_data_main_id;
    QSpinBox *spinBox_user_data_main_id;
    QHBoxLayout *horizontalLayout_21;
    QLabel *label_user_data_sub_id;
    QSpinBox *spinBox_user_data_sub_id;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_user_data_send;

    void setupUi(QWidget *userwidget)
    {
        if (userwidget->objectName().isEmpty())
            userwidget->setObjectName(QString::fromUtf8("userwidget"));
        userwidget->resize(230, 190);
        verticalLayout = new QVBoxLayout(userwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        label_user_data_user_typ = new QLabel(userwidget);
        label_user_data_user_typ->setObjectName(QString::fromUtf8("label_user_data_user_typ"));

        horizontalLayout_18->addWidget(label_user_data_user_typ);

        comboBox_user_data_user_typ = new QComboBox(userwidget);
        comboBox_user_data_user_typ->setObjectName(QString::fromUtf8("comboBox_user_data_user_typ"));

        horizontalLayout_18->addWidget(comboBox_user_data_user_typ);


        verticalLayout->addLayout(horizontalLayout_18);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        label_user_data_event_typ = new QLabel(userwidget);
        label_user_data_event_typ->setObjectName(QString::fromUtf8("label_user_data_event_typ"));

        horizontalLayout_17->addWidget(label_user_data_event_typ);

        comboBox_user_data_event_typ = new QComboBox(userwidget);
        comboBox_user_data_event_typ->setObjectName(QString::fromUtf8("comboBox_user_data_event_typ"));

        horizontalLayout_17->addWidget(comboBox_user_data_event_typ);


        verticalLayout->addLayout(horizontalLayout_17);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setObjectName(QString::fromUtf8("horizontalLayout_19"));
        label_user_data_auth_typ = new QLabel(userwidget);
        label_user_data_auth_typ->setObjectName(QString::fromUtf8("label_user_data_auth_typ"));

        horizontalLayout_19->addWidget(label_user_data_auth_typ);

        comboBox_user_data_auth_typ = new QComboBox(userwidget);
        comboBox_user_data_auth_typ->setObjectName(QString::fromUtf8("comboBox_user_data_auth_typ"));

        horizontalLayout_19->addWidget(comboBox_user_data_auth_typ);


        verticalLayout->addLayout(horizontalLayout_19);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setObjectName(QString::fromUtf8("horizontalLayout_20"));
        label_user_data_main_id = new QLabel(userwidget);
        label_user_data_main_id->setObjectName(QString::fromUtf8("label_user_data_main_id"));

        horizontalLayout_20->addWidget(label_user_data_main_id);

        spinBox_user_data_main_id = new QSpinBox(userwidget);
        spinBox_user_data_main_id->setObjectName(QString::fromUtf8("spinBox_user_data_main_id"));
        spinBox_user_data_main_id->setMaximum(255);

        horizontalLayout_20->addWidget(spinBox_user_data_main_id);


        verticalLayout->addLayout(horizontalLayout_20);

        horizontalLayout_21 = new QHBoxLayout();
        horizontalLayout_21->setObjectName(QString::fromUtf8("horizontalLayout_21"));
        label_user_data_sub_id = new QLabel(userwidget);
        label_user_data_sub_id->setObjectName(QString::fromUtf8("label_user_data_sub_id"));

        horizontalLayout_21->addWidget(label_user_data_sub_id);

        spinBox_user_data_sub_id = new QSpinBox(userwidget);
        spinBox_user_data_sub_id->setObjectName(QString::fromUtf8("spinBox_user_data_sub_id"));
        spinBox_user_data_sub_id->setMaximum(255);

        horizontalLayout_21->addWidget(spinBox_user_data_sub_id);


        verticalLayout->addLayout(horizontalLayout_21);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_user_data_send = new QPushButton(userwidget);
        pushButton_user_data_send->setObjectName(QString::fromUtf8("pushButton_user_data_send"));
        pushButton_user_data_send->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(pushButton_user_data_send);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(userwidget);

        QMetaObject::connectSlotsByName(userwidget);
    } // setupUi

    void retranslateUi(QWidget *userwidget)
    {
        userwidget->setWindowTitle(QCoreApplication::translate("userwidget", "\347\224\250\346\210\267\347\256\241\347\220\206", nullptr));
        label_user_data_user_typ->setText(QCoreApplication::translate("userwidget", "\347\224\250\346\210\267\347\261\273\345\236\213\357\274\232", nullptr));
        label_user_data_event_typ->setText(QCoreApplication::translate("userwidget", "\344\272\213\344\273\266\347\261\273\345\236\213\357\274\232", nullptr));
        label_user_data_auth_typ->setText(QCoreApplication::translate("userwidget", "\346\235\203\351\231\220\347\261\273\345\236\213\357\274\232", nullptr));
        label_user_data_main_id->setText(QCoreApplication::translate("userwidget", "\347\224\250\346\210\267\344\270\273ID\357\274\232", nullptr));
        label_user_data_sub_id->setText(QCoreApplication::translate("userwidget", "\347\224\250\346\210\267\345\255\220ID\357\274\232", nullptr));
        pushButton_user_data_send->setText(QCoreApplication::translate("userwidget", "\350\256\276\347\275\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class userwidget: public Ui_userwidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERWIDGET_H
