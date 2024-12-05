#include "SerialProtocol3V.h"
#include"clogcat.h"
#include "DataTyp.h"

CSerialProtocol3V::CSerialProtocol3V()
{

}
CSerialProtocol3V::~CSerialProtocol3V()
{

}

int CSerialProtocol3V::Init(void)
{
    return 0;
}
int CSerialProtocol3V::EncKey(void)
{
    return 0;
}
int CSerialProtocol3V::TriggerCap(uint8_t mediaTyp,uint32_t capTime,bool bCloud,uint8_t capTyp)
{
    return 0;
}
int CSerialProtocol3V::SetVolume(uint8_t volume)
{
    return 0;
}

int CSerialProtocol3V::GetVersion(void)
{

    unsigned char cmd[255];
    unsigned char cmdlen=0;
    memset(cmd,0,sizeof(cmd));

    cmdlen = 5;
    cmd[0]=0x7B;
    cmd[1]=0x05;
    cmd[2]=PeepHole3VCmdTypeGetVersion;
    cmd[3]=0x00;
    uint8_t crc = XOR_Inverted_Check(cmd,cmdlen-1);
    cmd[4]=crc;

    SendCmd(cmd,cmdlen);
    return 0;
}

int CSerialProtocol3V::ConfigWifi(std::string ssid,std::string pwd,std::string reserved,bool bFac)
{
    unsigned char cmd[255];
    unsigned char cmdlen=0;
    memset(cmd,0,sizeof(cmd));


    cmdlen = 5+33+65;
    cmd[0]=0x7B;
    cmd[1]=cmdlen;
    if(bFac)
        cmd[2] = 0xA5;
    else
        cmd[2]=0xD8;
    cmd[3]=0x00;
    unsigned char *pTmp = cmd + 4;
    strncpy((char *)pTmp,(const char *)ssid.c_str(),ssid.length());
    pTmp += 33;
    strncpy((char *)pTmp,(const char *)pwd.c_str(),pwd.length());

    uint8_t crc = XOR_Inverted_Check(cmd,cmdlen-1);
    cmd[cmdlen-1]=crc;

    SendCmd(cmd,cmdlen);
    return 0;
}
int CSerialProtocol3V::GetWifiMac(void)
{
#if 1
    unsigned char cmd[255];
    unsigned char cmdlen=0;
    memset(cmd,0,sizeof(cmd));

    cmdlen=5;
    cmd[0]=0x7B;
    cmd[1]=0x05;
    cmd[2]=0xD7;
    cmd[3]=0x00;

    uint8_t crc = XOR_Inverted_Check(cmd,cmdlen-1);
    cmd[cmdlen-1]=crc;
#else
    unsigned char cmd[255];
    unsigned char cmdlen=0;
    memset(cmd,0,sizeof(cmd));

    cmdlen=6;
#if 0
    cmd[1]=0x7B;
    cmd[2]=0x05;
    cmd[3]=0xD7;
    cmd[4]=0x00;
    cmd[5]=0x56;
#endif
#endif

    SendCmd(cmd,cmdlen);
    return 0;
}
int CSerialProtocol3V::GetWifiSta(void)
{
    unsigned char cmd[255];
    unsigned char cmdlen=0;
    memset(cmd,0,sizeof(cmd));

    cmdlen = 5;
    cmd[0]=0x7B;
    cmd[1]=0x05;
    cmd[2]=0xD6;
    cmd[3]=0x00;
    uint8_t crc = XOR_Inverted_Check(cmd,cmdlen-1);
    cmd[4]=crc;

    SendCmd(cmd,cmdlen);
    return 0;
}
int CSerialProtocol3V::SetCloudKey(std::string productKey,std::string productSecret,std::string deviceName,std::string deviceSecret)
{
    unsigned char cmd[255];
    unsigned char cmdlen=0;
    memset(cmd,0,sizeof(cmd));

    cmdlen = 5+32+64+32+64;
    cmd[0]=0x7B;
    cmd[1]=cmdlen;
    cmd[2]=0xD5;
    cmd[3]=0x00;

    unsigned char *pTmp = cmd +4;
    strncpy((char *)pTmp,(const char *)productKey.c_str(),productKey.length());
    pTmp +=32;
    strncpy((char *)pTmp,(const char *)productSecret.c_str(),productSecret.length());
    pTmp += 64;
    strncpy((char *)pTmp,(const char *)deviceName.c_str(),deviceName.length());
    pTmp += 32;
    strncpy((char *)pTmp,(const char *)deviceSecret.c_str(),deviceSecret.length());

    uint8_t crc = XOR_Inverted_Check(cmd,cmdlen-1);
    cmd[cmdlen-1]=crc;

    SendCmd(cmd,cmdlen);
    return 0;
}

