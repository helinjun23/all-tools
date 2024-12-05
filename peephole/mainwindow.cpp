#include "mainwindow.h"

#include "ui_mainwindow.h"
#include "ui_eventwidget.h"
#include "ui_userwidget.h"
#include "ui_netwidget.h"
#include "ui_cloudwidget.h"
#include "ui_miscwidget.h"
#include "ui_factory1widget.h"
#include "ui_factory2widget.h"

#include <QTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QCheckBox>

#include <functional>   // std::bind
#include "FileHelper.h"
#include "opencv/highgui.h"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"

#include "DataShow.h"
#include "clogcat.h"
#include "OrbDataTyp.h"
#include "SerialProtocol3V.h"
#include "SerialProtocolK1S.h"
#include "SerialProtocolOrb.h"

#include "FileHelper.h"
//
#include <iostream>
#include <fstream>

#include "qtaskpane/QTaskPane.h"

const QVector<QPair<QString, qint8>> kEventTypTable{
	{"冻结", 0x01},{"报警", 0x02},{"事件", 0x03},{"电池电量", 0x04},
};

const QVector<QPair<QString, qint8>> kOpenesTable{
	{"无", 0x0},
	{"指纹", 0x01},{"密码", 0x02},{"卡片", 0x03},
	{"人脸解锁", 0x04},{"手指静脉", 0x05},{"临时密码", 0x06},{"APP", 0x07},
	{"虹膜", 0x08},{"机械钥匙", 0x09},{"指纹&密码", 0x0a},{"人脸&指纹", 0x0b},
	{"手指静脉&密码", 0x0c},{"人脸&手指静脉", 0x0d},{"门内开锁", 0x0e},{"人脸&密码", 0x0f},
	{"限时密码", 0x10},{"门外开锁", 0x11},{"掌静脉", 0x12},
};

const QVector<QPair<QString, qint8>> kActionsTable{
	{"无", 0x0},
	{"录制视频", 0x01},{"录制音频", 0x02},{"录制音频视频", 0x03},
	{"抓图", 0x04},{"云存", 0x05}
};

const QVector<QPair<QString, qint8>> kEventsTable{
	{"无", 0x0},
	{"门铃呼叫", 0x01}, {"门打开!", 0x02},	{"关门!", 0x03}, {"反锁", 0x04},
	{"取消反锁", 0x05},	{"恢复出厂", 0x06},	{"门锁试错", 0x07},	{"离家模式open", 0x08},
	{"离家模式关闭!", 0x09},{"门外留言", 0x0a},{"远程密码开锁成功",0x0b},{"远程密码开锁失败",0x0c},
	{"授权远程密码开锁成功",0x0d},{"授权远程密码开锁失败",0x0e},{"开门失败",0x0f},
};

const QVector<QPair<QString, qint8>> kFreezesTable{
	{"无", 0x0},
	{"指纹冻结", 0x01},{"密码冻结", 0x02},{"卡片冻结", 0x03},
	{"人脸冻结", 0x04},{"手指静脉冻结", 0x05},{"掌静脉冻结", 0x06},
	{"虹膜冻结", 0x07},
};

const QVector<QPair<QString, qint8>> kAlarmsTable{
	{"无", 0x0},
	{"防拆报警", 0x01}, {"非法闯入", 0x02},	{"禁试报警", 0x03}, {"假锁报警", 0x04},
	{"布防报警", 0x05},	{"超时窥视", 0x06},	{"锁体异常", 0x07},	{"劫持指纹", 0x08},
	{"劫持密码", 0x09},{"门未关", 0x0a},{"门虚掩",0x0b},{"移动侦测",0x0c},
	{"劫持人脸",0x0d},{"劫持指静脉",0x0e},{"锁舌无法收回",0x0f},{"锁舌无法伸出",0x10},
	{"无故障",0x11},{"人脸模组故障",0x12},{"指纹模组故障",0x13},{"后板故障",0x14},
	{"人脸&指纹故障",0x15},	{"人脸&后板故障",0x16},{"指纹&后板故障",0x17},{"人脸&指纹&后板故障",0x18},
	{"童锁",0x19},
};

const QVector<QPair<QString, qint8>> kBatStatTable{
	{"大小电池电量正常", 0x00},{"大电池电量低", 0x01},{"小电池电量低", 0x02},{"大小电池电量低", 0x03},
};

