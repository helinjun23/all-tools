#ifndef _SERIAL_ORB_CMD_PKT_H_
#define _SERIAL_ORB_CMD_PKT_H_

#include <stdint.h>
#include <string.h>
#include "DataTyp.h"
#include "OrbProtocolDataTyp.h"

#include "md5/md5sum.h"
#include "Qt-AES/qaesencryption.h"
#include "utility/utility.hpp"


class CSerialOrbCmdPkt
{
public:
	CSerialOrbCmdPkt() :encrypt(false) {
	}
	~CSerialOrbCmdPkt() {}

	bool GetOrbEncryptEnable() {
		return encrypt;
	}

	void SetOrbEncryptEnable(bool bEnable) {
		encrypt = bEnable;
	}

	void ClearEncryptKey() {
		orbCrypt.clearKey();
	}
	void SetEncryptSeqrule(const QByteArray& seqrule) {
		orbCrypt.set_seqrule(seqrule);
	}
	void SetEncryptSeed(const QByteArray& seed) {
		orbCrypt.set_seed(seed);
	}
	QByteArray GetEncryptKey() {
		return orbCrypt.get_encKey();
	}
	int FillHostPkt(OrbProtocolPktTyp pktTyp, const std::vector<uint8_t>& indata, SerialProtocolCmdData* pktData)
	{
		QByteArray tmpdata(indata.size(), 0);
		memcpy(tmpdata.data(), indata.data(), indata.size());
		return FillHostPkt(pktTyp, tmpdata, pktData);
	}
	//pktdata:cmd buff
	//pktTyp:enum OrbProtocolPktTyp
	int FillHostPkt(OrbProtocolPktTyp pktTyp, const QByteArray& indata, SerialProtocolCmdData* pktData)
	{
		if (nullptr == pktData || indata.isEmpty())
			return -1;
		AddHead(OrbProtocolSourceHost, pktTyp, pktData);
		AddData(indata, pktData);
		AddCheckSum(pktData);
		return 0;
	}
	//pktdata:cmd buff
	//pktTyp:enum OrbProtocolPktTyp
	int FillSlavePkt(OrbProtocolPktTyp pktTyp, const QByteArray& indata, SerialProtocolCmdData* pktData)
	{
		if (nullptr == pktData || indata.isEmpty())
			return -1;
		AddHead(OrbProtocolSourceSlave, pktTyp, pktData);
		AddData(indata, pktData);
		AddCheckSum(pktData);
		return 0;
	}
	// ParseOrbCmd
	int ParseOrbCmd(const QByteArray& indata, OrbProtocolCmd& orbCmd)
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
			QVector<uint8_t> outdata;
			OrbProtocolCmdConvertStr(orbCmd, outdata);
		}
		else
		{
			memcpy(orbCmd.data, pHead + 6, orbCmd.len); // 7
			uint32_t bCrcLen = 6 + orbCmd.len + 2;
			orbCmd.crc = (*(pHead + bCrcLen - 1) << 8 & 0xff00) | (*(pHead + bCrcLen - 2) & 0x00ff);
		}

		return 1;
	}
	// 将OrbProtocolCmd数据转为字符串形式
	QString OrbProtocolCmdConvertStr(OrbProtocolCmd& indata, QVector<uint8_t>& outdata)
	{
		outdata.clear();
		QVector<uint8_t>& deCryptData = outdata;
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
private:
	//pktdata:cmd buff
	//pktTyp:enum OrbProtocolPktTyp
	//srcTyp:enum OrbProtocolSource
	void AddHead(OrbProtocolSource srcTyp, OrbProtocolPktTyp pktTyp, SerialProtocolCmdData* pktData)
	{
		if (nullptr == pktData)
		{
			return;
		}

		pktData->data[0] = ORB_PROTOCOL_TAG_LOW;
		pktData->data[1] = ORB_PROTOCOL_TAG_HIGH;
		pktData->data[2] = srcTyp;
		pktData->data[3] = pktTyp;
		pktData->dataLen += 4;
	}

	void AddData(const QByteArray& indata, SerialProtocolCmdData* pktData)
	{
		if (nullptr == pktData)
		{
			return;
		}

		auto encrypt_data = indata;
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

		pktData->data[pktData->dataLen] = datalen & 0x00ff;
		pktData->data[pktData->dataLen + 1] = (datalen & 0xff00) >> 8;
		pktData->dataLen += 2;

		uint8_t* pData = pktData->data + pktData->dataLen;
		memcpy(pData, encrypt_data.data(), datalen * sizeof(uint8_t));
		pktData->dataLen += datalen;
	}

	void AddCheckSum(SerialProtocolCmdData* pktData)
	{
		if (nullptr == pktData)
		{
			return;
		}
		uint16_t crc = 0;
		crc = CheckSum(pktData->data, pktData->dataLen);
		pktData->data[pktData->dataLen] = (crc & 0x00ff);
		pktData->data[pktData->dataLen + 1] = (crc & 0xff00) >> 8;
		pktData->dataLen += 2;
	}

	uint16_t CheckSum(uint8_t* cmd, uint32_t cmdLen)
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
protected:

private:
	bool encrypt;
	OrbbecCryptMannage orbCrypt;
};

#endif
