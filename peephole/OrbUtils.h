#ifndef _ORB_UTILS_H_
#define _ORB_UTILS_H_
#include<stdio.h>
#include<stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

void Bin2HexAscii(unsigned char *src,int srclen,unsigned char *ascii,int asciiLen,bool bUpperCase);
int Random(int min,int max);


#ifdef __cplusplus
}
#endif

#endif
