#include "OrbbecFaceDemoTool.h"
#include "protocols/common.h"
#include <QByteArray>
#include <QTimer>

using namespace orbprotocol;

OrbbecFaceDemoTool::OrbbecFaceDemoTool(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	initUi();
	
	SPDLOG_DEBUG("thread id {}", QThread::currentThreadId());
}

OrbbecFaceDemoTool::~OrbbecFaceDemoTool()
{
	if (nullptr != m_serial_dev)
	{
		m_serial_dev->GetSerialObj()->Close();
		delete m_serial_dev, m_serial_dev = nullptr;
	}
}

void OrbbecFaceDemoTool::on_pushButton_picture_snap_char_clicked()
{
	
}

void OrbbecFaceDemoTool::on_pushButton_send_picture_char_file_clicked()
{

}

void OrbbecFaceDemoTool::on_pushButton_send_picture_snap_char_clicked()
{

}

void OrbbecFaceDemoTool::on_pushButton_recong_level_set_clicked()
{
	AsyncProc([=]
		{
			uint8_t level = ui.spinBox_recong_level->value();
			m_protocol->SetRecongLevel(level);
		}
	);
}

void OrbbecFaceDemoTool::on_pushButton_recong_level_get_clicked()
{
	AsyncProc([=] 
		{
			uint8_t level;
			if (0 == m_protocol->GetRecongLevel(level)) {
				ui.spinBox_recong_level->setValue(level);
			}
		}
	);
}

void OrbbecFaceDemoTool::on_pushButton_recong_level_reset_clicked()
{
	AsyncProc([=]
		{
			m_protocol->ResetRecongLevel();
		}
	);
}

void OrbbecFaceDemoTool::on_pushButton_live_level_set_clicked()
{
	AsyncProc([=]
		{
			uint8_t level = ui.spinBox_live_level->value();
			m_protocol->SetLiveLevel(level);
		}
	);
}

void OrbbecFaceDemoTool::on_pushButton_live_level_get_clicked()
{
	AsyncProc([=]
		{
			uint8_t level;
			if (0 == m_protocol->GetLiveLevel(level)) {
				ui.spinBox_live_level->setValue(level);
			}
		}
	);
}

void OrbbecFaceDemoTool::on_pushButton_live_level_reset_clicked()
{
	AsyncProc([=]
		{
			m_protocol->ResetLiveLevel();
		}
	);
}

void OrbbecFaceDemoTool::on_checkBox_repeat_register_stateChanged(int change)
{

}

void OrbbecFaceDemoTool::on_pushButton_repeat_register_get_clicked()
{

}

void OrbbecFaceDemoTool::on_pushButton_get_door_lock_module_ver_clicked()
{
	AsyncProc([=]
		{
			std::string ver;
			m_protocol->GetModuleVer(ver);
			ui.label_door_lock_module_ver2->setText(ver.c_str());
		}
	);
}

void OrbbecFaceDemoTool::on_pushButton_get_door_lock_alg_board_ver_clicked()
{
	AsyncProc([=]
		{
			std::string ver; 
			m_protocol->GetAlgBoardVer(ver);
			ui.label_door_lock_alg_board_ver2->setText(ver.c_str());
		}
	);
}

void OrbbecFaceDemoTool::on_pushButton_face_demo_mode_clicked()
{

}

void OrbbecFaceDemoTool::on_pushButton_person_register_clicked()
{

}

void OrbbecFaceDemoTool::on_pushButton_person_del_clicked()
{

}

void OrbbecFaceDemoTool::on_pushButton_person_del_all_clicked()
{

}

void OrbbecFaceDemoTool::on_pushButton_person_lookup_all_clicked()
{

}

void OrbbecFaceDemoTool::on_pushButton_person_recongnize_clicked()
{

}

