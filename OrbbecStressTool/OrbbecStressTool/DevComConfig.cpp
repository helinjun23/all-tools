#include "DevComConfig.h"

#include <QMap>
#include <QMessageBox>
#include <QDebug>

#include "common.h"

DevComConfig::DevComConfig(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setFixedSize(QSize(320, 320));

	initUi();

	spdlog::debug("{} thread id {}", __FUNCTION__, QThread::currentThreadId());
}

DevComConfig::~DevComConfig()
{
	for (auto& obj : serial_objs) {
		if (nullptr != obj) {
			delete obj;
			obj = nullptr;
		}
	}
}

void DevComConfig::showEvent(QShowEvent* event)
{
	getAvailableComPorts();
	QDialog::showEvent(event);
}

QStringList DevComConfig::getAvailableComPorts()
{
	serial_coms.clear();
	ui.comboBox_com_port->clear();
	QStringList comLists;
	int idx = 0;
	auto serialInfoList = SerialPortEx::FindAllComs();
	for (auto& serial : serialInfoList)
	{
		serial_coms.insert(idx++, serial.portName());
		QString strCom = QString("%1 #%2").arg(serial.portName()).arg(serial.description());
		comLists.push_back(strCom);
		ui.comboBox_com_port->addItem(strCom);
	}
	return comLists;
}

void DevComConfig::initUi()
{
	// 设备类型
	ui.comboBox_dev_typ->addItems({ "通信串口","调试串口","继电器串口" });
	serial_objs.resize(ui.comboBox_dev_typ->count());
	for (auto& obj : serial_objs) {
		if (nullptr == obj) {
			obj = new SerialPortEx(SerialPortEx::MODE_SYNC);
		}
	}

	connect(ui.comboBox_dev_typ, &QComboBox::currentTextChanged, this, [=] {
		int dev_idx = ui.comboBox_dev_typ->currentIndex();
		bool enable = true;
		if (serial_objs[dev_idx]) {
			enable = !serial_objs[dev_idx]->IsOpen();
		}
		enableComsUi(enable);
		restore_ui();

		}
	);

	// 串口号
	getAvailableComPorts();
	combos_group.push_back(ui.comboBox_com_port);

	// 波特率
	ui.comboBox_baude_rate->addItems({ "1200","2400","4800","9600","19200","38400","43000","56000","57600","115200","custom" });
	combos_group.push_back(ui.comboBox_baude_rate);
	ui.comboBox_baude_rate->setCurrentText("115200");

	// 校验位
	ui.comboBox_parity->addItems({ "NONE","EVEN","ODD","SPACE","MARK" });
	combos_group.push_back(ui.comboBox_parity);

	// 数据位
	ui.comboBox_data_bit->addItems({ "5","6","7","8" });
	ui.comboBox_data_bit->setCurrentIndex(3);
	combos_group.push_back(ui.comboBox_data_bit);

	// 停止位
	ui.comboBox_stop_bit->addItems({ "1","2","1.5" });
	combos_group.push_back(ui.comboBox_stop_bit);

	// 流控制 硬件流控、软件流控
	ui.comboBox_flow_ctrl->addItems({ "NONE","RTS/CTS", "XON/XOFF" });
	combos_group.push_back(ui.comboBox_flow_ctrl);

	// 打开/关闭串口
	connect(ui.pushButton_open_com, &QPushButton::clicked, this, [=] {
		on_pushButton_open_com_clicked();

		}
	);

	dev_comparas.resize(3);
	get_compara(dev_comparas[0]);
	dev_comparas[1] = dev_comparas[0];
	dev_comparas[2] = dev_comparas[0];

	// 从注册表获取上次串口号
	get_reg();
	emit ui.comboBox_dev_typ->currentTextChanged("");

	connect(this, &DevComConfig::send, this, &DevComConfig::send_data);
	connect(this, &DevComConfig::recv, this, &DevComConfig::recv_data);

	// 初始化日志


}

