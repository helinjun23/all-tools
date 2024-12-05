#include "OrbProtocol.h"
#include "utility/crc.hpp"

using namespace orbprotocol;

OrbProtocol::OrbProtocol(QObject* parent)
	:OrbbecFaceModuleProtocolInterface(parent), encrypt(false)
{
	SPDLOG_DEBUG("thread id {}", QThread::currentThreadId());

}

OrbProtocol::~OrbProtocol()
{

}

int OrbProtocol::FillHostPkt(OrbProtocolPktTyp pktTyp, const std::vector<uint8_t>& indata, QByteArray& pktData)
{
	QByteArray tmpdata(indata.size(), 0);
	memcpy(tmpdata.data(), indata.data(), indata.size());
	return FillHostPkt(pktTyp, tmpdata, pktData);
}

int OrbProtocol::FillHostPkt(OrbProtocolPktTyp pktTyp, const QByteArray& indata, QByteArray& pktData)
{
	if (indata.isEmpty())
		return -1;

	AddHead(OrbProtocolSourceHost, pktTyp, pktData);
	AddData(indata, pktData);
	AddCheckSum(pktData);

	return 0;
}

int OrbProtocol::FillSlavePkt(OrbProtocolPktTyp pktTyp, const QByteArray& indata, QByteArray& pktData)
{
	if (indata.isEmpty())
		return -1;

	AddHead(OrbProtocolSourceSlave, pktTyp, pktData);
	AddData(indata, pktData);
	AddCheckSum(pktData);

	return 0;
}
// ParseOrbCmd
int OrbProtocol::ParseOrbCmd(uint8_t cmdtype, const QByteArray& indata, OrbProtocolCmd& orbCmd)
{
	int cmdlen = indata.size();
	if (cmdlen <= 0)
		return -1;

	memset(&orbCmd, 0, sizeof(orbCmd));

	uint8_t* pHead = (uint8_t*)indata.data();
	if ((cmdlen >= 6) && (ORB_PROTOCOL_TAG_LOW == (*pHead)) && (ORB_PROTOCOL_TAG_HIGH == *(pHead + 1))) //get cmd length
	{
		uint8_t high = *(pHead + 5);
		uint8_t low = *(pHead + 4);
		orbCmd.len = (((high << 8) & 0xff00) | (low & 0x00ff));
	}

	if (cmdlen < (orbCmd.len + 6))
	{
		return -1;
	}
	if (cmdlen < orbCmd.len + 8)
	{
		return -1;
	}

	//parse data
	orbCmd.hLow = *(pHead);
	orbCmd.hHigh = *(pHead + 1);
	orbCmd.source = *(pHead + 2);
	orbCmd.typ = *(pHead + 3);

	// data 
	if (encrypt && orbCmd.len >= 16) //decrypt 加密数据至少16个字节长度
	{
		auto tmpdata = orbCrypt.aes128_decrypt(indata.mid(6, orbCmd.len));
		//data len
		int encdatalen = (tmpdata[1] << 8 & 0xff00) | tmpdata[0] & 0x00ff;
		if (encdatalen != tmpdata.size())
		{
			return -1;
		}
		//check crc
		uint16_t calcrc = CheckSum((uint8_t*)tmpdata.data(), encdatalen - 2);
		uint16_t datacrc = ((tmpdata[encdatalen - 1] << 8) & 0xff00) | tmpdata[encdatalen - 2] & 0x00ff;
		if (calcrc != datacrc)
		{
			return -1;
		}

		orbCmd.len = encdatalen - 4;
		memcpy(orbCmd.data, (uint8_t*)tmpdata.mid(2, orbCmd.len).data(), orbCmd.len);
	}
	else
	{
		memcpy(orbCmd.data, pHead + 6, orbCmd.len); // 7
		uint32_t bCrcLen = 6 + orbCmd.len + 2;
		orbCmd.crc = (*(pHead + bCrcLen - 1) << 8 & 0xff00) | (*(pHead + bCrcLen - 2) & 0x00ff);
	}

	std::vector<uint8_t> outdata;
	OrbProtocolCmdConvertStr(orbCmd, outdata);

	if (cmdtype != UNKNOWN_TYPE && orbCmd.typ == OrbProtocolPktTypErr)
	{
		SPDLOG_WARN("parse cmd err = ", GetErrPktInfo((OrbProtocolPktErrTyp)orbCmd.data[0]));
		return -1;
	}

	if (cmdtype != UNKNOWN_TYPE && orbCmd.data[0] != cmdtype)
	{
		SPDLOG_WARN("recv cmd type {} <> {} invalid!", orbCmd.data[0], cmdtype);
		return -1;
	}

	return outdata.size();
}
// 将OrbProtocolCmd数据转为字符串形式
QString OrbProtocol::OrbProtocolCmdConvertStr(OrbProtocolCmd& indata, std::vector<uint8_t>& outdata)
{
	outdata.clear();
	std::vector<uint8_t>& deCryptData = outdata;
	deCryptData.push_back(indata.hLow);
	deCryptData.push_back(indata.hHigh);
	deCryptData.push_back(indata.source);
	deCryptData.push_back(indata.typ);
	deCryptData.push_back(indata.len & 0x00ff);
	deCryptData.push_back(indata.len >> 8 & 0x00ff);

	for (int i = 0; i < indata.len; i++)
	{
		deCryptData.push_back(indata.data[i]);
	}

	uint16_t crc16 = CheckSum(deCryptData.data(), deCryptData.size());
	deCryptData.push_back(crc16 & 0x00ff);
	deCryptData.push_back(crc16 >> 8 & 0x00ff);
	indata.crc = crc16;

	QString strcmd;
	for (int i = 0; i < deCryptData.size(); i++)
	{
		char _hexstr[3]{ 0 };
		sprintf(_hexstr, "%02x ", deCryptData[i]);
		strcmd.append(_hexstr);
	}
	return strcmd;
}
void OrbProtocol::AddHead(OrbProtocolSource srcTyp, OrbProtocolPktTyp pktTyp, QByteArray& pktData)
{
	pktData.push_back(ORB_PROTOCOL_TAG_LOW);
	pktData.push_back(ORB_PROTOCOL_TAG_HIGH);
	pktData.push_back(srcTyp);
	pktData.push_back(pktTyp);
}

