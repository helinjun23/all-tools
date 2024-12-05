
#include "MediaPly.h"
#include "H265Dec.h"
#include <opencv2/opencv.hpp>
#include "FileHelper.h"
#include "CvDisply.h"
#include "ImageUtils.h"
#include "DataShow.h"
#include "clogcat.h"
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

CMediaPly::CMediaPly(int decTyp)
{
	//
	memset(m_url, 0, sizeof(m_url));
	m_decTyp = decTyp;
	//
	memset(m_picSavePath, 0, sizeof(m_picSavePath));
	m_bSnap = false;

	//
	m_dstFmt = MediaDstFmtRGBA;
	m_showFmt = MediaShowFmtOpencv;

	m_pDataPly = NULL;
	//thread
	m_bPlyRun = false;
#ifdef WIN32
	m_hThread = NULL;
#endif
}

CMediaPly::~CMediaPly()
{
	m_pDataPly = NULL;
	if (m_pVideoDec != NULL)
		delete m_pVideoDec;
	m_pVideoDec = NULL;
}

void CMediaPly::Play(char* url, uint8_t dstFmt, int width, int height, uint8_t showFmt)
{
	//save url
	memset(m_url, 0, sizeof(m_url));
	strcpy(m_url, url);

	//
	m_dstFmt = dstFmt;
	m_showFmt = showFmt;

	m_pDataPly = CDataShow::Instance()->GetDataPly();
	//param
	//int width = 1080, height = 1920;
	m_width = width;
	m_height = height;
	m_pVideoDec = new CH265Dec(width, height);

	//thread
	m_bPlyRun = true;
	//video frame thread
#ifdef WIN32
	m_hThread = CreateThread(NULL, 0, PlyThreadProc, this, 0, NULL);
#else

#endif
}

void CMediaPly::Stop(void)
{
	m_bPlyRun = false;
	//emit CLogcat::Instance()->ShowNoteInfo("111",0,true);
#ifdef WIN32
	Sleep(50);
	if (m_hThread != NULL)
	{
		TerminateThread(m_hThread, 12345);
		CloseHandle(m_hThread);
	}
	m_hThread = NULL;
#else

#endif
	//emit CLogcat::Instance()->ShowNoteInfo("222",0,true);
	if (m_pVideoDec != NULL)
		delete m_pVideoDec;
	m_pVideoDec = NULL;
	//emit CLogcat::Instance()->ShowNoteInfo("333",0,true);
	CDataBufManager::Instance()->Clear(DataListTypDecSrc);

	m_pDataPly->DataDisply(0, 0, RenderDataTypFmtRGBA, 0);
	//emit CLogcat::Instance()->ShowNoteInfo("444",0,true);
	//m_pDataPly = NULL;
}

void CMediaPly::Pause(void)
{

}

void CMediaPly::SnapPic(char* savePath, int fmt)
{
	m_mutexSnap.Enter();
	m_bSnap = true;
	strcpy(m_picSavePath, savePath);
	m_mutexSnap.Leave();
}

void CMediaPly::RecordCtl(bool bRec)
{
	char recPath[128] = { 0 };
	time_t rawtime;
	struct tm* timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	sprintf(recPath, "./rec_%d_%d_%d_%d_%d_%d.bin", timeinfo->tm_year, timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour,
		timeinfo->tm_min, timeinfo->tm_sec);

	m_mutexRec.Enter();
	m_bRec = bRec;
	if (m_bRec)
	{
		m_rec.Open(recPath, REC_FILE_WRITE2);
	}
	else {
		m_rec.Close();
	}
	m_mutexRec.Leave();
}

#ifdef WIN32
DWORD WINAPI CMediaPly::PlyThreadProc(LPVOID lpParameter)
{
	CMediaPly* pHandle = (CMediaPly*)lpParameter;
	if (pHandle != NULL)
	{
		pHandle->PlyProc();
	}
	return 0;
}
#else

#endif

void CMediaPly::PlyProc(void)
{
	int width = m_width, height = m_height;
	cv::Mat showImg(height, width, CV_8UC4);
	cv::Mat snapImg(height, width, CV_8UC4);
	while (m_bPlyRun)
	{
		if (CDataBufManager::Instance()->GetSz(DataListTypDecSrc) <= 5)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			//emit CLogcat::Instance()->ShowNoteInfo("buffer sz="+QString::number(CDataBufManager::Instance()->GetSz(DataListTypDecSrc)),0,true);
			continue;
		}

		//get buffer
		CDataBuf* pSrcData = CDataBufManager::Instance()->Pop(DataListTypDecSrc);
		if (NULL == pSrcData)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			continue;
		}

		m_mutexRec.Enter();
		if (m_bRec)
		{
			m_rec.Write2(pSrcData->GetPacket()->data, pSrcData->GetPacket()->size);
		}
		m_mutexRec.Leave();
		//if(CDataBufManager::Instance()->GetSz(DataListTypDecSrc) > 5)
			//printf("dec src sz=%d\n", CDataBufManager::Instance()->GetSz(DataListTypDecSrc));
		//emit CLogcat::Instance()->ShowNoteInfo(QString::number(CDataBufManager::Instance()->GetSz(DataListTypDecSrc)),0,true);
		//decode
		int res = m_pVideoDec->Decode(pSrcData, m_dstFmt, showImg.data);
		CDataBufManager::Instance()->Push(DataListTypIdle, pSrcData);
		if (res < 0)
		{
			//emit CLogcat::Instance()->ShowNoteInfo("Dec Data is NULL!",0,true);
			continue;
		}


		//display
#if 1
#if 0

#else
		if (m_pDataPly != NULL)
		{
			m_pDataPly->DataDisply(width, height, RenderDataTypFmtRGBA, showImg.data);
		}

		m_mutexSnap.Enter();
		if (m_bSnap)
		{
			m_bSnap = false;
			CImgUtil imgSave;
			cv::cvtColor(showImg, snapImg, cv::COLOR_RGBA2BGRA);
			imgSave.SaveBmp(snapImg.data, width, height, 4, m_picSavePath);
		}
		m_mutexSnap.Leave();
#endif
#else
		char yuvPath[128];
		sprintf(yuvPath, "%d.yuv", decPicNum++);
		FileHelper::write_file(yuvPath, (char*)pDecData->GetPacket()->data, pDecData->GetPacket()->size);
#endif
		//Sleep(30);
		//delete pDecData;
	}
}
