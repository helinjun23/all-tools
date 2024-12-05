#pragma once
/**
 * @file utility.hpp
 * @brief 提供常用功能库
 * @author xuenuo (xuenuo@orbbec.com)
 * @version 1.0
 * @date 2023-02-27
 *
 * @copyright Copyright (c) ORBBEC Technology Group Co., Ltd. All rights reserved
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2023-02-27 <td>1.0     <td>wangh     <td>内容
 * </table>
 */

#include <QObject>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>

#include <vector>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/async.h"
#include "spdlog/stopwatch.h"
#include "spdlog/fmt/ranges.h"
#include "spdlog/fmt/bin_to_hex.h"

#include "md5/md5sum.h"

#include "Qt-AES/qaesencryption.h"

#ifdef EN_OPENCV_LIB
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs/legacy/constants_c.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

#pragma comment(lib, "../../3rdlib/opencv/Win64_Debug/opencv_core455d.lib")
#pragma comment(lib, "../../3rdlib/opencv/Win64_Debug/opencv_imgcodecs455d.lib")
#pragma comment(lib, "../../3rdlib/opencv/Win64_Debug/opencv_imgproc455d.lib")
#endif

 // 平台库
#include <windows.h>
#include <Dbt.h>


 /* 16位小端转换 */
inline uint16_t LittleEnidan16(const char* val)
{
	return ((*(val + 1) << 8) & 0xff00)
		| ((*val) & 0x00ff);
}

/* 32位小端转换 */
inline uint32_t LittleEnidan32(const char* val)
{
	return ((*(val + 3) << 24) & 0xff000000)
		| ((*(val + 2) << 16) & 0x00ff0000)
		| ((*(val + 1) << 8) & 0x0000ff00)
		| ((*val) & 0x000000ff);
}

/* 16位大端转换 */
inline uint16_t BigEnidan16(const char* val)
{
	return (((*val) << 8) & 0xff00)
		| (*(val + 1) & 0x00ff);
}

/* 32位大端转换 */
inline uint32_t BigEnidan32(const char* val)
{
	return ((*val) << 24 & 0xff000000)
		| ((*(val + 1) << 16) & 0x00ff0000)
		| ((*(val + 2) << 8) & 0x0000ff00)
		| ((*(val + 3) & 0x000000ff));
}

inline QByteArray LittleEnidan16(uint16_t val)
{
	char buf[3]{ (char)(val & 0x00ff), val >> 8 };
	return QByteArray(buf, 2);
}

inline QByteArray LittleEnidan32(uint32_t val)
{
	char buf[5]{ (char)(val & 0x000000ff),(char)(val >> 8),(char)(val >> 16),(char)(val >> 24) };
	return QByteArray(buf, 4);
}

inline QByteArray BigEnidan16(uint16_t val)
{
	char buf[3]{ (char)(val >> 8), (char)val };
	return QByteArray(buf, 2);
}

inline QByteArray BigEnidan32(uint32_t val)
{
	char buf[5]{ (char)(val >> 24),(char)(val >> 16),(char)(val >> 8), (char)val };
	return QByteArray(buf, 4);
}

static double calculatePercentage(int numerator, int denominator) {
	return (double)numerator / denominator * 100.0;
}

#include <QDateTime>
//static void PrintByteArray(QString info, const QByteArray& indata, bool benable = false)
//{
//	if (!benable)
//	{
//		return;
//	}
//	std::string nowtime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:z").toStdString();
//	//std::cout << info.toStdString() << " size= " << indata.size() << std::endl;
//	printf("[%s] %s size= %d\n", nowtime.c_str(), info.toStdString().c_str(), indata.size());
//	for (int i = 0; i < indata.size(); i++)
//	{
//		uint8_t ch = indata[i];
//		printf("%02x ", ch);
//	}
//	printf("\n\n");
//}


static uint8_t ParityCheck(const QByteArray& indata)
{
	uint8_t ret = indata.front();

	for (size_t i = 1; i < indata.size(); i++)
	{
		ret ^= indata.at(i);
	}

	return ret;
}


class OrbbecCryptMannage
{
public:
	OrbbecCryptMannage() {
	}
	~OrbbecCryptMannage() {
	}

	bool isready() {
		return !key.isEmpty();
	}

	QByteArray get_encKey() {
		return key;
	}

	void clearKey() {
		key.clear();
		keyMD5.clear();
		seqKey.clear();
	}

	void set_seqrule(const QByteArray& seqrule)
	{
		seqKey = seqrule;
	}

