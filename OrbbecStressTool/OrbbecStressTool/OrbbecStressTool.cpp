#include "OrbbecStressTool.h"
#include "DevLogManage.h"
#include "TaskManage.h"
#include "DevComConfig.h"
#include "common.h"
#include <QMessageBox>
#include <thread>
#include <QThread>
#include <iostream>
#include <QProgressBar>

#include <QDebug>
#include "relay/RelayProtocol.hpp"

OrbbecStressTool::OrbbecStressTool(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	initUi();

	spdlog::debug("{} thread id {}", __FUNCTION__, QThread::currentThreadId());

}

OrbbecStressTool::~OrbbecStressTool()
{

}

void OrbbecStressTool::initUi()
{
	// 设备日志窗口
	m_devLogWidget = new DevLogManage(this);
	QHBoxLayout* log_hlayout = new QHBoxLayout();
	log_hlayout->addWidget(m_devLogWidget);
	log_hlayout->setContentsMargins(0, 0, 0, 0);
	ui.frame_dev_log->setLayout(log_hlayout);

	// 任务信息窗口
	//m_threadTaskManage = new QThread();
	m_taskManageWidget = new TaskManage(this);
	//m_taskManageWidget->moveToThread(m_threadTaskManage);

	//connect(m_threadTaskManage, SIGNAL(finished()), m_threadTaskManage, SLOT(deleteLater()));
	//connect(m_threadTaskManage, SIGNAL(finished()), m_taskManageWidget, SLOT(deleteLater()));

	QHBoxLayout* task_hlayout = new QHBoxLayout(this);
	task_hlayout->addWidget(m_taskManageWidget);
	task_hlayout->setContentsMargins(0, 0, 0, 0);
	ui.frame_task->setLayout(task_hlayout);

	// 串口连接信息
	//m_threadDevCom = new QThread();
	m_comsConfig = new DevComConfig(this);
	//m_comsConfig->moveToThread(m_threadDevCom);
	//connect(m_threadDevCom, SIGNAL(finished()), m_threadDevCom, SLOT(deleteLater()));
	//connect(m_threadDevCom, SIGNAL(finished()), m_comsConfig, SLOT(deleteLater()));
	m_comsConfig->setModal(true);
	//m_threadDevCom->start();

	// 菜单栏初始化
	initMenu();

	// 参数信息窗口
	initParam();

	// 状态栏初始化
	initStausBar();


	// 信号槽初始化
	connect(m_devLogWidget, &DevLogManage::send, m_comsConfig, &DevComConfig::send, Qt::DirectConnection);
	connect(m_devLogWidget, &DevLogManage::recv, m_comsConfig, &DevComConfig::recv, Qt::DirectConnection);

	connect(m_taskManageWidget, &TaskManage::send, m_comsConfig, &DevComConfig::send);
	connect(m_taskManageWidget, &TaskManage::recv, m_comsConfig, &DevComConfig::recv);
	connect(m_taskManageWidget, &TaskManage::relay_sw, this, &OrbbecStressTool::relay_sw);
	connect(m_taskManageWidget, &TaskManage::reopen_serial, this, &OrbbecStressTool::reopen_serial);
	connect(m_taskManageWidget, &TaskManage::update_test_process, this, &OrbbecStressTool::update_test_process);
	connect(this, &OrbbecStressTool::start, m_taskManageWidget, &TaskManage::Start);
	connect(this, &OrbbecStressTool::stop, m_taskManageWidget, &TaskManage::Stop);
	//m_threadTaskManage->start();

	connect(this, &OrbbecStressTool::send, m_comsConfig, &DevComConfig::send, Qt::DirectConnection);
	connect(this, &OrbbecStressTool::recv, m_comsConfig, &DevComConfig::recv, Qt::BlockingQueuedConnection);

	connect(m_comsConfig, &DevComConfig::sync_com_stat, this, [=](int dev_type, bool stat) {
		if (dev_type == DEV_COM_TYP_CMMC) {
			m_taskManageWidget->sync_com_stat(stat);
			m_dev_com_stat = stat;
		}
		else if (dev_type == DEV_COM_TYP_DEBUG) {
			m_devLogWidget->sync_com_stat(stat);
		}
		else if (dev_type == DEV_COM_TYP_RELAY) {
			m_relay_com_stat = stat;
			ui.groupBox_relay->setEnabled(stat);
		}
		else
		{

		}
		}
	);

	spdlog::debug("{} thread id {}", __FUNCTION__, QThread::currentThreadId());

}

