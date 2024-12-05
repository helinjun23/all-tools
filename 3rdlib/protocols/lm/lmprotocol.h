/**
 * @file lmprotocol.h
 * @brief 基于绿米&奥比门锁加密协议
 * @details
 * @author xuenuo
 * @version 1.0
 * @date 2022-06-14
 *
 * @copyright Copyright (c) 2022  ORBBEC.INC
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-06-14 <td>1.0     <td>xuenuo  <td>
 * </table>
 *
 */

#pragma once
#include <iostream>
#include <vector>
#include <mutex>

#include <QObject>
#include <QSerialPort>
#include <QMutex>
#include <QSharedPointer>
#include <QList>

#include "utility/crc.hpp"

#pragma execution_character_set("utf-8")

 /**
  * @brief 绿米提供加密协议方案，用于U2-SLM门锁模组项目.
  * <p>数据格式:
  * <table>
  * <tr><th>2Bytes  <th>2Bytes  <th>4Bytes  <th>4Bytes  <th>2Bytes  <th>4Bytes  <th>2Bytes  <th>NBytes
  * <tr><td>Head    <td>Length1  <td>crc32   <td>Type    <td>cmd     <td>cmdseq  <td>Length2  <td>Payload
  * </table>
  * <p>详细定义:
  * Head: 0x5AA5;
  * Length1: 2+4+2+Length2;
  * crc32: crc32(cmd+cmdseq+length2+payload),校验该区域数据;
  * Type: 包类型;
  * cmd: 命令类型;
  * cmdseq: 命令流水号;
  * Length2: N;
  * Payload: 数据区;
  *
  */