void OrbProtocol::AddData(const QByteArray& indata, QByteArray& pktData)
{
	QByteArray encrypt_data = indata;
	int datalen = indata.size();
	if (encrypt)
	{
		int encyyDataLen = datalen + 4; // 数据区长度nByte+数据长度2Byte+校验和2Byte

		QByteArray newdata;
		newdata.push_back(encyyDataLen & 0x00ff);
		newdata.push_back(encyyDataLen >> 8);
		newdata.push_back(indata);

		uint16_t crc = CheckSum((uint8_t*)newdata.data(), newdata.size());
		newdata.push_back(crc & 0x00ff);
		newdata.push_back((crc & 0xff00) >> 8);

		encrypt_data = orbCrypt.aes128_encrypt(newdata);
		datalen = encrypt_data.size();
	}

	pktData.push_back(datalen & 0x00ff);
	pktData.push_back((datalen & 0xff00) >> 8);
	pktData.push_back(encrypt_data);
}

void OrbProtocol::AddCheckSum(QByteArray& pktData)
{
	uint16_t crc = 0;
	crc = CheckSum((uint8_t*)pktData.data(), pktData.size());
	pktData.push_back(crc & 0x00ff);
	pktData.push_back((crc & 0xff00) >> 8);
}

uint16_t OrbProtocol::CheckSum(uint8_t* cmd, uint32_t cmdLen)
{
	uint16_t crc = 0;
	uint16_t sum = 0;
	for (uint32_t i = 0; i < cmdLen; i++)
	{
		sum += cmd[i];
	}
	crc = 0x10000 - sum;
	return crc;
}

std::string OrbProtocol::GetErrPktInfo(OrbProtocolPktErrTyp err)
{
	std::string strErr;
	switch (err)
	{
	case Err_CrcCheck:
		strErr = "crc error";
		break;
	case Err_DevType:
		strErr = "dev type error";
		break;
	case Err_PktFlag:
		strErr = "packet flag error";
		break;
	case Err_DataType:
		strErr = "data type error";
		break;
	case Err_Reverse_1:
	case Err_Reverse_2:
	case Err_Reverse_3:
	case Err_Reverse_4:
		strErr = "reserve";
		break;
	default:
		break;
	}
	return strErr;
}

int orbprotocol::OrbProtocol::SetRecongLevel(uint8_t val)
{
	uint8_t cmdtype = SET_RECONG_LEVEL;
	std::vector<uint8_t> sendatas;
	sendatas.emplace_back(cmdtype);
	sendatas.emplace_back(val);

	QByteArray pktData;
	FillHostPkt(OrbProtocolPktTypCmd, sendatas, pktData);

	OrbProtocolCmd proCmd;
	if (query_and_parse_data(cmdtype, pktData, proCmd) != ProtocolResultTyp::NONE)
	{
		return -1;
	}
	SPDLOG_INFO("set recong level {} {}", val, CmdOprResultInfo(proCmd.data[1]));

	return 0;
}

int orbprotocol::OrbProtocol::GetRecongLevel(uint8_t& val)
{
	uint8_t cmdtype = GET_RECONG_LEVEL;
	QByteArray sendatas;
	sendatas.push_back(cmdtype);

	QByteArray pktData;
	FillHostPkt(OrbProtocolPktTypLookUp, sendatas, pktData);

	OrbProtocolCmd proCmd;
	if (query_and_parse_data(cmdtype, pktData, proCmd) != ProtocolResultTyp::NONE)
	{
		return -1;
	}

	val = proCmd.data[1];
	SPDLOG_INFO("get recong level {}", val);

	return 0;
}

int orbprotocol::OrbProtocol::ResetRecongLevel()
{
	uint8_t cmdtype = RESET_RECONG_LEVEL;
	std::vector<uint8_t> sendatas;
	sendatas.emplace_back(cmdtype);
	sendatas.emplace_back(0x01);

	QByteArray pktData;
	FillHostPkt(OrbProtocolPktTypCmd, sendatas, pktData);

	OrbProtocolCmd proCmd;
	if (query_and_parse_data(cmdtype, pktData, proCmd) != ProtocolResultTyp::NONE)
	{
		return -1;
	}

	SPDLOG_INFO("reset recong level {}", CmdOprResultInfo(proCmd.data[1]));

	return 0;
}

