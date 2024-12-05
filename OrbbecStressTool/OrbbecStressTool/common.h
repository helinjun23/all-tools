#pragma once

// 3rd
#include "utility/crc_clac.h"
#include "utility/time_duration.hpp"
#include "utility/utility.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/async.h"
#include "spdlog/stopwatch.h"
#include "spdlog/fmt/ranges.h"
#include "spdlog/fmt/bin_to_hex.h"

// STL
#include <iostream>
#include <string>


// QT
#include <QSettings>
#include <QTimer>
#include <QTextStream>
#include <QStringList>
#include <QTextCodec>
#include <QButtonGroup>
#include <QObject>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QApplication>
#include <QFileDialog>
#include <QAtomicInteger>
#include <QThread>
#include <QMetaType>
#include <QCompleter>


// 设备串口类型
enum enDevComType {
	DEV_COM_TYP_CMMC = 0,  // 通信串口
	DEV_COM_TYP_DEBUG, // 调试串口
	DEV_COM_TYP_RELAY, // 继电器串口
};

// 异步执行耗时调用，暂不支持成员函数
template<typename Func, typename...Args>
void asyncproc(Func proc, Args...args)
{
	auto future = QtConcurrent::run(proc, args...);
	while (!future.isFinished())
	{
		QApplication::processEvents(QEventLoop::AllEvents, 100);
	}
}
