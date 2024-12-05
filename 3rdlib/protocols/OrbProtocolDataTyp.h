#ifndef _ORB_PROTOCOL_DATA_TYP_H_
#define _ORB_PROTOCOL_DATA_TYP_H_

#include <stdint.h>

#define ORB_PROTOCOL_TAG_LOW   0xF5
#define ORB_PROTOCOL_TAG_HIGH  0xAE

#define LK_PROTOCOL_TAG_LOW   0xEF
#define LK_PROTOCOL_TAG_HIGH  0xAA

const uint16_t OB_PACKET_HEAD = 0xAEF5;
enum OrbProtocolSource {
	OrbProtocolSourceHost = 0x01,
	OrbProtocolSourceSlave = 0x02,
	OrbProtocolSourceEnd
};

enum OrbProtocolPktTyp {
	OrbProtocolPktTypCmd = 0x01,
	OrbProtocolPktTypData = 0x02,
	OrbProtocolPktTypLookUp = 0x03,
	OrbProtocolPktTypErr = 0x04,
	OrbProtocolPktTypOTA = 0x05,
	OrbProtocolPktTypPeepholeEncryp = 0xFD,
	OrbProtocolPktTypPeepholeComm = 0xFE,
	OrbProtocolPktTypPeepholeFac = 0xFF,
	OrbProtocolPktTypEnd
};

enum OrbProtocolPktErrTyp {
	Err_CrcCheck = 0x01,
	Err_DevType = 0x02,
	Err_PktFlag = 0x04,
	Err_DataType = 0x08,
	Err_Reverse_1 = 0x10,
	Err_Reverse_2 = 0x20,
	Err_Reverse_3 = 0x40,
	Err_Reverse_4 = 0x80,
};

enum PeepholeOrbCmdTyp {
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
	PeepholeOrbCmdTypPirThreshGet = 0x0201,
	PeepholeOrbCmdTypPirThreshSet = 0x0202,
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

enum PeepholeOrbCmdResultTyp {
	PeepholeOrbCmdResultTypFail = 0x00,
	PeepholeOrbCmdResultTypSucc = 0x01,
	PeepholeOrbCmdResultTypEnd
};

constexpr uint32_t ORBCMD_MAXSIZE = 2048;
typedef struct OrbProtocolCmd {
	uint8_t hLow;
	uint8_t hHigh;
	uint8_t source;
	uint8_t typ;
	uint32_t len;
	uint8_t  data[ORBCMD_MAXSIZE]; //main id + sub id + data
	uint16_t crc;
}OrbProtocolCmd;

enum OrbProtocolCmdType
{
	SET_RECONG_THR = 0xB2,          // 设置识别阈值
	GET_RECONG_THR = 0xB3,          // 查询识别阈值
	RESET_RECONG_THR = 0xB7,        // 重置识别阈值
	SET_RECONG_LEVEL = 0xD4,        // 设置识别阈值档位
	GET_RECONG_LEVEL = 0xE0,        // 查询识别阈值档位
	RESET_RECONG_LEVEL = 0xD5,      // 重置识别阈值档位
	SET_LIVING_THR = 0xD8,          // 设置活体阈值
	GET_LIVING_THR = 0xE3,          // 获取活体阈值
	SET_LIVING_LEVEL = 0xD6,        // 设置活体阈值档位
	GET_LIVING_LEVEL = 0xE2,        // 查询活体阈值挡位
	RESET_LIVING_LEVEL = 0xD7,      // 重置活体阈值挡位
	SET_ADDITONREC_THR = 0xD0,      // 设置补录阈值
	GET_ADDITONREC_THR = 0xE1,      // 查询补录阈值
	RESET_ADDITONREC_THR = 0xD2,    // 重置补录阈值
	SET_REPEAT_REGISTER = 0xBE,     // 设置重复注册
	GET_REPEAT_REGISTER = 0xBF,     // 查询重复注册
	GET_ALL_REGISTER_ID = 0x50,     // 查询所有注册ID
	REGISTER_FACE_ID = 0x01,        // 注册人脸ID
	REGISTER_FACE_ID_INTER = 0x57,  // 注册人脸ID中间状态
	REGISTER_FACE_ID_INTER_CONTINUE = 0xA0, // 注册人脸ID中间状态继续
	DELETE_FACE_ID = 0x02,          // 删除人脸ID
	FACE_RECONG = 0x40,             // 人脸识别

	GET_ALGBOARD_SWVER = 0x61,      // 查询算法板软件版本号
	GET_OBMODEL_VER = 0x62,         // 查询奥比摄像头模组版本号
	GET_SNAP_PICTURE = 0x73,        // 抓取一张JPEG格式的图片
	UP_SNAP_PICTURE_INFO = 0x77,        // 告知上位机图片文件的信息

	FACE_DEMO_MODE = 0x51,          // 演示识别

	// 加密协议相关
	SET_DEBUG_ENC_KEY = 0xF0,       // 设置Debug Key
	SET_RELEASE_ENC_KEY = 0xF1,     // 设置Release Key
	INIT_ENC_KEY = 0xF3,            // 初始化随机因子
	EXIT_ENC_MODE = 0xF4,           // 退出加密模式
	CRYPT_RESET = 0xBD,             // 恢复出厂设置，清除加密信息

	GET_CAPTURE_FACE_FEATURE = 0xA9,    // 抓拍人脸并提取特征数组
	REGISTER_FACE_BY_FEATURE = 0xA8,// 下发特征值注册

	SET_PROTOCOL_TYPE = 0xDA,		// 设置协议类型

	SET_SERIAL_BAUTERATE = 0x13,	//配置波特率

	SET_OTA_START = 0xD9,			// 开始OTA升级
	SET_OTA_FILE_INFO = 0x8B,		// 升级文件包信息
	OTA_REPORT_STAT = 0x56,			// OTA上报升级状态

	SET_START_MODE = 0x8A,			// 设置启动模式 0x00代表OTA升级工作模式(mini mode)，0xFF代表正常模式；

};

#endif  