#define CONNECT_SIGNAL_CLICKED(ui_widget, pb_name) \
	connect(ui_widget->pb_name, SIGNAL(clicked()), this, SLOT(on_##pb_name##_clicked()))

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	m_pNoteListModel = NULL;
	m_pDataProc = NULL;
	m_bDevOpen = false;
	m_bPicSave = true;

	//display area
	CDataShow::Instance()->SetDataPly(MediaShowFmtOpenGL, ui->openGLWidget);
	connect(ui->openGLWidget, &QOpenGLWidget::resized, this, [=] {
		//QPoint pos = ui->openGLWidget->pos();
		//int disWidth = ui->openGLWidget->width();
		//int disHeight = ui->openGLWidget->height();

		CDataShow::Instance()->SetShowArea(ui->openGLWidget->x(), ui->openGLWidget->y(),
			ui->openGLWidget->width(), ui->openGLWidget->height());
		}
	);

	//
	QString str;
	//baut rate param
	m_baudRate[0] = 600;    m_baudRate[1] = 1200; m_baudRate[2] = 2400;  m_baudRate[3] = 4800;
	m_baudRate[4] = 9600;   m_baudRate[5] = 14400; m_baudRate[6] = 19200; m_baudRate[7] = 28800;
	m_baudRate[8] = 38400;  m_baudRate[9] = 56000; m_baudRate[10] = 57600; m_baudRate[11] = 115200;
	m_baudRate[12] = 128000; m_baudRate[13] = 256000; m_baudRate[14] = 460800; m_baudRate[15] = 921600;
	m_baudRate[16] = 1500000;

	for (int i = 0; i < 17; i++)
	{
		str.sprintf("%d", m_baudRate[i]);
		ui->comboBox_peephole_baud_rate->addItem(str, i);
	}
	ui->comboBox_peephole_baud_rate->setCurrentIndex(11);

	ui_event = new Ui::eventwidget;
	QWidget* event_widget(new QWidget);
	ui_event->setupUi(event_widget);

	// 事件类型
	InitComboBoxItems(ui_event->comboBox_event_type, kEventTypTable);
	// 事件码
	InitComboBoxItems(ui_event->comboBox_event_code, kEventsTable);
	// action
	InitComboBoxItems(ui_event->comboBox_event_action, kActionsTable);
	// open typ
	InitComboBoxItems(ui_event->comboBox_event_open_typ, kOpenesTable);
	connect(ui_event->pushButton_event_send, SIGNAL(clicked()), this, SLOT(on_pushButton_event_send_clicked()));
	connect(ui_event->comboBox_event_type, SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboBox_event_type_currentIndexChanged(int)));

	ui_user = new Ui::userwidget;
	QWidget* user_widget(new QWidget);
	ui_user->setupUi(user_widget);

	InitUserDataParam();
	connect(ui_user->pushButton_user_data_send, SIGNAL(clicked()), this, SLOT(on_pushButton_user_data_send_clicked()));


	ui_net = new Ui::netwidget;
	QWidget* net_widget(new QWidget);
	ui_net->setupUi(net_widget);
	CONNECT_SIGNAL_CLICKED(ui_net, pushButton_wifi_sn_get);
	CONNECT_SIGNAL_CLICKED(ui_net, pushButton_device_sn_get);
	CONNECT_SIGNAL_CLICKED(ui_net, pushButton_wifi_ip_info_get);
	CONNECT_SIGNAL_CLICKED(ui_net, pushButton_wifi_mac_get);
	CONNECT_SIGNAL_CLICKED(ui_net, pushButton_wifi_ssid_pwd_set);
	CONNECT_SIGNAL_CLICKED(ui_net, pushButton_wifi_state_get);
	connect(ui_net->checkBox_wifi_reserved, SIGNAL(stateChanged(int)), this, SLOT(on_checkBox_wifi_reserved_stateChanged(int)));



	ui_cloud = new Ui::cloudwidget;
	QWidget* cloud_widget(new QWidget);
	ui_cloud->setupUi(cloud_widget);
	CONNECT_SIGNAL_CLICKED(ui_cloud, pushButton_key_set);
	CONNECT_SIGNAL_CLICKED(ui_cloud, pushButton_key_get);
	CONNECT_SIGNAL_CLICKED(ui_cloud, pushButton_data_pass);
	CONNECT_SIGNAL_CLICKED(ui_cloud, pushButton_protocol_extend_send);

	ui_misc = new Ui::miscwidget;
	QWidget* misc_widget(new QWidget);
	ui_misc->setupUi(misc_widget);

	InitRemindParam();

	//wifi face action typ
	m_wifiActionFaceTypGp.addButton(ui_misc->radioButton_wifi_action_face_live, 0);
	m_wifiActionFaceTypGp.addButton(ui_misc->radioButton_wifi_action_face_sleep, 1);
	connect(ui_misc->radioButton_wifi_action_face_live, SIGNAL(clicked()), this, SLOT(on_groupBox_wifi_live_face_clicked()));
	connect(ui_misc->radioButton_wifi_action_face_sleep, SIGNAL(clicked()), this, SLOT(on_groupBox_wifi_live_face_clicked()));

	//pic directory
	m_picCtlGp.addButton(ui_misc->radioButton_pic_landscape, 0);
	m_picCtlGp.addButton(ui_misc->radioButton_pic_portrait, 1);
	//ui->radioButton_pic_landscape->setChecked(true);
	connect(ui_misc->radioButton_pic_landscape, SIGNAL(clicked()), this, SLOT(on_groupBox_pic_dir_clicked()));
	connect(ui_misc->radioButton_pic_portrait, SIGNAL(clicked()), this, SLOT(on_groupBox_pic_dir_clicked()));

	//wifi action typ
	m_wifiActionTypGp.addButton(ui_misc->radioButton_wifi_action_standard, 0);
	m_wifiActionTypGp.addButton(ui_misc->radioButton_wifi_action_live, 1);
	m_wifiActionTypGp.addButton(ui_misc->radioButton_wifi_action_sleep, 2);
	connect(ui_misc->radioButton_wifi_action_standard, SIGNAL(clicked()), this, SLOT(on_groupBox_wifi_action_typ_clicked()));
	connect(ui_misc->radioButton_wifi_action_live, SIGNAL(clicked()), this, SLOT(on_groupBox_wifi_action_typ_clicked()));
	connect(ui_misc->radioButton_wifi_action_sleep, SIGNAL(clicked()), this, SLOT(on_groupBox_wifi_action_typ_clicked()));

	connect(ui_misc->checkBox_pir, SIGNAL(stateChanged(int)), this, SLOT(on_checkBox_pir_stateChanged(int)));
	connect(ui_misc->checkBox_ota, SIGNAL(stateChanged(int)), this, SLOT(on_checkBox_ota_stateChanged(int)));
	connect(ui_misc->checkBox_scan_conf, SIGNAL(stateChanged(int)), this, SLOT(on_checkBox_scan_conf_stateChanged(int)));

	CONNECT_SIGNAL_CLICKED(ui_misc, pushButton_dev_state_get);
	CONNECT_SIGNAL_CLICKED(ui_misc, pushButton_look_up_remind);
	CONNECT_SIGNAL_CLICKED(ui_misc, pushButton_version_get);
	CONNECT_SIGNAL_CLICKED(ui_misc, pushButton_net_time_get);
	CONNECT_SIGNAL_CLICKED(ui_misc, pushButton_fov_set);
	CONNECT_SIGNAL_CLICKED(ui_misc, pushButton_person_detect_state_get);
	CONNECT_SIGNAL_CLICKED(ui_misc, pushButton_mask_detect_state_get);
	CONNECT_SIGNAL_CLICKED(ui_misc, pushButton_peephole_sleep);
	CONNECT_SIGNAL_CLICKED(ui_misc, pushButton_peephole_reset_2);


	ui_fac1 = new Ui::factory1widget;
	QWidget* fac1_widget(new QWidget);
	ui_fac1->setupUi(fac1_widget);
	//
	ui_fac1->comboBox_ircut_state->clear();
	ui_fac1->comboBox_ircut_state->addItem("day", 0);
	ui_fac1->comboBox_ircut_state->addItem("night", 1);
	ui_fac1->lineEdit_orb_sn->setText("Shark 3V-20210331-100000");

	CONNECT_SIGNAL_CLICKED(ui_fac1, pushButton_sdcard_get);
	CONNECT_SIGNAL_CLICKED(ui_fac1, pushButton_sensor_sta_get);

	CONNECT_SIGNAL_CLICKED(ui_fac1, pushButton_ircut_state_set);
	CONNECT_SIGNAL_CLICKED(ui_fac1, pushButton_ircut_state_get);

	CONNECT_SIGNAL_CLICKED(ui_fac1, pushButton_orb_sn_set);
	CONNECT_SIGNAL_CLICKED(ui_fac1, pushButton_orb_sn_get);

	CONNECT_SIGNAL_CLICKED(ui_fac1, pushButton_light_sensitive_get);
	CONNECT_SIGNAL_CLICKED(ui_fac1, pushButton_light_sensitive_open);

	CONNECT_SIGNAL_CLICKED(ui_fac1, pushButton_pir_time_get);
	CONNECT_SIGNAL_CLICKED(ui_fac1, pushButton_pir_time_set);

	CONNECT_SIGNAL_CLICKED(ui_fac1, pushButton_pir_state_get);
	CONNECT_SIGNAL_CLICKED(ui_fac1, pushButton_pir_state_set);

	CONNECT_SIGNAL_CLICKED(ui_fac1, pushButton_pir_thresh_get);
	CONNECT_SIGNAL_CLICKED(ui_fac1, pushButton_pir_thresh_set);

	CONNECT_SIGNAL_CLICKED(ui_fac1, pushButton_peephole_reset);
	CONNECT_SIGNAL_CLICKED(ui_fac1, pushButton_version_wifi_get);
	CONNECT_SIGNAL_CLICKED(ui_fac1, pushButton_version_system_get);
	connect(ui_fac1->checkBox_led, SIGNAL(stateChanged(int)), this, SLOT(on_checkBox_led_stateChanged(int)));



	ui_fac2 = new Ui::factory2widget;
	QWidget* fac2_widget(new QWidget);
	ui_fac2->setupUi(fac2_widget);
	//
	ui_fac2->comboBox_media_stream_typ->clear();
	ui_fac2->comboBox_media_stream_typ->addItem("main", 0);
	ui_fac2->comboBox_media_stream_typ->addItem("sub", 1);
	ui_fac2->comboBox_media_stream_typ->addItem("third", 2);

	ui_fac2->lineEdit_wire_ip->setText("192.168.2.222");
	ui_fac2->lineEdit_wire_ip_gate->setText("192.168.2.0");
	ui_fac2->lineEdit_wire_ip_mask->setText("255.0.0.0");

	CONNECT_SIGNAL_CLICKED(ui_fac2, pushButton_wifi_state_get_2);
	CONNECT_SIGNAL_CLICKED(ui_fac2, pushButton_wifi_state_set);
	CONNECT_SIGNAL_CLICKED(ui_fac2, pushButton_wifi_ap_get);
	CONNECT_SIGNAL_CLICKED(ui_fac2, pushButton_wifi_ap_set);
	CONNECT_SIGNAL_CLICKED(ui_fac2, pushButton_wire_ip_set);

	CONNECT_SIGNAL_CLICKED(ui_fac2, pushButton_media_audio_record);
	CONNECT_SIGNAL_CLICKED(ui_fac2, pushButton_audio_rec_play);

	CONNECT_SIGNAL_CLICKED(ui_fac2, pushButton_media_start);
	CONNECT_SIGNAL_CLICKED(ui_fac2, pushButton_media_cur_frm_get);
	CONNECT_SIGNAL_CLICKED(ui_fac2, pushButton_media_url_get);

	CONNECT_SIGNAL_CLICKED(ui_fac2, pushButton_media_play);
	CONNECT_SIGNAL_CLICKED(ui_fac2, pushButton_media_snap);
	CONNECT_SIGNAL_CLICKED(ui_fac2, pushButton_media_record);

	connect(ui_fac2->checkBox_factory_mode, SIGNAL(stateChanged(int)), this, SLOT(on_checkBox_factory_mode_stateChanged(int)));

	m_paramTaskPane = new QTaskPane;
	m_paramTaskPane->AddWidget("事件管理", event_widget);
	m_paramTaskPane->AddWidget("用户管理", user_widget);
	m_paramTaskPane->AddWidget("网络管理", net_widget);
	m_paramTaskPane->AddWidget("云管理", cloud_widget);
	m_paramTaskPane->AddWidget("系统命令", misc_widget);
	m_paramTaskPane->AddWidget("生产1", fac1_widget);
	m_paramTaskPane->AddWidget("生产2", fac2_widget);

	ui->dockWidget->setWidget(m_paramTaskPane);

	connect(ui_net->pushButton_wifi_state_get, SIGNAL(clicked()), this, SLOT(on_pushButton_wifi_state_get_clicked()));


	//TODO: 从配置文件读取，支持记录历史数据
	//ali cloud information
	ui_cloud->lineEdit_product_key->setText("a1SLjjSoO0J");
	ui_cloud->lineEdit_product_secret->setText("StVTh3eowRm2px7G");
	ui_cloud->lineEdit_device_name->setText("test1");
	ui_cloud->lineEdit_device_secret->setText("300117e205044fe4893156658a9489a7");


#if 0
	//alarm event typ
	m_alarmEventGp.addButton(ui->radioButton_door_bell, 0);
	m_alarmEventGp.addButton(ui->radioButton_anti_disassembly, 1);
	m_alarmEventGp.addButton(ui->radioButton_forbid_try, 2);
	m_alarmEventGp.addButton(ui->radioButton_lock_protection, 3);
	m_alarmEventGp.addButton(ui->radioButton_lock_fake_lock, 4);
	m_alarmEventGp.addButton(ui->radioButton_lock_reset, 5);
	//ui->radioButton_door_bell->setChecked(true);

	connect(ui->radioButton_door_bell, SIGNAL(clicked()), this, SLOT(on_groupBox_event_clicked()));
	connect(ui->radioButton_anti_disassembly, SIGNAL(clicked()), this, SLOT(on_groupBox_event_clicked()));
	connect(ui->radioButton_forbid_try, SIGNAL(clicked()), this, SLOT(on_groupBox_event_clicked()));
	connect(ui->radioButton_lock_protection, SIGNAL(clicked()), this, SLOT(on_groupBox_event_clicked()));
	connect(ui->radioButton_lock_fake_lock, SIGNAL(clicked()), this, SLOT(on_groupBox_event_clicked()));
	connect(ui->radioButton_lock_reset, SIGNAL(clicked()), this, SLOT(on_groupBox_event_clicked()));

	//alarm event param
	m_eventParamGp.addButton(ui->radioButton_event_param_record, 0);
	m_eventParamGp.addButton(ui->radioButton_event_param_snap, 1);
	m_eventParamGp.addButton(ui->radioButton_event_param_none, 2);


	connect(ui->radioButton_event_param_record, SIGNAL(clicked()), this, SLOT(on_groupBox_event_param_clicked()));
	connect(ui->radioButton_event_param_snap, SIGNAL(clicked()), this, SLOT(on_groupBox_event_param_clicked()));
	connect(ui->radioButton_event_param_none, SIGNAL(clicked()), this, SLOT(on_groupBox_event_param_clicked()));

	ui->radioButton_event_param_record->setChecked(true);
#endif
	//EnableRecordParam(true);
	//EnableSnapParam(false);

	//serial  protocol type
	m_protocolTypGp.addButton(ui->radioButton_protocol_3v, 0);
	m_protocolTypGp.addButton(ui->radioButton_protocol_k1s, 1);
	m_protocolTypGp.addButton(ui->radioButton_protocol_orbbec, 2);
	connect(ui->radioButton_protocol_3v, SIGNAL(clicked()), this, SLOT(on_groupBox_protocol_clicked()));
	connect(ui->radioButton_protocol_k1s, SIGNAL(clicked()), this, SLOT(on_groupBox_protocol_clicked()));
	connect(ui->radioButton_protocol_orbbec, SIGNAL(clicked()), this, SLOT(on_groupBox_protocol_clicked()));
	ui->radioButton_protocol_orbbec->setChecked(true);
	m_protocolTyp = PeepholeProtocolTypOrb;
	m_pSerialProtocol = nullptr;

	if (m_protocolTyp != PeepholeProtocolTyp3V)
	{
		ui_net->checkBox_wifi_fac_mode->setEnabled(false);
	}

	//
	m_pDataProc = new CDataProc();
	//media
	m_bAudioRec = false;
	m_bAudioPly = false;
	//
	m_bStreamStart = false;



	m_bLightSensitiveOpen = false;


	//event param
   // ui->lineEdit_event_param_record_duration->setText("10");


	//
	ui->checkBox_msg_encrypt->setChecked(false);

	ui_cloud->lineEdit_data_pass->setText("Orbbec Test");
	//
	m_bPlay = false;
	m_snapPicNum = 0;
	//
	m_bRec = false;
	//
	//ui->lineEdit_event_param_snap_num->setText("1");
	ui_cloud->lineEdit_protocol_extend->setText("EF aa 00 0a f0 F1");

	//log
	connect(CLogcat::Instance(), SIGNAL(ShowInfoDialog(QString)), this, SLOT(ShowInfoDialog(QString)));
	connect(CLogcat::Instance(), SIGNAL(ShowNoteInfo(QString, int, bool)), this, SLOT(ShowNoteInfo(QString, int, bool)));
	//connect(CLogcat::Instance(),SIGNAL(SetLogPath(char *)),this,SLOT(SetLogPath(char *)));
	connect(CLogcat::Instance(), SIGNAL(ShowDevInfo(int, QString)), this, SLOT(ShowDevInfo(int, QString)));
	m_productNum = -1;
	ReadConfig();

	//ssid and pwd
	ui_net->lineEdit_wifi_ssid->setText(m_ssid);
	ui_net->lineEdit_wifi_pwd->setText(m_pwd);

	//InitAlarmParam();
	//InitEventParam();
	//InitBatteryParam();

#ifdef    ORBBEC_CLIENT_VER
	QString winTitle = windowTitle();
	QString verStr = winTitle.mid(winTitle.count() - 8, 8);

	QString subwinTitle = winTitle.left(winTitle.count() - 8);
	setWindowTitle("猫眼 v1.0.6 client ");

	//remove table
	ui->tabWidget_func->removeTab(FUNC_TAB_TYPE_FAC2);
	ui->tabWidget_func->removeTab(FUNC_TAB_TYPE_FAC1);
	ui->radioButton_protocol_k1s->setEnabled(false);
	ui->radioButton_protocol_3v->setEnabled(false);
	ui->radioButton_protocol_orbbec->setChecked(true);
	m_protocolTyp = PeepholeProtocolTypOrb;
	EnableProtocolFunc(false);
#endif

	ui_misc->lineEdit_fov_set->setText("80");

	setWindowIcon(QIcon(":/res/orbbec.ico"));
	setWindowTitle("门锁猫眼协议工具 V" + QApplication::applicationVersion());
}

