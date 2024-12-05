#include "SerialProtocolK1S.h"
#include"clogcat.h"
#include"Secure.h"
#include "DataTyp.h"
#include "OrbProtocolDataTyp.h"
#include "OrbUtils.h"

CSerialProtocolK1S::CSerialProtocolK1S()
{
    memset(&m_recvBuf,0,sizeof(SerialProtocolCmdData));
    memset(&m_encrypInfo,0,sizeof(m_encrypInfo));
    memset(&m_initMsg,0,sizeof(m_initMsg));
}

CSerialProtocolK1S::~CSerialProtocolK1S()
{

}

int CSerialProtocolK1S::Init(void)
{
    m_seed[0] = (uint8_t)Random(0,255);
    m_seed[1] = (uint8_t)Random(0,255);
    m_seed[2] = (uint8_t)Random(0,255);
    m_seed[3] = (uint8_t)Random(0,255);
    //SetKey(m_seed, 4);
    emit CLogcat::Instance()->PrintHex(m_seed,4);
    //msg
    uint8_t msg[16];
    memset(msg,0,sizeof(msg));
    memcpy(msg,m_seed,sizeof(uint8_t)*4); //seed
    //
    msg[4]=0x00;//encrypt
    if(m_bMsgEncrypt)
         msg[4]=0x01;//encrypt
    memcpy(msg+5,m_seed,sizeof(uint8_t)*4); //ctime
    msg[9]=0;  //network
    msg[10]=0x0A;
    //reserved
    msg[11]=0x00;
    msg[12]=0x00;
    msg[13]=0x00;
    msg[14]=0x00;
    msg[15]=0x00;

    FillMsgData(K1S_MID_INIT,msg,16,false);
    return 0;
}

int CSerialProtocolK1S::EncKey(void)
{
    uint8_t orderNum[16]={6,18,7, 3, 13,13,23,4, 8,1,0, 25, 9,2, 2, 7};
    SetKey(m_seed, 4,orderNum,16);
    //msg
    uint8_t msg[16];
    memset(msg,0,sizeof(msg));
    memcpy(msg,orderNum,16);
    FillMsgData(K1S_MID_ENCKEY,msg,16,false);
    return 0;
}

int CSerialProtocolK1S::TriggerCap(uint8_t mediaTyp,uint32_t capTime,bool bCloud,uint8_t capTyp)
{
    //msg
    uint8_t msg[128];
    uint32_t msglen=0;
    memset(msg,0,sizeof(msg));
    msg[0]=mediaTyp ;//0:video;1:audio
    msg[1]=(capTime&0xff000000)>>24;
    msg[2]=(capTime&0x00ff0000)>>16;
    msg[3]=(capTime&0x0000ff00)>>8;
    msg[4]=(capTime&0xff);
    msg[5]=bCloud;
    msg[6]=capTyp;
    msglen += 7;

    FillMsgData(K1S_MID_TRIGGER_CAPTURE,msg,msglen,(m_encrypInfo.algTyp > 0));
    return 0;
}

int CSerialProtocolK1S::SetVolume(uint8_t volume)
{
    //msg
    uint8_t msg[128];
    uint32_t msglen=0;
    memset(msg,0,sizeof(msg));
    msg[0]=volume ;
    msglen += 1;

    FillMsgData(K1S_MID_SET_VOLUME,msg,msglen,(m_encrypInfo.algTyp > 0));
    return 0;
}

int CSerialProtocolK1S::GetVersion(void)
{
    //msg
    uint8_t msg[128];
    uint32_t msglen=0;
    memset(msg,0,sizeof(msg));


    FillMsgData(K1S_MID_GET_VERSION,msg,msglen,(m_encrypInfo.algTyp > 0));
    return 0;
}

int CSerialProtocolK1S::ConfigWifi(std::string ssid,std::string pwd,std::string reserved,bool bFac)
{
    if(ssid.length() > 33 || pwd.length() >65)
        return -1;
    //msg
    uint8_t msg[128];
    uint32_t msglen=0;
    memset(msg,0,sizeof(msg));
    memcpy(msg,ssid.c_str(),ssid.length());
    msglen += 33;
    memcpy(msg+33,pwd.c_str(),pwd.length());
    msglen += 65;

    FillMsgData(K1S_MID_CONFIG_WIFI,msg,msglen,(m_encrypInfo.algTyp > 0));
    return 0;
}

int CSerialProtocolK1S::GetWifiMac(void)
{
    //msg
    uint8_t msg[128];
    uint32_t msglen=0;
    memset(msg,0,sizeof(msg));

    FillMsgData(K1S_MID_GET_WIFIMAC,msg,msglen,(m_encrypInfo.algTyp > 0));

    return 0;
}

