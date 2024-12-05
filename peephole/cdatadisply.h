#ifndef CDATADISPLY_H
#define CDATADISPLY_H

#include <stdint.h>



class CDataDisply{
public:
    CDataDisply() {}
    virtual ~CDataDisply() {}

    virtual void SetDisplyArea(int x,int y,int width,int height)=0;
    virtual void DataDisply(int width,int height,int fmt,uint8_t *data)=0;
protected:

private:

};
#endif // CDATADISPLY_H
