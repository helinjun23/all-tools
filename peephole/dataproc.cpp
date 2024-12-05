#include "dataproc.h"
#include "PeepholeSerialK1SDataTyp.h"
#include "PeepholeSerial3vDataTyp.h"
#include "clogcat.h"
#include "SerialProtocolOrb.h"

CDataProc::CDataProc()
{
	m_bPicSave = true;
	m_serialTimeOut = 100;//3000;
	m_trySerialOpenNum = 200;

	m_pThreadProc = NULL;
	m_bRun = false;

	m_pSerialProtocol = nullptr;
	m_pOrbProtocol = nullptr;

	//test
	m_parseSta = SerialCmdParseStaEnd;
}

CDataProc::~CDataProc()
{

}

int CDataProc::Start(CSerialProtocolBase* pProtocol, uint8_t protocolTyp)
{
	m_pSerialProtocol = pProtocol;
	switch (protocolTyp)
	{
	case PeepholeProtocolTyp3V:
	case PeepholeProtocolTypK1S: //senseTime
		m_pOrbProtocol = new CSerialProtocolOrb();
		break;
	case PeepholeProtocolTypOrb:
		m_pOrbProtocol = pProtocol;
		break;
	default:
		break;
	}

	m_pThreadProc = new CThreadProc();
	if (m_pThreadProc != NULL)
	{
		m_bRun = true;
		m_pThreadProc->Start(this, CDataProc::SerialReadThreadProc, 1);
	}
	return 0;
}

int CDataProc::Stop()
{
	m_bRun = false;

	if (m_pThreadProc != NULL)
	{
		m_pThreadProc->Stop();
		m_bRun = false;
		delete m_pThreadProc;
	}
	m_pThreadProc = NULL;

	m_pSerialProtocol = nullptr;
	return 0;
}


int CDataProc::SetParam(QString portName, int iBaudRate, int iDatabits, int stopBits)
{
	m_devPortName = portName;
	m_devBaudRate = iBaudRate;
	m_devDatabits = iDatabits;
	m_devStopBit = stopBits;

	return 1;
}

void CDataProc::SetDevComParam(QSerialPort& serial, QString portName, int iBaudRate, int iDatabits, int stopBits)
{
	qint32 baudRate;
	switch (iBaudRate)
	{
	case 1200:
		baudRate = QSerialPort::Baud1200;
		break;
	case 2400:
		baudRate = QSerialPort::Baud2400;
		break;
	case 4800:
		baudRate = QSerialPort::Baud4800;
		break;
	case 9600:
		baudRate = QSerialPort::Baud9600;
		break;
	case 19200:
		baudRate = QSerialPort::Baud19200;
		break;
	case 38400:
		baudRate = QSerialPort::Baud38400;
		break;
	case 57600:
		baudRate = QSerialPort::Baud57600;
		break;
	case 115200:
		baudRate = QSerialPort::Baud115200;
		break;
	case 256000:
		baudRate = QSerialPort::Baud256000;
		break;
	case 460800:
		baudRate = QSerialPort::Baud460800;
		break;
	case 921600:
		baudRate = QSerialPort::Baud921600;
		break;
	case 1382400:
		baudRate = QSerialPort::Baud1382400;
		break;
	case 1500000:
		baudRate = QSerialPort::Baud1500000;
		break;
	default:
		break;
	}

	serial.setPortName(portName);
	serial.setBaudRate(baudRate);
	serial.setDataBits(QSerialPort::Data8);
	serial.setStopBits(QSerialPort::OneStop);
	serial.setParity(QSerialPort::NoParity);
	serial.setFlowControl(QSerialPort::NoFlowControl);
	//serial.setRequestToSend(true);
}

