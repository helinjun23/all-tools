#ifndef CLOGCAT_H
#define CLOGCAT_H

#include <QObject>

class CLogcat : public QObject
{
    Q_OBJECT
public:
    ~CLogcat() {}
    static CLogcat* Instance()
    {
        static CLogcat mLog(nullptr);
        return &mLog;
    }
    void OutSendCmd(QString tag,unsigned char *cmd,unsigned char cmdlen)
    {
        QString str,strTmp;
        for(int i= 0; i < cmdlen;i++)
        {
            strTmp.sprintf("%02x ",cmd[i]);
            str += strTmp;
        }
        emit ShowNoteInfo(tag + str,0,true);
    }

    void OutArrayStr(QString tag,QByteArray arrayData)
    {
        QString str,strTmp;
        if(arrayData.length() > 0)
        {
#if 0
            str="";
            strTmp="";
            for(int i=0;i < arrayData.length();i++)
            {
                 unsigned char ch = (unsigned char)arrayData.at(i);
                 strTmp.sprintf("%02x ",ch);
                 str +=  strTmp;
            }
#endif
            QByteArray hexArr =(QByteArray)arrayData.toHex(' ');
            emit ShowNoteInfo(tag+QString(hexArr.toStdString().c_str()),0,true);
        }
    }
    void PrintHex(unsigned char *data,int datalen,QString tag=QString("hex:"))
    {
        QString str,strTmp;
        if(datalen > 0)
        {
            str="";
            strTmp="";
            for(int i=0;i < datalen;i++)
            {
                 strTmp.sprintf("%02X ",data[i]);
                 str +=  strTmp;
            }
            emit ShowNoteInfo(tag+str,0,true);
        }
    }
protected:
    explicit CLogcat(QObject *parent = nullptr): QObject(parent) {}

signals:
    void ShowNoteInfo(QString noteStr,int bkColor,bool bShow);
    void ShowInfoDialog(QString info);
    void SetLogPath(char *logPath);
    void ShowDevInfo(int infoTyp,QString info);
public slots:


};

#endif // CLOGCAT_H
