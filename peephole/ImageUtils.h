#pragma once

#include <stdint.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


typedef struct RGB888{
	uint8_t R;
	uint8_t G;
	uint8_t B;
}RGB888;

class CImgUtil
{
public:
	CImgUtil();
	~CImgUtil();

	void convertRaw16ToGray(const void* src, cv::Mat &dst);
	void convertRaw16ToRGB(const void* src, char* dst, int w, int h);

	int SaveBmpGray(cv::Mat &src,char *path);
	int SaveBmpGray(uint16_t *data,int w,int h, char *path);
	int SaveBmpGray(uint8_t *data,int w,int h,char *path);
	int  SaveBmp(unsigned char *data, int width, int height,int channel, char *path);
	
	int  SavePng(uint16_t *data, int width, int height, char *path);
	int  SavePng(uint8_t *data,int widht,int height,char *path);

    int  SaveRawData(char *fileName, const void *bufData, int bufLen);

    int SaveJpg(char *fileName,unsigned char *data,int width,int height,int channel);
    int SaveJpg(char *fileName, cv::Mat &src);
protected:

private:

};
