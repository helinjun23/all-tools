#ifndef _DATA_BUF_H_
#define _DATA_BUF_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/opt.h>
#ifdef __cplusplus
}
#endif

#include "common.h"

class CDataBuf {
public:
	CDataBuf(int memsz);
	~CDataBuf();

	int GetCodecId(void);
	void SetSize(int memSz);
   
	CDataBuf *AddRef() { CScopeMutex lock(m_mutex); m_nRefs++; return this; }
	void Release(); //调用者通知: 不再使用Buffer
	unsigned int Skip(int i);
	AVPacket    *GetPacket() const { return m_pPacket; }
protected:

private:
	int m_memSz;
	int m_codecid; //for recorder
	AVPacket  *m_pPacket;
	CMutex    m_mutex;
	int       m_nRefs;
};

class CLocalBuf {
public:
	CLocalBuf(CDataBuf *pData=NULL):m_pDataBuf(pData) {}
	~CLocalBuf() {
		if (m_pDataBuf != NULL)
			delete m_pDataBuf;
		m_pDataBuf = NULL;
	}
    void  Attach(CDataBuf *pBuffer) { if (m_pDataBuf) m_pDataBuf->Release();
        m_pDataBuf = pBuffer; }
	CDataBuf *Detach()
	{
		CDataBuf *pBuffer = m_pDataBuf;
		m_pDataBuf = NULL;
		return pBuffer;
	}

	void  Release(void)
	{
		if (m_pDataBuf)
		{
			m_pDataBuf->Release();
			m_pDataBuf = NULL;
		}
	}

	CDataBuf *operator->() { return m_pDataBuf; }

protected:

private:
	CDataBuf *m_pDataBuf;
};
#endif
