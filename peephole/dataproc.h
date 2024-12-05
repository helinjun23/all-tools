#ifndef DATAPROC_H
#define DATAPROC_H

#include <QSerialPortInfo>
#include <QSerialPort>
#include <QMutex>
#include <QStandardItem>
#include <fstream>
#include <io.h>
#include <iostream>
#include <QThread>
#include "RecFile.h"
#include "ThreadProc.h"
#include "SerialProtocolBase.h"

class CDataProc:public QObject
{
    Q_OBJECT
public:
    CDataProc();
    ~CDataProc();

    int Start(CSerialProtocolBase *pProtocol=nullptr,uint8_t protocolTyp=0);
    int Stop();

    int  SetParam(QString portName,int iBaudRate,int iDatabits,int stopBits);
    void PushCmd(const uint8_t *cmd,const uint32_t cmdlen,int cmdTyp=0);
    static int  CmdSend(void *param,const uint8_t *cmd,const uint32_t cmdlen);

protected:
    void SetDevComParam(QSerialPort &serial,QString portName, int iBaudRate,int iDatabits,int stopBits);
    bool OpenCom(QSerialPort &serial,QSerialPort::OpenMode mode= QIODevice::ReadWrite);
    bool CloseCom(QSerialPort &serial);

    int PopCmd(SerialCmd &cmd);
    void PrintHex(unsigned char *data,int datalen);
    uint16_t OrbCheckSum(unsigned char *cmd,uint32_t cmdLen);
    void SendCmd(QSerialPort &serial,unsigned char *cmd,uint32_t cmdlen);
    int SerialRead(QSerialPort &serial,QByteArray &respData);

    void ParseCmd(QByteArray &cmdData);
    static void SerialReadThreadProc(void *param);
    void SerialReadProc();

private:
    int m_serialTimeOut;

    //bool m_bDevOpen;
    bool m_bPicSave;
    QString m_devPortName;
    int m_devBaudRate;
    int m_devDatabits;
    int m_devStopBit;
    int m_trySerialOpenNum;
    //serial read thread
    CThreadProc *m_pThreadProc;
    bool m_bRun;

    QMutex m_mutex;
    std::list<SerialCmd> m_cmdlist;
    CSerialProtocolBase *m_pSerialProtocol;

    uint8_t m_parseSta;
    CSerialProtocolBase *m_pOrbProtocol;

    uint8_t m_curPclTyp;
};
#endif // DATAPROC_H
