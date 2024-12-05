#include "lkprotocol.h"
#include <sstream>


using namespace lkprotocol;

LKProtocol::LKProtocol(QSerialPort& serial, QByteArray keyseq, QByteArray keyseed)
	:comDev_(serial), rcvTimeout_(2000), keyseq_(keyseq), keyseed_(keyseed), isEncryted(false),
	isLogEn(true)
{

}

LKProtocol::~LKProtocol()
{

}

std::string GetLKResultCodeDesc(uint8_t result_code)
{
	static const std::unordered_map<uint8_t, std::string> LK_ResultCode{
		{0, "成功"},
		{1, "模块拒绝该命令"},
		{2, "人脸录入_解锁算法已终止"},
		{4, "相机打开失败"},
		{5, "未知错误"},
		{6, "无效的参数"},
		{7, "内存不足"},
		{8, "人脸不存在"},
		{9, "人脸数超过最大数量"},
		{10, "人脸已录入"},
		{12, "活体检测失败"},
		{13, "人体录入或解锁超时"},
		{14, "加密芯片授权失败"},
		{16, "人脸库对比失败"},
		{19, "读文件失败"},
		{20, "写文件失败"},
		{21, "通信协议未加密"},
		{27, "人脸算法模型版本不对"},
		{28, "人脸算法模型大小超过阈值"},
		{29, "人脸算法接口异常"},
		{34, "人脸高相似"},
		{37, "member id不合法"},
		{38, "release key已存在"},
		{43, "未录入人脸，人脸活体检测失败"},
		{44, "未录入人脸，人脸活体检测成功"},
		{101, "OTA数据包校验失败"},
		{102, "OTA包数量不全"},
	};

	if (LK_ResultCode.find(result_code) != LK_ResultCode.end())
	{
		return LK_ResultCode.at(result_code);
	}

	return "未知错误";
}

QByteArray LKProtocol::PacketForm(LK_Cmd_Type msg_id, const QByteArray& payload)
{
	QByteArray cmd_buf;
	cmd_buf.push_back(BigEnidan16(lkprotocol::kFrame_head));

	QByteArray inoutdata;
	inoutdata.push_back((uint8_t)msg_id);

	uint16_t data_len = payload.size();
	inoutdata.push_back(BigEnidan16(data_len));
	if (data_len > 0)
	{
		inoutdata.push_back(payload);
	}


	// 加密模式，只校验帧数据部分
	if (isEncryted)
	{
		PrintByteArray("EncryptRawData: ", inoutdata, true);
		inoutdata = orbCrypt.aes128_encrypt(inoutdata);
		cmd_buf.push_back(BigEnidan16((uint16_t)inoutdata.size()));
		cmd_buf.push_back(inoutdata);
		cmd_buf.push_back(ParityCheck(cmd_buf.mid(4)));
		PrintByteArray("AES-128 EBC EncryptRawData: ", inoutdata, true);
	}
	else
	{
		cmd_buf.push_back(inoutdata);
		cmd_buf.push_back(ParityCheck(cmd_buf.mid(2)));
	}

	return cmd_buf;
}

// 4字节转3字节流
auto uint32_to_qbytearray = [](uint32_t inval) {
	QByteArray retval;
	retval.push_back((inval & 0xff0000) >> 16);
	retval.push_back((inval & 0xff00) >> 8);
	retval.push_back(inval & 0xff);
	return retval;
};

int LKProtocol::StartOtaRequest(uint32_t baute_rate, uint32_t trans_size, uint32_t fw_size)
{
	QByteArray cmd_buf;
	QByteArray data_buf;

	// 波特率
	data_buf.push_back(uint32_to_qbytearray(baute_rate));
	// 单包传输大小
	data_buf.push_back(uint32_to_qbytearray(trans_size));
	// 固件包大小
	data_buf.push_back(uint32_to_qbytearray(fw_size));


	cmd_buf = PacketForm(LK_Cmd_Type::LK_OTA_START, data_buf);

	QByteArray read_data;
	if (!queryData(cmd_buf, read_data))
	{
		printf("LKProtocol StartOtaRequest recv failed!\n");
		return -1;
	}

	isEncryted = false;
	if (0 != parseData(read_data, read_data))
	{
		return -1;
	}
	isEncryted = true;

	packet_reply_form packet;
	packet.frame_head = BigEnidan16(read_data.mid(0, 2));
	packet.msg_type = read_data.at(2);
	packet.frame_len = BigEnidan16(read_data.mid(3, 2));
	packet_reply_paylod_form payload;
	memset(&payload, 0, sizeof(payload));
	memcpy(&payload, read_data.mid(5, packet.frame_len), packet.frame_len);

	isLogEn = false;

	return payload.result;
}

