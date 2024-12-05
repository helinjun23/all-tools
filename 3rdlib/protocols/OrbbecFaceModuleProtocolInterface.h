#pragma once

#include <iostream>
#include <QObject>

#include "common.h"

/**
 * @brief 奥比人脸模组协议通用接口
 */
class OrbbecFaceModuleProtocolInterface : public QObject
{
	Q_OBJECT
public:
	OrbbecFaceModuleProtocolInterface(QObject* parent = nullptr) :QObject(parent) {};
	virtual ~OrbbecFaceModuleProtocolInterface() {};

	// 查询模组版本
	virtual int GetModuleVer(std::string& ver) = 0;
	// 查询算法板版本
	virtual int GetAlgBoardVer(std::string& ver) = 0;

	// 设置识别挡位
	virtual int SetRecongLevel(uint8_t level) = 0;
	// 查询识别挡位
	virtual int GetRecongLevel(uint8_t& level) = 0;
	// 重置识别挡位
	virtual int ResetRecongLevel() = 0;

	// 设置活体挡位
	virtual int SetLiveLevel(uint8_t level) = 0;
	// 查询活体挡位
	virtual int GetLiveLevel(uint8_t& level) = 0;
	// 重置活体挡位
	virtual int ResetLiveLevel() = 0;

	// 人脸识别请求
	virtual int SetFaceRecongRequest(uint8_t max_times, uint8_t& id) = 0;
	// 人脸注册请求
	virtual int SetFaceRegisterRequeset(uint8_t id) = 0;
	// 获取人脸注册中间状态
	virtual int GetFaceRegisterStat(std::vector<uint8_t>& resp) = 0;

	// 人脸多参数识别请求
	virtual int SetFaceMultiParamRecongRequest(uint8_t max_times) = 0;
	// 获取人脸多参数识别结果
	virtual int GetFaceMultiParamRecongResult(face_recongnize_res& recongRes) = 0;

	// 设置人脸重复注册使能
	virtual int SetMultiFaceRegisterEnable(bool en) = 0;
	// 查询人脸重复注册使能
	virtual int GetMultiFaceRegisterEnable(bool& en) = 0;

	// 删除人脸ID
	virtual int DeleteFaceId(uint8_t id) = 0;
	// 查询全部人脸ID
	virtual int GetAllFaceId(std::vector<uint8_t>& ids) = 0;
	// 清空人脸库
	virtual int DeleteAllFaceId() = 0;
	// 演示识别
	virtual int SetFaceRecongDemoMode(uint8_t max_times) = 0;

	// 开始抓拍请求
	virtual int SetStartCapture(SnapParam& param) = 0;
	// 停止抓拍请求
	virtual int SetStopCapture(SnapParam& param) = 0;
	// 接收抓拍数据
	virtual int GetCaptrueData(QList<QByteArray>& imgCmd) = 0;
	// 解析抓拍数据
	virtual int ParseCaptrueData(const QList<QByteArray>& imgCmd, QByteArray& imgData) = 0;

	// 获取人脸特征值
	virtual int GetFaceFeature(QByteArray& feature_data) = 0;
	// 特征值注册人脸
	virtual int SetFeatureRegisterFace(uint8_t id, const QByteArray& feature_data) = 0;


	// OTA升级请求
	virtual int SetStartOTARequest() = 0;
	// 发送OTA包信息
	virtual int sendOTAFilePacketInfo(uint32_t fileSize, uint32_t pktNums, const QByteArray& fileCrc) = 0;
	// 发送OTA数据包
	virtual int sendOTAFileData(uint32_t pktIdx, const QByteArray& data) = 0;
	// 获取OTA升级状态
	virtual int getOTAUpdateStatus() = 0;

signals:
	int SendData(const QByteArray&);
	int RecvData(QByteArray&);
	void ReOpenCom(qint32);
};