	void set_seed(const QByteArray& seed)
	{
		assert(seed.size() == 4);
		// 转md5编码
		MD5 md5_(seed.data(), seed.size());
		keyMD5 = md5_.toString();
		key.clear();
		if (seqKey.size() == 16 && !keyMD5.empty()) {
			// 通过规则序列获取最终密钥
			for (const auto& idx : seqKey)
			{
				key.push_back(keyMD5.at(idx));
			}
		}
	}

	// AES128加密
	QByteArray aes128_encrypt(const QByteArray& rawdata) {
		if (!isready())
		{
			return rawdata;
		}
		// AES128 加密
		return QAESEncryption::Crypt(QAESEncryption::AES_128, QAESEncryption::ECB, rawdata, key, QByteArray(), QAESEncryption::ZERO);
	}

	// AES128解密
	QByteArray aes128_decrypt(const QByteArray& crypdata) {
		if (!isready())
		{
			return crypdata;
		}

		QByteArray bfresult;
		QByteArray afresult;		// AES128 解密
		bfresult = QAESEncryption::Decrypt(QAESEncryption::AES_128, QAESEncryption::ECB, crypdata, key, QByteArray(), QAESEncryption::ZERO);
		//afresult = QAESEncryption::RemovePadding(bfresult, QAESEncryption::ZERO);
		//if (bfresult.size() == afresult.size()) // 两种尝试移除补齐位数据
		//{
		//	afresult = QAESEncryption::RemovePadding(bfresult, QAESEncryption::PKCS7);
		//}
		return bfresult;
	}

protected:
private:
	QByteArray key;
	std::string keyMD5;
	QByteArray seqKey;
};


// 异步执行耗时调用，暂不支持成员函数
template<typename Func, typename...Args>
void AsyncProc(Func proc, Args...args)
{
	auto future = QtConcurrent::run(proc, args...);
	while (!future.isFinished())
	{
		QApplication::processEvents(QEventLoop::AllEvents, 100);
	}
}

typedef union
{
	float a;
	uint8_t b[4];
}float_type;

inline void convertThrstoFloat(const uint8_t buffs[4], float& fthres)
{
	float_type tmpVal;
	for (int i = 0; i < 4; i++)
	{
		tmpVal.b[i] = buffs[i];
	}
	fthres = tmpVal.a;
}

inline void convertFloatToThrs(const float& fthres, std::vector<uint8_t>& buffs)
{
	float_type tmpVal;
	tmpVal.a = fthres;
	for (int i = 0; i < 4; i++)
	{
		buffs[i] = tmpVal.b[i];
	}
}

static QByteArray orbCalcSha256(QByteArray indata)
{
	QByteArray hash = QCryptographicHash::hash(indata, QCryptographicHash::Sha256);
	return hash;
}

// 读取CSV文件
static bool readCsv(QString filePath, QList<QStringList>& data)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	QTextStream stream(&file);
	while (!stream.atEnd())
	{
		QString line = stream.readLine();
		QStringList row = line.split(',', Qt::SkipEmptyParts);
		data.append(row);
	}

	file.close();
	return true;
}

// 写入CSV文件
static bool writeCsv(QString filePath, QList<QStringList>& data)
{
	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;

	QTextStream stream(&file);
	for (int i = 0; i < data.size(); i++)
	{
		QStringList row = data.at(i);
		for (int j = 0; j < row.size(); j++)
		{
			stream << row.at(j);
			if (j < row.size() - 1)
				stream << ",";
		}
		stream << "\n";
	}

	file.close();
	return true;
}


// 读取CSV文件
static bool readCsv_1(QString filePath, QList<QStringList>& data)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly))
		return false;

	QTextCodec* codec = QTextCodec::codecForName("UTF-8");   // 使用UTF-8编码
	QByteArray content = file.readAll();
	QString text = codec->toUnicode(content);
	QStringList lines = text.split('\n');

	for (int i = 0; i < lines.size(); i++)
	{
		if (!lines.at(i).isEmpty())
		{
			QStringList row = lines.at(i).split(',');
			data.append(row);
		}
	}

	file.close();
	return true;
}

// 写入CSV文件
static bool writeCsv_1(QString filePath, QList<QStringList>& data)
{
	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly))
		return false;

	QTextCodec* codec = QTextCodec::codecForName("UTF-8");   // 使用UTF-8编码

	for (int i = 0; i < data.size(); i++)
	{
		QStringList row = data.at(i);
		QString line = row.join(",");
		line += "\n";
		QByteArray encodedLine = codec->fromUnicode(line);
		file.write(encodedLine);
	}

	file.close();
	return true;
}

