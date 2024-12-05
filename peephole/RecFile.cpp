#include "RecFile.h"


CRecFile::CRecFile()
{
   m_pFile = NULL;
   m_fileLen = 0;
   m_writeFileName[0] ='\0';
}

CRecFile::~CRecFile()
{
   Close();  
   m_pFile = NULL;
   m_fileLen = 0;
}

  
int CRecFile::Open(const char *fileName,int type)
{
   if(NULL == fileName)
   {
      return -1;
   } 
   
   if(REC_FILE_READ == type)
   {
     	m_pFile = fopen(fileName, "rb");
        m_fileLen = GetFileLen();     
   }
    
   if(REC_FILE_WRITE == type)
   {
        m_pFile = fopen(fileName, "ab+");
        strcpy(m_writeFileName,fileName);
        Close();
   }

   if(REC_FILE_WRITE2 == type)
   {
        m_pFile = fopen(fileName, "ab+");
        strcpy(m_writeFileName,fileName);
   }

   if(NULL == m_pFile)
   {
      return -1;
   }

   return 0; 
}

int CRecFile::Close()
{
   if(m_pFile != NULL)
   {
      fclose(m_pFile); 
   }
   m_pFile = NULL;
   return 0;
}
  
int CRecFile::Read(char *readBuf,int readBufLen)
{
    if(NULL == m_pFile || NULL == readBuf || 0 == readBufLen)
    {
        return -1;
    }
    
    int readLen = 0;
    if(readBufLen >= m_fileLen)
    {
        readLen = m_fileLen;
    }
    else
    {
        readLen = readBufLen;
    }
    
    int ret= fread(readBuf, readLen, 1, m_pFile);
    
    return ret;
}

int CRecFile::Write2(void *data,int datalen)
{
   if(NULL == m_pFile)
   {
      return -1;
   }
   int len = fwrite((const char*)data, sizeof(char)*datalen, 1, m_pFile); 
   return len;
}



int CRecFile::Write(void *data,int datalen)
{
   if(m_writeFileName[0] == 0)
   {
        return -1;
    }
   m_pFile = fopen(m_writeFileName, "a+");
   if(NULL == m_pFile)
   {
      return -1;
   }
   int len = fwrite((const char*)data, sizeof(char)*datalen, 1, m_pFile); 
   Close();
   return len;
}

int  CRecFile::WriteFile(char *fileName, const void *bufData, int bufLen)
{
    if ('\0' == fileName[0] || NULL == bufData || 0 == bufLen)
    {
        return -2;
    }

    FILE *pFile = NULL;
    pFile = fopen(fileName, "a+");
    if (NULL == pFile)
    {
        return -1;
    }
    int len = fwrite(bufData, sizeof(char), bufLen, pFile);

    fclose(pFile);
    pFile = NULL;
    return 0;
}

int CRecFile::GetFileLen()
{
   if(NULL == m_pFile)
   {
       return -1;
   }
   
   int len = 0;
   fseek(m_pFile, 0, SEEK_END);   
   len = ftell(m_pFile);	   
   fseek(m_pFile, 0, SEEK_SET);
   
   return len;
}
