#ifndef _DATA_TYP_H_
#define _DATA_TYP_H_

#include "stdio.h"
#include "stdlib.h"
#include<stdint.h>

#define   IR_RGB_CHANNEL    3
#define   DEPTH_RGB_CHANNEL 3
#define   COLOR_RGB_CHANNEL 3
#define   HISTSIZE  0xFFFF
#define   ADB_RESP_BUFF_LEN 1024
#define   COM_CHANNEL_NUM 4

#define DEV_COM_COMM_LEN 256
#define DEV_COM_READ_LEN 256

#define CHANNEL_NUM 4

const int VIDEO_PARAM_NUM = 20;
const int AE_AREA_PARAM_NUM = 6;

typedef void (*ThreadProcFunc)(void* param);

typedef enum DEPTH_DRAW_COLOR_TYPE
{
	DEPTH_DRAW_COLOR_TYPE_LINEAR_HISTOGRAM = 0,
	DEPTH_DRAW_COLOR_TYPE_PSYCHEDELIC_SHADES,
	DEPTH_DRAW_COLOR_TYPE_PSYCHEDELIC,
	DEPTH_DRAW_COLOR_TYPE_RAINBOW,
	DEPTH_DRAW_COLOR_TYPE_CYCLIC_RAINBOW,
	DEPTH_DRAW_COLOR_TYPE_CYCLIC_RAINBOW_HISTOGRAM,
	DEPTH_DRAW_COLOR_TYPE_END
} DEPTH_DRAW_COLOR_TYPE;

typedef enum STREAM_SAVE_DATA_TYPE
{
	STREAM_SAVE_DATA_RAW = 0,
	STREAM_SAVE_DATA_BMP,
	STREAM_SAVE_DATA_PNG,
	STREAM_SAVE_DATA_JPEG,
	STREAM_SAVE_DATA_END
}STREAM_SAVE_DATA_TYPE;

typedef enum STREAM_TYPE
{
	STREAM_TYPE_DEPTH = 0,
	STREAM_TYPE_IR,
	STREAM_TYPE_COLOR,
	STREAM_TYPE_UVC,
	STREAM_TYPE_POINT_CLOUD,
	STREAM_TYPE_IR_PURE,
	STREAM_TYPE_D2C_RGB,
	STREAM_TYPE_END
}STREAM_TYPE;

typedef enum RENDER_DATA_TYPE
{
	RENDER_DATA_TYPE_RGB = 3,
	RENDER_DATA_TYPE_RGBA = 4,
	RENDER_DATA_TYPE_GRAY,
	RENDER_DATA_TYPE_YUV420,
	RENDER_DATA_TYPE_NV12,
	RENDER_DATA_TYPE_NV21,
	RENDER_DATA_TYPE_POINT_CLOUD,
	RENDER_DATA_TYPE_END
}RENDER_DATA_TYPE;

typedef enum SHOW_DATA_TYPE
{
	SHOW_DATA_TYPE_DEPTH = 0,
	SHOW_DATA_TYPE_IR = 1,
	SHOW_DATA_TYPE_COLOR = 2,
	SHOW_DATA_TYPE_PC = 3,
	SHOW_DATA_TYPE_END
}SHOW_DATA_TYPE;

typedef enum CAMERA_TYPE {
	CAMERA_TYPE_SINGLE = 0,
	CAMERA_TYPE_DUAL,
	CAMERA_TYPE_END
}CAMERA_TYPE;

typedef enum CAPTRUE_TYPE
{
	CAPTRUE_TYPE_TEC = 0,
	CAPTRUE_TYPE_TEMP,
	CAPTRUE_TYPE_ALIGMENT,
	CAPTRUE_TYPE_END
}CAPTRUE_TYPE;

typedef enum CAPTURE_DATA_TYPE
{
	CAPTURE_DATA_TYPE_NORMAL = 0,
	CAPTURE_DATA_TYPE_DELAY,
	CAPTURE_DATA_TYPE_CONTINUE,
	CAPTURE_DATA_TYPE_AE,
	CAPTURE_DATA_TYPE_TEMP,
	CAPTURE_DATA_TYPE_END
}CAPTURE_DATA_TYPE;

