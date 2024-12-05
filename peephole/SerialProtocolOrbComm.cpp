#include <QTime>
#include <QDateTime>
#include "SerialProtocolOrbComm.h"
#include "OrbUtils.h"
#include "clogcat.h"
#include "DataBuffer.h"
#include "FileHelper.h"

CSerialProtocolOrbComm::CSerialProtocolOrbComm() :m_timeZone(8)
{
	m_bCmdEncrypt = false;
	m_bCmdEncSet = false;

	memset(m_keyOrder, 0, sizeof(m_keyOrder));
	memset(m_aesKey, 0, sizeof(m_aesKey));
	m_aesKeyLen = 0;
	memset(m_seed, 0, sizeof(m_seed));
	//
	m_pFw = nullptr;
	m_otaPktlen = 128;
	m_sleepTime = 0;
	ReadConfig();
}

CSerialProtocolOrbComm::~CSerialProtocolOrbComm()
{

}

void CSerialProtocolOrbComm::ReadConfig()
{
	int conNum = 0;
	char* pNum = NULL;
	std::ifstream fin("./paramConfig.ini");
	const int LINE_LENGTH = 1024;
	char str[LINE_LENGTH] = { 0 };
	while (fin.getline(str, LINE_LENGTH))
	{
		//ShowNoteInfo(str,1,true);

		pNum = NULL;
		//depth
		pNum = strstr(str, "mcuOtaPkt=");
		if (pNum != NULL)
		{
			pNum += strlen("mcuOtaPkt=");
			conNum = atoi(pNum);
			m_otaPktlen = (uint16_t)conNum;

			char resStr[128] = { 0 };
			sprintf(resStr, "OTA:mcuOtaPkt=%d", m_otaPktlen);
			emit CLogcat::Instance()->ShowNoteInfo(resStr, 2, true);
		}
		pNum = strstr(str, "sleepTime=");
		if (pNum != NULL)
		{
			pNum += strlen("sleepTime=");
			conNum = atoi(pNum);
			m_sleepTime = (uint8_t)conNum;

			char resStr[128] = { 0 };
			sprintf(resStr, "sleepTime=%d", m_sleepTime);
			emit CLogcat::Instance()->ShowNoteInfo(resStr, 2, true);
		}
	}

}
void CSerialProtocolOrbComm::SetEncrypt(bool bEncrypt)
{
	m_bCmdEncSet = bEncrypt;
}

int CSerialProtocolOrbComm::Init(CDataBuffer& resp)
{
	//msg
	uint8_t msg[64] = { 0 };
	uint32_t msglen = 0;

	msg[0] = PeepholeOrbCommCmdTypInit;
	msg[1] = 0x00;
	msglen += 2;

	m_seed[0] = (uint8_t)Random(0, 255);
	m_seed[1] = (uint8_t)Random(0, 255);
	m_seed[2] = (uint8_t)Random(0, 255);
	m_seed[3] = (uint8_t)Random(0, 255);

	if (m_bCmdEncSet)
	{
		msg[2] = 0x01;
		//SetKey(m_seed,4,m_keyOrder,16);
		//m_bCmdEncrypt = true;
	}
	else
	{
		msg[2] = 0x00;
		//m_bCmdEncrypt = false;
	}
	msglen += 1;

	msg[3] = m_seed[0];
	msg[4] = m_seed[1];
	msg[5] = m_seed[2];
	msg[6] = m_seed[3];
	msglen += 4;


	for (int i = 0; i < 16; i++)
	{
		if (m_bCmdEncSet)
			m_keyOrder[i] = Random(0, 31);
		else
			m_keyOrder[i] = 0;
	}

	memcpy(msg + 7, m_keyOrder, sizeof(m_keyOrder));
	msglen += 16;

	emit CLogcat::Instance()->PrintHex((unsigned char*)m_keyOrder, 16, QString("key order:"));
	if (m_bCmdEncSet)
	{
		SetKey(m_seed, 4, m_keyOrder, 16);
		m_bCmdEncrypt = true;
	}
	else
	{
		m_bCmdEncrypt = false;
	}
	QDateTime localTime;
	localTime = QDateTime::currentDateTime();
	uint32_t curT = localTime.toSecsSinceEpoch();
	emit CLogcat::Instance()->ShowNoteInfo("init curT=" + QString::number(curT), 0, true);

	//
	QDateTime utcTime = QDateTime::currentDateTimeUtc();
	uint32_t utcT = utcTime.toSecsSinceEpoch();
	emit CLogcat::Instance()->ShowNoteInfo("init utcT=" + QString::number(utcT), 0, true);

	utcT -= m_timeZone * 3600;

#if 0
	msg[23] = curT & 0xff;
	msg[24] = (curT >> 8) & 0xff;
	msg[25] = (curT >> 16) & 0xff;
	msg[26] = (curT >> 24) & 0xff;
#else
	msg[23] = utcT & 0xff;
	msg[24] = (utcT >> 8) & 0xff;
	msg[25] = (utcT >> 16) & 0xff;
	msg[26] = (utcT >> 24) & 0xff;
#endif

	msglen += 4;

	FillMsg(msg, msglen, false, resp);

	return 1;
}

int CSerialProtocolOrbComm::EncOrder(CDataBuffer& resp)
{
	//msg
	uint8_t msg[64] = { 0 };
	uint32_t msglen = 0;

	msg[0] = 0x00;//PeepholeOrbCommCmdTypKeyOrder;
	msg[1] = 0x00;
	msglen += 2;


	for (int i = 0; i < 16; i++)
	{
		m_keyOrder[i] = Random(0, 31);
	}

	memcpy(msg + 2, m_keyOrder, sizeof(m_keyOrder));
	msglen += 16;
	FillMsg(msg, msglen, false, resp);
	return 1;
}

int CSerialProtocolOrbComm::TriggerCap(const uint8_t mediaTyp, const uint32_t capTime, const bool bCloud, const uint8_t capTyp, CDataBuffer& resp)
{
	return -1;
}

int CSerialProtocolOrbComm::SetVolume(const uint8_t volume, CDataBuffer& resp)
{
	return -1;
}

int CSerialProtocolOrbComm::GetVersion(CDataBuffer& resp)
{
	//msg
	uint8_t msg[64] = { 0 };
	uint32_t msglen = 0;

	msg[0] = PeepholeOrbCommCmdTypVersion;
	msg[1] = 0x00;
	msglen += 2;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::ConfigWifi(const std::string ssid, const std::string pwd, const std::string reserved, bool bFac, CDataBuffer& resp)
{
	//msg
	uint8_t msg[128] = { 0 };
	uint32_t msglen = 0;

	msg[0] = PeepholeOrbCommCmdTypSsidPwd;
	msg[1] = 0x00;
	if (reserved.length() > 0 && reserved.length() < 255)
		msg[1] = reserved.length();
	msglen += 2;

	memcpy(msg + 2, ssid.c_str(), ssid.length());
	msglen += 32;
	memcpy(msg + 34, pwd.c_str(), pwd.length());
	msglen += 32;
	if (reserved.length() > 0 && reserved.length() < 255)
	{
		memcpy(msg + 66, reserved.c_str(), reserved.length());
		msglen += reserved.length();
	}

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);

	return 1;
}