int CSerialProtocolK1S::GetWifiSta(void)
{
    //msg
    uint8_t msg[128];
    uint32_t msglen=0;
    memset(msg,0,sizeof(msg));

    FillMsgData(K1S_MID_GET_WIFISTATUS,msg,msglen,(m_encrypInfo.algTyp > 0));

    return 0;
}
int CSerialProtocolK1S::SetCloudKey(std::string productKey,std::string productSecret,std::string deviceName,std::string deviceSecret)
{
    if(productKey.length()>32 || productSecret.length()>64 || deviceName.length()>32 || deviceSecret.length()>64)
        return -1;
    //msg
    uint8_t msg[256];
    uint32_t msglen=0;
    memset(msg,0,sizeof(msg));

    memcpy(msg,productKey.c_str(),productKey.length());
    msglen+=32;
    memcpy(msg+msglen,productSecret.c_str(),productSecret.length());
    msglen+=64;
    memcpy(msg+msglen,deviceName.c_str(),deviceName.length());
    msglen+=32;
    memcpy(msg+msglen,deviceSecret.c_str(),deviceSecret.length());
    msglen += 64;

    FillMsgData(K1S_MID_SET_LINKKEY,msg,msglen,(m_encrypInfo.algTyp > 0));
    return 0;
}

int CSerialProtocolK1S::GetDeviceName(void)
{
    //msg
    uint8_t msg[128];
    uint32_t msglen=0;
    memset(msg,0,sizeof(msg));

    FillMsgData(K1S_MID_GET_DEVICENAME,msg,msglen,(m_encrypInfo.algTyp > 0));
    return 0;
}
int CSerialProtocolK1S::PeepholeSleep(int liveTyp)
{
    //msg
    uint8_t msg[128];
    uint32_t msglen=0;
    memset(msg,0,sizeof(msg));
    if(liveTyp > 0)
        msg[0] =1;
    else
       msg[0]=0;
    msglen += 1;
    FillMsgData(K1S_MID_POWERDOWN,msg,msglen,(m_encrypInfo.algTyp > 0));
    return 0;
}
int CSerialProtocolK1S::GetIpInfo(void)
{
    return 0;
}
int CSerialProtocolK1S::PicDirCtl(int dir)
{
    return 0;
}
int CSerialProtocolK1S::PirCtl(bool bOpen)
{
    return 0;
}
int CSerialProtocolK1S::EventProc(uint32_t eventid,MediaCapEventParam &param)
{
    uint8_t event =0;
    uint8_t mediaTyp;
    uint32_t duration;
    bool bCloud;
    uint8_t capEnable=0;
    if(param.event == event_alarm_typ_event)
    {
        if(param.eventtyp == event_typ_door_bell)
        {
            TriggerVideoCall(param); //门铃
        }
    }
   if(param.event == event_alarm_typ_alarm)
   {
       switch(param.eventtyp)
       {
        case alarm_typ_tamper: //防撬
        TriggerCap(mediaTyp,duration,bCloud,K1S_EVENT_TYP_ANTI_PRIZING);
        break;
        case alarm_typ_try_ban:      //禁试
        TriggerCap(mediaTyp,duration,bCloud,K1S_EVENT_TYP_BAN_TRY);
        break;
        case alarm_typ_defence:   //布防
        TriggerCap(mediaTyp,duration,bCloud,K1S_EVENT_TYP_PROTECTION);
        break;
        case alarm_typ_fake_lock:   //假锁
        TriggerCap(mediaTyp,duration,bCloud,K1S_EVENT_TYP_LOCK_ERR);
        break;
        // case LOCK_EVENT_TYP_RESET:        //reset
        //    break;
        default:
        break;
    }
   }
    return 0;
}
int CSerialProtocolK1S::Ota(bool bStart)
{
    return 0;
}
int CSerialProtocolK1S::DataPassToServer(uint8_t *data,uint16_t datalen)
{
    return 0;
}

int CSerialProtocolK1S::GetNetTime(void)
{
    return 0;
}
int CSerialProtocolK1S::SetWifiKeepLive(uint8_t typ,bool bFace)
{
    return 0;
}

int CSerialProtocolK1S::Reset(void)
{
    return 0;
}
int CSerialProtocolK1S::GetDevState(void)
{
    return 0;
}
int CSerialProtocolK1S::ScanConf(bool bStart)
{

   return 0;
}
int CSerialProtocolK1S::ProtocolExtend(uint8_t *msg,uint32_t msglen)
{

    return 0;
}

int CSerialProtocolK1S::GetPersonDetectSta(void)
{
    return 0;
}

int CSerialProtocolK1S::GetMaskDetectSta(void)
{
    return 0;
}

int CSerialProtocolK1S::LookupCloudRemind(uint8_t openTyp,uint8_t userId)
{
    return 0;
}
int CSerialProtocolK1S::GetDeviceSN(void)
{
    return 0;
}

