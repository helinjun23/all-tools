#ifndef _ORB_CMD_PKT_BASE_H_
#define _ORB_CMD_PKT_BASE_H_

#include "OrbDataTyp.h"
#include "OrbProtocolDataTyp.h"
#include "DataBuffer.h"

class COrbCmdPktBase{
public:
    COrbCmdPktBase() {}
    virtual ~COrbCmdPktBase() {}
    //pktdata:cmd buff
   //pktTyp:enum OrbProtocolPktTyp
   //srcTyp:enum OrbProtocolSource
   void AddHead(const uint8_t pktTyp,const uint8_t srcTyp,CDataBuffer& cmd)
   {
        cmd.AddData(ORB_PROTOCOL_TAG_LOW);
        cmd.AddData(ORB_PROTOCOL_TAG_HIGH);
        cmd.AddData(srcTyp);
        cmd.AddData(pktTyp);
   }

   void AddData(const uint8_t *data,const uint16_t datalen,CDataBuffer& cmd)
   {
        if(nullptr == data)
        {
             return;
        }
        cmd.AddData(datalen&0x00ff);
        cmd.AddData((datalen &0xff00)>>8);
        cmd.AddData(data,datalen);
   }

   void AddCheckSum(CDataBuffer& cmd)
   {
        uint16_t crc = 0;
        crc = CheckSum(cmd.GetData(),cmd.GetDataLen());
        cmd.AddData(crc&0x00ff);
        cmd.AddData((crc&0xff00)>>8);
   }

protected:
   uint16_t CheckSum(const uint8_t *cmd,const uint32_t cmdLen)
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

};


#endif
