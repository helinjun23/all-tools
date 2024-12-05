#include "lmprotocol.h"
#include "QT-AES/qaesencryption.h"

#include <QThread>
#include <QTimer>
#include <QDebug>
#include <unordered_map>
#include <thread>

#include "utility/utility.hpp"

using namespace lmprotocol;

//#define _LM_TEST // 虚拟模组串口接收数据

//////////////////////////////////////////////////////////////////////////

/**
 * @brief 转换payloadform
 * @param  data
 * @return std::vector<uint8_t>
 */
LMProtocol::type_payload TransformPayLoad(const payloadform& data)
{
	LMProtocol::type_payload buff;

	buff.push_back(LittleEnidan16(data.cmdtype));
	buff.push_back(LittleEnidan32(data.cmdseq));
	buff.push_back(LittleEnidan16(data.cmdlen));

	for (int i = 0; i < data.cmdlen; i++)
	{
		buff.push_back(data.cmdpaylod[i]);
	}

	return buff;
}

LMProtocol::type_payload TransformHead(const packetheadform& data)
{
	LMProtocol::type_payload buff;

	buff.push_back(LittleEnidan16(data.head));
	buff.push_back(LittleEnidan16(data.payloadlen));
	buff.push_back(LittleEnidan32(data.payloadcrc32));
	buff.push_back(LittleEnidan16(data.packettype));

	return buff;
}

LMProtocol::type_payload TransformPacket(const packetform& data)
{
	LMProtocol::type_payload packet_buff;

	packet_buff.push_back(TransformHead(data.head));
	packet_buff.push_back(TransformPayLoad(data.payload));

	return packet_buff;
}


#define PointerCast(val) reinterpret_cast<uint8_t*>(val)

//////////////////////////////////////////////////////////////////////////
LMProtocol::LMProtocol(QSerialPort& serial) :
	tx_seq_(0), rx_seq_(0), comDev_(serial), enAsyncRead(false), rcvTimeout_(2000)
{
	recvDataCache = QSharedPointer<QList<QByteArray>>(new QList<QByteArray>);
	//setAsyncReadData(true);
}

LMProtocol::~LMProtocol()
{
}

void LMProtocol::onTimerSerialAsyncRead(const QByteArray& rcvBuf)
{
	if (!rcvBuf.isEmpty())
	{
		std::lock_guard<std::mutex> lck(readMutex);
		recvDataCache->push_back(rcvBuf);
	}
}

OprErrNum LMProtocol::setWorkMode(WorkMode mode)
{
	uint16_t cmdtype = (uint16_t)PlainTextCmdType::PLA_READALGPARAM;
	uint32_t OBCmdHead = 0xcc66bb77;
	uint16_t OBFunctionCode = 0x0001;

	QByteArray cmd_buf;
	cmd_buf.push_back(LittleEnidan32(OBCmdHead));
	cmd_buf.push_back(LittleEnidan16(OBFunctionCode));
	cmd_buf.push_back((uint8_t)mode);

	payloadform payload;
	packPlayload(cmdtype, ++tx_seq_, cmd_buf, payload);

	LMProtocol::type_payload sendbuf = packData(PacketType::PACKETTYPE_PLAINTEXT, payload);
	if (!sendData(sendbuf))
	{
		qDebug() << "com send data failed!\n";
		return OprErrNum::OPRERR_OTHER;
	}

	QByteArray read_data;
	if (!recvData(read_data))
	{
		return OprErrNum::OPRERR_MODEL_PACKETLOSS;
	}

	packetform result_data;
	if (!parseData(read_data, result_data) && result_data.payload.cmdpaylod[6] == 0x01)
	{
		return OprErrNum::OPRERR_NEGOTIATE;
	}

	return OprErrNum::OPRERR_NONE;
}