int CSerialProtocolK1S::SetFov(int fov)
{
    return  0;
}
//parse
bool CSerialProtocolK1S::CheckTag(const uint8_t *cmd,const uint32_t cmdlen)
{
    if(nullptr == cmd || cmdlen <= 1)
        return false;
    bool bCheck=false;
    const uint8_t *pTmp = cmd;
    int checkLen = 0;
    do{
        if(PEEPHOLE_K1S_TAG1 == *(pTmp) && PEEPHOLE_K1S_TAG2 == *(pTmp + 1))
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

int  CSerialProtocolK1S::ParseCmd(const uint8_t *cmd,const uint32_t cmdlen,bool bClearBuf)
{
    if(bClearBuf)
    {
        //printf("---------------clear buf \n");
        memset(&m_recvBuf,0,sizeof(m_recvBuf));
    }

    if (cmdlen <= 0)
    {
        return SerialCmdParseStaClientRun;
    }

    memcpy(m_recvBuf.data + m_recvBuf.dataLen,cmd,cmdlen);
    m_recvBuf.dataLen += cmdlen;

    //check cmd recv complete
    bool bCmdRecvFinish = false;
    if(m_encrypInfo.algTyp > 0){
        bCmdRecvFinish=CheckEncryptRecvFinish();
    }
    else{
        bCmdRecvFinish=CheckRecvFinish();
    }

    if(!bCmdRecvFinish)
    {
        QString note;
        note.sprintf("***recv len=%d,cmdLen=%d\n",m_recvBuf.dataLen,m_recvBuf.cmdlen);
        //emit CLogcat::Instance()->ShowNoteInfo(note,0,true);
        return SerialCmdParseStaClientRun;
    }

    CLogcat::Instance()->PrintHex(m_recvBuf.data, m_recvBuf.dataLen,"k1s finish:");
    if(m_encrypInfo.algTyp > 0){
        ProcessEncryptRecvData();
    }else{
        ProcessRecvData();
    }
    //clear recv buff
    memset(&m_recvBuf,0,sizeof(m_recvBuf));
    return SerialCmdParseStaEnd;
}

int CSerialProtocolK1S::TriggerVideoCall(const MediaCapEventParam &param)
{
    uint32_t duration= param.actionParam;
    uint8_t msg[128]={0};
    uint32_t msglen=0;
    memset(msg,0,sizeof(msg));
    msg[0]=(duration&0xff000000)>>24;
    msg[1]=(duration&0x00ff0000)>>16;
    msg[2]=(duration&0x0000ff00)>>8;
    msg[3]=(duration&0xff);
    msglen += 4;
    if(param.action == event_alarm_action_typ_snap)
        msg[4] =1;
    else
        msg[4] =0;
    msglen +=1;
    msglen +=3; //reserved

    FillMsgData(K1S_MID_TRIGGER_VIDEOCALL,msg,msglen,(m_encrypInfo.algTyp > 0));
    return 0;
}

int CSerialProtocolK1S::ProcMidInit(uint8_t *data,uint32_t dataSz)
{
    if(nullptr == data)
        return -1;
    emit CLogcat::Instance()->ShowNoteInfo("device sn="+QString((char *)data),0,true);

    if(m_bMsgEncrypt)
    {
        //   EncKey();
        m_encrypInfo.algTyp = 1; //encrypt

        uint8_t orderNum[16]={6,18,7, 3, 13,13,23,4, 8,1,0, 25, 9,2, 2, 7};
        SetKey(m_seed, 4,orderNum,16);
    }
    return 1;
}

int CSerialProtocolK1S::ProcMidEncKey(uint8_t *data,uint32_t dataSz)
{
    if(nullptr == data)
        return -1;

    m_encrypInfo.algTyp = 1; //encrypt

    return 1;
}

int CSerialProtocolK1S::ProcMidTriggerCap(uint8_t *data,uint32_t dataSz)
{
    if(nullptr == data)
        return -1;



    return 1;
}

int CSerialProtocolK1S::ProcMidDelMediaFile(uint8_t *data,uint32_t dataSz)
{
    if(nullptr == data)
        return -1;

    return 1;
}

int CSerialProtocolK1S::ProcMidSetVolume(uint8_t *data,uint32_t dataSz)
{
    if(nullptr == data)
        return -1;

    return 1;
}

int CSerialProtocolK1S::ProcMidGetVersion(uint8_t *data,uint32_t dataSz)
{
    if(nullptr == data)
        return -1;
    emit CLogcat::Instance()->ShowNoteInfo("version="+QString((char *)data),0,true);
    return 1;
}
int CSerialProtocolK1S::ProcMidStartOta(uint8_t *data,uint32_t dataSz)
{
    if(nullptr == data)
        return -1;

    return 1;
}
int CSerialProtocolK1S::ProcMidStopOta(uint8_t *data,uint32_t dataSz)
{
    if(nullptr == data)
        return -1;

    return 1;
}
int CSerialProtocolK1S::ProcMidSetBaudrate(uint8_t *data,uint32_t dataSz)
{
    if(nullptr == data)
        return -1;

    return 1;
}
int CSerialProtocolK1S::ProcMidConfigWifi(uint8_t *data,uint32_t dataSz)
{
    if(nullptr == data)
        return -1;

    return 1;
}

int CSerialProtocolK1S::ProcMidWifiMac(uint8_t *data,uint32_t dataSz)
{
    if(nullptr == data)
        return -1;
    uint8_t *pTmp = data;
    QString str;
    str.sprintf("%02X:%02X:%02X:%02X:%02X:%02X",*(pTmp),*(pTmp+1),*(pTmp+2),*(pTmp+3),*(pTmp+4),*(pTmp+5));
    emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypMac,str);
    emit CLogcat::Instance()->ShowNoteInfo(str,0,true);

    return 1;
}

int CSerialProtocolK1S::ProcMidGetWifiSta(uint8_t *data,uint32_t dataSz)
{
    if(nullptr == data)
        return -1;
    unsigned char *pTmp = data;
    unsigned char sta =*pTmp++;
    if(1 == sta)
        emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypWifiSta,"connect");
    else
        emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypWifiSta,"not connect");

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

    return 1;
}
int CSerialProtocolK1S::ProcMidSetLinkKey(uint8_t *data,uint32_t dataSz)
{
    if(nullptr == data)
        return -1;


    return 1;
}

