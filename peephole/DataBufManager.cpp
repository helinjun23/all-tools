#include "DataBufManager.h"

CDatalistBuf::CDatalistBuf():m_listSz(10),m_memSize(100000)
{

}

CDatalistBuf::~CDatalistBuf() 
{
	Clear();
}

int CDatalistBuf::SetSzParam(int size, int memSize)
{
	m_listSz = size;
    m_memSize = memSize;
	return 0;
}

int CDatalistBuf::GetListSz(void)
{
	int size = 0;
	m_mtx.Enter();
	size = m_dataList.size();
	m_mtx.Leave();
	return size;
}

void CDatalistBuf::Clear(void)
{
	m_mtx.Enter();
	for (std::list<CDataBuf*>::iterator it = m_dataList.begin(); it != m_dataList.end(); it++)
	{
		CDataBuf *pBuffer = *it;
		delete pBuffer;
	}
	m_mtx.Leave();
}

CDataBuf *CDatalistBuf::GetIdle(void)
{
	CDataBuf *pBuffer = NULL;
	m_mtx.Enter();
	if (m_dataList.empty() == false) {
		pBuffer = m_dataList.front();
		m_dataList.pop_front();
		//m_nSize[1] --;
	}
	m_mtx.Leave();

	if (pBuffer == NULL)
	{
		pBuffer = new CDataBuf(m_memSize);
	}
	else
	{
		pBuffer->SetSize(m_memSize);
	}

	return pBuffer;
}

CDataBuf *CDatalistBuf::Pop(void)
{
	CDataBuf *pBuffer = NULL;
	m_mtx.Enter();
	if (m_dataList.empty() == false) {
		pBuffer = m_dataList.front();
		m_dataList.pop_front();
		//m_nSize[1] --;
	}
	m_mtx.Leave();

	return pBuffer;
}

int CDatalistBuf::Push(CDataBuf *dataBuf)
{
	int res = -1;
	m_mtx.Enter();
	//if (m_dataList.size() < m_listSz)
	{
		m_dataList.push_back(dataBuf);
		//m_nSize[1] ++;
		dataBuf = NULL;
		res = 0;
	}
	m_mtx.Leave();

	return res;
}

CDataBufManager::CDataBufManager()
{

}

CDataBufManager::~CDataBufManager()
{
	m_dataList[DataListTypDec].Clear();
	m_dataList[DataListTypDecSrc].Clear();
}

CDataBufManager* CDataBufManager::Instance(void)
{
	static CDataBufManager mDataBuffMana;
	return &mDataBuffMana;
}

int CDataBufManager::SetSz(int typ, int size, int memSize)
{
	if (typ > DataListTypEnd)
	{
		return -1;
	}
	m_dataList[typ].SetSzParam(size, memSize);
	return 0;
}

int CDataBufManager::GetSz(int typ)
{
	if (typ > DataListTypEnd)
	{
		return 0;
	}
	return m_dataList[typ].GetListSz();
}


CDataBuf *CDataBufManager::GetIdle(int typ)
{
	if (typ > DataListTypEnd)
	{
		return NULL;
	}
	return m_dataList[typ].GetIdle();
}

CDataBuf *CDataBufManager::Pop(int typ)
{
	if (typ > DataListTypEnd)
	{
		return NULL;
	}
	return m_dataList[typ].Pop();
}

int CDataBufManager::Push(int typ, CDataBuf *dataBuf)
{
	if (typ > DataListTypEnd)
	{
		return -1;
	}

	return m_dataList[typ].Push(dataBuf);
}

void CDataBufManager::Clear(int typ)
{
	if (typ > DataListTypEnd)
	{
		return;
	}
	m_dataList[typ].Clear();
}

void CDataBufManager::ClearAll(void)
{
	for (uint8_t i = DataListTypIdle; i < DataListTypEnd; i++)
	{
		m_dataList[i].Clear();
	}
}

	
