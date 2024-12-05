#pragma once

#include <QtWidgets/QMainWindow>
#include <QAbstractNativeEventFilter>
#include <QButtonGroup>

#include "utility/utility.hpp"
#include "qserialex/serialportex.h"

#include "protocols/lk/lkprotocol.h"
#include "protocols/ob/OrbProtocol.h"
#include "protocols/lm/lmprotocol.h"

#include "ui_OrbbecFaceDemoTool.h"

#pragma execution_character_set("utf-8")

class OrbbecFaceModuleProtocolInterface;

class SerialComManage :public QObject
{
	Q_OBJECT

public:
	explicit SerialComManage(QObject* parent = nullptr) :QObject(parent) {
		m_com_dev = new SerialPortEx(SerialPortEx::MODE_ASYNC);
		//connect(this, &SerialComManage::send_data, m_com_dev, [=](const QByteArray& data) {
		//	SPDLOG_DEBUG("{} thread id {}", __FUNCTION__, QThread::currentThreadId());
		//	m_com_dev->sendData(data);
		//	});
		//connect(this, &SerialComManage::recv_data, m_com_dev, [=](QByteArray& data) {
		//	SPDLOG_DEBUG("{} thread id {}", __FUNCTION__, QThread::currentThreadId());
		//	m_com_dev->recvData(data);
		//	}/*, Qt::QueuedConnection*/);
		//connect(this, &SerialComManage::recv_data, m_com_dev, &SerialPortEx::recvData);
	};
	~SerialComManage() {
		if (nullptr != m_com_dev)
		{
			delete m_com_dev, m_com_dev = nullptr;
		}
	};

	SerialPortEx* GetSerialObj() {
		return m_com_dev;
	}

signals:

public slots:
	void send_data(const QByteArray& data) {
		m_com_dev->sendData(data);
	}
	void recv_data(QByteArray& data) {
		m_com_dev->recvData(data);
	}
	void reopen(qint32 baute) {
		comconfigpara compara;
		m_com_dev->GetComParam(compara);
		compara.baute = baute;
		m_com_dev->ReOpen(compara);
	}

private:
	SerialPortEx* m_com_dev = nullptr;

};

class OrbbecFaceDemoTool : public QMainWindow, public QAbstractNativeEventFilter
{
	Q_OBJECT

public:
	OrbbecFaceDemoTool(QWidget* parent = nullptr);
	~OrbbecFaceDemoTool();

private slots:
	// 串口设备相关
	void on_pushButton_door_lock_open_clicked();

	// 抓图特征值
	void on_pushButton_picture_snap_char_clicked();
	// 图片特征值
	void on_pushButton_send_picture_char_file_clicked();
	// 下发特征值
	void on_pushButton_send_picture_snap_char_clicked();

	void on_pushButton_recong_level_set_clicked();
	void on_pushButton_recong_level_get_clicked();
	void on_pushButton_recong_level_reset_clicked();

	void on_pushButton_live_level_set_clicked();
	void on_pushButton_live_level_get_clicked();
	void on_pushButton_live_level_reset_clicked();

	void on_checkBox_repeat_register_stateChanged(int change);
	void on_pushButton_repeat_register_get_clicked();

	void on_pushButton_get_door_lock_module_ver_clicked();
	void on_pushButton_get_door_lock_alg_board_ver_clicked();

	void on_pushButton_face_demo_mode_clicked();
	void on_pushButton_person_register_clicked();
	void on_pushButton_person_del_clicked();
	void on_pushButton_person_del_all_clicked();
	void on_pushButton_person_lookup_all_clicked();
	void on_pushButton_person_recongnize_clicked();
	void on_pushButton_picture_snap_clicked();


	//IQC
	void on_pushButton_IQC_Start_clicked();
	void on_radioButton_RealPeople_clicked();
	void on_radioButton_HeadMould_clicked();
	void on_checkBox_IQC_SN_stateChanged(int arg1);


	QStringList show_usable_com_ports();
	// USB热插拔，自动关闭串口
	void autoCloseDevCom();
	void flipComButtonText(bool en);

	void serial_protocol_changed(int idx);

	void CaptureProces(bool value);


private:
	void initUi();
	bool nativeEventFilter(const QByteArray& eventType, void* message, long* result);

	void ShowPic(unsigned char* data, int datalen, int width, int height);
private:
	Ui::OrbbecFaceDemoToolClass ui;

	SerialComManage* m_serial_dev;
	OrbbecFaceModuleProtocolInterface* m_protocol;
	orbprotocol::OrbProtocol* m_orbProtocol = nullptr;
	char     m_savePicPath[128]{ 0 }; //save pic path

};
