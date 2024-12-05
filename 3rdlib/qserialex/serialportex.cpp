#include "serialportex.h"
#include <iostream>
#include <QThread>



SerialPortEx::SerialPortEx(WorkMode mode, QObject* parent)
	: QObject(parent), com_(nullptr), mode_(mode)
{
	com_ = new QSerialPort();
	com_param = new comconfigpara();

	// async recv data
	connect(com_, &QSerialPort::readyRead, this, [=]() {
		if (MODE_ASYNC == mode_) {
			QByteArray data = com_->readAll();
			PrintByteArray("async data: ", data, false);

			std::lock_guard<QMutex> lock_(mutex_);
			async_recv_list_.push_back(data);
		}		
		}/*,Qt::QueuedConnection*/);

	//if (MODE_ASYNC == mode_) {
	//	m_timeReadData.setInterval(100);
	//	connect(&m_timeReadData, &QTimer::timeout, this, [=] {
	//		if (com_->waitForReadyRead(10))
	//		{
	//			QByteArray data = com_->readAll();

	//			std::lock_guard<QMutex> lock_(mutex_);
	//			async_recv_list_.push_back(data);
	//		}
	//		});
	//	m_timeReadData.start();
	//}
	//SPDLOG_DEBUG("thread id {}", QThread::currentThreadId());
}

SerialPortEx::~SerialPortEx()
{
	m_timeReadData.stop();

	if (com_)
	{
		Close();
		com_->deleteLater();
	}
	if (com_param)
	{
		delete com_param;
		com_param = nullptr;
	}
}

bool SerialPortEx::Open(QString portName, qint32 rate, qint8 databit, qint8 stopbit, qint8 parity, qint8 flowctrl)
{
	//SPDLOG_DEBUG("thread id {}", QThread::currentThreadId());

	Close();

	com_param->com_port = portName.split("COM", Qt::SkipEmptyParts).at(0).toInt();

	com_->setPortName(portName);
	com_->setBaudRate(com_param->baute = rate);
	com_->setDataBits(QSerialPort::DataBits(com_param->data_bit = databit));
	com_->setStopBits(QSerialPort::StopBits(com_param->stop_bit = stopbit));
	com_->setParity(QSerialPort::Parity(com_param->parity = parity));
	com_->setFlowControl(QSerialPort::FlowControl(com_param->flow_ctrl = flowctrl));
	com_->setDataTerminalReady(true);

	return com_->open(QIODevice::ReadWrite);
}

bool SerialPortEx::Open(const comconfigpara& compara)
{
	Close();

	com_->setPortName(QString("COM%1").arg(compara.com_port));
	com_->setBaudRate(compara.baute);
	com_->setDataBits(QSerialPort::DataBits(compara.data_bit));
	com_->setStopBits(QSerialPort::StopBits(compara.stop_bit));
	com_->setParity(QSerialPort::Parity(compara.parity));
	com_->setFlowControl(QSerialPort::FlowControl(compara.flow_ctrl));

	bool bret = com_->open(QIODevice::ReadWrite);
	com_->setDataTerminalReady(true);
	*com_param = compara;

	return bret;
}

void SerialPortEx::Close()
{
	if (com_->isOpen())
	{
		com_->close();
	}
}

bool SerialPortEx::ReOpen(const comconfigpara& para)
{
	if (!Open(para))
	{
		printf("[ERR]SerialPortEx ReOpen failed!\n");
		return false;
	}

	return true;
}

int SerialPortEx::sendData(const QByteArray& indata)
{
	//SPDLOG_DEBUG("thread id {}", QThread::currentThreadId());
	if (!com_->isOpen()) {
		return -1;
	}

	//com_->clear();
	int ret = com_->write(indata);
	//com_->flush();

	if (ret != indata.size() || !com_->waitForBytesWritten())
	{
		std::cout << __FUNCTION__ << " write failed\n";
		return -1;
	}
	//com_->flush();

	std::lock_guard<QMutex> lock_(mutex_);
	async_recv_list_.clear();

	return ret;
}

int SerialPortEx::recvData(QByteArray& outdata)
{
	//SPDLOG_DEBUG("{} thread id {}", __FUNCTION__, QThread::currentThreadId());
	if (mode_ == MODE_ASYNC) {
		return AsyncRecvData(outdata);
	}
	else
	{
		return SyncRecvData(outdata);
	}	
}

QList<QSerialPortInfo> SerialPortEx::FindAllComs()
{
	return QSerialPortInfo::availablePorts();
}

bool SerialPortEx::GetComParam(comconfigpara& para)
{
	para = *com_param;
	return IsOpen();
}

int SerialPortEx::SyncRecvData(QByteArray& data)
{
	if (!com_->isOpen()) {
		return -1;
	}

	if (com_->waitForReadyRead(20))
	{
		data = com_->readAll();
		while (com_->waitForReadyRead(20))
		{
			data += com_->readAll();
		}
	}

	return data.size();
}

int SerialPortEx::AsyncRecvData(QByteArray& data)
{
	std::lock_guard<QMutex> lock_(mutex_);	
	while (!async_recv_list_.isEmpty()) {
		data.append(async_recv_list_.takeFirst());
	}

	return data.size();
}