MainWindow::~MainWindow()
{
	delete ui;
	if (m_pSerialProtocol != NULL)
	{
		delete m_pSerialProtocol;
	}
	m_pSerialProtocol = NULL;

	if (m_pDataProc != NULL)
	{
		m_pDataProc->Stop();
		delete m_pDataProc;
	}
	m_pDataProc = NULL;

	ClearNoteListRes(true);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	if (NULL == event)
	{
		return;
	}
}

void MainWindow::ReadConfig()
{
	int conNum = 0;
	char* pNum = NULL;
	std::ifstream fin("./paramConfig.ini");
	const int LINE_LENGTH = 1024;
	char str[LINE_LENGTH] = { 0 };
	while (fin.getline(str, LINE_LENGTH))
	{
		//ShowNoteInfo(str,1,true);

		pNum = NULL;
		//depth
		pNum = strstr(str, "mainWidth=");
		if (pNum != NULL)
		{
			pNum += strlen("mainWidth=");
			conNum = atoi(pNum);
			m_mainWidth = conNum;
		}
		pNum = strstr(str, "mainHeight=");
		if (pNum != NULL)
		{
			pNum += strlen("mainHeight=");
			conNum = atoi(pNum);
			m_mainHeight = conNum;
		}
		//ir
		pNum = strstr(str, "subWidth=");
		if (pNum != NULL)
		{
			pNum += strlen("subWidth=");
			conNum = atoi(pNum);
			m_subWidth = conNum;
		}
		pNum = strstr(str, "subHeight=");
		if (pNum != NULL)
		{
			pNum += strlen("subHeight=");
			conNum = atoi(pNum);
			m_subHeight = conNum;
		}
		pNum = strstr(str, "productnum=");
		if (pNum != NULL)
		{
			pNum += strlen("productnum=");
			conNum = atoi(pNum);
			m_productNum = conNum;
		}
		//ssid=123456
		//pwd=123456
		pNum = strstr(str, "ssid=");
		if (pNum != NULL)
		{
			pNum += strlen("ssid=");
			strcpy(m_ssid, pNum);
		}

		pNum = strstr(str, "pwd=");
		if (pNum != NULL)
		{
			pNum += strlen("pwd=");
			strcpy(m_pwd, pNum);
		}
	}

}

void MainWindow::ClearNoteListRes(bool bDel)
{
	if (NULL == m_pNoteListModel)
	{
		return;
	}

	QStandardItem* item = NULL;
	int i = 0;
	for (i = 0; i < m_pNoteListModel->rowCount(); i++)
	{
		item = m_pNoteListModel->takeItem(i);
		if (item != NULL)
		{
			delete item;
		}
		item = NULL;
	}
	m_pNoteListModel->clear();

	if (bDel)
	{
		if (m_pNoteListModel != NULL)
		{
			delete m_pNoteListModel;
		}
		m_pNoteListModel = NULL;
	}
}

void MainWindow::ShowNoteInfo(QString noteStr, int bkColor, bool bShow)
{
	QString strTmp = noteStr;

	if (NULL == m_pNoteListModel)
	{
		m_pNoteListModel = new QStandardItemModel(this);
	}

	if (bShow)
	{
		if (m_pNoteListModel->rowCount() > 100)
		{
			ClearNoteListRes();
		}

		QStandardItem* item = new QStandardItem(noteStr);
		if (bkColor == BkColorTypRed)
		{
			QBrush brush(QColor(255, 0, 0));
			item->setForeground(brush);
		}
		if (bkColor == BkColorTypGreen)
		{
			QBrush brush(QColor(0, 255, 0));
			item->setForeground(brush);
		}
		if (bkColor == BkColorTypBlue)
		{
			QBrush brush(QColor(0, 0, 255));
			item->setForeground(brush);
		}
		m_pNoteListModel->appendRow(item);

		ui->listView_note->setModel(m_pNoteListModel);
		ui->listView_note->scrollToBottom();
	}

	if (m_logFilePath[0] != '\0')
	{
		QDateTime current_date_time = QDateTime::currentDateTime();
		QString current_date = current_date_time.toString("yyyy/MM/dd hh:mm:ss:zzz");
		strTmp = QString("[") + current_date + QString("] ") + noteStr;

		std::string logStr = strTmp.toStdString();
		logStr += '\n';
		m_logWrite.WriteFile(m_logFilePath, logStr.c_str(), logStr.length());
	}
}

void MainWindow::ShowInfoDialog(QString info)
{
	QMessageBox::information(this, "notice", info, QMessageBox::Ok);
}

