/**
 * @file ProtocolManage.h
 * @brief 串口协议管理
 * @author xuenuo (xuenuo@orbbec.com)
 * @version 1.0
 * @date 2023-06-06
 *
 * @copyright Copyright (c) ORBBEC Technology Group Co., Ltd. All rights reserved
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2023-06-06 <td>1.0     <td>wangh     <td>内容
 * </table>
 */
#pragma once
#include "common.h"
#include "protocols/SerialOrbCmdPkt.h"

class ProtocolManage
{
public:
	ProtocolManage() {}
	~ProtocolManage() {}

	// 类型识别
	static int JudgeProtocolType(QVector<uint8_t>& io_data) {
		if (io_data.empty())
		{
			spdlog::warn("input data is empty!");
			return -1;
		}

		// 帧头
		if (0xF5 == io_data[0] && 0xAE == io_data[1]) // 奥比协议
		{
			CSerialOrbCmdPkt cmdPkt;
			OrbProtocolCmd orbCmd;
			if (0 > cmdPkt.ParseOrbCmd(QByteArray((char*)io_data.data(), io_data.size()), orbCmd))
			{
				spdlog::warn("frame data crc sum failed!");
				return -1;
			}

			cmdPkt.OrbProtocolCmdConvertStr(orbCmd, io_data);
			//// 数据长度
			//size_t data_len = io_data.size();
			//uint16_t frame_len = (io_data[5] << 8) & 0xff00 | io_data[4];
			//if (frame_len == data_len - 8)
			//{
			//	uint16_t crc_16 = crc_sum_16(io_data.data(), data_len - 2);
			//	if (((crc_16 & 0x00ff) == io_data[data_len - 2])
			//		&& (((crc_16 & 0xff00) >> 8) == io_data[data_len - 1]))
			//	{
			//		spdlog::debug("complete frame data, dont need any process!");
			//	}
			//	else
			//	{
			//		spdlog::warn("frame data crc sum failed!");
			//	}
			//}
			//else if (frame_len == data_len - 6)
			//{
			//	// 缺少校验位，自动计算补全
			//	uint16_t crc_16 = crc_sum_16(io_data.data(), data_len);
			//	io_data.push_back(crc_16 & 0x00ff);
			//	io_data.push_back((crc_16 & 0xff00) >> 8);
			//	spdlog::info("auto complete frame crc sum.");
			//}
			//else
			//{
			//	spdlog::info("unknown frame len {} calc data len {}", frame_len, data_len);
			//}
		}

		return 0;
	}

private:

};