int CSerialProtocol3V::GetDeviceName(void)
{
    unsigned char cmd[255];
    unsigned char cmdlen=0;
    memset(cmd,0,sizeof(cmd));
    cmdlen = 5;
    cmd[0]=0x7B;
    cmd[1]=0x05;
    cmd[2]=0xDA;
    cmd[3]=0x00;
    uint8_t crc = XOR_Inverted_Check(cmd,cmdlen-1);
    cmd[4]=crc;
    SendCmd(cmd,cmdlen);
    return 0;
}

int CSerialProtocol3V::PeepholeSleep(int liveTyp)
{
    uint8_t cmd[255];
    uint32_t cmdlen=0;
    memset(cmd,0,sizeof(cmd));

    cmdlen = 5;
    cmd[0]=0x7B;
    cmd[1]=0x05;
    cmd[2]=0xDE;
    cmd[3]=0x00;
    uint8_t crc = XOR_Inverted_Check(cmd,cmdlen-1);
    cmd[4]=crc;

    SendCmd(cmd,cmdlen);
    return 0;
}

int CSerialProtocol3V::GetIpInfo(void)
{
    unsigned char cmd[255];
    unsigned char cmdlen=0;
    memset(cmd,0,sizeof(cmd));

    cmdlen = 5;
    cmd[0]=0x7B;
    cmd[1]=0x05;
    cmd[2]=0xDB;
    cmd[3]=0x00;
    uint8_t crc = XOR_Inverted_Check(cmd,cmdlen-1);
    cmd[4]=crc;

    SendCmd(cmd,cmdlen);
    return 0;
}

int CSerialProtocol3V::PicDirCtl(int dir)
{
    unsigned char cmd[255];
    unsigned char cmdlen=0;
    memset(cmd,0,sizeof(cmd));

    cmdlen = 6;
    cmd[0]=0x7B;
    cmd[1]=0x06;
    cmd[2]=0xD1;
    cmd[3]=0x00;

    cmd[4]=dir;
    uint8_t crc = XOR_Inverted_Check(cmd,cmdlen-1);
    cmd[5]=crc;
    SendCmd(cmd,cmdlen);

    return 0;
}

int CSerialProtocol3V::PirCtl(bool bOpen)
{
    unsigned char cmd[255];
    unsigned char cmdlen=0;
    memset(cmd,0,sizeof(cmd));

    cmdlen = 6;
    cmd[0]=0x7B;
    cmd[1]=0x06;
    cmd[2]=0xC7;
    cmd[3]=0x00;

    if(bOpen)
    {
       cmd[4]=0x01;
    }
    else
    {
       cmd[4]=0x00;
    }
    uint8_t crc = XOR_Inverted_Check(cmd,cmdlen-1);
    cmd[5]=crc;

    SendCmd(cmd,cmdlen);
    return 0;
}

uint8_t CSerialProtocol3V::GetEvent(uint8_t eventtyp)
{

    uint8_t event=0;
    switch(eventtyp)
    {
    case event_typ_none:
        event = LOCK_3V_EVENT_NO;
        break;
    case event_typ_door_bell:
        event = LOCK_3V_EVENT_DOOR_BELL;
        break;
    case event_typ_door_open:
        event = LOCK_3V_EVENT_DOOR_OPEN;
        break;
    case event_typ_door_close:
        event = LOCK_3V_EVENT_DOOR_CLOSE;
        break;
    case event_typ_door_anti_lock:
        event = LOCK_3V_EVENT_DOOR_LOCKED;
        break;
    case event_typ_door_cancel:
        event = LOCK_3V_EVENT_DOOR_LOCKED_CANCEL;
        break;
    case event_typ_door_reset:
        event = LOCK_3V_EVENT_DOOR_RESET_FACTORY;
        break;
    case event_typ_door_try_err:
        event = LOCK_3V_EVENT_DOOR_TRY_ERR;
        break;
    default:
        break;
    }
    return event;
}