void MainWindow::ShowDevInfo(int infoTyp, QString info)
{
	switch (infoTyp)
	{
	case PeepHoleCmdTypMac:
		ui_net->lineEdit_wifi_mac->setText(info);
		break;
	case PeepHoleCmdTypDevName:
	{
		ui_net->lineEdit_wifi_device_name->setText(info);
	}
	break;
	case PeepHoleCmdTypIP:
		ui_net->lineEdit_wifi_ip->setText(info);
		break;
	case PeepHoleCmdTypAlarmEvent:
		//ClearEventCheck(m_alarmEventGp.checkedId());
		break;
	case PeepHoleCmdTypIpMask:
		ui_net->lineEdit_wifi_ip_mask->setText(info);
		break;
	case PeepHoleCmdTypIPGate:
		ui_net->lineEdit_wifi_gate->setText(info);
		break;
	case PeepHoleCmdTypWifiSta:
		ui_net->lineEdit_wifi_state->setText(info);
		break;
	case PeepHoleCmdTypWifiStaDb:
		ui_net->lineEdit_wifi_db->setText(info);
		break;
	case PeepHoleCmdTypWifiStaSNR:
		ui_net->lineEdit_wifi_snr->setText(info);
		break;
	case PeepHoleCmdTypNetTime:
		ui_misc->lineEdit_net_time->setText(info);
		break;
	case PeepHoleCmdTypVersion:
		ui_misc->lineEdit_version->setText(info);
		break;
	case PeepholeOrbCmdTypDevSta:
		ui_misc->lineEdit_dev_state->setText(info);
		break;
	case PeepholeOrbCmdTypSNGet:
	{
		ui_fac1->lineEdit_orb_sn->setText(info);
	}
	break;
	case PeepholeOrbCmdTypSNSet:
		break;
	case PeepholeOrbCmdTypSysVer:
		ui_fac1->lineEdit_version_system->setText(info);
		break;
	case PeepholeOrbCmdTypWifiVer:
		ui_fac1->lineEdit_version_wifi->setText(info);
		break;
	case PeepHoleCmdTypDeviceSN:
		ui_net->lineEdit_device_sn->setText(info);
		break;
	case PeepholeOrbCmdTypReset:
		break;

	case PeepholeOrbCmdTypTF:
		ui_fac1->lineEdit_sdcard->setText(info);
		break;
	case PeepholeOrbCmdTypSensorSta:
		ui_fac1->lineEdit_sensor_sta->setText(info);
		break;
	case PeepholeOrbCmdTypLedCtl:
		break;
	case PeepholeOrbCmdTypPirThreshGet:
		ui_fac1->lineEdit_pir_dist->setText(info);
		break;
	case PeepholeOrbCmdTypPirThreshSet:
		break;
	case PeepholeOrbCmdTypPirCtl:
		break;
	case PeepholeOrbCmdTypPirSta:
		ui_fac1->lineEdit_pir_state->setText(info);
		break;
	case PeepholeOrbCmdTypPirTimeGet:
		ui_fac1->lineEdit_pir_time->setText(info);
		break;
	case PeepholeOrbCmdTypPirTimeSet:
		break;
	case PeepholeOrbCmdTypMeidaAudioRec:
		break;
	case PeepholeOrbCmdTypMeidaAudioPlay:
		break;
	case PeepholeOrbCmdTypMeidaStreamUrl:
		ui_fac2->lineEdit_media_url->setText(info);
		break;
	case PeepholeOrbCmdTypMeidaStreamOpen:
		ui_fac2->lineEdit_media_url->setText(info);
		break;
	case PeepholeOrbCmdTypMeidaStreamClose:
		break;
	case PeepholeOrbCmdTypMeidaStreamFrmCount:
		ui_fac2->lineEdit_media_cur_frm->setText(info);
		break;
	case PeepholeOrbCmdTypWifiApGet:
		ui_fac2->lineEdit_wifi_ap->setText(info);
		break;
	case PeepholeOrbCmdTypWifiApSet:
		break;
	case PeepholeOrbCmdTypWifiCtl:
		break;
	case PeepholeOrbCmdTypWifiSta:
		ui_fac2->lineEdit_wifi_state_2->setText(info);
		break;
	case PeepholeOrbCmdTypIrCutCtl:
		break;
	case PeepholeOrbCmdTypIrCutSta:
	{
		ui_fac1->comboBox_ircut_state->setCurrentIndex(info.toInt() - 1);
		//ui->lineEdit_ircut_state->setText(info);
	}
	break;
	case PeepholeOrbCmdTypLightSensitiveVal:
		ui_fac1->lineEdit_light_sensitive_val->setText(info);
		break;
	case PeepholeOrbCmdTypLightSensitiveCtl:
		break;
	case PeepHoleOrbCmdTypGetPersonSta:
		ui_misc->lineEdit_person_detect->setText(info);
		break;
	case PeepHoleOrbCmdTypGetMaskSta:
		ui_misc->lineEdit_mask_detect_state->setText(info);
		break;
	case PeepholeOrbCmdTypGetCloudKeyProductKey:
		ui_cloud->lineEdit_product_key->setText(info);
		break;
	case PeepholeOrbCmdTypGetCloudKeyProductSecret:
		ui_cloud->lineEdit_product_secret->setText(info);
		break;
	case PeepholeOrbCmdTypGetCloudKeyDeviceName:
		ui_cloud->lineEdit_device_name->setText(info);
		break;
	case PeepholeOrbCmdTypGetCloudKeyDeviceSecret:
		ui_cloud->lineEdit_device_secret->setText(info);
		break;
	default:
		break;
	}
}

void MainWindow::on_pushButton_wifi_ssid_pwd_set_clicked()
{
	if (!m_bDevOpen || nullptr == m_pSerialProtocol)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	QString ssid = ui_net->lineEdit_wifi_ssid->text();
	QString pwd = ui_net->lineEdit_wifi_pwd->text();

	if (ssid.length() > 33)
	{
		ShowNoteInfo("ssid长度错误!", 0, true);
		return;
	}

	if (pwd.length() > 65)
	{
		ShowNoteInfo("密码长度错误!", 0, true);
		return;
	}
	QString reserved = "";
	reserved.clear();
	if (ui_net->checkBox_wifi_reserved->isChecked())
		reserved = ui_net->lineEdit_wifi_reserved->text();
	bool bFac = ui_net->checkBox_wifi_fac_mode->isChecked();
	m_pSerialProtocol->ConfigWifi(ssid.toStdString(), pwd.toStdString(), reserved.toStdString(), bFac);
}

void MainWindow::on_pushButton_wifi_mac_get_clicked()
{
	if (!m_bDevOpen || nullptr == m_pSerialProtocol)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	ui_net->lineEdit_wifi_mac->setText("");
	m_pSerialProtocol->GetWifiMac();
}

void MainWindow::on_pushButton_wifi_sn_get_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}

	ui_net->lineEdit_wifi_device_name->setText("");

	m_pSerialProtocol->GetDeviceName();
}

void MainWindow::on_pushButton_wifi_ip_info_get_clicked()
{
	if (!m_bDevOpen || nullptr == m_pSerialProtocol)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	ui_net->lineEdit_wifi_ip->setText("");
	ui_net->lineEdit_wifi_ip_mask->setText("");
	ui_net->lineEdit_wifi_gate->setText("");
	m_pSerialProtocol->GetIpInfo();
}

void MainWindow::on_checkBox_pir_stateChanged(int arg1)
{
	if (!m_bDevOpen || nullptr == m_pSerialProtocol)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}

	if (arg1 > 0)
	{
		m_pSerialProtocol->PirCtl(true);
	}
	else
	{
		m_pSerialProtocol->PirCtl(false);
	}
}

void MainWindow::on_pushButton_key_set_clicked()
{
	if (!m_bDevOpen || nullptr == m_pSerialProtocol)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}

	QString product_key = ui_cloud->lineEdit_product_key->text();
	if (product_key.length() > 32)
	{
		ShowNoteInfo("product_key长度错误", 0, true);
		return;
	}
	QString product_secret = ui_cloud->lineEdit_product_secret->text();
	if (product_secret.length() > 64)
	{
		ShowNoteInfo("product_secret长度错误", 0, true);
		return;
	}

	QString device_name = ui_cloud->lineEdit_device_name->text();
	if (device_name.length() > 32)
	{
		ShowNoteInfo("device_name长度错误", 0, true);
		return;
	}

	QString device_secret = ui_cloud->lineEdit_device_secret->text();
	if (device_secret.length() > 64)
	{
		ShowNoteInfo("device_secret长度错误", 0, true);
		return;
	}

	m_pSerialProtocol->SetCloudKey(product_key.toStdString(), product_secret.toStdString(),
		device_name.toStdString(), device_secret.toStdString());
}

void MainWindow::on_groupBox_pic_dir_clicked()
{
	if (!m_bDevOpen || nullptr == m_pSerialProtocol)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	switch (m_picCtlGp.checkedId())
	{
	case 0:
		m_pSerialProtocol->PicDirCtl(0x00);
		break;
	case 1:
		m_pSerialProtocol->PicDirCtl(0x01);
		break;
	default:
		break;
	}

}

void MainWindow::InitAlarmParam(void)
{
	//事件码
	InitComboBoxItems(ui_event->comboBox_event_code, kAlarmsTable);
}

void MainWindow::InitEventParam(void)
{
	// 事件码
	InitComboBoxItems(ui_event->comboBox_event_code, kEventsTable);
}

void MainWindow::InitFreezeParam()
{
	//freeze type
	InitComboBoxItems(ui_event->comboBox_event_code, kFreezesTable);

}

void MainWindow::InitRemindParam(void)
{
	//open typ
	InitComboBoxItems(ui_misc->comboBox_remind_open_typ, kOpenesTable);

	//user id
	ui_misc->lineEdit_remind_user_id->setText("1");
}

void MainWindow::InitBatteryParam(void)
{
	InitComboBoxItems(ui_event->comboBox_event_code, kBatStatTable);
}

void MainWindow::InitUserDataParam(void)
{
	//event type
	ui_user->comboBox_user_data_event_typ->addItem("添加用户", 1);
	ui_user->comboBox_user_data_event_typ->addItem("删除用户", 2);
	ui_user->comboBox_user_data_event_typ->addItem("清空用户", 3);
	ui_user->comboBox_user_data_event_typ->addItem("初始化用户", 4);
	ui_user->comboBox_user_data_event_typ->addItem("添加用户失败", 5);
	ui_user->comboBox_user_data_event_typ->addItem("删除用户失败", 6);
	ui_user->comboBox_user_data_event_typ->addItem("初始绑定用户", 7);

	//user type
	ui_user->comboBox_user_data_user_typ->addItem("指纹", 1);
	ui_user->comboBox_user_data_user_typ->addItem("密码", 2);
	ui_user->comboBox_user_data_user_typ->addItem("卡片", 3);
	ui_user->comboBox_user_data_user_typ->addItem("人脸", 4);
	ui_user->comboBox_user_data_user_typ->addItem("指纹静脉", 5);
	ui_user->comboBox_user_data_user_typ->addItem("临时密码", 6);
	ui_user->comboBox_user_data_user_typ->addItem("保留", 7);
	ui_user->comboBox_user_data_user_typ->addItem("全部(删除整个用户)", 8);
	ui_user->comboBox_user_data_user_typ->addItem("掌静脉", 9);
	ui_user->comboBox_user_data_user_typ->addItem("虹膜", 0x0a);

	//authentic type
	ui_user->comboBox_user_data_auth_typ->addItem("普通用户", 1);
	ui_user->comboBox_user_data_auth_typ->addItem("胁迫用户", 2);
	ui_user->comboBox_user_data_auth_typ->addItem("管理用户", 3);
	ui_user->comboBox_user_data_auth_typ->addItem("once密码", 4);
	ui_user->comboBox_user_data_auth_typ->addItem("限时段密码", 5);
	ui_user->comboBox_user_data_auth_typ->addItem("限次数密码", 6);

	ui_user->spinBox_user_data_main_id->setValue(1);
	ui_user->spinBox_user_data_sub_id->setValue(0);
}