int CSerialProtocolK1S::ProcMidDataPassthough(uint8_t *data,uint32_t dataSz)
{
    if(nullptr == data)
        return -1;


    return 1;
}

int CSerialProtocolK1S::ProcMidTriggerVideoCall(uint8_t *data,uint32_t dataSz)
{
    if(nullptr == data)
        return -1;

    return 1;
}
int CSerialProtocolK1S::ProcMidGetDevicename(uint8_t *data,uint32_t dataSz)
{
    if(nullptr == data)
        return -1;
    emit CLogcat::Instance()->ShowNoteInfo("device name="+QString((char *)data),0,true);
    emit CLogcat::Instance()->ShowDevInfo(PeepHoleCmdTypDevName,QString((char *)data));
    return 1;
}

int CSerialProtocolK1S::ProcMidGetModuleSta(uint8_t *data,uint32_t dataSz)
{
    if(nullptr == data)
        return -1;

    return 1;
}
int CSerialProtocolK1S::ProcMidPowerdown(uint8_t *data,uint32_t dataSz)
{
    if(nullptr == data)
        return -1;

    return 1;
}

int CSerialProtocolK1S::ProcMidDebugMode(uint8_t *data,uint32_t dataSz)
{
    if(nullptr == data)
        return -1;

    return 1;
}

int CSerialProtocolK1S::ProcMidGetDebugInfo(uint8_t *data,uint32_t dataSz)
{
    if(nullptr == data)
        return -1;

    return 1;
}

int CSerialProtocolK1S::ProcMidUploadDebugInfo(uint8_t *data,uint32_t dataSz)
{
    if(nullptr == data)
        return -1;
    return 1;
}

int CSerialProtocolK1S::PorcMidHiJack(uint8_t *data,uint32_t dataSz)
{
    if(nullptr == data)
        return -1;

    return 1;
}
int CSerialProtocolK1S::ProcCmd(k1s_cmd_msg &cmd)
{
    switch(cmd.msgId)
    {
    case K1SMsgTypReply:
        ProcReply(cmd);
        break;
    case K1SMsgTypNote:
        ProcNote(cmd);
        break;
    case K1SMsgTypImg:
        break;
    }
    return 0;
}

int CSerialProtocolK1S::ProcNote(k1s_cmd_msg &cmd)
{
    uint8_t nid = cmd.data[0];
    switch(nid)
    {
    case K1S_NID_READY://orbbec code
        Init();
        break;
    case K1S_NID_FACE_STATE:
        break;
    case K1S_NID_UNKNOWNERROR:
        break;
    case K1S_NID_OTA_STATE:
        OtaSta(cmd);
        break;
    case K1S_NID_EYE_STATE:
        break;
    case K1S_NID_MEDIA_CAP_STATE:
        RecSta(cmd);
        break;
    case K1S_NID_MEDIA_STATE:
        MediaSta(cmd);
        break;
    case K1S_NID_MEDIA_PLAYBACK_STATE:
        break;
    default:
        break;
    }
    return 0;
}

