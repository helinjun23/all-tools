#ifndef _SERIAL_ORB_CMD_PKT_H_
#define _SERIAL_ORB_CMD_PKT_H_

#include <stdint.h>
#include <string.h>
#include "OrbDataTyp.h"
#include "OrbProtocolDataTyp.h"

class CSerialOrbCmdPkt
{
public:
   CSerialOrbCmdPkt() {}
   ~CSerialOrbCmdPkt() {}
   //pktdata:cmd buff
   //pktTyp:enum OrbProtocolPktTyp
   int FillHostPkt(SerialProtocolCmdData *pktData,int pktTyp,uint8_t *data,uint32_t datalen)
   {
       if(nullptr == pktData || nullptr == data)
           return -1;
       AddHead(pktData,pktTyp,OrbProtocolSourceHost);
       AddData(pktData,data,datalen);
       AddCheckSum(pktData);
       return 0;
   }
   //pktdata:cmd buff
   //pktTyp:enum OrbProtocolPktTyp
   int FillSlavePkt(SerialProtocolCmdData *pktData,int pktTyp,uint8_t *data,uint32_t datalen)
   {
        if(nullptr == pktData || nullptr == data)
           return -1;
        AddHead(pktData,pktTyp,OrbProtocolSourceSlave);
        AddData(pktData,data,datalen);
        AddCheckSum(pktData);
        return 0;
   }
   //pktdata:cmd buff
   //pktTyp:enum OrbProtocolPktTyp
   //srcTyp:enum OrbProtocolSource
   void AddHead(SerialProtocolCmdData *pktData,int pktTyp,int srcTyp)
   {
        if(nullptr == pktData)
        {
             return;
        }

        pktData->data[0]= ORB_PROTOCOL_TAG_LOW;
        pktData->data[1]= ORB_PROTOCOL_TAG_HIGH;
        pktData->data[2]=srcTyp;
        pktData->data[3]=pktTyp;
        pktData->dataLen += 4;
   }

   void AddData(SerialProtocolCmdData *pktData,uint8_t *data,uint16_t datalen)
   {
        if(nullptr == pktData)
        {
             return;
        }
        pktData->data[pktData->dataLen]=datalen&0x00ff;
        pktData->data[pktData->dataLen + 1] =(datalen &0xff00)>>8;
        pktData->dataLen += 2;
        uint8_t *pData = pktData->data + pktData->dataLen;
        memcpy(pData,data,datalen*sizeof(uint8_t));
        pktData->dataLen += datalen;  
   }

   void AddCheckSum(SerialProtocolCmdData *pktData)
   {
        if(nullptr == pktData)
        {
             return;
        }
        uint16_t crc = 0;
        crc = CheckSum(pktData->data,pktData->dataLen);
        pktData->data[pktData->dataLen]=(crc&0x00ff);
        pktData->data[pktData->dataLen + 1]=(crc&0xff00)>>8;
        pktData->dataLen += 2;
   }

   uint16_t CheckSum(uint8_t *cmd,uint32_t cmdLen)
   {
        uint16_t crc=0;
        uint16_t sum=0;
        for(uint32_t i=0;i<cmdLen;i++)
        {
            sum += cmd[i];
        }
        crc = 0x10000-sum;
        return crc;
    }
protected:

private:

};

#endif