uint8_t CSerialProtocol3V::GetAlarm(uint8_t alarmtyp)
{
    uint8_t alarm=0;
    switch(alarmtyp)
    {
    case alarm_typ_none:
        alarm =LOCK_3V_ALARM_NO;
        break;
    case alarm_typ_tamper: //撬锁
        alarm =LOCK_3V_ALARM_TAMPER;
        break;
    case alarm_typ_break_in://非法闯入
        alarm =LOCK_3V_ALARM_INTRUDER;
        break;
    case alarm_typ_try_ban: //禁试
        alarm =LOCK_3V_ALARM_TEST_BAN;
        break;
    case alarm_typ_fake_lock: //假锁
        alarm =LOCK_3V_ALARM_NO;
        break;
    case alarm_typ_defence: //一键布防
        alarm =LOCK_3V_ALARM_DEFENCE;
        break;
    case alarm_typ_peep_timeout: //超时窥视
        alarm =LOCK_3V_ALARM_NO;
        break;
    case alarm_typ_lock_exception://锁体异常
        alarm =LOCK_3V_ALARM_NO;
        break;
    case alarm_typ_hijack_finger_print://劫持指纹
        alarm =LOCK_3V_ALARM_NO;
        break;
    case alarm_typ_hijack_pwd: //劫持密码
        alarm =LOCK_3V_ALARM_NO;
        break;
    default:
        break;
    }
    return alarm;
}
int CSerialProtocol3V::EventProc(uint32_t eventid,MediaCapEventParam &param)
{
    unsigned char cmd[255];
    unsigned char cmdlen=0;
    memset(cmd,0,sizeof(cmd));

    cmdlen = 9;
    cmd[0]=0x7B;
    cmd[1]=0x09;
    cmd[2]=0xCA;
    cmd[3]=0x00;
    cmd[4]=0x00;
    cmd[5]=0x02;
    switch(param.event)
    {
    case event_alarm_typ_freeze:

        break;
    case event_alarm_typ_alarm:
       {
          cmd[6]=GetAlarm(param.eventtyp);
          cmd[7]=0x00;
        }
        break;
    case event_alarm_typ_event:
        {
           cmd[6]=0x00;
           cmd[7]=GetEvent(param.eventtyp);
        }
        break;
    case event_alarm_typ_battery:

        break;
    default:
        break;
    }

    uint8_t crc = XOR_Inverted_Check(cmd,cmdlen-1);
    cmd[8]=crc;
    SendCmd(cmd,cmdlen);
    return 0;
}

int CSerialProtocol3V::Ota(bool bStart)
{
    return 0;
}

int CSerialProtocol3V::DataPassToServer(uint8_t *data,uint16_t datalen)
{
    return 0;
}

int CSerialProtocol3V::GetNetTime(void)
{
    unsigned char cmd[255];
    unsigned char cmdlen=0;
    memset(cmd,0,sizeof(cmd));

    cmdlen = 5;
    cmd[0]=0x7B;
    cmd[1]=cmdlen;
    cmd[2]=0xA0;
    cmd[3]=0x00;

    uint8_t crc = XOR_Inverted_Check(cmd,cmdlen-1);
    cmd[cmdlen-1]=crc;

    SendCmd(cmd,cmdlen);
    return 0;
}

int CSerialProtocol3V::SetWifiKeepLive(uint8_t typ,bool bFace)
{
    unsigned char cmd[255];
    unsigned char cmdlen=0;
    memset(cmd,0,sizeof(cmd));

    cmdlen = 6;
    cmd[0]=0x7B;
    cmd[1]=cmdlen;
    cmd[2]=0xA3;
    cmd[3]=0x00;
    if(wifi_action_typ_live == typ)
    {
        cmd[4]=0x00;
    }
    if(wifi_action_typ_sleep == typ)
    {
        cmd[4]=0x01;
    }

    uint8_t crc = XOR_Inverted_Check(cmd,cmdlen-1);
    cmd[cmdlen-1]=crc;

    SendCmd(cmd,cmdlen);
    return 0;
}

