/**
 * @file lkprotocol.h
 * @brief 基于⿅客⼈脸识别模组串⼝通信协议v1.5实现
 * @author xuenuo (xuenuo@orbbec.com)
 * @version 1.0
 * @date 2023-02-14
 *
 * @copyright Copyright (c) ORBBEC Technology Group Co., Ltd. All rights reserved
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2023-02-14 <td>1.0     <td>wangh     <td>内容
 * </table>
 */

 /*
	通信使⽤的数据序列全部是⼩端模式，即⾼字节在后，低字节在前。
	// 数据帧结构
	主控发送给⼈脸模组数据格式
	帧头 MsgID 帧数据⻓度 帧数据 校验值
	2 字节 1 字节 2 字节 N 字节 1 字节

	模组回复主控数据格式
	帧头 MsgID 帧数据⻓度 帧数据(mid result data) 校验值
	2 字节 1 字节 2 字节 1 字节1 字节 N 字节 1 字节

	模组通知主控数据格式
	帧头 MsgID 帧数据⻓度 帧数据(nid result data) 校验值
	2 字节 1 字节 2 字节 1 字节1 字节 N 字节 1 字节

	加密消息数据格式（校验值仅针对加密帧数据）
	帧头 加密帧数据⻓度 加密帧数据 校验值
	2 字节 2 字节 加密数据 1 字节

	帧头:固定为 0xEF 0xAA。
	MsgID:消息 ID
	帧数据⻓度:指明“帧数据”字段的数据⻓度。
	帧数据:需要发送的数据内容：
	校验值:协议的奇偶校验码， 计算⽅式为整条协议除去帧头部分后， 其余字节按位做 XOR 运算。
	加密帧数据：MsgID+帧数据⻓度+帧数据
 */

#pragma once
#include <iostream>
#include <vector>
#include <QObject>
#include <QSerialPort>

#include <unordered_map>
#include <string>
#include <vector>

#include "protocols/OrbbecFaceModuleProtocolInterface.h"
#include "utility/utility.hpp"

#pragma execution_character_set("utf-8")

namespace lkprotocol {
	enum class LK_Cmd_Type : uint8_t
	{
		/*识别*/
		LK_RECOGNIZE = 0x12,
		/*注册一张人脸消息*/
		LK_REGISTER_FACE = 0x1D,
		/*删除指定人脸*/
		LK_DELETE_ASSIGN_FACE = 0x20,
		/*删除全部人脸*/
		LK_DELETE_ALL_FACE = 0x21,
		/*查询注册人脸数目*/
		LK_QUERY_FACE_NUMS = 0x24,
		/*获取软件版本*/
		LK_GET_CAMERA_VERSION = 0x30,
		/*开始OTA升级*/
		LK_OTA_START = 0x40,
		/*发送OTA升级数据包*/
		LK_OTA_SEND_DATA = 0x44,
		/*设置加密模式及随机数*/
		LK_SET_ENCRYPT_MODE = 0x50,
		/*设置出厂密钥抽取规则，下电后保存，仅限一次操作*/
		LK_SET_FACTORY_KEY = 0x52,
		/*设置调试密钥抽取规则，下电后不保存*/
		LK_SET_DEBUG_KEY = 0x53,
		/*strobe切换波特率指令*/
		LK_STROBE_BAUD_RATE = 0x67,
		/*strobe起始指令*/
		LK_STROBE_START = 0x68,
		/*返修指令*/
		LK_REPAIR = 0x71,
		/*主控Poweroff*/
		LK_MCU_POWER_OFF = 0xED,
		/*演示模式*/
		LK_DEMO_MODE = 0xFE,
	};

	enum class LKEncryptMode : uint16_t
	{
		LK_ENCRYPT_MODE_PLAINTEXT = 0, // 明文
		LK_ENCRYPT_MODE_AES = 1,	// AES加密
		LK_ENCRYPT_MODE_XOR = 2,	// 简单异或加密
		LK_ENCRYPT_MODE_REPAIR = 3, // 返修模式
		LK_ENCRYPT_MODE_BUTT
	};

	//! 帧头
	constexpr uint16_t LK_PACKET_HEAD = 0xAAEF;
	constexpr uint16_t kFrame_head = 0xEFAA;

#pragma pack(1)/* 字节对齐 */

	//! 明文数据包格式
	struct packet_plaintext_form {
		//! 帧头
		uint16_t frame_head;
		//! 消息ID
		uint8_t  msg_id;
		//! 帧数据长度
		uint16_t frame_len;
		//! 帧数据
		uint8_t* pframe;
		//! 校验值
		uint8_t  check_xor;
	};

	//! 加密消息数据格式
	struct packet_encrypt_form {
		//! 帧头
		uint16_t frame_head;
		//! 加密帧数据长度
		uint16_t encrypt_frame_len;
		//! 消息ID
		uint8_t  msg_id;
		//! 帧数据长度
		uint16_t frame_len;
		//! 帧数据
		uint8_t* pframe;
		//! 校验值
		uint8_t  check_xor;
	};

	//! 应答帧数据区格式
	struct packet_reply_paylod_form {
		//! 消息ID
		uint8_t  mid;
		//! result
		uint8_t result;
		//! 数据
		uint8_t pdata[1080];
	};

	//! 数据应答格式
	struct packet_reply_form {
		//! 帧头
		uint16_t frame_head;
		//! 消息类型
		uint8_t msg_type;
		//! 帧数据长度
		uint16_t frame_len;
		//! 帧数据
		packet_reply_paylod_form payload;
		//! 校验值
		uint8_t  check_xor;
	};