int orbprotocol::OrbProtocol::SetLiveLevel(uint8_t val)
{
	uint8_t cmdtype = SET_LIVING_LEVEL;
	std::vector<uint8_t> sendatas;
	sendatas.emplace_back(cmdtype);
	//sendatas.emplace_back(0x00);
	sendatas.emplace_back(val);

	QByteArray pktData;
	FillHostPkt(OrbProtocolPktTypCmd, sendatas, pktData);

	OrbProtocolCmd proCmd;
	if (query_and_parse_data(cmdtype, pktData, proCmd) != ProtocolResultTyp::NONE)
	{
		return -1;
	}

	SPDLOG_INFO("set live level {}", CmdOprResultInfo(proCmd.data[1]));
	return 0;
}

int orbprotocol::OrbProtocol::GetLiveLevel(uint8_t& val)
{
	const uint8_t cmdtype = GET_LIVING_LEVEL;
	QByteArray sendatas;
	sendatas.push_back(cmdtype);
	char reverse = 0x00;
	sendatas.push_back(reverse);

	QByteArray pktData;
	FillHostPkt(OrbProtocolPktTypLookUp, sendatas, pktData);

	OrbProtocolCmd proCmd;
	if (query_and_parse_data(cmdtype, pktData, proCmd) != ProtocolResultTyp::NONE)
	{
		return -1;
	}
	
	SPDLOG_INFO("get live level {}", val = proCmd.data[1]);

	return 0;
}

int orbprotocol::OrbProtocol::ResetLiveLevel()
{
	const uint8_t cmdtype = RESET_LIVING_LEVEL;
	std::vector<uint8_t> sendatas;
	sendatas.push_back(cmdtype);
	sendatas.push_back(0x00);

	QByteArray pktData;
	FillHostPkt(OrbProtocolPktTypCmd, sendatas, pktData);

	OrbProtocolCmd proCmd;
	if (query_and_parse_data(cmdtype, pktData, proCmd) != ProtocolResultTyp::NONE)
	{
		return -1;
	}

	SPDLOG_INFO("reset live level {}", CmdOprResultInfo(proCmd.data[1]));

	return 0;
}

int orbprotocol::OrbProtocol::DeleteFaceId(uint8_t id)
{
	const uint8_t cmdtype = DELETE_FACE_ID;
	QByteArray sendatas;
	sendatas.push_back(cmdtype);
	sendatas.push_back(id);

	QByteArray pktData;
	FillHostPkt(OrbProtocolPktTypCmd, sendatas, pktData);

	OrbProtocolCmd proCmd;
	if (query_and_parse_data(cmdtype, pktData, proCmd) != ProtocolResultTyp::NONE)
	{
		return -1;
	}

	SPDLOG_INFO("delete face id {} {}!", id, CmdOprResultInfo(proCmd.data[1]));

	return 0;
}


int orbprotocol::OrbProtocol::DeleteAllFaceId()
{
	static const uint8_t delete_allfaceid = 0xff;
	return DeleteFaceId(delete_allfaceid);
}

int orbprotocol::OrbProtocol::SetMultiFaceRegisterEnable(bool en)
{
	const uint8_t cmdtype = SET_REPEAT_REGISTER;
	QByteArray sendatas;
	sendatas.push_back(cmdtype);
	sendatas.push_back(en ? 1 : 0);

	QByteArray pktData;
	FillHostPkt(OrbProtocolPktTypCmd, sendatas, pktData);

	OrbProtocolCmd proCmd;
	if (query_and_parse_data(cmdtype, pktData, proCmd) != ProtocolResultTyp::NONE)
	{
		return -1;
	}

	SPDLOG_INFO("set multi face register sw {} {}", en, CmdOprResultInfo(proCmd.data[1]));

	return 0;
}

int orbprotocol::OrbProtocol::GetMultiFaceRegisterEnable(bool& en)
{
	const uint8_t cmdtype = GET_REPEAT_REGISTER;
	QByteArray sendatas;
	sendatas.push_back(cmdtype);

	QByteArray pktData;
	FillHostPkt(OrbProtocolPktTypLookUp, sendatas, pktData);

	OrbProtocolCmd proCmd;
	if (query_and_parse_data(cmdtype, pktData, proCmd) != ProtocolResultTyp::NONE)
	{
		return -1;
	}

	en = proCmd.data[1];
	SPDLOG_INFO("get multi face register sw {}", en);

	return 0;
}

int OrbProtocol::ParseDualRecogData(const OrbProtocolCmd& orbCmd, face_recongnize_res& recogRes)
{
	uint8_t* pData = (uint8_t*)orbCmd.data + 4;
	memcpy(&recogRes, pData, sizeof(recogRes));
	return 1;
}

int orbprotocol::OrbProtocol::getOTAUpdateStatus()
{
	QByteArray rcvbuf;
	int timeoutcnt = 0;
	while (timeoutcnt < 20)
	{
		if (0 != recvData(rcvbuf))
		{
			timeoutcnt++;
		}
		else
		{
			timeoutcnt = 0;
		}

		if (rcvbuf.isEmpty())
		{
			continue;
		}
		// 数据解析
		OrbProtocolCmd resdata;
		if (0 > ParseOrbCmd(UNKNOWN_TYPE, rcvbuf, resdata))
		{
			continue;
		}

		static const QStringList errInfo{ "升级中","升级完成","CRC校验失败","升级失败" };
		if (resdata.data[0] == OTA_REPORT_STAT)
		{
			auto flag = resdata.data[1];
			SPDLOG_INFO("ota upgrade stat {}", errInfo.at(flag).toStdString());
			if (flag == 0) // 升级中
			{
				continue;
			}

			if (flag == 1) //升级完成
			{
				return 0;
			}
			else // crc校验失败、升级失败
			{
				break;
			}
		}
	}
	return -1;
}