OprErrNum LMProtocol::setUnbind()
{
	uint16_t cmdtype = (uint16_t)PlainTextCmdType::PLA_UNBIND;

	QByteArray cmd_buf;
	payloadform payload;
	packPlayload(cmdtype, ++tx_seq_, cmd_buf, payload);

	LMProtocol::type_payload sendbuf = packData(PacketType::PACKETTYPE_PLAINTEXT, payload);
	if (!sendData(sendbuf))
	{
		qDebug() << "com send data failed!\n";
		return OprErrNum::OPRERR_OTHER;
	}

	QByteArray read_data;
	if (!recvData(read_data))
	{
		return OprErrNum::OPRERR_MODEL_PACKETLOSS;
	}

	packetform result_data;
	if (!parseData(read_data, result_data)
		|| (result_data.head.packettype != (uint16_t)PlainTextCmdType::PLA_UNBIND_ACK))
	{
		return OprErrNum::OPRERR_NEGOTIATE;
	}

	return OprErrNum::OPRERR_NONE;
}

OprErrNum LMProtocol::rootKeyNegotiateProc(const QByteArray& root_key, const QByteArray& root_iv)
{
	payloadform payload;
	uint16_t cmdtype = (uint16_t)PlainTextCmdType::PLA_ROOTKEY;
	root_iv_ = root_iv;
	packPlayload(cmdtype, ++tx_seq_, root_key, payload);

	LMProtocol::type_payload sendbuf = packData(PacketType::PACKETTYPE_PLAINTEXT, payload);

	if (!sendData(sendbuf))
	{
		qDebug() << "com send data failed!\n";
		return OprErrNum::OPRERR_OTHER;
	}

	QByteArray read_data;
	if (!recvData(read_data))
	{
		return OprErrNum::OPRERR_MODEL_PACKETLOSS;
	}

	packetform result_data;
	if (!parseData(read_data, result_data))
	{
		return OprErrNum::OPRERR_NEGOTIATE;
	}

	if (result_data.payload.cmdtype == (uint16_t)PlainTextCmdType::PLA_ROOTKEY_RESULT
		&& (NegotiateResult)result_data.payload.cmdpaylod[0] == NegotiateResult::SUCCESS)
	{
		// 写入根密钥
		root_key_ = root_key;
		// TODO校验根密钥

		return OprErrNum::OPRERR_NONE;
	}

	return OprErrNum::OPRERR_NEGOTIATE;
}

OprErrNum LMProtocol::sessionKeyNegotiateProc(const QByteArray& session_key, const QByteArray& session_iv)
{
	payloadform payload;
	uint16_t cmdtype = (uint16_t)RootCiphetextCmdType::ROOT_CIP_SESSIONKEY;
	session_iv_ = session_iv;
	packPlayload(cmdtype, ++tx_seq_, session_key, payload);

	LMProtocol::type_payload sendbuf = packData(PacketType::PACKETTYPE_ROOTENC, payload);

	if (!sendData(sendbuf))
	{
		qDebug() << "com send data failed!\n";
		return OprErrNum::OPRERR_OTHER;
	}

	QByteArray read_data;
	if (!recvData(read_data))
	{
		return OprErrNum::OPRERR_MODEL_PACKETLOSS;
	}

	packetform result_data;
	if (!parseData(read_data, result_data))
	{
		return OprErrNum::OPRERR_NEGOTIATE;
	}

	if (result_data.payload.cmdtype != (uint16_t)RootCiphetextCmdType::ROOT_CIP_SESSIONKEY_ACK)
	{
		if (result_data.payload.cmdtype == (uint16_t)PlainTextCmdType::PLA_ROOT_NOTREADY)
		{
			std::cout << "root key not ready!\n";
			resetRootKey();
		}
		return OprErrNum::OPRERR_NEGOTIATE;
	}

	session_key_ = session_key;

	// 会话信道探测
	return sessionEncryptProbe();
}

OprErrNum LMProtocol::sessionEncryptProbe()
{
	clearSeq();

	constexpr char random_data[]{ 0x11,0x22,0x33,0x44 };

	payloadform payload;
	uint16_t cmdtype = (uint16_t)SessionCiphetextCmdType::SESSION_CIP_PROBEDATA;
	packPlayload(cmdtype, ++tx_seq_, random_data, payload);

	LMProtocol::type_payload sendbuf = packData(PacketType::PACKETTYPE_SESSIONENC, payload);

	if (!sendData(sendbuf))
	{
		qDebug() << "com send data failed!\n";
		return OprErrNum::OPRERR_OTHER;
	}

	QByteArray read_data;
	if (!recvData(read_data))
	{
		return OprErrNum::OPRERR_MODEL_PACKETLOSS;
	}

	packetform result_data;
	if (!parseData(read_data, result_data))
	{
		return OprErrNum::OPRERR_NEGOTIATE;
	}

	if (result_data.payload.cmdtype != (uint16_t)SessionCiphetextCmdType::SESSION_CIP_PROBEECHO)
	{
		return OprErrNum::OPRERR_PROBE;
	}

	return OprErrNum::OPRERR_NONE;
}

