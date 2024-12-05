#ifndef _H265_DEC_HW_H_
#define _H265_DEC_HW_H_

#include "MediaDec.h"

class CH265DecHw:public CMediaDec
{
public:
    CH265DecHw();
    ~CH265DecHw();

    int GetPixFmt(void);
    CDataBuf *Decode(CDataBuf *pSrc);
    int Decode(CDataBuf *pSrc,int dstFmt,uint8_t *dst);
protected:

private:

};

#endif