	//! 人脸状态信息
	struct face_state {
		int16_t state;
		int16_t left;
		int16_t top;
		int16_t right;
		int16_t bottom;
		int16_t yaw;
		int16_t pitch;
		int16_t roll;
	};

	//! 识别结果
	struct recong_result
	{
		uint16_t user_id;
		uint8_t user_name[32];
		uint8_t admin;
		uint8_t unlock_Status;
	};
#pragma pack()

	//! 获取错误结果信息
	std::string GetLKResultCodeDesc(uint8_t result_code);

	class LKProtocol : public OrbbecFaceModuleProtocolInterface
	{
		Q_OBJECT
	public:
		explicit LKProtocol(QSerialPort& serial, QByteArray keyseq, QByteArray keyseed);
		~LKProtocol();

		/**
		 * @brief 设置加密模式及随机数
		 * @param  encrypt_mode  加密模式
		 * @param  random_seed   随机数
		 * @return int 0: 成功 -1: 失败
		 */
		int SetEncryptMode(LKEncryptMode encrypt_mode);
		/**
		 * @brief 设置调试密钥规则
		 * 	密钥抽取规则：使⽤密钥抽取规则对 0x50 随机数⽣成的 md5 字符进⾏抽取，获得加密密钥。
		 * @param  out_keystr
		 * @return int
		 */
		int SetDebugKey(QByteArray& out_key);
		/**
		 * @brief 设置出厂密钥规则
		 * @param  out_keystr
		 * @return int
		 */
		int SetFactoryKey(QByteArray& out_keystr);

		/**
		 * @brief 获取人脸模组软件版本
		 * @param  version
		 * @return int
		 */
		int GetModelVer(std::string& version);

		/**
		 * @brief 主控Power off指令
		 * @return int
		 */
		int SetMcuPowerOff();
		/**
		 * @brief 查询注册人脸数量
		 * @param  face_ids
		 * @return int
		 */
		int GetRegisterFaceNums(std::vector<uint16_t>& face_ids);

		/**
		 * @brief 删除全部人脸
		 * @return int
		 */
		int DeleteAllFaces();

		/**
		 * @brief 删除指定人脸
		 * @param  face_id
		 * @return int
		 */
		int DeleteAssignFace(uint16_t face_id);

		/**
		 * @brief 设置演示模式
		 * @param  enable true:开 false:关
		 * @return int
		 */
		int SetDemoMode(bool enable);
		/**
		 * @brief 人脸识别
		 * @param  mode  0：识别失败时，仅在超时后回复相对应错误码。1：识别失败时，立马回复错误码。
		 * @param  timeout
		 * @return int
		 */
		int RecongnizeFace(uint8_t mode, uint8_t timeout, recong_result& result);

		/**
		 * @brief 注册人脸
		 * @param  admin 是否是管理员
		 * @param  user_name    录入的用户姓名,最大32字节长度
		 * @param  direction_face   录入方向
		 * 	• 0x10 录⼊朝上的⼈脸
			• 0x08 录⼊朝下的⼈脸
			• 0x04 录⼊朝左的⼈脸
			• 0x02 录⼊朝右的⼈脸
			• 0x01 录⼊正向的⼈脸
			• 0x00 未定义， 默认为正向。注册单张⼈脸时默认为 0x00
		 * @param  user_id	注册返回ID
		 * @return int
		 */
		int RegisterFace(bool admin, QByteArray user_name, uint8_t direction_face, uint8_t& user_id);

		/**
		 * @brief 开始OTA升级
		 * @param  baute_rate       波特率
		 * @param  trans_size       单包传输大小
		 * @param  fw_size          固件包大小
		 * @return int
		 */
		int StartOtaRequest(uint32_t baute_rate, uint32_t trans_size, uint32_t fw_size);

		/**
		 * @brief 发送OTA升级包
		 * @param  fw_data          数据包
		 * @return int
		 */
		int SendOtaData(uint16_t packet_id, const QByteArray& fw_data);

		int SendOtaData(uint16_t packet_id, char* fw_data, uint32_t fw_datalen);

		int WaitOtaFinish();

		/**
		 * @brief strobe测试模式
		 * @param  enable
		 * @return int
		 */
		int StrobeTest(uint8_t enable);
		/**
		 * @brief 设置返修模式
		 * @return int
		 */
		int SetRepairMode();

		int SendData(const QByteArray& indata);

		int RecvData(QByteArray& outdata);
	private:
		QSerialPort& comDev_;
		QByteArray key_;
		uint32_t rcvTimeout_;

		QByteArray keyseq_;
		QByteArray keyseed_;
		OrbbecCryptMannage orbCrypt;

		bool isEncryted;
		bool isReady;
		bool isLogEn;
	private:
		bool aes128_ecb_decrypt(const QByteArray& indata, QByteArray& outdata);
		bool aes128_ecb_encrypt(const QByteArray& indata, QByteArray& outdata);

		int parseData(const QByteArray& indata, QByteArray& outdata);

		bool queryData(const QByteArray& send_data, QByteArray& read_data);
		bool recvData(QByteArray& data);
		bool sendData(const QByteArray& data);

		QByteArray PacketForm(lkprotocol::LK_Cmd_Type msg_id, const QByteArray& payload);

		bool register_recong_report_process(packet_reply_form& packet, packet_reply_paylod_form& payload);
	protected:

	signals:
		void ShowCmd(const QString tag, const uint8_t* data, const uint32_t datalen);
		void ShowResp(const QString tag, const QByteArray& resp);
		void ShowNoteInfo(QString noteStr, int bkColor = 0, bool bShow = true);

		bool ReOpenCom(uint32_t bauterate);

	};

}