int LKProtocol::SendData(const QByteArray& indata)
{
	return sendData(indata);
}

int LKProtocol::RecvData(QByteArray& outdata)
{
	return recvData(outdata);
}

int LKProtocol::SendOtaData(uint16_t packet_id, char* fw_data, uint32_t fw_datalen)
{
	QByteArray cmd_buf;
	cmd_buf.push_back(BigEnidan16(lkprotocol::kFrame_head));
	cmd_buf.push_back((uint8_t)LK_Cmd_Type::LK_OTA_SEND_DATA);

	QByteArray data_buf;
	data_buf.push_back(BigEnidan16(packet_id));
	data_buf.push_back(QByteArray(fw_data, fw_datalen));

	uint32_t data_len = data_buf.size();
	cmd_buf.push_back(uint32_to_qbytearray(data_len));

	if (data_len > 0)
	{
		cmd_buf.push_back(data_buf);
	}
	cmd_buf.push_back(ParityCheck(cmd_buf.mid(2)));

	isEncryted = false;
	QByteArray read_data;
	if (!queryData(cmd_buf, read_data))
	{
		printf("LKProtocol SendOtaData recv failed!\n");
		return -1;
	}

	if (0 != parseData(read_data, read_data))
	{
		return -1;
	}
	isEncryted = true;

	packet_reply_form packet;
	packet.frame_head = BigEnidan16(read_data.mid(0, 2));
	packet.msg_type = read_data.at(2);
	packet.frame_len = BigEnidan16(read_data.mid(3, 2));
	packet_reply_paylod_form payload;
	memset(&payload, 0, sizeof(payload));
	memcpy(&payload, read_data.mid(5, packet.frame_len), packet.frame_len);

	return payload.result;
}

int LKProtocol::SendOtaData(uint16_t packet_id, const QByteArray& fw_data)
{
	QByteArray cmd_buf;
	cmd_buf.push_back(BigEnidan16(lkprotocol::kFrame_head));
	cmd_buf.push_back((uint8_t)LK_Cmd_Type::LK_OTA_SEND_DATA);

	QByteArray data_buf;
	data_buf.push_back(BigEnidan16(packet_id));
	data_buf.push_back(fw_data);

	uint32_t data_len = data_buf.size();
	cmd_buf.push_back(uint32_to_qbytearray(data_len));

	if (data_len > 0)
	{
		cmd_buf.push_back(data_buf);
	}
	cmd_buf.push_back(ParityCheck(cmd_buf.mid(2)));

	isEncryted = false;
	QByteArray read_data;
	if (!queryData(cmd_buf, read_data))
	{
		printf("LKProtocol SendOtaData recv failed!\n");
		return -1;
	}

	if (0 != parseData(read_data, read_data))
	{
		return -1;
	}
	isEncryted = true;

	packet_reply_form packet;
	packet.frame_head = BigEnidan16(read_data.mid(0, 2));
	packet.msg_type = read_data.at(2);
	packet.frame_len = BigEnidan16(read_data.mid(3, 2));
	packet_reply_paylod_form payload;
	memset(&payload, 0, sizeof(payload));
	memcpy(&payload, read_data.mid(5, packet.frame_len), packet.frame_len);

	return payload.result;
}

int LKProtocol::WaitOtaFinish()
{
	QByteArray readbuf;
	uint8_t cnt = 30;
	do
	{
		recvData(readbuf);
		QThread::sleep(1);
	} while (!isReady && cnt--);

	isLogEn = true;

	return cnt != 0 ? 0 : -1;
}

