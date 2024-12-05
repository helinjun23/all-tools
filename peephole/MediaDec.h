#ifndef _MEDIA_DEC_H_
#define _MEDIA_DEC_H_

extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
#include <libavformat/version.h>
#include <libavutil/time.h>
#include <libavutil/mathematics.h>
}

#include "DataBufManager.h"


class CMediaDec {
public:
	CMediaDec() {}
	virtual ~CMediaDec() {}

	virtual int GetPixFmt(void) = 0;
	virtual CDataBuf *Decode(CDataBuf *pSrc)=0;
	virtual int Decode(CDataBuf *pSrc,int dstFmt,uint8_t *dst) = 0;

protected:

private:
  
};

#endif
