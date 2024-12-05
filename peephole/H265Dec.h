#ifndef _H265_DEC_H_
#define _H265_DEC_H_

#include "MediaDec.h"

class CH265Dec:public CMediaDec
{
public:
	CH265Dec(int width,int height);
	~CH265Dec();	
	int GetPixFmt(void);

	CDataBuf *Decode(CDataBuf *pSrc);
	int Decode(CDataBuf *pSrc, int dstFmt, uint8_t *dst);
protected:
	CDataBuf *DecYuv(CDataBuf *pSrc);
    int      DecRgbA(CDataBuf *pSrc,int dstFmt,uint8_t *rgbBuf);
private:
	CMutex  m_mutex;
	AVCodecContext *m_pCodecCtx;
	AVFrame *m_frame; //YUVÊý¾Ý
#if 1//def VIDEO_SF_DEC_TEST		//
	uint8_t              *m_pRgbBuf;
	AVFrame              *m_pFrameRGB;
	struct SwsContext    *m_pSwsCtx;
#endif	
};


#endif