int OrbProtocol::ParseCaptrueData(const QList<QByteArray>& picData, QByteArray& imgData)
{
	if (picData.isEmpty())
	{
		return -1;
	}

	// 首帧数据包含0x77图片文件信息，以及第一帧图片数据
	QByteArray firstframe = picData.front();

	// 获取图片文件信息		
	OrbProtocolCmd orbcmd;
	ParseOrbCmd(UNKNOWN_TYPE, firstframe.mid(0, 14), orbcmd);
	// CMD[1Byte]+pic_size[3Byte:0~24bit]+pic_crc[2Byte:lsb+msb]
	uint32_t pic_size = 0;
	uint16_t pic_crc = 0;
	if (orbcmd.data[0] == UP_SNAP_PICTURE_INFO)
	{
		pic_size = (orbcmd.data[1]) & 0x0000ff
			| (orbcmd.data[2] << 8) & 0x00ff00
			| (orbcmd.data[3] << 16) & 0xff0000;

		pic_crc = (orbcmd.data[4]) & 0x00ff | (orbcmd.data[5] << 8) & 0xff00;
	}
	else
	{
		//emit ShowNoteInfo("第一帧不是0x77图片文件信息", BkColorTypRed);
		SPDLOG_WARN("first frame not 0x77!");
		return -1;
	}

	// 第一帧图片数据
	memset(orbcmd.data, 0, ORBCMD_MAXSIZE);
	//m_orbPkt.ParseOrbCmd(firstframe.mid(14, firstframe.size() - 14), orbcmd);
	//outData.push_back(QByteArray((char*)orbcmd.data, orbcmd.len));

	int rcvImglen = 0;
	// 获取剩余图片数据
	for (int i = 1; i < picData.size(); i++)
	{
		memset(orbcmd.data, 0, ORBCMD_MAXSIZE);
		if (0 > ParseOrbCmd(UNKNOWN_TYPE, picData.at(i), orbcmd)) {
			SPDLOG_WARN("error cmd data!");
		}
		imgData.push_back(QByteArray((char*)orbcmd.data, orbcmd.len));
		rcvImglen += orbcmd.len;
	}

	CrcCal<crc_ccitt_type> crc_ccitt;
	auto fileCrc = crc_ccitt.getCrc((unsigned char*)imgData.data(), imgData.size());
	if (fileCrc != pic_crc)
	{
		SPDLOG_WARN("img crc [0x{:x}]0x{:x} failed", pic_crc, fileCrc);
		return -1;
	}

	return 0;
}

char bandrate(int val)
{
	char c = 0x00;
	switch (val)
	{
	case 115200:
		c = 0x00;
		break;
	case 230400:
		c = 0x01;
		break;
	case 460800:
		c = 0x02;
		break;
	case 1500000:
		c = 0x03;
		break;
	default:
		break;
	}
	return c;
}

bool OrbProtocol::SetBaudRateNoReturn(unsigned char num, int baudRate)
{
	uint8_t cmdtype = SET_SERIAL_BAUTERATE;
	std::vector<uint8_t> sendatas;
	sendatas.emplace_back(cmdtype);
	sendatas.emplace_back(num);

	QByteArray pktData;
	FillHostPkt(OrbProtocolPktTypCmd, sendatas, pktData);

	int res = -1;
	QByteArray resp;
	queryData(pktData, resp);
	//sendData(pktData);
	return true;
}


bool OrbProtocol::sendData(const QByteArray& data)
{
#if 1
	emit SendData(data);
#else
	if (!comDev_.isOpen())
	{
		return false;
	}
	else
	{
		auto baudrate = comDev_.baudRate();
		emit ReOpenCom(baudrate); // 解决偶发接收不到数据问题
	}

	int datalen = data.size();
	//! 清楚缓冲区数据，防止数据粘连
	comDev_.clear(QSerialPort::AllDirections);
	comDev_.write((const char*)data.data(), datalen);
	//! 立即发送
	comDev_.flush();
#endif

	if (isLogEn)
	{
		PrintByteArray("send: ", data, true);
	}

	return true;
}

bool OrbProtocol::recvData(QByteArray& data)
{
	QByteArray read_data;
	int timeout_nums = 0;
	int max_timeout_times = 500;
	int framelen = -1;

	//SPDLOG_DEBUG("{} thread id {}", __FUNCTION__, QThread::currentThreadId());

	while ((++timeout_nums < max_timeout_times) && framelen != read_data.size())
	{
		//std::cout << "can read size = " << comDev_.bytesAvailable() << std::endl;
		QByteArray tmpbuf;
		emit RecvData(tmpbuf);
#if 0
		if (comDev_.waitForReadyRead(10))
		{
			tmpbuf = comDev_.readAll();
		}
#endif

		//! check whether recv complete frame
		if (!tmpbuf.isEmpty()) // 滤掉ready上报
		{
			read_data.append(tmpbuf);
		}
	}

	if (read_data.isEmpty())
	{
		SPDLOG_WARN("recv data failed!");
		return false;
	}
	if (isLogEn)
	{
		PrintByteArray("recv: ", read_data, false);
	}

	data = read_data;
	return true;
}

