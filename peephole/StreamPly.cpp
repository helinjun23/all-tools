#include "StreamPly.h"
#include "RtspClient.h"
#include "MediaPly.h"
#include "clogcat.h"

CRtspClient rtspConn;
CMediaPly* gMediaPly = NULL;

int Play(char* url, uint8_t dstFmt, uint8_t showFmt, int srcW, int srcH)
{
	//emit CLogcat::Instance()->ShowNoteInfo("before rtsp open",0,true);
	rtspConn.Open(url);
	//emit CLogcat::Instance()->ShowNoteInfo("after rtsp open",0,true);
	if (gMediaPly != NULL)
		delete gMediaPly;
	gMediaPly = new CMediaPly(MediaDecTypSoft);
	if (gMediaPly != NULL)
		gMediaPly->Play(url, dstFmt, srcW, srcH, showFmt);
	//emit CLogcat::Instance()->ShowNoteInfo("after play",0,true);
	return 0;
}

int Stop(void)
{
	rtspConn.Close();

	Sleep(200);

	if (gMediaPly != NULL)
	{
		gMediaPly->Stop();
		delete gMediaPly;
	}
	gMediaPly = NULL;
	//emit CLogcat::Instance()->ShowNoteInfo("666",0,true);
	return 0;
}

int Pause(void)
{
	return 0;
}

int SnapPic(char* savePath, int fmt)
{
	if (gMediaPly != NULL)
		gMediaPly->SnapPic(savePath, fmt);
	return 0;
}

int RecordCtl(bool bRec)
{
	if (gMediaPly != NULL)
		gMediaPly->RecordCtl(bRec);
	return 0;
}

