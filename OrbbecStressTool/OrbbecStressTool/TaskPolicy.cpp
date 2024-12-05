#include "TaskPolicy.h"

TaskPolicy::TaskPolicy(QObject* parent)
	: QObject(parent)
{}

TaskPolicy::~TaskPolicy()
{}

void TaskPolicy::SetRunParam(uint32_t max_timeout_ms, bool relay_en)
{
	m_max_recv_timeout = max_timeout_ms;
	m_relay_en = relay_en;
}

bool TaskPolicy::GetWorkRunStat()
{
	return m_runState;
}

void TaskPolicy::stop()
{
	m_runState = false;
}

void TaskPolicy::start(const QVector<stTaskData>& tasks, uint32_t test_times)
{
	//2. 执行任务
	emit disbaled_ui_action(false);
	m_runState = true;
	m_fail_cmd_nums = 0;
	m_success_cmd_nums = 0;
	constexpr int pwr_wait_ms = 3000; // default 3000ms

	if (m_relay_en) {
		emit relay_sw(1, true);
		QThread::msleep(pwr_wait_ms);
		spdlog::info("relay power on wait {} ms", pwr_wait_ms);
	}

	for (int current_time = 1; current_time <= test_times && m_runState; current_time++) {
		spdlog::info("==========================start time {} auto stress====================", current_time);
		if (!doTaskPolicy(tasks)) {
			m_fail_cmd_nums++;
		}
		else {
			m_success_cmd_nums++;
		}
		spdlog::info("==========================stop time {} auto stress====================\n", current_time);
		emit update_test_process(current_time, m_fail_cmd_nums, m_success_cmd_nums);
	}

	if (m_relay_en) {
		emit relay_sw(1, false);
		QThread::msleep(pwr_wait_ms);
		spdlog::info("relay power off wait {} ms", pwr_wait_ms);
	}

	//3. 统计执行结果
	m_runState = false;
	emit disbaled_ui_action(true);
	emit update_test_process(-1, m_fail_cmd_nums, m_success_cmd_nums);
}

bool TaskPolicy::doTaskPolicy(const QVector<stTaskData>& tasks)
{
	int succ = 0, fail = 0;
	//2. 执行列表任务
	for (const auto& task_row : tasks) {
		if (!m_runState) {
			spdlog::warn("half-stopping!!!");
			return false;
		}

		m_current_task_item = task_row;

		emit select_highlight_row(m_current_task_item.id - 1);
		// 获取命令码数据
		QString cmd_data = QString::fromUtf8(m_current_task_item.cmd.c_str());
		QByteArray sendbuff = inputstr_convert(cmd_data);

		spdlog::info("==========================begin task id {} auto stress====================", m_current_task_item.id);

		// 上电控制
		relay_sw_handle(true);
		// reopen通信串口，重新上电后，串口需要重新打开
		emit reopen_serial(DEV_COM_TYP_CMMC);

		do {
			// 指令为空时,不发送
			if (sendbuff.isEmpty()) {
				break;
			}
			// 发送数据
			if (!send_data(sendbuff)) {
				spdlog::warn("send data failed, go on next!");
				fail++;
				break;
			}

			// 接收数据
			QByteArray byte_recv;
			utils::TimeDuration timespan;
			do {
				recv_data(byte_recv);
				if (timespan.duration_ms() > m_max_recv_timeout) {
					spdlog::info("recv timeout [{}ms]...", m_max_recv_timeout);
					break;
				}
			} while (byte_recv.isEmpty() && m_runState);

			if (byte_recv.isEmpty()) {
				fail++;
				break;
			}

			// 发送间隔
			int send_duraiton = m_current_task_item.send_duration_ms;
			spdlog::info("cmd send wait {} ms", send_duraiton);
			QThread::msleep(send_duraiton);
		} while (0);

		// 下电控制
		relay_sw_handle(false);
		spdlog::info("==========================end task id {} auto stress====================", m_current_task_item.id);

		succ++;
	}

	if (fail != 0) {
		return false;
	}

	spdlog::info("====success {} failed {}====", succ, fail);

	return true;
}

void TaskPolicy::relay_sw_handle(bool pwr_on)
{
	if (!m_relay_en) {
		return;
	}

	if (m_current_task_item.pwr_sw) {
		// 上/下电控制
		emit relay_sw(m_current_task_item.chn, pwr_on);
		int pwr_wait_ms = pwr_on ? m_current_task_item.pwr_on_duration_ms : m_current_task_item.pwr_off_duration_ms;
		// 上/下电延时
		QThread::msleep(pwr_wait_ms);
		spdlog::info("relay ch{} power {} wait {} ms", m_current_task_item.chn, pwr_on ? "on" : "off", pwr_wait_ms);
	}
}

bool TaskPolicy::send_data(const QByteArray& data)
{
	spdlog::debug("{} thread id {}", __FUNCTION__, QThread::currentThreadId());

	if (data.isEmpty()) {
		return false;
	}
	PrintByteArray("send: ", data, 1);
	emit send(DEV_COM_TYP_CMMC, data);
	return true;
}

void TaskPolicy::recv_data(QByteArray& data)
{
	spdlog::debug("{} thread id {}", __FUNCTION__, QThread::currentThreadId());

	int retry = 100 * 3;
	do {
		emit recv(DEV_COM_TYP_CMMC, data);
		QThread::msleep(10);
	} while (data.isEmpty() && retry--);
	PrintByteArray("recv: ", data, 1);
}