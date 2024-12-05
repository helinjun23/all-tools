#ifndef _STREAM_PLY_H_
#define _STREAM_PLY_H_

#include <stdint.h>

extern "C" __declspec(dllexport) int Play(char *url, uint8_t dstFmt, uint8_t showFmt,int srcW,int srcH);
extern "C" __declspec(dllexport) int Stop(void);
extern "C" __declspec(dllexport) int Pause(void);
extern "C" __declspec(dllexport) int SnapPic(char *savePath,int fmt);
extern "C" __declspec(dllexport) int RecordCtl(bool bRec);

#endif
