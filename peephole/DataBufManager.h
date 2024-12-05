#ifndef _DATA_BUF_MANAGER_H_
#define _DATA_BUF_MANAGER_H_

#include <list>
#include "DataBuf.h"

typedef enum DataListTyp {
	DataListTypIdle = 0,
	DataListTypDecSrc,
	DataListTypDec,
	DataListTypEnd
}DataListTyp;

class CDatalistBuf {
public:
	CDatalistBuf();
	~CDatalistBuf();

	int SetSzParam(int size, int memSize);
	int GetListSz(void);

	CDataBuf* GetIdle(void);
	CDataBuf* Pop(void);
	int Push(CDataBuf* dataBuf);

	void Clear(void);
protected:

private:
	CMutex              m_mtx;
	std::list<CDataBuf*> m_dataList;
	int  m_listSz;
	int  m_memSize;
};

class CDataBufManager
{
public:
	~CDataBufManager();
	static CDataBufManager* Instance(void);

	int SetSz(int typ, int size, int memSize);
	int GetSz(int typ);

	CDataBuf* GetIdle(int typ);
	CDataBuf* Pop(int typ);
	int Push(int typ, CDataBuf* dataBuf);

	void Clear(int typ);
	void ClearAll(void);
protected:
	CDataBufManager();
	CDatalistBuf m_dataList[DataListTypEnd];
};

#endif
