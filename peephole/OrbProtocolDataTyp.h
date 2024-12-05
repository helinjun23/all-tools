#ifndef _ORB_PROTOCOL_DATA_TYP_H_
#define _ORB_PROTOCOL_DATA_TYP_H_

#include <stdint.h>

#define ORB_PROTOCOL_TAG_LOW   0xF5
#define ORB_PROTOCOL_TAG_HIGH  0xAE

enum OrbProtocolSource {
	OrbProtocolSourceHost = 0x01,
	OrbProtocolSourceSlave = 0x02,
	OrbProtocolSourceEnd
};

enum OrbProtocolPktTyp {
	OrbProtocolPktTypCmd = 0x01,
	OrbProtocolPktTypData = 0x02,
	OrbProtocolPktTypLookUp = 0x03,
	OrbProtocolPktTypPeepholeEncryp = 0xFD,
	OrbProtocolPktTypPeepholeComm = 0xFE,
	OrbProtocolPktTypPeepholeFac = 0xFF,
	OrbProtocolPktTypEnd
};

typedef enum PeepholeOrbCommCmdTyp {
	PeepholeOrbCommCmdTypInit = 0x10,
	PeepholeOrbCommCmdTypScanNetConf = 0x11,
	PeepholeOrbCommCmdTypVersion = 0x12,
	PeepholeOrbCommCmdTypReset = 0x13,
	PeepholeOrbCommCmdTypPowerOff = 0x14,
	PeepholeOrbCommCmdTypPicDir = 0x15,
	PeepholeOrbCommCmdTypOta = 0x16,
	PeepholeOrbCommCmdTypReserved1 = 0x17,
	PeepholeOrbCommCmdTypDevStaGet = 0x18,
	PeepholeOrbCommCmdTypEvent = 0x19,
	PeepholeOrbCommCmdTypPir = 0x1a,
	PeepholeOrbCommCmdTypUserInfo = 0x1b,
	PeepholeOrbCommCmdTypSsidPwd = 0x20,
	PeepholeOrbCommCmdTypMac = 0x21,
	PeepholeOrbCommCmdTypDataPass = 0x22,
	PeepholeOrbCommCmdTypIpAddr = 0x23,
	PeepholeOrbCommCmdTypDevName = 0x24,
	PeepholeOrbCommCmdTypLinkKey = 0x25,
	PeepholeOrbCommCmdTypNetSta = 0x26,
	PeepholeOrbCommCmdTypNetTime = 0x27,
	PeepholeOrbCommCmdTypWifiKeepLive = 0x28,
	PeepholeOrbCommCmdTypCloudKey = 0x29,
	PeepholeOrbCommCmdTypPrepare = 0x30,
	PeepholeOrbCommCmdTypOtaSta = 0x31,
	PeepholeOrbCommCmdTypRecSta = 0x32,
	PeepholeOrbCommCmdTypDevSta = 0x33,
	PeepholeOrbCommCmdTypSleep = 0x34,
	PeepholeOrbCommCmdTypNetTimeNote = 0x35,
	PeepholeOrbCommCmdTypWakeUp = 0x36,
	PeepholeOrbCommCmdTypWorkMode = 0x40,
	PeepholeOrbCommCmdTypAlarmCancel = 0x41,
	PeepholeOrbCommCmdTypChildLockCtl = 0x42,
	PeepholeOrbCommCmdTypTamperCtl = 0x43,
	PeepholeOrbCommCmdTypHumanDistSet = 0x44,
	PeepholeOrbCommCmdTypVolumeSet = 0x45,
	PeepholeOrbCommCmdTypLockStaGet = 0x46,
	PeepholeOrbCommCmdTypLockVerGet = 0x47,
	PeepholeOrbCommCmdTypUserAdd = 0x48,
	PeepholeOrbCommCmdTypUserDel = 0x49,
	PeepholeOrbCommCmdTypLockPwdSet = 0x4a,
	PeepholeOrbCommCmdTypBatteryLevelGet = 0x4b,
	PeepholeOrbCommCmdTypRemotePwdUnlock = 0x4c,
	PeepholeOrbCommCmdTypHumanIntervalSet = 0x4d,
	PeepholeOrbCommCmdTypOutAntiOpenIntervalSet = 0x4e,
	PeepholeOrbCommCmdTypAppAutoLockSet = 0x4f,//自动关锁
	PeepholeOrbCommCmdTypAppDataPass = 0x50,
	PeepholeOrbCommCmdTypAppLockSet = 0x51,
	PeepholeOrbCommCmdTypAppLockResp = 0x52,
	PeepholeOrbCommCmdTypAppMotorTorque = 0x53,
	PeepholeOrbCommCmdTypAppBoltRebound = 0x54,
	PeepholeOrbCommCmdTypAppBoltBackTime = 0x55,
	PeepholeOrbCommCmdTypAppLockOpenOrientation = 0x56,
	PeepholeOrbCommCmdTypAppLockSnap = 0x57,
	PeepholeOrbCommCmdTypAppRemotePwdAuth = 0x58,
	PeepholeOrbCommCmdTypAppKeepLive = 0x59,
	PeepholeOrbCommCmdTypMcuOta = 0x60,
	PeepholeOrbCommCmdTypMcuOtaFwInfo = 0x61,
	PeepholeOrbCommCmdTypMcuOtaFwPkt = 0x62,
	PeepholeOrbCommCmdTypMcuOtaSta = 0x63,
	PeepholeOrbCommCmdTypCmdExtend = 0xFF,
	PeepholeOrbCommCmdTypEnd
}PeepholeOrbCommCmdTyp;

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
	PeepholeOrbCmdTypGetCloudKey = 0x010b,
	PeepholeOrbCmdTypSetClientInfo = 0x010c,
	PeepholeOrbCmdTypGetClientInfo = 0x010d,
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
	PeepholeOrbCmdResultTypSucc = 0x00,
	PeepholeOrbCmdResultTypFail = 0x01,
	PeepholeOrbCmdResultTypEnd
};

