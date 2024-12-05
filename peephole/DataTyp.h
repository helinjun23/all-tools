#ifndef _DATA_TYP_H_
#define _DATA_TYP_H_

#include <stdint.h>
#include "stdio.h"
#include "stdlib.h"

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

#if 0
typedef enum SerialProtocolTyp
{
	SERIAL_PROTOCOL_TYP_ORBBEC = 0,
	SERIAL_PROTOCOL_TYP_KAIDISHI, //凯迪仕
	SERIAL_PROTOCOL_TYP_DESHIMAN, //德施曼
	SERIAL_PROTOCOL_TYP_END
}SerialProtocolTyp;
#endif

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


typedef struct Serial_Comm_Struc
{
	unsigned char* comm;
	int  commlen;
}Serial_Comm_Struc;


enum PeepHoleCmdTyp {
	PeepHoleCmdTypUartPrepare = 0x90,
	PeepHoleCmdTypWifiStaNote = 0xc5,
	PeepHoleCmdTypPirCtl = 0xc7,
	PeepHoleCmdTypAlarmEvent = 0xca,
	PeepHoleCmdTypPicDir = 0xd1,
	PeepHoleCmdTypAliKey = 0xd5,
	PeepHoleCmdTypWifiSta = 0xd6,
	PeepHoleCmdTypMac = 0xd7,
	PeepHoleCmdTypSsidPwd = 0xd8,
	PeepHoleCmdTypDataTrans = 0xd9,
	PeepHoleCmdTypDevName = 0xda,
	PeepHoleCmdTypIP = 0xdb,
	PeepHoleCmdTypWakeNote = 0xdc,
	PeepHoleCmdTypSleep = 0xde,
	PeepHoleCmdTypIpMask,
	PeepHoleCmdTypIPGate,
	PeepHoleCmdTypWifiStaDb,
	PeepHoleCmdTypWifiStaSNR,
	PeepHoleCmdTypNetTime,
	PeepHoleCmdTypVersion,
	PeepHoleCmdTypDeviceSN,
	PeepholeOrbCmdTypDevSta,
	PeepHoleOrbCmdTypGetPersonSta,
	PeepHoleOrbCmdTypGetMaskSta,
	PeepholeOrbCmdTypGetCloudKeyProductKey,
	PeepholeOrbCmdTypGetCloudKeyProductSecret,
	PeepholeOrbCmdTypGetCloudKeyDeviceName,
	PeepholeOrbCmdTypGetCloudKeyDeviceSecret,
	PeepHoleCmdTypEnd
};
#if 0
enum WifiStaTyp {
	WifiStaTypConfNet = 0,
	WifiStaTypConnectHot = 1,
	WifiStaTypConnectSucc = 2,
	WifiStaTypConfNetFail = 3,
	WifiStaTypNetNoUse = 7,
	WifiStaTypEnd
};
#endif

#if 0
enum PeepholeOrbCmdTyp22 {
	PeepholeOrbCmdTypSNGet = 0x0101,
	PeepholeOrbCmdTypSNSet = 0x0102,
	PeepholeOrbCmdTypSysVer = 0x0103,
	PeepholeOrbCmdTypWifiVer = 0x0104,
	PeepholeOrbCmdTypReset = 0x0105,
	PeepholeOrbCmdTypFactoryMode = 0x0106,
	PeepholeOrbCmdTypTF = 0x0107,
	PeepholeOrbCmdTypSensorSta = 0x0108,
	PeepholeOrbCmdTypLedCtl = 0x0109,
	PeepholeOrbCmdTypWireIp = 0x010a,
	PeepholeOrbCmdTypPirDistGet = 0x0201,
	PeepholeOrbCmdTypPirDistSet = 0x0202,
	PeepholeOrbCmdTypPirCtl = 0x0203,
	PeepholeOrbCmdTypPirSta = 0x0204,
	PeepholeOrbCmdTypPirTimeGet = 0x0205,
	PeepholeOrbCmdTypPirTimeSet = 0x0206,
	PeepholeOrbCmdTypMeidaAudioRec = 0x0301,
	PeepholeOrbCmdTypMeidaAudioPlay = 0x0302,
	PeepholeOrbCmdTypMeidaStreamUrl = 0x0303,
	PeepholeOrbCmdTypMeidaStreamOpen = 0x0304,
	PeepholeOrbCmdTypMeidaStreamClose = 0x0305,
	PeepholeOrbCmdTypMeidaStreamFrmCount = 0x0306,
	PeepholeOrbCmdTypWifiApGet = 0x0401,
	PeepholeOrbCmdTypWifiApSet = 0x0402,
	PeepholeOrbCmdTypWifiCtl = 0x0403,
	PeepholeOrbCmdTypWifiSta = 0x0404,
	PeepholeOrbCmdTypIrCutCtl = 0x0501,
	PeepholeOrbCmdTypIrCutSta = 0x0502,
	PeepholeOrbCmdTypLightSensitiveVal = 0x0601,
	PeepholeOrbCmdTypLightSensitiveCtl = 0x0602,
	PeepholeOrbCmdTypEnd
};
#endif

