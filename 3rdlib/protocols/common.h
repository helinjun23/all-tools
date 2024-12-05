/**
 * @file common.h
 * @brief 协议公共类型
 * @author xuenuo (xuenuo@orbbec.com)
 * @version 1.0
 * @date 2023-09-11
 *
 * @copyright Copyright (c) ORBBEC Technology Group Co., Ltd. All rights reserved
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2023-09-11 <td>1.0     <td>wangh     <td>内容
 * </table>
 */
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <list>

enum class ProtocolResultTyp :uint8_t {
	NONE, // 成功
	TRANS_FAIL, // 数据发送、接收传输失败
	PARSE_FAIL, // 接收数据解析失败
};

/**
 * @brief 多参数识别结果
 */
typedef struct
{
	int valid_flg; //识别结果
	uint32_t face_id;//最高分人脸id
	float similarity;//最高分相似度
	float desensitized_similarity;
	unsigned int face_num;
}face_recongnize_res;

/* 抓拍参数*/
struct SnapParam {
	uint32_t snap_bauterate;
	uint32_t com_bauterate;
	bool ensnap_param; // 是否支持扩展参数
	uint8_t img_type;
	uint8_t gain_val;
};

static QString getPrintfInfo(const QByteArray& data, int start, int end)
{
	QString str = "";
	QString strTmp = "";

	if (data.length() > start)
	{
		for (int i = start; i < end; i++)
		{
			unsigned char ch = (unsigned char)data.at(i);
			strTmp.sprintf("%02X ", ch);
			str += strTmp;
		}
	}
	return str;
}

static QString getSendFaceCharError(int error)
{
	QString strError;
	switch (error)
	{
	case 0x00:
		strError = "调用注册，识别，删除接口失败";
		break;
	case 0x01:
		strError = "授权码不正确、过期或其他与license错误";
		break;
	case 0x02:
		strError = "SDK内部错误，请联系研发定位";
		break;
	case 0x03:
		strError = "识别模型或者人脸算法版本校验出错";
		break;
	case 0x04:
		strError = "传入参数出错";
		break;
	case 0x05:
		strError = "注册的人脸ID已占用";
		break;
	case 0x06:
		strError = "人脸库已满，请删除部分无用ID";
		break;
	case 0x07:
		strError = "人脸特征值无效";
		break;
	case 0x08:
		strError = "特征值类型出错";
		break;
	case 0x09:
		strError = "特征数组解密失败";
		break;
	case 0x0A:
		strError = "不支持录入操作";
		break;
	case 0x0F:
		strError = "其他错误";
		break;
	default:
		strError = "未知错误";
		break;
	}
	return strError;
}
