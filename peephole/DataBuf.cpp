#include "DataBuf.h"

CDataBuf::CDataBuf(int memsz)
{
	m_pPacket = av_packet_alloc();
	if (memsz != 0) SetSize(memsz);
}

CDataBuf::~CDataBuf()
{
	av_packet_free(&m_pPacket);
}

int CDataBuf::GetCodecId(void)
{
	return m_codecid;
}

void CDataBuf::SetSize(int memSz)
{
	if (memSz == 0) return;

	if (m_pPacket->buf == NULL ||
		m_pPacket->buf->size < memSz + AV_INPUT_BUFFER_PADDING_SIZE)
	{
		int ret = av_buffer_realloc(&m_pPacket->buf, memSz + AV_INPUT_BUFFER_PADDING_SIZE);

		memset(m_pPacket->buf->data + memSz, 0, AV_INPUT_BUFFER_PADDING_SIZE);
	}
	m_pPacket->data = m_pPacket->buf->data; //refix data/size
	m_pPacket->size = memSz;
}

void CDataBuf::Release() //调用者通知: 不再使用Buffer
{
	{//check ref
		CScopeMutex lock(m_mutex);
		if (m_nRefs >= 2)
		{
			m_nRefs--;
			return;
		}
	}

	if (m_pPacket->buf != NULL) { //恢复工作
		m_pPacket->data = m_pPacket->buf->data;
		m_pPacket->size = m_pPacket->buf->size - AV_INPUT_BUFFER_PADDING_SIZE;
	}
	else {
		m_pPacket->data = 0;
		m_pPacket->size = 0;
	}
}

unsigned int CDataBuf::Skip(int i) 
{ 
	m_pPacket->data += i; 
	m_pPacket->size -= i; 
	return m_pPacket->size; 
}
