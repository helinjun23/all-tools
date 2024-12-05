#ifndef _SERIAL_PROTOCOL_3V_H_
#define _SERIAL_PROTOCOL_3V_H_

#include "SerialProtocolBase.h"
#include "PeepholeSerial3vDataTyp.h"
#include "OrbDataTyp.h"

class CSerialProtocol3V:public CSerialProtocolBase
{
public:
    CSerialProtocol3V();
    ~CSerialProtocol3V();
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
    //parse
    bool CheckTag(const uint8_t *cmd,const uint32_t cmdlen);
    int  ParseCmd(const uint8_t *cmd,const uint32_t cmdlen,bool bClearBuf);
protected:
    uint8_t GetAlarm(uint8_t alarmtyp);
    uint8_t GetEvent(uint8_t eventtyp);
    void DevNameRespProc(unsigned char *data,unsigned char datalen);
    void MacRespProc(unsigned char *data,unsigned char datalen);
    void IpRespProc(uint8_t *data,unsigned char datalen);
    void WifiStaRespProc(unsigned char *data,unsigned char datalen);
    void NetTimeProc(unsigned char *data,unsigned char datalen);
    void VerProc(unsigned char *data,unsigned char datalen);
    void DeviceSnProc(unsigned char *data,unsigned char datalen);
    void CmdProc(uint8_t cmd,unsigned char *data,unsigned char datalen);
    int SendCmd(const uint8_t *cmd,const uint32_t cmdlen);
    uint8_t XOR_Inverted_Check(unsigned char *inBuf,unsigned char inLen);
    //
    uint8_t GetOpenTyp(uint8_t typ);
private:
};


#endif
