#include "OrbUtils.h"

//
void Bin2HexAscii(unsigned char *src,int srclen,unsigned char *ascii,int asciiLen,bool bUpperCase)
{
    if(asciiLen < 2*srclen || srclen <= 0)
    {
        return;
    }
    int k=0;
    for(int i=0;i<srclen;i++)
    {
        unsigned char data = src[i];
        unsigned char low = data & 0x0F;
        unsigned char high = (data & 0xF0)>>4;
        unsigned char lowAsi=0,highAsi=0;
        //low
        if(low >= 0x0 && low <=0x9)
        {
             lowAsi = low + 0x30;
        }

        if(low >=0x0A && low <= 0x0F)
        {
            if(bUpperCase)
              lowAsi = low + 0x41;
            else
              lowAsi = low + 0x61;
            lowAsi -= 0x0A;
        }
        //high
        if(high >= 0x0 && high <= 0x9)
        {
             highAsi = high + 0x30;
        }

        if(high >=0x0A && high <= 0x0F)
        {
            if(bUpperCase)
              highAsi = high + 0x41;
            else
              highAsi = high + 0x61;
            highAsi -= 0x0A;
        }
        ascii[k]=highAsi;
        ascii[k+1]=lowAsi;
        k =k+2;
    }
}

int Random(int min,int max)
{
    int temp = 0;
    if (min > max)
    {
        temp = max;
        max = min;
        min = temp;
    }
    return rand() / (double)RAND_MAX *(max - min) + min;
}