typedef enum CAPTURE_STA
{
	CAPTURE_STA_NO = 0,
	CAPTURE_STA_RUN,
	CAPTURE_STA_END
}CAPTURE_STA;

typedef enum RelayType
{
	RELAY_TYPE_A = 0,
	RELAY_TYPE_B,
	RELAY_TYPE_USB,
	RELAY_TYPE_END
}RelayType;

typedef enum SerialOpSta
{
	SERIAL_OP_STA_IDLE = 0,
	SERIAL_OP_STA_POWER_ON,
	SERIAL_OP_STA_POWER_OFF,
	SERIAL_OP_STA_END
}SerialOpSta;

typedef enum SerialProtocolTyp
{
	SERIAL_PROTOCOL_TYP_ORBBEC = 0,
	SERIAL_PROTOCOL_TYP_KAIDISHI, //凯迪仕
	SERIAL_PROTOCOL_TYP_DESHIMAN, //德施曼
	SERIAL_PROTOCOL_TYP_END
}SerialProtocolTyp;


typedef struct ComChnnInfo {
	bool bUse;
	int chnn;
	char sn[64];
	int comSta; //Serrial operation status
}ComChnnInfo;

typedef struct CaptureDataOp
{
	bool bCap; // capture flag
	char savePath[128]; //save path
	char ErrSavePath[128];//err save path
	int capType; //capture type
	int capSta;   //capture status
	int capCount;  //capture count from the cap
	int totalCapNum; //total capture num
	int curCapNum;   //current capture num
	int totaldelayNum;  //total delay capture num
	int curDelayNum;    //current delay capture num
	bool bSaveBmpName;
}CaptureDataOp;

typedef struct VideoParams {
	int width;
	int height;
	int fps;
	int unit;
}VideoParams;

typedef struct {
	unsigned char* pBuff;
	int nMaxLen;
	int len;
	int width;
	int height;
	int nDataType;  //RENDER_DATA_TYPE
}VideoBuff;

typedef struct World3D {
	float X;
	float Y;
	float Z;
}World3D;

typedef struct PixelColorVal {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
}PixelColorVal;

typedef struct PointCloudParam {
	int width;
	int height;
	World3D* pWordData;
	//PointCloudColor *pColor;
}PointCloudParam;

typedef struct RenderDataStuctType {

	int dataTyp; //RENDER_DATA_TYPE
	int width;
	int height;
	int datalen;
	void* data;  // Video Buffer data or point cloud data
}RenderDataType;

typedef struct ProductInfo {
	char vid[16];
	char pid[16];
	char name[64];
}ProductInfo;

typedef struct IR_Params
{
	IR_Params() :
		c_x(320.0), c_y(240.0), focal(480.0)
	{}

	IR_Params(float c_x_, float c_y_, float focal_) :
		c_x(c_x_), c_y(c_y_), focal(focal_)
	{}

	float c_x;
	float c_y;
	float focal;
}IR_Params;


typedef struct Tec_Param_Struct
{
	int tecCount;
	int tecInterval;
	int tecThreshold;
	int tecRange;
	int useTec;
	int useTecCount;
}Tec_Param_Struct;

typedef struct Serial_Comm_Struc
{
	unsigned char* comm;
	int  commlen;
}Serial_Comm_Struc;

typedef struct SerialProtocolCmdData
{
	uint8_t data[5 * 1024]; //1088
	uint32_t dataLen;
	uint32_t cmdlen; //parse cmd len
}SerialProtocolCmdData;

typedef struct
{
	int valid_flg; //识别结果
	uint32_t face_id;//最高分人脸id
	float similarity;//最高分相似度
	float desensitized_similarity;
	unsigned int face_num;
}face_recongnize_res;

typedef struct
{
	uint8_t level;
	float thresh;
}thresh_res;

#endif
