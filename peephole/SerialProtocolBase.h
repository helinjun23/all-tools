#ifndef _SERIAL_PROTOCOL_BASE_H_
#define _SERIAL_PROTOCOL_BASE_H_
#include<stdlib.h>
#include<string>
#include<stdio.h>
#include<stdint.h>
#include <functional>
#include "DataTyp.h"
#include "OrbDataTyp.h"
#include "clogcat.h"

class CSerialProtocolBase
{
public:
    CSerialProtocolBase() {m_bMsgEncrypt=false;m_sendFunc=nullptr;m_pParam=nullptr;}
    virtual ~CSerialProtocolBase() {}

    virtual int Init(void)=0;
    virtual int EncKey(void)=0;
    virtual int TriggerCap(uint8_t mediaTyp,uint32_t capTime,bool bCloud,uint8_t capTyp)=0;
    virtual int SetVolume(uint8_t volume)=0;
    virtual int GetVersion(void)=0;
    virtual int ConfigWifi(std::string ssid,std::string pwd,std::string reserved,bool bFac)=0;
    virtual int GetWifiMac(void)=0;
    virtual int GetWifiSta(void)=0;
    virtual int SetCloudKey(std::string productKey,std::string productSecret,std::string deviceName,std::string deviceSecret)=0;
    virtual int GetDeviceName(void)=0;
    virtual int PeepholeSleep(int liveTyp)=0;
    virtual int GetIpInfo(void)=0;
    virtual int PicDirCtl(int dir)=0;
    virtual int PirCtl(bool bOpen)=0;
    virtual int EventProc(uint32_t eventid,MediaCapEventParam &param)=0;
    virtual int Ota(bool bStart)=0;
    virtual int DataPassToServer(uint8_t *data,uint16_t datalen)=0;
    virtual int GetNetTime(void)=0;
    virtual int SetWifiKeepLive(uint8_t typ,bool bFace)=0;
    virtual int Reset(void)=0;
    virtual int GetDevState(void)=0;
    virtual int ScanConf(bool bStart)=0;
    virtual int ProtocolExtend(uint8_t *msg,uint32_t msglen)=0;
    virtual int GetPersonDetectSta(void)=0;
    virtual int GetMaskDetectSta(void)=0;
    virtual int LookupCloudRemind(uint8_t openTyp,uint8_t userId)=0;
    virtual int GetDeviceSN(void)=0;
    virtual int SetFov(int fov)=0;
    virtual int GetCloudKey(void) {return 0;}
    virtual int SetUserData(uint8_t eventTyp,uint8_t userTyp,uint8_t authTyp,uint8_t mainId,uint8_t subId){return 0;}
    virtual bool CheckTag(const uint8_t *cmd,const uint32_t cmdlen)=0;
    virtual int  ParseCmd(const uint8_t *cmd,const uint32_t cmdlen,bool bClearBuf)=0;
    virtual void SetMsgEncrypt(bool bEncrypt) {m_bMsgEncrypt = bEncrypt;}
    virtual void SetSendFunc(std::function<int(void *param,const uint8_t*,uint32_t)> func,void *param)
    {

        m_sendFunc = func;
        m_pParam = param;
    }

    int SendData(const uint8_t *data,const uint32_t datalen)
    {
        if(m_sendFunc != nullptr)
        {
            CLogcat::Instance()->OutSendCmd("Send:",(unsigned char *)data,datalen);
            m_sendFunc(m_pParam,data,datalen);
        }
        else
        {
            emit CLogcat::Instance()->ShowNoteInfo("send func is null",1,true);
        }
        return 0;
    }
protected:
    bool m_bMsgEncrypt;
    std::function<int(void *param,const uint8_t*,uint32_t)> m_sendFunc;
    void *m_pParam;
};


#endif