bool OrbProtocol::queryData(const QByteArray& send_data, QByteArray& read_data)
{
	if (!sendData(send_data))
	{
		SPDLOG_WARN("com send data failed!");
		return false;
	}

	QThread::msleep(200);
	QByteArray recv_data;
	if (!recvData(recv_data))
	{
		return false;
	}

	read_data = recv_data;

	return true;
}

ProtocolResultTyp OrbProtocol::query_and_parse_data(uint8_t cmd_typ, const QByteArray& send_data, OrbProtocolCmd& orbCmd)
{
	QByteArray resp;
	if (!queryData(send_data, resp))
	{
		return ProtocolResultTyp::TRANS_FAIL;
	}

	if (0 > ParseOrbCmd(cmd_typ, resp, orbCmd))
	{
		return ProtocolResultTyp::PARSE_FAIL;
	}

	return ProtocolResultTyp::NONE;
}

// 2022.4.24更新错误码
QString OrbProtocol::ReadenseAlgErrCodeProc(unsigned char failRes)
{
	static const QMap<uint8_t, QString> recong_errcode{
		{0x00, QObject::tr("授权失败")},
		{0x01, QObject::tr("授权失败")},
		{0x02, QObject::tr("SDK内部错误")},
		{0x03, QObject::tr("无人脸")},
		{0x04, QObject::tr("人脸过近")},
		{0x05, QObject::tr("人脸过远")},
		{0x06, QObject::tr("人脸角度偏左")},
		{0x07, QObject::tr("人脸角度偏右")},
		{0x08, QObject::tr("人脸角度偏上")},
		{0x09, QObject::tr("人脸角度偏下")},
		{0x0a, QObject::tr("人脸顺时针旋转偏大")},
		{0x0b, QObject::tr("人脸逆时针旋转偏大")},
		{0x0c, QObject::tr("人脸靠近左边缘")},
		{0x0d, QObject::tr("人脸靠近右边缘")},
		{0x0e, QObject::tr("人脸靠近上边缘")},
		{0x0f, QObject::tr("人脸靠近下边缘")},
		{0x11, QObject::tr("非活体")},
		{0x12, QObject::tr("人脸已注册")},
		{0x13, QObject::tr("数据库已满")},
		{0x14, QObject::tr("陌生人")},
		{0x15, QObject::tr("传参异常")},
		{0x16, QObject::tr("人脸已注册")},
		{0x18, QObject::tr("人脸质量太低")},
		{0x19, QObject::tr("人脸质量太低")},
		{0x1a, QObject::tr("人脸遮挡")},
		{0x1d, QObject::tr("张嘴过大")},
		{0x1f, QObject::tr("多人脸")},
		{0x20, QObject::tr("正视摄像头")},
		{0x21, QObject::tr("正视摄像头")},
		{0x23, QObject::tr("人脸光照亮度过亮")},
		{0x24, QObject::tr("人脸光照亮度过暗")},
		{0x25, QObject::tr("注册超时")},
		{0xa0, QObject::tr("注册中间状态")},
		{0xf0, QObject::tr("其他错误")},
		{0xf3, QObject::tr("无人脸,激光保护")},
	};

	if (recong_errcode.find(failRes) == recong_errcode.end())
	{
		return QString(QObject::tr("未知错误") + ", %1").arg(failRes);
	}
	return recong_errcode[failRes];
}

const std::string OrbProtocol::CmdOprResultInfo(uint8_t ret_num)
{
	if (ret_num == 1)
	{
		return "success";
	}
	else
	{
		return "fail";
	}
}

int orbprotocol::OrbProtocol::GetModuleVer(std::string& ver)
{
	const uint8_t cmdtype = GET_OBMODEL_VER;
	QByteArray sendatas;
	sendatas.push_back(cmdtype);

	QByteArray pktData;
	FillHostPkt(OrbProtocolPktTypLookUp, sendatas, pktData);

	OrbProtocolCmd proCmd;
	if (query_and_parse_data(cmdtype, pktData, proCmd) != ProtocolResultTyp::NONE)
	{
		return -1;
	}
	unsigned char major = proCmd.data[1];
	unsigned char minor = proCmd.data[2];
	unsigned char compile = proCmd.data[3];

	QString moduleVer;
	moduleVer.sprintf("v%x.%x.%x", major, minor, compile);
	SPDLOG_INFO("get module ver {}", moduleVer.toStdString());
	ver = moduleVer.toStdString();

	return 0;
}

int orbprotocol::OrbProtocol::GetAlgBoardVer(std::string& ver)
{
	QString algBoardVer;
	const uint8_t cmdtype = GET_ALGBOARD_SWVER;
	QByteArray sendatas;
	sendatas.push_back(cmdtype);

	QByteArray pktData;
	FillHostPkt(OrbProtocolPktTypLookUp, sendatas, pktData);

	OrbProtocolCmd proCmd;
	if (query_and_parse_data(cmdtype, pktData, proCmd) != ProtocolResultTyp::NONE)
	{
		return -1;
	}

	unsigned char major = proCmd.data[1];
	unsigned char minor = proCmd.data[2];
	unsigned char compile = proCmd.data[3];
	if (proCmd.len == 5)
	{
		algBoardVer.sprintf("v%x.%x.%x.%x", major, minor, compile, proCmd.data[4]);
	}
	else
	{
		algBoardVer.sprintf("v%x.%x.%x", major, minor, compile);
	}

	SPDLOG_INFO("get algboard ver {}", algBoardVer.toStdString());
	ver = algBoardVer.toStdString();

	return 0;
}

