#include "SerialProtocolOrb.h"
#include "OrbProtocolDataTyp.h"
#include "clogcat.h"

CSerialProtocolOrb::CSerialProtocolOrb()
{
	memset(&m_recvData, 0, sizeof(m_recvData));

}

CSerialProtocolOrb::~CSerialProtocolOrb()
{

}

void CSerialProtocolOrb::SetSendFunc(std::function<int(void* param, const uint8_t*, uint32_t)> func, void* param)
{
	m_sendFunc = func;
	m_pParam = param;
	m_orbComm.SetSendFun(func, param);
}

int CSerialProtocolOrb::Init(void)
{
	CDataBuffer resp(512);
	int procRes = m_orbComm.Init(resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}

int CSerialProtocolOrb::EncKey(void)
{
	CDataBuffer resp(512);
	int procRes = m_orbComm.EncOrder(resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}

int CSerialProtocolOrb::TriggerCap(uint8_t mediaTyp, uint32_t capTime, bool bCloud, uint8_t capTyp)
{
	CDataBuffer resp(512);
	int procRes = m_orbComm.TriggerCap(mediaTyp, capTime, bCloud, capTyp, resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}

int CSerialProtocolOrb::SetVolume(uint8_t volume)
{
	CDataBuffer resp(512);
	int procRes = m_orbComm.SetVolume(volume, resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}

int CSerialProtocolOrb::GetVersion(void)
{
	CDataBuffer resp(512);
	int procRes = m_orbComm.GetVersion(resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}

int CSerialProtocolOrb::ConfigWifi(std::string ssid, std::string pwd, std::string reserved, bool bFac)
{
	CDataBuffer resp(512);
	int procRes = m_orbComm.ConfigWifi(ssid, pwd, reserved, bFac, resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}

int CSerialProtocolOrb::GetWifiMac(void)
{
	CDataBuffer resp(512);
	int procRes = m_orbComm.GetWifiMac(resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}

int CSerialProtocolOrb::GetWifiSta(void)
{
	CDataBuffer resp(512);
	int procRes = m_orbComm.GetWifiSta(resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}

int CSerialProtocolOrb::SetCloudKey(std::string productKey, std::string productSecret, std::string deviceName, std::string deviceSecret)
{
	CDataBuffer resp(512);
	int procRes = m_orbComm.SetCloudKey(productKey, productSecret, deviceName, deviceSecret, resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}


int CSerialProtocolOrb::GetDeviceName(void)
{
	CDataBuffer resp(512);
	int procRes = m_orbComm.GetDeviceName(resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}

int CSerialProtocolOrb::PeepholeSleep(int liveTyp)
{
	CDataBuffer resp(512);
	int procRes = m_orbComm.PeepholeSleep(liveTyp, resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}

int CSerialProtocolOrb::GetIpInfo(void)
{
	CDataBuffer resp(512);
	int procRes = m_orbComm.GetIpInfo(resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}

int CSerialProtocolOrb::PicDirCtl(int dir)
{
	CDataBuffer resp(512);
	int procRes = m_orbComm.PicDirCtl(dir, resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}

int CSerialProtocolOrb::PirCtl(bool bOpen)
{
	CDataBuffer resp(512);
	int procRes = m_orbComm.PirCtl(bOpen, resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}

int CSerialProtocolOrb::EventProc(uint32_t eventid, MediaCapEventParam& param)
{
	CDataBuffer resp(512);
	int procRes = m_orbComm.EventProc(eventid, param, resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}

int CSerialProtocolOrb::Ota(bool bStart)
{
	CDataBuffer resp(512);
	int procRes = m_orbComm.Ota(bStart, resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}
int CSerialProtocolOrb::DataPassToServer(uint8_t* data, uint16_t datalen)
{
	CDataBuffer resp(512);
	int procRes = m_orbComm.DataPass2Server(data, datalen, resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}

int CSerialProtocolOrb::GetNetTime(void)
{
	CDataBuffer resp(512);
	int procRes = m_orbComm.GetNetTime(resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}

int CSerialProtocolOrb::SetWifiKeepLive(uint8_t typ, bool bFace)
{
	CDataBuffer resp(512);
	int procRes = m_orbComm.SetWifiKeepLive(typ, bFace, resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}

int CSerialProtocolOrb::Reset(void)
{
	CDataBuffer resp(256);
	int procRes = m_orbComm.Reset(resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}
int CSerialProtocolOrb::GetDevState(void)
{
	CDataBuffer resp(256);
	int procRes = m_orbComm.GetDevState(resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}

int CSerialProtocolOrb::ScanConf(bool bStart)
{
	CDataBuffer resp(128);
	int procRes = m_orbComm.ScanConf(bStart, resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}

int CSerialProtocolOrb::ProtocolExtend(uint8_t* msg, uint32_t msglen)
{
	CDataBuffer resp(1056);
	int procRes = m_orbComm.ProtocolExtend(msg, msglen, resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}

int CSerialProtocolOrb::GetPersonDetectSta(void)
{
	CDataBuffer resp(1056);
	int procRes = m_orbComm.GetPersonDetectSta(resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}

int CSerialProtocolOrb::GetMaskDetectSta(void)
{
	CDataBuffer resp(1056);
	int procRes = m_orbComm.GetMaskDetectSta(resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}

int CSerialProtocolOrb::LookupCloudRemind(uint8_t openTyp, uint8_t userId)
{
	CDataBuffer resp(1056);
	int procRes = m_orbComm.LookupCloudRemind(openTyp, userId, resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	return 0;
}

int CSerialProtocolOrb::GetDeviceSN(void)
{
	return 0;
}

int CSerialProtocolOrb::SetFov(int fov)
{
	return  0;
}

int CSerialProtocolOrb::GetCloudKey(void)
{
	CDataBuffer resp(512);
	int procRes = m_orbComm.GetCloudKey(resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	//m_orbFac.GetCloudKey();
	return 0;
}

int CSerialProtocolOrb::SetUserData(uint8_t eventTyp, uint8_t userTyp, uint8_t authTyp, uint8_t mainId, uint8_t subId)
{
	CDataBuffer resp(512);
	int procRes = m_orbComm.SetUserData(eventTyp, userTyp, authTyp, mainId, subId, resp);
	if (procRes > 0)
	{
		SendMsg(resp.GetData(), resp.GetDataLen());
	}
	//m_orbFac.GetCloudKey();
	return 0;
}
//parse
bool CSerialProtocolOrb::CheckTag(const uint8_t* cmd, const uint32_t cmdlen)
{
	bool bCheck = false;
	const uint8_t* pTmp = cmd;
	int checkLen = 0;
	do {
		if ((ORB_PROTOCOL_TAG_LOW == *(pTmp)) && (ORB_PROTOCOL_TAG_HIGH == *(pTmp + 1)))
		{
			bCheck = true;
			break;
		}
		else
		{
			pTmp++;
			checkLen++;
		}
	} while (checkLen < cmdlen);

	return bCheck;
}

int  CSerialProtocolOrb::ParseCmd(const uint8_t* cmd, const uint32_t cmdlen, bool bClearBuf)
{
	if (bClearBuf)
	{
		//printf("---------------clear buf \n");
		memset(&m_recvData, 0, sizeof(m_recvData));
	}

	if (cmd == nullptr || cmdlen <= 0)
	{
		emit CLogcat::Instance()->ShowNoteInfo("cmdlen <4,len=" + QString::number(cmdlen), 0, true);
		return SerialCmdParseStaOrbRun;
	}
	//emit CLogcat::Instance()->PrintHex((unsigned char *)cmd,cmdlen);
	memcpy(m_recvData.data + m_recvData.dataLen, cmd, cmdlen);
	m_recvData.dataLen += cmdlen;

	OrbProtocolCmd orbCmd;
	memset(&orbCmd, 0, sizeof(orbCmd));

	uint8_t* pHead = m_recvData.data;
	if ((m_recvData.dataLen >= 6) && (ORB_PROTOCOL_TAG_LOW == (*pHead)) && (ORB_PROTOCOL_TAG_HIGH == *(pHead + 1))) //get cmd length
	{
		orbCmd.len = ((m_recvData.data[5] << 8) | m_recvData.data[4]);
	}

	if (m_recvData.dataLen < (orbCmd.len + 6))
	{
		emit CLogcat::Instance()->ShowNoteInfo("***ORB recv len=" + QString::number(cmdlen), 0, true);
		return SerialCmdParseStaOrbRun;
	}
	//emit CLogcat::Instance()->ShowNoteInfo("111",0,true);
	//parse data
	orbCmd.hLow = m_recvData.data[0];
	orbCmd.hHigh = m_recvData.data[1];
	orbCmd.source = m_recvData.data[2];
	orbCmd.typ = m_recvData.data[3];
	orbCmd.len = ((m_recvData.data[5] << 8) | m_recvData.data[4]);
	if (orbCmd.len > 0)
	{
		memcpy(orbCmd.data, m_recvData.data + 6, orbCmd.len);
	}
	//emit CLogcat::Instance()->ShowNoteInfo("orb cmdlen="+QString::number(orbCmd.len),0,true);

	uint32_t bCrcLen = 6 + orbCmd.len + 2;
	orbCmd.crc = (m_recvData.data[bCrcLen - 1]) << 8 | (m_recvData.data[bCrcLen - 2]);

	int procRes = -1;
	if (OrbProtocolPktTypPeepholeComm == orbCmd.typ
		|| OrbProtocolPktTypPeepholeEncryp == orbCmd.typ)
	{
		if (PeepholeOrbCommCmdTypPrepare == orbCmd.data[0])
		{
			m_orbComm.SetEncrypt(m_bMsgEncrypt);
		}
		CDataBuffer resp(512);
		procRes = m_orbComm.CmdProc(orbCmd, resp);
		if (procRes > 0)
		{
			SendMsg(resp.GetData(), resp.GetDataLen());
		}
	}

	//emit CLogcat::Instance()->ShowNoteInfo("222",0,true);
	if (OrbProtocolPktTypPeepholeFac == orbCmd.typ) //peephole factory
	{
		procRes = m_orbFac.OrbFacParseData(orbCmd);
	}

	//clear recv buf
	memset(&m_recvData, 0, sizeof(m_recvData));
	return SerialCmdParseStaEnd;
}

int CSerialProtocolOrb::SendMsg(const uint8_t* msg, const uint32_t msglen)
{
	if (msglen <= 0)
	{
		return -1;
	}

	//hex_print(msg,msglen,32,(char *)"orb Sendcmd:");
	//CLogcat::Instance()->OutSendCmd("Orb Send:",(unsigned char *)msg,msglen);
	if (m_sendFunc != nullptr)
	{
		//CLogcat::Instance()->OutSendCmd("Orb Send:",(unsigned char *)msg,msglen);
		m_sendFunc(m_pParam, msg, msglen);
	}
	else
	{
		emit CLogcat::Instance()->ShowNoteInfo("send func is null", 1, true);
	}

	return 1;
}