namespace lmprotocol {
	constexpr const char lm_rootkey[32] = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73,
	0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81, 0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
	0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 };

	constexpr const char lm_key_iv[16] = { 0x50, 0x21, 0x9E, 0xB6, 0x57, 0x4C, 0xE8, 0x93, 0x56, 0x32,
	0xEA, 0xC4, 0xD8, 0x0B, 0xA3, 0x72 };

	constexpr const char lm_sessionkey[32] = { 0x15, 0x01, 0xc0, 0xd0, 0xe4, 0xfd, 0xdf, 0xd7, 0x7a, 0x65,
	0xf1, 0x2f, 0x45, 0x61, 0x0b, 0x59, 0xd9, 0x0a, 0x9c, 0x61, 0x0c, 0x04, 0x76, 0xdb,
	0x0b, 0xbe, 0x9e, 0xe4, 0x7f, 0x8d, 0xe1, 0x46 };

	/**
		* @brief 数据包类型
		*/
	enum class PacketType :uint16_t
	{
		//! 明文
		PACKETTYPE_PLAINTEXT = 0xE583,
		//! 根密钥加密         
		PACKETTYPE_ROOTENC = 0x59A6,
		//! 会话密钥加密      
		PACKETTYPE_SESSIONENC = 0x8D27,
	};

	/**
		* @brief 明文命令类型
		*/
	enum class PlainTextCmdType :uint16_t
	{
		//! 根密钥协商
		PLA_ROOTKEY = 0x16C0,
		//! 根密钥协商结果
		PLA_ROOTKEY_RESULT = 0xEB7A,
		//! 读取算法参数,奥比协议扩展设备去激活
		PLA_READALGPARAM = 0x485E,
		//! 算法参数结果
		PLA_READALGPARAM_RESULT = 0xC1A5,
		//! 文件数据包
		PLA_FILEDATA = 0x7817,
		//! 文件接收状态
		PLA_FILESTAT = 0x2490,
		//! 根密钥未协商
		PLA_ROOT_NOTREADY = 0x2DAB,
		//! UNBIND
		PLA_UNBIND = 0x3A88,
		//! UNBIND_ACK
		PLA_UNBIND_ACK = 0x4A88
	};

	/**
		* @brief 密文命令类型-session会话密钥加密
		* 加密方式: AES-256-CBC
		* 密钥长度: 32Byte
		* IV均为指定: 私下发布
		*/
	enum class SessionCiphetextCmdType :uint16_t
	{
		//! 会话密钥信道探测
		SESSION_CIP_PROBEDATA = 0x73C2,
		//! 会话密钥信道正常
		SESSION_CIP_PROBEECHO = 0xD69B,
		//! 模组协议
		SESSION_CIP_ORBCMD = 0x1E50,
		//! ACK流水号
		SESSION_CIP_SEQUENT = 0x49B2,
		//! RESET
		SESSION_CIP_RESET = 0x77AC,
		//! RESET ACK
		SESSION_CIP_RESETACK = 0x1C7D,
	};

	/**
		* @brief 密文命令类型-root根密钥加密
		* 加密方式: AES-256-CBC
		* 密钥长度: 32Byte
		* IV均为指定: 私下发布
		*/
	enum class RootCiphetextCmdType :uint16_t
	{
		//! 会话密钥协商
		ROOT_CIP_SESSIONKEY = 0x27A8,
		//! 会话密钥协商成功
		ROOT_CIP_SESSIONKEY_ACK = 0x83F1,
		//! 会话未就绪
		ROOT_CIP_SESSION_NOT_READY = 0x30A6,
	};

	/**
	 * @brief 协商结果类型
	 */
	enum class NegotiateResult
	{
		FAIL = 0,
		SUCCESS = 1,
	};

	/**
	 * @brief 工作模式
	 */
	enum class WorkMode
	{
		//! 对设备去激活模式,设备通讯只能使用奥比协议通讯；重启后生效
		MODE_ENCRYPT_DISABLE = 2,
		//! 加密测试模式,研发工具使用，主要用于验证会话加解密通讯
		MODE_ENCRYPT_TEST = 3,
	};

	/**
		* @brief 操作异常码
		*/
	enum class OprErrNum :uint8_t
	{
		//! 正常
		OPRERR_NONE = 0,
		//! 协商异常,1s后重试
		OPRERR_NEGOTIATE = 1,
		//! 连续协商异常,连续200次或连续2分钟无法协商会话密钥主控端硬重启模组
		OPRERR_CONTINUOUS_NEGOTIATE = 2,
		//! 探测异常,连续5次未收到信道探测Echo,主控端回滚至会话密钥协商流程
		OPRERR_PROBE = 3,
		//! 流水号异常,模组端不应答
		OPRERR_SEQ = 4,
		//! 命令异常,连续5次收到无法解释命令,清空会话加密信息，缓冲结构光协议包,回滚至密钥协商阶段
		OPRERR_CMD_UNKNOWN = 5,
		//! 模组侧丢包异常,5次发送失败，清空会话加密信息，缓冲结构光协议包，回滚至密钥协商阶段
		OPRERR_MODEL_PACKETLOSS = 6,
		//! 主控侧丢包异常,连续5次发送消息，收不到Ack回复，清空会话加密信息，缓冲结构光协议包，回滚至密钥协商阶段
		OPRERR_MCU_PACKETLOSS = 7,
		//! 模组重启或异常,清空会话加密信息，缓冲结构光协议包，回滚至密钥协商阶段
		OPRERR_MODEL_REBOOT = 8,
		//! 其他错误
		OPRERR_OTHER = 9,
	};

	/**
	* @brief OTA升级包错误码
	*/
	enum class OTAErrNum :uint8_t
	{
		//! 成功
		OTA_SUCCESS = 0,
		//! 超时
		OTA_TIMEOUT = 1,
		//! 重传
		OTA_RETRANS = 2,
		//! 校验失败
		OTA_CRCFAIL = 3,
		//! 其他
		OTA_OTHER = 4,
	};
	/**
	 * @brief payload区固定头长度
	 */
	const uint32_t PAYLOAD_HEADLEN = 8;

	/**
	 * @brief 包头固定头长度
	 */
	const uint32_t PACKET_HEADLEN = 10;

	/**
	 * @brief 固定包头
	 */
	const uint16_t LM_PACKET_HEAD = 0xA55A;