int orbprotocol::OrbProtocol::SetFaceRecongRequest(uint8_t max_times, uint8_t& id)
{
	const uint8_t cmdtype = FACE_RECONG;
	QByteArray sendatas;
	sendatas.push_back(cmdtype);
	sendatas.push_back(max_times);

	QByteArray pktData;
	FillHostPkt(OrbProtocolPktTypCmd, sendatas, pktData);

	OrbProtocolCmd proCmd;
	if (query_and_parse_data(cmdtype, pktData, proCmd) != ProtocolResultTyp::NONE)
	{
		return -1;
	}

	if (proCmd.data[1] == 1)
	{
		id = proCmd.data[2];
		SPDLOG_INFO("recong face id {} success", id);
	}
	else
	{
		SPDLOG_INFO("recong face fail, err={}", ReadenseAlgErrCodeProc(proCmd.data[2]).toStdString());
	}

	return 0;
}

int orbprotocol::OrbProtocol::SetFaceRegisterRequeset(uint8_t id)
{
	uint8_t cmdtype = REGISTER_FACE_ID;
	QByteArray sendatas;
	sendatas.push_back(cmdtype);
	sendatas.push_back(id);

	QByteArray respData;
	QByteArray pktData;
	FillHostPkt(OrbProtocolPktTypCmd, sendatas, pktData);
	sendData(pktData);

	std::vector<uint8_t> respbuf;
	GetFaceRegisterStat(respbuf);
	respData = QByteArray((char*)respbuf.data(), respbuf.size());
	OrbProtocolCmd proCmd;
	if (0 > ParseOrbCmd(cmdtype, respData, proCmd))
	{
		return -1;
	}

	unsigned char regRes = (unsigned char)proCmd.data[1];
	if (regRes == 0x01)
	{
		SPDLOG_INFO("register face id {} success!", id);
	}
	if (regRes == 0x02)
	{
		unsigned char failRes = (unsigned char)proCmd.data[2];
		QString tp = ReadenseAlgErrCodeProc(failRes);
		SPDLOG_INFO("register face id {} fail, res {}", id, tp.toStdString());
	}

	return 0;
}

int orbprotocol::OrbProtocol::GetFaceRegisterStat(std::vector<uint8_t>& resp)
{
	clock_t sendClockFirst = clock();
	QString str, strTmp;
	unsigned char cmdResp1 = 0x00;
	unsigned char cmdResp2 = 0x00;
	unsigned char cmdResp3 = 0x00;
	OrbProtocolCmd resdata;
	QByteArray respData;

	int res = 0;
	clock_t lastRecvClk = 0, curRecvClk = 0;
	while (1)
	{
		curRecvClk = clock();
		if (curRecvClk - lastRecvClk < 15)
		{
			continue;
		}

		respData.clear();

		recvData(respData);

		lastRecvClk = curRecvClk;
		// 总注册耗时
		clock_t recvClockFirst = clock();
		if ((recvClockFirst - sendClockFirst) >= 20000)
		{
			res = -1;
			SPDLOG_WARN("register total time over max {} ms", 20000);
			break;
		}

		if (respData.isEmpty()) {
			continue;
		}

		ParseOrbCmd(REGISTER_FACE_ID_INTER, respData, resdata);

		cmdResp1 = resdata.data[0];
		cmdResp2 = resdata.data[1];
		cmdResp3 = resdata.data[2];

		if (cmdResp1 == REGISTER_FACE_ID_INTER
			&& cmdResp3 != REGISTER_FACE_ID_INTER_CONTINUE)
		{
			QString tp = ReadenseAlgErrCodeProc(cmdResp3);
			SPDLOG_INFO("register {} frame, info {}", cmdResp2, tp.toStdString());
		}

		res = 0;
		if ((0x01 == cmdResp1 && 0x01 == cmdResp2)
			|| (0x01 == cmdResp1 && 0x02 == cmdResp2))
		{
			OrbProtocolCmdConvertStr(resdata, resp);
			break;
		}
	}

	if (resp.empty())
	{
		res = -1;
	}
	return res;
}

int orbprotocol::OrbProtocol::SetFaceMultiParamRecongRequest(uint8_t max_times)
{
	QByteArray sendatas;
	sendatas.push_back(0xBD);
	sendatas.push_back(0x01);
	sendatas.push_back(max_times);

	QByteArray pktData;
	FillHostPkt(OrbProtocolPktTypLookUp, sendatas, pktData);

	if (!sendData(pktData))
	{
		return -1;
	}
	return 0;
}

