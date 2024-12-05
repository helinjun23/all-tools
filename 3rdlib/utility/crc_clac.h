#pragma once

#include <vector>

// 校验和计算
static uint8_t calc_sum(const uint8_t* pData, size_t len) {
	uint8_t crc_8 = 0;

	for (int i = 0; i < len; i++)
	{
		crc_8 += *(pData++);
	}

	return crc_8;
}

static uint8_t calc_sum(std::vector<uint8_t> dataBuf) {
	return calc_sum(dataBuf.data(), dataBuf.size());
}

static uint16_t crc_sum_16(const uint8_t* pData, size_t len) {
	uint16_t sum_crc_16 = 0;

	for (int i = 0; i < len; i++)
	{
		sum_crc_16 += *pData++;
	}

	return 0x10000 - sum_crc_16;
}