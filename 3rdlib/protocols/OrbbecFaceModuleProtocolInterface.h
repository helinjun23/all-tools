#pragma once

#include <iostream>
#include <QObject>

#include "common.h"

/**
 * @brief �±�����ģ��Э��ͨ�ýӿ�
 */
class OrbbecFaceModuleProtocolInterface : public QObject
{
	Q_OBJECT
public:
	OrbbecFaceModuleProtocolInterface(QObject* parent = nullptr) :QObject(parent) {};
	virtual ~OrbbecFaceModuleProtocolInterface() {};

	// ��ѯģ��汾
	virtual int GetModuleVer(std::string& ver) = 0;
	// ��ѯ�㷨��汾
	virtual int GetAlgBoardVer(std::string& ver) = 0;

	// ����ʶ��λ
	virtual int SetRecongLevel(uint8_t level) = 0;
	// ��ѯʶ��λ
	virtual int GetRecongLevel(uint8_t& level) = 0;
	// ����ʶ��λ
	virtual int ResetRecongLevel() = 0;

	// ���û��嵲λ
	virtual int SetLiveLevel(uint8_t level) = 0;
	// ��ѯ���嵲λ
	virtual int GetLiveLevel(uint8_t& level) = 0;
	// ���û��嵲λ
	virtual int ResetLiveLevel() = 0;

	// ����ʶ������
	virtual int SetFaceRecongRequest(uint8_t max_times, uint8_t& id) = 0;
	// ����ע������
	virtual int SetFaceRegisterRequeset(uint8_t id) = 0;
	// ��ȡ����ע���м�״̬
	virtual int GetFaceRegisterStat(std::vector<uint8_t>& resp) = 0;

	// ���������ʶ������
	virtual int SetFaceMultiParamRecongRequest(uint8_t max_times) = 0;
	// ��ȡ���������ʶ����
	virtual int GetFaceMultiParamRecongResult(face_recongnize_res& recongRes) = 0;

	// ���������ظ�ע��ʹ��
	virtual int SetMultiFaceRegisterEnable(bool en) = 0;
	// ��ѯ�����ظ�ע��ʹ��
	virtual int GetMultiFaceRegisterEnable(bool& en) = 0;

	// ɾ������ID
	virtual int DeleteFaceId(uint8_t id) = 0;
	// ��ѯȫ������ID
	virtual int GetAllFaceId(std::vector<uint8_t>& ids) = 0;
	// ���������
	virtual int DeleteAllFaceId() = 0;
	// ��ʾʶ��
	virtual int SetFaceRecongDemoMode(uint8_t max_times) = 0;

	// ��ʼץ������
	virtual int SetStartCapture(SnapParam& param) = 0;
	// ֹͣץ������
	virtual int SetStopCapture(SnapParam& param) = 0;
	// ����ץ������
	virtual int GetCaptrueData(QList<QByteArray>& imgCmd) = 0;
	// ����ץ������
	virtual int ParseCaptrueData(const QList<QByteArray>& imgCmd, QByteArray& imgData) = 0;

	// ��ȡ��������ֵ
	virtual int GetFaceFeature(QByteArray& feature_data) = 0;
	// ����ֵע������
	virtual int SetFeatureRegisterFace(uint8_t id, const QByteArray& feature_data) = 0;


	// OTA��������
	virtual int SetStartOTARequest() = 0;
	// ����OTA����Ϣ
	virtual int sendOTAFilePacketInfo(uint32_t fileSize, uint32_t pktNums, const QByteArray& fileCrc) = 0;
	// ����OTA���ݰ�
	virtual int sendOTAFileData(uint32_t pktIdx, const QByteArray& data) = 0;
	// ��ȡOTA����״̬
	virtual int getOTAUpdateStatus() = 0;

signals:
	int SendData(const QByteArray&);
	int RecvData(QByteArray&);
	void ReOpenCom(qint32);
};
