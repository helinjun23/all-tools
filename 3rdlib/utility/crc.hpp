/**
 * @file crc.h
 * @brief implement crc_ccitt/crc-16/crc-32
 * @author xuenuo (xuenuo@orbbec.com)
 * @version 1.0
 * @date 2022-06-15
 *
 * @copyright Copyright (c) 2022  ORBBEC.INC
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-06-15     <td>1.0     <td>xuenuo  <td>内容
 * </table>
 */

#pragma once
#include <iostream>

using crc_ccitt_type = unsigned short;
using crc_32_type = unsigned long;
using crc_16_type = unsigned short;

template<typename CRC_TYPE>
class CrcCal
{
public:
	CrcCal() :
		bit_width(8 * sizeof(CRC_TYPE))
		, top_bit(1 << (bit_width - 1))
		, init_val(get_initial_reminder())
		, final_xor(get_final_xor_value())
	{
		crcInit();
	}

	~CrcCal()
	{

	}

	CRC_TYPE getCrc(unsigned char const message[], int nBytes)
	{
		CRC_TYPE	   remainder = init_val;
		unsigned char  data;
		int            byte;

		/*
		* Divide the message by the polynomial, a byte at a time.
		*/
		for (byte = 0; byte < nBytes; ++byte)
		{
			data = (uint8_t)reflect_data<CRC_TYPE>(message[byte]) ^ (remainder >> (bit_width - 8));
			remainder = crc_table[data] ^ (remainder << 8);
		}

		/*
		* The final remainder is the CRC.
		*/
		return reflect_remainder<CRC_TYPE>(remainder);
	}

private:
	template<typename T>
	uint8_t reflect_data(unsigned char message)
	{
		return static_cast<uint8_t>(reflect(message, 8));
	}

	template<>
	uint8_t reflect_data<crc_ccitt_type>(unsigned char message)
	{
		return message;
	}

	template<typename T>
	CRC_TYPE reflect_remainder(unsigned long message)
	{
		return ((CRC_TYPE)reflect(message, bit_width)) ^ final_xor;
	}

	template<>
	CRC_TYPE reflect_remainder<crc_ccitt_type>(unsigned long message)
	{
		return message ^ final_xor;
	}

	static unsigned long reflect(unsigned long data, unsigned char nBits)
	{
		unsigned long  reflection = 0x00000000;
		unsigned char  bit;

		/*
		* Reflect the data about the center bit.
		*/
		for (bit = 0; bit < nBits; ++bit)
		{
			/*
			* If the LSB bit is set, set the reflection of it.
			*/
			if (data & 0x01)
			{
				reflection |= (1 << ((nBits - 1) - bit));
			}

			data = (data >> 1);
		}

		return (reflection);
	}

	/**
	 * @brief crc初始化
	 */
	void crcInit()
	{
		CRC_TYPE  ploynomial = get_ploynomial();
		CRC_TYPE    remainder;
		int			dividend;
		unsigned char  bit;

		/*
		* Compute the remainder of each possible dividend.
		*/
		for (dividend = 0; dividend < 256; ++dividend)
		{
			/*
			* Start with the dividend followed by zeros.
			*/
			remainder = dividend << (bit_width - 8);

			/*
			* Perform modulo-2 division, a bit at a time.
			*/
			for (bit = 8; bit > 0; --bit)
			{
				/*
				* Try to divide the current data bit.
				*/
				if (remainder & top_bit)
				{
					remainder = (remainder << 1) ^ ploynomial;
				}
				else
				{
					remainder = (remainder << 1);
				}
			}

			/*
			* Store the result into the table.
			*/
			crc_table[dividend] = remainder;
		}

	}

	/**
	 * @brief 初始值
	 * @return CRC_TYPE
	 */
	CRC_TYPE get_initial_reminder() const
	{
		if (typeid(CRC_TYPE) == typeid(crc_ccitt_type))
		{
			return (CRC_TYPE)0xFFFF;
		}
		else if (typeid(CRC_TYPE) == typeid(crc_16_type))
		{
			return (CRC_TYPE)0x0000;
		}
		else
		{
			return (CRC_TYPE)0xFFFFFFFF;
		}
	}

	/**
	 * @brief 结果异或值xor
	 * @return CRC_TYPE
	 */
	CRC_TYPE get_final_xor_value() const
	{
		if (typeid(CRC_TYPE) == typeid(crc_ccitt_type))
		{
			return (CRC_TYPE)0x0000;
		}
		else if (typeid(CRC_TYPE) == typeid(crc_16_type))
		{
			return (CRC_TYPE)0x0000;
		}
		else
		{
			return (CRC_TYPE)0xFFFFFFFF;
		}
	}

	/**
	 * @brief 获取多项式
	 * @return CRC_TYPE
	 */
	CRC_TYPE get_ploynomial() const
	{
		if (typeid(CRC_TYPE) == typeid(crc_ccitt_type))
		{
			return (CRC_TYPE)0x1021;
		}
		else if (typeid(CRC_TYPE) == typeid(crc_16_type))
		{
			return (CRC_TYPE)0x8005;
		}
		else
		{
			return (CRC_TYPE)0x04C11DB7;
		}
	}
private:
	const uint32_t bit_width; /* 宽度位数 */
	const uint32_t top_bit;
	CRC_TYPE  crc_table[256];
	const CRC_TYPE  init_val;
	const CRC_TYPE  final_xor;
};