int CSerialProtocolK1S::ProcReply(k1s_cmd_msg &cmd)
{
    uint8_t msgId=0;
    msgId = cmd.data[0];
    uint8_t *pData = cmd.data+2;
    uint32_t size = cmd.size-2;
    switch(msgId)
    {
    case K1S_MID_INIT:
    case K1S_MID_INIT_RESP:
         ProcMidInit(pData,size);
    break;
    case K1S_MID_ENCKEY:
        ProcMidEncKey(pData,size);
    break;
    case K1S_MID_TRIGGER_CAPTURE:
        ProcMidTriggerCap(pData,size);
    break;
    case K1S_MID_DELETE_MEDIA_FILE:
        ProcMidDelMediaFile(pData,size);
    break;
    case K1S_MID_SET_VOLUME:
        ProcMidSetVolume(pData,size);
    break;
    case K1S_MID_GET_VERSION:
        ProcMidGetVersion(pData,size);
    break;
    case K1S_MID_START_OTA:
        ProcMidStartOta(pData,size);
    break;
    case K1S_MID_STOP_OTA:
        ProcMidStopOta(pData,size);
    break;
    case K1S_MID_CONFIG_BAUDRATE:
        ProcMidSetBaudrate(pData,size);
    break;
    case K1S_MID_CONFIG_WIFI:
        ProcMidConfigWifi(pData,size);
    break;
    case K1S_MID_GET_WIFIMAC:
        ProcMidWifiMac(pData,size);
    break;
    case K1S_MID_GET_WIFISTATUS:
        ProcMidGetWifiSta(pData,size);
    break;
    case K1S_MID_SET_LINKKEY:
        ProcMidSetLinkKey(pData,size);
    break;
    case K1S_MID_DATA_PASSTHROUGH:
        ProcMidDataPassthough(pData,size);
    break;
    case K1S_MID_TRIGGER_VIDEOCALL:
        ProcMidTriggerVideoCall(pData,size);
    break;
    case K1S_MID_GET_DEVICENAME:
        ProcMidGetDevicename(pData,size);
    break;
    case K1S_MID_GET_MODULE_STATUS:
        ProcMidGetModuleSta(pData,size);
    break;
    case K1S_MID_POWERDOWN:
        ProcMidPowerdown(pData,size);
    break;
    case K1S_MID_DEBUG_MODE:
        ProcMidDebugMode(pData,size);
    break;
    case K1S_MID_GET_DEBUG_INFO:
        ProcMidGetDebugInfo(pData,size);
    break;
    case K1S_MID_UPLOAD_DEBUG_INFO:
        ProcMidUploadDebugInfo(pData,size);
    break;
    case K1S_MID_HIJACK:
        PorcMidHiJack(pData,size);
    break;
    default:
    break;
    }
    return 0;
}
int CSerialProtocolK1S::OtaSta(k1s_cmd_msg &cmd)
{
    uint8_t otaSta;
    uint8_t otaErr;
    uint8_t otaPro;
    otaSta = cmd.data[1];
    otaErr = cmd.data[2];
    otaPro = cmd.data[3];
    QString note;
    note.sprintf("ota sta:sta=%d,err=%d",otaSta,otaErr,otaPro);
    emit CLogcat::Instance()->ShowNoteInfo(note,1,true);
    return 0;
}

int CSerialProtocolK1S::RecSta(k1s_cmd_msg &cmd)
{
    uint8_t recFinish = 0;
    uint32_t recTime=0;
    uint8_t recTyp=0;
    recTyp = cmd.data[1];
    recTime = (cmd.data[2]<<24) | (cmd.data[3]<<16) | (cmd.data[4]<<8) |(cmd.data[5]);
    recFinish = cmd.data[6];
    QString note;
    note.sprintf("recSta:typ=%d,finish=%d,time=%d",recTyp,recFinish,recTime);
    emit CLogcat::Instance()->ShowNoteInfo(note,1,true);
    return 0;
}

int CSerialProtocolK1S::MediaSta(k1s_cmd_msg &cmd)
{
    uint8_t mediaSta;
    uint8_t mediaErr;
    mediaSta = cmd.data[1];
    mediaErr = cmd.data[2];
    QString note;
    note.sprintf("media sta:sta=%d,err=%d",mediaSta,mediaErr);
    emit CLogcat::Instance()->ShowNoteInfo(note,1,true);
    return 0;
}

bool CSerialProtocolK1S::CheckMsgId(uint32_t mid)
{
    return true;
}

