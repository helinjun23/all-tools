#include "SerialProtocolOrbFac.h"
#include "clogcat.h"
#include "DataTyp.h"

CSerialProcotolOrbFac::CSerialProcotolOrbFac()
{
    m_sendFunc = nullptr;
    m_pParam = nullptr;
}

CSerialProcotolOrbFac::~CSerialProcotolOrbFac()
{

}
//cmd
int CSerialProcotolOrbFac::GetSysVer(void)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));
    data[0] =0x01;
    data[1] =0x03;
    datalen +=2;
    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
     SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}

int CSerialProcotolOrbFac::GetWifiVer(void)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));
    data[0] =0x01;
    data[1] =0x04;
    datalen +=2;
    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}
int CSerialProcotolOrbFac::GetSN(void)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypSNGet & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypSNGet & 0x00ff); //sub id
    datalen +=2;
    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
     SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}
int CSerialProcotolOrbFac::SetSN(uint8_t *sn,uint32_t snlen)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypSNSet & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypSNSet & 0x00ff); //sub id
    datalen +=2;

    memcpy(data+2,sn,snlen);
    datalen += snlen;
    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}
int CSerialProcotolOrbFac::Reset(void)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypReset & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypReset & 0x00ff); //sub id
    datalen +=2;
    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}
int CSerialProcotolOrbFac::GetPirSta(void)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypPirSta & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypPirSta & 0x00ff); //sub id
    datalen +=2;
    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
     SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}
int CSerialProcotolOrbFac::SetPirSta(bool bOpen)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypPirCtl & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypPirCtl & 0x00ff); //sub id
    datalen +=2;

    if(bOpen)
        data[2] = 0x01;
    else
        data[2] =0x00;
    datalen += 1;
    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}
int CSerialProcotolOrbFac::GetPirThresh(void)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypPirThreshGet & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypPirThreshGet & 0x00ff); //sub id
    datalen +=2;
    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}
int CSerialProcotolOrbFac::SetPirThresh(uint8_t thresh)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypPirThreshSet & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypPirThreshSet & 0x00ff); //sub id
    datalen +=2;

    data[2] =thresh;
    datalen += 1;
    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}
int CSerialProcotolOrbFac::GetPirTime(void)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypPirTimeGet & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypPirTimeGet & 0x00ff); //sub id
    datalen +=2;
    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}
int CSerialProcotolOrbFac::SetPirTime(uint8_t time)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypPirTimeSet & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypPirTimeSet & 0x00ff); //sub id
    datalen +=2;

    data[2] =time;
    datalen += 1;
    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}
int CSerialProcotolOrbFac::GetWifiSta(void)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypWifiSta & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypWifiSta & 0x00ff); //sub id
    datalen += 2;
    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}
int CSerialProcotolOrbFac::SetWifiSta(bool bCtl)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypWifiCtl & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypWifiCtl & 0x00ff); //sub id
    datalen += 2;
    data[2] =0x00; //xiumian
    if(bCtl)
        data[2]=0x01; //huanxing
    datalen += 1;
    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}
int CSerialProcotolOrbFac::GetWifiAp(void)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypWifiApGet & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypWifiApGet & 0x00ff); //sub id
    datalen += 2;
    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}

int CSerialProcotolOrbFac::SetWifiAp(uint8_t ap)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypWifiApSet & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypWifiApSet & 0x00ff); //sub id
    datalen += 2;
    data[2] =ap;
    datalen += 1;
    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}
int CSerialProcotolOrbFac::AudioRec(bool bRec)
{
    uint8_t data[255];
     uint32_t datalen=0;
     memset(data,0,sizeof(data));

     SerialProtocolCmdData sendData;
     memset(&sendData,0,sizeof(SerialProtocolCmdData));

     //data
     data[0] = (PeepholeOrbCmdTypMeidaAudioRec & 0xff00)>>8; //main id
     data[1] = (PeepholeOrbCmdTypMeidaAudioRec & 0x00ff); //sub id
     datalen += 2;

     if(bRec)
     {
         data[2] =0x01;
         datalen += 1;
     }
     else
     {
         data[2] =0x00;
         datalen += 1;
     }

    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}