int LKProtocol::SetEncryptMode(LKEncryptMode encrypt_mode)
{
	QByteArray cmd_buf;

	orbCrypt.set_seed(keyseed_);
	key_ = orbCrypt.get_encKey();
	emit ShowCmd("[key]: ", (const uint8_t*)key_.data(), (uint32_t)key_.size());

	QByteArray data_buf;
	data_buf.push_back(keyseed_);
	data_buf.push_back((uint8_t)encrypt_mode);
	uint32_t timestamp = 0; // 暂未使用,默认0
	data_buf.push_back(BigEnidan32(timestamp));

	cmd_buf = PacketForm(LK_Cmd_Type::LK_SET_ENCRYPT_MODE, data_buf);

	isEncryted = encrypt_mode == LKEncryptMode::LK_ENCRYPT_MODE_AES;


	QByteArray read_data;
	if (!queryData(cmd_buf, read_data))
	{
		printf("LKProtocol SetEncryptMode recv failed!\n");
		return -1;
	}
	if (0 != parseData(read_data, read_data))
	{
		return -1;
	}
	if (read_data.at(5) == (uint8_t)LK_Cmd_Type::LK_SET_ENCRYPT_MODE)
	{
		return read_data.at(6) == 0 ? 0 : -1;
	}
	return -1;
}

int LKProtocol::SetDebugKey(QByteArray& out_key)
{
	QByteArray cmd_buf;
	orbCrypt.set_seqrule(keyseq_);

	isEncryted = false;
	cmd_buf = PacketForm(LK_Cmd_Type::LK_SET_DEBUG_KEY, keyseq_);

	QByteArray read_data;
	if (!queryData(cmd_buf, read_data))
	{
		printf("LKProtocol SetDebugKey recv failed!\n");
		return -1;
	}
	if (0 != parseData(read_data, read_data))
	{
		return -1;
	}
	packet_reply_form packet;
	packet.frame_head = BigEnidan16(read_data.mid(0, 2));
	packet.msg_type = read_data.at(2);
	packet.frame_len = BigEnidan16(read_data.mid(3, 2));
	packet_reply_paylod_form payload;
	memset(&payload, 0, sizeof(payload));
	memcpy(&payload, read_data.mid(5, packet.frame_len), packet.frame_len);

	return payload.result;
}

int LKProtocol::SetFactoryKey(QByteArray& out_keystr)
{
	QByteArray cmd_buf;

	orbCrypt.set_seqrule(keyseq_);

	cmd_buf = PacketForm(LK_Cmd_Type::LK_SET_FACTORY_KEY, keyseq_);

	QByteArray read_data;
	if (!queryData(cmd_buf, read_data))
	{
		printf("LKProtocol SetFactoryKey recv failed!\n");
		return -1;
	}
	if (0 != parseData(read_data, read_data))
	{
		return -1;
	}
	packet_reply_form packet;
	packet.frame_head = BigEnidan16(read_data.mid(0, 2));
	packet.msg_type = read_data.at(2);
	packet.frame_len = BigEnidan16(read_data.mid(3, 2));
	packet_reply_paylod_form payload;
	memset(&payload, 0, sizeof(payload));
	memcpy(&payload, read_data.mid(5, packet.frame_len), packet.frame_len);

	return payload.result;
}

int LKProtocol::GetModelVer(std::string& version)
{
	QByteArray cmd_buf;
	QByteArray data_buf;
	cmd_buf = PacketForm(LK_Cmd_Type::LK_GET_CAMERA_VERSION, data_buf);

	QByteArray read_data;
	if (!queryData(cmd_buf, read_data))
	{
		printf("LKProtocol GetModelVer recv failed!\n");
		return -1;
	}
	if (0 != parseData(read_data, read_data))
	{
		return -1;
	}
	packet_reply_form packet;
	packet.frame_head = BigEnidan16(read_data.mid(0, 2));
	packet.msg_type = read_data.at(2);
	packet.frame_len = BigEnidan16(read_data.mid(3, 2));
	packet_reply_paylod_form payload;
	memset(&payload, 0, sizeof(payload));
	memcpy(&payload, read_data.mid(5, packet.frame_len), packet.frame_len);
	std::stringstream strVer;
	strVer << "v" << payload.pdata;
	version = strVer.str();

	return payload.result;
}

