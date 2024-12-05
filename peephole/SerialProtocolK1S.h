#ifndef _SERIAL_PROTOCOL_K1S_H_
#define _SERIAL_PROTOCOL_K1S_H_

#include"SerialProtocolBase.h"
#include "PeepholeSerialK1SDataTyp.h"
#include "OrbDataTyp.h"

class CSerialProtocolK1S:public CSerialProtocolBase
{
public:
    CSerialProtocolK1S();
    ~CSerialProtocolK1S();
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
    int ProcMidInit(uint8_t *data,uint32_t dataSz);
    int ProcMidEncKey(uint8_t *data,uint32_t dataSz);
    int ProcMidTriggerCap(uint8_t *data,uint32_t dataSz);
    int ProcMidDelMediaFile(uint8_t *data,uint32_t dataSz);
    int ProcMidSetVolume(uint8_t *data,uint32_t dataSz);
    int ProcMidGetVersion(uint8_t *data,uint32_t dataSz);
    int ProcMidStartOta(uint8_t *data,uint32_t dataSz);
    int ProcMidStopOta(uint8_t *data,uint32_t dataSz);
    int ProcMidSetBaudrate(uint8_t *data,uint32_t dataSz);
    int ProcMidConfigWifi(uint8_t *data,uint32_t dataSz);
    int ProcMidWifiMac(uint8_t *data,uint32_t dataSz);
    int ProcMidGetWifiSta(uint8_t *data,uint32_t dataSz);
    int ProcMidSetLinkKey(uint8_t *data,uint32_t dataSz);
    int ProcMidDataPassthough(uint8_t *data,uint32_t dataSz);
    int ProcMidTriggerVideoCall(uint8_t *data,uint32_t dataSz);
    int ProcMidGetDevicename(uint8_t *data,uint32_t dataSz);
    int ProcMidGetModuleSta(uint8_t *data,uint32_t dataSz);
    int ProcMidPowerdown(uint8_t *data,uint32_t dataSz);
    int ProcMidDebugMode(uint8_t *data,uint32_t dataSz);
    int ProcMidGetDebugInfo(uint8_t *data,uint32_t dataSz);
    int ProcMidUploadDebugInfo(uint8_t *data,uint32_t dataSz);
    int PorcMidHiJack(uint8_t *data,uint32_t dataSz);
    int ProcCmd(k1s_cmd_msg &cmd);
    int ProcReply(k1s_cmd_msg &cmd);
    int ProcNote(k1s_cmd_msg &cmd);
    int OtaSta(k1s_cmd_msg &cmd);
    int RecSta(k1s_cmd_msg &cmd);
    int MediaSta(k1s_cmd_msg &cmd);
    bool CheckMsgId(uint32_t mid);
    int TriggerVideoCall(const MediaCapEventParam &param);
    bool CheckEncryptRecvFinish(void);
    bool CheckRecvFinish(void);
    int ProcessRecvData(void);
    int ProcessEncryptRecvData(void);
    int Random(int min,int max);
    int SetKey(uint8_t *seed, int size,uint8_t *order,uint8_t orderlen);
    int FillMsgData(int typ,const uint8_t *data,int datalen,bool bEncrypt);
    int FillNormalMsg(uint8_t typ,const uint8_t *data,uint32_t datalen, SerialProtocolCmdData &sendData);
    int FillEncryptMsg(uint8_t typ,const uint8_t *data,uint32_t datalen,SerialProtocolCmdData &sendData);
    int SendMsg(const SerialProtocolCmdData &sendMsg);
    uint8_t XOR_Inverted_Check(uint8_t *inBuf,int inLen);
private:
    SerialProtocolCmdData m_recvBuf;
    SerialProtocolCmdData m_sendMsg;
    k1s_encryp_info       m_encrypInfo;
    k1s_msg_init_data     m_initMsg;
    uint8_t m_seed[4];
};


#endif