void OrbbecFaceDemoTool::on_pushButton_picture_snap_clicked()
{
	CaptureProces(false);
	ui.label_picture_snap->clear();
	AsyncProc([=]
		{
			SnapParam para;
			memset(&para, 0, sizeof(para));

			para.com_bauterate = 115200;
			para.snap_bauterate = 1500000;
			if (ui.groupBox_snap_param->isChecked()) {
				para.img_type = ui.comboBox_snap_img_typ->currentIndex();
				para.gain_val = ui.spinBox_gain_val->value();
				para.ensnap_param = true;				
			}

			m_protocol->SetStartCapture(para);

			QByteArrayList img_data;
			m_protocol->GetCaptrueData(img_data);
			if (img_data.isEmpty()) {
				return;
			}

			QByteArray outImg;
			if (-1 == m_protocol->ParseCaptrueData(img_data, outImg)) {
				return;
			}

			//显示界面
			constexpr int pic_w = 544;
			constexpr int pic_h = 640;
			ShowPic((unsigned char*)outImg.data(), outImg.size(), pic_w, pic_h);

			//保存文件
			QDateTime localTime = QDateTime::currentDateTime();
			sprintf(m_savePicPath, "./PicData/pic_%d_%d_%d_%d_%d_%d.jpg", localTime.date().year(), localTime.date().month(), localTime.date().day(),
				localTime.time().hour(), localTime.time().minute(), localTime.time().second());

			QFile picfile(m_savePicPath);
			picfile.open(QFile::WriteOnly);
			QDataStream datastream(&picfile);
			datastream.writeRawData(outImg, outImg.size());
			picfile.close();

			m_protocol->SetStopCapture(para);
		}
	);
	CaptureProces(true);

}

void OrbbecFaceDemoTool::autoCloseDevCom()
{
	if (m_serial_dev->GetSerialObj()->IsOpen()) {
		// 获取当前显示串口号
		QString strCom = ui.comboBox_door_lock_com_port->currentText();
		strCom = strCom.split(" ", Qt::SkipEmptyParts).at(0);
		comconfigpara comPara;
		m_serial_dev->GetSerialObj()->GetComParam(comPara);
		if (strCom.indexOf(comPara.com_port) == -1) // 工作中USB串口已被拔出，需要自动关闭串口
		{
			m_serial_dev->GetSerialObj()->Close();
			SPDLOG_INFO("auto close serial {} success", strCom.toStdString());
			flipComButtonText(false);
		}
	}

}

void OrbbecFaceDemoTool::on_pushButton_door_lock_open_clicked()
{
	// 获取当前显示串口号
	QString strCom = ui.comboBox_door_lock_com_port->currentText();
	strCom = strCom.split(" ", Qt::SkipEmptyParts).at(0);

	if (!m_serial_dev->GetSerialObj()->IsOpen()) {
		if (!m_serial_dev->GetSerialObj()->Open(strCom, 115200)) {
			SPDLOG_WARN("open serial {} fail", strCom.toStdString());
			return;
		}
		SPDLOG_INFO("open serial {} success", strCom.toStdString());
		flipComButtonText(true);
	}
	else {

		m_serial_dev->GetSerialObj()->Close();
		SPDLOG_INFO("close serial {} success", strCom.toStdString());
		flipComButtonText(false);
	}

}

void OrbbecFaceDemoTool::flipComButtonText(bool en)
{
	ui.comboBox_door_lock_com_port->setEnabled(en ? false : true);
	ui.pushButton_door_lock_open->setText(en ? tr("关闭") : tr("打开"));
}

void OrbbecFaceDemoTool::initUi()
{
	registerUSBNotification((HANDLE)(this->winId()));
	show_usable_com_ports();

	m_serial_dev = new SerialComManage;
	QThread* threadSerial = new QThread;
	m_serial_dev->moveToThread(threadSerial);
	threadSerial->start();

	QButtonGroup* groupProtocol = new QButtonGroup(this);
	groupProtocol->addButton(ui.radioButton_ob, 0);
	groupProtocol->addButton(ui.radioButton_obEncrypt, 1);
	groupProtocol->addButton(ui.radioButton_lmEncryot, 2);
	groupProtocol->addButton(ui.radioButton_lkEncrypt, 3);

	connect(groupProtocol, SIGNAL(buttonClicked(int)), this, SLOT(serial_protocol_changed(int)));
	groupProtocol->button(0)->setChecked(true);
	groupProtocol->buttonClicked(0);

	ui.comboBox_snap_img_typ->addItem(QObject::tr("泛光"));
	ui.comboBox_snap_img_typ->addItem(QObject::tr("激光"));
	ui.comboBox_snap_img_typ->setCurrentIndex(0);

	ui.spinBox_gain_val->setValue(0);
}