OprErrNum LMProtocol::sessionEncryptComProc(const QByteArray& indata, QByteArray& outdata)
{
	auto ret = sessionEncryptComSend(indata);
	if (OprErrNum::OPRERR_NONE != ret)
	{
		return ret;
	}
	return sessionEncryptComRecv(outdata);
}

OprErrNum lmprotocol::LMProtocol::sessionEncryptComSend(const QByteArray& indata)
{
	payloadform payload;
	uint16_t cmdtype = (uint16_t)SessionCiphetextCmdType::SESSION_CIP_ORBCMD;
	packPlayload(cmdtype, ++tx_seq_, indata, payload);
	LMProtocol::type_payload sendbuf = packData(PacketType::PACKETTYPE_SESSIONENC, payload);

	QByteArray read_data;
	// 1.发送奥比模组协议，然后接收ACK
	if (!queryData(sendbuf, read_data))
	{
		return OprErrNum::OPRERR_MODEL_PACKETLOSS;
	}

	packetform result_data;
	if (!parseData(read_data, result_data))
	{
		return OprErrNum::OPRERR_NEGOTIATE;
	}

	if (result_data.payload.cmdtype != (uint16_t)SessionCiphetextCmdType::SESSION_CIP_SEQUENT
		|| result_data.payload.cmdseq < tx_seq_)
	{
		return OprErrNum::OPRERR_SEQ;
	}

	return OprErrNum::OPRERR_NONE;
}

OprErrNum lmprotocol::LMProtocol::sessionEncryptComRecv(QByteArray& outdata)
{
	QByteArray read_data;

	// 2.接收奥比协议应答
	if (!recvData(read_data))
	{
		return OprErrNum::OPRERR_MODEL_PACKETLOSS;
	}

	packetform result_data;
	if (!parseData(read_data, result_data))
	{
		return OprErrNum::OPRERR_NEGOTIATE;
	}

	if (result_data.payload.cmdtype != (uint16_t)SessionCiphetextCmdType::SESSION_CIP_ORBCMD
		|| result_data.payload.cmdseq < tx_seq_)
	{
		return OprErrNum::OPRERR_SEQ;
	}

	// 3.发送应答协议ack
	if (OprErrNum::OPRERR_NONE != sessionEncryptComAck())
	{
		return OprErrNum::OPRERR_OTHER;
	}

	// 4.返回接收奥比数据
	outdata = QByteArray((char*)result_data.payload.cmdpaylod, result_data.payload.cmdlen);

	return OprErrNum::OPRERR_NONE;
}

bool LMProtocol::resetRootKey()
{
	payloadform payload;
	uint16_t cmdtype = (uint16_t)SessionCiphetextCmdType::SESSION_CIP_RESET;
	packPlayload(cmdtype, ++tx_seq_, QByteArray(), payload);

	LMProtocol::type_payload sendbuf = packData(PacketType::PACKETTYPE_SESSIONENC, payload);

	if (!sendData(sendbuf))
	{
		qDebug() << "com send data failed!\n";
		return false;
	}

	QByteArray read_data;
	if (!recvData(read_data))
	{
		return false;
	}

	packetform result_data;
	if (!parseData(read_data, result_data))
	{
		//return OprErrNum::OPRERR_NEGOTIATE;
		return false;
	}

	if (result_data.payload.cmdtype != (uint16_t)SessionCiphetextCmdType::SESSION_CIP_RESETACK)
	{

		return false;
	}

	// 清除root、session密钥
	root_key_.clear();
	root_iv_.clear();
	session_key_.clear();
	session_iv_.clear();
	clearSeq();

	return true;
}