void MainWindow::ClearEventCheck(uint8_t id)
{
	ShowNoteInfo("1:" + QString::number(id), 1, true);
#if 0
	switch (id)
	{
	case 0:
		//门铃
		ui->radioButton_door_bell->setChecked(false);
		ShowNoteInfo("2", 1, true);
		break;
	case 1:
		//防撬
		ui->radioButton_anti_disassembly->setChecked(false);
		break;
	case 2:
		//禁试
		ui->radioButton_forbid_try->setChecked(false);
		break;
	case 3:
		//布防
		ui->radioButton_lock_protection->setChecked(false);
		break;
	case 4:
		//假锁
		ui->radioButton_lock_fake_lock->setChecked(false);
		break;
	case 5:
		//reset
		ui->radioButton_lock_reset->setChecked(false);
		break;
	default:
		break;
	}
#endif
	//m_alarmEventGp.setExclusive(false);
	//m_alarmEventGp.button(id)->setChecked(false);
	//m_alarmEventGp.setExclusive(true);
}

void MainWindow::on_groupBox_event_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	//alarm event
	MediaCapEventParam param;
	memset(&param, 0, sizeof(param));
#if 0
	if (ui->radioButton_event_param_record->isChecked())
	{
		param.action = event_action_typ_record;
		param.duration = ui->lineEdit_event_param_record_duration->toPlainText().toInt();
		if (ui->checkBox_event_param_cap->isChecked())
			param.capEnable = 1;
		if (ui->checkBox_event_param_cloud->isChecked())
			param.cloudSave = 1;
		//media
		if (ui->checkBox_event_param_video->isChecked())
			param.mediaTyp = event_media_typ_video; //k1s
		if (ui->checkBox_event_paran_audio->isChecked())
			param.mediaTyp = event_media_typ_audio;//k1s
		if (ui->checkBox_event_param_video->isChecked() && ui->checkBox_event_paran_audio->isChecked())
			param.mediaTyp = event_media_typ_av;
	}

	if (ui->radioButton_event_param_snap->isChecked())
	{
		param.action = event_action_typ_snap;
		param.duration = ui->lineEdit_event_param_snap_num->toPlainText().toInt();
	}
#endif
	uint8_t event = 0;
	switch (m_alarmEventGp.checkedId())
	{
	case 0:
		event = LOCK_EVENT_TYP_DOOR_BELL;//门铃
		ShowNoteInfo("门铃事件", 1, true);
		break;
	case 1:
		event = LOCK_EVENT_TYP_ANTI_PRIYING;//防撬
		ShowNoteInfo("防撬事件", 1, true);
		break;
	case 2:
		event = LOCK_EVENT_TYP_BAN_TRY; //禁试
		ShowNoteInfo("禁试事件", 1, true);
		break;
	case 3:
		event = LOCK_EVENT_TYP_PROTECTION; //布防
		ShowNoteInfo("布防事件", 1, true);
		break;
	case 4:
		event = LOCK_EVENT_TYP_FAKE_LOCK;    //假锁
		ShowNoteInfo("假锁事件", 1, true);
		break;
	case 5:
		event = LOCK_EVENT_TYP_RESET;   //reset
		ShowNoteInfo("重置事件", 1, true);
		break;
	default:
		break;
	}

	m_pSerialProtocol->EventProc(event, param);

	//QThread::msleep(200);
	m_alarmEventGp.setExclusive(false);
	m_alarmEventGp.button(m_alarmEventGp.checkedId())->setChecked(false);
	m_alarmEventGp.setExclusive(true);
}

void MainWindow::on_pushButton_peephole_coms_clicked()
{
	ui->comboBox_peephole_com_port->clear();

	QList<QSerialPortInfo> serialInfoList = QSerialPortInfo::availablePorts();

	ShowNoteInfo(QString("size=") + QString::number(serialInfoList.size()), 0, true);

	QList<QSerialPortInfo>::iterator itor;
	int i = 0;
	for (itor = serialInfoList.begin(); itor != serialInfoList.end(); itor++)
	{
		//ShowNoteInfo(QString("serialNum=")+itor->serialNumber()+QString(",portName=")+itor->portName()+QString(",descri=")+itor->description()+QString(",manu=")+itor->manufacturer());
		ShowNoteInfo(QString("portName=") + itor->portName() + QString(",descri=") + itor->description(), 0, true);
		ui->comboBox_peephole_com_port->addItem(itor->portName(), i++);
	}
}

void MainWindow::on_pushButton_peephole_open_clicked()
{
	QDateTime localTime;
	localTime = QDateTime::currentDateTime();

	m_bDevOpen = !m_bDevOpen;
	if (m_bDevOpen)
	{
		ReadConfig(); //

		ui->pushButton_peephole_open->setText("关闭");

		sprintf(m_logFilePath, "./Log/door_%d_%d_%d_%d_%d_%d.log", localTime.date().year(), localTime.date().month(), localTime.date().day(),
			localTime.time().hour(), localTime.time().minute(), localTime.time().second());
#if 0
		sprintf(m_savePicPath, "./PicData/pic_%d_%d_%d_%d_%d_%d", localTime.date().year(), localTime.date().month(), localTime.date().day(),
			localTime.time().hour(), localTime.time().minute(), localTime.time().second());

		//create save picture detectory
		if (m_bPicSave)
		{
			CheckDirect(m_savePicPath);
		}
#endif
		switch (m_protocolTyp)
		{
		case PeepholeProtocolTyp3V:
			m_pSerialProtocol = new CSerialProtocol3V();
			break;
		case PeepholeProtocolTypK1S: //senseTime
			m_pSerialProtocol = new CSerialProtocolK1S();
			break;
		case PeepholeProtocolTypOrb: //orbbec
			m_pSerialProtocol = new CSerialProtocolOrb();
			break;
		default:
			break;
		}

		if (m_pDataProc != NULL)
		{
			m_pDataProc->SetParam(ui->comboBox_peephole_com_port->currentText(),
				m_baudRate[ui->comboBox_peephole_baud_rate->currentIndex()],
				8, 1);
		}
		if (ui->checkBox_msg_encrypt->isChecked())
			m_pSerialProtocol->SetMsgEncrypt(true);
		else
			m_pSerialProtocol->SetMsgEncrypt(false);

		if (m_pSerialProtocol != nullptr)
		{
			m_pSerialProtocol->SetSendFunc(&CDataProc::CmdSend, m_pDataProc);
		}
		m_orbFac.SetSendFunc(&CDataProc::CmdSend, m_pDataProc);

		QThread::msleep(200);
		m_pDataProc->Start(m_pSerialProtocol, m_protocolTyp);
	}
	else
	{
		ui->pushButton_peephole_open->setText("打开");

		memset(m_logFilePath, 0, sizeof(m_logFilePath));
		memset(m_savePicPath, 0, sizeof(m_savePicPath));

		if (m_pDataProc != NULL)
		{
			m_pDataProc->Stop();
		}
	}
}

void MainWindow::on_pushButton_wifi_state_get_clicked()
{
	if (!m_bDevOpen || nullptr == m_pSerialProtocol)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	ui_net->lineEdit_wifi_state->setText("");
	ui_net->lineEdit_wifi_db->setText("");
	ui_net->lineEdit_wifi_snr->setText("");
	m_pSerialProtocol->GetWifiSta();

}

void MainWindow::on_pushButton_peephole_sleep_clicked()
{
	if (!m_bDevOpen || nullptr == m_pSerialProtocol)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}

	m_pSerialProtocol->PeepholeSleep(0);
}



void MainWindow::ShowPic(unsigned char* data, int datalen, int width, int height)
{
	if (NULL == data)
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

	//gray to RGB
	cv::Mat rgbMat(width, height, CV_8UC3);
	cv::cvtColor(img_decode, rgbMat, cv::COLOR_GRAY2RGB);
	int img_width = img_decode.cols;
	int img_height = img_decode.rows;
	//int channelnum = imageMat.channels();

	//show
	uchar* colorData = rgbMat.data;
	QImage image(colorData, img_width, img_height, QImage::Format_RGB888);

	int w = 0, h = 0;
	//w= ui->label_picture_snap->width();
	//h= ui->label_picture_snap->height();

	//ui->label_picture_snap->setPixmap(QPixmap::fromImage(image).scaled(w,h));
	//ui->label_picture_snap->setAlignment(Qt::AlignCenter);
	//ui->label_picture_snap->show();
}

void MainWindow::ReadConfig1(void)
{
	int conNum = 0;
	char* pNum = NULL;
	std::ifstream fin("./config.ini");
	const int LINE_LENGTH = 100;
	char str[LINE_LENGTH];
	while (fin.getline(str, LINE_LENGTH))
	{
		pNum = NULL;
	}
}

void  MainWindow::CheckDirect(char* directPath)
{
	if (NULL == directPath)
	{
		return;
	}

	QDir dir;
	if (!dir.exists(QString(directPath)))
	{
		dir.mkpath(QString(directPath));
	}
}

void MainWindow::on_pushButton_version_system_get_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	m_orbFac.GetSysVer();

}

void MainWindow::on_pushButton_version_wifi_get_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	m_orbFac.GetWifiVer();
}