int orbprotocol::OrbProtocol::GetFaceMultiParamRecongResult(face_recongnize_res& recongRes)
{
	clock_t start, finish;
	start = clock();

	bool bFinish = false;
	int res;
	while (!bFinish)
	{
		finish = clock();
		if ((finish - start) > 5 * 1000)
		{
			SPDLOG_INFO("multi param recong time out [5s]");
			break;
		}
		QByteArray resp;
		recvData(resp);

		if (resp.isEmpty())
			continue;

		// 数据解析
		OrbProtocolCmd resdata;
		if (0 > ParseOrbCmd(UNKNOWN_TYPE, resp, resdata))
		{
			return -1;
		}

		ParseDualRecogData(resdata, recongRes);
		if (resdata.typ == 0x0f)
		{
			continue;
		}

		if (resdata.typ == 0x01 && resdata.data[2] == 0x01)
		{
			SPDLOG_INFO("Dual param recognize:res={},face_id={},similarity={},desensitized_similarity={},face_num={}",
				recongRes.valid_flg, recongRes.face_id,
				recongRes.similarity, recongRes.desensitized_similarity,
				recongRes.face_num);
			SPDLOG_INFO("multi param recong success");
		}
		else
		{
			SPDLOG_INFO("multi param recong fail,err={}", ReadenseAlgErrCodeProc(resdata.data[3]).toStdString());
		}

		if (resdata.typ == 0x01)
		{
			break;
		}
	}
	return 0;
}

int orbprotocol::OrbProtocol::GetAllFaceId(std::vector<uint8_t>& ids)
{
	const uint8_t cmdtype = GET_ALL_REGISTER_ID;
	QByteArray sendatas;
	sendatas.push_back(cmdtype);

	QByteArray pktData;
	FillHostPkt(OrbProtocolPktTypLookUp, sendatas, pktData);

	OrbProtocolCmd proCmd;
	if (query_and_parse_data(cmdtype, pktData, proCmd) != ProtocolResultTyp::NONE)
	{
		return -1;
	}

	int idsize = (int)proCmd.len - 1;
	SPDLOG_INFO("ID size= {}", idsize);
	if (idsize > 0)
	{
		QString strAllID;
		for (int i = 0; i < idsize; i++)
		{
			strAllID.append(QString::number(proCmd.data[1 + i]));
			if (i + 1 < idsize)
			{
				strAllID.append(",");
			}
			ids.emplace_back(proCmd.data[1 + i]);
		}
		SPDLOG_INFO("[{}]", strAllID.toStdString());
	}
	return 0;
}

int orbprotocol::OrbProtocol::SetFaceRecongDemoMode(uint8_t max_times)
{
	const uint8_t cmdtype = FACE_DEMO_MODE;
	QByteArray sendatas;
	sendatas.push_back(cmdtype);
	sendatas.push_back(max_times);

	QByteArray pktData;
	FillHostPkt(OrbProtocolPktTypCmd, sendatas, pktData);

	OrbProtocolCmd proCmd;
	if (query_and_parse_data(cmdtype, pktData, proCmd) != ProtocolResultTyp::NONE)
	{
		return -1;
	}

	uint8_t res = proCmd.data[1];
	uint8_t value = proCmd.data[2];
	if (res == 0x01)
	{
		SPDLOG_INFO("demo recong face ID = {} success", value);
	}
	if (res == 0x02)
	{
		SPDLOG_INFO("demo recong face fail,err=", ReadenseAlgErrCodeProc(value).toStdString());
	}
	return 0;
}

int orbprotocol::OrbProtocol::SetStartCapture(SnapParam& param)
{
	const uint8_t cmdtype = GET_SNAP_PICTURE;
	QByteArray sendatas;
	sendatas.push_back(cmdtype);
	// U4开始支持抓拍参数
	if (param.ensnap_param)
	{
		sendatas.push_back(param.img_type);
		sendatas.push_back(param.gain_val);
	}
	else
	{
		uint8_t reserve(0x00);
		sendatas.push_back(reserve);
	}

	QByteArray pktData;
	FillHostPkt(OrbProtocolPktTypCmd, sendatas, pktData);


	char ComboxVal = bandrate(param.snap_bauterate);
	SetBaudRateNoReturn(ComboxVal, param.com_bauterate);

	emit ReOpenCom(param.snap_bauterate);

	if (!sendData(pktData))
	{
		return -1;
	}



	return 0;
}

int orbprotocol::OrbProtocol::GetCaptrueData(QList<QByteArray>& imgCmd)
{
	clock_t start, finish;
	clock_t lastRecvClk = 0, curRecvClk = 0;
	start = clock();
	int res1 = 0;
	const clock_t max_timeout = 10000;
	QByteArray respData;

	imgCmd.clear();
	while (curRecvClk - start < max_timeout)
	{
		// 超时判断
		curRecvClk = clock();
		if (curRecvClk - lastRecvClk < 15)
		{
			continue;
		}

		respData.clear();

		QThread::msleep(500);
		//Sleep(30);
		// read request
		recvData(respData);
		lastRecvClk = curRecvClk;
		if (respData.isEmpty())
		{
			continue;
		}
		 
		OrbProtocolCmd cmd;
		int datalen = 0;

		do {
			datalen = ParseOrbCmd(UNKNOWN_TYPE, respData, cmd);
			if (0 > datalen)
			{
				break;
			}

			if (cmd.typ!= OrbProtocolPktTypErr && cmd.typ != OrbProtocolPktTypOTA)
			{
				break;
			}


			if (datalen < respData.length())
			{
				imgCmd.push_back(respData.mid(0, datalen));
				//SPDLOG_INFO("snap picture packet resp subpackage:", respData.mid(0, datalen));
				respData = respData.mid(datalen);
				continue;
			}
			else
			{
				imgCmd.push_back(respData);
				//SPDLOG_INFO("snap picture packet resp:", respData);
				break;
			}
		} while (1);

		if (cmd.data[0] == GET_SNAP_PICTURE
			&& cmd.source == OrbProtocolSourceSlave
			&& cmd.typ == OrbProtocolPktTypCmd)
		{
			res1 = 1;
			//emit dbarProces(100);
			break;
		}
	}

	if (respData.length() <= 0)
	{
		res1 = -1;
	}

	if (res1 < 0)
	{
		return -1;
	}

	return 0;
}