bool CDataProc::OpenCom(QSerialPort& serial, QSerialPort::OpenMode mode)
{
	int i = 0;
	for (i = 0; i < m_trySerialOpenNum; i++)
	{
		if (!serial.open(mode))//用ReadWrite 的模式尝试打开串口
		{
			emit CLogcat::Instance()->ShowNoteInfo("the " + QString::number(i + 1) + "th " + serial.portName() + " 打开失败!", 0, true);
			QThread::msleep(20);
		}
		else
		{
			break;
		}
	}
	//ShowNoteInfo("---try open serial num="+QString::number(m_trySerialOpenNum)+",i="+QString::number(i));
	if (i >= m_trySerialOpenNum)
	{
		return false;
	}

	//ShowNoteInfo(serial.portName() + "打开成功!");

	return true;
}

bool CDataProc::CloseCom(QSerialPort& serial)
{
	serial.close();
	return true;
}

void CDataProc::PushCmd(const uint8_t* cmd, const uint32_t cmdlen, int cmdTyp)
{
	if (NULL == cmd || cmdlen == 0)
	{
		return;
	}

	SerialCmd cmdData;
	memset(&cmdData, 0, sizeof(cmdData));
	//emit CLogcat::Instance()->ShowNoteInfo("push cmd len="+QString::number(cmdlen),1,true);
	memcpy(cmdData.cmd, cmd, sizeof(unsigned char) * cmdlen);
	cmdData.cmdlen = cmdlen;
	cmdData.cmdTyp = cmdTyp;
	m_mutex.lock();
	m_cmdlist.push_back(cmdData);
	m_mutex.unlock();
}

int  CDataProc::CmdSend(void* param, const uint8_t* cmd, const uint32_t cmdlen)
{
	if (nullptr == cmd)
		return -1;
	CDataProc* pThis = (CDataProc*)param;
	if (pThis != nullptr)
	{
		pThis->PushCmd(cmd, cmdlen, 0);
	}
	return 0;
}

int CDataProc::PopCmd(SerialCmd& cmd)
{
	int res = -1;
	m_mutex.lock();
	if (m_cmdlist.size() > 0)
	{
		res = 1;
		cmd = m_cmdlist.front();
		//emit CLogcat::Instance()->ShowNoteInfo("pop cmd len="+QString::number(cmd.cmdlen),1,true);
		m_cmdlist.pop_front();
	}
	m_mutex.unlock();

	return res;
}

void CDataProc::PrintHex(unsigned char* data, int datalen)
{
	QString str, strTmp;
	if (datalen > 0)
	{
		str = "";
		strTmp = "";
		for (int i = 0; i < datalen; i++)
		{
			strTmp.sprintf("%02X ", data[i]);
			str += strTmp;
		}
		emit CLogcat::Instance()->ShowNoteInfo("data:" + str, 0, true);
	}
}




void CDataProc::SendCmd(QSerialPort& serial, unsigned char* cmd, uint32_t cmdlen)
{
	if (NULL == cmd || 0 == cmdlen)
	{
		return;
	}
	//
	QString str, strTmp;

	//emit CLogcat::Instance()->ShowNoteInfo("send cmd len="+QString::number(cmdlen),1,true);
	serial.clear();
	qint64 len = serial.write((char*)cmd, cmdlen);
	//emit CLogcat::Instance()->ShowNoteInfo("after send cmd len="+QString::number(len),1,true);
	if (cmdlen > 0)
	{
		str = "";
		strTmp = "";
		for (int i = 0; i < cmdlen; i++)
		{
			strTmp.sprintf("%02X ", cmd[i]);
			str += strTmp;
		}
		emit CLogcat::Instance()->ShowNoteInfo("send cmd:" + str, 0, true);
	}
}

int CDataProc::SerialRead(QSerialPort& serial, QByteArray& respData)
{
	int res = -1;
	//respData = devSerial.readAll();
	QString noteStr;
	if (serial.waitForReadyRead(m_serialTimeOut))
	{
		// read request
		respData = serial.readAll();
		while (serial.waitForReadyRead(20))
		{
			respData += serial.readAll();
		}
		res = 1;
	}

	return res;
}

