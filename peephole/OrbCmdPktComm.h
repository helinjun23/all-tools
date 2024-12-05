#ifndef _ORB_CMD_PKT_COMM_H_
#define _ORB_CMD_PKT_COMM_H_

#include "OrbCmdPktBase.h"

class COrbCmdPktComm
{
public:
    COrbCmdPktComm() {}
    ~COrbCmdPktComm() {}
    //pktdata:cmd buff
    //pktTyp:enum OrbProtocolPktTyp
    int FillHostPkt(const int pktTyp,const uint8_t *data,const uint32_t datalen,CDataBuffer& cmd)
    {
       if(nullptr == data)
           return -1;
       m_pkt.AddHead(pktTyp,OrbProtocolSourceHost,cmd);
       m_pkt.AddData(data,datalen,cmd);
       m_pkt.AddCheckSum(cmd);
       return 0;
   }
   //pktdata:cmd buff
   //pktTyp:enum OrbProtocolPktTyp
   int FillSlavePkt(const int pktTyp,const uint8_t *data,const uint32_t datalen,CDataBuffer& cmd)
   {
        if(nullptr == data)
           return -1;
        m_pkt.AddHead(pktTyp,OrbProtocolSourceSlave,cmd);
        m_pkt.AddData(data,datalen,cmd);
        m_pkt.AddCheckSum(cmd);
        return 0;
   }
protected:
   COrbCmdPktBase m_pkt;
};

#endif
