#pragma once

#include "common.h"

#include <QWidget>
#include <QMenu>
#include <thread>
#include <QTextStream>
#include <QIODevice>
#include <QFile>


#pragma execution_character_set("utf-8")

class QPushButton;
class QLineEdit;
class QTextEditEx;

class DevLogWork :public QObject
{
	Q_OBJECT
public:
	DevLogWork(QObject* parent = nullptr) :QObject(parent) {}
	~DevLogWork() {}

	void SetComStat(bool enable) {
		m_comStat = enable;
	}
Q_SIGNALS:
	void send(int, const QByteArray&);
	void recv(int, QByteArray&);
	void append_data(const QString&);

public Q_SLOTS:
	void start() {
		m_runState = true;
		thread_recv_proc();
	}
	void stop() {
		m_runState = false;
	}
private:
	void thread_recv_proc()
	{
		while (m_runState)
		{
			if (!m_comStat) {
				QThread::msleep(100);
				continue;
			}

			static uint64_t count = 0;
			QByteArray recv_buf;
			emit recv(DEV_COM_TYP_DEBUG, recv_buf);
			if (!recv_buf.isEmpty()) {
				//QString now_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
				QString log_data = QString::fromUtf8(recv_buf);
				// 消除全空格数据
				//QStringList data_list = log_data.split(' ', Qt::SkipEmptyParts);
				//if (!data_list.isEmpty()) {
				emit append_data(QString("%1").arg(log_data));
				//}
			}
			QThread::msleep(10);
		}
	}
private:
	bool m_runState = false;
	QAtomicInteger<bool> m_comStat = false;

};

class DevLogManage : public QWidget
{
	Q_OBJECT

public:
	DevLogManage(QWidget* parent = nullptr);
	~DevLogManage();

	void sync_com_stat(bool stat) {
		m_comStat = stat;
		m_devLogWork->SetComStat(stat);
	}
private:
	void initUi();
	void initRegisterParam();

	void clear_all();

	void initShellCompleter();
signals:
	void send(int dev_typ, const QByteArray&);
	void recv(int dev_typ, QByteArray&);

	void append_data(const QString&);
	void start();
public Q_SLOTS:
	void send_data(const QByteArray& data);
	void recv_data(QByteArray& data);

private Q_SLOTS:
	void custom_context_menu_requested(const QPoint& point);

private:
	QTextEditEx* m_devLog = nullptr;
	QLineEdit* m_sendEdit = nullptr;
	QPushButton* m_sendButton = nullptr;
	QMenu* m_contextMenu = nullptr;
	QSharedPointer<QFile> m_logSaveFile = nullptr;
	QSharedPointer<QTextStream> m_logTextStream = nullptr;

	bool m_runState = false;
	QAtomicInteger<bool> m_comStat = false;
	QThread* m_recvThread = nullptr;
	DevLogWork* m_devLogWork = nullptr;
};
