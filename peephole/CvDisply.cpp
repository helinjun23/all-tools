#include "CvDisply.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

CCvDisply::CCvDisply()
{

}

CCvDisply::~CCvDisply()
{

}

void CCvDisply::SetDisplyArea(int x, int y, int width, int height)
{

}

void CCvDisply::DataDisply(int width, int height, int fmt, uint8_t *data)
{
	cv::Mat showImg(height, width, CV_8UC4);
	
	showImg.data = data;

	cv::imshow("Show", showImg);
	cvWaitKey(5);
}