int LKProtocol::SetMcuPowerOff()
{
	QByteArray cmd_buf;
	QByteArray data_buf;
	cmd_buf = PacketForm(LK_Cmd_Type::LK_MCU_POWER_OFF, data_buf);

	QByteArray read_data;
	if (!queryData(cmd_buf, read_data))
	{
		printf("LKProtocol SetMcuPowerOff recv failed!\n");
		return -1;
	}
	if (0 != parseData(read_data, read_data))
	{
		return -1;
	}
	packet_reply_form packet;
	packet.frame_head = BigEnidan16(read_data.mid(0, 2));
	packet.msg_type = read_data.at(2);
	packet.frame_len = BigEnidan16(read_data.mid(3, 2));
	packet_reply_paylod_form payload;
	memset(&payload, 0, sizeof(payload));
	memcpy(&payload, read_data.mid(5, packet.frame_len), packet.frame_len);

	return payload.result;
}

int LKProtocol::GetRegisterFaceNums(std::vector<uint16_t>& face_ids)
{
	QByteArray cmd_buf;
	QByteArray data_buf;
	cmd_buf = PacketForm(LK_Cmd_Type::LK_QUERY_FACE_NUMS, data_buf);

	QByteArray read_data;
	if (!queryData(cmd_buf, read_data))
	{
		printf("LKProtocol GetRegisterFaceNums recv failed!\n");
		return -1;
	}
	if (0 != parseData(read_data, read_data))
	{
		return -1;
	}
	packet_reply_form packet;
	packet.frame_head = BigEnidan16(read_data.mid(0, 2));
	packet.msg_type = read_data.at(2);
	packet.frame_len = BigEnidan16(read_data.mid(3, 2));
	packet_reply_paylod_form payload;
	memset(&payload, 0, sizeof(payload));
	memcpy(&payload, read_data.mid(5, packet.frame_len), packet.frame_len);
	int offset = 1;
	for (int i = 1; i <= payload.pdata[0]; i++)
	{
		uint16_t face_id = BigEnidan16((const char*)(payload.pdata + offset));
		face_ids.push_back(face_id);
		offset += i * 2;
	}

	return payload.result;
}

int LKProtocol::DeleteAllFaces()
{
	QByteArray cmd_buf;
	QByteArray data_buf;
	cmd_buf = PacketForm(LK_Cmd_Type::LK_DELETE_ALL_FACE, data_buf);

	QByteArray read_data;
	if (!queryData(cmd_buf, read_data))
	{
		printf("LKProtocol DeleteAllFaces recv failed!\n");
		return -1;
	}
	if (0 != parseData(read_data, read_data))
	{
		return -1;
	}
	packet_reply_form packet;
	packet.frame_head = BigEnidan16(read_data.mid(0, 2));
	packet.msg_type = read_data.at(2);
	packet.frame_len = BigEnidan16(read_data.mid(3, 2));
	packet_reply_paylod_form payload;
	memset(&payload, 0, sizeof(payload));
	memcpy(&payload, read_data.mid(5, packet.frame_len), packet.frame_len);

	return payload.result;
}

int LKProtocol::DeleteAssignFace(uint16_t face_id)
{
	QByteArray cmd_buf;
	QByteArray data_buf;
	data_buf.push_back(BigEnidan16(face_id));
	cmd_buf = PacketForm(LK_Cmd_Type::LK_DELETE_ASSIGN_FACE, data_buf);

	QByteArray read_data;
	if (!queryData(cmd_buf, read_data))
	{
		printf("LKProtocol DeleteAssignFace recv failed!\n");
		return -1;
	}
	if (0 != parseData(read_data, read_data))
	{
		return -1;
	}
	packet_reply_form packet;
	packet.frame_head = BigEnidan16(read_data.mid(0, 2));
	packet.msg_type = read_data.at(2);
	packet.frame_len = BigEnidan16(read_data.mid(3, 2));
	packet_reply_paylod_form payload;
	memset(&payload, 0, sizeof(payload));
	memcpy(&payload, read_data.mid(5, packet.frame_len), packet.frame_len);

	return payload.result;
}