OprErrNum LMProtocol::sessionOTAFileData(const QByteArray& indata, QByteArray& outdata)
{
	uint16_t cmdtype = (uint16_t)PlainTextCmdType::PLA_FILEDATA;

	payloadform payload;
	packPlayload(cmdtype, ++tx_seq_, indata, payload);

	LMProtocol::type_payload sendbuf = packData(PacketType::PACKETTYPE_PLAINTEXT, payload);
	if (!sendData(sendbuf))
	{
		qDebug() << "com send data failed!\n";
		return OprErrNum::OPRERR_OTHER;
	}


	//setAsyncReadData(true);
	QByteArray read_data;
	if (!recvData(read_data))
	{
		return OprErrNum::OPRERR_NEGOTIATE;
	}
	//setAsyncReadData(false);

	packetform result_data;
	if (!parseData(read_data, result_data))
	{
		return OprErrNum::OPRERR_CMD_UNKNOWN;
	}

	// 4.返回接收奥比数据
	outdata = QByteArray((char*)result_data.payload.cmdpaylod, result_data.payload.cmdlen);

	return OprErrNum::OPRERR_NONE;
}

OprErrNum LMProtocol::sessionPlainTextComRecv(QByteArray& outdata)
{
	QByteArray read_data;
	if (!recvData(read_data))
	{
		return OprErrNum::OPRERR_NEGOTIATE;
	}

	packetform result_data;
	if (!parseData(read_data, result_data))
	{
		return OprErrNum::OPRERR_CMD_UNKNOWN;
	}

	// 4.返回接收奥比数据
	outdata = QByteArray((char*)result_data.payload.cmdpaylod, result_data.payload.cmdlen);

	return OprErrNum::OPRERR_NONE;
}

void LMProtocol::setRcvTimeout(uint32_t time_ms)
{
	rcvTimeout_ = time_ms;
}

OprErrNum LMProtocol::sessionEncryptComAck()
{
	uint16_t cmdtype = (uint16_t)SessionCiphetextCmdType::SESSION_CIP_SEQUENT;
	payloadform payload;
	memset(&payload, 0, sizeof(payload));
	packPlayload(cmdtype, ++tx_seq_, QByteArray(), payload);

	auto sendbuf = packData(PacketType::PACKETTYPE_SESSIONENC, payload);
	if (!sendData(sendbuf))
	{
		qDebug() << "com send data failed!\n";
		return OprErrNum::OPRERR_OTHER;
	}

	return OprErrNum::OPRERR_NONE;
}

LMProtocol::type_payload LMProtocol::packPlayload(uint16_t cmd_type, uint32_t cmd_seq,
	const type_payload& payload, payloadform& outpayload)
{
	payloadform stPayload;

	stPayload.cmdtype = cmd_type;
	stPayload.cmdseq = cmd_seq;
	stPayload.cmdlen = payload.size();
	//std::cout << "cmd seq = " << cmd_seq << std::endl;

	if (!payload.isEmpty())
	{
		memcpy(stPayload.cmdpaylod, payload.data(), payload.size());
	}
	outpayload = stPayload;

	return TransformPayLoad(stPayload);
}

LMProtocol::type_payload LMProtocol::packData(PacketType packet_type, const payloadform& payload)
{
	packetheadform packet_head;

	packet_head.head = LM_PACKET_HEAD;
	//packet_head.payloadlen = PACKET_HEADLEN + PAYLOAD_HEADLEN + payload.cmdlen;
	packet_head.packettype = static_cast<uint16_t>(packet_type);

	QByteArray payload_data;
	payload_data = TransformPayLoad(payload);
	if (!encrypt(packet_type, payload_data))
	{
		return QByteArray();
	}

	packet_head.payloadlen = PACKET_HEADLEN + payload_data.size();
	packet_head.payloadcrc32 = crc_.getCrc(PointerCast(payload_data.data()), payload_data.size());

	QByteArray packet_buff;
	packet_buff.push_back(TransformHead(packet_head));
	packet_buff.push_back(payload_data);

	return 	packet_buff;
}