void OrbbecStressTool::initStausBar()
{
	m_workstat_icon = new QLabel(this);
	m_test_result_info = new QLabel(this);
	m_test_result_info->setText("S/F: 0/0");

	m_test_progress = new QProgressBar(this);
	m_test_progress->setRange(0, 100);
	m_test_progress->setTextVisible(false);
	m_test_progress->setMaximumSize(50, 12);
	const QString styleSheet = "QProgressBar {"
		"    border-radius: 2px;"
		"    border: 1px solid grey; "
		"}";
	m_test_progress->setStyleSheet(styleSheet);
	connect(this, &OrbbecStressTool::update_test_process, this, [=](int process, uint64_t failed, uint64_t success) {
		if (process == -1) {
			set_workstate(TaskWorkState::READY);
		}
		else {
			int percent = calculatePercentage(process, ui.spinBox_test_nums->value());
			m_test_progress->setValue(percent);
			m_test_result_info->setText(QString("S/F: %1/%2").arg(QString::number(success)).arg(QString::number(failed)));

		}}
	);

	statusBar()->setSizeGripEnabled(true);
	statusBar()->addPermanentWidget(m_workstat_icon);
	statusBar()->addPermanentWidget(m_test_progress);
	statusBar()->addPermanentWidget(m_test_result_info);

	set_workstate(TaskWorkState::READY);
}

void OrbbecStressTool::initParam()
{
	ui.spinBox_test_nums->setValue(100);
	ui.spinBox_recv_max_timeout->setValue(5000);

	ui.comboBox_relay_type->addItems({ "USB","继电器A","继电器B" });
	ui.comboBox_relay_type->setCurrentIndex(0);

	ui.groupBox_relay->setEnabled(false);
}

void OrbbecStressTool::relay_sw_ctrl(int ch, bool sw)
{
	std::vector<uint8_t> cmd;
	RelayProtocol::GetSwitchCmd(ui.comboBox_relay_type->currentIndex(), ch, sw, cmd);
	emit send(DEV_COM_TYP_RELAY, QByteArray((char*)cmd.data(), cmd.size()));
}

void OrbbecStressTool::initMenu()
{
	// 菜单栏响应绑定
	connect(this->menuBar(), &QMenuBar::triggered, this, [=](QAction* action) {
		if (action->text() == "设备配置") {
			m_comsConfig->show();
		}
		else if (action->text() == "退出") {
			spdlog::info("sys quit success!");
			qApp->exit();
		}
		else if (action->text() == "日志") {

		}
		});


	ui.actiondebug_log->setCheckable(true);
	ui.actioninfo_log->setCheckable(true);
	ui.actioninfo_log->setChecked(true); // 默认info打印
	connect(ui.actiondebug_log, &QAction::triggered, this, [=](bool checked) {
		if (checked) {
			spdlog::set_level(spdlog::level::trace);
			QSettings setting;
			setting.setValue("log_level", spdlog::level::trace);
		}
		ui.actioninfo_log->setChecked(!checked);
		}
	);

	connect(ui.actioninfo_log, &QAction::triggered, this, [=](bool checked) {
		if (checked) {
			spdlog::set_level(spdlog::level::info);
			QSettings setting;
			setting.setValue("log_level", spdlog::level::info);
		}
		ui.actiondebug_log->setChecked(!checked);
		}
	);
}