int CSerialProtocol3V::Reset(void)
{
    return 0;
}
int CSerialProtocol3V::GetDevState(void)
{
    return 0;
}

int CSerialProtocol3V::ScanConf(bool bStart)
{

   return 0;
}
int CSerialProtocol3V::ProtocolExtend(uint8_t *msg,uint32_t msglen)
{

    return 0;
}
int CSerialProtocol3V::GetPersonDetectSta(void)
{
    return 0;
}

int CSerialProtocol3V::GetMaskDetectSta(void)
{
    return 0;
}


uint8_t CSerialProtocol3V::GetOpenTyp(uint8_t typ)
{
    uint8_t ret=0;
    switch(typ)
    {
    case lock_open_typ_none:
        ret = open_typ_3v_none;
        break;
    case lock_open_typ_finger_print:
        ret =open_typ_3v_finger;
        break;
    case lock_open_typ_pwd:
        ret = open_typ_3v_pwd;
        break;
    case lock_open_typ_card:
        ret = open_typ_3v_card;
        break;
    case lock_open_typ_key:
        ret = open_typ_3v_key;
        break;
    case lock_open_typ_finger_pwd:

        break;
    case lock_open_typ_tmp_pwd:

        break;
    case lock_open_typ_app:

        break;
    case lock_open_typ_face:
        ret = open_typ_3v_face;
        break;
    case lock_open_typ_finger_vein:
        ret =open_typ_3v_vein;
        break;
    case lock_open_typ_face_finger:

        break;
    case lock_open_typ_finger_vein_pwd:

        break;
    case lock_open_typ_face_finger_vein:

        break;
    case lock_open_typ_in_door:

        break;
    case lock_open_typ_iris:
        ret = open_typ_3v_iris;
        break;
    default:
        break;
    }
    return ret;
}

int CSerialProtocol3V::LookupCloudRemind(uint8_t openTyp,uint8_t userId)
{
    unsigned char cmd[255];
    unsigned char cmdlen=0;
    memset(cmd,0,sizeof(cmd));

    cmdlen = 10;
    cmd[0]=0x7B;
    cmd[1]=cmdlen;
    cmd[2]=0xE1;
    cmd[3]=0x00;
    cmd[4] =GetOpenTyp(openTyp);
    cmd[5]=0x00;
    cmd[6]=userId;
    cmd[7]=0x00;
    cmd[8]=0x00;
    cmd[9]=0x00;

    uint8_t crc = XOR_Inverted_Check(cmd,cmdlen-1);
    cmd[cmdlen-1]=crc;

    SendCmd(cmd,cmdlen);
    return 0;
}
int CSerialProtocol3V::GetDeviceSN(void)
{
    unsigned char cmd[255];
    unsigned char cmdlen=0;
    memset(cmd,0,sizeof(cmd));

    cmdlen = 5;
    cmd[0]=0x7B;
    cmd[1]=cmdlen;
    cmd[2]=0xA1;
    cmd[3]=0x00;

    uint8_t crc = XOR_Inverted_Check(cmd,cmdlen-1);
    cmd[cmdlen-1]=crc;

    SendCmd(cmd,cmdlen);
    return 0;
}

int CSerialProtocol3V::SetFov(int fov)
{
    unsigned char cmd[255];
    unsigned char cmdlen=0;
    memset(cmd,0,sizeof(cmd));

    cmdlen = 6;
    cmd[0]=0x7B;
    cmd[1]=cmdlen;
    cmd[2]=0xE3;
    cmd[3]=0x00;
    cmd[4]=(uint8_t)fov;

    uint8_t crc = XOR_Inverted_Check(cmd,cmdlen-1);
    cmd[cmdlen-1]=crc;

    SendCmd(cmd,cmdlen);
    return  0;
}

