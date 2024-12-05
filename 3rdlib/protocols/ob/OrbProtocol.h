/**
 * @file OrbProtocol.h
 * @brief 奥比人脸模组协议
 * @author xuenuo (xuenuo@orbbec.com)
 * @version 1.0
 * @date 2023-09-07
 *
 * @copyright Copyright (c) ORBBEC Technology Group Co., Ltd. All rights reserved
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2023-09-07 <td>1.0     <td>wangh     <td>内容
 * </table>
 */
#pragma once
#include <iostream>

#include <QObject>
#include <QSerialPort>

#include "utility/utility.hpp"
#include "protocols/OrbbecFaceModuleProtocolInterface.h"

#pragma execution_character_set("utf-8")

 /**
  * @brief 奥比人脸模组协议、奥比加密协议
  */
namespace orbprotocol {
	constexpr uint8_t ORB_PROTOCOL_TAG_LOW = 0xF5;
	constexpr uint8_t ORB_PROTOCOL_TAG_HIGH = 0xAE;
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

	/**
	 * @brief 命令格式
	 */
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

	/**
	 * @brief 协议命令码
	 */
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
		UNKNOWN_TYPE = 0xFF,			// 未知类型
	};

	class OrbProtocol : public OrbbecFaceModuleProtocolInterface
	{
		Q_OBJECT
	public:
		OrbProtocol(QObject* parent = nullptr);
		~OrbProtocol();

		bool GetOrbEncryptEnable() {
			return encrypt;
		}

		void SetOrbEncryptEnable(bool bEnable) {
			encrypt = bEnable;
		}

		void ClearEncryptKey() {
			orbCrypt.clearKey();
		}
		void SetEncryptSeqrule(const QByteArray& seqrule) {
			orbCrypt.set_seqrule(seqrule);
		}
		void SetEncryptSeed(const QByteArray& seed) {
			orbCrypt.set_seed(seed);
		}
		QByteArray GetEncryptKey() {
			return orbCrypt.get_encKey();
		}

		// 通过 OrbbecFaceModuleProtocolInterface 继承
		int GetModuleVer(std::string& ver) override;
		int GetAlgBoardVer(std::string& ver) override;

		int SetFaceRecongRequest(uint8_t max_times, uint8_t& id) override;

		int SetFaceRegisterRequeset(uint8_t id) override;
		int GetFaceRegisterStat(std::vector<uint8_t>& resp) override;

		int SetFaceMultiParamRecongRequest(uint8_t max_times) override;
		int GetFaceMultiParamRecongResult(face_recongnize_res& recongRes) override;

		int GetAllFaceId(std::vector<uint8_t>& ids) override;
		int SetFaceRecongDemoMode(uint8_t max_times) override;

		int SetStartCapture(SnapParam& param) override;
		int SetStopCapture(SnapParam& param) override;

		int GetCaptrueData(QList<QByteArray>& imgCmd) override;

		int GetFaceFeature(QByteArray& feature_data) override;
		int SetFeatureRegisterFace(uint8_t id, const QByteArray& feature_data) override;

		int SetStartOTARequest() override;
		int sendOTAFilePacketInfo(uint32_t fileSize, uint32_t pktNums, const QByteArray& fileCrc) override;
		int sendOTAFileData(uint32_t pktIdx, const QByteArray& data) override;

		int SetRecongLevel(uint8_t val) override;
		int GetRecongLevel(uint8_t& val) override;
		int ResetRecongLevel() override;

		int SetLiveLevel(uint8_t val) override;
		int GetLiveLevel(uint8_t& val) override;
		int ResetLiveLevel() override;

		int DeleteFaceId(uint8_t id) override;
		int DeleteAllFaceId() override;

		int SetMultiFaceRegisterEnable(bool en) override;
		int GetMultiFaceRegisterEnable(bool& en) override;

		int getOTAUpdateStatus() override;

		int ParseCaptrueData(const QList<QByteArray>& picData, QByteArray& imgData) override;
	private:
		int FillHostPkt(OrbProtocolPktTyp pktTyp, const std::vector<uint8_t>& indata, QByteArray& pktData);
		int FillHostPkt(OrbProtocolPktTyp pktTyp, const QByteArray& indata, QByteArray& pktData);
		int FillSlavePkt(OrbProtocolPktTyp pktTyp, const QByteArray& indata, QByteArray& pktData);
		int ParseOrbCmd(uint8_t cmdtype, const QByteArray& indata, OrbProtocolCmd& orbCmd);

		// 将OrbProtocolCmd数据转为字符串形式
		QString OrbProtocolCmdConvertStr(OrbProtocolCmd& indata, std::vector<uint8_t>& outdata);
		void AddHead(OrbProtocolSource srcTyp, OrbProtocolPktTyp pktTyp, QByteArray& pktData);
		void AddData(const QByteArray& indata, QByteArray& pktData);
		void AddCheckSum(QByteArray& pktData);
		uint16_t CheckSum(uint8_t* cmd, uint32_t cmdLen);

		bool sendData(const QByteArray& data);
		bool recvData(QByteArray& data);
		bool queryData(const QByteArray& send_data, QByteArray& read_data);
		ProtocolResultTyp query_and_parse_data(uint8_t cmd_typ, const QByteArray& send_data, OrbProtocolCmd& orbCmd);

		std::string GetErrPktInfo(OrbProtocolPktErrTyp err);

		QString ReadenseAlgErrCodeProc(unsigned char failRes);
		bool SetBaudRateNoReturn(unsigned char num, int baudRate);

		const std::string CmdOprResultInfo(uint8_t ret_num);

		int ParseDualRecogData(const OrbProtocolCmd& orbCmd, face_recongnize_res& recogRes);

	private:
		bool isLogEn = true;
		bool encrypt = false;
		OrbbecCryptMannage orbCrypt;

	};

}