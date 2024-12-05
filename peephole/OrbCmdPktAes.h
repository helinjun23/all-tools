#ifndef _ORB_CMD_PKT_AES_H_
#define _ORB_CMD_PKT_AES_H_

#include"OrbCmdPktBase.h"
#ifdef WIN32
#include"Secure.h"
#else
#include "DataSecure.h"
#endif

class COrbCmdPktAes
{
public:
    explicit COrbCmdPktAes(void){}
    ~COrbCmdPktAes() {}
    
    //pktdata:cmd buff
    //pktTyp:enum OrbProtocolPktTyp
    int FillHostPktAes(const int pktTyp,const uint8_t *data,const uint32_t datalen,const uint8_t* key,const uint8_t keylen,CDataBuffer& cmd)
    {
       if(nullptr == data)
           return -1;
       m_pkt.AddHead(pktTyp,OrbProtocolSourceHost,cmd);
       AddDataAes(data,datalen,key,keylen,cmd);
       m_pkt.AddCheckSum(cmd);
       return 0;
   }
   //pktdata:cmd buff
   //pktTyp:enum OrbProtocolPktTyp
   int FillSlavePktAes(const int pktTyp,const uint8_t *data,const uint32_t datalen,const uint8_t* key,const uint8_t keylen,CDataBuffer& cmd)
   {
        if(nullptr == data)
           return -1;
        m_pkt.AddHead(pktTyp,OrbProtocolSourceSlave,cmd);
        AddDataAes(data,datalen,key,keylen,cmd);
        m_pkt.AddCheckSum(cmd);
        return 0;
   }
protected:
      void AddDataAes(const uint8_t *data,const uint16_t datalen,const uint8_t* key,const uint8_t keylen,CDataBuffer& cmd)
   {
        if(nullptr == data)
        {
             return;
        }
        uint8_t dataSrc[512]={0};
        uint32_t dataSrcLen=0;
        uint8_t dataEnc[512]={0};
        dataSrc[0] = datalen&0x00ff;
        dataSrc[1] = (datalen &0xff00)>>8;
        dataSrcLen += 2;
        memcpy(dataSrc + 2,data,datalen*sizeof(uint8_t));
        dataSrcLen += datalen;

#ifdef WIN32
        CSecure dataSecure;
        int encLen = dataSecure.AesEcbEncrypt(key,keylen,dataSrc,dataSrcLen,dataEnc);
#else
        CDataSecure dataSecure;
        int encLen = dataSecure.AesEcbEnc(key,keylen,dataSrc,dataSrcLen,dataEnc);
#endif
        cmd.AddData(encLen&0x00ff);
        cmd.AddData((encLen &0xff00)>>8);
        cmd.AddData(dataEnc,encLen); 
   } 
private:
   COrbCmdPktBase m_pkt;
};



#endif