int CSerialProcotolOrbFac::AudioRecPly(uint8_t plyTyp)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypMeidaAudioPlay & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypMeidaAudioPlay & 0x00ff); //sub id
    datalen += 2;

    data[2] =plyTyp;
    datalen += 1;

    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}

int CSerialProcotolOrbFac::StartMedia(uint8_t protocolTyp,uint8_t streamTyp,uint8_t netTyp)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));
    //data
    data[0] = (PeepholeOrbCmdTypMeidaStreamOpen & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypMeidaStreamOpen & 0x00ff); //sub id
    datalen += 2;
    //typ:0x01:RTSP;0x02:RTMP;0x03:其他
    data[2] = protocolTyp;
    datalen += 1;
    //connect:0x01:wifi连接；0x02:有线连接
    data[3]=netTyp;
    datalen += 1;

    //0x01:主码流;0x02:子码流；0x03:第三码流
    data[4]=streamTyp;
    datalen += 1;
    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}

int CSerialProcotolOrbFac::StopMedia(void)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));
    //data
    data[0] = (PeepholeOrbCmdTypMeidaStreamClose & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypMeidaStreamClose & 0x00ff); //sub id
    datalen += 2;
    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}
int CSerialProcotolOrbFac::GetMediaUrl(void)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypMeidaStreamUrl & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypMeidaStreamUrl & 0x00ff); //sub id
    datalen += 2;

    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}

int CSerialProcotolOrbFac::GetMediaCurFrm(uint8_t streamTyp)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypMeidaStreamFrmCount & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypMeidaStreamFrmCount & 0x00ff); //sub id
    datalen += 2;

    //0x01:主码流;0x02:子码流；0x03:第三码流
    data[2]=streamTyp;
    datalen += 1;
    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}

int CSerialProcotolOrbFac::GetIrcutSta(void)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypIrCutSta & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypIrCutSta & 0x00ff); //sub id
    datalen += 2;
    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}
int CSerialProcotolOrbFac::SetIrcutSta(uint8_t sta)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypIrCutCtl & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypIrCutCtl & 0x00ff); //sub id
    datalen += 2;

    data[2]=sta;
    datalen +=1;

    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}
int CSerialProcotolOrbFac::GetLightSensitive(void)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypLightSensitiveVal & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypLightSensitiveVal & 0x00ff); //sub id
    datalen += 2;
    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}

int CSerialProcotolOrbFac::LightSensitiveCtl(bool bOpen)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypLightSensitiveCtl & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypLightSensitiveCtl & 0x00ff); //sub id
    datalen += 2;

    if(bOpen)
    {
        data[2] = 0x01;
    }
    else
    {
        data[2] =0x00;
    }
    datalen +=1;
    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}

int CSerialProcotolOrbFac::FactoryMode(bool bOpen)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypFactoryMode & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypFactoryMode & 0x00ff); //sub id
    datalen += 2;

    if(bOpen)
    {
        data[2] =0x01;
    }
    else
    {
        data[2] =0x00;
    }
    datalen +=1;
    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}

int CSerialProcotolOrbFac::GetSDSize(void)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypTF & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypTF & 0x00ff); //sub id
    datalen += 2;
    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}
int CSerialProcotolOrbFac::LedCtl(bool bOpen)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypLedCtl & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypLedCtl & 0x00ff); //sub id
    datalen += 2;

    if(bOpen)
    {
        data[2] = 0x01;
        datalen += 1;
    }
    else
    {
        data[2] = 0x00;
        datalen += 1;
    }
    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}
int CSerialProcotolOrbFac::GetSensorSta(void)
{
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypSensorSta & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypSensorSta & 0x00ff); //sub id
    datalen += 2;

    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}

int CSerialProcotolOrbFac::SetWireIP(uint8_t *ipv4,uint8_t *gateway,uint8_t *mask)
{
    //
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypWireIp & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypWireIp & 0x00ff); //sub id
    datalen += 2;

    memcpy(data +2,ipv4,4*sizeof(uint8_t));
    datalen += 4;

    memcpy(data +6,mask,4*sizeof(uint8_t));
    datalen += 4;

    memcpy(data +10,gateway,4*sizeof(uint8_t));
    datalen += 4;

    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
    return 0;
}