static bool is_hex_str(const QString& str)
{
	static QRegExp hex_regexp("^[0-9A-Fa-f]+$");
	bool bhex = (-1 != hex_regexp.indexIn(str));
	return bhex;
}

static QByteArray inputstr_convert(const QString& str)
{
	QByteArray result(str.toUtf8());
	// 去除空格
	QStringList strlist = str.split(' ', Qt::SkipEmptyParts);
	if (strlist.size() == 1) {
		// 未找到空格
		return result;
	}

	QByteArray bytebuff;
	for (auto& str : strlist) {
		bytebuff.push_back(str.toUtf8());
	}
	// 调用通信串口进行发送
	if (is_hex_str(bytebuff)) {
		result.clear();
		for (const auto& ch : strlist)
		{
			bool ok;
			uint8_t _hex = ch.toInt(&ok, 16);
			result.push_back(_hex);
		}
	}
	return result;
}

static void PrintByteArray(QString info, const QByteArray& indata, bool benable = false)
{
	if (!benable)
	{
		return;
	}
	std::vector<uint8_t> vt(indata.begin(), indata.end());
	SPDLOG_INFO("{}[{}] {:Xp}", info.toStdString().c_str(), indata.size(), spdlog::to_hex(vt));
}




static const GUID GUID_DEVINTERFACE_LIST[] =
{
	// GUID_DEVINTERFACE_USB_DEVICE
	{ 0xA5DCBF10, 0x6530, 0x11D2,{ 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } },
};

/**
 * @brief 注册USB设备热插拔监听
 * @param  handle
 */
static void registerUSBNotification(HANDLE handle)
{
	HDEVNOTIFY hDevNotify;
	DEV_BROADCAST_DEVICEINTERFACE NotifacationFiler;

	ZeroMemory(&NotifacationFiler, sizeof(DEV_BROADCAST_DEVICEINTERFACE));
	NotifacationFiler.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	NotifacationFiler.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;

	for (int i = 0; i < sizeof(GUID_DEVINTERFACE_LIST) / sizeof(GUID); i++)
	{
		NotifacationFiler.dbcc_classguid = GUID_DEVINTERFACE_LIST[i];
		hDevNotify = RegisterDeviceNotification(handle, &NotifacationFiler, DEVICE_NOTIFY_WINDOW_HANDLE);
		if (!hDevNotify)
		{
			int Err = GetLastError();
			printf("register usb device fail %d\n", Err);
		}
	}
}

/**
 * @brief USB设备热插拔事件监听
 * @param  eventType
 * @param  message
 * @param  result
 * @return int 1:设备连接 2:设备断开 0:其他
 */
static int orbNativeEventFilter(const QByteArray& eventType, void* message, long* result)
{
	//其中eventType表明了此次消息的类型，message存储着具体是什么信息，result是个传出变量；
	//eventType在windows操作系统下是"windows_generic_MSG"字符串，可以查看Qt的文档知道
	//message表明这个信号附带哪些信息，在热插拔事件中是WM_DEVICECHANGE类型，具体windows定义了哪些，可以查看“Dbt.h”文件
	int ret = 0;
	if (eventType == QByteArray("windows_generic_MSG"))
	{
		MSG* pMsg = reinterpret_cast<MSG*>(message);
		if (pMsg->message == WM_DEVICECHANGE) {
			PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)pMsg->lParam;
			switch (pMsg->wParam)
			{
				//设备连上
			case DBT_DEVICEARRIVAL:
			{
				if (DBT_DEVTYP_DEVICEINTERFACE == lpdb->dbch_devicetype)
				{
					PDEV_BROADCAST_DEVICEINTERFACE pDevInf = (PDEV_BROADCAST_DEVICEINTERFACE)lpdb;
					QString name = QString::fromWCharArray(pDevInf->dbcc_name);
					printf("usb device connect: %s\n", name.toStdString().c_str());
					ret = 1;
				}
			}
			break;
			//设备断开
			case DBT_DEVICEREMOVECOMPLETE:
			{
				PDEV_BROADCAST_DEVICEINTERFACE pDevInf = (PDEV_BROADCAST_DEVICEINTERFACE)lpdb;
				QString name = QString::fromWCharArray(pDevInf->dbcc_name);
				if (!name.isEmpty()) {
					printf("usb device disconnect: %s", name.toStdString().c_str());
					ret = 2;
				}
				break;
			}
			case DBT_DEVNODES_CHANGED:
				//检查设备连接状态的改变
				//qDebug() << "DBT_DEVNODES_CHANGED";

				break;
			}

		}
	}
	return ret;
}