//parse
bool CSerialProtocol3V::CheckTag(const uint8_t *cmd,const uint32_t cmdlen)
{
    bool bCheck=false;
    const uint8_t *pTmp = cmd;
    int checkLen = 0;
    do{
        if(0x7B == *(pTmp))
        {
            bCheck = true;
            break;
        }
        else
        {
            pTmp++;
            checkLen++;
        }
    }while(checkLen < cmdlen);

    return bCheck;
}

int  CSerialProtocol3V::ParseCmd(const uint8_t *cmd,const uint32_t cmdlen,bool bClearBuf)
{
    unsigned char len=cmdlen;
    unsigned char cmdId = cmd[2];
    QString noteStr;

    uint8_t crc = XOR_Inverted_Check((unsigned char *)cmd,len-1);
    noteStr.sprintf("cmdlen=%d,cmd=%02X,crc=%02X",len,cmdId,crc);
    emit CLogcat::Instance()->ShowNoteInfo(noteStr,0,true);
    if(crc != (uint8_t)cmd[len-1])
    {
        emit CLogcat::Instance()->ShowNoteInfo("Crc error!",1,true);
        return -1;
    }
    unsigned char data[256];
    memset(data,0,sizeof(data));
    unsigned char datalen = len-4;

    memcpy(data,cmd+3,len-4);

    //PrintHex(data,datalen);
    CmdProc(cmdId,data,datalen);
    return SerialCmdParseStaEnd;
}

void CSerialProtocol3V::DevNameRespProc(unsigned char *data,unsigned char datalen)
{
    if(NULL == data || datalen == 0)
    {
        return;
    }

    unsigned char *pTmp = data;

    pTmp++; //reserved
    pTmp++;//(
    unsigned char factory = *pTmp++;
    pTmp++;//)
    unsigned char snlen= *pTmp++;
    unsigned char sn[255]={0};
    memset(sn,0,sizeof(sn));
    emit CLogcat::Instance()->ShowNoteInfo("厂家="+QString::number(factory)+",devnamelen="+QString::number(snlen),0,true);
    strncpy((char *)sn,(const char *)pTmp,snlen);
    QString str;
    str.sprintf("%s",sn);
    emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypDevName,str);
}

void CSerialProtocol3V::MacRespProc(unsigned char *data,unsigned char datalen)
{
    if(NULL == data || datalen == 0)
    {
        return;
    }
    unsigned char *pTmp = data;
    pTmp++;
    QString str;
    str.sprintf("%02X:%02X:%02X:%02X:%02X:%02X",*(pTmp),*(pTmp+1),*(pTmp+2),*(pTmp+3),*(pTmp+4),*(pTmp+5));
    emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypMac,str);
}

void CSerialProtocol3V::IpRespProc(uint8_t *data,unsigned char datalen)
{
    //emit ShowNoteInfo("1111",0,true);
    if(datalen == 0)
    {
        //emit ShowNoteInfo("2222",0,true);
        return;
    }

    uint8_t *pTmp = data;
    pTmp++;  //reserved
    uint8_t res = *pTmp++; //result
    if(res == 0x00)
    {
        emit CLogcat::Instance()->ShowNoteInfo("get ip err!",1,true);
        return;
    }

    QString str;
    str.sprintf("%d.%d.%d.%d",*(pTmp),*(pTmp+1),*(pTmp+2),*(pTmp+3)); //IP
    emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypIP,str);

    //mask
    pTmp += 4;
    str.sprintf("%d.%d.%d.%d",*(pTmp),*(pTmp+1),*(pTmp+2),*(pTmp+3));
    emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypIpMask,str);

    //gate
    pTmp += 4;
    str.sprintf("%d.%d.%d.%d",*(pTmp),*(pTmp+1),*(pTmp+2),*(pTmp+3));
    emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypIPGate,str);
}