int CSerialProcotolOrbFac::GetCloudKey(void)
{
    //
    uint8_t data[255];
    uint32_t datalen=0;
    memset(data,0,sizeof(data));

    SerialProtocolCmdData sendData;
    memset(&sendData,0,sizeof(SerialProtocolCmdData));

    //data
    data[0] = (PeepholeOrbCmdTypGetCloudKey & 0xff00)>>8; //main id
    data[1] = (PeepholeOrbCmdTypGetCloudKey & 0x00ff); //sub id
    datalen += 2;

    data[2]=0x00;
    datalen += 1;

    //fill send data
    m_cmdPkt.FillHostPkt(&sendData,OrbProtocolPktTypPeepholeFac,data,datalen);
    SendCmd((const uint8_t *)sendData.data,sendData.dataLen);
}
//parse
int CSerialProcotolOrbFac::OrbFacParseData(OrbProtocolCmd &orbCmd)
{
    uint16_t cmdTyp = orbCmd.data[0]<<8 | orbCmd.data[1];
    //emit ShowNoteInfo("cmdtyp="+QString::number(cmdTyp),0,true);
    switch (cmdTyp) {
    case PeepholeOrbCmdTypSNGet:
        OrbSnProc(orbCmd);
        break;
    case PeepholeOrbCmdTypSNSet:
        break;
    case PeepholeOrbCmdTypSysVer:
    case PeepholeOrbCmdTypWifiVer:
        OrbVerProc(orbCmd);
        break;
    case PeepholeOrbCmdTypTF:
        OrbTfProc(orbCmd);
        break;
    case PeepholeOrbCmdTypReset:
        break;
    case PeepholeOrbCmdTypFactoryMode:
        break;
    case PeepholeOrbCmdTypSensorSta:
        SensorStaProc(orbCmd);
        break;
    case PeepholeOrbCmdTypLedCtl:
        break;
    case PeepholeOrbCmdTypPirThreshGet:
        PirDistProc(orbCmd);
        break;
    case PeepholeOrbCmdTypPirThreshSet:
        break;
    case PeepholeOrbCmdTypPirCtl:
        break;
    case PeepholeOrbCmdTypPirSta:
        PirStaProc(orbCmd);
        break;
    case PeepholeOrbCmdTypPirTimeGet:
        PirTimeProc(orbCmd);
        break;
    case PeepholeOrbCmdTypPirTimeSet:
        break;
    case PeepholeOrbCmdTypMeidaAudioRec:
        break;
    case PeepholeOrbCmdTypMeidaAudioPlay:
        break;
    case PeepholeOrbCmdTypMeidaStreamUrl:
        MediaStreamUrlProc(orbCmd);
        break;
    case PeepholeOrbCmdTypMeidaStreamOpen:
        MediaStreamOpenProc(orbCmd);
        break;
    case PeepholeOrbCmdTypMeidaStreamClose:
        break;
    case PeepholeOrbCmdTypMeidaStreamFrmCount:
        MediaStreamFrmCountProc(orbCmd);
        break;
    case PeepholeOrbCmdTypWifiApGet:
        WifiApProc(orbCmd);
        break;
    case PeepholeOrbCmdTypWifiApSet:
        break;
    case PeepholeOrbCmdTypWifiCtl:
        break;
    case PeepholeOrbCmdTypWifiSta:
        WifiStaProc(orbCmd);
        break;
    case PeepholeOrbCmdTypIrCutCtl:
        break;
    case PeepholeOrbCmdTypIrCutSta:
        IrcutStaProc(orbCmd);
        break;
    case PeepholeOrbCmdTypLightSensitiveVal:
        LightSensitiveValProc(orbCmd);
        break;
    case PeepholeOrbCmdTypLightSensitiveCtl:
        break;
    case PeepholeOrbCmdTypGetCloudKey:
        GetCloudKeyProc(orbCmd);
        break;
    case PeepholeOrbCmdTypGetClientInfo:
        GetClientInfoProc(orbCmd);
        break;
    default:
        break;
    }
    return 1;
}