bool CSerialProtocolK1S::CheckEncryptRecvFinish(void)
{
    bool bFinish = true;
    if(m_recvBuf.dataLen < 4)
       return false;

    uint8_t *pHead = nullptr;
    m_recvBuf.cmdlen=0;
    //check cmd recv complete
    do{
       pHead = m_recvBuf.data + m_recvBuf.cmdlen;
       printf("K1S encrypt m_recvData.dataLen=%d,cmdlen=%d\n", m_recvBuf.dataLen,m_recvBuf.cmdlen);
       if((*pHead == PEEPHOLE_K1S_TAG1) && (*(pHead + 1) == PEEPHOLE_K1S_TAG2))
       {
            if(m_recvBuf.dataLen < m_recvBuf.cmdlen + 4)
            {
                //recv buf is include length
                bFinish = false;
                break;
            }
            uint8_t lenH = *(pHead+2);
            uint8_t lenL = *(pHead+3);
            m_recvBuf.cmdlen += (lenH<<8)|lenL;
            m_recvBuf.cmdlen += 4;
            printf("Cmdlen =%d,cmdlen=%d\n", m_recvBuf.cmdlen,(lenH<<8)|lenL);
            if(m_recvBuf.dataLen > m_recvBuf.cmdlen)
               m_recvBuf.cmdlen += 1;//parity
            else
            {
                bFinish = false;
                break;
            }
        }
        else
        {
            bFinish = false;
            //m_recvBuf.cmdlen++;
            break;
        }
    }while(m_recvBuf.dataLen > m_recvBuf.cmdlen);

    return bFinish;
}

bool CSerialProtocolK1S::CheckRecvFinish(void)
{
    bool bFinish = true;
    if(m_recvBuf.dataLen < 5)
       return false;

    uint8_t *pHead = nullptr;
    m_recvBuf.cmdlen=0;
    //check cmd recv complete
    do{
       pHead = m_recvBuf.data + m_recvBuf.cmdlen;
       QString note;
       note.sprintf("K1S m_recvData.dataLen=%d,cmdlen=%d\n", m_recvBuf.dataLen,m_recvBuf.cmdlen);
       emit CLogcat::Instance()->ShowNoteInfo(note,0,true);
       if((*pHead == PEEPHOLE_K1S_TAG1) && (*(pHead + 1) == PEEPHOLE_K1S_TAG2))
       {
            if(m_recvBuf.dataLen < m_recvBuf.cmdlen + 5)
            {
                //recv buf is include length
                bFinish = false;
                break;
            }
            uint8_t msgId = *(pHead+2);
            uint8_t lenH = *(pHead+3);
            uint8_t lenL = *(pHead+4);
            m_recvBuf.cmdlen += (lenH<<8)|lenL;
            m_recvBuf.cmdlen += 5;
            printf("K1S Cmdlen =%d,cmdid=%d\n", m_recvBuf.cmdlen,msgId);
            if(m_recvBuf.dataLen > m_recvBuf.cmdlen)
               m_recvBuf.cmdlen += 1;//parity
            else
            {
                bFinish = false;
                break;
            }
        }
        else
        {
            bFinish = false;
            //m_recvBuf.cmdlen++;
            break;
        }
    }while(m_recvBuf.dataLen > m_recvBuf.cmdlen);

    return bFinish;
}

int CSerialProtocolK1S::ProcessRecvData(void)
{
    k1s_cmd_msg   cmdData;
    uint32_t parseLen=0;
    uint8_t *pCmdTmp = nullptr;
    while(parseLen < m_recvBuf.dataLen)
    {
        memset(&cmdData,0,sizeof(cmdData));

        //parse data
        pCmdTmp = (uint8_t *)(m_recvBuf.data + parseLen);
        cmdData.tag1 = *pCmdTmp++;
        cmdData.tag2 = *pCmdTmp++;
        if ((cmdData.tag1 != PEEPHOLE_K1S_TAG1) && (cmdData.tag1 != PEEPHOLE_K1S_TAG2))
        {
            CLogcat::Instance()->ShowNoteInfo("flag is not PEEP_HOLE_K1S_TAG !",0,true);
            //return SerialCmdParseStaClientRun;
            parseLen++;
        }
        else
        {
            uint8_t szH = 0,szL=0;
            cmdData.msgId = *pCmdTmp++;
            szH = *pCmdTmp++;
            szL = *pCmdTmp++;
            cmdData.size = (szH<<8)|szL;//cmd len

            if(CheckMsgId(cmdData.msgId))
            {
                unsigned char checkCrc = XOR_Inverted_Check(m_recvBuf.data+parseLen+2, cmdData.size + 3);
                cmdData.parity = m_recvBuf.data[cmdData.size + 5 + parseLen];//parity

                if (checkCrc != cmdData.parity)
                {
                    QString note;
                    note.sprintf("parity error!datalen=%d,cmdParity=%02x,crc=%02x", cmdData.size,cmdData.parity, checkCrc);
                    CLogcat::Instance()->ShowNoteInfo(note,0,true);
                }
                else
                {
                    memcpy(cmdData.data, pCmdTmp, sizeof(uint8_t) * cmdData.size);
                    //process cmd data
                    ProcCmd(cmdData);
                }
            }
            else
            {
                CLogcat::Instance()->ShowNoteInfo("****cmd id error!id="+QString::number(cmdData.msgId),0,true);
            }

            parseLen += 5;  //head
            parseLen += cmdData.size; //data size
            parseLen += 1;//parity
        }
    }

    return 1;
}

