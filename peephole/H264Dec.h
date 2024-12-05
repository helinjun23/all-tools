#ifndef _H264_DEC_H_
#define _H264_DEC_H_

#include "MediaDec.h"

class CH264Dec:public CMediaDec
{
public:
	CH264Dec();
	~CH264Dec();

	CDataBuf *Decode(CDataBuf *pSrc);
	int Decode(CDataBuf *pSrc, int dstFmt, uint8_t *dst);
protected:

private:

};

#endif
