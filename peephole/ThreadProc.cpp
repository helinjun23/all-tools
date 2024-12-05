#include "ThreadProc.h"

CThreadProc::CThreadProc(QObject *parent):m_bRun(false),m_pParam(NULL),m_pFunc(NULL)
{
    m_runFlag=0;
}

CThreadProc::~CThreadProc()
{
    Stop();
}

void CThreadProc::Start(void *param,ThreadProcFunc pCbFunc,int flag)
{
    m_pFunc = pCbFunc;
    m_pParam = param;

    m_runFlag = flag;
    //thread start function
    m_bRun = true;
    start();
    QObject object;
    object.moveToThread(this);
}

void CThreadProc::Stop(void)
{
    m_pFunc = NULL;
    m_pParam = NULL;
    m_bRun = false;
    //thread terminate function
    msleep(50);
    //terminate();
    quit();
    wait();
}

//thread run function
void CThreadProc::run()
{
    if(m_runFlag <= 0){
       while(m_bRun)
       {
           if(m_pFunc != NULL)
           {
               m_pFunc(m_pParam);
           }
          msleep(5);
       }
    }else{
        if(m_pFunc != NULL)
        {
            m_pFunc(m_pParam);
        }
    }
}
