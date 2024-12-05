#include "DataShow.h"
#include "GLDisply.h"
#include"CvDisply.h"

CDataShow::CDataShow()
{
	m_pDataPly = NULL;
	m_curPly = MediaShowFmtEnd;
}

CDataShow::~CDataShow()
{
	//if(m_pDataPly != NULL)
	//    delete m_pDataPly;
	//m_pDataPly = NULL;
}

CDataShow* CDataShow::Instance()
{
	static CDataShow mDataShow;
	return &mDataShow;
}

void CDataShow::SetDataPly(int typ, void* param)
{
	if (m_pDataPly != NULL)
		delete m_pDataPly;
	m_pDataPly = NULL;
	switch (typ)
	{
	case MediaShowFmtOpencv:
		m_pDataPly = new CCvDisply();
		break;
	case MediaShowFmtOpenGL:
		m_pDataPly = new CGlDisply((QWidget*)param);
		break;
	case MediaShowFmtQT:
		break;
	case MediaShowFmtOther:
		break;
	case MediaShowFmtNo:
		break;
	default:
		break;
	}
}

CDataDisply* CDataShow::GetDataPly(void)
{
	return m_pDataPly;
}

void CDataShow::SetShowArea(int x, int y, int width, int height)
{
	if (m_pDataPly != NULL)
	{
		m_pDataPly->SetDisplyArea(x, y, width, height);
	}
}

void CDataShow::UpdateData(int dataW, int dataH, int fmt, uint8_t* data)
{
	if (m_pDataPly != NULL)
	{
		m_pDataPly->DataDisply(dataW, dataH, fmt, data);
	}
}


