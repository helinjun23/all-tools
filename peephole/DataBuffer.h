#ifndef _DATA_BUFFER_H_
#define _DATA_BUFFER_H_

#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

class CDataBuffer
{
public:
    explicit CDataBuffer(uint32_t size):m_bufferSize(size),m_offset(0),m_dataLen(0) {
        m_pBuffer = new uint8_t[m_bufferSize];
        if(m_pBuffer != nullptr)
            memset(m_pBuffer,0,sizeof(uint8_t)*m_bufferSize);
    }
    CDataBuffer(const CDataBuffer& buffer)
    {
        Reset(buffer);      
    }
    ~CDataBuffer() {
        m_bufferSize =0;
        m_offset = 0;
        m_dataLen = 0;
        if(m_pBuffer != nullptr)
            delete m_pBuffer;
        m_pBuffer = nullptr;
    }

    CDataBuffer& operator=(const CDataBuffer& buffer){
        if(this == &buffer)
            return *this;
        //reset 
        Reset(buffer);
        return *this;
    }

    uint32_t GetBufferSize(void) const {
         return m_bufferSize;
    }

    uint32_t GetDataLen(void) const {
        return m_dataLen;
    }
    uint32_t GetOffset(void) const {
        return m_offset;
    }
    uint8_t *GetData(void) const {
        return m_pBuffer;
    }

    uint32_t AddData(const uint8_t* data,const uint32_t datalen)
    {
        if(m_dataLen + datalen > m_bufferSize)
            return 0;
        memcpy(m_pBuffer + m_offset,data,datalen * sizeof(uint8_t));
        m_dataLen += datalen;
        m_offset  += datalen;
        return m_dataLen;
    }

    uint32_t AddData(const uint8_t data)
    {
        m_pBuffer[m_offset] = data;
        m_offset++;
        m_dataLen++;
        return m_dataLen;
    }
    
    uint8_t* Skip(int pos){
        int position = 0;
        uint8_t *pTmp= nullptr;
        if(m_offset > 0)
        {
            position = m_offset + pos;
            m_pBuffer += position;
        }
        pTmp = m_pBuffer;

        return pTmp;
    }


protected:
    void Reset(const CDataBuffer& buffer)
    {
        m_offset = 0;
        m_dataLen = 0;
        if(buffer.GetBufferSize() > m_bufferSize)
        {
              delete[] m_pBuffer;
              m_pBuffer = nullptr;
              m_bufferSize=0;
              
              m_bufferSize = buffer.GetBufferSize();
              m_pBuffer = new uint8_t[m_bufferSize];
              if(m_pBuffer != nullptr)
                  memset(m_pBuffer,0,sizeof(uint8_t)*m_bufferSize);              
        }
        else
        {
            memset(m_pBuffer,0,sizeof(uint8_t)*m_bufferSize);
        }

        memcpy(m_pBuffer,buffer.GetData(),buffer.GetBufferSize());        
    }
private:
    uint8_t *m_pBuffer;
    uint32_t m_bufferSize;
    uint32_t m_offset;
    uint32_t m_dataLen;
};


#endif