int CSerialProtocolOrbComm::GetWifiMac(CDataBuffer& resp)
{
	//msg
	uint8_t msg[64] = { 0 };
	uint32_t msglen = 0;

	msg[0] = PeepholeOrbCommCmdTypMac;
	msg[1] = 0x00;
	msglen += 2;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::GetWifiSta(CDataBuffer& resp)
{
	//msg
	uint8_t msg[64] = { 0 };
	uint32_t msglen = 0;

	msg[0] = PeepholeOrbCommCmdTypNetSta;
	msg[1] = 0x00;
	msglen += 2;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::SetCloudKey(const std::string productKey, const std::string productSecret, const std::string deviceName, const std::string deviceSecret, CDataBuffer& resp)
{
	//msg
	uint8_t msg[256] = { 0 };
	uint32_t msglen = 0;

	msg[0] = PeepholeOrbCommCmdTypLinkKey;
	msg[1] = 0x00;
	msglen += 2;

	memcpy(msg + msglen, productKey.c_str(), productKey.length());
	msglen += 32;
	memcpy(msg + msglen, productSecret.c_str(), productSecret.length());
	msglen += 64;
	memcpy(msg + msglen, deviceName.c_str(), deviceName.length());
	msglen += 32;
	memcpy(msg + msglen, deviceSecret.c_str(), deviceSecret.length());
	msglen += 64;


	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}


int CSerialProtocolOrbComm::GetCloudKey(CDataBuffer& resp)
{
	//msg
	uint8_t msg[256] = { 0 };
	uint32_t msglen = 0;

	msg[0] = PeepholeOrbCommCmdTypCloudKey;
	msg[1] = 0x00;
	msglen += 2;


	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return  1;
}

int CSerialProtocolOrbComm::GetDeviceName(CDataBuffer& resp)
{
	//msg
	uint8_t msg[32] = { 0 };
	uint32_t msglen = 0;

	msg[0] = PeepholeOrbCommCmdTypDevName;
	msg[1] = 0x00;
	msglen += 2;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::PeepholeSleep(const int liveTyp, CDataBuffer& resp)
{
	return -1;
}

int CSerialProtocolOrbComm::GetIpInfo(CDataBuffer& resp)
{
	//msg
	uint8_t msg[32] = { 0 };
	uint32_t msglen = 0;

	msg[0] = PeepholeOrbCommCmdTypIpAddr;
	msg[1] = 0x00;
	msglen += 2;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::PicDirCtl(const int dir, CDataBuffer& resp)
{
	//msg
	uint8_t msg[32] = { 0 };
	uint32_t msglen = 0;

	msg[0] = PeepholeOrbCommCmdTypPicDir;
	msg[1] = 0x00;
	msglen += 2;

	msg[2] = (uint8_t)dir;
	msglen += 1;
	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::PirCtl(const bool bOpen, CDataBuffer& resp)
{
	return -1;
}

uint8_t GetAlarm(uint8_t alarmtyp)
{
	uint8_t alarm = 0;
	switch (alarmtyp)
	{
	case alarm_typ_none:
		alarm = PeepholeOrbAlarmTypNone;
		break;
	case alarm_typ_tamper: //撬锁
		alarm = PeepholeOrbAlarmTypTamper;
		break;
	case alarm_typ_break_in://非法闯入
		alarm = PeepholeOrbAlarmTypBreakIn;
		break;
	case alarm_typ_try_ban: //禁试
		alarm = PeepholeOrbAlarmTypTryBan;
		break;
	case alarm_typ_fake_lock: //假锁
		alarm = PeepholeOrbAlarmTypFakeLock;
		break;
	case alarm_typ_defence: //一键布防
		alarm = PeepholeOrbAlarmTypDefence;
		break;
	case alarm_typ_peep_timeout: //超时窥视
		alarm = PeepholeOrbAlarmTypPeepTimeout;
		break;
	case alarm_typ_lock_exception://锁体异常
		alarm = PeepholeOrbAlarmTypLockException;
		break;
	case alarm_typ_hijack_finger_print://劫持指纹
		alarm = PeepholeOrbAlarmTypHijackFingerprint;
		break;
	case alarm_typ_hijack_pwd: //劫持密码
		alarm = PeepholeOrbAlarmTypHijackPwd;
		break;
	case alarm_typ_door_not_close:
		alarm = PeepholeOrbAlarmTypDoorOpen;
		break;
	case alarm_typ_door_mask:
		alarm = PeepholeOrbAlarmTypDoorMask;
		break;
	case alarm_typ_move_detect:
		alarm = PeepholeOrbAlarmTypMotionDetection;
		break;
	case alarm_typ_hijack_face:
		alarm = PeepholeOrbAlarmTypHijackFace;
		break;
	case alarm_typ_hijack_finger_vein:
		alarm = PeepholeOrbAlarmTypHijackFingerVerin;
		break;
	case alarm_typ_sprint_bolt_not_in:
		alarm = PeepholeOrbAlarmTypSpringBoltNotIn;
		break;
	case alarm_typ_spring_bolt_not_out:
		alarm = PeepholeOrbAlarmTypSpringBoltNotOut;
		break;
	case alarm_typ_face_err:
		alarm = PeepholeOrbAlarmTypFaceErr;
		break;
	case alarm_typ_finger_err:
		alarm = PeepholeOrbAlarmTypFingerprintErr;
		break;
	case alarm_typ_back_board_err:
		alarm = PeepholeOrbAlarmTypBackBoardErr;
		break;
	default:
		break;
	}
	return alarm;
}

uint8_t GetEvent(uint8_t eventtyp)
{
	uint8_t event = 0;
	switch (eventtyp)
	{
	case event_typ_none:
		event = PeepholeOrbEventTypNone;
		break;
	case event_typ_door_bell:
		event = PeepholeOrbEventTypDoorBell;
		break;
	case event_typ_door_open:
		event = PeepholeOrbEventTypDoorOpen;
		break;
	case event_typ_door_close:
		event = PeepholeOrbEventTypDoorClose;
		break;
	case event_typ_door_anti_lock:
		event = PeepholeOrbEventTypDoorAntiLock;
		break;
	case event_typ_door_cancel:
		event = PeepholeOrbEventTypDoorCancel;
		break;
	case event_typ_door_reset:
		event = PeepholeOrbEventTypDoorReset;
		break;
	case event_typ_door_try_err:
		event = PeepholeOrbEventTypDoorTryErr;
		break;
	case event_typ_leave_home_open:
		event = PeepholeOrbEventTypDoorLeaveOpen;
		break;
	case event_typ_leave_home_close:
		event = PeepholeOrbEventTypDoorLeaveClose;
		break;
	case event_typ_out_remind:
		event = PeepholeOrbEventTypDoorVoiceRemind;
		break;
	case event_typ_remote_open_success:
		event = PeepholeOrbEventTypDoorRemoteOpenSuccess;
		break;
	case event_typ_remote_open_fail:
		event = PeepholeOrbEventTypDoorRemoteOpenFail;
		break;
	case evnet_typ_temp_pwd_open_success:
		event = PeepholeOrbEventTypDoorRemotePwdOpenSuccess;
		break;
	case evnet_typ_temp_pwd_open_fail:
		event = PeepholeOrbEventTypDoorRemotePwdOpenFail;
		break;
	case evnet_typ_door_open_fail:
		event = PeepholeOrbEventTypDoorOpenFail;
		break;
	default:
		break;
	}
	return event;
}

uint8_t  GetFreeze(uint8_t eventtyp)
{
	uint8_t typ = 0;
	switch (eventtyp)
	{
	case event_freeze_typ_none:
		typ = PeepholeOrbFreezeTypNone;
		break;
	case event_freeze_typ_print:
		typ = PeepholeOrbFreezeTypPrint;
		break;
	case event_freeze_typ_pwd:
		typ = PeepholeOrbFreezeTypCode;
		break;
	case event_freeze_typ_card:
		typ = PeepholeOrbFreezeTypCard;
		break;
	case event_freeze_typ_face:
		typ = PeepholeOrbFreezeTypFace;
		break;
	case event_freeze_typ_finger_vein:
		typ = PeepholeOrbFreezeTypFingerVein;
		break;
	default:
		break;
	}
	//PeepholeOrbFreezeTypNone=0x00,
	//PeepholeOrbFreezeTypPrint=0x01,
	//PeepholeOrbFreezeTypCode=0x02,
	//PeepholeOrbFreezeTypCard=0x03,
	//PeepholeOrbFreezeTypFace=0x04,
	//PeepholeOrbFreezeTypFingerVein=0x05,
	return typ;
}

uint8_t GetEventTyp(uint8_t event, uint8_t eventtyp)
{
	uint8_t typ = 0;
	switch (event)
	{
	case event_alarm_typ_freeze:
		typ = GetFreeze(eventtyp);
		break;
	case event_alarm_typ_alarm:
		typ = GetAlarm(eventtyp);
		break;
	case event_alarm_typ_event:
		typ = GetEvent(eventtyp);
		break;
	case event_alarm_typ_battery:
		typ = eventtyp;
		break;
	default:
		break;
	}
	return typ;
}

uint8_t GetOpenTyp(uint8_t typ)
{
	uint8_t ret = 0;
	switch (typ)
	{
	case lock_open_typ_none:
		ret = orb_lock_open_typ_none;
		break;
	case lock_open_typ_finger_print:
		ret = orb_lock_open_typ_finger_print;
		break;
	case lock_open_typ_pwd:
		ret = orb_lock_open_typ_pwd;
		break;
	case lock_open_typ_card:
		ret = orb_lock_open_typ_card;
		break;
	case lock_open_typ_key:
		ret = orb_lock_open_typ_key;
		break;
	case lock_open_typ_finger_pwd:
		ret = orb_lock_open_typ_finger_pwd;
		break;
	case lock_open_typ_tmp_pwd:
		ret = orb_lock_open_typ_tmp_pwd;
		break;
	case lock_open_typ_app:
		ret = orb_lock_open_typ_app;
		break;
	case lock_open_typ_face:
		ret = orb_lock_open_typ_face;
		break;
	case lock_open_typ_finger_vein:
		ret = orb_lock_open_typ_finger_vein;
		break;
	case lock_open_typ_face_finger:
		ret = orb_lock_open_typ_face_finger;
		break;
	case lock_open_typ_finger_vein_pwd:
		ret = orb_lock_open_typ_finger_vein_pwd;
		break;
	case lock_open_typ_face_finger_vein:
		ret = orb_lock_open_typ_face_finger_vein;
		break;
	case lock_open_typ_in_door:
		ret = orb_lock_open_typ_in_door;
		break;
	case lock_open_typ_iris:
		ret = orb_lock_open_typ_iris;
		break;
	case lock_open_typ_limit_time_pwd:
		ret = orb_lock_open_typ_limit_time_pwd;
		break;
	case lock_open_typ_out_door:
		ret = orb_lock_open_typ_out_door;
		break;
	case lock_open_typ_palmvein:
		ret = orb_lock_open_typ_palmvein;
		break;
	default:
		break;
	}
	return ret;
}

int CSerialProtocolOrbComm::EventProc(const uint32_t eventid, const MediaCapEventParam& param, CDataBuffer& resp)
{
	//msg
	uint8_t msg[32] = { 0 };
	uint32_t msglen = 0;

	msg[0] = PeepholeOrbCommCmdTypEvent;
	msg[1] = param.offline;
	msglen += 2;

	msg[2] = param.eventid & 0xff;
	msg[3] = (param.eventid >> 8) & 0xff;
	msglen += 2;

	switch (param.event)
	{
	case event_alarm_typ_freeze:
		msg[4] = PeepholeOrbEventAlarmTypFreeze;
		break;
	case event_alarm_typ_alarm:
		msg[4] = PeepholeOrbEventAlarmTypAlarm;
		break;
	case event_alarm_typ_event:
		msg[4] = PeepholeOrbEventAlarmTypEvent;
		break;
	case event_alarm_typ_battery:
		msg[4] = PeepholeOrbEventAlarmTypBatteryLevel;
		break;
	default:
		break;
	}
	msglen += 1;

	msg[5] = GetEventTyp(param.event, param.eventtyp);
	msglen += 1;
	if (param.event == event_alarm_typ_battery)
	{
		msg[6] = param.opentyp;
	}
	else {
		msg[6] = GetOpenTyp(param.opentyp);
	}

	msglen += 1;
	msg[7] = param.userid;
	msglen += 1;

	switch (param.action)
	{
	case event_alarm_action_typ_none:
		msg[8] = PeepholeOrbEventActionTypNone;
		break;
	case event_alarm_action_typ_rec_video:
		msg[8] = PeepholeOrbEventActionTypVideoRec;
		break;
	case event_alarm_action_typ_rec_audio:
		msg[8] = PeepholeOrbEventActionTypAudioRec;
		break;
	case event_alarm_action_typ_rec_av:
		msg[8] = PeepholeOrbEventActionTypAVRec;
		break;
	case event_alarm_action_typ_snap:
		msg[8] = PeepholeOrbEventActionTypSnap;
		break;
	case event_alarm_action_typ_cloud:
		msg[8] = PeepholeOrbEventActionTypCloud;
		break;
	default:
		break;
	}

	msg[9] = param.actionParam;

	if (param.ts != 0)
	{
		msg[10] = param.ts & 0xff;
		msg[11] = (param.ts >> 8) & 0xff;
		msg[12] = (param.ts >> 16) & 0xff;
		msg[13] = (param.ts >> 24) & 0xff;
		msglen += 4;
	}

	msglen += 2;
	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::Ota(bool start, CDataBuffer& resp)
{
	uint8_t msg[64] = { 0 };
	uint16_t msglen = 0;
#if 0
	msg[0] = PeepholeOrbCommCmdTypOta;
	msg[1] = 0x00;
	msglen += 2;

	if (start)
	{
		msg[2] = 0x01;
	}
	else
	{
		msg[2] = 0x02;
	}
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
#else
	msg[0] = PeepholeOrbCommCmdTypMcuOtaSta;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x02;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
#endif

	return 1;
}

int CSerialProtocolOrbComm::DataPass2Server(const uint8_t* data, uint16_t datalen, CDataBuffer& resp)
{

	uint8_t msg[512] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypDataPass;
	msg[1] = 0x00;
	msglen += 2;
	//域名
	msg[2] = 0;
	msglen += 1;
	//port
	msg[3] = 0;
	msg[4] = 0;
	msglen += 2;
	//data
	if (datalen > 0)
	{
		msg[5] = datalen & 0xff;
		msglen += 1;
		memcpy(msg + 6, data, datalen);
		msglen += datalen;
	}

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::GetNetTime(CDataBuffer& resp)
{
	uint8_t msg[64] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypNetTime;
	msg[1] = 0x00;
	msglen += 2;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::SetWifiKeepLive(uint8_t typ, bool bFace, CDataBuffer& resp)
{

	uint8_t msg[64] = { 0 };
	uint16_t msglen = 0;
	if (bFace)
	{
		msg[0] = PeepholeOrbCommCmdTypPowerOff;
	}
	else
		msg[0] = PeepholeOrbCommCmdTypWifiKeepLive;
	msg[1] = m_sleepTime;
	msglen += 2;

	if (bFace)
	{
		if (wifi_action_typ_live == typ)
			msg[2] = 0x01;
		if (wifi_action_typ_sleep == typ)
			msg[2] = 0x00;
	}
	else {
		if (wifi_action_typ_standard == typ)
			msg[2] = 0x00;
		if (wifi_action_typ_live == typ)
			msg[2] = 0x01;
		if (wifi_action_typ_sleep == typ)
			msg[2] = 0x02;
	}
	msglen += 1;
	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::Reset(CDataBuffer& resp)
{
	uint8_t msg[64] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypReset;
	msg[1] = 0x00;
	msglen += 2;

	msg[2] = 0x00;
	msg[3] = 0x01;
	msg[4] = 0x00;
	msglen += 3;
	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::GetDevState(CDataBuffer& resp)
{
	uint8_t msg[64] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypDevStaGet;
	msg[1] = 0x00;
	msglen += 2;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::ScanConf(bool bStart, CDataBuffer& resp)
{
	uint8_t msg[64] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypScanNetConf;
	msg[1] = 0x00;
	msglen += 2;

	if (bStart)
	{
		msg[2] = 0x01;
	}
	else
	{
		msg[2] = 0x02;
	}
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::ProtocolExtend(const uint8_t* data, const uint32_t datalen, CDataBuffer& resp)
{
	uint8_t msg[1024] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypCmdExtend;
	msg[1] = 0x00;
	msglen += 2;

	if (datalen > 0)
	{
		memcpy(msg + 2, data, datalen);
		msglen += datalen;
	}

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::GetPersonDetectSta(CDataBuffer& resp)
{
	uint8_t msg[1024] = { 0 };
	uint16_t msglen = 0;
	msg[0] = 0x011;//PeepholeOrbCommCmdTypPersonGet;
	msg[1] = 0x00;
	msglen += 2;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::GetMaskDetectSta(CDataBuffer& resp)
{
	uint8_t msg[1024] = { 0 };
	uint16_t msglen = 0;
	msg[0] = 0x011;//PeepholeOrbCommCmdTypMaskGet;
	msg[1] = 0x00;
	msglen += 2;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::LookupCloudRemind(const uint8_t openTyp, const uint8_t userId, CDataBuffer& resp)
{
	uint8_t msg[1024] = { 0 };
	uint16_t msglen = 0;
	msg[0] = 0x011;//PeepholeOrbCommCmdTypLookRemind;
	msg[1] = 0x00;
	msglen += 2;

	msg[2] = GetOpenTyp(openTyp);
	msg[3] = userId;
	msglen += 2;
	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::SetUserData(uint8_t eventTyp, uint8_t userTyp, uint8_t authTyp, uint8_t mainId, uint8_t subId, CDataBuffer& resp)
{
	uint8_t msg[256] = { 0 };
	uint16_t msglen = 0;
	msg[0] = 0x01b;
	msg[1] = 0x00;
	msglen += 2;

	msg[2] = eventTyp;
	msg[3] = userTyp;
	msg[4] = authTyp;
	msg[5] = mainId;
	msg[6] = subId;
	msglen += 5;
	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::CmdProc(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	if (OrbProtocolPktTypPeepholeEncryp == orbCmd.typ)
	{
		uint8_t* pDstDec = new uint8_t[orbCmd.len];
		memset(pDstDec, 0, sizeof(uint8_t) * orbCmd.len);
		uint32_t dstDecLen = 0;
		CSecure dataSecure;
		dstDecLen = dataSecure.AesEcbDecrypt(m_aesKey, m_aesKeyLen, orbCmd.data, orbCmd.len, pDstDec);

		orbCmd.len = pDstDec[1] << 8 | pDstDec[0];
		memcpy(orbCmd.data, pDstDec + 2, dstDecLen);
		CLogcat::Instance()->OutSendCmd("dec data:", pDstDec, dstDecLen);

		delete pDstDec;
	}

	int res = 0;
	uint8_t cmdid = orbCmd.data[0];
	switch (cmdid)
	{
	case PeepholeOrbCommCmdTypInit:
		ProcInit(orbCmd);
		break;
	case PeepholeOrbCommCmdTypScanNetConf:
		ProcScanConf(orbCmd);
		break;
	case PeepholeOrbCommCmdTypVersion:
		ProcVersion(orbCmd);
		break;
	case PeepholeOrbCommCmdTypReset:
		ProcReset(orbCmd);
		break;
	case PeepholeOrbCommCmdTypPowerOff:
		ProcPowerOff(orbCmd);
		break;
	case PeepholeOrbCommCmdTypPicDir:
		ProcPicDir(orbCmd);
		break;
	case PeepholeOrbCommCmdTypOta:
		ProcOta(orbCmd);
		break;
	case PeepholeOrbCommCmdTypReserved1:
		break;
	case PeepholeOrbCommCmdTypDevStaGet:
		ProcGetDevSta(orbCmd);
		break;
	case PeepholeOrbCommCmdTypEvent:
		ProcEvent(orbCmd);
		break;
	case PeepholeOrbCommCmdTypSsidPwd:
		ProcSsidPwd(orbCmd);
		break;
	case PeepholeOrbCommCmdTypMac:
		ProcMacAddr(orbCmd);
		break;
	case PeepholeOrbCommCmdTypDataPass:
		ProcDataPass(orbCmd);
		break;
	case PeepholeOrbCommCmdTypIpAddr:
		ProcIpAddr(orbCmd);
		break;
	case PeepholeOrbCommCmdTypDevName:
		ProcDevName(orbCmd);
		break;
	case PeepholeOrbCommCmdTypLinkKey:
		ProcLinkKey(orbCmd);
		break;
	case PeepholeOrbCommCmdTypNetSta:
		ProcNetSta(orbCmd);
		break;
	case PeepholeOrbCommCmdTypNetTime:
		ProNetTime(orbCmd);
		break;
	case PeepholeOrbCommCmdTypCloudKey:
		ProcGetCloudkey(orbCmd);
		break;
	case PeepholeOrbCommCmdTypPrepare:
#if 0
		if (m_bCmdEncSet)
		{
			EncOrder(resp);
			res = 1;
		}
		else
		{
			Init(resp);
			res = 1;
		}
#else
		Init(resp);
		res = 1;
#endif
		break;
	case PeepholeOrbCommCmdTypOtaSta:
		ProcOtaSta(orbCmd);
		break;
	case PeepholeOrbCommCmdTypRecSta:
		ProcRecSta(orbCmd);
		break;
	case PeepholeOrbCommCmdTypDevSta:
		ProcDevSta(orbCmd);
		break;
	case PeepholeOrbCommCmdTypSleep:
		ProcSleepNote(orbCmd);
		break;
	case PeepholeOrbCommCmdTypNetTimeNote:
		ProcNetTimeNote(orbCmd);
		break;
	case PeepholeOrbCommCmdTypWakeUp:
		ProcWakeUpNote(orbCmd);
		break;
	case PeepholeOrbCommCmdTypWorkMode:
		res = ProcWorkMode(orbCmd, resp);
		break;
	case PeepholeOrbCommCmdTypAlarmCancel:
		res = ProcAlarmCancel(orbCmd, resp);
		break;
	case     PeepholeOrbCommCmdTypChildLockCtl:
		res = ProcChildLockCtl(orbCmd, resp);
		break;
	case     PeepholeOrbCommCmdTypTamperCtl:
		res = ProcTamperCtl(orbCmd, resp);
		break;
	case     PeepholeOrbCommCmdTypHumanDistSet:
		res = ProcHumanDistSet(orbCmd, resp);
		break;
	case     PeepholeOrbCommCmdTypVolumeSet:
		res = ProcVolumeSet(orbCmd, resp);
		break;
	case    PeepholeOrbCommCmdTypLockStaGet:
		res = ProcLockStaGet(orbCmd, resp);
		break;
	case    PeepholeOrbCommCmdTypLockVerGet:
		res = ProcLockVerGet(orbCmd, resp);
		break;
	case    PeepholeOrbCommCmdTypUserAdd:
		res = ProcUserAdd(orbCmd, resp);
		break;
	case    PeepholeOrbCommCmdTypUserDel:
		res = ProcUserDel(orbCmd, resp);
		break;
	case     PeepholeOrbCommCmdTypLockPwdSet:
		res = ProcLockPwdSet(orbCmd, resp);
		break;
	case    PeepholeOrbCommCmdTypBatteryLevelGet:
		res = ProcBatteryLevelGet(orbCmd, resp);
		break;
	case    PeepholeOrbCommCmdTypRemotePwdUnlock:
		res = ProcRemotePwdUnlock(orbCmd, resp);
		break;
	case PeepholeOrbCommCmdTypHumanIntervalSet:
		res = HumanIntervalSet(orbCmd, resp);
		break;
	case PeepholeOrbCommCmdTypOutAntiOpenIntervalSet:
		res = OutAntiOpenInterSet(orbCmd, resp);
		break;
	case PeepholeOrbCommCmdTypAppAutoLockSet://自动关锁
		res = AppAutoLockSet(orbCmd, resp);
		break;
	case PeepholeOrbCommCmdTypAppDataPass:// =0x50,
		res = AppDataPass(orbCmd, resp);
		break;
	case PeepholeOrbCommCmdTypAppLockSet://  =0x51,
		res = AppLockSet(orbCmd, resp);
		break;
	case PeepholeOrbCommCmdTypAppLockResp:// =0x52,
		res = AppLockResp(orbCmd, resp);
		break;
	case PeepholeOrbCommCmdTypAppMotorTorque:// =0x53,
		res = AppMotorTorque(orbCmd, resp);
		break;
	case PeepholeOrbCommCmdTypAppBoltRebound:// =0x54,
		res = AppBoltRebound(orbCmd, resp);
		break;
	case     PeepholeOrbCommCmdTypAppBoltBackTime://0x55
		res = AppBoltBackTime(orbCmd, resp);
		break;
	case PeepholeOrbCommCmdTypAppLockOpenOrientation://=0x56,
		res = AppLockOpenOrientation(orbCmd, resp);
		break;
	case PeepholeOrbCommCmdTypAppLockSnap://  =0x57,
		res = AppLockSnap(orbCmd, resp);
		break;
	case PeepholeOrbCommCmdTypAppRemotePwdAuth:// = 0x58,
		res = AppRemotePwdAuth(orbCmd, resp);
		break;
	case PeepholeOrbCommCmdTypAppKeepLive:// = 0x59,
		res = AppKeepLive(orbCmd, resp);
		break;
	case PeepholeOrbCommCmdTypMcuOta:
		res = McuOta(orbCmd, resp);
		break;
	case PeepholeOrbCommCmdTypMcuOtaFwInfo:
		res = McuOtaInfo(orbCmd, resp);
		break;
	case PeepholeOrbCommCmdTypMcuOtaFwPkt:
		res = McuOtaPkt(orbCmd, resp);
		break;
	case PeepholeOrbCommCmdTypMcuOtaSta:
		res = McuOtaSta(orbCmd, resp);
		break;
	default:
		break;
	}
	return res;
}

int CSerialProtocolOrbComm::SetKey(uint8_t* seed, int size, uint8_t* order, uint8_t orderlen)
{
	// the input seed must be 4 bytes
	if (size != 4) {
		emit CLogcat::Instance()->ShowNoteInfo("Incorrect input seed size", 0, true);
		return -1;
	}

	QString noteStr;
	noteStr.sprintf("seed:%d,%d,%d,%d", seed[0], seed[1], seed[2], seed[3]);
	emit CLogcat::Instance()->ShowNoteInfo(noteStr, 0, true);

	uint8_t md5[32];
	memset(md5, 0, sizeof(md5));

	CSecure seInfo;
	seInfo.GenMd5(seed, size, md5);
	emit CLogcat::Instance()->OutSendCmd("MD5:", md5, 32);

	uint8_t hexascii[32];
	memset(hexascii, 0, sizeof(hexascii));

	Bin2HexAscii(md5, 16, hexascii, 32, false);
	memset(m_aesKey, 0, sizeof(m_aesKey));
	for (int i = 0; i < 16; i++)
		m_aesKey[i] = hexascii[order[i]];
	m_aesKeyLen = 16;
	emit CLogcat::Instance()->OutSendCmd("KEY:", m_aesKey, 16);
	return 1;
}


int CSerialProtocolOrbComm::FillMsg(const uint8_t* msg, const uint32_t msglen, bool bEncrypt, CDataBuffer& resp)
{
	if (nullptr == msg)
		return -1;

	if (bEncrypt)
	{
		m_cmdPktAes.FillHostPktAes(OrbProtocolPktTypPeepholeEncryp, msg, msglen, m_aesKey, m_aesKeyLen, resp);
	}
	else
	{
		m_cmdPkt.FillHostPkt(OrbProtocolPktTypPeepholeComm, msg, msglen, resp);
	}
	return 1;
}

int CSerialProtocolOrbComm::ProcInit(OrbProtocolCmd& orbCmd)
{
	return 0;
}

int CSerialProtocolOrbComm::ProcScanConf(OrbProtocolCmd& orbCmd)
{
	return 0;
}
int CSerialProtocolOrbComm::ProcVersion(OrbProtocolCmd& orbCmd)
{
	if (orbCmd.data[2] != 0x00)
	{
		emit CLogcat::Instance()->ShowNoteInfo("Get verison failed!", 1, true);
		return -1;
	}
	char ver[32] = { 0 };
	memcpy(ver, orbCmd.data + 3, orbCmd.len - 3);
	QString version;
	version.sprintf("%d.%d.%d", ver[0], ver[1], ver[2]);
	emit CLogcat::Instance()->ShowNoteInfo(version, 2, true);

	emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypVersion, version);
	return 0;
}

int CSerialProtocolOrbComm::ProcReset(OrbProtocolCmd& orbCmd)
{
	return 0;
}

int CSerialProtocolOrbComm::ProcPowerOff(OrbProtocolCmd& orbCmd)
{
	return 0;
}

int CSerialProtocolOrbComm::ProcPicDir(OrbProtocolCmd& orbCmd)
{
	return 0;
}

int CSerialProtocolOrbComm::ProcOta(OrbProtocolCmd& orbCmd)
{
	return 0;
}

int CSerialProtocolOrbComm::ProcGetDevSta(OrbProtocolCmd& orbCmd)
{
	QString noteStr;
	noteStr.sprintf("devive sta=%02x,err=%02x", orbCmd.data[2], orbCmd.data[3]);

	emit CLogcat::Instance()->ShowNoteInfo(noteStr, 2, true);

	QString staStr;
	switch (orbCmd.data[2])
	{
	case 0x00: //idle
		staStr = "idle";
		break;
	case 0x01: //wait
		staStr = "wait";
		break;
	case 0x02: //work
		staStr = "work";
		break;
	case 0x03: //error
		staStr = "error";
		break;
	default:
		break;
	}

	emit CLogcat::Instance()->ShowDevInfo(PeepholeOrbCmdTypDevSta, staStr);
	return 0;
	return 0;
}

int CSerialProtocolOrbComm::ProcSsidPwd(OrbProtocolCmd& orbCmd)
{
	return 0;
}

int CSerialProtocolOrbComm::ProcMacAddr(OrbProtocolCmd& orbcmd)
{
	if (orbcmd.data[2] != 0x00)
	{
		emit CLogcat::Instance()->ShowNoteInfo("Get Mac Addr err!", 1, true);
		return -1;
	}
	uint8_t* pTmp = orbcmd.data + 3;
	QString str;
	str.sprintf("%02X:%02X:%02X:%02X:%02X:%02X", *(pTmp), *(pTmp + 1), *(pTmp + 2), *(pTmp + 3), *(pTmp + 4), *(pTmp + 5));
	emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypMac, str);
	return 0;
}

int CSerialProtocolOrbComm::ProcDataPass(OrbProtocolCmd& orbCmd)
{

	return 0;
}

int CSerialProtocolOrbComm::ProcIpAddr(OrbProtocolCmd& orbCmd)
{
	if (orbCmd.data[2] != 0x00)
	{
		emit CLogcat::Instance()->ShowNoteInfo("Get IP Addr err!", 1, true);
		return -1;
	}
	//emit CLogcat::Instance()->ShowNoteInfo("Get IP Addr succ!",2,true);
	uint8_t* pTmp = orbCmd.data + 3;
	QString str;
	str.sprintf("%d.%d.%d.%d", *(pTmp), *(pTmp + 1), *(pTmp + 2), *(pTmp + 3)); //IP
	emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypIP, str);

	//mask
	pTmp += 4;
	str.sprintf("%d.%d.%d.%d", *(pTmp), *(pTmp + 1), *(pTmp + 2), *(pTmp + 3));
	emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypIpMask, str);

	//gate
	pTmp += 4;
	str.sprintf("%d.%d.%d.%d", *(pTmp), *(pTmp + 1), *(pTmp + 2), *(pTmp + 3));
	emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypIPGate, str);
	return 0;
}

int CSerialProtocolOrbComm::ProcDevName(OrbProtocolCmd& orbCmd)
{
	if (orbCmd.data[2] != 0x00)
	{
		emit CLogcat::Instance()->ShowNoteInfo("Get device name failed!", 1, true);
		return -1;
	}

	char devName[64] = { 0 };
	memcpy(devName, orbCmd.data + 3, 32);

	QString str;
	str.sprintf("%s", devName);
	emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypDevName, str);
	return 0;
}

int CSerialProtocolOrbComm::ProcLinkKey(OrbProtocolCmd& orbCmd)
{
	if (orbCmd.data[2] == 0x00)
		emit CLogcat::Instance()->ShowNoteInfo("set link key success!", 2, true);
	else
		emit CLogcat::Instance()->ShowNoteInfo("set link key fail!", 1, true);
	return 0;
}

int CSerialProtocolOrbComm::ProcEvent(OrbProtocolCmd& orbCmd)
{
	if (orbCmd.data[2] != 0x00)
	{
		emit CLogcat::Instance()->ShowNoteInfo("event proc err!", 1, true);
	}
	else
	{
		emit CLogcat::Instance()->ShowNoteInfo("event proc success!", 2, true);
	}
	emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypAlarmEvent, "event");
	return 0;
}

int CSerialProtocolOrbComm::ProcNetSta(OrbProtocolCmd& orbCmd)
{

	unsigned char* pTmp = orbCmd.data;
	pTmp++;  //reserved
	if (orbCmd.data[2] != 0x00)
	{
		emit CLogcat::Instance()->ShowNoteInfo("Get net state fail!", 1, true);
		return -1;
	}

	unsigned char sta = orbCmd.data[3];
	switch (sta)
	{
	case OrbWifiStateTypConfigNet:
		emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypWifiSta, "正在配网");
		break;
	case OrbWifiStateTypConnectHot:
		emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypWifiSta, "拿到SSID和密码，正在连接热点");
		break;
	case OrbWifiStateTypConnectSucc:
		emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypWifiSta, "WiFi连接成功");
		break;
	case OrbWifiStateTypConfigFail:
		emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypWifiSta, "配网失败");
		break;
	case OrbWifiStateTypNoNet:
		emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypWifiSta, "网络功能未启用！");
		break;
	default:
		break;
	}


	QString str;
	//db
	char db = (char)(orbCmd.data[4]);
	str.sprintf("%d dbm", db); //DB
	emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypWifiStaDb, str);

	//mask
	pTmp++;
	char snr = (char)(orbCmd.data[5]);
	str.sprintf("%d", snr);
	emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypWifiStaSNR, str);
	return 0;
}

int CSerialProtocolOrbComm::ProNetTime(OrbProtocolCmd& orbCmd)
{
	if (orbCmd.data[2] != 0x00)
	{
		emit CLogcat::Instance()->ShowNoteInfo("get net time err!", 1, true);
	}

	uint32_t utcT = orbCmd.data[6] << 24 | orbCmd.data[5] << 16 | orbCmd.data[4] << 8 | orbCmd.data[3];
	emit CLogcat::Instance()->ShowNoteInfo("get netTime utcT=" + QString::number(utcT), 0, true);
	utcT += m_timeZone * 3600;

	//QTimeZone timeZone;
	QDateTime  localT = QDateTime::fromTime_t(utcT);

	uint16_t year = 0;
	uint8_t  month = 0;
	uint8_t  day = 0;
	uint8_t  hour = 0;
	uint8_t  miniute = 0;
	uint8_t  second = 0;
#if 0
	year = orbCmd.data[4] << 8 | orbCmd.data[3];
	month = orbCmd.data[5];
	day = orbCmd.data[6];
	hour = orbCmd.data[7];
	miniute = orbCmd.data[8];
	second = orbCmd.data[9];
#else
	year = localT.date().year();
	month = localT.date().month();
	day = localT.date().day();
	hour = localT.time().hour();
	miniute = localT.time().minute();
	second = localT.time().second();
#endif
	QString timeStr;
	timeStr.sprintf("%d.%d.%d %d:%d:%d", year, month, day, hour, miniute, second);
	emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypNetTime, timeStr);
	return 0;
}

int CSerialProtocolOrbComm::ProcGetCloudkey(OrbProtocolCmd& orbCmd)
{
	uint16_t cmdTyp = orbCmd.data[0];
	if (cmdTyp != PeepholeOrbCommCmdTypCloudKey)
	{
		emit CLogcat::Instance()->ShowNoteInfo("cmdTyp err", 0, true);
		return -1;
	}
	char productKey[32] = { 0 };
	char productSecret[64] = { 0 };
	char deviceName[32] = { 0 };
	char deviceSecret[64] = { 0 };

	uint8_t* pTmp = orbCmd.data + 3;
	memcpy(productKey, pTmp, 32);
	pTmp += 32;
	memcpy(productSecret, pTmp, 64);
	pTmp += 64;
	memcpy(deviceName, pTmp, 32);
	pTmp += 32;
	memcpy(deviceSecret, pTmp, 64);
	pTmp += 64;


	//QString noteStr;
	//noteStr.sprintf("%d",val);
	//emit CLogcat::Instance()->ShowDevInfo(cmdTyp,noteStr);

	emit CLogcat::Instance()->ShowDevInfo(PeepholeOrbCmdTypGetCloudKeyProductKey, productKey);
	emit CLogcat::Instance()->ShowDevInfo(PeepholeOrbCmdTypGetCloudKeyProductSecret, productSecret);
	emit CLogcat::Instance()->ShowDevInfo(PeepholeOrbCmdTypGetCloudKeyDeviceName, deviceName);
	emit CLogcat::Instance()->ShowDevInfo(PeepholeOrbCmdTypGetCloudKeyDeviceSecret, deviceSecret);

	QString noteStr;
	noteStr.sprintf("productKey=%s,productSecret=%s,deviceName=%s,deviceSecret=%s", productKey, productSecret, deviceName, deviceSecret);
	emit CLogcat::Instance()->ShowNoteInfo(noteStr, 0, true);

	return 0;
}

int CSerialProtocolOrbComm::ProcOtaSta(OrbProtocolCmd& orbCmd)
{
	uint8_t sta = orbCmd.data[2];
	uint8_t err = orbCmd.data[3];
	uint8_t progress = orbCmd.data[4];

	QString staStr, errStr, progressStr, note;
	switch (sta) {
	case 0x00:
		staStr = "空闲,";
		break;
	case 0x01:
		staStr = "升级中,";
		break;
	case 0x02:
		staStr = "升级完成,";
		break;
	case 0x03:
		staStr = "升级错误,";
		break;
	default:
		break;
	}
	switch (err) {
	case 0x00:
		errStr = "无错误,";
		break;
	case 0x01:
		errStr = "无版本信息,";
		break;
	case 0x02:
		errStr = "云端查询版本超时,";
		break;
	case 0x03:
		errStr = "校验失败,";
		break;
	case 0x04:
		errStr = "升级失败,";
		break;
	default:
		break;
	}
	note = staStr + errStr + ",进度=" + QString::number(progress);
	if (err != 0x00)
		emit CLogcat::Instance()->ShowNoteInfo(note, 1, true);
	else
		emit CLogcat::Instance()->ShowNoteInfo(note, 2, true);
	return 0;
}

int CSerialProtocolOrbComm::ProcRecSta(OrbProtocolCmd& orbCmd)
{
	QString typStr;
	switch (orbCmd.data[2]) {
	case 0x01:
		typStr = "audio";
		break;
	case 0x02:
		typStr = "video";
		break;
	case 0x03:
		typStr = "AV";
		break;
	default:
		break;
	}
	uint32_t time = orbCmd.data[3] | (orbCmd.data[4] << 8) | (orbCmd.data[5] << 16) | (orbCmd.data[6] << 24);
	uint8_t  bFinish = orbCmd.data[7];

	QString note;
	note.sprintf("typ=%s,time=%d,finish=%d", typStr.toStdString().c_str(), time, bFinish);
	emit CLogcat::Instance()->ShowNoteInfo(note, 0, true);
	return 0;
}

int CSerialProtocolOrbComm::ProcDevSta(OrbProtocolCmd& orbCmd)
{
	QString noteStr;
	noteStr.sprintf("devive sta=%02x,err=%02x", orbCmd.data[2], orbCmd.data[3]);

	emit CLogcat::Instance()->ShowNoteInfo(noteStr, 2, true);

	QString staStr;
	switch (orbCmd.data[2])
	{
	case 0x00: //idle
		staStr = "idle";
		break;
	case 0x01: //wait
		staStr = "wait";
		break;
	case 0x02: //work
		staStr = "work";
		break;
	case 0x03: //error
		staStr = "error";
		break;
	default:
		break;
	}

	emit CLogcat::Instance()->ShowDevInfo(PeepholeOrbCmdTypDevSta, staStr);
	return 0;
}

int CSerialProtocolOrbComm::ProcSleepNote(OrbProtocolCmd& orbCmd)
{
	return 0;
}

int CSerialProtocolOrbComm::ProcNetTimeNote(OrbProtocolCmd& orbCmd)
{
	QString noteStr;
#if 0
	uint16_t year = orbCmd.data[2] | (orbCmd.data[3] << 8);

	noteStr.sprintf("device time:%d.%d.%d %d:%d:%d", year, orbCmd.data[4], orbCmd.data[5], orbCmd.data[6], orbCmd.data[7], orbCmd.data[8]);
#else
	uint32_t utcT = orbCmd.data[6] << 24 | orbCmd.data[5] << 16 | orbCmd.data[4] << 8 | orbCmd.data[3];
	emit CLogcat::Instance()->ShowNoteInfo("get netTime utcT=" + QString::number(utcT), 0, true);
	utcT += m_timeZone * 3600;

	//QTimeZone timeZ=QDateTime::currentDateTime().timeZone();//当前时区的当前时间;
	QDateTime  localT = QDateTime::fromTime_t(utcT);


	uint16_t year = 0;
	uint8_t  month = 0;
	uint8_t  day = 0;
	uint8_t  hour = 0;
	uint8_t  miniute = 0;
	uint8_t  second = 0;

	year = static_cast<uint16_t>(localT.date().year());
	month = static_cast<uint8_t>(localT.date().month());
	day = static_cast<uint8_t>(localT.date().day());
	hour = static_cast<uint8_t>(localT.time().hour());
	miniute = static_cast<uint8_t>(localT.time().minute());
	second = static_cast<uint8_t>(localT.time().second());
	noteStr.sprintf("device time:%d.%d.%d %d:%d:%d", year, month, day, hour, miniute, second);
#endif
	emit CLogcat::Instance()->ShowNoteInfo(noteStr, 2, true);
	return 0;
}

int CSerialProtocolOrbComm::ProcWakeUpNote(OrbProtocolCmd& orbCmd)
{
	QString noteStr;
	switch (orbCmd.data[2])
	{
	case 0x00:
		noteStr = "上电";
		break;
	case 0x01:
		noteStr = "GPIO唤醒";
		break;
	case 0x02:
		noteStr = "PIR唤醒";
		break;
	case 0x03:
		noteStr = "复位键重启!";
		break;
	case 0x04:
		noteStr = "LCD唤醒";
		break;
	case 0x05:
		noteStr = "app唤醒";
		break;
	case 0x06:
		noteStr = "其他";
		break;
	default:
		break;
	}
	emit CLogcat::Instance()->ShowNoteInfo(noteStr, 2, true);
	return 0;
}

int CSerialProtocolOrbComm::ProcWorkMode(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypWorkMode;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);

	return 1;
}

int CSerialProtocolOrbComm::ProcAlarmCancel(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypAlarmCancel;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::ProcChildLockCtl(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypChildLockCtl;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::ProcTamperCtl(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypTamperCtl;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::ProcHumanDistSet(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypHumanDistSet;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::ProcVolumeSet(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypVolumeSet;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msg[3] = orbCmd.data[2];
	msglen += 2;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::ProcLockStaGet(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypLockStaGet;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msg[3] = 0x03;
	msg[4] = 0x02;
	msglen += 3;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::ProcLockVerGet(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypLockVerGet;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msg[3] = 0x00;
	msg[4] = 0x00;
	msg[5] = 0x00;
	msg[6] = 0x00;
	msglen += 5;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::ProcUserAdd(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	//
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypUserAdd;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;


	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	//

	//add user
	CDataBuffer resp2(256);
	uint8_t msg2[128] = { 0 };
	uint16_t msglen2 = 0;
	msg2[0] = PeepholeOrbCommCmdTypUserInfo;
	msg2[1] = 0x00;
	msglen2 += 2;
	msg2[2] = 0x01;
	msg2[3] = orbCmd.data[3];
	msg2[4] = orbCmd.data[2];
	msg2[5] = orbCmd.data[4];
	msg2[6] = 0x00;
	msglen2 += 5;
	FillMsg(msg2, msglen2, m_bCmdEncrypt, resp2);

	resp.AddData(resp2.GetData(), resp2.GetDataLen());

	return 1;
}

int CSerialProtocolOrbComm::ProcUserDel(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypUserDel;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);

	//add user
	CDataBuffer resp2(256);
	uint8_t msg2[128] = { 0 };
	uint16_t msglen2 = 0;
	msg2[0] = PeepholeOrbCommCmdTypUserInfo;
	msg2[1] = 0x00;    //add user

	msg2[0] = PeepholeOrbCommCmdTypUserInfo;
	msg2[1] = 0x00;
	msglen2 += 2;
	msg2[2] = 0x01;
	msg2[3] = orbCmd.data[2];
	msg2[4] = orbCmd.data[3];
	msg2[5] = orbCmd.data[4];
	msg2[6] = 0x00;
	msglen2 += 4;
	FillMsg(msg2, msglen2, m_bCmdEncrypt, resp2);

	resp.AddData(resp2.GetData(), resp2.GetDataLen());
	msglen2 += 2;
	msg2[2] = 0x02;
	msg2[3] = orbCmd.data[2];
	msg2[4] = orbCmd.data[3];
	msg2[5] = orbCmd.data[4];
	msg2[6] = 0x00;
	msglen2 += 4;
	FillMsg(msg2, msglen2, m_bCmdEncrypt, resp2);

	resp.AddData(resp2.GetData(), resp2.GetDataLen());

	return 1;
}

int CSerialProtocolOrbComm::ProcLockPwdSet(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypLockPwdSet;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);

	//add user
	CDataBuffer resp2(256);
	uint8_t msg2[128] = { 0 };
	uint16_t msglen2 = 0;
	msg2[0] = PeepholeOrbCommCmdTypUserInfo;
	msg2[1] = 0x00;
	msglen2 += 2;
	msg2[2] = 0x01;
	msg2[3] = 0x02;//密码
	msg2[4] = orbCmd.data[2];
	msg2[5] = orbCmd.data[11];
	msg2[6] = 0x00;
	msglen2 += 5;
	FillMsg(msg2, msglen2, m_bCmdEncrypt, resp2);

	resp.AddData(resp2.GetData(), resp2.GetDataLen());
	return 1;
}

int CSerialProtocolOrbComm::ProcBatteryLevelGet(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypBatteryLevelGet;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msg[3] = 0x50;
	msg[4] = 0x60;
	msglen += 3;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::ProcRemotePwdUnlock(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypRemotePwdUnlock;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::HumanIntervalSet(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypHumanIntervalSet;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::OutAntiOpenInterSet(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypOutAntiOpenIntervalSet;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}
int CSerialProtocolOrbComm::AppAutoLockSet(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypAppAutoLockSet;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}
int CSerialProtocolOrbComm::AppDataPass(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypAppDataPass;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::AppLockSet(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypAppLockSet;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::AppLockResp(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypAppLockResp;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}
int CSerialProtocolOrbComm::AppMotorTorque(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypAppMotorTorque;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}
int CSerialProtocolOrbComm::AppBoltRebound(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypAppBoltRebound;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::AppBoltBackTime(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypAppBoltBackTime;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::AppLockOpenOrientation(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypAppLockOpenOrientation;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}
int CSerialProtocolOrbComm::AppLockSnap(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypAppLockSnap;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;
	msg[3] = orbCmd.data[2];
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}
int CSerialProtocolOrbComm::AppRemotePwdAuth(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypAppRemotePwdAuth;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}
int CSerialProtocolOrbComm::AppKeepLive(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypAppKeepLive;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}
//mcu OTA
int CSerialProtocolOrbComm::McuOta(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	//info
	uint8_t typ = orbCmd.data[2];
	uint8_t ver[4] = { 0 };
	ver[0] = orbCmd.data[3];
	ver[1] = orbCmd.data[4];
	ver[2] = orbCmd.data[5];
	ver[3] = orbCmd.data[6];

	char resStr[128] = { 0 };
	sprintf(resStr, "OTA:typ=%d,ver=%d.%d.%d.%d", typ, ver[0], ver[1], ver[2], ver[3]);

	emit CLogcat::Instance()->ShowNoteInfo(resStr, 2, true);

	sprintf(m_mcuVer, "%d.%d.%d.%d", ver[0], ver[1], ver[2], ver[3]);
	//resp
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypMcuOta;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::McuOtaInfo(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	//cmd
	m_fwlen = orbCmd.data[5] << 24 | orbCmd.data[4] << 16 | orbCmd.data[3] << 8 | orbCmd.data[2];
	m_checkSum = orbCmd.data[7] << 8 | orbCmd.data[6];
	m_pktNum = orbCmd.data[9] << 8 | orbCmd.data[8];

	char resStr[128] = { 0 };
	sprintf(resStr, "OTA:len=%d,checkSum=%X,pktNum=%d", m_fwlen, m_checkSum, m_pktNum);
	emit CLogcat::Instance()->ShowNoteInfo(resStr, 2, true);
	if (m_pFw != nullptr)
		delete m_pFw;
	m_pFw = nullptr;
	m_pFw = new uint8_t[m_fwlen];
	memset(m_pFw, 0, sizeof(uint8_t) * m_fwlen);

	//resp
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypMcuOtaFwInfo;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::McuOtaPkt(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	//cmd
	uint16_t datalen = orbCmd.len;
	uint16_t pktId = orbCmd.data[3] << 8 | orbCmd.data[2];

	AddMcuOtaPkt(pktId, datalen - 4, (uint8_t*)(orbCmd.data + 4));

	//
	char resStr[128] = { 0 };
	sprintf(resStr, "OTA:len=%d,pktid=%d", datalen, pktId);
	emit CLogcat::Instance()->ShowNoteInfo(resStr, 2, true);
	//resp
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypMcuOtaFwPkt;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;
	msg[3] = orbCmd.data[2];
	msg[4] = orbCmd.data[3];
	msglen += 2;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}

int CSerialProtocolOrbComm::McuOtaSta(OrbProtocolCmd& orbCmd, CDataBuffer& resp)
{
	uint8_t msg[128] = { 0 };
	uint16_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypMcuOtaSta;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x00;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	return 1;
}


void  CSerialProtocolOrbComm::McuOtaRun(void* arg)
{
	CSerialProtocolOrbComm* pThis = (CSerialProtocolOrbComm*)arg;
	if (pThis != nullptr)
	{
		pThis->McuOtaProgress();
	}
	return;
}

void CSerialProtocolOrbComm::McuOtaProgress(void)
{
	emit CLogcat::Instance()->ShowNoteInfo("OTA:Start", 2, true);
	char path[128] = { 0 };
	sprintf(path, "mcu_%s.bin", m_mcuVer);
	FileHelper::write_file(path, (char*)m_pFw, (int)m_fwlen);
	//
	CDataBuffer resp(128);
	uint8_t msg[32] = { 0 };
	uint32_t msglen = 0;
	msg[0] = PeepholeOrbCommCmdTypMcuOtaSta;
	msg[1] = 0x00;
	msglen += 2;
	msg[2] = 0x01;
	msglen += 1;

	FillMsg(msg, msglen, m_bCmdEncrypt, resp);
	SendMsg(resp.GetData(), resp.GetDataLen());

	while (1)
	{
		std::this_thread::sleep_for(std::chrono::seconds(2));

		CDataBuffer resp2(128);
		uint8_t msg2[32] = { 0 };
		uint32_t msglen2 = 0;
		msg2[0] = PeepholeOrbCommCmdTypMcuOtaSta;
		msg2[1] = 0x00;
		msglen2 += 2;
		msg2[2] = 0x00;
		msglen2 += 1;

		FillMsg(msg2, msglen2, m_bCmdEncrypt, resp2);
		SendMsg(resp2.GetData(), resp2.GetDataLen());
		break;
	}
	emit CLogcat::Instance()->ShowNoteInfo("OTA:Stop", 2, true);
}

int CSerialProtocolOrbComm::AddMcuOtaPkt(const uint16_t pktId, const uint16_t datalen, const uint8_t* pData)
{
	memcpy(m_pFw + (pktId - 1) * m_otaPktlen, pData, datalen);
	if (pktId >= m_pktNum)
	{
		m_mcuOtaThead = std::thread(McuOtaRun, this);
		//std::thread mcuOtaThead(McuOtaRun,this);
	}
	return 0;
}

int CSerialProtocolOrbComm::SendMsg(const uint8_t* msg, const uint32_t msglen)
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
#if 0
int CSerialProtocolOrbComm::GetPersonSta(OrbProtocolCmd& orbCmd)
{
	if (orbCmd.data[2] > 0)
	{
		QString noteStr = "错误";
		emit CLogcat::Instance()->ShowNoteInfo(noteStr, 2, true);
	}
	else {
		QString info;
		if (orbCmd.data[3])
			info = "有人";
		else
			info = "无人";
		emit CLogcat::Instance()->ShowDevInfo(PeepHoleOrbCmdTypGetPersonSta, info);
	}
	return 0;
}

int CSerialProtocolOrbComm::GetMaskSta(OrbProtocolCmd& orbCmd)
{
	if (orbCmd.data[2] > 0)
	{
		QString noteStr = "错误";
		emit CLogcat::Instance()->ShowNoteInfo(noteStr, 2, true);
	}
	else {
		QString info;
		if (orbCmd.data[3])
			info = "遮挡";
		else
			info = "正常";
		emit CLogcat::Instance()->ShowDevInfo(PeepHoleOrbCmdTypGetMaskSta, info);
	}
	return 0;
}

int CSerialProtocolOrbComm::LookRemind(OrbProtocolCmd& orbCmd)
{
	QString note;
	if (orbCmd.data[2])
		note = "失败";
	else {
		note = "成功";
	}
	emit CLogcat::Instance()->ShowNoteInfo(note, 0, true);
	return 0;
}
#endif