void MainWindow::on_pushButton_orbbec_sn_get_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	m_orbFac.GetSN();

}

void MainWindow::on_pushButton_peephole_reset_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}

	m_orbFac.Reset();
}

void MainWindow::on_pushButton_pir_state_get_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	m_orbFac.GetPirSta();
}

void MainWindow::on_pushButton_pir_state_set_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}

	uint8_t ctl = (uint8_t)ui_fac1->lineEdit_pir_state->text().toInt();
	if (ctl > 0) //open
		m_orbFac.SetPirSta(true);
	else  //close
		m_orbFac.SetPirSta(false);

}

void MainWindow::on_pushButton_pir_thresh_get_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	m_orbFac.GetPirThresh();
}

void MainWindow::on_pushButton_pir_thresh_set_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}

	uint8_t ctl = (uint8_t)ui_fac1->lineEdit_pir_dist->text().toInt();
	if (ctl < 6)
		m_orbFac.SetPirThresh(ctl);
	else
		ShowNoteInfo("pir thresh err!", 0, true);
}

void MainWindow::on_pushButton_pir_time_get_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	m_orbFac.GetPirTime();
}

void MainWindow::on_pushButton_pir_time_set_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}

	uint8_t ctl = (uint8_t)ui_fac1->lineEdit_pir_time->text().toInt();
	if (ctl != 0)
		m_orbFac.SetPirTime(ctl);
	else
		ShowNoteInfo("pir time err!", 0, true);

}

void MainWindow::on_pushButton_wifi_state_get_2_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	m_orbFac.GetWifiSta();
}

void MainWindow::on_pushButton_wifi_state_set_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	uint8_t ctl = (uint8_t)ui_fac2->lineEdit_wifi_state_2->text().toInt();
	m_orbFac.SetWifiSta(ctl);
}

void MainWindow::on_pushButton_wifi_ap_get_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	m_orbFac.GetWifiAp();

}

void MainWindow::on_pushButton_wifi_ap_set_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}

	uint8_t ctl = (uint8_t)ui_fac2->lineEdit_wifi_ap->text().toInt();
	if (ctl > 0)
		m_orbFac.SetWifiAp(ctl);
}

void MainWindow::on_pushButton_media_audio_record_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}

	m_bAudioRec = !m_bAudioRec;
	if (m_bAudioRec)
	{
		ui_fac2->pushButton_media_audio_record->setText("stop");
		m_orbFac.AudioRec(true);
	}
	else
	{
		ui_fac2->pushButton_media_audio_record->setText("AudioRec");
		m_orbFac.AudioRec(false);
	}
}

void MainWindow::on_pushButton_audio_rec_play_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}

	m_bAudioPly = !m_bAudioPly;
	if (m_bAudioPly)
	{
		ui_fac2->pushButton_audio_rec_play->setText("stop");
		m_orbFac.AudioRecPly(0x02); //play record
	}
	else
	{
		ui_fac2->pushButton_audio_rec_play->setText("AudioPlay");
		m_orbFac.AudioRecPly(0x00); //stop
	}
}

void MainWindow::on_pushButton_media_start_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}

	m_bStreamStart = !m_bStreamStart;
	if (m_bStreamStart)
	{
		ui_fac2->pushButton_media_start->setText("stop");
		uint8_t protocolTyp, netTyp, streamTyp = 0;
		//typ:0x01:RTSP;0x02:RTMP;0x03:其他
		protocolTyp = 0x01;

		//connect:0x01:wifi连接；0x02:有线连接
		netTyp = ui_fac2->groupBox_wire->isChecked() ? 0x02 : 0x01;

		//0x01:主码流;0x02:子码流；0x03:第三码流
		uint8_t typIdx = ui_fac2->comboBox_media_stream_typ->currentIndex();
		switch (typIdx)
		{
		case 0:
			streamTyp = 0x01;
			break;
		case 1:
			streamTyp = 0x02;
			break;
		case 2:
			streamTyp = 0x03;
			break;
		default:
			break;
		}

		//ShowNoteInfo("streamTyp="+QString::number(streamTyp),1,true);
		//ShowNoteInfo("typIdx="+QString::number(typIdx),1,true);
		m_orbFac.StartMedia(protocolTyp, streamTyp, netTyp);
		memset(m_streamUrl, 0, sizeof(m_streamUrl));
		m_orbFac.GetMediaCurFrm(streamTyp);
		m_orbFac.GetMediaUrl();
	}
	else
	{
		ui_fac2->pushButton_media_start->setText("start");
		m_orbFac.StopMedia();
	}

}

void MainWindow::on_pushButton_media_url_get_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	m_orbFac.GetMediaUrl();
}

void MainWindow::on_pushButton_ircut_state_get_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	m_orbFac.GetIrcutSta();
}

void MainWindow::on_pushButton_ircut_state_set_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}

	uint8_t idx = ui_fac1->comboBox_ircut_state->currentIndex();
	if (idx == 0)
	{
		m_orbFac.SetIrcutSta(0x01);
	}
	else if (idx == 1)
	{
		m_orbFac.SetIrcutSta(0x02);
	}
	else
	{
		ShowNoteInfo("ircut err!", 0, true);
		return;
	}

	ShowNoteInfo(ui_fac1->comboBox_ircut_state->currentText(), 1, true);
}

void MainWindow::on_pushButton_light_sensitive_get_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	m_orbFac.GetLightSensitive();
}

void MainWindow::on_pushButton_light_sensitive_open_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}

	m_bLightSensitiveOpen = !m_bLightSensitiveOpen;
	if (m_bLightSensitiveOpen)
	{
		ui_fac1->pushButton_light_sensitive_open->setText("close");
		m_orbFac.LightSensitiveCtl(true);
	}
	else
	{
		ui_fac1->pushButton_light_sensitive_open->setText("open");
		m_orbFac.LightSensitiveCtl(false);
	}

}

void MainWindow::on_checkBox_factory_mode_stateChanged(int arg1)
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}

	if (arg1 > 0)
	{
		m_orbFac.FactoryMode(true);
	}
	else
	{
		m_orbFac.FactoryMode(false);
	}
}

void MainWindow::on_pushButton_sdcard_get_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	m_orbFac.GetSDSize();
}

void MainWindow::on_checkBox_led_stateChanged(int arg1)
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	if (arg1 > 0)
	{
		m_orbFac.LedCtl(true);
	}
	else
	{
		m_orbFac.LedCtl(false);
	}
}

void MainWindow::on_pushButton_sensor_sta_get_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	m_orbFac.GetSensorSta();
}

#include "StreamPly.h"
#include "StreamDataTyp.h"


void MainWindow::on_pushButton_media_play_clicked()
{
	ShowNoteInfo("***main w=" + QString::number(m_mainWidth) + ",h=" + QString::number(m_mainHeight), 1, true);
	ShowNoteInfo("***sub w=" + QString::number(m_subWidth) + ",h=" + QString::number(m_subHeight), 1, true);

	m_bPlay = !m_bPlay;
	if (m_bPlay)
	{
		ui_fac2->pushButton_media_play->setText("stop");
		QString url = ui_fac2->lineEdit_media_url->text();
		if (!url.isEmpty())
		{

			int width = 1080, height = 1920;
			//0x01:主码流;0x02:子码流；0x03:第三码流
			uint8_t streamTyp = ui_fac2->comboBox_media_stream_typ->currentIndex();
			if (streamTyp == 0)
			{
				width = m_mainWidth;
				height = m_mainHeight;
			}
			if (streamTyp == 1)
			{
				width = m_subWidth;
				height = m_subHeight;
			}
			strcpy(m_streamUrl, url.toStdString().c_str());
			Play(m_streamUrl, MediaDstFmtRGBA, MediaShowFmtOpenGL, width, height);
			//snap path
			QDateTime localTime;
			localTime = QDateTime::currentDateTime();
			sprintf(m_savePicPath, "./PicData/pic_%d_%d_%d_%d_%d_%d", localTime.date().year(), localTime.date().month(), localTime.date().day(),
				localTime.time().hour(), localTime.time().minute(), localTime.time().second());

			//create save picture detectory
			if (m_bPicSave)
			{
				CheckDirect(m_savePicPath);
			}
		}
	}
	else
	{
		ui_fac2->pushButton_media_play->setText("play");
		Stop();

		m_snapPicNum = 0;
	}
}

void MainWindow::on_pushButton_wire_ip_set_clicked()
{
	QString ip = ui_fac2->lineEdit_wire_ip->text();
	QString gate = ui_fac2->lineEdit_wire_ip_gate->text();
	QString mask = ui_fac2->lineEdit_wire_ip_mask->text();

	ShowNoteInfo("ip=" + ip, 0, true);
	ShowNoteInfo("mask=" + mask, 0, true);
	ShowNoteInfo("gate=" + gate, 0, true);
	uint8_t ipAddr[4] = { 0 };
	int ip1, ip2, ip3, ip4;
	sscanf(ip.toStdString().c_str(), "%d.%d.%d.%d", (int*)&ip1, (int*)&ip2, (int*)&ip3, (int*)&ip4);
	ipAddr[0] = (uint8_t)ip1;
	ipAddr[1] = (uint8_t)ip2;
	ipAddr[2] = (uint8_t)ip3;
	ipAddr[3] = (uint8_t)ip4;


	uint8_t maskAddr[4] = { 0 };
	sscanf(mask.toStdString().c_str(), "%d.%d.%d.%d", (int*)&ip1, (int*)&ip2, (int*)&ip3, (int*)&ip4);
	maskAddr[0] = (uint8_t)ip1;
	maskAddr[1] = (uint8_t)ip2;
	maskAddr[2] = (uint8_t)ip3;
	maskAddr[3] = (uint8_t)ip4;

	uint8_t gateAddr[4] = { 0 };
	sscanf(gate.toStdString().c_str(), "%d.%d.%d.%d", (int*)&ip1, (int*)&ip2, (int*)&ip3, (int*)&ip4);
	gateAddr[0] = (uint8_t)ip1;
	gateAddr[1] = (uint8_t)ip2;
	gateAddr[2] = (uint8_t)ip3;
	gateAddr[3] = (uint8_t)ip4;
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	m_orbFac.SetWireIP(ipAddr, gateAddr, maskAddr);

}