//orbbec
void CSerialProcotolOrbFac::OrbSnProc(OrbProtocolCmd &orbCmd)
{
    uint16_t cmdTyp = orbCmd.data[0]<<8 | orbCmd.data[1];
    if(cmdTyp != PeepholeOrbCmdTypSNGet)
    {
        emit CLogcat::Instance()->ShowNoteInfo("cmdTyp err",0,true);
        return;
    }
    uint8_t err = orbCmd.data[2];
    uint8_t da[256]={0};

    memcpy(da,orbCmd.data + 3,orbCmd.len -3);
    QString noteStr;
    noteStr.sprintf("%s",da);
    emit CLogcat::Instance()->ShowDevInfo(cmdTyp,noteStr);
    emit CLogcat::Instance()->ShowNoteInfo("sn="+noteStr,0,true);
}

void CSerialProcotolOrbFac::OrbVerProc(OrbProtocolCmd &orbCmd)
{
    uint16_t cmdTyp = orbCmd.data[0]<<8 | orbCmd.data[1];
    if((cmdTyp != PeepholeOrbCmdTypSysVer) && (cmdTyp != PeepholeOrbCmdTypWifiVer))
    {
        emit CLogcat::Instance()->ShowNoteInfo("cmdTyp err",0,true);
        return;
    }
    uint8_t err = orbCmd.data[2];
    uint8_t da[256]={0};
    QString noteStr;
    //emit ShowNoteInfo("cmdtyp="+QString::number(cmdTyp),0,true);

    memcpy(da,orbCmd.data + 3,orbCmd.len -3);
    noteStr.sprintf("%s",da);
    emit CLogcat::Instance()->ShowDevInfo(cmdTyp,noteStr);
    emit CLogcat::Instance()->ShowNoteInfo(noteStr,0,true);
}

void CSerialProcotolOrbFac::PirDistProc(OrbProtocolCmd &orbCmd)
{
    uint16_t cmdTyp = orbCmd.data[0]<<8 | orbCmd.data[1];
    if(cmdTyp != PeepholeOrbCmdTypPirThreshGet)
    {
        emit CLogcat::Instance()->ShowNoteInfo("cmdTyp err",0,true);
        return;
    }
    //uint8_t err = orbCmd.data[2];
    QString noteStr;
    noteStr.sprintf("%d",orbCmd.data[3]);
    emit CLogcat::Instance()->ShowDevInfo(cmdTyp,noteStr);
    //emit ShowNoteInfo(noteStr,0,true);
}
void CSerialProcotolOrbFac::PirStaProc(OrbProtocolCmd &orbCmd)
{
    uint16_t cmdTyp = orbCmd.data[0]<<8 | orbCmd.data[1];
    if(cmdTyp != PeepholeOrbCmdTypPirSta)
    {
        emit CLogcat::Instance()->ShowNoteInfo("cmdTyp err",0,true);
        return;
    }
    uint8_t err = orbCmd.data[2];
    QString noteStr;
    noteStr.sprintf("%d",orbCmd.data[3]);
    emit CLogcat::Instance()->ShowDevInfo(cmdTyp,noteStr);
    //emit ShowNoteInfo(noteStr,0,true);
}

void CSerialProcotolOrbFac::PirTimeProc(OrbProtocolCmd &orbCmd)
{
    uint16_t cmdTyp = orbCmd.data[0]<<8 | orbCmd.data[1];
    if(cmdTyp != PeepholeOrbCmdTypPirTimeGet)
    {
        emit CLogcat::Instance()->ShowNoteInfo("cmdTyp err",0,true);
        return;
    }
    //uint8_t err = orbCmd.data[2];
    QString noteStr;
    noteStr.sprintf("%d",orbCmd.data[3]);
    emit CLogcat::Instance()->ShowDevInfo(cmdTyp,noteStr);
}

void CSerialProcotolOrbFac::MediaStreamUrlProc(OrbProtocolCmd &orbCmd)
{
    uint16_t cmdTyp = orbCmd.data[0]<<8 | orbCmd.data[1];
    if(cmdTyp != PeepholeOrbCmdTypMeidaStreamUrl)
    {
        emit CLogcat::Instance()->ShowNoteInfo("cmdTyp err",0,true);
        return;
    }
    uint8_t err = orbCmd.data[2];
    uint8_t da[256]={0};
    QString noteStr;
    memcpy(da,orbCmd.data + 3,orbCmd.len -3);
    noteStr.sprintf("%s",da);
    emit CLogcat::Instance()->ShowDevInfo(cmdTyp,noteStr);
    emit CLogcat::Instance()->ShowNoteInfo(noteStr,0,true);
}
void CSerialProcotolOrbFac::MediaStreamOpenProc(OrbProtocolCmd &orbCmd)
{
    uint16_t cmdTyp = orbCmd.data[0]<<8 | orbCmd.data[1];
    if(cmdTyp != PeepholeOrbCmdTypMeidaStreamOpen)
    {
        emit CLogcat::Instance()->ShowNoteInfo("cmdTyp err",0,true);
        return;
    }
    uint8_t err = orbCmd.data[2];
    uint8_t da[256]={0};
    QString noteStr;
    memcpy(da,orbCmd.data + 3,orbCmd.len -3);
    noteStr.sprintf("%s",da);
    emit CLogcat::Instance()->ShowDevInfo(cmdTyp,noteStr);
    emit CLogcat::Instance()->ShowNoteInfo(noteStr,0,true);
}

