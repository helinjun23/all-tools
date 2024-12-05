/**
 * @file TaskPolicy.h
 * @brief 提供任务压测策略
 * @author xuenuo (xuenuo@orbbec.com)
 * @version 1.0
 * @date 2023-08-09
 *
 * @copyright Copyright (c) ORBBEC Technology Group Co., Ltd. All rights reserved
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2023-08-09 <td>1.0     <td>wangh     <td>内容
 * </table>
 */
#pragma once

#include "common.h"

#include <QObject>

struct stTaskData
{
	int id;
	uint8_t pwr_sw;
	uint8_t chn;
	uint32_t pwr_on_duration_ms;
	uint32_t send_duration_ms;
	uint32_t pwr_off_duration_ms;
	std::string note;
	std::string cmd;
	stTaskData()
		: id(-1), pwr_sw(0), chn(1), pwr_on_duration_ms(1000),
		send_duration_ms(1000), pwr_off_duration_ms(1000),
		note(""), cmd("") {	}
};

class TaskPolicy : public QObject
{
	Q_OBJECT

public:
	TaskPolicy(QObject* parent = nullptr);
	~TaskPolicy();

	void SetRunParam(uint32_t max_timeout_ms, bool relay_en);
	bool GetWorkRunStat();
Q_SIGNALS:
	void update_test_process(int, uint64_t, uint64_t);
	void reopen_serial(int);
	bool send(int, const QByteArray&);
	void recv(int, QByteArray&);
	void relay_sw(int, bool);

	void disbaled_ui_action(bool);
	void select_highlight_row(int);

public Q_SLOTS:
	void start(const QVector<stTaskData>& tasks, uint32_t test_times);
	void stop();
	bool send_data(const QByteArray& data);
	void recv_data(QByteArray& data);

private:
	void relay_sw_handle(bool pwr_on);
	bool doTaskPolicy(const QVector<stTaskData>& tasks);



private:
	QAtomicInteger<bool> m_runState = false;
	bool m_relay_en = false; // 使能继电器上下电控制
	stTaskData m_current_task_item;
	uint32_t m_max_recv_timeout = 5000;
	uint64_t m_success_cmd_nums = 0;
	uint64_t m_fail_cmd_nums = 0;
};
