#ifndef _REC_FILE_H_
#define _REC_FILE_H_

//#include<File>
#include<stdio.h>
#include<stdlib.h>

#include <vector>
#include <memory>
#include<string.h>

typedef enum REC_FILE_TYPE
{
   REC_FILE_READ = 0,
   REC_FILE_WRITE,
   REC_FILE_WRITE2,
   REC_FILE_END
}REC_FILE_TYPE;

class CRecFile
{
public:
  CRecFile();
  ~CRecFile();
  
  int Open(const char *fileName,int type);
  int Close();
  
  int Read(char *readBuf,int readBufLen);
  int Write(void *data,int datalen);
  int Write2(void *data,int datalen);
  int WriteFile(char *fileName, const void *bufData, int bufLen);

  int GetFileLen();
   
protected:

private:
  FILE *m_pFile;
  int m_fileLen;
  char m_writeFileName[128];
};

#endif
