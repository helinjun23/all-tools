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
	SET_RECONG_THR = 0xB2,          // ����ʶ����ֵ
	GET_RECONG_THR = 0xB3,          // ��ѯʶ����ֵ
	RESET_RECONG_THR = 0xB7,        // ����ʶ����ֵ
	SET_RECONG_LEVEL = 0xD4,        // ����ʶ����ֵ��λ
	GET_RECONG_LEVEL = 0xE0,        // ��ѯʶ����ֵ��λ
	RESET_RECONG_LEVEL = 0xD5,      // ����ʶ����ֵ��λ
	SET_LIVING_THR = 0xD8,          // ���û�����ֵ
	GET_LIVING_THR = 0xE3,          // ��ȡ������ֵ
	SET_LIVING_LEVEL = 0xD6,        // ���û�����ֵ��λ
	GET_LIVING_LEVEL = 0xE2,        // ��ѯ������ֵ��λ
	RESET_LIVING_LEVEL = 0xD7,      // ���û�����ֵ��λ
	SET_ADDITONREC_THR = 0xD0,      // ���ò�¼��ֵ
	GET_ADDITONREC_THR = 0xE1,      // ��ѯ��¼��ֵ
	RESET_ADDITONREC_THR = 0xD2,    // ���ò�¼��ֵ
	SET_REPEAT_REGISTER = 0xBE,     // �����ظ�ע��
	GET_REPEAT_REGISTER = 0xBF,     // ��ѯ�ظ�ע��
	GET_ALL_REGISTER_ID = 0x50,     // ��ѯ����ע��ID
	REGISTER_FACE_ID = 0x01,        // ע������ID
	REGISTER_FACE_ID_INTER = 0x57,  // ע������ID�м�״̬
	REGISTER_FACE_ID_INTER_CONTINUE = 0xA0, // ע������ID�м�״̬����
	DELETE_FACE_ID = 0x02,          // ɾ������ID
	FACE_RECONG = 0x40,             // ����ʶ��

	GET_ALGBOARD_SWVER = 0x61,      // ��ѯ�㷨������汾��
	GET_OBMODEL_VER = 0x62,         // ��ѯ�±�����ͷģ��汾��
	GET_SNAP_PICTURE = 0x73,        // ץȡһ��JPEG��ʽ��ͼƬ
	UP_SNAP_PICTURE_INFO = 0x77,        // ��֪��λ��ͼƬ�ļ�����Ϣ

	FACE_DEMO_MODE = 0x51,          // ��ʾʶ��

	// ����Э�����
	SET_DEBUG_ENC_KEY = 0xF0,       // ����Debug Key
	SET_RELEASE_ENC_KEY = 0xF1,     // ����Release Key
	INIT_ENC_KEY = 0xF3,            // ��ʼ���������
	EXIT_ENC_MODE = 0xF4,           // �˳�����ģʽ
	CRYPT_RESET = 0xBD,             // �ָ��������ã����������Ϣ

	GET_CAPTURE_FACE_FEATURE = 0xA9,    // ץ����������ȡ��������
	REGISTER_FACE_BY_FEATURE = 0xA8,// �·�����ֵע��

	SET_PROTOCOL_TYPE = 0xDA,		// ����Э������

	SET_SERIAL_BAUTERATE = 0x13,	//���ò�����

	SET_OTA_START = 0xD9,			// ��ʼOTA����
	SET_OTA_FILE_INFO = 0x8B,		// �����ļ�����Ϣ
	OTA_REPORT_STAT = 0x56,			// OTA�ϱ�����״̬

	SET_START_MODE = 0x8A,			// ��������ģʽ 0x00����OTA��������ģʽ(mini mode)��0xFF��������ģʽ��

};

#endif  
