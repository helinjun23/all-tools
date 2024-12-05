/********************************************************************************
** Form generated from reading UI file 'cloudwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLOUDWIDGET_H
#define UI_CLOUDWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_cloudwidget
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_ali_key;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_ali_product_key;
    QLineEdit *lineEdit_product_key;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_ali_product_secret;
    QLineEdit *lineEdit_product_secret;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_ali_device_name;
    QLineEdit *lineEdit_device_name;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_ali_device_secret;
    QLineEdit *lineEdit_device_secret;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_key_set;
    QPushButton *pushButton_key_get;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_data_pass;
    QLineEdit *lineEdit_data_pass;
    QPushButton *pushButton_data_pass;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_protocol_extend;
    QLineEdit *lineEdit_protocol_extend;
    QPushButton *pushButton_protocol_extend_send;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *cloudwidget)
    {
        if (cloudwidget->objectName().isEmpty())
            cloudwidget->setObjectName(QString::fromUtf8("cloudwidget"));
        cloudwidget->resize(337, 260);
        verticalLayout_2 = new QVBoxLayout(cloudwidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox_ali_key = new QGroupBox(cloudwidget);
        groupBox_ali_key->setObjectName(QString::fromUtf8("groupBox_ali_key"));
        verticalLayout = new QVBoxLayout(groupBox_ali_key);
        verticalLayout->setSpacing(2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(3, 3, 3, 3);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_ali_product_key = new QLabel(groupBox_ali_key);
        label_ali_product_key->setObjectName(QString::fromUtf8("label_ali_product_key"));

        horizontalLayout->addWidget(label_ali_product_key);

        lineEdit_product_key = new QLineEdit(groupBox_ali_key);
        lineEdit_product_key->setObjectName(QString::fromUtf8("lineEdit_product_key"));

        horizontalLayout->addWidget(lineEdit_product_key);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_ali_product_secret = new QLabel(groupBox_ali_key);
        label_ali_product_secret->setObjectName(QString::fromUtf8("label_ali_product_secret"));

        horizontalLayout_2->addWidget(label_ali_product_secret);

        lineEdit_product_secret = new QLineEdit(groupBox_ali_key);
        lineEdit_product_secret->setObjectName(QString::fromUtf8("lineEdit_product_secret"));

        horizontalLayout_2->addWidget(lineEdit_product_secret);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_ali_device_name = new QLabel(groupBox_ali_key);
        label_ali_device_name->setObjectName(QString::fromUtf8("label_ali_device_name"));

        horizontalLayout_3->addWidget(label_ali_device_name);

        lineEdit_device_name = new QLineEdit(groupBox_ali_key);
        lineEdit_device_name->setObjectName(QString::fromUtf8("lineEdit_device_name"));

        horizontalLayout_3->addWidget(lineEdit_device_name);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_ali_device_secret = new QLabel(groupBox_ali_key);
        label_ali_device_secret->setObjectName(QString::fromUtf8("label_ali_device_secret"));

        horizontalLayout_4->addWidget(label_ali_device_secret);

        lineEdit_device_secret = new QLineEdit(groupBox_ali_key);
        lineEdit_device_secret->setObjectName(QString::fromUtf8("lineEdit_device_secret"));

        horizontalLayout_4->addWidget(lineEdit_device_secret);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);

        pushButton_key_set = new QPushButton(groupBox_ali_key);
        pushButton_key_set->setObjectName(QString::fromUtf8("pushButton_key_set"));
        pushButton_key_set->setMinimumSize(QSize(0, 30));

        horizontalLayout_5->addWidget(pushButton_key_set);

        pushButton_key_get = new QPushButton(groupBox_ali_key);
        pushButton_key_get->setObjectName(QString::fromUtf8("pushButton_key_get"));
        pushButton_key_get->setMinimumSize(QSize(0, 30));

        horizontalLayout_5->addWidget(pushButton_key_get);


        verticalLayout->addLayout(horizontalLayout_5);


        verticalLayout_2->addWidget(groupBox_ali_key);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_data_pass = new QLabel(cloudwidget);
        label_data_pass->setObjectName(QString::fromUtf8("label_data_pass"));

        horizontalLayout_6->addWidget(label_data_pass);

        lineEdit_data_pass = new QLineEdit(cloudwidget);
        lineEdit_data_pass->setObjectName(QString::fromUtf8("lineEdit_data_pass"));

        horizontalLayout_6->addWidget(lineEdit_data_pass);

        pushButton_data_pass = new QPushButton(cloudwidget);
        pushButton_data_pass->setObjectName(QString::fromUtf8("pushButton_data_pass"));
        pushButton_data_pass->setMinimumSize(QSize(0, 30));

        horizontalLayout_6->addWidget(pushButton_data_pass);


        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_protocol_extend = new QLabel(cloudwidget);
        label_protocol_extend->setObjectName(QString::fromUtf8("label_protocol_extend"));

        horizontalLayout_7->addWidget(label_protocol_extend);

        lineEdit_protocol_extend = new QLineEdit(cloudwidget);
        lineEdit_protocol_extend->setObjectName(QString::fromUtf8("lineEdit_protocol_extend"));

        horizontalLayout_7->addWidget(lineEdit_protocol_extend);

        pushButton_protocol_extend_send = new QPushButton(cloudwidget);
        pushButton_protocol_extend_send->setObjectName(QString::fromUtf8("pushButton_protocol_extend_send"));
        pushButton_protocol_extend_send->setMinimumSize(QSize(0, 30));

        horizontalLayout_7->addWidget(pushButton_protocol_extend_send);


        verticalLayout_2->addLayout(horizontalLayout_7);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);


        retranslateUi(cloudwidget);

        QMetaObject::connectSlotsByName(cloudwidget);
    } // setupUi

    void retranslateUi(QWidget *cloudwidget)
    {
        cloudwidget->setWindowTitle(QCoreApplication::translate("cloudwidget", "\344\272\221\347\256\241\347\220\206", nullptr));
        groupBox_ali_key->setTitle(QCoreApplication::translate("cloudwidget", "\345\233\233\345\205\203\347\273\204", nullptr));
        label_ali_product_key->setText(QCoreApplication::translate("cloudwidget", "product_key\357\274\232", nullptr));
        label_ali_product_secret->setText(QCoreApplication::translate("cloudwidget", "product_secret\357\274\232", nullptr));
        label_ali_device_name->setText(QCoreApplication::translate("cloudwidget", "device_name\357\274\232", nullptr));
        label_ali_device_secret->setText(QCoreApplication::translate("cloudwidget", "device_secret\357\274\232", nullptr));
        pushButton_key_set->setText(QCoreApplication::translate("cloudwidget", "\350\256\276\347\275\256", nullptr));
        pushButton_key_get->setText(QCoreApplication::translate("cloudwidget", "\350\216\267\345\217\226", nullptr));
        label_data_pass->setText(QCoreApplication::translate("cloudwidget", "\346\225\260\346\215\256\351\200\217\344\274\240\357\274\232", nullptr));
        pushButton_data_pass->setText(QCoreApplication::translate("cloudwidget", "\350\256\276\347\275\256", nullptr));
        label_protocol_extend->setText(QCoreApplication::translate("cloudwidget", "\345\215\217\350\256\256\346\211\251\345\261\225\357\274\232", nullptr));
        pushButton_protocol_extend_send->setText(QCoreApplication::translate("cloudwidget", "\350\256\276\347\275\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class cloudwidget: public Ui_cloudwidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLOUDWIDGET_H