int LKProtocol::SetDemoMode(bool enable)
{
	QByteArray cmd_buf;
	QByteArray data_buf;
	data_buf.push_back((uint8_t)enable);
	cmd_buf = PacketForm(LK_Cmd_Type::LK_DEMO_MODE, data_buf);

	sendData(cmd_buf);

	packet_reply_form packet;
	packet_reply_paylod_form payload;
	if (!register_recong_report_process(packet, payload))
	{
		return -1;
	}

	return (payload.mid == (uint8_t)LK_Cmd_Type::LK_DEMO_MODE) ? payload.result : -1;
}

int LKProtocol::RecongnizeFace(uint8_t mode, uint8_t timeout, recong_result& result)
{
	QByteArray cmd_buf;
	QByteArray data_buf;
	data_buf.push_back(mode);
	data_buf.push_back(timeout);
	cmd_buf = PacketForm(LK_Cmd_Type::LK_RECOGNIZE, data_buf);

	sendData(cmd_buf);

	packet_reply_form packet;
	packet_reply_paylod_form payload;
	if (!register_recong_report_process(packet, payload))
	{
		return -1;
	}

	memset(&result, 0, sizeof(result));
	result.user_id = BigEnidan16((const char*)payload.pdata);
	memcpy(result.user_name, payload.pdata + 2, 32);
	result.admin = payload.pdata[34];
	result.unlock_Status = payload.pdata[35];

	return payload.result;
}

bool LKProtocol::register_recong_report_process(packet_reply_form& packet, packet_reply_paylod_form& payload)
{
	do {
		QByteArray recv_data;
		if (!recvData(recv_data))
		{
			printf("LKProtocol register_recong_report_process recv failed!\n");
			emit ShowNoteInfo("register_recong_report_process recv report data failed!");
			QThread::msleep(100);
			return false;
		}


		QByteArray out_data;
		if (0 != parseData(recv_data, out_data))
		{
			emit ShowNoteInfo("register_recong_report_process parse data failed!");
			QThread::msleep(100);
			return false;
		}

		packet.frame_head = BigEnidan16(out_data.mid(0, 2));
		packet.msg_type = out_data.at(2);
		packet.frame_len = BigEnidan16(out_data.mid(3, 2));

		// 中间注册状态
		if (packet.msg_type == 0x01)
		{
			// nid
			uint8_t nid = out_data[5];
			// 人脸状态
			face_state up_state;
			memset(&up_state, 0, sizeof(face_state));
			memcpy(&up_state, out_data.data() + 6, sizeof(face_state));

			static const std::unordered_map<uint8_t, std::string> kFaceState{
				{0, "人脸正常"},
				{1, "未检测到⼈脸"},
				{2, "人脸太靠近图片上边沿"},
				{3, "人脸太靠近图片下边沿"},
				{4, "人脸太靠近图片左边沿"},
				{5, "人脸太靠近图片右边沿"},
				{6, "人脸距离太远"},
				{7, "⼈脸距离太近"},
				{8, "多张人脸"},
				{9, "保留"},
				{10, "脸部遮挡"},
				{11, "偏转角过大"},
				{16, "人脸库比对失败"},
				{17, "人体质量过低"},
				{18, "3D活体失败"},
				{19, "2D活体失败"},
				{20, "头部左转角度过大"},
				{21, "头部右转角度过大"},
				{22, "头部上仰角度过大"},
				{23, "头部下仰角度过大"},
				{24, "高相似比对失败"},
			};

			printf("register state=%d left=%d top=%d right=%d bottom=%d yaw=%d pitch=%d roll=%d\n",
				up_state.state, up_state.left, up_state.top, up_state.right,
				up_state.bottom, up_state.yaw, up_state.pitch, up_state.roll);

			if (0 != up_state.state)
			{
				QString strNote;

				if (kFaceState.find(up_state.state) != kFaceState.end())
				{
					strNote = kFaceState.at((uint8_t)up_state.state).c_str();
					emit ShowNoteInfo(strNote);
				}
				else
				{
					emit ShowNoteInfo("未知错误");
				}
			}
			QThread::msleep(50);
		}
		else if (packet.msg_type == 0x00)
		{
			memset(&payload, 0, sizeof(packet_reply_paylod_form));
			memcpy(&payload, out_data.mid(5, packet.frame_len), packet.frame_len);
			break;
		}
		else
		{
			break;
		}
	} while (1);

	return true;
}