typedef struct SerialCmd {
	unsigned char cmd[2028];
	uint32_t cmdlen;
	int cmdTyp;  //PeepHoleCmdTyp
}SerialCmd;

#if 0
typedef struct SerialProtocolCmdData
{
	uint8_t data[2048];
	uint32_t dataLen;
}SerialProtocolCmdData;
#endif

enum event_action_typ {
	event_action_typ_record = 0,
	event_action_typ_snap = 1,
	event_action_typ_end
};
typedef struct MediaCapEventParam {
	//uint8_t mediaTyp;
	//uint32_t duration;
	//uint8_t  capEnable;
	//uint8_t  cloudSave;
	uint16_t eventid;
	uint8_t  event;
	uint8_t  eventtyp;
	uint8_t  opentyp;
	uint8_t  userid;
	uint8_t  action; //record or snap
	uint8_t  actionParam;
	uint32_t ts;
	uint8_t  offline;
}MediaCapEventParam;


typedef struct SerialCmdOrb {
	uint8_t hLow;
	uint8_t hHigh;
	uint8_t source;
	uint8_t typ;
	uint32_t len;
	uint8_t  data[1024];
	uint16_t crc;
}SerialCmdOrb;

enum EventAlarmTyp
{
	event_alarm_typ_freeze = 0x01,
	event_alarm_typ_alarm = 0x02,
	event_alarm_typ_event = 0x03,
	event_alarm_typ_battery = 0x04,
	event_alarm_typ_end
};

enum EventFreezeTyp {
	event_freeze_typ_none = 0x00,
	event_freeze_typ_print = 0x01,
	event_freeze_typ_pwd,
	event_freeze_typ_card,
	event_freeze_typ_face,
	event_freeze_typ_finger_vein,
	event_freeze_typ_end
};

enum AlarmTyp
{
	alarm_typ_none = 0x00,
	alarm_typ_tamper, //撬锁
	alarm_typ_break_in,//非法闯入
	alarm_typ_try_ban, //禁试
	alarm_typ_fake_lock, //假锁
	alarm_typ_defence, //一键布防
	alarm_typ_peep_timeout, //超时窥视
	alarm_typ_lock_exception,//锁体异常
	alarm_typ_hijack_finger_print,//劫持指纹
	alarm_typ_hijack_pwd, //劫持密码
	alarm_typ_door_not_close, //
	alarm_typ_door_mask,
	alarm_typ_move_detect,
	alarm_typ_hijack_face,
	alarm_typ_hijack_finger_vein,
	alarm_typ_sprint_bolt_not_in,
	alarm_typ_spring_bolt_not_out,
	alarm_typ_face_err,
	alarm_typ_finger_err,
	alarm_typ_back_board_err,
	alarm_typ_end
};

enum EventTyp {
	event_typ_none = 0x00,
	event_typ_door_bell,
	event_typ_door_open,
	event_typ_door_close,
	event_typ_door_anti_lock,
	event_typ_door_cancel,
	event_typ_door_reset,
	event_typ_door_try_err,
	event_typ_leave_home_open,
	event_typ_leave_home_close,
	event_typ_out_remind,
	event_typ_remote_open_success,
	event_typ_remote_open_fail,
	evnet_typ_temp_pwd_open_success,
	evnet_typ_temp_pwd_open_fail,
	evnet_typ_door_open_fail,
	event_typ_end
};

enum LockOpenTyp {
	lock_open_typ_none = 0x00,
	lock_open_typ_finger_print,
	lock_open_typ_pwd,
	lock_open_typ_card,
	lock_open_typ_face,
	lock_open_typ_finger_vein,
	lock_open_typ_tmp_pwd,
	lock_open_typ_app,
	lock_open_typ_iris,
	lock_open_typ_key,
	lock_open_typ_finger_pwd,
	lock_open_typ_face_finger,
	lock_open_typ_finger_vein_pwd,
	lock_open_typ_face_finger_vein,
	lock_open_typ_in_door,
	lock_open_typ_face_pwd,
	lock_open_typ_limit_time_pwd,
	lock_open_typ_out_door,
	lock_open_typ_palmvein,
	lock_open_typ_end
};


enum EventAlarmActionTyp
{
	event_alarm_action_typ_none = 0x00,
	event_alarm_action_typ_rec_video,
	event_alarm_action_typ_rec_audio,
	event_alarm_action_typ_rec_av,
	event_alarm_action_typ_snap,
	event_alarm_action_typ_cloud,
	event_alarm_action_typ_end
};

enum LockEventTyp {
	LOCK_EVENT_TYP_DOOR_BELL = 0x00,
	LOCK_EVENT_TYP_ANTI_PRIYING,
	LOCK_EVENT_TYP_BAN_TRY,
	LOCK_EVENT_TYP_PROTECTION,
	LOCK_EVENT_TYP_FAKE_LOCK,
	LOCK_EVENT_TYP_RESET,
};
#endif