void CDataProc::ParseCmd(QByteArray& cmdData)
{
	uint32_t datalen = cmdData.length();
	uint8_t* data = (uint8_t*)cmdData.data();
	//emit CLogcat::Instance()->ShowNoteInfo("111",0,true);
	if (SerialCmdParseStaEnd == m_parseSta)
	{
		m_curPclTyp = PeepholeProtocolTypEnd;
		if (m_pSerialProtocol != m_pOrbProtocol)
		{
			if (m_pSerialProtocol->CheckTag(data, datalen))
			{
				//emit CLogcat::Instance()->ShowNoteInfo("other tag!!",1,true);
				m_parseSta = SerialCmdParseStaClientBegin;
				m_curPclTyp = PeepholeProtocolTypOth;
			}
		}

		if (m_pOrbProtocol->CheckTag(data, datalen))
		{
			//emit CLogcat::Instance()->ShowNoteInfo("orbbec tag!!",1,true);
			m_parseSta = SerialCmdParseStaOrbBegin;
			m_curPclTyp = PeepholeProtocolTypOrb;
		}

	}

	bool bClearBuf = false;
	if (m_curPclTyp != PeepholeProtocolTypEnd)
	{
		if (m_pSerialProtocol != m_pOrbProtocol)
		{
			if (m_pSerialProtocol->CheckTag(data, datalen))
			{
				//buf data is error,recv buf is new cmd
				printf("**************** clearBuf other recv buf************\n");
				bClearBuf = true;
				m_parseSta = SerialCmdParseStaClientBegin;
			}
		}

		if (m_pOrbProtocol->CheckTag(data, datalen))
		{
			//buf data is error,recv buf is new cmd
			printf("**************** clearBuf orbbec recv buf************\n");
			bClearBuf = true;
			m_parseSta = SerialCmdParseStaOrbBegin;
		}
	}
	if ((SerialCmdParseStaClientBegin == m_parseSta) || (SerialCmdParseStaClientRun == m_parseSta))
	{
		m_parseSta = m_pSerialProtocol->ParseCmd(data, datalen, bClearBuf);
	}

	if ((SerialCmdParseStaOrbBegin == m_parseSta) || (SerialCmdParseStaOrbRun == m_parseSta))
	{
		//emit CLogcat::Instance()->ShowNoteInfo("3",0,true);
		m_parseSta = m_pOrbProtocol->ParseCmd(data, datalen, bClearBuf);
	}

}

void CDataProc::SerialReadThreadProc(void* param)
{
	CDataProc* pThis = (CDataProc*)param;
	if (pThis != NULL)
	{
		pThis->SerialReadProc();
	}
}

void CDataProc::SerialReadProc()
{
	QSerialPort devSerial; //device

	SetDevComParam(devSerial, m_devPortName, m_devBaudRate, m_devDatabits, m_devStopBit);
	//connect(&devSerial, &QSerialPort::readyRead, this, &CWqSerialPower::handleReadyRead);
	bool res = OpenCom(devSerial, QIODevice::ReadWrite);
	if (!res)
	{
		emit CLogcat::Instance()->ShowNoteInfo("open serial failed!", 0, true);
		return;
	}

	while (m_bRun)
	{
		int res = 0;

		SerialCmd cmdData;
		memset(&cmdData, 0, sizeof(cmdData));

		res = PopCmd(cmdData);
		if (res > 0)
		{
			//emit CLogcat::Instance()->ShowNoteInfo("after pop len="+QString::number(cmdData.cmdlen),2,true);
			SendCmd(devSerial, cmdData.cmd, cmdData.cmdlen);
		}

		QByteArray respData;
		respData.clear();

		res = SerialRead(devSerial, respData);
		QString str, strTmp;

		if (respData.length() > 0)
		{
			str = "";
			strTmp = "";

			for (int i = 0; i < respData.length(); i++)
			{
				unsigned char ch = (unsigned char)respData.at(i);
				strTmp.sprintf("%02X ", ch);
				str += strTmp;
			}
			emit CLogcat::Instance()->ShowNoteInfo("resp:" + str, 0, true);

			ParseCmd(respData);
		}

		QThread::msleep(10);
	}

	CloseCom(devSerial);
}