bool LMProtocol::parseData(const type_payload& indata, packetform& outdata)
{
	packetform packet_data;

	packetheadform& pkt_head = packet_data.head;
	if (!parseHead(indata, pkt_head))
	{
		std::cout << "[LMProtocol]parseHead fail\n";
		return false;
	}
	uint32_t payloadlen = pkt_head.payloadlen - PACKET_HEADLEN;
	QByteArray payload_buf(indata.mid(sizeof(packetheadform), payloadlen));

	// crc check
	uint32_t crc32 = crc_.getCrc(PointerCast(payload_buf.data()), payloadlen);
	if (crc32 != pkt_head.payloadcrc32)
	{
		std::cout << "[LMProtocol]crc32 check fail\n";
		return false;
	}

	PacketType pkt_type = static_cast<PacketType>(pkt_head.packettype);
	// decrypt
	if (!decrypt(pkt_type, payload_buf))
	{
		return false;
	}

	// parse
	if (!parsePayload(payload_buf, packet_data.payload))
	{
		std::cout << "[LMProtocol]parsePayload fail\n";
		PrintByteArray("payload: ", payload_buf, true);
		return false;
	}

	pkt_head.payloadlen = packet_data.payload.cmdlen + PAYLOAD_HEADLEN;

	outdata = packet_data;

	return true;
}

bool LMProtocol::parseHead(const type_payload& indata, packetheadform& outdata)
{
	// head
	packetheadform& pkt_head(outdata);
	pkt_head.head = LittleEnidan16(indata.data());
	if (pkt_head.head != LM_PACKET_HEAD)
	{
		return false;
	}

	pkt_head.payloadlen = LittleEnidan16(indata.mid(2).data());
	pkt_head.payloadcrc32 = LittleEnidan32(indata.mid(4).data());
	pkt_head.packettype = LittleEnidan16(indata.mid(8).data());

	return true;
}

bool LMProtocol::parsePayload(const type_payload& indata, payloadform& outdata)
{
	const int kCmdTypeLen = 2;
	const int kCmdSeqLen = 4;
	const int kCmdLen = 2;

	// paylod 
	payloadform& payload_data = outdata;
	payload_data.cmdtype = LittleEnidan16(indata.data());
	payload_data.cmdseq = LittleEnidan32(indata.mid(kCmdTypeLen).data());
	payload_data.cmdlen = LittleEnidan16(indata.mid(kCmdTypeLen + kCmdSeqLen).data());

	if (payload_data.cmdlen > indata.size())
	{
		return false;
	}

	if (0 != payload_data.cmdlen)
	{
		memcpy(payload_data.cmdpaylod, indata.mid(kCmdTypeLen + kCmdSeqLen + kCmdLen).data(), payload_data.cmdlen);
	}

	return true;
}

void LMProtocol::handleErr(OprErrNum err_num)
{
}

bool LMProtocol::sendData(const QByteArray& data)
{
#if 0
	emit SendData(data);
#else
	if (!comDev_.isOpen())
	{
		return false;
	}

#ifndef _LM_TEST
	int datalen = data.size();
	//! 清楚缓冲区数据，防止数据粘连
	comDev_.clear(QSerialPort::AllDirections);
	comDev_.write((const char*)data.data(), datalen);
	//! 立即发送
	comDev_.flush();
#else
	moduleDummySerialWrite(data);
#endif
#endif
	PrintByteArray("send: ", data);
	return true;
}

bool LMProtocol::recvData(QByteArray& data)
{
	QByteArray read_data;
	if (!comDev_.isOpen())
	{
		return false;
	}

	int timeout_nums = 0;
	int max_timeout_times = rcvTimeout_ / 20;
	int framelen = -1;

#ifndef _LM_TEST
	while ((++timeout_nums < max_timeout_times) && framelen != read_data.size())
	{
		//std::cout << "can read size = " << comDev_.bytesAvailable() << std::endl;
		QByteArray tmpbuf;
		if (enAsyncRead)
		{
			emit RecvData(tmpbuf);
			QThread::msleep(20);
		}
		else
		{
			if (comDev_.waitForReadyRead(20))
			{
				tmpbuf = comDev_.readAll();
			}
		}

		//! check whether recv complete frame
		if (!tmpbuf.isEmpty())
		{
			uint16_t framehead = ((tmpbuf.at(0) & 0x00ff) | tmpbuf.at(1) << 8 & 0xff00);
			if (framehead == LM_PACKET_HEAD)
			{
				read_data = tmpbuf;
				framelen = ((tmpbuf.at(3) << 8) & 0xff00) | tmpbuf.at(2);
			}
			else
			{
				read_data.append(tmpbuf);
			}
		}

	}

	if (read_data.isEmpty())
	{
		return false;
	}
#else
	moduleDummySerialRead(read_data);
#endif
	PrintByteArray("recv: ", read_data);
	data = read_data;
	return true;
}

