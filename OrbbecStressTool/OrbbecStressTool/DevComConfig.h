#pragma once

#include "common.h"
#include "qserialex/serialportex.h"

#include <QDialog>

#include "ui_devcomconfig.h"
#pragma execution_character_set("utf-8")


class DevComConfig : public QDialog
{
	Q_OBJECT


public:
	DevComConfig(QWidget* parent = nullptr);
	~DevComConfig();

protected:
	void showEvent(QShowEvent* event) override;

	QStringList getAvailableComPorts();
private:
	void initUi();

	void on_pushButton_open_com_clicked();
	void enableComsUi(bool bEnable);

	void get_compara(comconfigpara& para);
	void restore_ui();

	void update_reg();
	void get_reg();

signals:
	void send(int dev_typ, const QByteArray&);
	void recv(int dev_typ, QByteArray&);

	// 同步com连接状态
	void sync_com_stat(int dev_typ, bool stat);

public slots:
	int reopen_com_serial(int dev_typ);
	void send_data(int dev_typ, const QByteArray& data);
	void recv_data(int dev_typ, QByteArray& data);

private:
	Ui::devcomconfig ui;

	QMap<int, QString> serial_coms; // 当前可用串口

	QVector<SerialPortEx*> serial_objs;
	QVector<comconfigpara> dev_comparas;
	QList<QComboBox*> combos_group;

};