void CSerialProcotolOrbFac::MediaStreamFrmCountProc(OrbProtocolCmd &orbCmd)
{
    uint16_t cmdTyp = orbCmd.data[0]<<8 | orbCmd.data[1];
    if(cmdTyp != PeepholeOrbCmdTypMeidaStreamFrmCount)
    {
        emit CLogcat::Instance()->ShowNoteInfo("cmdTyp err",0,true);
        return;
    }
    uint8_t err = orbCmd.data[2];
    uint8_t srcTyp = orbCmd.data[3];
    uint32_t frmNum =(orbCmd.data[7]<<24) |(orbCmd.data[6]<<16)|(orbCmd.data[5]<< 8) |orbCmd.data[4];
    QString noteStr;
    noteStr.sprintf("%d",frmNum);
    emit CLogcat::Instance()->ShowDevInfo(cmdTyp,noteStr);
    emit CLogcat::Instance()->ShowNoteInfo(noteStr,0,true);
}

void CSerialProcotolOrbFac::WifiApProc(OrbProtocolCmd &orbCmd)
{
    uint16_t cmdTyp = orbCmd.data[0]<<8 | orbCmd.data[1];
    if(cmdTyp != PeepholeOrbCmdTypWifiApGet)
    {
        emit CLogcat::Instance()->ShowNoteInfo("cmdTyp err",0,true);
        return;
    }
    uint8_t err = orbCmd.data[2];

    QString noteStr;
    noteStr.sprintf("%d",(char)orbCmd.data[3]);
    emit CLogcat::Instance()->ShowDevInfo(cmdTyp,noteStr);
    //emit ShowNoteInfo(noteStr,0,true);
}
void CSerialProcotolOrbFac::WifiStaProc(OrbProtocolCmd &orbCmd)
{
    uint16_t cmdTyp = orbCmd.data[0]<<8 | orbCmd.data[1];
    if(cmdTyp != PeepholeOrbCmdTypWifiSta)
    {
        emit CLogcat::Instance()->ShowNoteInfo("cmdTyp err",0,true);
        return;
    }
    uint8_t err = orbCmd.data[2];

    QString noteStr;
    noteStr.sprintf("%d",orbCmd.data[3]);
    emit CLogcat::Instance()->ShowDevInfo(cmdTyp,noteStr);
    //emit ShowNoteInfo(noteStr,0,true);
}
void CSerialProcotolOrbFac::IrcutStaProc(OrbProtocolCmd &orbCmd)
{
    uint16_t cmdTyp = orbCmd.data[0]<<8 | orbCmd.data[1];
    if(cmdTyp != PeepholeOrbCmdTypIrCutSta)
    {
        emit CLogcat::Instance()->ShowNoteInfo("cmdTyp err",0,true);
        return;
    }
    uint8_t err = orbCmd.data[2];
    //QString noteStr;
    //noteStr.sprintf("%d",orbCmd.data[3]);
    emit CLogcat::Instance()->ShowDevInfo(cmdTyp,QString::number(orbCmd.data[3]));
    //emit ShowNoteInfo(noteStr,0,true);
}

void CSerialProcotolOrbFac::LightSensitiveValProc(OrbProtocolCmd &orbCmd)
{
    uint16_t cmdTyp = orbCmd.data[0]<<8 | orbCmd.data[1];
    if(cmdTyp != PeepholeOrbCmdTypLightSensitiveVal)
    {
        emit CLogcat::Instance()->ShowNoteInfo("cmdTyp err",0,true);
        return;
    }

    //uint8_t  err = orbCmd.data[2];
    uint16_t val = (orbCmd.data[3]) | (orbCmd.data[4]<<8);

    QString noteStr;
    noteStr.sprintf("%d",val);
    emit CLogcat::Instance()->ShowDevInfo(cmdTyp,noteStr);
    //emit ShowNoteInfo(noteStr,0,true);
}

