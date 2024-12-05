#ifndef _SERIAL_PROTOCOL_ORB_FAC_H_
#define _SERIAL_PROTOCOL_ORB_FAC_H_
#include <functional>
#include "SerialOrbCmdPkt.h"
#include "OrbProtocolDataTyp.h"
#include "clogcat.h"

class CSerialProcotolOrbFac{
public:
    CSerialProcotolOrbFac();
    ~CSerialProcotolOrbFac();
    //cmd
    int GetSysVer(void);
    int GetWifiVer(void);
    int GetSN(void);
    int SetSN(uint8_t *sn,uint32_t snlen);
    int Reset(void);
    int GetPirSta(void);
    int SetPirSta(bool bOpen);
    int GetPirThresh(void);
    int SetPirThresh(uint8_t thresh);
    int GetPirTime(void);
    int SetPirTime(uint8_t time);
    int GetWifiSta(void);
    int SetWifiSta(bool bCtl);
    int GetWifiAp(void);
    int SetWifiAp(uint8_t ap);
    int AudioRec(bool bRec);
    int AudioRecPly(uint8_t plyTyp);
    int StartMedia(uint8_t protocolTyp,uint8_t streamTyp,uint8_t netTyp);
    int StopMedia(void);
    int GetMediaUrl(void);
    int GetMediaCurFrm(uint8_t streamTyp);
    int GetIrcutSta(void);
    int SetIrcutSta(uint8_t sta);
    int GetLightSensitive(void);
    int LightSensitiveCtl(bool bOpen);
    int FactoryMode(bool bOpen);
    int GetSDSize(void);
    int LedCtl(bool bOpen);
    int GetSensorSta(void);
    int SetWireIP(uint8_t *ipv4,uint8_t *gateway,uint8_t *mask);
    int GetCloudKey(void);
    //parse
    int OrbFacParseData(OrbProtocolCmd &orbCmd);
    void SetSendFunc(std::function<int(void *param,const uint8_t*,uint32_t)> func,void *param) {
        m_sendFunc = func;
        m_pParam = param;
    }

    int SendCmd(const uint8_t *cmd,const uint32_t cmdlen)
    {

        if(m_sendFunc != nullptr)
            m_sendFunc(m_pParam,cmd,cmdlen);
        else
            emit CLogcat::Instance()->ShowNoteInfo("send func is null",1,true);
        return 0;
    }
protected:
    //orbbec
    void OrbSnProc(OrbProtocolCmd &orbCmd);
    void OrbVerProc(OrbProtocolCmd &orbCmd);
    void PirDistProc(OrbProtocolCmd &orbCmd);
    void PirStaProc(OrbProtocolCmd &orbCmd);
    void PirTimeProc(OrbProtocolCmd &orbCmd);
    void MediaStreamUrlProc(OrbProtocolCmd &orbCmd);
    void MediaStreamOpenProc(OrbProtocolCmd &orbCmd);
    void MediaStreamFrmCountProc(OrbProtocolCmd &orbCmd);
    void WifiApProc(OrbProtocolCmd &orbCmd);
    void WifiStaProc(OrbProtocolCmd &orbCmd);
    void IrcutStaProc(OrbProtocolCmd &orbCmd);
    void LightSensitiveValProc(OrbProtocolCmd &orbCmd);
    void OrbTfProc(OrbProtocolCmd &orbCmd);
    void SensorStaProc(OrbProtocolCmd &orbCmd);

    void GetCloudKeyProc(OrbProtocolCmd &orbCmd);
    void GetClientInfoProc(OrbProtocolCmd &orbCmd);

private:
    CSerialOrbCmdPkt m_cmdPkt;
    std::function<int(void *param,const uint8_t*,uint32_t)> m_sendFunc;
    void *m_pParam;
};


#endif