typedef struct OrbProtocolCmd {
	uint8_t hLow;
	uint8_t hHigh;
	uint8_t source;
	uint8_t typ;
	uint32_t len;
	uint8_t  data[1024]; //main id + sub id + reserved + data
	uint16_t crc;
}OrbProtocolCmd;

//comm
enum PeepholeOrbEventAlarmTyp {
	PeepholeOrbEventAlarmTypFreeze = 0x01,
	PeepholeOrbEventAlarmTypAlarm = 0x02,
	PeepholeOrbEventAlarmTypEvent = 0x03,
	PeepholeOrbEventAlarmTypBatteryLevel = 0x04,
	PeepholeOrbEventAlarmTypEnd
};

enum PeepholeOrbFreezeTyp {
	PeepholeOrbFreezeTypNone = 0x00,
	PeepholeOrbFreezeTypPrint = 0x01,
	PeepholeOrbFreezeTypCode = 0x02,
	PeepholeOrbFreezeTypCard = 0x03,
	PeepholeOrbFreezeTypFace = 0x04,
	PeepholeOrbFreezeTypFingerVein = 0x05,
	PeepholeOrbFreezeTypEnd
};

enum PeepholeOrbAlarmTyp {
	PeepholeOrbAlarmTypNone = 0x00,
	PeepholeOrbAlarmTypTamper = 0x01,
	PeepholeOrbAlarmTypBreakIn = 0x02,
	PeepholeOrbAlarmTypTryBan = 0x03,
	PeepholeOrbAlarmTypFakeLock = 0x04,
	PeepholeOrbAlarmTypDefence = 0x05,
	PeepholeOrbAlarmTypPeepTimeout = 0x06,
	PeepholeOrbAlarmTypLockException = 0x07,
	PeepholeOrbAlarmTypHijackFingerprint = 0x08,
	PeepholeOrbAlarmTypHijackPwd = 0x09,
	PeepholeOrbAlarmTypDoorOpen = 0x0a,
	PeepholeOrbAlarmTypDoorMask = 0x0b,
	PeepholeOrbAlarmTypMotionDetection = 0x0c,
	PeepholeOrbAlarmTypHijackFace = 0x0d,
	PeepholeOrbAlarmTypHijackFingerVerin = 0x0e,
	PeepholeOrbAlarmTypSpringBoltNotIn = 0x0f,
	PeepholeOrbAlarmTypSpringBoltNotOut = 0x10,
	PeepholeOrbAlarmTypFaceErr = 0x11,
	PeepholeOrbAlarmTypFingerprintErr = 0x12,
	PeepholeOrbAlarmTypBackBoardErr = 0x13,
	PeepholeOrbAlarmTypEnd
};