int LKProtocol::RegisterFace(bool admin, QByteArray user_name, uint8_t direction_face, uint8_t& user_id)
{
	QByteArray cmd_buf;
	QByteArray data_buf;
	data_buf.push_back((uint8_t)admin);
	QByteArray _tmp_user(32, 0);
	for (int i = 0; i < user_name.size() && i < _tmp_user.size(); i++)
	{
		_tmp_user[i] = user_name.at(i);
	}
	data_buf.push_back(_tmp_user);
	data_buf.push_back(direction_face);
	uint16_t member_id = 0;
	data_buf.push_back(BigEnidan16(member_id));
	uint8_t timeout = 10;
	data_buf.push_back(timeout);

	cmd_buf = PacketForm(LK_Cmd_Type::LK_REGISTER_FACE, data_buf);

	sendData(cmd_buf);

	packet_reply_form packet;
	packet_reply_paylod_form payload;
	if (!register_recong_report_process(packet, payload))
	{
		return -1;
	}

	// 注册ID
	user_id = BigEnidan16((const char*)payload.pdata);
	// 录入方向
	//uint8_t diretion = payload.pdata[1];

	return payload.result;
}


int LKProtocol::StrobeTest(uint8_t enable)
{
	QByteArray cmd_buf;
	QByteArray data_buf;
	data_buf.push_back(enable);
	cmd_buf = PacketForm(LK_Cmd_Type::LK_STROBE_BAUD_RATE, data_buf);

	QByteArray read_data;
	if (!queryData(cmd_buf, read_data))
	{
		printf("LKProtocol StrobeTest recv failed!\n");
		return -1;
	}
	if (0 != parseData(read_data, read_data))
	{
		return -1;
	}
	packet_reply_form packet;
	packet.frame_head = BigEnidan16(read_data.mid(0, 2));
	packet.msg_type = read_data.at(2);
	packet.frame_len = BigEnidan16(read_data.mid(3, 2));
	packet_reply_paylod_form payload;
	memset(&payload, 0, sizeof(payload));
	memcpy(&payload, read_data.mid(5, packet.frame_len), packet.frame_len);

	return payload.result;
}

int LKProtocol::SetRepairMode()
{
	QByteArray cmd_buf;
	QByteArray data_buf;
	cmd_buf = PacketForm(LK_Cmd_Type::LK_REPAIR, data_buf);

	QByteArray read_data;
	if (!queryData(cmd_buf, read_data))
	{
		printf("LKProtocol SetRepairMode recv failed!\n");
		return -1;
	}
	if (0 != parseData(read_data, read_data))
	{
		return -1;
	}
	packet_reply_form packet;
	packet.frame_head = BigEnidan16(read_data.mid(0, 2));
	packet.msg_type = read_data.at(2);
	packet.frame_len = BigEnidan16(read_data.mid(3, 2));
	packet_reply_paylod_form payload;
	memset(&payload, 0, sizeof(payload));
	memcpy(&payload, read_data.mid(5, packet.frame_len), packet.frame_len);

	return payload.result;
}


bool LKProtocol::sendData(const QByteArray& data)
{
#if 0
	emit SendData(data);
#else
	if (!comDev_.isOpen())
	{
		return false;
	}
	else
	{
		auto baudrate = comDev_.baudRate();
		emit ReOpenCom(baudrate); // 解决偶发接收不到数据问题
	}

#ifndef _LM_TEST
	int datalen = data.size();
	//! 清楚缓冲区数据，防止数据粘连
	comDev_.clear(QSerialPort::AllDirections);
	comDev_.write((const char*)data.data(), datalen);
	//! 立即发送
	comDev_.flush();
#else
	moduleDummySerialWrite(data);
#endif
#endif

	if (isLogEn)
	{
		emit ShowCmd("send: ", (const uint8_t*)data.data(), (uint32_t)data.size());
		//PrintByteArray("send: ", data, true);
	}

	return true;
}