void CSerialProcotolOrbFac::GetCloudKeyProc(OrbProtocolCmd &orbCmd)
{
    uint16_t cmdTyp = orbCmd.data[0]<<8 | orbCmd.data[1];
    if(cmdTyp != PeepholeOrbCmdTypGetCloudKey)
    {
        emit CLogcat::Instance()->ShowNoteInfo("cmdTyp err",0,true);
        return;
    }
    char productKey[32]={0};
    char productSecret[64]={0};
    char deviceName[32] ={0};
    char deviceSecret[64]={0};

    uint8_t *pTmp = orbCmd.data + 3;
    memcpy(productKey,pTmp,32);
    pTmp += 32;
    memcpy(productSecret,pTmp,64);
    pTmp += 64;
    memcpy(deviceName,pTmp,32);
    pTmp += 32;
    memcpy(deviceSecret,pTmp,64);
    pTmp += 64;


    //QString noteStr;
    //noteStr.sprintf("%d",val);
    //emit CLogcat::Instance()->ShowDevInfo(cmdTyp,noteStr);

    emit CLogcat::Instance()->ShowDevInfo(PeepholeOrbCmdTypGetCloudKeyProductKey,productKey);
    emit CLogcat::Instance()->ShowDevInfo(PeepholeOrbCmdTypGetCloudKeyProductSecret,productSecret);
    emit CLogcat::Instance()->ShowDevInfo(PeepholeOrbCmdTypGetCloudKeyDeviceName,deviceName);
    emit CLogcat::Instance()->ShowDevInfo(PeepholeOrbCmdTypGetCloudKeyDeviceSecret,deviceSecret);

    QString noteStr;
    noteStr.sprintf("productKey=%s,productSecret=%s,deviceName=%s,deviceSecret=%s",productKey,productSecret,deviceName,deviceSecret);
    emit CLogcat::Instance()->ShowNoteInfo(noteStr,0,true);


}

void CSerialProcotolOrbFac::GetClientInfoProc(OrbProtocolCmd &orbCmd)
{

}
void CSerialProcotolOrbFac::OrbTfProc(OrbProtocolCmd &orbCmd)
{
    uint16_t cmdTyp = orbCmd.data[0]<<8 | orbCmd.data[1];
    if(cmdTyp != PeepholeOrbCmdTypTF)
    {
        emit CLogcat::Instance()->ShowNoteInfo("cmdTyp err",0,true);
        return;
    }

    uint8_t  err = orbCmd.data[2];
    uint32_t val =0;
    val = (orbCmd.data[3]);
    //emit ShowNoteInfo("data="+QString::number(orbCmd.data[3]),0,true);
    //emit ShowNoteInfo("val="+QString::number(val),0,true);
    val |= (orbCmd.data[4])<<8;
    val |= (orbCmd.data[5])<<16;
    val |= (orbCmd.data[6])<<24;

    QString noteStr;
    noteStr.sprintf("%d MB",val);
    emit CLogcat::Instance()->ShowDevInfo(cmdTyp,noteStr);
    //emit ShowNoteInfo(noteStr,0,true);
}

void CSerialProcotolOrbFac::SensorStaProc(OrbProtocolCmd &orbCmd)
{
    uint16_t cmdTyp = orbCmd.data[0]<<8 | orbCmd.data[1];
    if(cmdTyp != PeepholeOrbCmdTypSensorSta)
    {
        emit CLogcat::Instance()->ShowNoteInfo("cmdTyp err",0,true);
        return;
    }

    uint8_t  err = orbCmd.data[2];
    uint8_t val =0;
    val = (orbCmd.data[3]);
    QString noteStr;
    if(0x00 == val)
    {
        noteStr.sprintf("normal");
    }

    if(0x01 == val)
    {
        noteStr.sprintf("error");
    }

    if(!err)
    {
        noteStr.sprintf("other err");
    }
    emit CLogcat::Instance()->ShowDevInfo(cmdTyp,noteStr);
}