enum PeepholeOrbEventTyp {
	PeepholeOrbEventTypNone = 0x00,
	PeepholeOrbEventTypDoorBell = 0x01,
	PeepholeOrbEventTypDoorOpen = 0x02,
	PeepholeOrbEventTypDoorClose = 0x03,
	PeepholeOrbEventTypDoorAntiLock = 0x04,
	PeepholeOrbEventTypDoorCancel = 0x05,
	PeepholeOrbEventTypDoorReset = 0x06,
	PeepholeOrbEventTypDoorTryErr = 0x07,
	PeepholeOrbEventTypDoorLeaveOpen = 0x08,
	PeepholeOrbEventTypDoorLeaveClose = 0x09,
	PeepholeOrbEventTypDoorVoiceRemind = 0x0a,
	PeepholeOrbEventTypDoorRemoteOpenSuccess,
	PeepholeOrbEventTypDoorRemoteOpenFail,
	PeepholeOrbEventTypDoorRemotePwdOpenSuccess,
	PeepholeOrbEventTypDoorRemotePwdOpenFail,
	PeepholeOrbEventTypDoorOpenFail,
	PeepholeOrbEventTypEnd
};

enum OrbBatteryLevel {
	OrbBatteryLevelNormal = 0x00,
	OrbBatteryLevelBigLow = 0x01,
	OrbBatteryLevelSmallLow = 0x02,
	OrbBatteryLevelBigSmallLow = 0x03,
	OrbBatteryLevelEnd
};

enum OrbLockOpenTyp {
	orb_lock_open_typ_none = 0x00,
	orb_lock_open_typ_finger_print = 0x01,
	orb_lock_open_typ_pwd = 0x02,
	orb_lock_open_typ_card = 0x03,
	orb_lock_open_typ_face = 0x04,
	orb_lock_open_typ_finger_vein = 0x05,
	orb_lock_open_typ_tmp_pwd = 0x06,
	orb_lock_open_typ_app = 0x07,
	orb_lock_open_typ_iris = 0x08,
	orb_lock_open_typ_key = 0x09,
	orb_lock_open_typ_finger_pwd = 0x0a,
	orb_lock_open_typ_face_finger = 0x0b,
	orb_lock_open_typ_finger_vein_pwd = 0x0c,
	orb_lock_open_typ_face_finger_vein = 0x0d,
	orb_lock_open_typ_in_door = 0x0e,
	orb_lock_open_typ_face_pwd = 0x0f,
	orb_lock_open_typ_limit_time_pwd = 0x10,
	orb_lock_open_typ_out_door = 0x11,
	orb_lock_open_typ_palmvein = 0x12,
	orb_lock_open_typ_end
};

enum OrbUserInfo {
	OrbUserInfoNone = 0x00,
	OrbUserInfoAdd,
	OrbUserInfoDel,
	OrbUserInfoClear,
	OrbUserInfoInit,
	OrbUserInfoAddFail,
	OrbUserInfoDelFail,
	OrbUserInfoInitBind,
	OrbUserInfoEnd
};

enum OrbUserAuth {
	OrbUserAuthNone = 0x00,
	OrbUserAuthComm,
	OrbUserAuthForce,
	OrbUserAuthAdmin,
	OrbUserAuthTmp,
	OrbUserAuthEnd,
};

