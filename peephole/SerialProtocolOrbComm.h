#ifndef _SERIAL_PROTOCOL_ORB_COMM_H_
#define _SERIAL_PROTOCOL_ORB_COMM_H_

#include<stdint.h>
#include<string>
#include "DataBuffer.h"
#include "DataTyp.h"
#include "Secure.h"
#include "OrbCmdPktComm.h"
#include "OrbCmdPktAes.h"
#include<thread>
#include<chrono>

class CSerialProtocolOrbComm
{
public:
    CSerialProtocolOrbComm();
    ~CSerialProtocolOrbComm();

    void SetEncrypt(bool bEncrypt);
    void SetSendFun(std::function<int(void *param,const uint8_t*,uint32_t)> func,void *param)
    {
        m_sendFunc = func;
        m_pParam = param;
    }

    int Init(CDataBuffer& resp);
    int EncOrder(CDataBuffer& resp);
    int TriggerCap(const uint8_t mediaTyp,const uint32_t capTime,const bool bCloud,const uint8_t capTyp,CDataBuffer& resp);
    int SetVolume(const uint8_t volume,CDataBuffer& resp);
    int GetVersion(CDataBuffer& resp);
    int ConfigWifi(const std::string ssid,const std::string pwd,const std::string reserved,bool bFac,CDataBuffer& resp);
    int GetWifiMac(CDataBuffer& resp);
    int GetWifiSta(CDataBuffer& resp);
    int SetCloudKey(const std::string productKey,const std::string productSecret,const std::string deviceName,const std::string deviceSecret,CDataBuffer& resp);
    int GetCloudKey(CDataBuffer& resp);
    int GetDeviceName(CDataBuffer& resp);
    int PeepholeSleep(const int liveTyp,CDataBuffer& resp);
    int GetIpInfo(CDataBuffer& resp);
    int PicDirCtl(const int dir,CDataBuffer& resp);
    int PirCtl(const bool bOpen,CDataBuffer& resp);
    int EventProc(const uint32_t eventid,const MediaCapEventParam &param,CDataBuffer& resp);
    int Ota(bool start,CDataBuffer& resp);
    int DataPass2Server(const uint8_t *data,uint16_t datalen,CDataBuffer& resp);
    int GetNetTime(CDataBuffer& resp);
    int SetWifiKeepLive(uint8_t typ,bool bFace,CDataBuffer& resp);
    int Reset(CDataBuffer& resp);
    int GetDevState(CDataBuffer& resp);
    int ScanConf(bool bStart,CDataBuffer& resp);
    int ProtocolExtend(const uint8_t *data,const uint32_t datalen,CDataBuffer& resp);
    int GetPersonDetectSta(CDataBuffer& resp);
    int GetMaskDetectSta(CDataBuffer& resp);
    int LookupCloudRemind(const uint8_t openTyp,const uint8_t userId,CDataBuffer& resp);
    int CmdProc(OrbProtocolCmd &orbCmd,CDataBuffer& resp);
    int SetUserData(uint8_t eventTyp,uint8_t userTyp,uint8_t authTyp,uint8_t mainId,uint8_t subId,CDataBuffer& resp);
protected:
    void ReadConfig();
    int SetKey(uint8_t *seed, int size,uint8_t *order,uint8_t orderlen);
    int FillMsg(const uint8_t* msg,const uint32_t msglen,bool bEncrypt,CDataBuffer& resp);
    int ProcInit(OrbProtocolCmd& orbCmd);
    int ProcScanConf(OrbProtocolCmd& orbCmd);
    int ProcVersion(OrbProtocolCmd& orbCmd);
    int ProcReset(OrbProtocolCmd& orbCmd);
    int ProcPowerOff(OrbProtocolCmd& orbCmd);
    int ProcPicDir(OrbProtocolCmd& orbCmd);
    int ProcOta(OrbProtocolCmd& orbCmd);
    int ProcGetDevSta(OrbProtocolCmd& orbCmd);
    int ProcSsidPwd(OrbProtocolCmd& orbCmd);
    int ProcMacAddr(OrbProtocolCmd& orbcmd);
    int ProcDataPass(OrbProtocolCmd& orbCmd);
    int ProcIpAddr(OrbProtocolCmd& orbCmd);
    int ProcDevName(OrbProtocolCmd& orbCmd);
    int ProcLinkKey(OrbProtocolCmd& orbCmd);
    int ProcEvent(OrbProtocolCmd& orbCmd);
    int ProcNetSta(OrbProtocolCmd& orbCmd);
    int ProNetTime(OrbProtocolCmd& orbCmd);
    int ProcGetCloudkey(OrbProtocolCmd& orbCmd);
    int ProcOtaSta(OrbProtocolCmd& orbCmd);
    int ProcRecSta(OrbProtocolCmd& orbCmd);
    int ProcDevSta(OrbProtocolCmd& orbCmd);
    int ProcSleepNote(OrbProtocolCmd& orbCmd);
    int ProcNetTimeNote(OrbProtocolCmd& orbCmd);
    int ProcWakeUpNote(OrbProtocolCmd& orbCmd);
    int ProcWorkMode(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int ProcAlarmCancel(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int ProcChildLockCtl(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int ProcTamperCtl(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int ProcHumanDistSet(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int ProcVolumeSet(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int ProcLockStaGet(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int ProcLockVerGet(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int ProcUserAdd(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int ProcUserDel(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int ProcLockPwdSet(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int ProcBatteryLevelGet(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int ProcRemotePwdUnlock(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int HumanIntervalSet(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int OutAntiOpenInterSet(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int AppAutoLockSet(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int AppDataPass(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int AppLockSet(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int AppLockResp(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int AppMotorTorque(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int AppBoltRebound(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int AppBoltBackTime(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int AppLockOpenOrientation(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int AppLockSnap(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int AppRemotePwdAuth(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int AppKeepLive(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    //int GetPersonSta(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    //int GetMaskSta(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    //int LookRemind(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    //mcu OTA
    static void  McuOtaRun(void *arg);
    void McuOtaProgress(void);
    int McuOta(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int McuOtaInfo(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int McuOtaPkt(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int McuOtaSta(OrbProtocolCmd& orbCmd,CDataBuffer& resp);
    int AddMcuOtaPkt(const uint16_t pktId,const uint16_t datalen,const uint8_t *pData);

     int SendMsg(const uint8_t *msg,const uint32_t msglen);
private:
    bool m_bCmdEncrypt; //begin encrypt
    bool m_bCmdEncSet; //cmd encrypt

    uint8_t m_keyOrder[16];
    uint8_t m_aesKey[16];
    uint8_t m_aesKeyLen;
    uint8_t m_seed[4];

    COrbCmdPktComm m_cmdPkt;
    COrbCmdPktAes  m_cmdPktAes;
    int m_timeZone;
    //ota
    uint8_t m_sleepTime;
    uint16_t m_otaPktlen;
    uint32_t m_fwlen;
    uint16_t m_checkSum;
    uint16_t m_pktNum;
    uint8_t  *m_pFw;
    char m_mcuVer[32];
    std::thread m_mcuOtaThead;

    //send
    std::function<int(void *param,const uint8_t*,uint32_t)> m_sendFunc;
    void *m_pParam;
};


#endif