bool LMProtocol::queryData(const QByteArray& send_data, QByteArray& read_data)
{
	if (!sendData(send_data))
	{
		std::cout << "com send data failed!\n";
		return false;
	}

	if (!recvData(read_data))
	{
		return false;
	}

	return true;
}

bool LMProtocol::aes256_cbc_encrypt(const QByteArray& indata, const QByteArray& key,
	const QByteArray& iv, QByteArray& outdata)
{
	QByteArray result;
	result = QAESEncryption::Crypt(QAESEncryption::AES_256, QAESEncryption::CBC, indata,
		key.leftJustified(32, 0), iv);
	if (result.isEmpty())
	{
		outdata = indata;
		return false;
	}

#if 0
	PrintByteArray("key: ", key);
	PrintByteArray("iv: ", iv_);
#endif
	outdata = result;
	return true;
}

bool LMProtocol::aes256_cbc_decrypt(const QByteArray& indata, const QByteArray& key,
	const QByteArray& iv, QByteArray& outdata)
{
	QByteArray bfresult;
	QByteArray afresult;

	bfresult = QAESEncryption::Decrypt(QAESEncryption::AES_256, QAESEncryption::CBC,
		indata, key.leftJustified(32, 0), iv);
	afresult = QAESEncryption::RemovePadding(bfresult);

#if 0
	PrintByteArray("key: ", key);
	PrintByteArray("iv: ", iv_);
#endif
	outdata = afresult;
	return true;
}

void LMProtocol::clearSeq()
{
	tx_seq_ = 0;
	rx_seq_ = 0;
}

bool LMProtocol::encrypt(PacketType packet_type, QByteArray& inoutdata)
{
	if (packet_type == PacketType::PACKETTYPE_PLAINTEXT) // 密文处理
	{
		return true;
	}

	QByteArray key;
	QByteArray iv;
	if (packet_type == PacketType::PACKETTYPE_ROOTENC)
	{
		key = root_key_;
		iv = root_iv_;
	}
	else
	{
		key = session_key_;
		iv = session_iv_;
	}

	//PrintByteArray("EncryptRawData: ", inoutdata);
	QByteArray encryptdata;
	if (!aes256_cbc_encrypt(inoutdata, key, iv, encryptdata))
	{
		return false;
	}
	//PrintByteArray("Aes256-CBC EncryptData: ", encryptdata);

	inoutdata = encryptdata;

	return true;
}

bool LMProtocol::decrypt(PacketType packet_type, QByteArray& inoutdata)
{
	if (packet_type == PacketType::PACKETTYPE_PLAINTEXT)
	{
		return true;
	}

	QByteArray decryptdata;

	QByteArray key;
	QByteArray iv;
	if (packet_type == PacketType::PACKETTYPE_ROOTENC)
	{
		key = root_key_;
		iv = root_iv_;
	}
	else
	{
		key = session_key_;
		iv = session_iv_;
	}

	//PrintByteArray("DecryptRawData: ", inoutdata);
	if (!aes256_cbc_decrypt(inoutdata, key, iv, decryptdata))
	{
		std::cout << "[LMProtocol]aes256_cbc_decrypt fail\n";
		return false;
	}
	//PrintByteArray("Aes256-CBC DecryptData: ", decryptdata);

	inoutdata = decryptdata;

	return true;
}

void LMProtocol::setAsyncReadData(bool enable)
{
	emit enableAsyncRead(enable);
	enAsyncRead = enable;
}