void OrbbecStressTool::on_pushButton_start_stop_clicked()
{
	if (!m_dev_com_stat) {
		QMessageBox::information(this, "提示", "设备串口未打开!");
		return;
	}

	if (!m_taskManageWidget->WorkStat()) {
		int test_nums = ui.spinBox_test_nums->value();
		int max_recv_timeout = ui.spinBox_recv_max_timeout->value();
		bool relay_enable = (ui.checkBox_relay_enable->checkState() == Qt::Checked);
		if (relay_enable && !m_relay_com_stat) {
			QMessageBox::information(this, "提示", "继电器串口未打开！请重试！");
			return;
		}
		emit update_test_process(0, 0, 0);
		emit start(test_nums, max_recv_timeout, relay_enable);
		set_workstate(TaskWorkState::RUNNING);
	}
	else {
		auto ret = QMessageBox::information(this, "提示", "压测进行中，是否确定停止?", QMessageBox::Yes, QMessageBox::No);
		if (ret == QMessageBox::Yes) {
			emit stop();
			set_workstate(TaskWorkState::READY);
		}
	}


}


void OrbbecStressTool::on_comboBox_relay_type_currentTextChanged()
{
	auto enableRealyChCheckBox = [=](bool bSet) {
		ui.checkBox_ch1->setEnabled(bSet);
		ui.checkBox_ch2->setEnabled(bSet);
		ui.checkBox_ch3->setEnabled(bSet);
		ui.checkBox_ch4->setEnabled(bSet);
	};

	auto clearRealyChCheckBox = [=]() {
		ui.checkBox_ch1->setChecked(false);
		relay_sw_ctrl(1, false);
		ui.checkBox_ch2->setChecked(false);
		relay_sw_ctrl(2, false);
		ui.checkBox_ch3->setChecked(false);
		relay_sw_ctrl(3, false);
		ui.checkBox_ch4->setChecked(false);
		relay_sw_ctrl(4, false);
	};

	switch (ui.comboBox_relay_type->currentIndex())
	{
	case RELAY_TYP_USB:
		enableRealyChCheckBox(false);
		ui.checkBox_ch1->setEnabled(true);
		break;
	case RELAY_TYP_MULA:
		enableRealyChCheckBox(true);
		clearRealyChCheckBox();
		break;
	case RELAY_TYP_MULB:
		enableRealyChCheckBox(true);
		clearRealyChCheckBox();
		break;
	default:
		break;
	}
	//ui.checkBox_ch1->setChecked(true);
}

void OrbbecStressTool::on_checkBox_ch1_stateChanged(int stage)
{
	relay_sw_ctrl(1, stage);
}

void OrbbecStressTool::on_checkBox_ch2_stateChanged(int stage)
{
	relay_sw_ctrl(2, stage);
}

void OrbbecStressTool::on_checkBox_ch3_stateChanged(int stage)
{
	relay_sw_ctrl(3, stage);
}

void OrbbecStressTool::on_checkBox_ch4_stateChanged(int stage)
{
	relay_sw_ctrl(4, stage);
}

void OrbbecStressTool::relay_sw(int ch, bool sw_on)
{
	relay_sw_ctrl(ch, sw_on);
}

void OrbbecStressTool::reopen_serial(int dev_typ)
{
	m_comsConfig->reopen_com_serial(dev_typ);
}

void OrbbecStressTool::set_workstate(TaskWorkState stat)
{
	if (stat == TaskWorkState::READY || stat == TaskWorkState::FINISH) {
		statusBar()->showMessage("空闲");
		m_workstat_icon->setPixmap(QPixmap(":/res/ready.png"));
		ui.pushButton_start_stop->setIcon(QIcon(":/res/start_task.png"));
		ui.pushButton_start_stop->setText("开始");
	}
	else if (stat == TaskWorkState::RUNNING) {
		statusBar()->showMessage("运行中");
		m_workstat_icon->setPixmap(QPixmap(":/res/work.png"));
		ui.pushButton_start_stop->setIcon(QIcon(":/res/stop_task.png"));
		ui.pushButton_start_stop->setText("停止");
	}
}
