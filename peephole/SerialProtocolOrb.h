#ifndef _SERIAL_PROTOCOL_ORB_H_
#define _SERIAL_PROTOCOL_ORB_H_

#include "SerialProtocolBase.h"
#include "SerialProtocolOrbFac.h"
#include "SerialProtocolOrbComm.h"

class CSerialProtocolOrb:public CSerialProtocolBase
{
public:
    CSerialProtocolOrb();
    ~CSerialProtocolOrb();
    void SetSendFunc(std::function<int(void *param,const uint8_t*,uint32_t)> func,void *param) override;
    int Init(void);
    int EncKey(void);
    int TriggerCap(uint8_t mediaTyp,uint32_t capTime,bool bCloud,uint8_t capTyp);
    int SetVolume(uint8_t volume);
    int GetVersion(void);
    int ConfigWifi(std::string ssid,std::string pwd,std::string reserved,bool bFac);
    int GetWifiMac(void);
    int GetWifiSta(void);
    int SetCloudKey(std::string productKey,std::string productSecret,std::string deviceName,std::string deviceSecret);
    int GetDeviceName(void);
    int PeepholeSleep(int liveTyp);
    int GetIpInfo(void);
    int PicDirCtl(int dir);
    int PirCtl(bool bOpen);
    int EventProc(uint32_t eventid,MediaCapEventParam &param);
    int Ota(bool bStart);
    int DataPassToServer(uint8_t *data,uint16_t datalen);
    int GetNetTime(void);
    int SetWifiKeepLive(uint8_t typ,bool bFace);
    int Reset(void);
    int GetDevState(void);
    int ScanConf(bool bStart);
    int ProtocolExtend(uint8_t *msg,uint32_t msglen);
    int GetPersonDetectSta(void);
    int GetMaskDetectSta(void);
    int LookupCloudRemind(uint8_t openTyp,uint8_t userId);
    int GetDeviceSN(void);
    int SetFov(int fov);
    int GetCloudKey(void) override;
    int SetUserData(uint8_t eventTyp,uint8_t userTyp,uint8_t authTyp,uint8_t mainId,uint8_t subId) override;
    //parse
    bool CheckTag(const uint8_t *cmd,const uint32_t cmdlen);
    int  ParseCmd(const uint8_t *cmd,const uint32_t cmdlen,bool bClearBuf);
protected:
    int SendMsg(const uint8_t *msg,const uint32_t msglen);
private:
    SerialProtocolCmdData m_recvData;
    CSerialProcotolOrbFac m_orbFac;
    CSerialProtocolOrbComm m_orbComm;
};


#endif