#ifdef _LM_TEST
bool LMProtocol::moduleDummySerialWrite(const QByteArray& indata)
{
	static const std::unordered_map<uint16_t, std::pair<uint16_t, uint16_t>> cmdTypeCouple{
	{(uint16_t)PlainTextCmdType::PLA_ROOTKEY,{(uint16_t)PlainTextCmdType::PLA_ROOTKEY_RESULT,(uint16_t)PacketType::PACKETTYPE_PLAINTEXT}},
	{(uint16_t)RootCiphetextCmdType::ROOT_CIP_SESSIONKEY, {(uint16_t)RootCiphetextCmdType::ROOT_CIP_SESSIONKEY_ACK,(uint16_t)PacketType::PACKETTYPE_ROOTENC}},
	{(uint16_t)SessionCiphetextCmdType::SESSION_CIP_PROBEDATA,{(uint16_t)SessionCiphetextCmdType::SESSION_CIP_PROBEECHO,(uint16_t)PacketType::PACKETTYPE_SESSIONENC}},
	{(uint16_t)SessionCiphetextCmdType::SESSION_CIP_ORBCMD,{(uint16_t)SessionCiphetextCmdType::SESSION_CIP_SEQUENT,(uint16_t)PacketType::PACKETTYPE_SESSIONENC}},
	{(uint16_t)SessionCiphetextCmdType::SESSION_CIP_SEQUENT,{(uint16_t)SessionCiphetextCmdType::SESSION_CIP_ORBCMD,(uint16_t)PacketType::PACKETTYPE_SESSIONENC}},
	};

	PrintByteArray("dummyWriteData: ", indata);

	packetform packet;
	parseData(indata, packet);

	auto rqtype = cmdTypeCouple.find(packet.payload.cmdtype);
	if (rqtype == cmdTypeCouple.end())
	{
		return false;
	}

	// 匹配应答数据
	QByteArray paylod_buf;
	uint16_t cmdtype = rqtype->second.first;

	if (cmdtype == (uint16_t)PlainTextCmdType::PLA_ROOTKEY_RESULT)
	{
		paylod_buf.push_back(0x01);
	}
	else if (cmdtype == (uint16_t)RootCiphetextCmdType::ROOT_CIP_SESSIONKEY_ACK)
	{
		paylod_buf.push_back(root_key_);
	}
	else if (cmdtype == (uint16_t)SessionCiphetextCmdType::SESSION_CIP_PROBEECHO
		|| cmdtype == (uint16_t)SessionCiphetextCmdType::SESSION_CIP_ORBCMD)
	{
		for (int i = 0; i < packet.payload.cmdlen; i++)
		{
			paylod_buf.push_back(packet.payload.cmdpaylod[i]);
		}
	}
	else
	{

	}

	payloadform payload;
	packPlayload(cmdtype, ++rx_seq_, paylod_buf, payload);
	LMProtocol::type_payload sendbuf = packData((PacketType)rqtype->second.second, payload);

	dummyDataBuff_.push_back(sendbuf);

	if (cmdtype == (uint16_t)SessionCiphetextCmdType::SESSION_CIP_SEQUENT)
	{
		cmdtype = (uint16_t)SessionCiphetextCmdType::SESSION_CIP_ORBCMD;
		paylod_buf = std::move(QByteArray((char*)packet.payload.cmdpaylod, packet.payload.cmdlen));
		payloadform payload;
		packPlayload(cmdtype, ++rx_seq_, paylod_buf, payload);
		LMProtocol::type_payload sendbuf = packData((PacketType)rqtype->second.second, payload);
		dummyDataBuff_.push_back(sendbuf);
	}

	return true;
}

bool LMProtocol::moduleDummySerialRead(QByteArray& outdata)
{
	if (dummyDataBuff_.isEmpty())
	{
		return false;
	}

	outdata = dummyDataBuff_.takeFirst();

	PrintByteArray("dummyReadData: ", outdata);

	return true;
}

#include <QSerialPortInfo>
#include "windows.h"

//decodedString == inputStr !!

uint8_t default_key[32] = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73,
0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81, 0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 };

uint8_t default_key_iv[16] = { 0x50, 0x21, 0x9E, 0xB6, 0x57, 0x4C, 0xE8, 0x93, 0x56, 0x32,
0xEA, 0xC4, 0xD8, 0x0B, 0xA3, 0x72
};