void CSerialProtocol3V::WifiStaRespProc(unsigned char *data,unsigned char datalen)
{
    if(NULL == data || datalen == 0)
    {
        return;
    }
    unsigned char *pTmp = data;
    pTmp++;  //reserved
    unsigned char sta =*pTmp++;
    switch(sta)
    {
    case WIFI_3V_STATE_CONFIG_NET:
        emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypWifiSta,"正在配网");
        break;
    case WIFI_3V_STATE_CONNECT:
        emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypWifiSta,"拿到SSID和密码，正在连接热点");
        break;
    case WIFI_3V_STATE_CONNECT_SUCCESS:
        emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypWifiSta,"WiFi连接成功");
        break;
    case WIFI_3V_STATE_CONFIG_FAIL:
        emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypWifiSta,"配网失败");
        break;
    case WIFI_3V_STATE_NO_CONFIG_NET:
        emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypWifiSta,"网络功能未启用！");
        break;
    default:
    break;
    }


    QString str;
    //db
    char db = (char)(*pTmp);
    str.sprintf("%d dbm",db); //DB
    emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypWifiStaDb,str);

    //mask
    pTmp++;
    char snr = (char)(*pTmp);
    str.sprintf("%d",snr);
    emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypWifiStaSNR,str);
}

void CSerialProtocol3V::NetTimeProc(unsigned char *data,unsigned char datalen)
{
    uint16_t year=0;
    uint8_t  month=0;
    uint8_t  day  =0;
    uint8_t  hour =0;
    uint8_t  miniute = 0;
    uint8_t  second  =0;

    year = data[1]<<8 | data[2];
    month = data[3];
    day   = data[4];
    hour  = data[5];
    miniute = data[6];
    second  = data[7];

    QString timeStr;
    timeStr.sprintf("%d.%d.%d %d:%d:%d",year,month,day,hour,miniute,second);
    emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypNetTime,timeStr);
}
void CSerialProtocol3V::VerProc(unsigned char *data,unsigned char datalen)
{
    //printf("ver=%s \n",data+2);
    emit CLogcat::Instance()->ShowNoteInfo("ver="+QString((char *)(data+2)),2,true);

    emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypVersion,QString((char *)(data+2)));
}

void CSerialProtocol3V::DeviceSnProc(unsigned char *data,unsigned char datalen)
{
    //int len = data[2];
    //char sn[256]={0};
    QString SN((char *)(data+2));
    emit CLogcat::Instance()->ShowNoteInfo("sn="+SN,2,true);

    emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypDeviceSN,SN);
}

void CSerialProtocol3V::CmdProc(uint8_t cmd,unsigned char *data,unsigned char datalen)
{
    switch(cmd)
    {
    case PeepHole3VCmdTypUartPrepareNote:
        break;
    case PeepHole3VCmdTypWifiStaNote:
        WifiStaRespProc(data,datalen);
        break;
    case PeepHole3VCmdTypSsidPwd:
        break;
    case PeepHole3VCmdTypPirCtl:
        break;
    case PeepHole3VCmdTypPicDir:
        break;
    case PeepHole3VCmdTypWifiSta:
        WifiStaRespProc(data,datalen);
        break;
    case PeepHole3VCmdTypMac:
         MacRespProc(data,datalen);
        break;
    case PeepHole3VCmdTypDataTrans:
        break;
    case PeepHole3VCmdTypDevName:
        DevNameRespProc(data,datalen);
        break;
    case PeepHole3VCmdTypIP:
        IpRespProc(data,datalen);
        break;
    case PeepHole3VCmdTypIpcWakeNote:
        break;
    case PeepHole3VCmdTypSleep:
        break;
    case PeepHole3VCmdTypAlarmEvent:
        break;
    case PeepHole3VCmdTypAliKey:
        break;
    case PeepHole3VCmdTypeGetNetTime:
        NetTimeProc(data,datalen);
        break;
    case PeepHole3VCmdTypeGetVersion:
        VerProc(data,datalen);
        break;
    case PeepHole3VCmdTypeGetDeviceSn:
        DeviceSnProc(data,datalen);
        break;
    default:
        break;
    }
}
int CSerialProtocol3V::SendCmd(const uint8_t *cmd,const uint32_t cmdlen)
{
    if(m_sendFunc != nullptr)
        m_sendFunc(m_pParam,cmd,cmdlen);
    return 0;
}

uint8_t CSerialProtocol3V::XOR_Inverted_Check(unsigned char *inBuf,unsigned char inLen)
{
    uint8_t check = 0,i;
    for(i = 0;i < inLen;i++)
    check ^= inBuf[i];
    return ~check;
}
