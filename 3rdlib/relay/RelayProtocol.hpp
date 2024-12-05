#pragma once
/**
 * @file RelayProtocol.hpp
 * @brief 常用继电器控制协议实现
 * @author xuenuo (xuenuo@orbbec.com)
 * @version 1.0
 * @date 2023-05-30
 *
 * @copyright Copyright (c) ORBBEC Technology Group Co., Ltd. All rights reserved
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2023-05-30 <td>1.0     <td>wangh     <td>内容
 * </table>
 */
#include "utility/crc_clac.h"
#include <vector>

enum RelayType
{
	RELAY_TYP_USB = 0,
	RELAY_TYP_MULA = 1,
	RELAY_TYP_MULB = 2,

	RELAY_TYP_BUTTY
};

class RelayProtocol
{
public:
	RelayProtocol() {}
	~RelayProtocol() {}

	static void GetSwitchCmd(int typ, int chn, bool sw_on, std::vector<uint8_t>& data_cmd) {
		if (typ == RELAY_TYP_USB) {
			data_cmd = relay_usb_cmd(sw_on);
		}
		else if (typ == RELAY_TYP_MULA) {
			data_cmd = relay_multiA_cmd(chn, sw_on);
		}
		else if (typ == RELAY_TYP_MULB) {
			data_cmd = relay_multiB_cmd(chn, sw_on);
		}
		else {
			printf("unknown relay type!\n");
		}
	}

protected:
	// 单通道
	static std::vector<uint8_t> relay_usb_cmd(bool on) {
		// 0xA0, 0x01, 0x01, 0xA2
		std::vector<uint8_t> buf;

		buf.push_back(0xA0);
		buf.push_back(0x01);
		buf.push_back(on ? 0x01 : 0x00);
		buf.push_back(calc_sum(buf));

		return buf;
	}

	// 多通道A chn:1~4
	static std::vector<uint8_t> relay_multiA_cmd(int chn, bool on) {
		// 0x55, 0x01, 0x01, 0x02, 0x00, 0x00, 0x00, 0x59
		std::vector<uint8_t> buf{ 0x55, 0x01, 0x01 };
		for (int ich = 1; ich <= 4; ich++) {
			if (chn == ich) {
				buf.push_back(on ? 0x02 : 0x01); // 开:0x02 关:0x01
			}
			else {
				buf.push_back(0x00);
			}
		}
		buf.push_back(calc_sum(buf));

		return buf;
	}

	// 多通道B chn:1~4
	static std::vector<uint8_t> relay_multiB_cmd(int chn, bool on) {
		// 0x55, 0x01, 0x32, 0x00, 0x00, 0x00, 0x01, 0x89
		std::vector<uint8_t> buf{ 0x55, 0x01 };
		buf.push_back(on ? 0x32 : 0x31);
		buf.push_back(0x00);
		buf.push_back(0x00);
		buf.push_back(0x00);
		buf.push_back(chn);
		buf.push_back(calc_sum(buf));

		return buf;
	}
};