int CSerialProtocolK1S::ProcessEncryptRecvData(void)
{
    k1s_cmd_msg   cmdData;
    uint32_t parseLen=0;
    uint8_t *pCmdTmp = nullptr;
    //emit CLogcat::Instance()->ShowNoteInfo(">>>ProcessEncryptRecvData",1,true);
    while(parseLen < m_recvBuf.dataLen)
    {
        memset(&cmdData,0,sizeof(cmdData));

        //parse data
        pCmdTmp = (uint8_t *)(m_recvBuf.data + parseLen);
        cmdData.tag1 = *pCmdTmp++;
        cmdData.tag2 = *pCmdTmp++;
        if ((cmdData.tag1 != PEEPHOLE_K1S_TAG1) && (cmdData.tag1 != PEEPHOLE_K1S_TAG2))
        {
            CLogcat::Instance()->ShowNoteInfo("flag is not PEEP_HOLE_K1S_TAG !",0,true);
            parseLen++;
        }
        else
        {
            uint8_t szH = 0,szL=0;
            szH = *pCmdTmp++;
            szL = *pCmdTmp++;
            uint32_t encLen = (szH<<8)|szL;

            unsigned char checkCrc = XOR_Inverted_Check(m_recvBuf.data+parseLen+4, encLen);
            cmdData.parity = m_recvBuf.data[encLen + 4 + parseLen];//parity
            if (checkCrc != cmdData.parity)
            {
                QString note;
                note.sprintf("encrypt parity error!datalen=%d,cmdParity=%02x,crc=%02x\n",encLen,cmdData.parity, checkCrc);
                emit CLogcat::Instance()->ShowNoteInfo(note,0,true);
            }
            else
            {
                uint8_t decDst[1024]={0};
                CSecure secure;
                int res =secure.AesEcbDecrypt(m_encrypInfo.key,m_encrypInfo.keylen,pCmdTmp,encLen,decDst,encLen);
                if(res < 0)
                {
                    emit CLogcat::Instance()->ShowNoteInfo("Decrypt Aes failed!",0,true);
                    parseLen += 4;  //head
                    parseLen += encLen; //encrypt size
                    parseLen += 1;//parity
                    continue;
                }
                emit CLogcat::Instance()->PrintHex(decDst,32,"decDst:");
                uint8_t *pTmp = decDst;

                cmdData.msgId = *pTmp++;
                szH = *pTmp++;
                szL = *pTmp++;
                cmdData.size = (szH<<8)|szL;//cmd len
                if(CheckMsgId(cmdData.msgId))
                {
                    memcpy(cmdData.data, pTmp, sizeof(uint8_t) * cmdData.size);
                    //process cmd data
                    ProcCmd(cmdData);
                }
                else
                {
                    emit CLogcat::Instance()->ShowNoteInfo("****cmd id error!id="+QString::number(cmdData.msgId),0,true);
                }
            }

            parseLen += 4;  //head
            parseLen += encLen; //encrypt size
            parseLen += 1;//parity
        }
    }
    return 1;
}

int CSerialProtocolK1S::Random(int min,int max)
{
    int temp = 0;
    if (min > max)
    {
        temp = max;
        max = min;
        min = temp;
    }
    return rand() / (double)RAND_MAX *(max - min) + min;
}

int CSerialProtocolK1S::SetKey(uint8_t *seed, int size,uint8_t *order,uint8_t orderlen)
{
    // the input seed must be 4 bytes
    if (size != 4) {
        emit CLogcat::Instance()->ShowNoteInfo("Incorrect input seed size",0,true);
        return -1;
    }

    QString noteStr;
    noteStr.sprintf("seed:%d,%d,%d,%d",seed[0],seed[1],seed[2],seed[3]);
    emit CLogcat::Instance()->ShowNoteInfo(noteStr,0,true);

    uint8_t md5[32];
    memset(md5,0,sizeof(md5));

    CSecure seInfo;
    seInfo.GenMd5(seed,size,md5);
    emit CLogcat::Instance()->OutSendCmd("MD5:",md5,32);

    uint8_t hexascii[32];
    memset(hexascii,0,sizeof(hexascii));

    Bin2HexAscii(md5,16,hexascii,32,false);
    memset(m_encrypInfo.key,0,sizeof(m_encrypInfo.key));
    for(int i=0;i<16;i++)
        m_encrypInfo.key[i] = hexascii[order[i]];
    m_encrypInfo.keylen = 16;
    emit CLogcat::Instance()->OutSendCmd("KEY:",m_encrypInfo.key,16);
    return 1;
}

