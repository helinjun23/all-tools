#ifndef _RTSP_CLIENT_H_
#define _RTSP_CLIENT_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"


class CRtspClient {
public:
	CRtspClient();
	~CRtspClient();

	int Open(char *url);
	int Close(void);

protected:
    void openURL(UsageEnvironment& env, char const* progName, char const* rtspURL);
#ifdef WIN32
	static DWORD WINAPI StreamThread(LPVOID lpParameter);
#endif
	void StreamProc(void);
private:
	char eventLoopWatchVariable;
	char m_url[256];

    //
    RTSPClient *m_pRtspClient;
	//thread
	bool m_bRun;
#ifdef WIN32
	HANDLE m_hThread;
#endif
};

#endif