uint8_t default_sessionkey[32] = { 0x15, 0x01, 0xc0, 0xd0, 0xe4, 0xfd, 0xdf, 0xd7, 0x7a, 0x65,
0xf1, 0x2f, 0x45, 0x61, 0x0b, 0x59, 0xd9, 0x0a, 0x9c, 0x61, 0x0c, 0x04, 0x76, 0xdb,
0x0b, 0xbe, 0x9e, 0xe4, 0x7f, 0x8d, 0xe1, 0x46 };

uint8_t default_session_iv[16] = { 0xe6,0x4a,0x55,0x7c,0x39,0x8b,0x56,0x31,0x7f,0x5e,0xb8,
0x74,0xc9,0xa3,0x85,0x36 };

int main(void)
{
	//char val[]{ 0x12,0x34,0x55,0x66 };
	//uint16_t reval = LittleEnidan16(val);
	//uint32_t reval32 = LittleEnidan32(val);

	//auto b16 = LittleEnidan16(reval);
	//auto b32 = LittleEnidan32(reval32);

	AllocConsole();
	freopen("CONOUT$", "w+t", stdout);

	//char data[]{ 0xa8,0x27,0x02,0x00,0x00,0x00,0x0a,0x00,0x68,0x65,0x6c,0x6c,0x6f,0x77,0x6f,0x72,0x6c,0x64 };
	//char key_[]{ 0x6f,0x72,0x62,0x62,0x65,0x63 };
	//QByteArray iv_(16, 0x00);

	//QByteArray hashKey(key_, std::size(key_));// = QCryptographicHash::hash(/*key.toLocal8Bit()*/key_, QCryptographicHash::Sha256);
	//QByteArray hashIV(iv_);// = QCryptographicHash::hash(/*iv_.toLocal8Bit()*/iv_, QCryptographicHash::Md5);

	QSerialPort com;
	com.setPortName("COM9");
	com.setBaudRate(115200);
	com.setDataBits(QSerialPort::Data8);
	com.setFlowControl(QSerialPort::NoFlowControl);
	com.setStopBits(QSerialPort::OneStop);
	com.setParity(QSerialPort::NoParity);
	com.open(QIODevice::ReadWrite);

	//uint8_t LMProtocolEnableCmd[] = { 0xF5,0xAE,0x01,0x01,0x02,0x00,0xDA,0x01,0x7E,0xfd };
	//com.write((char*)LMProtocolEnableCmd, std::size(LMProtocolEnableCmd));

	LMProtocol lmprotocolObj(com);
	OprErrNum err;

	// 设置加密测试模式
	if (OprErrNum::OPRERR_NONE != lmprotocolObj.setWorkMode(WorkMode::MODE_ENCRYPT_TEST))
	{
		std::cout << "enable LM encrypt test mode fail!" << std::endl;
		return 0;
	}

	QByteArray root_key((char*)default_key, std::size(default_key));
	QByteArray root_iv((char*)default_key_iv, std::size(default_key_iv));
	QByteArray session_key((char*)default_sessionkey, std::size(default_sessionkey));
	QByteArray session_iv(root_iv);// ((char*)default_session_iv, std::size(default_session_iv));

	//1 根密钥协商
	err = lmprotocolObj.rootKeyNegotiateProc(root_key, root_iv);
	if (err != OprErrNum::OPRERR_NONE)
	{
		;
	}

	//2 会话密钥协商
	err = lmprotocolObj.sessionKeyNegotiateProc(session_key, session_iv);
	if (err != OprErrNum::OPRERR_NONE)
	{
		;
	}

	//3 会话通信
	QByteArray incmd(5, 0x23);
	char cmd[] = { 0xF5, 0xAE, 0x01 , 0x03 , 0x01 , 0x00 , 0x62 , 0xf6 , 0xfd };


	QByteArray outcmd;
	err = lmprotocolObj.sessionEncryptComProc(QByteArray(cmd, std::size(cmd)), outcmd);
	if (err != OprErrNum::OPRERR_NONE)
	{
		;
	}

	com.close();
	getchar();
	return 0;
}
#endif