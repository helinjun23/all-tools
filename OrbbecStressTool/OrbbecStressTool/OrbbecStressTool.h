#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OrbbecStressTool.h"

#pragma execution_character_set("utf-8")

class QProgressBar;
class DevComConfig;
class TaskManage;
class DevLogManage;
class OrbbecStressTool : public QMainWindow
{
	Q_OBJECT

public:
	OrbbecStressTool(QWidget* parent = nullptr);
	~OrbbecStressTool();

	enum class TaskWorkState :uint8_t { READY, RUNNING, FINISH };
private:
	void initMenu();
	void initUi();
	void initParam();
	void initStausBar();

	void relay_sw_ctrl(int ch, bool sw);


Q_SIGNALS:
	void send(int dev_typ, const QByteArray&);
	void recv(int dev_typ, QByteArray&);
	void update_test_process(int, uint64_t, uint64_t);

	void start(int test_nums, int max_recv_timeout, bool relay_en);
	void stop();
public Q_SLOTS:
	void on_pushButton_start_stop_clicked();
	void on_comboBox_relay_type_currentTextChanged();
	void on_checkBox_ch1_stateChanged(int stage);
	void on_checkBox_ch2_stateChanged(int stage);
	void on_checkBox_ch3_stateChanged(int stage);
	void on_checkBox_ch4_stateChanged(int stage);

	void relay_sw(int ch, bool sw_on);
	void reopen_serial(int dev_typ);

	void set_workstate(TaskWorkState stat);
private:
	Ui::OrbbecStressToolClass ui;

	DevLogManage* m_devLogWidget = nullptr;
	TaskManage* m_taskManageWidget = nullptr;
	DevComConfig* m_comsConfig = nullptr;

	bool m_relay_com_stat = false;
	bool m_dev_com_stat = false;

	QThread* m_threadDevCom;
	QThread* m_threadTaskManage;

	/*×´Ì¬À¸Ïà¹Ø¿Ø¼þ*/
	QLabel* m_workstat_icon;
	QLabel* m_test_result_info;
	QProgressBar* m_test_progress;

};