void MainWindow::on_pushButton_media_snap_clicked()
{
	//create save picture detectory
	char savePath[256] = { 0 };
	sprintf(savePath, "%s/%d.bmp", m_savePicPath, m_snapPicNum++);
	ShowNoteInfo("snap:" + QString(savePath), 0, true);
	SnapPic(savePath, 0);

}

void MainWindow::on_pushButton_media_record_clicked()
{
	m_bRec = !m_bRec;
	if (m_bRec)
	{
		ui_fac2->pushButton_media_record->setText("StopRecord");
		RecordCtl(true);
	}
	else
	{
		ui_fac2->pushButton_media_record->setText("Record");
		RecordCtl(false);
	}
}

void MainWindow::on_pushButton_media_cur_frm_get_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	//0x01:主码流;0x02:子码流；0x03:第三码流
	uint8_t streamTyp = ui_fac2->comboBox_media_stream_typ->currentIndex();
	uint8_t typ;
	if (streamTyp == 0)
		typ = 0x01;
	if (streamTyp == 1)
		typ = 0x02;
	if (streamTyp == 2)
		typ = 0x03;
	m_orbFac.GetMediaCurFrm(typ);
}

void MainWindow::on_checkBox_msg_encrypt_stateChanged(int arg1)
{
	if (nullptr == m_pSerialProtocol)
		return;
	if (arg1)
	{
		//m_pSerialProtocol->SetMsgEncrypt(true);
	}
	else
	{
		//m_pSerialProtocol->SetMsgEncrypt(false);
	}
}

void MainWindow::EnableProtocolFunc(bool bEnable)
{
	ui_misc->checkBox_pir->setEnabled(bEnable);
	ui_misc->groupBox_pic_dir->setEnabled(bEnable);
#if 0
	ui->radioButton_lock_fake_lock->setEnabled(bEnable);
	ui->radioButton_lock_reset->setEnabled(bEnable);
	ui->radioButton_lock_protection->setEnabled(bEnable);
	ui->checkBox_event_param_cloud->setEnabled(bEnable);
	ui->checkBox_event_param_cap->setEnabled(bEnable);
#endif
	ui_cloud->pushButton_data_pass->setEnabled(bEnable);
	ui_cloud->lineEdit_data_pass->setEnabled(bEnable);
	ui_misc->pushButton_peephole_sleep->setEnabled(bEnable);
	if (ProductNumTypK1t == m_productNum)
		ui_cloud->lineEdit_product_secret->setEnabled(bEnable);
	if (ProductNumTypK1a == m_productNum)
		ui_net->checkBox_wifi_reserved->setEnabled(bEnable);

	ui_misc->checkBox_ota->setEnabled(bEnable);
}

void MainWindow::on_groupBox_protocol_clicked()
{
	EnableProtocolFunc(true);
	ui_net->checkBox_wifi_fac_mode->setEnabled(false);
	switch (m_protocolTypGp.checkedId())
	{
	case 0:
		m_protocolTyp = PeepholeProtocolTyp3V;
		ui->checkBox_msg_encrypt->setChecked(false);
		ui_net->checkBox_wifi_fac_mode->setEnabled(true);
		break;
	case 1:
		m_protocolTyp = PeepholeProtocolTypK1S;
		break;
	case 2:
		m_protocolTyp = PeepholeProtocolTypOrb;
		EnableProtocolFunc(false);
		break;
	default:
		break;
	}
}

void MainWindow::on_pushButton_orb_sn_get_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}

	ui_fac1->lineEdit_orb_sn->setText("");
	m_orbFac.GetSN();
}

void MainWindow::on_pushButton_orb_sn_set_clicked()
{
	if (!m_bDevOpen)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}

	QString snStr = ui_fac1->lineEdit_orb_sn->text();

	ShowNoteInfo(snStr, 1, true);
	ShowNoteInfo("snlen=" + QString::number(snStr.length()), 1, true);
	m_orbFac.SetSN((uint8_t*)snStr.toStdString().c_str(), snStr.length());
}

void MainWindow::on_pushButton_data_pass_clicked()
{
	if (!m_bDevOpen || nullptr == m_pSerialProtocol)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	std::string dataStr = ui_cloud->lineEdit_data_pass->text().toStdString();

	m_pSerialProtocol->DataPassToServer((uint8_t*)dataStr.c_str(), dataStr.length());
}

void MainWindow::on_checkBox_ota_stateChanged(int arg1)
{
	if (!m_bDevOpen || nullptr == m_pSerialProtocol)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}

	if (arg1 > 0)
	{
		m_pSerialProtocol->Ota(true);
	}
	else
	{
		m_pSerialProtocol->Ota(false);
	}
}

void MainWindow::on_pushButton_version_get_clicked()
{
	if (!m_bDevOpen || nullptr == m_pSerialProtocol)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	m_pSerialProtocol->GetVersion();
}

void MainWindow::on_pushButton_net_time_get_clicked()
{
	if (!m_bDevOpen || nullptr == m_pSerialProtocol)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}

	m_pSerialProtocol->GetNetTime();
}

void MainWindow::on_groupBox_wifi_action_typ_clicked()
{
	if (!m_bDevOpen || nullptr == m_pSerialProtocol)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	switch (m_wifiActionTypGp.checkedId())
	{
	case 0:
		m_pSerialProtocol->SetWifiKeepLive(wifi_action_typ_standard, false);
		break;
	case 1:
		m_pSerialProtocol->SetWifiKeepLive(wifi_action_typ_live, false);
		break;
	case 2:
		m_pSerialProtocol->SetWifiKeepLive(wifi_action_typ_sleep, false);
		break;
	default:
		break;
	}
}

void MainWindow::EnableRecordParam(bool bEnable)
{
#if 0
	ui->lineEdit_event_param_record_duration->setEnabled(bEnable);
	ui->checkBox_event_param_cap->setEnabled(bEnable);
	ui->checkBox_event_param_video->setEnabled(bEnable);
	ui->checkBox_event_paran_audio->setEnabled(bEnable);
	ui->checkBox_event_param_cloud->setEnabled(bEnable);
#endif
}

void MainWindow::EnableSnapParam(bool bEnable)
{
	//ui->lineEdit_event_param_snap_num->setEnabled(bEnable);
}

void MainWindow::on_groupBox_event_param_clicked()
{
	if (!m_bDevOpen || nullptr == m_pSerialProtocol)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	bool bRecEnable = false;
	bool bSnapEnable = false;
	switch (m_eventParamGp.checkedId())
	{
	case 0:// record
		bRecEnable = true;
		bSnapEnable = false;
		break;
	case 1: //snap
		bRecEnable = false;
		bSnapEnable = true;
		break;
	case 2: //none
		bRecEnable = false;
		bSnapEnable = false;
		break;
	default:
		break;
	}

	EnableRecordParam(bRecEnable);
	EnableSnapParam(bSnapEnable);
}

void MainWindow::on_pushButton_peephole_reset_2_clicked()
{
	if (!m_bDevOpen || nullptr == m_pSerialProtocol)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}

	m_pSerialProtocol->Reset();
}

void MainWindow::on_pushButton_dev_state_get_clicked()
{
	if (!m_bDevOpen || nullptr == m_pSerialProtocol)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	m_pSerialProtocol->GetDevState();
}

void MainWindow::on_checkBox_wifi_reserved_stateChanged(int arg1)
{
	ui_net->lineEdit_wifi_reserved->setEnabled(arg1 > 0);
}

void MainWindow::on_checkBox_scan_conf_stateChanged(int arg1)
{
	m_pSerialProtocol->ScanConf(arg1 > 0);
}

bool MainWindow::atoi2(const char* str, int* pOut)
{
	int output = 0;
	int base = 16;
	int start = 0;

	if (strlen(str) > 1 && str[0] == '0' && str[1] == 'x')
	{
		start = 2;
		base = 16;
	}

	for (size_t i = start; i < strlen(str); i++)
	{
		output *= base;
		if (str[i] >= '0' && str[i] <= '9')
			output += str[i] - '0';
		else if (base == 16 && str[i] >= 'a' && str[i] <= 'z')
			output += 10 + str[i] - 'a';
		else if (base == 16 && str[i] >= 'A' && str[i] <= 'Z')
			output += 10 + str[i] - 'A';
		else
			return false;
	}
	*pOut = output;
	return true;
}

int MainWindow::ParseCommStr(QString commStr, uint8_t* pCommBuf, int bufLen)
{
	QString pattern("[A-Za-z0-9]");
	QRegExp rx(pattern);
	int pos = 0;
	if (commStr.count() > bufLen)
	{
		return -1;
	}
	if (commStr.count() <= 0) {
		return 0;
	}
	uint8_t* pBuf = pCommBuf;
	int tmp = 0, len = 0;
	while ((pos = rx.indexIn(commStr, pos)) != -1)
	{
		QString str = rx.capturedTexts().at(0);
		pos += rx.matchedLength();

		atoi2(str.toStdString().c_str(), &tmp);
		*pBuf = (uint8_t)tmp;

		pBuf++;
		len++;
	}

	return len;
}

void MainWindow::InitComboBoxItems(QComboBox* combo, const QVector<QPair<QString, qint8>>& items_custom)
{
	combo->clear();
	for (auto& event : items_custom) {
		combo->addItem(event.first, event.second);
	}
	combo->setCurrentIndex(0);
}

