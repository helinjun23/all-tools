#ifndef _DATA_SHOW_H_
#define _DATA_SHOW_H_

#include "cdatadisply.h"
#include "StreamDataTyp.h"

class CDataShow
{
public:
    ~CDataShow();

    static CDataShow *Instance();
    void SetDataPly(int typ,void *param);
    CDataDisply *GetDataPly(void);

    void SetShowArea(int x,int y,int width,int height);
    void UpdateData(int dataW,int dataH,int fmt,uint8_t *data);
protected:
    CDataShow();
private:
    CDataDisply *m_pDataPly;
    int m_curPly;
};

#endif