void DevComConfig::on_pushButton_open_com_clicked()
{
	int dev_type = ui.comboBox_dev_typ->currentIndex();
	if (ui.pushButton_open_com->text() == "打开") {
		get_compara(dev_comparas[dev_type]);
		bool bret = serial_objs[dev_type]->Open(dev_comparas[dev_type]);
		if (!bret) {
			QMessageBox::information(this, "提示", "打开串口失败");
			spdlog::warn("open com{} failed, devtyp {}", dev_comparas[dev_type].com_port, dev_type);
			return;
		}
		update_reg();
		enableComsUi(false);
		emit sync_com_stat(dev_type, true);
		spdlog::info("open com{} success, devtyp {} ", dev_comparas[dev_type].com_port, dev_type);
	}
	else {
		if (nullptr != serial_objs[dev_type]) {
			serial_objs[dev_type]->Close();
		}
		enableComsUi(true);
		emit sync_com_stat(dev_type, false);
		spdlog::info("close com success, devtyp {} ", dev_type);
	}
}

void DevComConfig::enableComsUi(bool bEnable)
{
	for (auto& combo : combos_group) {
		combo->setEnabled(bEnable);
	}

	ui.pushButton_open_com->setText(bEnable ? "打开" : "关闭");
}

void DevComConfig::get_compara(comconfigpara& para)
{
	int com_idx = ui.comboBox_com_port->currentIndex();

	para.com_port = serial_coms[com_idx].split("COM", Qt::SkipEmptyParts).at(0).toInt();
	para.baute = ui.comboBox_baude_rate->currentText().toInt();
	para.data_bit = ui.comboBox_data_bit->currentText().toInt();
	para.stop_bit = ui.comboBox_stop_bit->currentIndex() + 1;
	para.flow_ctrl = ui.comboBox_flow_ctrl->currentIndex();

	int parity_idx = ui.comboBox_parity->currentIndex();
	para.parity = parity_idx == 0 ? 0 : parity_idx + 1;

}

void DevComConfig::restore_ui()
{
	int dev_typ = ui.comboBox_dev_typ->currentIndex();
	comconfigpara& para = dev_comparas[dev_typ];
	//if (nullptr == serial_objs[dev_typ] || !serial_objs[dev_typ]->GetComParam(para)) {
	//	return;
	//}
	QString com_port(QString("COM%1").arg(para.com_port));
	for (int idx = 0; idx < serial_coms.size(); idx++) {
		if (com_port == serial_coms[idx]) {
			ui.comboBox_com_port->setCurrentIndex(idx);
			break;
		}
	}

	ui.comboBox_baude_rate->setCurrentText(QString("%1").arg(para.baute));
	ui.comboBox_data_bit->setCurrentText(QString("%1").arg(para.data_bit));
	ui.comboBox_stop_bit->setCurrentIndex(para.stop_bit - 1);
	ui.comboBox_parity->setCurrentIndex(para.parity != 0 ? para.parity - 1 : 0);
	ui.comboBox_flow_ctrl->setCurrentIndex(para.flow_ctrl);
}

void DevComConfig::update_reg()
{
	QSettings settings;
	settings.setValue("devCom/cmcc_com", dev_comparas[DEV_COM_TYP_CMMC].com_port);
	settings.setValue("devCom/debug_com", dev_comparas[DEV_COM_TYP_DEBUG].com_port);
	settings.setValue("devCom/relay_com", dev_comparas[DEV_COM_TYP_RELAY].com_port);
}

void DevComConfig::get_reg()
{
	QSettings settings;
	if (settings.contains("devCom/cmcc_com")) {
		dev_comparas[DEV_COM_TYP_CMMC].com_port = settings.value("devCom/cmcc_com").toInt();
		dev_comparas[DEV_COM_TYP_DEBUG].com_port = settings.value("devCom/debug_com").toInt();
		dev_comparas[DEV_COM_TYP_RELAY].com_port = settings.value("devCom/relay_com").toInt();
	}
	else {
		update_reg();
	}
}

int DevComConfig::reopen_com_serial(int dev_typ)
{
	comconfigpara com_para;
	serial_objs[dev_typ]->GetComParam(com_para);
	if (serial_objs[dev_typ]->ReOpen(com_para)) {
		return 0;
	}
	return -1;
}

void DevComConfig::send_data(int dev_typ, const QByteArray& data)
{
	spdlog::debug("{} thread id {}", __FUNCTION__, QThread::currentThreadId());
	if (nullptr != serial_objs[dev_typ]) {
		serial_objs[dev_typ]->sendData(data);
	}
}

void DevComConfig::recv_data(int dev_typ, QByteArray& data)
{
	spdlog::debug("{} thread id {}", __FUNCTION__, QThread::currentThreadId());
	if (nullptr != serial_objs[dev_typ]) {
		serial_objs[dev_typ]->recvData(data);
	}
}