bool LKProtocol::recvData(QByteArray& data)
{
	QByteArray read_data;
	if (!comDev_.isOpen())
	{
		return false;
	}

	int timeout_nums = 0;
	int max_timeout_times = rcvTimeout_ / 20;
	int framelen = -1;

	isReady = false;
	while ((++timeout_nums < max_timeout_times) && framelen != read_data.size())
	{
		//std::cout << "can read size = " << comDev_.bytesAvailable() << std::endl;
		QByteArray tmpbuf;
		if (comDev_.waitForReadyRead(10))
		{
			tmpbuf = comDev_.readAll();
		}

		//! check whether recv complete frame
		if (!tmpbuf.isEmpty()) // 滤掉ready上报
		{
			uint16_t framehead = ((tmpbuf.at(0) & 0x00ff) | tmpbuf.at(1) << 8 & 0xff00);
			if (framehead == LK_PACKET_HEAD)
			{
				if (tmpbuf.size() == 7 && tmpbuf.at(2)) // ready 上报  EF AA 01 00 01 00 00
				{
					isReady = true;
				}
				else
				{
					read_data = tmpbuf;
					if (isEncryted) // 加密模式下,数据帧格式不一样
					{
						framelen = ((tmpbuf.at(2) << 8) & 0xff00) | tmpbuf.at(3) + 5;// encrytdata_len + frame_len
					}
					else
					{
						framelen = ((tmpbuf.at(3) << 8) & 0xff00) | tmpbuf.at(4) + 6;// data_len + frame_len
					}
				}
			}
			else
			{
				read_data.append(tmpbuf);
			}
		}

	}

	if (read_data.isEmpty())
	{
		std::string nowtime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:z").toStdString();
		printf("[%s]recv data failed!\n", nowtime.c_str());
		return false;
	}
	if (isLogEn)
	{
		emit ShowResp("recv: ", read_data);
		PrintByteArray("recv: ", read_data, true);
	}

	data = read_data;
	return data.size() > 7;
}

int LKProtocol::parseData(const QByteArray& indata, QByteArray& outdata)
{
	QByteArray tmp_data;
	// 加密模式
	if (isEncryted)
	{
		QByteArray encrypt_data(indata.mid(4, indata.size() - 5));
		QByteArray decrypt_data;

		// PrintByteArray("DecryptRawData: ", encrypt_data, true);
		decrypt_data = orbCrypt.aes128_decrypt(encrypt_data);

		// 帧头
		tmp_data.push_back(indata.mid(0, 2));
		// msgid + datalen + data
		tmp_data.push_back(decrypt_data);
		// 计算校验
		tmp_data.push_back(ParityCheck(tmp_data.mid(2)));

		//PrintByteArray("AES-128 EBC DecryptRawData: ", tmp_data, true);
		if (isLogEn)
		{
			emit ShowResp("decrypt recv : ", tmp_data);
		}
	}
	else
	{
		tmp_data = indata;
	}

	outdata = tmp_data;

	return 0;
}

bool LKProtocol::queryData(const QByteArray& send_data, QByteArray& read_data)
{
	if (!sendData(send_data))
	{
		std::cout << "com send data failed!\n";
		return false;
	}

	QByteArray recv_data;
	if (!recvData(recv_data))
	{
		return false;
	}

	read_data = recv_data;

	return true;
}

bool LKProtocol::aes128_ecb_encrypt(const QByteArray& indata, QByteArray& outdata)
{
	outdata = QAESEncryption::Crypt(QAESEncryption::AES_128, QAESEncryption::ECB,
		indata, key_, QByteArray(), QAESEncryption::ZERO);
	return true;
}

bool LKProtocol::aes128_ecb_decrypt(const QByteArray& indata, QByteArray& outdata)
{
	QByteArray bfresult;
	QByteArray afresult;		// AES128 解密
	bfresult = QAESEncryption::Decrypt(QAESEncryption::AES_128, QAESEncryption::ECB,
		indata, key_, QByteArray(), QAESEncryption::ZERO);
	afresult = QAESEncryption::RemovePadding(bfresult, QAESEncryption::ZERO);
	outdata = afresult;

	return true;
}
