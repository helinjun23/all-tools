#pragma once

#include "utility/utility.hpp"

#include <QObject>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QThread>

struct comconfigpara
{
	uint8_t com_port;
	uint32_t baute;
	uint8_t parity;
	uint8_t data_bit;
	float stop_bit;
	uint8_t flow_ctrl;
};

class SerialPortEx : public QObject
{
	Q_OBJECT
		
public:
	~SerialPortEx();
	enum WorkMode { MODE_SYNC, MODE_ASYNC };
	explicit SerialPortEx(WorkMode mode, QObject* parent=nullptr);

	bool Open(QString portName, qint32 rate, qint8 databit = QSerialPort::Data8,
		qint8 stopbit = QSerialPort::OneStop, qint8 parity = QSerialPort::NoParity, qint8 flowctrl = QSerialPort::NoFlowControl);
	bool Open(const comconfigpara& compara);
	void Close();
	bool ReOpen(const comconfigpara& para);

	bool IsOpen() {
		if (com_) {
			return com_->isOpen();
		}
		return false;
	}
	static QList<QSerialPortInfo> FindAllComs();

	bool GetComParam(comconfigpara& para);

	int SyncRecvData(QByteArray& data);
	int AsyncRecvData(QByteArray& data);

private:

Q_SIGNALS:
	//void send(const QByteArray&);
	//void recv(QByteArray&);

public Q_SLOTS:
	int sendData(const QByteArray& indata);
	int recvData(QByteArray& outdata);

private:
	QSerialPort* com_;

	comconfigpara* com_param;
	QMutex mutex_;
	QByteArrayList async_recv_list_;
	WorkMode mode_;
	QTimer m_timeReadData;
};