int CSerialProtocolK1S::FillMsgData(int typ,const uint8_t *data,int datalen,bool bEncrypt)
{
    //
    SerialProtocolCmdData sendMsg;
    memset(&sendMsg,0,sizeof(sendMsg));

    if(bEncrypt)
    {
        //emit CLogcat::Instance()->ShowNoteInfo(">>>encrypt>>>",1,true);
        FillEncryptMsg(typ,data,datalen,sendMsg);
    }
    else
    {
        FillNormalMsg(typ,data,datalen,sendMsg);
        emit CLogcat::Instance()->PrintHex(sendMsg.data,sendMsg.dataLen);
    }
    SendMsg(sendMsg);
    return 0;
}

int CSerialProtocolK1S::FillNormalMsg(uint8_t typ,const uint8_t *data,uint32_t datalen,SerialProtocolCmdData &sendData)
{
    sendData.data[0] = PEEPHOLE_K1S_TAG1;
    sendData.data[1] = PEEPHOLE_K1S_TAG2;
    sendData.dataLen = 2;
    //emit CLogcat::Instance()->PrintHex(sendData.data,sendData.dataLen);
    sendData.data[2] = typ;
    sendData.dataLen += 1;
    sendData.data[3] = (datalen&0xff00)>>8;
    sendData.data[4] = (datalen) & 0xff;

    sendData.dataLen += 2;
    //emit CLogcat::Instance()->PrintHex(sendData.data,sendData.dataLen);
    if(datalen > 0)
    {
        memcpy(sendData.data + 5,data,datalen*sizeof(uint8_t));
        sendData.dataLen += datalen;
    }
    sendData.data[sendData.dataLen]=XOR_Inverted_Check(sendData.data+2,sendData.dataLen-2);
    sendData.dataLen += 1;
    //emit CLogcat::Instance()->PrintHex(sendData.data,sendData.dataLen);
    if(0)//typ == K1S_MID_SET_LINKKEY)
    {
        //emit CLogcat::Instance()->ShowNoteInfo("datalen="+QString::number(sendData.dataLen),1,true);
        memcpy(sendData.data + sendData.dataLen,sendData.data,sendData.dataLen);
        sendData.dataLen += sendData.dataLen;
    }
    return 0;
}

int CSerialProtocolK1S::FillEncryptMsg(uint8_t typ,const uint8_t *data,uint32_t datalen,SerialProtocolCmdData &sendData)
{
    sendData.data[0] = PEEPHOLE_K1S_TAG1;
    sendData.data[1] = PEEPHOLE_K1S_TAG2;
    sendData.dataLen = 2;

    uint8_t encDst[512]={0};
    uint8_t encSrc[256]={0};
    uint32_t encSrcLen=0;
    encSrc[0]= typ;
    encSrcLen=1;
    encSrc[1] = (datalen&0xff00)>>8;
    encSrc[2] = (datalen & 0xff);
    encSrcLen += 2;
    if(datalen > 0)
    {
        memcpy(encSrc+encSrcLen,data,datalen);
        encSrcLen += datalen;
    }
    emit CLogcat::Instance()->PrintHex(encSrc,encSrcLen,"encSrc:");
    //encrypt
    CSecure infoSe;
    int len =infoSe.AesEcbEncrypt(m_encrypInfo.key, m_encrypInfo.keylen, encSrc,encSrcLen, encDst, 512);
    if(len >= 0)
    {
        emit CLogcat::Instance()->PrintHex(encDst,len);
        sendData.data[2] = (len&0xff00)>>8;
        sendData.data[3] = (len & 0xff);
        sendData.dataLen += 2;
        memcpy(sendData.data + 4,encDst,len);
        sendData.dataLen += len;
        sendData.data[sendData.dataLen] = XOR_Inverted_Check(encDst,len);
        sendData.dataLen += 1;

        if(typ == K1S_MID_SET_LINKKEY)
        {
            //emit CLogcat::Instance()->ShowNoteInfo("datalen="+QString::number(sendData.dataLen),1,true);
            memcpy(sendData.data + sendData.dataLen,sendData.data,sendData.dataLen);
            sendData.dataLen += sendData.dataLen;
        }
        emit CLogcat::Instance()->PrintHex(sendData.data,sendData.dataLen,"before:");
    }
    else
    {
        sendData.dataLen += encSrcLen;
        sendData.dataLen += 1;
    }

    return 0;
}

int CSerialProtocolK1S::SendMsg(const SerialProtocolCmdData &sendMsg)
{
    if(sendMsg.dataLen <= 0)
    {
        return -1;
    }
    //hex_print(sendMsg.data,sendMsg.dataLen,32,(char *)"K1S Sendcmd:");
    if(m_sendFunc != nullptr)
        m_sendFunc(m_pParam,sendMsg.data,sendMsg.dataLen);
    return 1;
}

uint8_t CSerialProtocolK1S::XOR_Inverted_Check(uint8_t *inBuf,int inLen)
{
    uint8_t check = 0;
    int i=0;

    for(i = 0;i < inLen;i++)
        check ^= inBuf[i];
    //return ~check;
    return check;
}