void OrbbecFaceDemoTool::serial_protocol_changed(int idx)
{
	if (idx == 0)
	{
		if (nullptr == m_orbProtocol) {
			QThread* threadProtocol = new QThread;
			m_orbProtocol = new OrbProtocol(this);
			m_protocol = m_orbProtocol;

			m_protocol->moveToThread(threadProtocol);
			threadProtocol->start();

			connect(m_protocol, &OrbbecFaceModuleProtocolInterface::SendData,
				m_serial_dev, &SerialComManage::send_data);
			connect(m_protocol, &OrbbecFaceModuleProtocolInterface::RecvData, [=](QByteArray& data) {
				emit m_serial_dev->recv_data(data);
				});
			connect(m_protocol, &OrbbecFaceModuleProtocolInterface::ReOpenCom,this,[=](qint32 baute) {
				emit m_serial_dev->reopen(baute);
				}, Qt::BlockingQueuedConnection);
		}
	}
	else if (idx == 1)
	{
		//m_protocol = new orbprotocol::OrbProtocol;

	}
	else if (idx == 2)
	{

	}
	else if (idx == 3)
	{

	}
	else
	{

	}

}

void OrbbecFaceDemoTool::CaptureProces(bool value)
{
	QPalette pe;
	if (value)
	{
		ui.statusBar->showMessage("抓图完成", 2000);

		// ui->label_captip->setText("抓图完成");
		// ui->label_captip->setStyleSheet("color:green;");
		 //pe.setColor(QPalette::WindowText,Qt::green);
		 //ui->label_captip->setPalette(pe);
		 //ui->label_captip->repaint();
	}
	else
	{
		ui.statusBar->showMessage("抓图中...", 1500);

		//   ui->label_captip->clear();
		//   ui->label_captip->setText("抓图中 ");
		   //ui->label_captip->setStyleSheet("color:blue;");

		   //pe.setColor(QPalette::WindowText,Qt::red);
		   //ui->label_captip->setPalette(pe);
		   //ui->label_captip->repaint();
	}
}

bool OrbbecFaceDemoTool::nativeEventFilter(const QByteArray& eventType, void* message, long* result)
{
	int ret = orbNativeEventFilter(eventType, message, result);
	if (ret == 1 || ret == 2) // 设备连接
	{
		if (ret == 2) {
			autoCloseDevCom();
		}
		show_usable_com_ports();
	}

	return false;
}

void OrbbecFaceDemoTool::on_pushButton_IQC_Start_clicked()
{

}

void OrbbecFaceDemoTool::on_radioButton_RealPeople_clicked()
{

}

void OrbbecFaceDemoTool::on_radioButton_HeadMould_clicked()
{

}

void OrbbecFaceDemoTool::on_checkBox_IQC_SN_stateChanged(int arg1)
{

}

QStringList OrbbecFaceDemoTool::show_usable_com_ports()
{
	QStringList comLists;
	auto serialInfoList = SerialPortEx::FindAllComs();
	ui.comboBox_door_lock_com_port->clear();
	for (auto& serial : serialInfoList)
	{
		QString strCom = QString("%1 #%2").arg(serial.portName()).arg(serial.description());
		comLists.push_back(strCom);
		ui.comboBox_door_lock_com_port->addItem(strCom);
	}
	return comLists;
}

void OrbbecFaceDemoTool::ShowPic(unsigned char* data, int datalen, int width, int height)
{
	if (nullptr == data)
	{
		return;
	}

	//decode
	cv::Mat img_decode;
	std::vector<uchar> decdata;
	for (int i = 0; i < datalen; ++i) {
		decdata.push_back(data[i]);
	}
	img_decode = cv::imdecode(decdata, CV_LOAD_IMAGE_GRAYSCALE);
	cv::imwrite("imdecode.jpg", img_decode);
	//gray to RGB
	cv::Mat rgbMat(width, height, CV_8UC3);
	cv::cvtColor(img_decode, rgbMat, cv::COLOR_GRAY2RGB);
	cv::imwrite("Ir.jpg", rgbMat);
	int img_width = img_decode.cols;
	int img_height = img_decode.rows;
	//int channelnum = imageMat.channels();

	//show
	uchar* colorData = rgbMat.data;
	QImage image(colorData, img_width, img_height, QImage::Format_RGB888);

	int w = 0, h = 0;
	w = ui.label_picture_snap->width();
	h = ui.label_picture_snap->height();

	ui.label_picture_snap->setPixmap(QPixmap::fromImage(image).scaled(w, h));
	ui.label_picture_snap->setAlignment(Qt::AlignCenter);
	ui.label_picture_snap->show();
}