int MainWindow::Char2Hex(uint8_t* buf, int buflen, char* out, int& outlen)
{
	if ((buflen % 2) || (NULL == out))
	{
		return -1;
	}

	QString note;
	int sz = buflen / 2;

	outlen = sz;
	int j = 0;
	for (int i = 0; i < sz; i++)
	{
		out[i] = buf[j] * 16 + buf[j + 1];
		//note.sprintf("0x%x",hexComm->comm[i]);
		//emit ShowNoteInfo(note);
		j += 2;
	}
	return 1;
}

void MainWindow::on_pushButton_protocol_extend_send_clicked()
{
	int datalen = 0;
	uint8_t data[2048] = { 0 };

	uint8_t commBuf[2048] = { 0 };
	int len = 0;

	QString cmdStr = ui_cloud->lineEdit_protocol_extend->text();
	memset(commBuf, 0, sizeof(commBuf));
	len = ParseCommStr(cmdStr, commBuf, 2048);
	Char2Hex(commBuf, len, (char*)data, datalen);

	if (datalen > 0)
	{
		m_pSerialProtocol->ProtocolExtend(data, datalen);
	}
}

void MainWindow::on_pushButton_alarm_send_clicked()
{
	//MediaCapEventParam alarmParam;
	//memset(&alarmParam, 0, sizeof(alarmParam));

	//alarmParam.eventid = static_cast<uint16_t>(ui->lineEdit_alarm_id->text().toInt());
	//alarmParam.event = event_alarm_typ_alarm;
	//alarmParam.eventtyp = static_cast<uint8_t>(ui->comboBox_alarm_typ->currentIndex());
	//alarmParam.opentyp = static_cast<uint8_t>(ui->comboBox_alarm_open_typ->currentIndex());
	//alarmParam.userid = static_cast<uint8_t>(ui->lineEdit_alarm_user_id->text().toInt());
	//alarmParam.action = static_cast<uint8_t>(ui->comboBox_alarm_action->currentIndex());
	//alarmParam.actionParam = static_cast<uint32_t>(ui->lineEdit_alarm_action_param->text().toInt());

	//m_pSerialProtocol->EventProc(alarmParam.event, alarmParam);
}

void MainWindow::on_pushButton_event_send_clicked()
{
	if (!m_bDevOpen) {
		QMessageBox::information(this, "提示", "设备未连接!");
		return;
	}
	MediaCapEventParam eventParam;
	memset(&eventParam, 0, sizeof(eventParam));

	eventParam.eventid = ui_event->spinBox_event_id->value();
	eventParam.event = m_eventTyp;
	if (m_eventTyp == event_alarm_typ_battery)
	{
		eventParam.opentyp = ui_event->spinBox_BAT1_percent->value();
		eventParam.userid = ui_event->spinBox_BAT2_percent->value();
	}
	else
	{
		eventParam.opentyp = static_cast<uint8_t>(ui_event->comboBox_event_open_typ->currentIndex());
		eventParam.userid = ui_event->spinBox_key_id->value();
	}

	eventParam.eventtyp = ui_event->comboBox_event_code->currentIndex();
	eventParam.action = static_cast<uint8_t>(ui_event->comboBox_event_action->currentIndex());
	eventParam.actionParam = ui_event->spinBox_event_action_param->value();
	eventParam.offline = ui_event->checkBox_event_offline->isChecked();

	if (ui_event->checkBox_timestamp->checkState() == Qt::Checked)
	{
		eventParam.ts = QDateTime::currentDateTime().toTime_t();
	}
	m_pSerialProtocol->EventProc(eventParam.event, eventParam);
}

void MainWindow::on_pushButton_person_detect_state_get_clicked()
{
	m_pSerialProtocol->GetPersonDetectSta();
}

void MainWindow::on_pushButton_mask_detect_state_get_clicked()
{
	m_pSerialProtocol->GetMaskDetectSta();
}

void MainWindow::on_pushButton_look_up_remind_clicked()
{
	uint8_t typ = static_cast<uint8_t>(ui_misc->comboBox_remind_open_typ->currentIndex());
	uint8_t id = static_cast<uint8_t>(ui_misc->lineEdit_remind_user_id->text().toInt());
	m_pSerialProtocol->LookupCloudRemind(typ, id);
}

void MainWindow::on_groupBox_wifi_live_face_clicked()
{
	if (!m_bDevOpen || nullptr == m_pSerialProtocol)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	switch (m_wifiActionFaceTypGp.checkedId())
	{
	case 0:
		m_pSerialProtocol->SetWifiKeepLive(wifi_action_typ_live, true);
		break;
	case 1:
		m_pSerialProtocol->SetWifiKeepLive(wifi_action_typ_sleep, true);
		break;
	default:
		break;
	}
}

void MainWindow::on_pushButton_device_sn_get_clicked()
{
	if (!m_bDevOpen || nullptr == m_pSerialProtocol)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	m_pSerialProtocol->GetDeviceSN();
}

void MainWindow::on_pushButton_fov_set_clicked()
{
	int fov = ui_misc->lineEdit_fov_set->text().toInt();
	if (fov < 1 || fov >100)
	{
		ShowNoteInfo("FOV值错误!", 0, true);
		return;
	}
	if (!m_bDevOpen || nullptr == m_pSerialProtocol)
	{
		ShowNoteInfo("串口未打开!", 0, true);
		return;
	}
	m_pSerialProtocol->SetFov(fov);
}

void MainWindow::on_pushButton_battery_level_send_clicked()
{
	//MediaCapEventParam eventParam;
	//memset(&eventParam, 0, sizeof(eventParam));
	////00:大小电池正常，01:大电池电量低 02:小电池电量低 03：大小电池电量低
	//eventParam.event = event_alarm_typ_battery;
	//int bigSta = ui->comboBox_big_battery_sta->currentIndex();
	//int smallSta = ui->comboBox_small_battery_sta->currentIndex();

	//if (bigSta == 1)
	//	eventParam.eventtyp = 0x01;
	//if (smallSta == 1)
	//	eventParam.eventtyp = 0x02;
	//if (bigSta == 1 && smallSta == 1)
	//	eventParam.eventtyp = 0x03;

	//eventParam.opentyp = static_cast<uint8_t>(ui->lineEdit_big_battery_level->text().toInt());
	//eventParam.userid = static_cast<uint8_t>(ui->lineEdit_small_battery_level->text().toInt());

	//m_pSerialProtocol->EventProc(eventParam.event, eventParam);
}

void MainWindow::on_pushButton_key_get_clicked()
{
	m_pSerialProtocol->GetCloudKey();
	//m_orbFac.GetCloudKey();
}

void MainWindow::on_groupBox_event_type_clicked()
{
	switch (m_eventTypGp.checkedId())
	{
		//case 0: //common event type
		//	ui->comboBox_event_typ->setEnabled(true);
		//	ui->comboBox_event_typ_freeze->setEnabled(false);
		//	m_eventTyp = 0;
		//	break;
		//case 1: //freeze
		//	ui->comboBox_event_typ->setEnabled(false);
		//	ui->comboBox_event_typ_freeze->setEnabled(true);
		//	m_eventTyp = 1;
		//	break;
	default:
		break;
	}
}

void MainWindow::on_pushButton_user_data_send_clicked()
{
	uint8_t eventTyp = ui_user->comboBox_user_data_event_typ->currentIndex() + 1;
	uint8_t userTyp = ui_user->comboBox_user_data_user_typ->currentIndex() + 1;
	uint8_t authTyp = ui_user->comboBox_user_data_auth_typ->currentIndex() + 1;
	uint8_t mainId = ui_user->spinBox_user_data_main_id->value();
	uint8_t subId = ui_user->spinBox_user_data_sub_id->value();

	m_pSerialProtocol->SetUserData(eventTyp, userTyp, authTyp, mainId, subId);
}

void MainWindow::on_comboBox_event_type_currentIndexChanged(int arg1)
{
	int typ = ui_event->comboBox_event_type->itemData(arg1).toInt();
	switch (typ)
	{
	case event_alarm_typ_freeze:
		InitFreezeParam();
		break;
	case event_alarm_typ_alarm:
		InitAlarmParam();
		break;
	case event_alarm_typ_event:
		InitEventParam();
		break;
	case event_alarm_typ_battery:
		InitBatteryParam();
		break;
	default:
		break;
	}

	bool bEnable(typ != event_alarm_typ_battery);
	ui_event->comboBox_event_open_typ->setEnabled(bEnable);
	ui_event->spinBox_key_id->setEnabled(bEnable);
	ui_event->comboBox_event_action->setEditable(bEnable);
	ui_event->spinBox_event_action_param->setEnabled(bEnable);

	m_eventTyp = typ;
}


bool is_hex_str(const QString& str)
{
	static QRegExp hex_regexp("^[0-9A-Fa-f]+$");
	bool bhex = (-1 != hex_regexp.indexIn(str));
	return bhex;
}

QByteArray inputstr_convert(const QString& str)
{
	QByteArray result(str.toUtf8());
	// 去除空格
	QStringList strlist = str.split(' ', Qt::SkipEmptyParts);
	if (strlist.size() == 1) {
		// 未找到空格
		return result;
	}

	QByteArray bytebuff;
	for (auto& str : strlist) {
		bytebuff.push_back(str.toUtf8());
	}
	// 调用通信串口进行发送
	if (is_hex_str(bytebuff)) {
		result.clear();
		for (const auto& ch : strlist)
		{
			bool ok;
			uint8_t _hex = ch.toInt(&ok, 16);
			result.push_back(_hex);
		}
	}
	return result;
}

void MainWindow::on_pushButton_send_cmd_clicked()
{
	QString sendCmdStr = ui->textEdit_send_cmd->toPlainText();
	if (sendCmdStr.isEmpty())
	{
		QMessageBox::information(nullptr, "提示", "待发送命令为空,请重新输入!");
		return;
	}
	QByteArray sendData = inputstr_convert(sendCmdStr);
	m_pSerialProtocol->SendData((uint8_t*)sendData.data(), sendData.size());
}
