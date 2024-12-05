#pragma once

#include "common.h"
#include "TaskPolicy.h"

#include <QThread>
#include <QAtomicInteger>
#include <QWidget>
#pragma execution_character_set("utf-8")

class QTableWidget;
class QMenu;
class TaskManage : public QWidget
{
	Q_OBJECT
public:
	TaskManage(QWidget* parent = nullptr);
	~TaskManage();


	bool WorkStat() {
		return m_taskPolicy->GetWorkRunStat();
	}
	void sync_com_stat(bool stat);
Q_SIGNALS:
	void send(int, const QByteArray&);
	void recv(int, QByteArray&);
	void start_work(const QVector<stTaskData>& tasks, uint32_t test_times);
	void relay_sw(int, bool);
	void reopen_serial(int);
	void update_test_process(int, uint64_t, uint64_t);
	bool send_data(const QByteArray& data);
	void recv_data(QByteArray& data);

public Q_SLOTS:
	void auto_run_task_lists();

	void Start(int test_nums, int max_recv_timeout, bool relay_en);
	void Stop();

private:
	void initUi();
	void add_row_data(int row, int idx, const stTaskData& data);
	void delete_last_row();
	void select_all_item();
	void delete_all_data();

	void add_data(const stTaskData& data);
	void delete_data();
	void select_data();
	void load_data();
	void save_data();

	QString get_task_cmd_data(int row);
	bool get_task_checked(int row);
	int get_task_send_duration(int row);
	int get_task_pwr_on_wait(int row);
	int get_task_pwr_off_wait(int row);
	int get_task_id(int row);
	QString get_task_note(int row);

	// 通道号: 开关状态
	std::pair<int, bool> get_task_sw_checked(int row) const;

	void get_row_data(int row, stTaskData& data);
	QStringList get_row_data(int row);
	void initCmdCompleter();
private:
	QTableWidget* m_taskTable = nullptr;
	QMenu* m_menu = nullptr;

	QAtomicInteger<bool> m_runState = false;

	int m_current_task_id = -1;
	bool m_relay_en = false;
	QAtomicInteger<bool> m_comStat = false;
	int m_test_nums = 0;
	int m_max_recv_timeout = 1000;

	QThread* m_threadRun;
	TaskPolicy* m_taskPolicy;
	QCompleter* m_cmdCompleter;
};