#ifndef _CV_DISPLY_H_
#define _CV_DISPLY_H_

#include "cdatadisply.h"

class CCvDisply:public CDataDisply
{
public:
	CCvDisply();
	~CCvDisply();
	
	void SetDisplyArea(int x, int y, int width, int height);
	void DataDisply(int width, int height, int fmt, uint8_t *data);
protected:
private:

};

#endif