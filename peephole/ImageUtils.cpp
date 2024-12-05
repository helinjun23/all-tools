
#include "ImageUtils.h"

CImgUtil::CImgUtil()
{
}
CImgUtil::~CImgUtil()
{

}
void CImgUtil::convertRaw16ToRGB(const void* src, char* dst, int w, int h){


	for (int i = 0; i < h; i++)
	{
		uint16_t* psrc = ((uint16_t*)src) + (w * i);
		RGB888* p_rgb = ((RGB888*)dst) + (w * i);
		for (int j = 0; j < w; j++){
			uint8_t value = (psrc[j] >> 2) & 0xff;
			p_rgb[j].R = value;
			p_rgb[j].G = value;
			p_rgb[j].B = value;
		}

	}

}

void CImgUtil::convertRaw16ToGray(const void* src, cv::Mat &dst){

	for (int i = 0; i < dst.rows; i++)
	{
		uint16_t* psrc = ((uint16_t*)src) + (dst.cols * i);
		uint8_t* p_gray = ((uint8_t*)dst.data) + (dst.cols * i);
		for (int j = 0; j < dst.cols; j++){
			uint8_t value = (psrc[j] >> 2) & 0xff;
			p_gray[j] = value;

		}
	}
}
int CImgUtil::SaveBmpGray(cv::Mat &src, char *path)
{
	if (NULL == path)
	{
		return -1;
	}

	cv::imwrite(path, src);
	
	return 0;
}

int CImgUtil::SaveBmpGray(uint16_t *data, int w, int h, char *path)
{
	if (NULL == path)
	{
		return -1;
	}
	cv::Mat dst(h,w, CV_8UC1);
	convertRaw16ToGray(data, dst);
	cv::imwrite(path, dst);

	return 0;
}

int CImgUtil::SaveBmpGray(uint8_t *data, int w, int h, char *path)
{
	if (NULL == path)
	{
		return -1;
	}

	cv::Mat mat(h, w, CV_8UC1);
	memcpy(mat.data, data, h*w * sizeof(char));
	cv::imwrite(path, mat);

	return 0;
}

int CImgUtil::SaveBmp(unsigned char *data, int width, int height,int channel, char *path)
{
	if (NULL == path)
	{
		return -1;
	}

	if(3== channel)
	{
		cv::Mat mat(height, width, CV_8UC3);
	       memcpy(mat.data, data, height*width * 3 * sizeof(char));
	       cv::imwrite(path, mat);
      }
        if(4==channel)
	{

		cv::Mat mat(height, width, CV_8UC4);
	       memcpy(mat.data, data, height*width * 4 * sizeof(char));
	       cv::imwrite(path, mat);
	}
	return 0;
}

int  CImgUtil::SavePng(uint16_t *data, int width, int height, char *path)
{
	if (NULL == path)
	{
		return -1;
	}

	cv::Mat cv16mat(height, width, CV_16UC1, data);


	std::vector<int> params;
	params.push_back(cv::IMWRITE_PNG_COMPRESSION);
	params.push_back(0);
	cv::imwrite(path, cv16mat, params);

	return 0;
}

int  CImgUtil::SavePng(uint8_t *data, int width, int height, char *path)
{
	if (NULL == path)
	{
		return -1;
	}

	cv::Mat cv8mat(height, width, CV_8UC1, data);
	std::vector<int> params;
	params.push_back(cv::IMWRITE_PNG_COMPRESSION);
	params.push_back(0);
	cv::imwrite(path, cv8mat, params);

	return 0;
}

int  CImgUtil::SaveRawData(char *fileName, const void *bufData, int bufLen)
{
    if (NULL == fileName || NULL == bufData || 0 == bufLen)
    {
        return -1;
    }


    FILE *pFile = NULL;
    pFile = fopen(fileName, "wb");
    if (NULL == pFile)
    {
        return -1;
    }
    int len = fwrite(bufData, sizeof(char), bufLen, pFile);
    if(len <= 0){
        fclose(pFile);
        return -1;
    }
    //note.Format(_T("dateLen = %d,writeLen=%d"), bufLen,len);
    //ShowDebugInfo(note);

    fclose(pFile);
    pFile = NULL;
    return 0;
}

int CImgUtil::SaveJpg(char *fileName, unsigned char *data, int width, int height, int channel)
{
    if (NULL == fileName)
    {
        return -1;
    }

    const int JPEG_QUALITY = 80;

    //cv::Mat cv8mat(height, width, CV_8UC3, data);
    cv::Mat *pMat = NULL;
    if(3==channel)
    {
        pMat= new cv::Mat(height, width, CV_8UC3, data);
    }

    if(4 == channel)
    {
        pMat= new cv::Mat(height, width, CV_8UC4, data);
    }

    std::vector<int> params;
    params.push_back(CV_IMWRITE_JPEG_QUALITY);
    params.push_back(JPEG_QUALITY);

    cv::imwrite(fileName, *pMat, params);
    //cv::imwrite(fileName, cv8mat, params);

    if(pMat != NULL)
    {
        delete pMat;
        pMat = NULL;
    }
    return 0;
}

int CImgUtil::SaveJpg(char *fileName, cv::Mat &src)
{
    if (NULL == fileName)
    {
        return -1;
    }

    const int JPEG_QUALITY = 80;
    std::vector<int> params;
    params.push_back(CV_IMWRITE_JPEG_QUALITY);
    params.push_back(JPEG_QUALITY);
    cv::imwrite(fileName, src, params);

    return 0;
}