#pragma pack(1)/* 字节对齐 */
	/**
	* @brief 数据包头格式
	*/
	struct packetheadform
	{
		uint16_t head;              //!< 数据包头 0x5AA5
		uint16_t payloadlen;        //!< 整包长度
		uint32_t payloadcrc32;      //!< 数据区crc32校验
		uint16_t packettype;        //!< 数据包类型
		packetheadform() :head(0), payloadlen(0), payloadcrc32(0), packettype(0) {}
	};

	/**
		* @brief payload数据区格式
		*/
	static constexpr uint32_t PAYLOAD_MAXSIZE = 1024 * 5;//1048
	struct payloadform
	{
		uint16_t cmdtype;				 //!< 命令类型
		uint32_t cmdseq;				 //!< 命令流水号,双⽅各⾃管理发送及接收流⽔号，发送成功或失败都⾃增
		uint16_t cmdlen;                 //!< 数据长度
		uint8_t cmdpaylod[PAYLOAD_MAXSIZE]{ 0 };    //!< 数据区，默认1048字节，不够再扩充
		payloadform() :cmdtype(0), cmdseq(0), cmdlen(0) {}
	};

	/**
		* @brief 数据包格式
		*/
	struct packetform
	{
		packetheadform head;		   //!< 数据包头
		payloadform payload;           //!< 数据区
	};