enum OrbUserAuthPwd {
	OrbUserAuthPwdNone = 0x00,
	OrbUserAuthPwdComm,
	OrbUserAuthPwdForce,
	OrbUserAuthPwdAdmin,
	OrbUserAuthPwdOnce,
	OrbUserAuthPwdTimeInter,
	OrbUserAuthPwdLimitNum,
	OrbUserAuthPwdEnd
};

enum OrbUserTyp {
	OrbUserTypFingerPrint = 0x01,
	OrbUserTypPwd,
	OrbUserTypCard,
	OrbUserTypFace,
	OrbUserTypFingerVein,
	OrbUserTypTmpPwd,
	OrbUserTypReserved2,
	OrbUserTypAll,
	OrbUserTypEnd
};

enum PeepholeOrbEventActionTyp {
	PeepholeOrbEventActionTypNone = 0x00,
	PeepholeOrbEventActionTypSnap = 0x01,
	PeepholeOrbEventActionTypAudioRec = 0x02,
	PeepholeOrbEventActionTypVideoRec = 0x03,
	PeepholeOrbEventActionTypAVRec = 0x04,
	PeepholeOrbEventActionTypCloud = 0x05,
	PeepholeOrbEventActionTypEnd
};


typedef struct OrbEventParam {
	uint8_t event;   //enum PeepholeOrbEventAlarmTyp
	uint8_t eventId; //PeepholeOrbEventTyp/PeepholeOrbAlarmTyp/PeepholeOrbFreezeTyp
	uint8_t typ;     //PeepholeOrbEventActionTyp
	uint8_t typParam;
}OrbEventParam;


//net sta
enum OrbWifiStateTyp {
	OrbWifiStateTypConnectSucc = 0x00,
	OrbWifiStateTypConnectHot = 0x01,
	OrbWifiStateTypConfigNet = 0x02,
	OrbWifiStateTypConfigFail = 0x03,
	OrbWifiStateTypNoNet = 0x04,
	OrbWifiStateTypEnd
};

enum OrbOtaStateTyp {
	OrbOtaStateTypIdle = 0x00,
	OrbOtaStateTypWorking = 0x01,
	OrbOtaStateTypDone = 0x02,
	OrbOtaStateTypErr = 0x03,
	OrbOtaStateTypEnd
};

enum OrbOtaErrTyp {
	OrbOtaErrTypNone = 0x00,
	OrbOtaErrTypNoVer = 0x01,
	OrbOtaErrTypQueryTimeout = 0x02,
	OrbOtaErrTypCrc = 0x03,
	OrbOtaErrTypUpdateFail = 0x04,
	OrbOtaErrTypEnd
};

enum orb_dev_sta_typ {
	orb_dev_sta_typ_idle = 0x00,
	orb_dev_sta_typ_wait = 0x01,
	orb_dev_sta_typ_work = 0x02,
	orb_dev_sta_typ_err = 0x03,
	orb_dev_sta_typ_end
};

enum orb_dev_sta_err_typ {
	orb_dev_sta_err_typ_none = 0x00,
	orb_dev_sta_err_typ_wifi_connect_fail = 0x01,
	orb_dev_sta_err_typ_cloud_connect_fail = 0x02,
	orb_dev_sta_err_typ_cloud_timeout = 0x03,
	orb_dev_sta_err_typ_video_call_fail = 0x04,
	orb_dev_sta_err_typ_video_call_rejected = 0x05,
	orb_dev_sta_err_typ_cam_err = 0x06,
	orb_dev_sta_err_typ_audio_err = 0x07,
	orb_dev_sta_err_typ_data_pass_fail = 0x08,
	orb_dev_sta_err_typ_snap_fail = 0x09,
	orb_dev_sta_err_typ_rec_video_fail = 0x0a,
	orb_dev_sta_err_typ_rec_audio_fail = 0x0b,
	orb_dev_sta_err_typ_unknown = 0xff,
	orb_dev_sta_err_typ_end
};

#endif  