int orbprotocol::OrbProtocol::GetFaceFeature(QByteArray& feature_data)
{
	const uint8_t cmdtype = GET_CAPTURE_FACE_FEATURE;
	QByteArray sendatas;
	sendatas.push_back(cmdtype);
	sendatas.push_back(QByteArray(4, 0xff));

	QByteArray pktData;
	FillHostPkt(OrbProtocolPktTypCmd, sendatas, pktData);


	OrbProtocolCmd procCmd;
	if (query_and_parse_data(cmdtype, pktData, procCmd) != ProtocolResultTyp::NONE)
	{
		return -1;
	}

	//成功
	if (1 == procCmd.data[1])
	{
		feature_data = QByteArray((char*)procCmd.data[0], procCmd.len);
		SPDLOG_INFO("{:Xp}", spdlog::to_hex(std::vector<uint8_t>(feature_data.begin(), feature_data.end())));
	}
	else
	{
		SPDLOG_WARN("get face feature fail");
	}
	return 0;
}

int orbprotocol::OrbProtocol::SetFeatureRegisterFace(uint8_t id, const QByteArray& feature_data)
{
	const uint8_t cmdtype = REGISTER_FACE_BY_FEATURE;
	QByteArray sendatas;
	sendatas.push_back(cmdtype);
	// 人脸序号
	sendatas.push_back(id);
	// 保留1、保留2
	sendatas.push_back(QByteArray(5, 0xff));
	// 特征数组
	sendatas.push_back(QByteArray(feature_data.data(), feature_data.size()));

	QByteArray pktData;
	FillHostPkt(OrbProtocolPktTypCmd, sendatas, pktData);

	OrbProtocolCmd proCmd;
	if (query_and_parse_data(cmdtype, pktData, proCmd) != ProtocolResultTyp::NONE)
	{
		return -1;
	}

	//成功
	if (1 == proCmd.data[1])
	{
		SPDLOG_INFO("register face feature id {} success", id);
	}
	else
	{
		SPDLOG_INFO("register face feature id {} fail, err={}", id, getSendFaceCharError(proCmd.data[2]).toStdString());
	}
	return 0;
}

int orbprotocol::OrbProtocol::SetStartOTARequest()
{
	const uint8_t cmdtype = SET_OTA_START;
	QByteArray sendatas;
	sendatas.push_back(cmdtype);

	QByteArray pktData;
	FillHostPkt(OrbProtocolPktTypCmd, sendatas, pktData);

	OrbProtocolCmd proCmd;
	if (query_and_parse_data(cmdtype, pktData, proCmd) != ProtocolResultTyp::NONE)
	{
		return -1;
	}

	SPDLOG_INFO("start ota requeset {}", CmdOprResultInfo(proCmd.data[1]));
	return 0;
}

int OrbProtocol::SetStopCapture(SnapParam& param)
{
	//抓完图再切回115200
	char ComboxVal = bandrate(param.snap_bauterate);
	SetBaudRateNoReturn(ComboxVal, param.com_bauterate);
	emit ReOpenCom(param.snap_bauterate);
	return 0;
}

int orbprotocol::OrbProtocol::sendOTAFilePacketInfo(uint32_t fileSize, uint32_t pktNums, const QByteArray& fileCrc)
{
	const uint8_t cmdtype = SET_OTA_FILE_INFO;
	QByteArray sendatas;
	sendatas.push_back(cmdtype);
	sendatas.push_back((uint8_t)fileSize);
	sendatas.push_back((uint8_t)(fileSize >> 8));
	sendatas.push_back((uint8_t)(fileSize >> 16));
	sendatas.push_back(fileCrc);

	return 0;
}

int orbprotocol::OrbProtocol::sendOTAFileData(uint32_t pktIdx, const QByteArray& data)
{
	QByteArray sendatas(data);
	QByteArray pktData;
	SetOrbEncryptEnable(false);// 奥比加密协议，需要发送明文数据包
	FillHostPkt(OrbProtocolPktTypOTA, sendatas, pktData);
	SetOrbEncryptEnable(true);

	QByteArray resp1;
	OrbProtocolCmd proCmd;
	if (query_and_parse_data(-1, pktData, proCmd) != ProtocolResultTyp::NONE)
	{
		return -1;
	}

	const static QStringList kErrInfo{ "成功","超时","重传","校验失败" };
	if (proCmd.typ == OrbProtocolPktTypOTA)
	{
		SPDLOG_INFO("send ota file data idx={} {}", pktIdx, kErrInfo[proCmd.data[0]].toStdString());
	}

	return 0;
}