#pragma pack()


	/**
		* @brief 主要用于管理协议组包、解包、加密、解密等操作
		*/
	class LMProtocol :public QObject
	{
		Q_OBJECT
	public:
		explicit LMProtocol(QSerialPort& serial);
		~LMProtocol();

		using type_payload = QByteArray;

		/**
		 * @brief 设置工作模式
		 * 设备进行去激活或进入测试模式
		 */
		OprErrNum setWorkMode(WorkMode mode);

		/**
		 * 解绑密钥
		 */
		OprErrNum setUnbind();

		/**
			* @brief 根密钥协商流程
			* - 介绍
			* 1. 仅允许由主控端发起的根密钥协商
			* 2. 协商成功后，不允许再次发起协商
			* 3. 根密钥协商后需要保存，以后⽤于协商会话密钥
			* 4. 加密环境发起重置命令，模组需清空根密钥
			* - 异常处理
			* 1. 协商异常，1s后重试
			*
			* @param root_key
			* @return OprErrNum 返回值
			*/
		OprErrNum rootKeyNegotiateProc(const QByteArray& root_key, const QByteArray& root_iv);

		/**
			* @brief 会话密钥协商流程
			* - 介绍
			* 1. 话密钥协商，在每次会话通信前，仅允许由主控端发起
			* 2. 由主控通过根密钥，传输每次会话通信的会话密钥
			* 3. 模组端收到后，需通过根密钥回复密钥接收成功，再切换为会话密钥
			* 4. 主控端收到密钥接收成功消息后，随即切换为会话密钥
			* 5. 双⽅切换为会话密钥时，随即清零会话流⽔号计数器 (⽤于防重放)
			* - 异常处理：
			* 1. 协商异常，1s后重试
			* 2. 连续协商异常，连续200次或连续2分钟无法协商会话密钥主控端硬重启模组
			* 3. 探测异常，连续5次未收到信道探测Echo主控端回滚至会话密钥协商流程
			*
			* @return OprErrNum 返回值
			*/
		OprErrNum sessionKeyNegotiateProc(const QByteArray& session_key, const QByteArray& session_iv);

		/**
			* @brief 会话加密协议通信流程
			* - 异常处理
			* 1. 流水号异常，
			* 2. 命令异常，
			* 3. 模组侧丢包异常，5次发送失败，清空会话加密信息，缓冲结构光协议包，回滚至密钥协商阶段
			* 4. 主控侧丢包异常，连续5次发送消息，收不到Ack回复，清空会话加密信息，缓冲结构光协议包，回滚至密钥协商阶段
			* 5. 模组重启或异常，清空会话加密信息，缓冲结构光协议包，回滚至密钥协商阶段
			*
			* @return OprErrNum 返回值
			*/
		OprErrNum sessionEncryptComProc(const QByteArray& indata, QByteArray& outdata);

		/**
			* @brief 会话加密通信发送数据
			* @param  indata
			* @return OprErrNum
			*/
		OprErrNum sessionEncryptComSend(const QByteArray& indata);

		/**
			* @brief 会话加密通信接收数据
			* @param  outdata
			* @return OprErrNum
			*/
		OprErrNum sessionEncryptComRecv(QByteArray& outdata);

		/**
			* @brief 根密钥重置
			* @return true 成功
			* @return false 失败
			*/
		bool resetRootKey();

		/**
			* @brief 发送ota数据包
			* @param  indata
			* @return OTAErrNum
			*/
		OprErrNum sessionOTAFileData(const QByteArray& indata, QByteArray& outdata);

		/**
			* @brief 会话明文通信接收数据
			* @param  outdata
			* @return OprErrNum
			*/
		OprErrNum sessionPlainTextComRecv(QByteArray& outdata);

		/**
		 * 设置接收应答超时时间，ms
		 */
		void setRcvTimeout(uint32_t time_ms);

		/**
		 * @brief ACK确认
		 * @return OprErrNum
		 */
		OprErrNum sessionEncryptComAck();
	private:


		/**
		 * @brief 会话加密信道探测
		 * @return OprErrNum
		 */
		OprErrNum sessionEncryptProbe();

		/**
		 * @brief 打包payload数据区
		 * @param  cmd_type         命令类型
		 * @param  cmd_seq          命令流水号
		 * @param  payload          数据区
		 * @param  outpayload       以payloadform类型，输出payload部分
		 * @return type_payload 	以type_payload类型，输出payload部分
		 */
		type_payload packPlayload(uint16_t cmd_type, uint32_t cmd_seq,
			const type_payload& payload, payloadform& outpayload);

		/**
		 * @brief 打包整包数据
		 * @param  packet_type      包类型
		 * @param  payload          加密数据区
		 * @return type_payload 	以type_payload类型，输出整包数据
		 */
		type_payload packData(PacketType packet_type, const payloadform& payload);

		/**
			* @brief 解析接收数据
			* @param  indata          待解析完整数据
			* @param  outdata         解析完成数据
			* @return true 		   	  成功
			* @return false 		  失败
			*/
		bool parseData(const type_payload& indata, packetform& outdata);

		/**
		 * @brief 解析数据包头
		 * @param  indata
		 * @param  outdata
		 * @return true
		 * @return false
		 */
		bool parseHead(const type_payload& indata, packetheadform& outdata);

		/**
		 * @brief 解析密文/明文部分数据
		 * @param  indata
		 * @param  outdata
		 * @return true
		 * @return false
		 */
		bool parsePayload(const type_payload& indata, payloadform& outdata);

		void handleErr(OprErrNum err_num);

		bool sendData(const QByteArray& data);
		/**
		 * @brief 接收单条数据
		 * @param  data
		 * @return true
		 * @return false
		 */
		bool recvData(QByteArray& data);

		bool queryData(const QByteArray& send_data, QByteArray& read_data);

		bool aes256_cbc_encrypt(const QByteArray& indata, const QByteArray& key, const QByteArray& iv, QByteArray& outdata);

		bool aes256_cbc_decrypt(const QByteArray& indata, const QByteArray& key, const QByteArray& iv, QByteArray& outdata);

		/* 清空流水号 */
		void clearSeq();

		bool encrypt(PacketType packet_type, QByteArray& inoutdata);

		bool decrypt(PacketType packet_type, QByteArray& inoutdata);

	private:
		uint32_t tx_seq_;//!< 发送ACK流水号
		uint32_t rx_seq_;//!< 接收ACK流水号

		QByteArray root_key_;//!< 根密钥
		QByteArray session_key_;//!< 会话密钥
		QByteArray root_iv_;//!< 偏移多项式
		QByteArray session_iv_;

		CrcCal<crc_32_type> crc_;

		QSerialPort& comDev_;

		std::atomic_bool enAsyncRead;

		QSharedPointer<QList<QByteArray>> recvDataCache;
		std::mutex readMutex;

		uint32_t rcvTimeout_;
	signals:
		void enableAsyncRead(bool);
		void SendData(const QByteArray&);
		void RecvData(QByteArray&);
	public slots:
		// 定时器异步接收串口数据
		void onTimerSerialAsyncRead(const QByteArray& rcvBuf);
		void setAsyncReadData(bool enable);
	private:
		// 测试接口
#ifdef _LM_TEST
		bool moduleDummySerialWrite(const QByteArray& indata);
		bool moduleDummySerialRead(QByteArray& outdata);
		QList<QByteArray> dummyDataBuff_;
#endif
	};
}