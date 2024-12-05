#ifndef THREADPROC_H
#define THREADPROC_H

#include <QThread>
#include "DataTyp.h"

class CThreadProc:public QThread
{
    //Q_OBJECT
public:
    CThreadProc(QObject *parent=0);
    ~CThreadProc();

    void Start(void *param,ThreadProcFunc pCbFunc,int flag=0);
    void Stop(void);

signals:
    void ActionSig(int action);
protected:
    //thread run function
    void run();
private:
    int m_runFlag;
    bool m_bRun;
    void *m_pParam;
    ThreadProcFunc m_pFunc;
};

#endif // THREADPROC_H
