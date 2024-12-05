#ifndef _MEDIA_PLY_H_
#define _MEDIA_PLY_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#ifdef WIN32
#include <windows.h>
#endif

#include "MediaDec.h"
#include "StreamDataTyp.h"
#include "cdatadisply.h"
#include "RecFile.h"

class CMediaPly {
public:
	CMediaPly(int decTyp);
	~CMediaPly();

	void Play(char* url, uint8_t dstFmt, int width, int height, uint8_t showFmt = 0);
	void Stop(void);
	void Pause(void);
	void SnapPic(char* savePath, int fmt);
	void RecordCtl(bool bRec);
protected:
#ifdef WIN32
	static DWORD WINAPI PlyThreadProc(LPVOID lpParameter);
#endif
	void PlyProc(void);

private:
	//
	char m_url[256];
	uint8_t m_decTyp;
	//record
	CMutex m_mutexRec;
	bool m_bRec;
	CRecFile m_rec;
	//
	CMutex  m_mutexSnap;
	char m_picSavePath[256];
	bool m_bSnap;
	//
	uint8_t m_dstFmt;
	uint8_t m_showFmt;
	int m_width, m_height;

	//thread
	bool m_bPlyRun;
#ifdef WIN32
	HANDLE m_hThread;
#endif
	CMediaDec* m_pVideoDec;
	CDataDisply* m_pDataPly;
};

#endif
